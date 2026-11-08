#!/usr/bin/env python3
"""Measure reviewed partial source without changing the verified image.

Complete native output and dependencies are accounted for. Data must still match;
code may differ in bytes or size. Only complete exact functions earn exact credit.
"""
import argparse
import copy
import hashlib
import json
import math
import re
import shutil

from setup import ROOT, fetch, platform_spec, setup
from tool_runner import run, sha256

PROFILE_KEYS = ("compiler_family", "compiler_version", "compiler_flags", "include_dirs")


def read(path):
    return json.loads(path.read_text())


def load_comparisons(root):
    config = root / "config/GR8E69"
    registry = read(config / "comparisons.json")
    if registry["schema_version"] != 1 or len(set(registry["units"])) != len(registry["units"]):
        raise ValueError("Invalid comparison registry")
    project = read(config / "project.json")
    units = []
    for name in registry["units"]:
        if not (config / name).resolve().is_relative_to(config.resolve()):
            raise ValueError("Comparison manifest is outside target configuration")
        unit = read(config / name)
        source = root / unit["source"]
        if not source.is_file() or not source.resolve().is_relative_to((root/"src").resolve()):
            raise ValueError("Comparison source must be maintained under src/")
        if any(k in unit for k in PROFILE_KEYS) or unit["profile_from"] not in project["units"]:
            raise ValueError("Comparison must reuse an accepted unit profile")
        profile = read(config / unit["profile_from"])
        unit.update({k: profile[k] for k in PROFILE_KEYS if k in profile})
        if unit.get("compiler_family", "ProDG") != "ProDG":
            raise ValueError("Comparison mode currently supports ProDG profiles")
        if not re.fullmatch(r"[A-Za-z0-9_]+", unit["id"]) or not unit.get("evidence"):
            raise ValueError("Comparison identifier/evidence is missing")
        units.append(unit)
    return units


def interval(item):
    return int(item["address"], 16), int(item["address"], 16) + item["size"]


def overlaps(a, b):
    return a[0] < b[1] and b[0] < a[1]


def validate_inventory(units, accepted):
    """Allow explicit exact context, never duplicate measured coverage."""
    accepted_by_id = {u["id"]: u for u in accepted}
    seen = []
    if len({u["id"] for u in units}) != len(units) or any(u["id"] in accepted_by_id for u in units):
        raise ValueError("Duplicate comparison identifier")
    for unit in units:
        if unit["category"] not in ("reconstructed_game", "restored_library"):
            raise ValueError("Unknown comparison category")
        names = [f["name"] for f in unit["functions"]]
        selected = unit["comparison_functions"]
        if (not selected or len(set(selected)) != len(selected) or not set(selected) <= set(names)
                or len(set(names)) != len(names)):
            raise ValueError("Invalid comparison function inventory")
        contexts = unit["context_units"]
        if len(set(contexts)) != len(contexts) or not set(contexts) <= set(accepted_by_id):
            raise ValueError("Unknown/duplicate comparison context")
        for context in contexts:
            other = accepted_by_id[context]
            if any(unit.get(k, "ProDG" if k == "compiler_family" else None) !=
                   other.get(k, "ProDG" if k == "compiler_family" else None) for k in PROFILE_KEYS):
                raise ValueError("Comparison context compiler profile differs")
        for f in unit["functions"]:
            owners = [a for a in accepted for af in a["functions"] if overlaps(interval(f), interval(af))]
            if f["name"] in selected:
                if owners or any(overlaps(interval(f), span) for span in seen):
                    raise ValueError("Overlapping comparison coverage")
                seen.append(interval(f))
            elif len(owners) != 1 or owners[0]["id"] not in contexts or f not in owners[0]["functions"]:
                raise ValueError("Unmeasured function is not declared exact context")
        for a in accepted:
            for section in a.get("sections", []):
                touching = [s for s in unit["sections"] if overlaps(interval(s), interval(section))]
                if touching and (a["id"] not in contexts or len(touching) != 1 or
                                 not interval(touching[0])[0] <= interval(section)[0] <
                                 interval(section)[1] <= interval(touching[0])[1]):
                    raise ValueError("Comparison storage overlaps undeclared context")
        # Different comparisons must not silently share any native storage.
        for other in units[:units.index(unit)]:
            if any(overlaps(interval(s), interval(t)) for s in unit["sections"] for t in other["sections"]):
                raise ValueError("Comparison units overlap")
    return sorted(seen)


def setup_objdiff(root):
    spec = read(root / "tools/toolchain.json")["objdiff"]
    binary = root / "build/tools/objdiff-cli"
    fetch(platform_spec(spec), binary)
    binary.chmod(0o755)
    return binary, spec


