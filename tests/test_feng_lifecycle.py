"""Original/native PPC checks for frontend input, lifecycle and reader sources.

Requires the pinned original, built units/comparisons and optional Unicorn.
External callbacks are stubbed; these are isolated behavior checks, not a game
runtime test or proof of complete equivalence. Memory and callback order compare
exactly; floating results use the shared six-decimal normalization.
"""
import itertools
import math
import random
import struct
import unittest
from pathlib import Path
import test_explosion_grenade as ppc
from test_shortlist_partials import Run
from test_feng_packages import add_list_stub
from test_feng_animation_lists import copy_stubs, allocation_stubs

ROOT = Path(__file__).resolve().parents[1]
EXACT = {'fengine_push_package', 'fetype_startup'}
OBJ, DATA, EXTRA, OUT, GAME, VT = [0x10020000 + i * 0x10000 for i in range(6)]


def native_directory(unit):
    return (ROOT / 'build/reconstruction/units' / unit if unit in EXACT else
            ROOT / 'build/comparisons' / ('compare_' + unit))


def snap(r, address, size):
    return bytes(r.u.mem_read(address, size))


def traces(r, names):
    for name, count in names:
        r.at(name, lambda n=name, c=count: r.tracecall(n, *[r.rr(i) for i in range(3, 3+c)]))


def object_update(r, case):
    running, old, duration, delta, mode, events, following, kind, flags, hold = case
    r.wr(OBJ + 0x14, running)
    r.wr(OBJ + 0x30, GAME, delta)
    r.wr(DATA + 0x18, kind, flags)
    r.wr(DATA + 0x54, EXTRA)
    r.wr(EXTRA + 0xc, duration, old, mode, OUT if following else 0)
    r.wr(EXTRA + 0x24, events)
    r.wr(OUT + 0xc, 12, 3, 0, 0)
    r.wr(OUT + 0x24, events)
    r.wr(ppc.symbols['_9FEPackage.uHoldDirtyFlags']['address'], hold)
    traces(r, [('UpdateObjectTracks__9FEPackageP8FEObjectP8FEScript', 3),
               ('IssueScriptMessages__9FEPackageP7FEngineP8FEObjectP8FEScriptll', 6),
               ('SetupMoveToTracks__8FEObject', 1), ('UpdateGroup__9FEPackageP7FEGroupl', 3)])
    def change():
        r.tracecall('change', r.rr(4))
        r.wr(r.rr(3) + 0x54, r.rr(4))
    r.at('SetCurrentScript__8FEObjectP8FEScript', change)
    for n in ['Update__9FEListBoxf', 'Update__13FECodeListBoxf']:
        r.at(n, lambda n=n: r.tracecall(n, r.rr(3), r.fr(1)))
    r.call('UpdateObject__9FEPackageP8FEObjectl', OBJ, DATA, 37)
    return snap(r, DATA, 0x60), snap(r, EXTRA, 0x30), snap(r, OUT, 0x30), r.trace


def engine_update(r, case):
    running, input_on, mouse, elapsed, stop, queue_again, hold = case
    r.wr(OBJ + 4, running, input_on, mouse)
    r.wr(OBJ + 0x18, OUT)
    r.wr(OBJ + 0xe8, 2)
    r.u.mem_write(OBJ + 0xec, struct.pack('>H', 1))
    r.wr(OBJ + 0xf4, DATA)
    r.wr(DATA, EXTRA); r.wr(EXTRA, 0)
    r.wr(DATA + 0xc, DATA + 0x400); r.cstring(DATA + 0x400, 'menu')
    r.wr(EXTRA + 0xc, EXTRA + 0x400); r.cstring(EXTRA + 0x400, 'overlay')
    r.wr(OBJ + 0x120, GAME); r.wr(GAME, VT)
    r.wr(OBJ + 0x5264, hold, 1)
    for off in [0xc8, 0xd0]:
        r.slot(VT, off, lambda off=off: r.tracecall('debug', off))
    r.slot(VT, 0x98, lambda: r.tracecall('mouse_info'))
    r.slot(VT, 0x90, lambda: (r.tracecall('pad_mask', r.rr(4)), 0x123)[1])
    traces(r, [('Update__8FEJoyPadUlUl', 3), ('DecrementHold__8FEJoyPadUlUl', 3),
               ('StartCheckMovieDemoScreen__7FEngine', 1), ('ProcessMouseForPackage__7FEngineP9FEPackage', 2)])
    r.at('Update__7FEMouseR11FEMouseInfoUl', lambda: r.tracecall('mouse_update', r.rr(3), r.rr(5)))
    r.at('CheckMovieDemoScreen__7FEnginePc', lambda: r.tracecall('movie', r.text(r.rr(4))))
    def pads():
        r.tracecall('pads', r.rr(4))
        r.wr(OBJ + 0x48, 5); r.wr(OBJ + 0x4c, 120, 0, 320)
        if stop: r.wr(OBJ + 4, 0)
    r.at('ProcessPadsForPackage__7FEngineP9FEPackage', pads)
    r.at('Update__9FEPackageP7FEnginel', lambda: r.tracecall('package', r.rr(3), r.rr(5), r.rd(ppc.symbols['_9FEPackage.uHoldDirtyFlags']['address'])))
    def commands():
        r.tracecall('commands'); r.wr(OBJ + 0x412c, DATA if queue_again else 0)
    r.at('ProcessPackageCommands__7FEngine', commands)
    r.at('ProcessMessageQueue__7FEngine', lambda: (r.tracecall('messages'), r.wr(OBJ + 0x412c, 0))[0])
    r.call('Update__7FEnginel', OBJ, elapsed)
    return snap(r, OBJ, 0x526c), r.trace


