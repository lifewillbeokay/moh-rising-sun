"""Mutation checks for source inclusion, data ownership and progress accounting."""
import copy
import json
import struct
import sys
import tempfile
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / 'tools'))
from audit import load_target
from formats import Elf32, verify_load_image, cstring
from project_build import progress, target_section, validate_object, validate_units, verify_unit, write_context
from tool_runner import run


class SourceVerification(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.build = ROOT / 'build/reconstruction'
        if not (cls.build / 'report.json').exists():
            raise unittest.SkipTest('Run reconstruct.py first')
        cls.original_path, cls.original = load_target()
        cls.project = json.loads((ROOT / 'config/GR8E69/project.json').read_text())
        cls.units = [json.loads((ROOT / 'config/GR8E69' / n).read_text()) for n in cls.project['units']]
        cls.by_name = {u['id']: u for u in cls.units}

    def linked(self, name):
        return (self.build / 'units' / name / 'compiled.elf').read_bytes()

    def test_all_verified_sources_and_progress(self):
        for unit in self.units:
            verify_unit(self.original, Elf32(self.linked(unit['id'])), unit)
        result = progress(self.original, self.units, self.project)
        self.assertEqual(result['matching_code_bytes'], 618368)
        self.assertEqual(result['total_executable_code_bytes'], 2492032)
        self.assertGreaterEqual(result['percent'], 23)
        self.assertEqual(result['categories']['reconstructed_game'], 226152)
        self.assertEqual(result['categories']['restored_library'], 392216)

    def test_private_ppp_entry_point_requires_its_function_symbol(self):
        damaged = bytearray(self.linked('PPP'))
        offset = self.symbol_offset(damaged, 'ControlIn')
        # Keep the generated code intact, but erase the local function's type.
        damaged[offset + 12] = 0
        with self.assertRaisesRegex(ValueError, 'Linked source functions differ'):
            verify_unit(self.original, Elf32(damaged), self.by_name['PPP'])

    def test_sn_bss_records_require_size_name_and_file_identity(self):
        unit = self.by_name['newlib_vfprintf']
        for mutation in ('size', 'name', 'file_record'):
            with self.subTest(mutation=mutation):
                data = bytearray(self.linked(unit['id']))
                changed = copy.deepcopy(unit)
                if mutation == 'file_record':
                    changed['original_file_index'] = self.by_name['newlib_vfiprintf']['original_file_index']
                else:
                    elf = Elf32(data)
                    symbol = next(s for s in elf.symbols() if s['name'].startswith('pch.') and s['size'])
                    offset = self.symbol_offset(data, symbol['name'])
                    if mutation == 'size':
                        struct.pack_into('>I', data, offset + 8, symbol['size'] - 4)
                    else:
                        table = next(s for s in elf.sections if s['type'] == 2)
                        strings = elf.sections[table['link']]
                        name_offset = struct.unpack_from('>I', data, offset)[0]
                        data[strings['offset'] + name_offset] = ord('q')
                with self.assertRaisesRegex(ValueError, 'BSS'):
                    verify_unit(self.original, Elf32(data), changed)

    def test_readonly_subsection_cannot_be_remapped_to_code(self):
        unit = copy.deepcopy(self.by_name['lcode'])
        prefix = next(s for s in unit['sections'] if s['name'] == '.rodata.prefix')
        prefix['target_name'] = '.text'
        with self.assertRaisesRegex(ValueError, 'Unsupported source section mapping'):
            validate_units(self.original, [unit])

    def test_native_vtable_must_own_the_complete_original_object(self):
        for field, value in [('size', 180), ('address', '0x802ecb7c')]:
            with self.subTest(field=field):
                unit = copy.deepcopy(self.by_name['eagl_anim_delta_f1'])
                section = next(s for s in unit['sections'] if s['name'].startswith('.gnu.linkonce.d.'))
                section[field] = value
                with self.assertRaisesRegex(ValueError, 'Native vtable object ownership differs'):
                    validate_units(self.original, [unit])

    def test_native_vtable_linked_metadata_is_verified(self):
        unit = self.by_name['eagl_anim_delta_f1']
        for mutation in ('name', 'size', 'binding', 'address'):
            with self.subTest(mutation=mutation):
                data = bytearray(self.linked(unit['id']))
                offset = self.symbol_offset(data, '_vt.Q28EAGLAnim9FnDeltaF1')
                if mutation == 'name':
                    struct.pack_into('>I', data, offset, 0)
                elif mutation == 'size':
                    struct.pack_into('>I', data, offset + 8, 180)
                elif mutation == 'binding':
                    data[offset + 12] = 0x21  # Weak after linking, unlike the original.
                else:
                    struct.pack_into('>I', data, offset + 4, 0x802ecb7c)
                with self.assertRaisesRegex(ValueError, 'Native vtable object ownership differs'):
                    verify_unit(self.original, Elf32(data), unit)

    def symbol_offset(self, data, name):
        elf = Elf32(data)
        for section in elf.sections:
            if section['type'] != 2:
                continue
            strings = elf.contents(elf.sections[section['link']])
            for offset in range(section['offset'], section['offset'] + section['size'], 16):
                string_offset = struct.unpack_from('>I', data, offset)[0]
                if cstring(strings, string_offset) == name:
                    return offset
        self.fail(f'Symbol not found: {name}')

    def test_bss_object_size_change_rejected(self):
        damaged = bytearray(self.linked('OSThread'))
        offset = self.symbol_offset(damaged, 'RunQueue')
        size = struct.unpack_from('>I', damaged, offset + 8)[0]
        struct.pack_into('>I', damaged, offset + 8, size + 4)
        with self.assertRaisesRegex(ValueError, 'BSS object ownership differs'):
            verify_unit(self.original, Elf32(damaged), self.by_name['OSThread'])

    def test_small_data_anchor_value_change_rejected(self):
        for unit, symbol in [('GXBump', '__GXData'), ('newlib_vprintf', '_impure_ptr')]:
            with self.subTest(unit=unit):
                damaged = bytearray(self.linked(unit))
                offset = self.symbol_offset(damaged, symbol)
                value = struct.unpack_from('>I', damaged, offset + 4)[0]
                struct.pack_into('>I', damaged, offset + 4, value + 4)
                with self.assertRaisesRegex(ValueError, 'Resolved small-data anchor differs'):
                    verify_unit(self.original, Elf32(damaged), self.by_name[unit])

    def test_global_function_cannot_be_discarded_as_local(self):
        unit = copy.deepcopy(self.by_name['EXIBios'])
        damaged = bytearray((self.build / 'units/EXIBios/compiled.o').read_bytes())
        offset = self.symbol_offset(damaged, 'CompleteTransfer')
        damaged[offset + 12] = (1 << 4) | 2  # Change STB_LOCAL to STB_GLOBAL.
        with self.assertRaisesRegex(ValueError, 'Discarded function is not local'):
            validate_object(Elf32(damaged), unit)

    def test_compiled_instruction_change_rejected(self):
        damaged = bytearray(self.linked('MathFun'))
        text = next(s for s in Elf32(damaged).sections if s['name'] == '.text')
        damaged[text['offset']] ^= 1
        with self.assertRaisesRegex(ValueError, 'Compiled source bytes differ'):
            verify_unit(self.original, Elf32(damaged), self.by_name['MathFun'])

    def test_compiled_constant_change_rejected(self):
        damaged = bytearray(self.linked('lvm'))
        data = next(s for s in Elf32(damaged).sections if s['name'] == '.rodata')
        damaged[data['offset']] ^= 1
        with self.assertRaisesRegex(ValueError, 'Compiled source bytes differ: lvm .rodata'):
            verify_unit(self.original, Elf32(damaged), self.by_name['lvm'])

    def test_extra_allocated_output_rejected(self):
        damaged = bytearray(self.linked('MathFun'))
        section = next(s for s in Elf32(damaged).sections if s['name'] == '.strtab')
        shoff = struct.unpack_from('>I', damaged, 32)[0]
        struct.pack_into('>I', damaged, shoff + section['index'] * 40 + 8, 2)
        with self.assertRaisesRegex(ValueError, 'Linked source sections differ'):
            verify_unit(self.original, Elf32(damaged), self.by_name['MathFun'])

    def test_wrong_external_rejected(self):
        units = copy.deepcopy(self.units)
        units[0]['externals']['rand'] = units[0]['sections'][0]['address']
        with self.assertRaisesRegex(ValueError, 'External is not an original definition'):
            validate_units(self.original, units)

    def test_private_context_dependency_scope_rejected(self):
        unit = self.by_name['OSInterrupt']
        validate_units(self.original, [unit])
        for change in ('file', 'binding', 'address', 'missing_scope'):
            with self.subTest(change=change):
                damaged = copy.deepcopy(unit)
                if change == 'file':
                    damaged['original_file'] = 'OSError.c'
                elif change == 'binding':
                    damaged['local_externals'] = []
                elif change == 'address':
                    value = int(damaged['externals']['ExternalInterruptHandler'], 16)
                    damaged['externals']['ExternalInterruptHandler'] = hex(value + 4)
                else:
                    del damaged['original_file']
                with self.assertRaises(ValueError):
                    validate_units(self.original, [damaged])

    def test_partial_function_range_rejected(self):
        units = copy.deepcopy(self.units)
        units[0]['sections'][0]['size'] -= 4
        with self.assertRaisesRegex(ValueError, 'excludes part of a function'):
            validate_units(self.original, units)

    def test_duplicate_source_credit_rejected(self):
        units = copy.deepcopy(self.units)
        duplicate = copy.deepcopy(units[0])
        duplicate['id'] = 'duplicate'
        units.append(duplicate)
        with self.assertRaisesRegex(ValueError, 'Source ranges overlap'):
            progress(self.original, units, self.project)

    def test_smaller_denominator_rejected(self):
        project = copy.deepcopy(self.project)
        project['progress']['code_bytes'] -= project['progress']['executable_sections'].pop('.init')
        with self.assertRaisesRegex(ValueError, 'denominator differs'):
            progress(self.original, self.units, project)

    def test_absent_dependency_cannot_enter_retained_code(self):
        damaged = bytearray((self.build / 'units/ldebug/compiled.o').read_bytes())
        obj = Elf32(damaged)
        symbols = list(obj.symbols())
        changed = False
        for section in obj.sections:
            if section['type'] != 4:
                continue
            for offset in range(0, section['size'], 12):
                location = section['offset'] + offset
                _, info, _ = struct.unpack_from('>IIi', damaged, location)
                if symbols[info >> 8]['name'] == 'luaA_pushobject':
                    retained = next(s for s in symbols if s['name'] == 'lua_getinfo' and s['type'] == 2)
                    struct.pack_into('>I', damaged, location, retained['address'])
                    changed = True
        self.assertTrue(changed)
        with self.assertRaisesRegex(ValueError, 'Retained code or data references an absent'):
            validate_object(Elf32(damaged), self.by_name['ldebug'])

    def test_context_really_includes_generated_code_and_data(self):
        # A bad source result must not be hidden by retaining original bytes.
        blobs = json.loads((self.build / 'report.json').read_text())['source_blobs']
        for unit, section in [('lvm', '.text'), ('lvm', '.rodata'),
                              ('lcode', '.rodata'), ('lcode', '.rodata.prefix')]:
            with self.subTest(unit=unit, section=section), tempfile.TemporaryDirectory() as temporary:
                directory = Path(temporary)
                sources = copy.deepcopy(blobs)
                blob = next(b for b in sources if b['unit'] == unit
                            and b.get('source_section', b['section']) == section)
                damaged = bytearray(Path(blob['path']).read_bytes())
                damaged[0] ^= 1
                target = directory / 'damaged.bin'
                target.write_bytes(damaged)
                blob['path'] = str(target)
                write_context(self.original_path, self.original, sources, directory)
                tools = ROOT / 'build/tools'
                commands = [
                    [tools / 'powerpc-eabi-as', 'context.s', '-o', 'context.o'],
                    [tools / 'powerpc-eabi-ld', '-T', 'context.ld', '-o', 'damaged.elf', 'context.o'],
                    [tools / 'dtk', 'elf2dol', 'damaged.elf', 'damaged.dol'],
                ]
                for i, command in enumerate(commands):
                    run(command, directory, directory / f'{i}.log')
                with self.assertRaisesRegex(ValueError, 'Loaded bytes changed'):
                    verify_load_image(self.original, (directory / 'damaged.dol').read_bytes())


class SectionMappingPolicy(unittest.TestCase):
    def test_only_named_native_vtables_can_map_to_data(self):
        section = dict(name='.gnu.linkonce.d._vt.Q28EAGLAnim9FnDeltaF1', target_name='.data')
        self.assertEqual(target_section(section), '.data')
        for changes in [dict(target_name='.text'), dict(target_name='.bss'), dict(type=8),
                        dict(name='.gnu.linkonce.d.arbitrary'),
                        dict(name='.gnu.linkonce.t._vt.Q28EAGLAnim9FnDeltaF1')]:
            with self.subTest(changes=changes), self.assertRaisesRegex(ValueError, 'Unsupported source section mapping'):
                target_section(dict(section, **changes))


if __name__ == '__main__':
    unittest.main()