def function_bytes(elf, function):
    section = next(s for s in elf.sections if s["flags"] & 4 and
                   s["address"] <= int(function["address"], 16) <
                   int(function["address"], 16) + function["size"] <= s["address"] + s["size"])
    offset = int(function["address"], 16) - section["address"]
    return elf.contents(section)[offset:offset + function["size"]]


def write_target(original, unit, work, tools):
    """Wrap complete unchanged target ranges with their original function symbols.

    objdiff cannot read this game's full ELF (_SDA_BASE_ has an out-of-range
    synthetic size). This comparison-only wrapper carries no patched bytes.
    """
    from project_build import target_section, verify_unit
    from formats import Elf32
    assembly, script = [], ["SECTIONS {"]
    for i, section in enumerate(unit["sections"]):
        original_section = next(s for s in original.sections if s["name"] == target_section(section))
        if original_section["type"] != 1:
            raise ValueError("Comparison BSS is not supported")
        offset = int(section["address"], 16) - original_section["address"]
        (work / f"target-{i}.bin").write_bytes(original.contents(original_section)[offset:offset + section["size"]])
        flags = 'a' + ('w' if original_section['flags'] & 1 else '') + ('x' if original_section['flags'] & 4 else '')
        assembly += [f'.section {section["name"]},"{flags}",@progbits', f'.incbin "target-{i}.bin"']
        script.append(f'{section["name"]} {section["address"]} : {{ *({section["name"]}) }}')
        if section["name"].startswith('.gnu.linkonce.d.'):
            name = section['name'].removeprefix('.gnu.linkonce.d.')
            assembly += [f'.global {name}', f'.type {name},@object',
                         f'.set {name},{section["name"]}', f'.size {name},{section["size"]}']
    text = next(s for s in unit["sections"] if s["name"] == '.text')
    for f in unit["functions"]:
        name = f["name"]
        assembly += [f'.{"global" if f["binding"] == 1 else "weak" if f["binding"] == 2 else "local"} {name}',
                     f'.type {name},@function',
                     f'.set {name},.text+{int(f["address"], 16)-int(text["address"], 16)}',
                     f'.size {name},{f["size"]}']
    (work / 'target.s').write_text('\n'.join(assembly)+'\n')
    (work / 'target.ld').write_text('\n'.join(script+['}'])+'\n')
    run([tools/'powerpc-eabi-as', 'target.s', '-o', 'target.o'], work, work/'target-assemble.log')
    run([tools/'powerpc-eabi-ld', '-T', 'target.ld', 'target.o', '-o', 'target.elf'], work, work/'target-link.log')
    # Independently recheck every wrapper byte, range, symbol and data object.
    verify_unit(original, Elf32((work/'target.elf').read_bytes()), unit)


def write_diff_object(linked, work, tools):
    """Rewrap unchanged linked sections at zero for objdiff's relocatable reader.

    No byte, instruction, relocation or function is omitted from native output.
    This object is diagnostic only and is never input to the verified image.
    """
    from formats import Elf32
    from project_build import allocated
    assembly = []
    for i, section in enumerate(allocated(linked)):
        (work/f'diff-{i}.bin').write_bytes(linked.contents(section))
        flags = 'a' + ('w' if section['flags'] & 1 else '') + ('x' if section['flags'] & 4 else '')
        assembly += [f'.section {section["name"]},"{flags}",@progbits', f'.incbin "diff-{i}.bin"']
        for f in linked.symbols():
            if f['section'] == section['index'] and f['type'] in (1, 2) and f['size']:
                name = f['name']
                assembly += [f'.{"global" if f["binding"] == 1 else "weak" if f["binding"] == 2 else "local"} {name}',
                             f'.type {name},@{"function" if f["type"] == 2 else "object"}',
                             f'.set {name},{section["name"]}+{f["address"]-section["address"]}',
                             f'.size {name},{f["size"]}']
    (work/'diff-base.s').write_text('\n'.join(assembly)+'\n')
    run([tools/'powerpc-eabi-as', 'diff-base.s', '-o', 'diff-base.o'], work, work/'diff-assemble.log')
    obj = Elf32((work/'diff-base.o').read_bytes())
    for section in allocated(linked):
        actual = next(s for s in allocated(obj) if s['name'] == section['name'])
        if obj.contents(actual) != linked.contents(section):
            raise ValueError("Diff wrapper changed native bytes")
        before = sorted((f['name'], f['address']-section['address'], f['size'], f['type'], f['binding'])
                        for f in linked.symbols() if f['section'] == section['index'] and f['type'] in (1, 2) and f['size'])
        after = sorted((f['name'], f['address'], f['size'], f['type'], f['binding'])
                       for f in obj.symbols() if f['section'] == actual['index'] and f['type'] in (1, 2) and f['size'])
        if before != after:
            raise ValueError("Diff wrapper changed native symbols")