def responses(r, case):
    code, value, found, remembered, mask = case
    r.wr(DATA + 0x10, 2, EXTRA)
    r.wr(EXTRA, code, value, -4 if value == 0 else 42)
    r.wr(EXTRA + 12, 2, 99, 0)
    r.wr(OUT + 0x54, OUT + 0x100); r.wr(OUT + 0x100 + 0x30, 7)
    r.wr(GAME + 0x10, 0x1234); r.wr(GAME + 0x28, 5)
    r.wr(GAME + 0x8c, OUT if found else 0); r.wr(OUT + 0xc, 0xabc)
    r.wr(GAME + 0xc, VT); r.cstring(VT, 'checkpoint')
    r.at('FindScript__C8FEObjectUl', lambda: (r.tracecall('script', r.rr(4)), OUT + 0x100 if found else 0)[1])
    r.at('FindObjectByGUID__9FEPackageUl', lambda: (r.tracecall('guid', r.rr(4)), OUT if found else 0)[1])
    r.at('RecallLastPackageButton__7FEngineUl', lambda: (r.tracecall('recall_button', r.rr(4)), remembered)[1])
    r.at('RecallPackageMarker__7FEngine', lambda: (r.tracecall('recall_marker'), VT if found else 0)[1])
    r.at('FindConditionBranchTarget__C17FEMessageResponseUl', lambda: (r.tracecall('branch', r.rr(4)), 1)[1])
    traces(r, [('SetCurrentScript__8FEObjectP8FEScript', 2), ('QueueMessage__7FEngineUlP8FEObjectP9FEPackageT2Ul', 6),
               ('SetCurrentButton__9FEPackageP7FEngineP8FEObjectb', 4), ('RecordLastPackageButton__7FEngineUlUl', 3),
               ('QueuePackageUserTransfer__7FEngineP9FEPackagebUl', 4), ('QueuePackageSwitch__7FEnginePCcP9FEPackage', 3),
               ('QueuePackagePush__7FEnginePCcP9FEPackageUl', 4), ('QueuePackagePop__7FEngineP9FEPackage', 2),
               ('RecordPackageMarker__7FEnginePCc', 2), ('ClearPackageMarkers__7FEngine', 1)])
    r.call('ProcessResponses__7FEngineP17FEMessageResponseP8FEObjectP9FEPackageUl', OBJ, DATA, OUT, GAME, mask)
    return r.rd(OBJ + 8), snap(r, OUT + 0x100, 0x30), r.trace


