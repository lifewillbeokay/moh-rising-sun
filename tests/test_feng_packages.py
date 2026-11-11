"""Original/native PPC checks for the package, object and localization batch.

Requires the pinned original, built units/comparisons and optional Unicorn.
External allocation and engine callbacks are stubbed. These are isolated checks,
not tests of a running game or a claim of complete semantic equivalence.
"""
import itertools
import struct
import unittest
from pathlib import Path

import test_explosion_grenade as ppc
from test_shortlist_partials import Run
from test_feng_animation_lists import copy_stubs, allocation_stubs

ROOT = Path(__file__).resolve().parents[1]
EXACT = {"fe_stricmp", "felocale_copy_string", "fetype_find", "fengine_unload_library"}


def native_directory(unit):
    return (ROOT / "build/reconstruction/units" / unit if unit in EXACT else
            ROOT / "build/comparisons" / ("compare_" + unit))


OBJ, DATA, EXTRA, OUT, GAME, VT = [0x10020000 + i * 0x10000 for i in range(6)]


def compare(unit, case, body):
    a = ppc.normalized(body(Run(), case))
    b = ppc.normalized(body(Run(native_directory(unit)), case))
    assert a == b, (unit, case, a, b)


def strings(r, case):
    operation, a, b = case
    r.u.mem_write(DATA, a + b"\0")
    r.u.mem_write(EXTRA, b + b"\0")
    if operation == "stricmp":
        return r.call("FEStricmp__FPCcT0", DATA, EXTRA)
    if operation == "atoi":
        return r.call("AtoI__FPCc", DATA if b else 0)
    if operation == "itoa":
        ret = r.call("ItoA__FPcUl", OUT, int(a))
        return ret, bytes(r.u.mem_read(OUT, 32))
    wide = [x if x < 128 else x - 256 for x in a] if operation != "wide_invalid" else [65, 300]
    r.u.mem_write(DATA, struct.pack(">" + "h" * (len(wide) + 1), *wide, 0))
    if operation == "length":
        return r.call("GetStringLength__FPCs", DATA if b else 0)
    if operation.startswith("wide"):
        r.u.mem_write(OUT, b"\xa5" * 32)
        ret = r.call("WideToCharString__FPcPCs", OUT, DATA)
        return ret, bytes(r.u.mem_read(OUT, 32))
    return r.call(("CompareUpperSubst" if operation == "prefix" else "CompareUpper") + "__FPCsPCc", DATA, EXTRA)


def read_string(r, case):
    data, offset = case
    r.u.mem_write(DATA, data + b"\0")
    r.u.mem_write(OUT, b"\xa5" * 128)
    r.wr(EXTRA, offset)
    ret = r.call("ReadString__11FELocalizerPsPcRUlUl", OBJ, OUT, DATA, EXTRA, len(data))
    return ret, r.rd(EXTRA), bytes(r.u.mem_read(OUT, 128))


def event_count(r, case):
    count, new = case
    r.wr(OBJ, count, DATA if count else 0)
    r.wr(DATA, *range(1, 19))
    blocks = allocation_stubs(r)
    r.at("__builtin_delete", lambda: r.tracecall("scalar_delete", r.rr(3)))
    r.call("SetCount__11FEEventListl", OBJ, new)
    return bytes(r.u.mem_read(OBJ, 8)), [bytes(r.u.mem_read(p, n)) for p, n in blocks], r.trace


def copy_string(r, case):
    old, text = case
    r.wr(OBJ, EXTRA if old else 0)
    r.string_stubs()
    blocks = allocation_stubs(r)
    r.cstring(DATA, text or "")
    r.call("FECopyString__FRPcPCc", OBJ, DATA if text is not None else 0)
    return r.rd(OBJ), [bytes(r.u.mem_read(p, n)) for p, n in blocks], r.trace


def find_child(r, case):
    sizes, wanted = case
    body = b"".join(struct.pack("<II", i + 10, size) + bytes(size) for i, size in enumerate(sizes))
    r.u.mem_write(DATA, struct.pack("<II", 1, len(body)) + body + struct.pack("<II", 99, 0))
    return r.call("FindChild__15FEPackageReaderP7FEChunkUl", OBJ, DATA, wanted)