def inspect_comparison(original, obj, linked, unit):
    from project_build import validate_object, inspect_unit, function_records
    validate_object(obj, unit)
    actual = function_records(linked)
    if sorted((n, b) for n, _, _, b in actual) != sorted((f['name'], f['binding']) for f in unit['functions']):
        raise ValueError("Comparison linked function inventory differs")
    native = copy.deepcopy(unit)
    native['functions'] = [dict(name=n, address=hex(a), size=s, binding=b) for n, a, s, b in actual]
    text = next(s for s in linked.sections if s['name'] == '.text')
    cursor = text['address']
    for f in sorted(native['functions'], key=lambda f: int(f['address'], 16)):
        if int(f['address'], 16) != cursor or f['size'] <= 0:
            raise ValueError("Unaccounted linked comparison code")
        cursor += f['size']
    if cursor != text['address'] + text['size']:
        raise ValueError("Unaccounted linked comparison text")
    next(s for s in native['sections'] if s['name'] == '.text')['size'] = text['size']
    blobs = inspect_unit(original, linked, native)
    for blob in blobs:
        if blob['section'] != '.text':
            target = next(s for s in original.sections if s['name'] == blob['section'])
            offset = blob['address'] - target['address']
            if blob['type'] != 1 or blob['data'] != original.contents(target)[offset:offset+blob['size']]:
                raise ValueError("Comparison data must match completely")
    result = []
    for f in unit['functions']:
        actual_f = next(x for x in native['functions'] if x['name'] == f['name'])
        a, b = function_bytes(original, f), function_bytes(linked, actual_f)
        exact = a == b and f == actual_f
        context = f['name'] not in unit['comparison_functions']
        if context and not exact:
            raise ValueError("Previously linked comparison context differs")
        result.append(dict(f, compiled_address=actual_f['address'], compiled_size=len(b),
                           target_sha256=hashlib.sha256(a).hexdigest(),
                           compiled_sha256=hashlib.sha256(b).hexdigest(),
                           differing_bytes=sum(x != y for x, y in zip(a, b)) + abs(len(a)-len(b)),
                           status='linked_context' if context else 'exact_unlinked' if exact else 'partial'))
    return result


def objdiff_scores(data, functions):
    # Filled from objdiff's target-side whole-symbol measurements, not a byte ratio.
    symbols = data['left']['symbols']
    results = {}
    for f in functions:
        found = [s for s in symbols if s['name'] == f['name']]
        if len(found) != 1:
            raise ValueError("Missing/ambiguous objdiff function")
        score = found[0]['match_percent']
        if int(found[0].get('size', 0)) != f['size']:
            raise ValueError("Objdiff function size differs")
        if not isinstance(score, (int, float)) or not math.isfinite(score) or not 0 <= score <= 100:
            raise ValueError("Invalid objdiff score")
        results[f['name']] = score
    return results