def push_package(r, case):
    mode, release, name = case
    r.cstring(DATA - 1, '@' + name)
    r.string_stubs(); r.wr(OBJ + 4, 1); r.wr(OBJ + 0x120, GAME); r.wr(GAME, VT)
    calls = []
    def find():
        text = r.text(r.rr(4)); calls.append(text); r.tracecall('find', text)
        return EXTRA if mode == 'idle' or (mode == 'basename' and len(calls) == 2) else 0
    r.at('FindIdlePackage__C7FEnginePCc', find)
    def data():
        r.tracecall('data', r.text(r.rr(4)))
        r.wr(r.rr(5), OUT); r.wr(r.rr(6), release)
        return 0 if mode == 'no_data' else OUT
    r.slot(VT, 8, data)
    r.slot(VT, 0x10, lambda: (r.tracecall('localize', r.rr(4)), -1 if mode == 'localize_fail' else 0)[1])
    r.slot(VT, 0x28, lambda: r.tracecall('loaded', r.rr(4)))
    r.at('LoadPackage__7FEnginePCvb', lambda: (r.tracecall('load', r.rr(4), r.rr(5)), 0 if mode == 'load_fail' else EXTRA)[1])
    r.at('ForAllObjects__9FEPackageR16FEObjectCallback', lambda: r.tracecall('init', r.rr(3), r.rd(r.rr(4))))
    traces(r, [('RemNode__9FEMinListP9FEMinNode', 2), ('AddPackage__13FEPackageListP9FEPackage', 2), ('__builtin_vec_delete', 1)])
    ret = r.call('PushPackage__7FEnginePCcUcUl', OBJ, DATA, 7, 13)
    return ret, snap(r, EXTRA, 0xb0), r.trace


def lifetime(r, case):
    destroy, populated, flag = case
    if not destroy:
        r.u.mem_write(OBJ, b'\xa5' * 0xb0)
        r.call('__9FEPackage', OBJ)
        return snap(r, OBJ, 0xb0)
    r.wr(OBJ + 0x9c + 4, DATA if populated else 0)
    if populated:
        for offset in [0x38, 0x68, 0x70, 0x98, 0x88]: r.wr(OBJ + offset, EXTRA + offset + 8)
        r.wr(EXTRA + 0x70, 0, 2)  # array cookie: size unused here, two target elements
        r.wr(DATA, 0, 0, VT, 0, OUT)
        r.slot(VT, 8, lambda: r.tracecall('pending_delete', r.rr(3), r.rr(4)))
    def head():
        p = r.rd(r.rr(3) + 4); r.wr(r.rr(3) + 4, 0); return p
    r.at('RemHead__9FEMinList', head)
    traces(r, [('__builtin_vec_delete', 1), ('Purge__9FEMinList', 1), ('_._6FENode', 2)])
    r.call('_._9FEPackage', OBJ, flag)
    return snap(r, OBJ, 0xb0), r.trace


def navigation(r, case):
    direction, wrap, disabled, positions = case
    r.wr(OBJ, DATA, len(positions)); r.wr(DATA, *[EXTRA + 0x100*i for i in range(len(positions))])
    for i, (x, y) in enumerate(positions):
        node = EXTRA + i*0x100
        r.wr(node + 0x1c, 0x4000000 if disabled and i == 1 else 0)
        r.wr(node + 0x2c, node + 0x60); r.wf(node + 0x60 + 28, x, y, 0)
    for i in range(8): r.wf(0x802ee580 + i*8, math.cos(i*math.pi/4), math.sin(i*math.pi/4))
    for i, xy in enumerate([(0, 0), (1000, 0), (-1000, 0), (0, 1000), (0, -1000)]): r.wf(0x802ee5c0 + i*8, *xy)
    r.at('sqrtf', r.sqrt)
    return r.call('GetButtonFrom__11FEButtonMapP8FEObjectlP15FEGameInterface16FEButtonWrapMode', OBJ, EXTRA, direction, 0, wrap)


def location(r, case):
    game, context, success = case
    r.wr(DATA + 0x2c, EXTRA); r.wf(EXTRA + 28, 2, -3, 7)
    r.u.mem_write(DATA + 0x20, struct.pack('>H', context))
    r.wr(GAME, VT)
    def transform():
        r.tracecall('context', r.rr(4)); return success
    r.slot(VT, 0x68, transform)
    def mult():
        r.tracecall('mult', r.rf(r.rr(5))); r.wf(r.rr(3), 4, -6, 14)
    r.at('FEMultMatrix__FP9FEVector3PC9FEMatrix4PC9FEVector3', mult)
    r.call('ComputeButtonLocation__11FEButtonMapP8FEObjectP15FEGameInterfaceR9FEVector2', OBJ, DATA, GAME if game else 0, OUT)
    return r.rf(OUT, 2), r.trace