def read_types(r, case):
    found, size = case
    r.wr(OBJ + 0x38, 123, 456)
    r.wr(OBJ + 4, DATA)
    r.u.mem_write(EXTRA, struct.pack("<II", 0x53707954, size))
    def find():
        r.tracecall("find", r.rr(4), r.rr(5))
        return EXTRA if found else 0
    r.at("FindChild__15FEPackageReaderP7FEChunkUl", find)
    ret = r.call("ReadTypeSizes__15FEPackageReader", OBJ)
    return ret, bytes(r.u.mem_read(OBJ, 0x58)), r.trace


def branch(r, case):
    ids, start = case
    r.wr(OBJ + 0x10, len(ids), DATA)
    for i, code in enumerate(ids + [0]):
        r.wr(DATA + i * 12, code, 0, 0)
    return r.call("FindConditionBranchTarget__C17FEMessageResponseUl", OBJ, start)


def marker(r, count):
    r.wr(OBJ + 0x415c, count)
    r.cstring(DATA, "menu/checkpoint")
    r.string_stubs()
    ret = r.call("RecordPackageMarker__7FEnginePCc", OBJ, DATA)
    return ret, bytes(r.u.mem_read(OBJ + 0x415c, 4 + 4096))


def close(r, case):
    for i, value in enumerate(case, 1):
        r.fw(i, value)
    return r.call("Close__Ffff")


def color(r, case):
    a, b = case
    r.wr(DATA, *a)
    r.wr(EXTRA, *b)
    r.call("__mi__C7FEColorRC7FEColor", OUT, DATA, EXTRA)
    return bytes(r.u.mem_read(OUT, 16))


def unload(r, case):
    approved, release, clear_game = case
    r.wr(OBJ + 0x120, GAME)
    r.wr(GAME, VT)
    r.wr(DATA + 8, VT + 0x200)
    r.slot(VT, 0x38, lambda: (r.tracecall("approve", r.rr(4)), approved)[1])
    r.slot(VT, 0x30, lambda: (r.tracecall("will_unload", r.rr(4)), release)[1])
    r.slot(VT + 0x200, 8, lambda: r.tracecall("destroy", r.rr(3), r.rr(4)))
    def remove():
        r.tracecall("remove", r.rr(4))
        if clear_game:
            r.wr(OBJ + 0x120, 0)
    r.at("RemoveFromLibraryList__7FEngineP9FEPackage", remove)
    r.at("Shutdown__9FEPackageP15FEGameInterface", lambda: r.tracecall("shutdown", r.rr(3), r.rr(4)))
    r.call("UnloadLibraryPackage__7FEngineP9FEPackage", OBJ, DATA)
    return r.trace


def render_object(r, case):
    visible, depth = case
    r.wr(OBJ + 0x124, 2)
    r.wr(DATA + 0x2c, EXTRA)
    r.wr(EXTRA + 12, visible)
    r.wf(EXTRA + 16, 1, 2, 3, 4, 5, 6)
    copy_stubs(r)
    def transform():
        r.tracecall("transform", r.rf(r.rr(5)))
        r.wf(r.rr(3), 1, 2, depth)
    r.at("FEMultMatrix__FP9FEVector3PC9FEMatrix4PC9FEVector3", transform)
    r.call("RenderObject__7FEngineP8FEObjectR9FEMatrix4Us", OBJ, DATA, OUT, 7)
    return bytes(r.u.mem_read(DATA, 0x58)), bytes(r.u.mem_read(OBJ + 0x124, 32)), r.trace


def track_update(r, case):
    running, visible, first_offset, flags, hold, count = case
    r.wr(OBJ + 0x14, running)
    r.wr(DATA + 0x1c, 0x1c00000)
    r.wr(DATA + 0x2c, OUT)
    r.wr(OUT + 12, visible)
    r.wr(EXTRA + 0x10, 42)
    r.wr(EXTRA + 0x1c, count, GAME if count else 0)
    for i, offset in enumerate([first_offset, 7, 17]):
        r.u.mem_write(GAME + i * 0x38, bytes([4, 12, 1, flags]))
        r.wr(GAME + i * 0x38 + 4, offset)
    r.wr(ppc.symbols["_9FEPackage.uHoldDirtyFlags"]["address"], hold)
    for callee in ["FEKeyInterp__FP10FEKeyTracklPv", "FEKeyInterpFast__FP10FEKeyTracklPv"]:
        r.at(callee, lambda name=callee: r.tracecall(name, r.rr(3), r.rr(4), r.rr(5)))
    r.call("UpdateObjectTracks__9FEPackageP8FEObjectP8FEScript", OBJ, DATA, EXTRA)
    return bytes(r.u.mem_read(DATA, 0x58)), r.trace


