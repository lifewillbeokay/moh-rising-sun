"""Native comparison safeguards. Original-dependent cases skip on public CI."""
import copy
import json
import struct
import sys
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / 'tools'))
from comparisons import inspect_comparison, load_comparisons, objdiff_scores, validate_inventory
from formats import Elf32
from project_build import validate_units, verify_unit


class NativeComparisons(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        if not (ROOT / 'build/comparisons/report.json').exists():
            raise unittest.SkipTest('Run comparisons.py first')
        from audit import load_target
        _, cls.original = load_target()
        cls.units = [u for u in load_comparisons(ROOT)
                     if u["id"] in ("compare_FnDeltaQFast", "compare_FnDeltaQ")]
        cls.unit = cls.units[0]
        cls.work = ROOT / 'build/comparisons' / cls.unit['id']
        cls.obj = Elf32((cls.work / 'compiled.o').read_bytes())
        cls.linked_data = (cls.work / 'compiled.elf').read_bytes()
        config = ROOT / 'config/GR8E69'
        cls.accepted = [json.loads((config/n).read_text())
                        for n in json.loads((config/'project.json').read_text())['units']]

    def test_registered_full_outputs_and_exact_context(self):
        validate_inventory(self.units, self.accepted)
        partials = []
        for unit in self.units:
            validate_units(self.original, [unit])
            work = ROOT / 'build/comparisons' / unit['id']
            records = inspect_comparison(self.original, Elf32((work/'compiled.o').read_bytes()),
                                         Elf32((work/'compiled.elf').read_bytes()), unit)
            partials += [f for f in records if f['status'] == 'partial']
        self.assertEqual(len(partials), 4)
        self.assertEqual(sum(f['size'] for f in partials), 15228)
        self.assertEqual(sorted(f['differing_bytes'] for f in partials), [3, 3, 15, 15])

    def test_partial_unit_still_fails_exact_build_verification(self):
        with self.assertRaisesRegex(ValueError, 'Compiled source bytes differ'):
            verify_unit(self.original, Elf32(self.linked_data), self.unit)

    def test_context_may_not_be_reclassified_as_new_source(self):
        unit = copy.deepcopy(self.unit)
        unit['comparison_functions'].append(unit['functions'][0]['name'])
        with self.assertRaisesRegex(ValueError, 'Overlapping'):
            validate_inventory([unit], self.accepted)

    def test_undeclared_shared_storage_is_rejected(self):
        unit = copy.deepcopy(self.unit)
        unit['context_units'] = []
        with self.assertRaisesRegex(ValueError, 'context'):
            validate_inventory([unit], self.accepted)

    def test_partial_does_not_allow_context_or_data_drift(self):
        for name, expected in (('.text', 'context differs'), ('.rodata', 'data must match')):
            with self.subTest(section=name):
                data = bytearray(self.linked_data)
                section = next(s for s in Elf32(data).sections if s['name'] == name)
                data[section['offset']] ^= 1
                with self.assertRaisesRegex(ValueError, expected):
                    inspect_comparison(self.original, self.obj, Elf32(data), self.unit)

    def test_unaccounted_linked_code_is_rejected(self):
        data = bytearray(self.linked_data)
        elf = Elf32(data)
        section = next(s for s in elf.sections if s['name'] == '.text')
        shoff = struct.unpack_from('>I', data, 32)[0]
        struct.pack_into('>I', data, shoff + section['index']*40 + 20, section['size']+4)
        with self.assertRaisesRegex(ValueError, 'Unaccounted linked'):
            inspect_comparison(self.original, self.obj, Elf32(data), self.unit)


class ScoreValidation(unittest.TestCase):
    def test_valid_target_weighted_measurement(self):
        data = {'left': {'symbols': [{'name': 'f', 'size': '4', 'match_percent': 75}]}}
        self.assertEqual(objdiff_scores(data, [{'name': 'f', 'size': 4}]), {'f': 75})
        with self.assertRaisesRegex(ValueError, 'size differs'):
            objdiff_scores(data, [{'name': 'f', 'size': 8}])

    def test_missing_duplicate_or_invalid_measurement_is_rejected(self):
        function = {'name': 'f', 'size': 4}
        for symbols in ([], [{'name':'f','size':'4','match_percent':101}],
                        [{'name':'f','size':'4','match_percent':float('nan')}],
                        [{'name':'f','size':'4','match_percent':90}]*2):
            with self.subTest(symbols=symbols), self.assertRaises(ValueError):
                objdiff_scores({'left': {'symbols': symbols}}, [function])


if __name__ == '__main__':
    unittest.main()