def package_commands(r, case):
    codes, mask, priority, parent, push_ok = case
    r.wr(DATA + 0x24, priority, 15); r.wr(DATA + 0x3c, OUT if parent else 0)
    r.wr(OUT + 0x28, 0x10)
    r.wr(OBJ + 0xf4, OUT); r.wr(OUT, OUT + 0x100); r.wr(OUT + 0x100 + 0x3c, DATA)
    commands = []
    for i, code in enumerate(codes):
        c = EXTRA + 0x40*i; commands.append(c)
        r.wr(c + 8, VT, EXTRA + 0x400, 0, code, mask, DATA)
    r.cstring(EXTRA + 0x400, 'next')
    r.slot(VT, 8, lambda: r.tracecall('delete', r.rr(3), r.rr(4)))
    r.at('RemHead__9FEMinList', lambda: commands.pop(0) if commands else 0)
    traces(r, [('ReplaceParentLinks__13FEPackageListPC9FEPackageT1', 3), ('UnloadPackage__7FEngineP9FEPackage', 2),
               ('QueueMessage__7FEngineUlP8FEObjectP9FEPackageT2Ul', 6)])
    def push():
        r.tracecall('push', r.text(r.rr(4)), r.rr(5), r.rr(6)); return GAME if push_ok else 0
    r.at('PushPackage__7FEnginePCcUcUl', push)
    r.at('GetPortNum__C14CDeviceManagerUc', lambda: (r.tracecall('port', r.rr(4)), r.rr(4) + 4)[1])
    r.call('ProcessPackageCommands__7FEngine', OBJ)
    return snap(r, DATA, 0xb0), snap(r, OUT, 0x1b0), snap(r, GAME, 0xb0), r.trace


def input_stubs(r, current, object_response, package_response, disable, navigate):
    r.wr(OBJ + 8, 1); r.wr(OBJ + 0x120, GAME); r.wr(GAME, VT)
    r.wr(DATA + 0x8c, EXTRA if current else 0)
    r.wr(OBJ + 0x98 + 4*4, EXTRA); r.wr(OBJ + 0x98 + 5*4, EXTRA)
    r.at('FindResponse__C8FEObjectUl', lambda: (r.tracecall('object_response', r.rr(3), r.rr(4)), OUT if object_response else 0)[1])
    r.at('FindResponse__9FEPackageUl', lambda: (r.tracecall('package_response', r.rr(4)), OUT if package_response else 0)[1])
    def queue():
        r.tracecall('queue', *[r.rr(i) for i in range(4, 9)])
        r.wr(OBJ + 0x4128, r.rd(OBJ + 0x4128) + 1)
    r.at('QueueMessage__7FEngineUlP8FEObjectP9FEPackageT2Ul', queue)
    def flush():
        r.tracecall('flush'); r.wr(OBJ + 0x4128, 0)
        if disable: r.wr(OBJ + 8, 0)
    r.at('ProcessMessageQueue__7FEngine', flush)
    def set_button():
        r.tracecall('button', r.rr(5), r.rr(6)); r.wr(DATA + 0x8c, r.rr(5))
    r.at('SetCurrentButton__9FEPackageP7FEngineP8FEObjectb', set_button)
    r.at('FindResponse__C17FEMessageResponseUl', lambda: (r.tracecall('response_command', r.rr(4)), -1 if navigate else 0)[1])
    r.at('GetButtonFrom__11FEButtonMapP8FEObjectlP15FEGameInterface16FEButtonWrapMode', lambda: (r.tracecall('navigate', *[r.rr(i) for i in range(4, 8)]), EXTRA + 0x100 if navigate else 0)[1])


def pads(r, case):
    previous, current_bits, duration, ports, current, obj_rsp, pkg_rsp, navigation, repeat, disable, alias, blocked = case
    input_stubs(r, current, obj_rsp, pkg_rsp, disable, navigation)
    copy_stubs(r)
    r.wr(OBJ + 0x18, OUT + 0x1000); r.wr(OBJ + 0xe8, 4)
    r.u.mem_write(OBJ + 0xec, struct.pack('>H', 15))
    r.wr(DATA + 0x28, ports); r.wr(DATA + 0x20, alias)
    r.wr(EXTRA + 0x1c, 0x80000 if blocked else 0)
    r.wr(OBJ + 0x40, repeat, 0xff)
    for i in range(4):
        pad = OUT + 0x1000 + i*136
        before = previous[i] if isinstance(previous, tuple) else previous
        after = current_bits[i] if isinstance(current_bits, tuple) else current_bits
        held_time = duration[i] if isinstance(duration, tuple) else duration + i
        r.wr(pad, before, after, *([held_time] * 32))
    r.call('ProcessPadsForPackage__7FEngineP9FEPackage', OBJ, DATA)
    return snap(r, OBJ + 0x40, 0xa4), r.rd(OBJ + 8), snap(r, OBJ + 0x526c, 1), r.rd(DATA + 0x8c), r.trace