def messages(r, case):
    start, end, target, special, sender = case
    r.wr(EXTRA + 0xc, 10)
    r.wr(EXTRA + 0x24, 4, OUT)
    for i, time in enumerate([0, 3, 5, 10]):
        r.wr(OUT + i * 12, 0x1b3909aa if special else i + 100, target, time)
    for callee, argc in [("SendMessageToGame__7FEngineUlP8FEObjectP9FEPackageUl", 5),
                         ("QueueMessage__7FEngineUlP8FEObjectP9FEPackageT2Ul", 6),
                         ("SetCurrentButton__9FEPackageP7FEngineP8FEObjectb", 4)]:
        r.at(callee, lambda name=callee, n=argc: r.tracecall(name, *[r.rr(i) for i in range(3, 3+n)]))
    r.at("FindObjectByGUID__9FEPackageUl", lambda: (r.tracecall("find", r.rr(4)), VT)[1])
    r.call("IssueScriptMessages__9FEPackageP7FEngineP8FEObjectP8FEScriptll", OBJ, GAME, DATA if sender else 0, EXTRA, start, end)
    return r.trace


def add_list_stub(r):
    def add():
        lst, before, node = r.rr(3), r.rr(4), r.rr(5)
        r.tracecall("add", lst, before, node)
        r.wr(node, 0, before)
        if before:
            r.wr(before, node)
        else:
            r.wr(lst + 4, node)
        r.wr(lst + 8, node)
        r.wr(lst, r.rd(lst) + 1)
    r.at("AddNode__9FEMinListP9FEMinNodeT1", add)


def move_to(r, case):
    kind, mode, empty, reference = case
    r.wr(OBJ + 0x54, DATA)
    r.wr(OBJ + 0x2c, OUT)
    r.wr(DATA + 0x1c, 1, EXTRA)
    r.u.mem_write(EXTRA, bytes([kind, 16, mode, 255]))
    r.wr(EXTRA + 4, 7)
    r.wf(EXTRA + 0x18, 0.1, -0.2, 0.3, 0.9)
    r.wf(OUT + 28, 1.25, -2.5, 3.75, 4.5)
    if kind == 1:
        r.wr(EXTRA + 0x18, -3)
    if kind == 6:
        r.wr(EXTRA + 0x18, 20, 30, 40, 50)
        r.wr(OUT + 28, 10, 25, 60, 100)
    head = 0 if empty else GAME
    r.wr(EXTRA + 0x28, int(reference), VT if reference else head, 0)
    r.wr(VT, 0, head, 0)
    r.wf(GAME + 16, 91, 92, 93, 94)
    r.call("SetupMoveToTracks__8FEObject", OBJ)
    values = bytes(r.u.mem_read(GAME + 16, 16)) if kind in [1, 6] else r.rf(GAME + 16, 4)
    return bytes(r.u.mem_read(EXTRA, 4)), values


def set_track(r, case):
    relative, present = case
    r.wr(OBJ + 0x48, DATA)
    r.wr(OBJ + 0x2c, OUT)
    r.wr(DATA, DATA + 0x80)
    r.wr(DATA + 0x80, 0)
    r.wf(EXTRA, 0.25, -0.5, 1.75)
    r.wf(OUT + 28, 2, 4, 6)
    r.u.mem_write(GAME + 3, b"\xff")
    r.wf(GAME + 0x18, 3, 5, 7)
    r.at("FindTrack__C8FEScript18FEKeyTrack_Indices", lambda: GAME if present and r.rr(3) == DATA else 0)
    r.at("GetDataOffset__8FEObject18FEKeyTrack_Indices", lambda: 28)
    r.call("SetTrackValue__8FEObject18FEKeyTrack_IndicesRC9FEVector3b", OBJ, 2, EXTRA, relative)
    return bytes(r.u.mem_read(GAME, 0x38)), bytes(r.u.mem_read(OUT, 64))


def position(r, case):
    kind, relative, value = case
    r.wr(OBJ + 0x18, kind, 0x123)
    r.wr(OBJ + 0x2c, DATA)
    r.wf(DATA + 28, 1, 2, 3)
    r.wf(EXTRA, *value)
    r.at("SetTrackValue__8FEObject18FEKeyTrack_IndicesRC9FEVector3b", lambda: r.tracecall("set", r.rr(4), r.rf(r.rr(5)), r.rr(6)))
    r.call("SetPosition__8FEObjectRC9FEVector3b", OBJ, EXTRA, relative)
    return r.rd(OBJ + 0x1c), r.trace