def measure(root=ROOT):
    from audit import load_target
    from formats import Elf32
    from project_build import compile_unit, validate_units
    from setup_compiler import setup_compiler
    from progress_report import fingerprints
    original_path, original = load_target()
    config = root/'config/GR8E69'
    units = load_comparisons(root)
    accepted = [read(config/n) for n in read(config/'project.json')['units']]
    validate_inventory(units, accepted)
    directory = root/'build/comparisons'
    if directory.exists():
        shutil.rmtree(directory)
    directory.mkdir(parents=True)
    inputs = fingerprints(root)
    tools = setup()
    objdiff, spec = setup_objdiff(root)
    keys = {('ProDG', '3.8.1')} | {(u.get('compiler_family','ProDG'), u['compiler_version']) for u in units}
    compilers = {f'{family}/{version}': setup_compiler(version, family) for family, version in sorted(keys)}
    records = []
    for unit in units:
        validate_units(original, [unit])
        work = directory/unit['id']; work.mkdir()
        compile_unit(unit, original, tools, compilers, work, comparison=True)
        functions = inspect_comparison(original, Elf32((work/'compiled.o').read_bytes()),
                                       Elf32((work/'compiled.elf').read_bytes()), unit)
        write_target(original, unit, work, tools)
        write_diff_object(Elf32((work/'compiled.elf').read_bytes()), work, tools)
        run([objdiff, 'diff', '-1', 'target.o', '-2', 'diff-base.o', '-o', 'diff.json'], work, work/'diff.log')
        scores = objdiff_scores(read(work/'diff.json'), functions)
        for f in functions:
            f['objdiff_percent'] = scores[f['name']]
            # objdiff can ignore differences. Never advertise 100 for nonexact bytes/layout.
            f['fuzzy_match_percent'] = min(scores[f['name']], 99.99) if f['status'] == 'partial' else 100
        records.append(dict(id=unit['id'], functions=functions,
                            object_sha256=sha256(work/'compiled.o'), linked_sha256=sha256(work/'compiled.elf'),
                            target_object_sha256=sha256(work/'target.o'), diff_object_sha256=sha256(work/'diff-base.o'), diff_sha256=sha256(work/'diff.json')))
        for f in functions:
            if f['status'] != 'linked_context':
                print(f"{f['name']}: {f['status']}, objdiff {f['fuzzy_match_percent']:.6f}%, {f['differing_bytes']} differing bytes", flush=True)
    if fingerprints(root) != inputs:
        raise ValueError("Inputs changed during comparison")
    report = dict(schema_version=1, original_elf_sha256=sha256(original_path), input_sha256=inputs,
                  objdiff_version=spec['version'], objdiff_sha256=sha256(objdiff),
                  compiler_executables={key: {p.name:sha256(p) for p in compiler.glob('*.exe')}
                                        for key,(compiler,_) in compilers.items()}, units=records)
    (directory/'report.json').write_text(json.dumps(report, indent=2)+'\n')
    return report


def validate_receipt(receipt, root, accepted, original_hash):
    """Public receipt checks; local capture separately reproduces native comparison."""
    from progress_report import require_fresh
    require_fresh(receipt, root)
    units = load_comparisons(root)
    validate_inventory(units, accepted)
    spec = read(root/'tools/toolchain.json')['objdiff']
    if (receipt['schema_version'] != 1 or receipt['original_elf_sha256'] != original_hash or
            receipt['objdiff_version'] != spec['version'] or
            receipt['objdiff_sha256'] not in {p['sha256'] for p in spec['platforms'].values()}):
        raise ValueError("Comparison provenance differs")
    records = receipt['units']
    if len(records) != len(units) or {u['id'] for u in records} != {u['id'] for u in units}:
        raise ValueError("Comparison receipt inventory differs")
    selected = []
    for unit in units:
        record = next(r for r in records if r['id'] == unit['id'])
        for key in ('object_sha256', 'linked_sha256', 'target_object_sha256', 'diff_object_sha256', 'diff_sha256'):
            if not re.fullmatch('[0-9a-f]{64}', record[key]):
                raise ValueError("Invalid comparison artifact hash")
        fs = record['functions']
        if len(fs) != len(unit['functions']) or {f['name'] for f in fs} != {f['name'] for f in unit['functions']}:
            raise ValueError("Comparison receipt function inventory differs")
        for f, expected in zip(fs, unit['functions']):
            if {k:f[k] for k in ('name','address','size','binding')} != expected:
                raise ValueError("Comparison function extent differs")
            equal = (f['target_sha256'] == f['compiled_sha256'] and f['size'] == f['compiled_size']
                     and f['address'] == f['compiled_address'])
            context = f['name'] not in unit['comparison_functions']
            status = 'linked_context' if context else 'exact_unlinked' if equal else 'partial'
            score = f['objdiff_percent']
            if (not isinstance(score, (int,float)) or not math.isfinite(score) or not 0 <= score <= 100 or
                    type(f['compiled_size']) != int or f['compiled_size'] <= 0 or
                    not re.fullmatch('0x[0-9a-f]+', f['compiled_address']) or
                    f['status'] != status or (context and not equal) or
                    f['fuzzy_match_percent'] != (min(score,99.99) if status == 'partial' else 100) or
                    type(f['differing_bytes']) != int or f['differing_bytes'] < 0 or
                    f['differing_bytes'] > max(f['size'], f['compiled_size']) or
                    (f['target_sha256'] == f['compiled_sha256']) != (f['differing_bytes'] == 0) or
                    (f['target_sha256'] == f['compiled_sha256'] and f['size'] != f['compiled_size'])):
                raise ValueError("Invalid comparison measurement/status")
            for key in ('target_sha256','compiled_sha256'):
                if not re.fullmatch('[0-9a-f]{64}', f[key]):
                    raise ValueError("Invalid comparison function hash")
            if not context:
                selected.append(dict(f, unit=unit['id'], source=unit['source'], category=unit['category']))
    return selected


if __name__ == '__main__':
    argparse.ArgumentParser(description=__doc__).parse_args()
    measure()