def mouse_input(r, case):
    previous, buttons, moved, current, obj_rsp, pkg_rsp, hit, disable, disabled = case
    input_stubs(r, current, obj_rsp, pkg_rsp, disable, True)
    r.wr(DATA + 0x28, 1)
    r.wr(OBJ + 0x1c, -13, 17, 0)
    r.u.mem_write(OBJ + 0x1c + 12, struct.pack('>HH', previous, buttons))
    r.wr(OBJ + 0x1c + 16, 123, 456, 789, 0, moved)
    r.wr(DATA + 0x90, OUT + 0x100, 2)
    r.wr(OUT + 0x100, EXTRA + 0x100, EXTRA + 0x200)
    r.wr(EXTRA + 0x100 + 0x1c, 0x4000000 if disabled else 0)
    def touch():
        r.tracecall('touch', r.fr(1), r.fr(2), r.rr(4))
        return hit if r.rr(4) == EXTRA else 2
    r.slot(VT, 0xa0, touch)
    r.call('ProcessMouseForPackage__7FEngineP9FEPackage', OBJ, DATA)
    return snap(r, OBJ + 0x98, 76), r.rd(OBJ + 8), r.rd(DATA + 0x8c), r.trace



def types(r, operation):
    blocks = allocation_stubs(r)
    fields, nodes = {}, []
    sizes = {1: 4, 3: 8, 4: 12, 5: 16, 6: 16}
    def add_field():
        obj, name, kind = r.rr(3), r.text(r.rr(4)), r.rr(5)
        field = EXTRA + len(fields)*0x40
        previous = r.rd(obj + 0x1c)
        r.wr(field, 0, previous, 0); r.wr(field + 0x14, kind, sizes[kind])
        if previous: r.wr(previous, field)
        else: r.wr(obj + 0x18, field)
        r.wr(obj + 0x1c, field)
        fields[(obj, name)] = field
        r.tracecall('field', obj, name, kind)
    r.at('AddField__10FETypeNodePCcl', add_field)
    r.at('GetField__10FETypeNodePCc', lambda: fields[(r.rr(3), r.text(r.rr(4)))])
    r.at('SetDefault__11FEFieldNodePv', lambda: r.tracecall('default', r.rr(3), snap(r, r.rr(4), r.rd(r.rr(3)+0x18))))
    r.at('SetName__6FENodePCc', lambda: r.tracecall('name', r.rr(3), r.text(r.rr(4))))
    def base():
        node = DATA + 0x100*len(nodes); nodes.append(node)
        r.tracecall('base', r.text(r.rr(4)))
        return node
    if operation != 'base': r.at('CreateBaseObjectType__9FETypeLibPCc', base)
    if operation == 'startup': r.at('CreateImageObjectType__9FETypeLibPCc', base)
    r.cstring(OUT, 'custom_type'); add_list_stub(r)
    names = {'base': 'CreateBaseObjectType__9FETypeLibPCc', 'image': 'CreateImageObjectType__9FETypeLibPCc', 'startup': 'Startup__9FETypeLib'}
    ret = r.call(names[operation], OBJ, OUT)
    return ret, snap(r, OBJ, 20), [snap(r, n, 40) for n in nodes], [snap(r, n, size) for n, size in blocks], r.trace