def script_copy(r, case):
    count, reference = case
    r.u.mem_write(OBJ, b"\xa5" * 0x34)
    r.wr(DATA + 0xc, 10, 20, 30)
    r.wr(DATA + 0x1c, count, EXTRA, 2, OUT)
    r.wr(DATA + 0x2c, GAME)
    for i in range(count):
        r.u.mem_write(EXTRA + i * 0x38, bytes(range(0x38)))
        r.u.mem_write(OUT + i * 0x38, b"\xa5" * 0x38)
    r.at("Init__8FEScript", lambda: r.wr(OBJ + 0xc, *([0] * 10)))
    r.at("SetName__8FEScriptPCc", lambda: r.tracecall("name", r.rr(4)))
    r.at("SetTrackCount__8FEScriptl", lambda: r.wr(OBJ + 0x1c, r.rr(4), OUT))
    for name in ["ReferenceList__9FERefListP9FERefList", "__as__10FEKeyTrackR10FEKeyTrack", "__as__11FEEventListR11FEEventList"]:
        r.at(name, lambda n=name: r.tracecall(n, r.rr(3), r.rr(4)))
    ret = r.call("__8FEScriptR8FEScriptb", OBJ, DATA, reference)
    return ret, bytes(r.u.mem_read(OBJ, 0x34)), bytes(r.u.mem_read(OUT, count * 0x38)), r.trace


def object_constructor(r, case):
    copy, count, reference = case
    r.u.mem_write(OBJ, b"\xa5" * 0x58)
    r.wr(ppc.symbols["_7FEngine.SysGUID"]["address"], 77)
    copy_stubs(r)
    if not copy:
        ret = r.call("__8FEObject", OBJ)
    else:
        r.wr(DATA + 0x14, GAME, 5, 0x1234, 0x12345678, 0x7654)
        r.wr(DATA + 0x30, 16)
        r.wr(DATA + 0x48, EXTRA if count else 0)
        r.wr(DATA + 0x54, EXTRA)
        for i in range(count):
            node = EXTRA + i * 0x80
            r.wr(node, node + 0x80 if i + 1 < count else 0)
            r.wr(node + 0x18, EXTRA if i else 0)
            r.wr(node + 0x30, i + 50)
        blocks = []
        def alloc():
            p = 0x10090000 + len(blocks) * 0x80
            blocks.append(p)
            return p
        r.at("__nw__8FEScriptUi", alloc)
        def ctor():
            r.wr(r.rr(3) + 0x30, r.rd(r.rr(4) + 0x30))
            r.tracecall("copy", r.rr(3), r.rr(4), r.rr(5))
            return r.rr(3)
        r.at("__8FEScriptR8FEScriptb", ctor)
        r.at("SetDataSize__8FEObjectUl", lambda: r.wr(OBJ + 0x2c, OUT, r.rr(4)))
        r.at("SetName__8FEObjectPCc", lambda: r.tracecall("name", r.rr(4)))
        r.at("FindScript__C8FEObjectUl", lambda: (r.tracecall("find", r.rr(4)), 0x10090000)[1])
        r.at("SetCurrentScript__8FEObjectP8FEScript", lambda: r.wr(OBJ + 0x54, r.rr(4)))
        add_list_stub(r)
        ret = r.call("__8FEObjectRC8FEObjectb", OBJ, DATA, reference)
    return ret, bytes(r.u.mem_read(OBJ, 0x58)), bytes(r.u.mem_read(OUT, 16)), bytes(r.u.mem_read(0x10090000, 0x100)), r.trace


def reader_list_tag(r, case):
    code, tagid, first = case
    r.wr(OBJ + 0x14, DATA)
    r.wr(OBJ + 0x2c, 0, 0, -1 if first else 0)
    r.wr(DATA + 0x5c, 3, 2)
    r.wr(DATA + 0x74, OUT, OUT + 0x100)
    r.wr(DATA + 0x84, 1, 1)
    r.u.mem_write(EXTRA, struct.pack("<HHIIII", tagid, 16, 0x3f000000, 0x3fc00000, 0x40000000, 0x40400000))
    r.at("GetPCellData__9FEListBoxUlUl", lambda: (r.tracecall("cell", r.rr(4), r.rr(5)), GAME)[1])
    for name, argc in [("SetNumColumns__9FEListBoxUl", 2), ("SetNumRows__9FEListBoxUl", 2),
                       ("IncrementCellByColumn__9FEListBox", 1), ("SetCellType__9FEListBoxUl", 2),
                       ("SetCellString__9FEListBoxPCs", 2), ("SetAutoWrap__9FEListBoxb", 2),
                       ("Initialize__13FECodeListBoxUlUl", 3), ("AllocateStrings__13FECodeListBoxUlUl", 3),
                       ("SetCellColor__13FECodeListBoxUlUlUlUlUl", 6),
                       ("SetCellJustification__13FECodeListBoxUlUlUlUlUl", 6)]:
        r.at(name, lambda n=name, count=argc: r.tracecall(n, *[r.rr(i) for i in range(3, 3 + count)]))
    r.at("SetCellScale__13FECodeListBoxUlUlRC7FEPointUlUl", lambda: r.tracecall("scale", r.rr(4), r.rr(5), r.rf(r.rr(6), 2), r.rr(7), r.rr(8)))
    r.call(("ProcessCodeListBoxTag" if code else "ProcessListBoxTag") + "__15FEPackageReaderP5FETag", OBJ, EXTRA)
    return bytes(r.u.mem_read(OBJ, 0x58)), bytes(r.u.mem_read(DATA, 0xc4)), bytes(r.u.mem_read(OUT, 0x200)), bytes(r.u.mem_read(GAME, 48)), r.trace


def resources(r, case):
    columns, rows = case
    r.wr(OBJ + 8, EXTRA)
    r.wr(EXTRA, OUT)
    r.wr(DATA + 0x5c, columns, rows)
    r.wr(DATA + 0x84, 9, 9, GAME)
    for i in range(columns * rows):
        r.wr(GAME + i * 48 + 0xc, 123, 456, [-1, 0, 1][i % 3])
    r.wr(OUT + 0x10, 0xabc, 0xdef)
    r.wr(OUT + 0x28, 0x123, 0x456)
    r.call("ConnectListBoxResources__17ResourceConnectorP9FEListBox", OBJ, DATA)
    return bytes(r.u.mem_read(DATA, 0xa8)), bytes(r.u.mem_read(GAME, columns * rows * 48))


def referenced(r, case):
    mode, owned = case
    r.wr(OBJ, DATA, EXTRA)
    r.wr(OBJ + 0x40, GAME, OUT)
    r.wr(GAME, VT)
    r.u.mem_write(EXTRA, struct.pack("<IIII", 0x4c62694c, 12, 1, 8) + b"lib\0")
    r.at("FindChild__15FEPackageReaderP7FEChunkUl", lambda: 0 if mode == "missing" else EXTRA)
    node, library, allocation = 0x10090000, 0x10091000, 0x10092000
    r.at("__builtin_new", lambda: node)
    r.at("__6FENode", lambda: r.rr(3))
    r.at("SetName__6FENodePCc", lambda: (r.tracecall("name", r.text(r.rr(4))), r.wr(node + 12, r.rr(4), 123))[1])
    add_list_stub(r)
    r.at("FindLibraryPackage__C7FEngineUl", lambda: library if mode == "existing" else 0)
    r.wr(library + 0x24, 7, 0xabcdef)
    def get_data():
        r.tracecall("data", r.text(r.rr(4)))
        r.wr(r.rr(5), allocation)
        r.wr(r.rr(6), owned)
        return 0 if mode == "no_data" else allocation
    r.slot(VT, 8, get_data)
    r.slot(VT, 0x28, lambda: r.tracecall("loaded", r.rr(4)))
    r.at("LoadPackage__7FEnginePCvb", lambda: (r.tracecall("load", r.rr(4), r.rr(5)), 0 if mode == "load_fail" else library)[1])
    r.at("__builtin_vec_delete", lambda: r.tracecall("delete", r.rr(3)))
    r.at("AddToLibraryList__7FEngineP9FEPackage", lambda: r.tracecall("library", r.rr(4)))
    ret = r.call("ReadReferencedPackagesChunk__15FEPackageReader", OBJ)
    return ret, bytes(r.u.mem_read(DATA, 0xb0)), bytes(r.u.mem_read(library, 0xb0)), r.trace