def locale(r, case):
    mode, languages, final_result = case
    lines = ['[HEADER]', 'comment', 'Languages = ' + str(languages), *['lang' + str(i) for i in range(languages)],
             '[LABEL] greeting', '[PLATFORM] GC', '[COMMENT] menu', '[FLAGS] F', '[MAX_SIZE] 80',
             '[unknown] ignored', *['[lang' + str(i) + '] text' + str(i) for i in range(languages)], '[FLAGS] final']
    if mode == 'empty': lines = []
    if mode == 'wrong_header': lines[0] = '[not_header]'
    if mode == 'no_languages': lines = lines[:2]
    def wide(ptr):
        values = []
        while True:
            c = struct.unpack('>H', r.u.mem_read(ptr, 2))[0]
            if not c: return ''.join(map(chr, values))
            values.append(c); ptr += 2
    cursor = [0]
    def read():
        i = cursor[0]; cursor[0] += 1
        text = lines[i] if i < len(lines) else ''
        r.u.mem_write(r.rr(4), text.encode('utf-16-be') + b'\0\0')
        return int(i < len(lines) - (not final_result))
    r.at('ReadNextString__11FELocalizerPsPcRUlUl', read)
    r.string_stubs(); values = {}
    r.at('__12FEWideString', lambda: (values.update({r.rr(3): ''}), r.rr(3))[1])
    r.at('_._12FEWideString', lambda: r.tracecall('wide_destroy'))
    def assign(kind):
        dest, src = r.rr(3), r.rr(4)
        value = values[src] if kind == 'copy' else (wide(src) if kind == 'wide' else (r.text(src) if src else ''))
        values[dest] = value
        r.tracecall('assign', kind, value, dest if dest < 0x10f00000 else 'temporary')
        return dest
    for signature, kind in [('PCc', 'char'), ('PCs', 'wide'), ('RC12FEWideString', 'copy')]:
        r.at('__as__12FEWideString' + signature, lambda kind=kind: assign(kind))
    r.at('WideToEscapeString__11FELocalizerPcPCs', lambda: r.cstring(r.rr(4), wide(r.rr(5))))
    def count():
        r.tracecall('languages', r.rr(4)); r.wr(OBJ, r.rr(4))
        r.wr(OBJ + 8, DATA if r.rr(4) else 0)
    r.at('SetNumLanguages__11FELocalizerUl', count)
    def language():
        i, name = r.rr(5), r.text(r.rr(4)); node = DATA + 32*i
        r.tracecall('language', name, i); r.wr(node, node + 32 if i + 1 < languages else 0)
        r.wr(node + 12, OUT + 64*i); r.cstring(OUT + 64*i, name)
    r.at('SetLanguageName__11FELocalizerPCcUl', language)
    r.wr(OBJ + 0x24, EXTRA); r.wr(EXTRA + 0x18, GAME)
    r.at('SetString__11FELocalizerPCcUlR12FEWideString', lambda: r.tracecall('label', r.text(r.rr(4)), r.rr(5), values[r.rr(6)]))
    r.at('GetLabelIndex__C11FELocalizerPCc', lambda: (r.tracecall('label_index', r.text(r.rr(4))), 0)[1])
    for name in ['Platform', 'Comment', 'Flags', 'Size']:
        r.at('Set' + name + '__13FELocalStringPCc', lambda name=name: r.tracecall(name, r.rr(3), r.text(r.rr(4))))
    ret = r.call('ReadLocaleData__11FELocalizerPvUl', OBJ, VT, 12345)
    return ret, cursor, r.trace