def message_targets(r, case):
    mode, targets = case
    r.wr(OBJ, DATA, EXTRA)
    tags = struct.pack("<HHI", 0x6354, 4, 1)
    if mode != "empty":
        tags += struct.pack("<HHI", 0x744d, 4 + 4 * targets, 123) + b"".join(struct.pack("<I", 10 + i) for i in range(targets))
    r.u.mem_write(EXTRA, struct.pack("<II", 0x67726154, len(tags)) + tags)
    r.at("FindChild__15FEPackageReaderP7FEChunkUl", lambda: 0 if mode == "missing" else EXTRA)
    blocks = allocation_stubs(r)
    r.at("Allocate__15FEMsgTargetListUl", lambda: r.tracecall("allocate", r.rr(3), r.rr(4)))
    r.at("FindObjectByGUID__9FEPackageUl", lambda: (r.tracecall("find", r.rr(4)), GAME + r.rr(4)*4)[1])
    r.at("AppendTarget__15FEMsgTargetListP8FEObject", lambda: r.tracecall("append", r.rr(3), r.rr(4)))
    ret = r.call("ReadMessageTargetListChunk__15FEPackageReader", OBJ)
    return ret, bytes(r.u.mem_read(DATA, 0xb0)), [bytes(r.u.mem_read(p, n)) for p, n in blocks], r.trace


def render(r, case):
    count, executing = case
    r.wr(OBJ + 4, executing)
    r.wr(OBJ + 0xf4, DATA if count else 0)
    r.wr(OBJ + 0x120, GAME)
    r.wr(GAME, VT)
    r.slot(VT, 0x88, lambda: r.tracecall("view"))
    r.slot(VT, 0x50, lambda: r.tracecall("begin", r.rr(4)))
    r.slot(VT, 0x58, lambda: r.tracecall("end", r.rr(4)))
    r.slot(VT, 0x70, lambda: r.tracecall("draw", r.rr(4), r.rr(5)))
    for i in range(count):
        package, object = DATA + i * 0x100, EXTRA + i * 0x200
        r.wr(package, package + 0x100 if i + 1 < count else 0)
        r.wr(package + 0x48, object)
        r.wr(object, object + 0x80)
        r.wr(object + 0x18, 5)
        r.wr(object + 0x80, 0)
        r.wr(object + 0x98, 1)
    r.at("Identify__9FEMatrix4", lambda: r.tracecall("identity"))
    r.at("SortObjects__t14FEObjectSorter1i1024", lambda: r.tracecall("sort", r.rr(3)))
    r.at("RenderGroup__7FEngineP7FEGroupR9FEMatrix4T2Us", lambda: r.tracecall("group", r.rr(4), r.rr(7)))
    r.at("RenderObject__7FEngineP8FEObjectR9FEMatrix4Us", lambda: r.tracecall("object", r.rr(4), r.rr(6)))
    r.call("Render__7FEngine", OBJ)
    return bytes(r.u.mem_read(OBJ, 0x5270)), r.trace


def release(r, count):
    r.u.mem_write(OBJ, b"\xa5" * 0x5270)
    r.wr(OBJ + 0x120, GAME)
    r.wr(OBJ + 0x18, EXTRA if count else 0)
    for i, offset in enumerate([0xf0, 0x100, 0x110, 0x4128, 0x4148]):
        r.wr(OBJ + offset, count, DATA + i * 0x400 if count else 0)
        for j in range(count):
            node = DATA + i * 0x400 + j * 0x80
            r.wr(node, node + 0x80 if j + 1 < count else 0, 0, VT)
    def pop():
        lst = r.rr(3)
        node = r.rd(lst + 4)
        if node:
            r.wr(lst + 4, r.rd(node))
        return node
    r.at("RemHead__9FEMinList", pop)
    r.at("RemovePackage__13FEPackageListP9FEPackage", pop)
    r.at("Shutdown__9FEPackageP15FEGameInterface", lambda: r.tracecall("shutdown", r.rr(3), r.rr(4)))
    r.at("__builtin_vec_delete", lambda: r.tracecall("delete_array", r.rr(3)))
    r.slot(VT, 8, lambda: r.tracecall("destroy", r.rr(3), r.rr(4)))
    copy_stubs(r)
    r.call("ReleaseAll__7FEngine", OBJ)
    return bytes(r.u.mem_read(OBJ, 0x5270)), r.trace


def type_fields(r, case):
    operation, count = case
    blocks = allocation_stubs(r)
    r.wr(OBJ + 0x18, DATA if count else 0)
    for i in range(count):
        node = DATA + i * 0x80
        r.wr(node, node + 0x80 if i + 1 < count else 0)
        r.wr(node + 12, EXTRA + i * 16)
        r.cstring(EXTRA + i * 16, "name" + str(i))
        r.wr(node + 0x18, (i + 1) * 4, 0xdead)
    r.cstring(OUT, "NAME1")
    if operation == "offsets":
        r.call("UpdateOffsets__10FETypeNode", OBJ)
    elif operation == "find":
        r.at("strcasecmp", lambda: (r.text(r.rr(3)).lower() > r.text(r.rr(4)).lower()) - (r.text(r.rr(3)).lower() < r.text(r.rr(4)).lower()))
        return r.call("GetField__10FETypeNodePCc", OBJ, OUT)
    elif operation == "default":
        r.wr(OBJ + 0x18, count * 4, 0, DATA)
        r.call("SetDefault__11FEFieldNodePv", OBJ, EXTRA)
    else:
        r.wr(OBJ + 0x14, count, DATA if count else 0, DATA + (count - 1) * 0x80 if count else 0)
        r.at("__6FENode", lambda: r.rr(3))
        r.at("SetName__6FENodePCc", lambda: r.tracecall("name", r.rr(3), r.text(r.rr(4))))
        add_list_stub(r)
        r.call("AddField__10FETypeNodePCcl", OBJ, OUT, 4)
    return bytes(r.u.mem_read(OBJ, 40)), bytes(r.u.mem_read(DATA, 0x200)), [bytes(r.u.mem_read(p, n)) for p, n in blocks], r.trace


def type_find(r, key):
    r.at("FindNode__C9FEMinListUl", lambda: (r.tracecall("find", r.rr(3), r.rr(4)), DATA)[1])
    return r.call("FindType__9FETypeLibUl", OBJ, key), r.trace


def group_copy(r, case):
    children, reference, count = case
    r.wr(DATA + 0x5c, EXTRA if count else 0)
    r.at("__8FEObjectRC8FEObjectb", lambda: (r.tracecall("base", r.rr(4), r.rr(5)), r.rr(3))[1])
    for i in range(count):
        r.wr(EXTRA + i * 0x80, EXTRA + (i + 1) * 0x80 if i + 1 < count else 0, 0, VT)
    r.slot(VT, 16, lambda: (r.tracecall("clone", r.rr(3), r.rr(4)), r.rr(3) + 0x2000)[1])
    add_list_stub(r)
    r.call("__7FEGroupRC7FEGroupbT2", OBJ, DATA, children, reference)
    return bytes(r.u.mem_read(OBJ, 0x68)), r.trace