def script_tags(r, case):
    operation, keep, reference, option = case
    r.wr(OBJ + 0x10, keep, DATA); r.wr(OBJ + 0x20, reference); r.wr(OBJ + 0x44, GAME)
    r.wr(DATA + 0x18, 1)
    r.wr(OUT + 0x30, 0x1744b3 if option else 42)
    blocks = []
    def alloc():
        size = r.rr(3); ptr = 0x10080000 + len(blocks)*0x1000
        blocks.append((ptr, size)); r.u.mem_write(ptr, b'\xa5'*size)
        r.tracecall('allocate', size); return ptr
    r.at('__builtin_vec_new', alloc); r.at('__nw__9FEKeyNodeUi', alloc)
    r.at('__nw__8FEScriptUi', lambda: OUT)
    def init():
        r.tracecall('init'); r.u.mem_write(OUT + 12, bytes(40))
    r.at('Init__8FEScript', init)
    r.at('SetName__8FEScriptPCc', lambda: r.tracecall('name', r.text(r.rr(4))))
    r.at('FindScript__C8FEObjectUl', lambda: (r.tracecall('find_script', r.rr(4)), OUT)[1])
    tracks = EXTRA + 0x1000
    def count():
        r.tracecall('tracks', r.rr(4)); r.wr(OUT + 0x1c, r.rr(4), tracks)
        if option: r.wr(tracks + 0x28, 1)
    r.at('SetTrackCount__8FEScriptl', count)
    def events():
        r.tracecall('events', r.rr(4)); r.wr(r.rr(3), r.rr(4), EXTRA + 0x2000)
    r.at('SetCount__11FEEventListl', events)
    def unreference():
        r.tracecall('unreference'); r.wr(r.rr(3), 0, 0, 0)
    r.at('ReferenceList__9FERefListP9FERefList', unreference)
    def add_key():
        lst, before, node = r.rr(3), r.rr(4), r.rr(5)
        r.tracecall('key', lst, before, node); r.wr(node, 0, before)
        if before: r.wr(before, node)
        else: r.wr(lst + 4, node)
        r.wr(lst + 8, node)
    r.at('AddNode__9FERefListP9FEMinNodeT1', add_key)
    add_list_stub(r)
    traces(r, [('Purge__9FERefList', 1), ('__builtin_vec_delete', 1)])
    def assign_track():
        r.tracecall('copy_track', r.rr(3), r.rr(4)); r.u.mem_write(r.rr(3), snap(r, r.rr(4), 56))
    r.at('__as__10FEKeyTrackR10FEKeyTrack', assign_track)
    def tag(code, data): return struct.pack('<HH', code, len(data)) + data
    def words(*v): return struct.pack('<' + 'I'*len(v), *[x & 0xffffffff for x in v])
    if operation == 'new':
        body = tag(0x6653, words(9)) + tag(0x6e53, b'animate\0')
        body += tag(0x6853, words(0x1744b3 if option else 42, 100, 2, 2))
        body += tag(0x6c53, words(250)) + tag(0x6653, words(1)) + tag(0x6353, words(0x1234))
        for i in range(2):
            body += tag(0x4946, bytes([4, 12, 1, 3]) + words(33+i))
            body += tag(0x6f54, b'\x07') + tag(0x7454, b'\x02') + tag(0x6154, b'\x80')
            body += tag(0x6254, words(0, 0x3f800000, 0x40000000, 0x40400000))
            body += tag(0x644b, words(0, 1, 2, 3, 10, 4, 5, 6, 20, 7, 8, 9))
        body += tag(0x5645, words(123, -4, 10, 456, -1, 20)) + tag(0xffff, b'x')
    else:
        old_count = 0 if option == 3 else 2
        r.wr(OUT + 0xc, 90); r.wr(OUT + 0x1c, old_count, tracks if old_count else 0)
        r.wr(tracks - 8, 0, old_count)
        for i in range(old_count):
            r.u.mem_write(tracks + i*56, bytes([4, 12, 1, 0]))
            r.wr(tracks + i*56 + 4, (33 << 8) | (3 + i*4))
        r.at('FindTrack__C8FEScript18FEKeyTrack_Indices', lambda: (r.tracecall('track', r.rr(4)), tracks if operation == 'found' else 0)[1])
        r.at('FindType__9FETypeLibUl', lambda: (r.tracecall('type', r.rr(4)), VT)[1])
        r.at('FindNode__C9FEMinListUl', lambda: (r.tracecall('field', r.rr(4)), VT + 0x100)[1])
        r.wr(VT + 0x100 + 0x14, 4, 12, [0, 20, 40, 0][option])
        body = tag(0x4953, words(123)) + tag(0x6954, struct.pack('<H', 7)) + tag(0x7454, b'\x03')
    r.u.mem_write(EXTRA, body)
    ret = r.call('ReadScriptTags__15FEPackageReaderP5FETagUl', OBJ, EXTRA, len(body))
    return ret, snap(r, DATA, 0x58), snap(r, OUT, 0x34), snap(r, tracks, 112), snap(r, EXTRA+0x2000, 24), [snap(r, p, n) for p, n in blocks], r.trace


@unittest.skipIf(ppc.Uc is None, 'Optional Unicorn not installed')
class LifecycleTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        try:
            _, ppc.original = ppc.load_target()
            ppc.symbols = {s['name']: s for s in ppc.original.symbols() if s['section']}
        except (FileNotFoundError, ValueError) as exc:
            raise unittest.SkipTest(str(exc))

    def cases(self, unit, cases, body):
        if not (native_directory(unit) / 'compiled.elf').exists(): self.skipTest('Build ' + unit + ' first')
        for case in cases:
            with self.subTest(case=case):
                a = ppc.normalized(body(Run(), case)); b = ppc.normalized(body(Run(native_directory(unit)), case))
                self.assertEqual(a, b)

    def test_type_base(self):
        self.cases('fetype_create_base', ['base'], types)

    def test_type_image(self):
        self.cases('fetype_create_image', ['image'], types)

    def test_type_startup(self):
        self.cases('fetype_startup', ['startup'], types)

    def test_locale(self):
        self.cases('felocale_read_data', itertools.product(['empty', 'wrong_header', 'no_languages', 'valid'], [0, 1, 3], [0, 1]), locale)

    def test_script_tags(self):
        self.cases('fereader_script_tags', itertools.product(['new', 'found', 'insert'], [0, 1], [0, 1], range(4)), script_tags)

    def test_package_commands(self):
        cases = [(codes, mask, priority, parent, ok) for codes, mask, priority, parent, ok in itertools.product([[1], [2], [3], [4], [8], [3, 1, 2, 4, 8], [0]], [0, 1, 2, 4, 8, 15], [-1, 0, 4], [0, 1], [0, 1]) if ok or not any(c & 3 == 3 for c in codes)]
        self.cases('fengine_package_commands', cases, package_commands)

    def test_pads(self):
        rng = random.Random(0x8004d7dc)
        states = [(0, 0), (0, 16), (16, 0), (16, 16), (0, 128), (128, 128), (0, 256), (256, 0), (0, 64), (0, 1), (1, 1), (0, 5), (5, 5), (0, 15), (0x7ffff, 0), (0, 0x7ffff)]
        cases = [(*rng.choice(states), *[rng.choice(xs) for xs in [[0, 1, 119, 120, 319, 320], [0, 1, 4, 8, 15], [0, 1], [0, 1], [0, 1], [0, 1], [0, 255], [0, 1], [0, 1], [0, 1]]]) for _ in range(240)]
        # Mixed ports distinguish diagonals pressed on separate controllers from
        # a diagonal on one controller, and exercise held/pressed/released overlap.
        mixed = [((0, 0, 0, 0), (1, 4, 0, 0)), ((1, 4, 0, 0), (1, 4, 0, 0)),
                 ((0, 0, 0, 0), (5, 0, 0, 0)), ((5, 0, 0, 0), (5, 0, 0, 0)),
                 ((16, 128, 0, 64), (0, 128, 16, 0))]
        cases += [(before, after, times, 15, 1, obj, pkg, 1, 0, 0, 1, 0)
                  for (before, after), times, obj, pkg in itertools.product(
                      mixed, [(0, 0, 0, 0), (119, 120, 319, 320), (320, 319, 120, 119)], [0, 1], [0, 1])]
        self.cases('fengine_pads', cases, pads)

    def test_mouse(self):
        rng = random.Random(0x8004e3f0)
        cases = [tuple(rng.choice(xs) for xs in [[0, 1, 2, 3], [0, 1, 2, 3], [0, 1], [0, 1], [0, 1], [0, 1], [0, 1, 2], [0, 1], [0, 1]]) for _ in range(160)]
        self.cases('fengine_mouse', cases, mouse_input)

    def test_object_update(self):
        rng = random.Random(0x8005158c)
        cases = [tuple(rng.choice(xs) for xs in [[0, 1, 2], [-3, 0, 4, 10, 11, 21], [0, 10], [-20, 0, 5, 30], [0, 1, 2, 3], [0, 2], [0, 1], [1, 4, 5, 6, 7], [0, 0x400000, 0x1800000], [0, 0xffffffff]]) for _ in range(240)]
        self.cases('fepackage_update_object', cases, object_update)

    def test_engine_update(self):
        cases = [(1, 1, m, dt, stop, q, hold) for m, dt, stop, q, hold in itertools.product([0, 1], [-5, 0, 20, 43], [0, 1], [0, 1], [0, 1])]
        cases += [(run, inp, 1, 43, 0, 0, 1) for run, inp in [(0, 0), (0, 1), (1, 0)]]
        self.cases('fengine_update', cases, engine_update)

    def test_responses(self):
        ids = [0, 1, 2, 3, *range(0x100, 0x109), *range(0x200, 0x205), *range(0x2c0, 0x2c3), 0x300, 0x301, 0x500, 0x777]
        self.cases('fengine_responses', itertools.product(ids, [0, 1, 7], [0, 1], [0, 42], [0, 0x85]), responses)

    def test_push_package(self):
        self.cases('fengine_push_package', itertools.product(['idle', 'basename', 'no_data', 'load_fail', 'localize_fail', 'loaded'], [0, 1], ['menu', 'fe/menu', 'fe\\menu']), push_package)

    def test_lifetime(self):
        self.cases('fepackage_lifetime', itertools.product([0, 1], [0, 1], [2, 3]), lifetime)

    def test_navigation(self):
        positions = [[(0, 0)], [(0, 0), (20, 0), (-20, 0), (0, 30), (0, -30)], [(0, 0), (0, 0), (1500, 0)], [(0, 0), (float('nan'), 3), (20, 1)]]
        self.cases('febutton_navigation', itertools.product(range(8), [0, 1, 3, 7], [0, 1], positions), navigation)

    def test_location(self):
        self.cases('febutton_navigation', itertools.product([0, 1], [0, 1, 0x8001], [0, 1]), location)


if __name__ == '__main__': unittest.main()