@unittest.skipUnless(ppc.Uc, "Optional Unicorn dependency unavailable")
class FEngPackages(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        try:
            _, ppc.original = ppc.load_target()
        except (FileNotFoundError, ValueError):
            raise unittest.SkipTest("Pinned original unavailable")
        ppc.symbols = {s["name"]: s for s in ppc.original.symbols() if s["section"]}

    def cases(self, unit, cases, body):
        if not (native_directory(unit) / "compiled.elf").exists():
            self.skipTest("Build " + unit + " first")
        for case in cases:
            with self.subTest(case=case):
                compare(unit, case, body)

    def test_stricmp(self):
        self.cases("fe_stricmp", [("stricmp", a, b) for a, b in itertools.product([b"", b"menu", b"MENU", b"men", b"\xff"], repeat=2)], strings)

    def test_locale_numbers(self):
        self.cases("felocale_numbers", [("atoi", a, b) for a, b in itertools.product([b"", b"123x", b"-1", b" 7", b"4294967295"], [b"", b"x"])] + [("itoa", str(i).encode(), b"") for i in [0, 1, 9, 10, 12345, 999999999]], strings)

    def test_wide_helpers(self):
        self.cases("felocale_compare", [(op, a, b) for op, a, b in itertools.product(["upper", "prefix", "wide", "wide_invalid"], [b"", b"Menu", b"MENU", b"\x7f", b"\xff", b"\x80"], [b"", b"menu", b"MENU", b"\xff"])], strings)

    def test_length(self):
        self.cases("fe_wide_length", [("length", a, b) for a, b in itertools.product([b"", b"menu"], [b"", b"x"])], strings)

    def test_read_string(self):
        self.cases("felocale_readstring", [(s, o) for s in [b"", b"abc", b"abc\r\n\rnext", b"\n", b"a\x80\xff", b"\\65\\b", b"\\a12b3\\!", b"\\\\65\\", b"\\65537\\", b"\\unfinished"] for o in [0, len(s)//2, len(s)]], read_string)

    def test_copy_string(self):
        self.cases("felocale_copy_string", itertools.product([False, True], [None, "", "menu"]), copy_string)

    def test_event_count(self):
        self.cases("feevent_setcount", itertools.product([0, 1, 3], [0, 1, 2, 5]), event_count)

    def test_find_child(self):
        self.cases("fereader_findchild", itertools.product([[], [0], [0, 4, 12]], [9, 10, 11, 12, 99]), find_child)

    def test_read_types(self):
        self.cases("fereader_readtypes", itertools.product([False, True], [0, 8, 17, 32]), read_types)

    def test_branch(self):
        self.cases("feresponse_branch_target", [(ids, start) for ids in [[0x300], [0x300, 0, 0x500, 0x501], [0x301, 0x300, 0x500, 0x501, 0x501], [0x300, 0, 0]] for start in range(len(ids))], branch)

    def test_marker(self):
        self.cases("fengine_record_marker", [0, 1, 15, 16], marker)

    def test_close(self):
        self.cases("feobject_close", itertools.product([0, 1, -1, float("nan")], [0, 1, -1, float("nan")], [0, 0.5, 1, -1]), close)

    def test_color(self):
        self.cases("fecolor_subtract", itertools.product([(0, 1, 2, 3), (255, 128, 64, 0), (-1, -2, 300, -400)], repeat=2), color)

    def test_unload(self):
        self.cases("fengine_unload_library", itertools.product([0, 1], [0, 1], [False, True]), unload)

    def test_render_object(self):
        self.cases("fengine_render_object", itertools.product([0, 1], [-1, 0, 1, float("nan")]), render_object)

    def test_track_update(self):
        self.cases("fepackage_tracks", itertools.product([0, 1], [0, 1], [0, 4], [0, 0x80], [0, 0x1800000], [0, 3]), track_update)

    def test_messages(self):
        self.cases("fepackage_script_messages", [(*bounds, target, special, sender) for bounds, target, special, sender in itertools.product([(0, 0), (0, 5), (3, 10), (7, 2), (-1, 10)], [-1, -6, -5, -4, 0, 123], [False, True], [False, True])], messages)

    def test_move_to(self):
        self.cases("feobject_moveto", itertools.product(range(8), [0, 2, 3, 4, 5], [False, True], [False, True]), move_to)

    def test_set_track(self):
        self.cases("feobject_track_value", itertools.product([0, 1], [False, True]), set_track)

    def test_position(self):
        self.cases("feobject_position", itertools.product([-1, 0, 255, 256], [0, 1], [(0, 0, 0), (1, 2, 3), (0.0000001, 0, 0), (float("nan"), 0, 0)]), position)

    def test_script_copy(self):
        self.cases("fescript_copy", itertools.product([0, 1, 3], [0, 1]), script_copy)

    def test_object_constructors(self):
        self.cases("feobject_constructors", itertools.product([False, True], [0, 1, 2], [0, 1]), object_constructor)

    def test_list_tags(self):
        self.cases("fereader_list_tags", [(code, tag, first) for code, tags in [(False, [0, 0x644c, 0x634c, 0x724c, 0x6343, 0x5443, 0x7443, 0x774c, 0x734c, 0x764c, 0x7243, 0x7343, 0x6943]), (True, [0, 0x444c, 0x4953, 0x6343, 0x6a4c, 0x744c, 0x764c, 0x7343])] for tag, first in itertools.product(tags, [False, True])], reader_list_tag)

    def test_resources(self):
        self.cases("feresource_listbox", itertools.product([0, 1, 3], [0, 1, 2]), resources)

    def test_referenced(self):
        self.cases("fereader_referenced_packages", itertools.product(["missing", "existing", "no_data", "load_fail", "loaded"], [0, 1]), referenced)

    def test_message_targets(self):
        self.cases("fereader_message_targets", itertools.product(["missing", "empty", "targets"], [0, 1, 3]), message_targets)

    def test_render(self):
        self.cases("fengine_render", itertools.product([0, 1, 2], [0, 1]), render)

    def test_release(self):
        self.cases("fengine_release", [0, 1, 2], release)

    def test_type_fields(self):
        self.cases("fetypenode_fields", itertools.product(["offsets", "find", "default", "add"], [0, 1, 3]), type_fields)

    def test_type_find(self):
        self.cases("fetype_find", [0, 1, 0xabcdef01], type_find)

    def test_group_copy(self):
        self.cases("fegroup_copy", itertools.product([0, 1], [0, 1], [0, 1, 3]), group_copy)


if __name__ == "__main__":
    unittest.main()
