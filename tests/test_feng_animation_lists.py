"""Isolated original/native PPC checks for the PR64/65 follow-up batch.

Requires the pinned original, built units/comparisons and optional Unicorn.
External allocation, callbacks and transcendental functions are stubbed. These
checks compare memory, return values and call order, not the running game.
"""
import itertools
import math
import struct
import unittest
from pathlib import Path

import test_explosion_grenade as ppc
from test_shortlist_partials import Run

ROOT = Path(__file__).resolve().parents[1]
EXACT = {"feng_lerp_values", "feng_lerp_color", "feng_interp_linear_wrapper",
         "fecode_scroll_axes", "felistbox_ctor"}


def native_directory(unit):
    return (ROOT / "build/reconstruction/units" / unit if unit in EXACT else
            ROOT / "build/comparisons" / ("compare_" + unit))


def compare(unit, case, body):
    a = ppc.normalized(body(Run(), case))
    b = ppc.normalized(body(Run(native_directory(unit)), case))
    assert a == b, (unit, case, a, b)


OBJ, DATA, EXTRA, OUT, GAME, VT = [0x10020000 + i * 0x2000 for i in range(6)]


def copy_stubs(r):
    def copy():
        dst, src, size = r.rr(3), r.rr(4), r.rr(5)
        r.u.mem_write(dst, bytes(r.u.mem_read(src, size)))
        return dst
    r.at("memcpy", copy)
    r.at("memset", lambda: r.u.mem_write(r.rr(3), bytes([r.rr(4) & 255]) * r.rr(5)))


def track(r, reference=False, empty=False):
    r.u.mem_write(OBJ, b"\0" * 0x38)
    r.u.mem_write(OBJ, bytes([0, 16, 0, 0]))
    r.wr(OBJ + 4, 9 << 8)
    r.wr(OBJ + 0x14, -1)
    r.wr(OBJ + 0x18, 11, 12, 13, 14)
    nodes = [DATA + i * 0x40 for i in range(3)]
    for i, (node, time) in enumerate(zip(nodes, [2, 5, 9])):
        r.wr(node, nodes[i + 1] if i < 2 else 0, nodes[i - 1] if i else 0, 0, time)
        r.wr(node + 16, i + 21, i + 31, i + 41, i + 51)
    r.wr(OBJ + 0x28, 0, 0 if empty else nodes[0], 0 if empty else nodes[-1])
    if reference:
        r.wr(EXTRA, 0, 0 if empty else nodes[0], 0 if empty else nodes[-1])
        r.wr(OBJ + 0x28, 1, EXTRA, 0)
    return nodes


def lookup(r, case):
    which, time, reference, empty = case
    track(r, reference, empty)
    return r.call(which + "__10FEKeyTrackl", OBJ, time)


def insert(r, case):
    time, empty = case
    track(r, empty=empty)
    r.wr(EXTRA, 0xABADCAFE, 0xABADCAFE, 0, time, 99)
    r.call("InsertKey__10FEKeyTrackP9FEKeyNode", OBJ, EXTRA)
    return (bytes(r.u.mem_read(OBJ, 0x38)), bytes(r.u.mem_read(DATA, 0xC0)),
            bytes(r.u.mem_read(EXTRA, 32)))


def interp_none(r, case):
    time, empty = case
    track(r, empty=empty)
    copy_stubs(r)
    r.wr(OUT, *([0xAAAAAAAA] * 4))
    r.call("FEInterpNone__FP10FEKeyTracklPv", OBJ, time, OUT)
    return bytes(r.u.mem_read(OUT, 16))


def dispatch(r, case):
    entry, kind, cached, empty = case
    track(r, empty=empty)
    r.u.mem_write(OBJ + 2, bytes([kind, cached]))
    for callee in ["FEInterpNone__FP10FEKeyTracklPv", "FEInterpLinear__FP10FEKeyTracklPv",
                   "FEInterpNone__FP8FEScriptUclPv", "FEInterpLinear__FP8FEScriptUclPv"]:
        r.at(callee, lambda name=callee: r.tracecall(name, *[r.rr(i) for i in range(3, 7)]))
    if entry == "script":
        r.wr(EXTRA + 0x20, OBJ)
        r.wr(DATA + 0x2C, OUT)
        r.call("FEKeyInterp__FP8FEScriptUclP8FEObject", EXTRA, 0, 5, DATA)
    else:
        r.call(("FEKeyInterpFast" if entry == "fast" else "FEKeyInterp") +
               "__FP10FEKeyTracklPv", OBJ, 5, OUT)
    # The fourth argument is not live for the track overloads.
    return bytes(r.u.mem_read(OBJ, 4)), [(x[0], *x[1:4]) for x in r.trace]


def key_values(r, case):
    color, time = case
    track(r)
    r.u.mem_write(OBJ + 3, b"\xff")
    if color:
        r.wr(OBJ + 0x18, 20, 30, 40, 50)
        r.wr(EXTRA, 120, 130, 140, 150)
    else:
        r.wf(OBJ + 0x18, 1.5, 2.0, -3.0)
        r.wf(EXTRA, 4.5, -2.0, 7.0)
    def create():
        r.tracecall("create", r.rr(4))
        return OUT
    r.at("CreateKeyAt__10FEKeyTrackl", create)
    ret = r.call("SetKeyValueAt__10FEKeyTracklR" + ("7FEColor" if color else "9FEVector3"),
                 OBJ, time, EXTRA)
    return ret, bytes(r.u.mem_read(OBJ, 0x38)), bytes(r.u.mem_read(OUT, 32)), r.trace


def matrix(r, case):
    operation, alias, fractional = case
    copy_stubs(r)
    r.wf(DATA, *[(i % 7 - 3) * (0.17 if fractional else 0.25) for i in range(16)])
    r.wf(EXTRA, *[(i % 5 - 2) * (0.29 if fractional else 0.5) for i in range(16)])
    out = [OUT, DATA, EXTRA][alias]
    if operation == "matrix":
        r.call("FEMultMatrix__FP9FEMatrix4PC9FEMatrix4T1", out, DATA, EXTRA)
        return r.rf(out, 16)
    if operation == "vector":
        r.call("FEMultMatrix__FP9FEVector3PC9FEMatrix4PC9FEVector3", out, DATA, EXTRA)
        return r.rf(out)
    if operation == "quaternion":
        r.call("GetMatrix__12FEQuaternionP9FEMatrix4", EXTRA, out)
    else:
        r.call("Identify__9FEMatrix4", out)
    return r.rf(out, 16)


def quaternion(r, case):
    a, b, t = case
    r.wf(DATA, *a)
    r.wf(EXTRA, *b)
    r.wf(OBJ, 0.1, 0.2, 0.3, 0.9)
    def math_call(fn):
        def call():
            try:
                result = fn(r.fr(1))
            except ValueError:
                result = float("nan")
            r.fw(1, struct.unpack(">f", struct.pack(">f", result))[0])
        return call
    for name, fn in [("sqrtf", math.sqrt), ("acosf", math.acos), ("sinf", math.sin)]:
        r.at(name, math_call(fn))
    r.fw(1, t)
    r.call("FELerpQuaternion__FR12FEQuaternionT0fP12FEQuaternionT3", DATA, EXTRA, OBJ, OUT)
    return r.rf(OUT, 4)


def list_setup(r, mode="override"):
    r.u.mem_write(OBJ, b"\0" * 0xC4)
    r.wr(OBJ + 0x58, GAME if mode == "game" else 0, 3, 2, 8, 7, 5, 2, 1, 2, 1)
    r.wr(OBJ + 0x88, DATA)
    r.wr(GAME, VT)
    def fill():
        r.tracecall("fill", r.rr(5), r.rr(6))
        # Model a refill that actually changes a cell, without calling back into
        # the code under test. Subsequent shifts must preserve these writes.
        cell = (r.rr(5) * 3 + r.rr(6)) % 6
        r.wr(DATA + cell * 48, 10000 + r.rr(5) * 100 + r.rr(6))
    r.slot(VT, 0xA8, fill)
    if mode == "override":
        r.wr(OBJ + 0xBC, r.callback(0x80001800, fill), 0x1234)
    for i in range(12):
        r.wr(DATA + i * 48, *[100 * i + j for j in range(12)])
    copy_stubs(r)
    r.slot(VT, 0x40, lambda: r.tracecall("notify", r.rr(4), r.rr(6), r.rr(7)))
    return OBJ


def indices(r, case):
    axis, value, total, visible, current = case
    list_setup(r)
    r.wr(OBJ + (0x68 if axis == "Column" else 0x6C), total)
    r.wr(OBJ + (0x5C if axis == "Column" else 0x60), visible)
    r.wr(OBJ + (0x70 if axis == "Column" else 0x74), current)
    return r.call("GetReal" + axis + "__C13FECodeListBoxl", OBJ, value)


def current_from_target(r, case):
    flags, target, total, visible = case
    list_setup(r)
    r.wr(OBJ + 0x64, flags)
    return r.call("CalculateCurrentFromTarget__13FECodeListBoxUlUlUl", OBJ, target, total, visible)


def movement(r, case):
    which, flags, step, current, target, total, visible = case
    list_setup(r, "game")
    r.wr(OBJ + 0x64, flags)
    if which == "check":
        ret = r.call("CheckMovement__13FECodeListBoxlllll", OBJ, step, current, target, total, visible)
    else:
        r.wr(EXTRA, current, target)
        ret = r.call("MakeMove__13FECodeListBoxlRUlT2UlUl", OBJ, step, EXTRA, EXTRA + 4, total, visible)
    return ret, bytes(r.u.mem_read(EXTRA, 8)), r.trace


def code_scroll(r, case):
    horizontal, step, mode = case
    list_setup(r, mode)
    current = OBJ + (0x70 if horizontal else 0x74)
    target = current + 8
    total, visible = (7, 3) if horizontal else (5, 2)
    ret = r.call("ScrollSelection__13FECodeListBoxlRUlT2UlUlb", OBJ, step, current, target,
                 total, visible, int(horizontal))
    return ret, bytes(r.u.mem_read(OBJ, 0xC4)), bytes(r.u.mem_read(DATA, 6 * 48)), r.trace


def fill_cells(r, case):
    mode, columns, rows, totalcols, totalrows = case
    list_setup(r, mode)
    r.wr(OBJ + 0x5C, columns, rows)
    r.wr(OBJ + 0x68, totalcols, totalrows)
    r.call("FillAllCells__13FECodeListBox", OBJ)
    return r.trace


def list_scroll_setup(r):
    r.u.mem_write(OBJ, b"\0" * 0xA8)
    r.wr(OBJ + 0x5C, 4, 3)
    r.wf(OBJ + 0x64, 3, 2, 1, 1)
    r.wr(OBJ + 0x74, DATA, EXTRA)
    r.wf(OBJ + 0x7C, 2, 3)
    r.wr(OBJ + 0x84, 1, 1)
    r.wf(OBJ + 0x90, 1, 1, 1, 0, 0.5, -0.25)
    for ptr, n in [(DATA, 4), (EXTRA, 3)]:
        for i in range(n):
            r.wf(ptr + i * 12, 1, i)
    r.at("sqrtf", r.sqrt)
    r.at("CompleteScroll__9FEListBox", lambda: r.tracecall("complete"))


def list_scroll(r, case):
    flags, column, row = case
    list_scroll_setup(r)
    r.wr(OBJ + 0x58, flags)
    r.call("ScrollSelection__9FEListBoxll", OBJ, column, row)
    return (r.rd(OBJ + 0x58), r.rd(OBJ + 0x84), r.rd(OBJ + 0x88),
            r.rf(OBJ + 0x6C, 2), r.rf(OBJ + 0x90, 4), r.trace)


def list_update(r, case):
    scrolling, blink, dt = case
    list_scroll_setup(r)
    r.wr(OBJ + 0x58, 2 if scrolling else 0)
    r.wf(OBJ + 0xA0, blink, 0.25)
    r.wf(OBJ + 0x90, 2, 3, 0.6, 0.8)
    r.fw(1, dt)
    r.call("Update__9FEListBoxf", OBJ)
    return r.rf(OBJ + 0x6C, 2), r.rf(OBJ + 0xA0, 2), r.trace


def allocation_stubs(r):
    copy_stubs(r)
    blocks = []
    def alloc():
        size = r.rr(4)
        ptr = 0x10080000 + len(blocks) * 0x2000
        blocks.append((ptr, size))
        r.u.mem_write(ptr, b"\xa5" * max(1, size))
        r.tracecall("alloc", r.text(r.rr(3)), size, r.rr(5))
        return ptr
    r.at("DWI_alloc", alloc)
    r.at("__builtin_vec_delete", lambda: r.tracecall("delete", r.rr(3)))
    return blocks


def allocation_result(r, blocks, size=0xC4):
    return (bytes(r.u.mem_read(OBJ, size)),
            [bytes(r.u.mem_read(ptr, count)) for ptr, count in blocks], r.trace)


def initialize_cells(r, count):
    r.u.mem_write(DATA, b"\xa5" * (48 * 5))
    r.call("InitializeCell__9FEListBoxP13FEListBoxCellUl", DATA, count)
    return bytes(r.u.mem_read(DATA, 48 * 5))


def resize(r, case):
    axis, count, existing = case
    list_scroll_setup(r)
    r.wr(OBJ + 0x8C, OUT if existing else 0)
    r.u.mem_write(OUT, bytes(range(240)) * 4)
    blocks = allocation_stubs(r)
    r.call("SetNum" + axis + "__9FEListBoxUl", OBJ, count)
    return allocation_result(r, blocks, 0xA8)


def code_initialize(r, case):
    columns, rows, initialized, old_present = case
    list_setup(r)
    r.wr(OBJ + 0x64, 8 | int(initialized))
    if not old_present:
        r.wr(OBJ + 0x88, 0)
    for i in range(6):
        r.wr(DATA + i * 48 + 24, 2 if i % 2 else 1)
    r.at("DeallocateString__13FECodeListBoxPs", lambda: r.tracecall("release_string", r.rr(4)))
    blocks = allocation_stubs(r)
    r.call("Initialize__13FECodeListBoxUlUl", OBJ, columns, rows)
    return allocation_result(r, blocks)


def allocate_strings(r, case):
    count, length, old_present = case
    list_setup(r)
    r.wr(OBJ + 0x8C, 6, 8, 0, EXTRA if old_present else 0, OUT if old_present else 0)
    for i in range(6):
        r.wr(DATA + i * 48 + 24, 2 if i % 2 else 0)
        r.wr(DATA + i * 48 + 32, OUT + 16 * i)
        r.u.mem_write(OUT + 16 * i, struct.pack(">4H", 65 + i, 66 + i, 67 + i, 0))
    blocks = allocation_stubs(r)
    r.call("AllocateStrings__13FECodeListBoxUlUl", OBJ, count, length)
    return allocation_result(r, blocks), bytes(r.u.mem_read(DATA, 6 * 48))


def copy_properties(r, case):
    constructor, null_string = case
    list_setup(r)
    # The source and destination have distinct cell and string storage.
    r.u.mem_write(EXTRA, bytes(r.u.mem_read(OBJ, 0xC4)))
    r.wr(EXTRA + 0x88, DATA)
    r.wr(EXTRA + 0x8C, 6, 8, 0, 0, 0)
    r.wr(OBJ + 0x88, OUT)
    r.wr(OBJ + 0x98, 0x10060000, 0x10062000)
    for i in range(6):
        r.wr(DATA + i * 48 + 24, i % 3)
        r.wr(DATA + i * 48 + 32, 0 if null_string else 0x10064000)
    r.u.mem_write(0x10064000, struct.pack(">4H", 65, 66, 67, 0))
    r.u.mem_write(OUT, b"\0" * (6 * 48))
    blocks = allocation_stubs(r)
    def initialize():
        r.wr(OBJ + 0x5C, r.rr(4), r.rr(5))
        r.wr(OBJ + 0x88, OUT)
        r.tracecall("initialize", r.rr(4), r.rr(5))
    def alloc_strings():
        r.tracecall("strings", r.rr(4), r.rr(5))
    sequence = []
    def alloc_string():
        ptr = 0x10066000 + len(sequence) * 32
        sequence.append(ptr)
        return ptr
    r.at("Initialize__13FECodeListBoxUlUl", initialize)
    r.at("AllocateStrings__13FECodeListBoxUlUl", alloc_strings)
    r.at("AllocateString__13FECodeListBox", alloc_string)
    def base_copy():
        r.u.mem_write(r.rr(3), bytes(r.u.mem_read(r.rr(4), 0x58)))
        return r.rr(3)
    r.at("__8FEObjectRC8FEObjectb", base_copy)
    r.call("__13FECodeListBoxRC13FECodeListBoxb" if constructor else
           "CopyProperties__13FECodeListBoxRC13FECodeListBox", OBJ, EXTRA, 1)
    return (allocation_result(r, blocks), bytes(r.u.mem_read(OUT, 6 * 48)),
            [bytes(r.u.mem_read(ptr, 16)) for ptr in sequence])


def track_assign(r, case):
    reference, empty, old_present = case
    nodes = track(r, reference, empty)
    r.u.mem_write(OUT, b"\0" * 0x38)
    if old_present:
        r.wr(OUT + 0x28, 0, 0x10060000, 0x10060000)
        r.wr(0x10060000, 0, 0, VT)
        r.slot(VT, 8, lambda: r.tracecall("destroy", r.rr(3), r.rr(4)))
    blocks = []
    def alloc():
        ptr = 0x10080000 + len(blocks) * 0x40
        blocks.append(ptr)
        r.u.mem_write(ptr, b"\xa5" * 32)
        return ptr
    r.at("__nw__9FEKeyNodeUi", alloc)
    r.call("__as__10FEKeyTrackR10FEKeyTrack", OUT, OBJ)
    return (bytes(r.u.mem_read(OUT, 0x38)), [bytes(r.u.mem_read(p, 32)) for p in blocks],
            bytes(r.u.mem_read(OBJ, 0x38)), r.trace)


def select_callback(r, blink):
    list_setup(r)
    r.wf(OBJ + 0xA0, blink)
    r.wr(OBJ + 0xA8, 11, 22, 33, 44)
    r.call("DefaultSelectCallback__13FECodeListBoxP13FECodeListBox", OBJ)
    return bytes(r.u.mem_read(OBJ + 0xA8, 16))


@unittest.skipUnless(ppc.Uc, "Optional Unicorn dependency unavailable")
class FEngAnimationLists(unittest.TestCase):
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

    def test_lookup(self):
        self.cases("fekeytrack_lookup", itertools.product(
            ["GetKeyAt", "GetDeltaKeyAt"], [-1, 0, 2, 4, 5, 9, 12], [False, True], [False, True]), lookup)

    def test_insert(self):
        self.cases("fekeytrack_lookup", itertools.product([-1, 2, 4, 5, 12], [False, True]), insert)

    def test_step_interpolation(self):
        self.cases("feng_interp_none", itertools.product([-1, 0, 2, 4, 5, 9, 12], [False, True]), interp_none)

    def test_dispatch(self):
        self.cases("feng_interp_dispatch", itertools.product(
            ["script", "track", "fast"], [0, 1, 2, 3, 4, 255], [0, 128], [False, True]), dispatch)

    def test_set_key_values(self):
        # CreateKeyAt is required to succeed; the original dereferences its result.
        self.cases("fekeytrack_set_values", itertools.product([False, True], [-1, 0, 2, 7]), key_values)

    def test_matrices(self):
        self.cases("feng_matrix", itertools.product(["matrix", "vector", "quaternion", "identity"],
            [0, 1, 2], [False, True]), matrix)

    def test_quaternion_lerp(self):
        self.cases("feng_lerp_quaternion", [
            ((0, 0, 0, 1), b, t) for b, t in itertools.product(
                [(0, 0, 0, 1), (0, 0, 0, -1), (0.6, 0, 0, 0.8),
                 (-0.6, 0, 0, -0.8), (float("nan"), 0, 0, 1)], [0, 0.25, 0.5, 1])], quaternion)

    def test_indices(self):
        self.cases("fecode_indices", itertools.product(["Column", "Row"],
            [-21, -7, -3, -1, 0, 2, 7, 14], [0, 1, 7], [1, 3], [0, 2]), indices)

    def test_current_from_target(self):
        self.cases("fecode_current_from_target", itertools.product([0, 8], [0, 1, 6, 9], [1, 7], [1, 4]), current_from_target)

    def test_movement(self):
        self.cases("fecode_movement", itertools.product(["check", "move"],
            [0, 2, 4, 6, 8], [-7, -1, 0, 1, 7], [0, 2], [0, 2], [7], [3, 7]), movement)

    def test_code_scroll(self):
        self.cases("fecode_scroll_selection", itertools.product([False, True], [-2, -1, 0, 1, 2],
            ["override", "game", "none"]), code_scroll)

    def test_fill_cells(self):
        self.cases("fecode_fill_cells", [(mode, *shape) for mode, shape in itertools.product(
            ["override", "game", "none"], [(3, 2, 7, 5), (5, 4, 2, 3), (0, 2, 7, 5), (3, 2, 0, 5)])], fill_cells)

    def test_list_scroll(self):
        self.cases("felistbox_scroll", [(flags, *step) for flags, step in itertools.product(
            [0, 4, 0x20, 0x40, 0x60], [(-1, 0), (1, 0), (3, 0), (0, -1), (0, 1), (0, 2), (1, 1), (0, 0)])], list_scroll)

    def test_list_update(self):
        self.cases("felistbox_update", itertools.product([False, True],
            [-1, 0, 0.5, 1, 2, float("nan")], [0, 0.25, 3]), list_update)

    def test_initialize_cells(self):
        self.cases("felistbox_initialize_cells", [0, 1, 4], initialize_cells)

    def test_resize(self):
        self.cases("felistbox_resize", itertools.product(["Columns", "Rows"], [0, 1, 3, 5], [False, True]), resize)

    def test_code_initialize(self):
        self.cases("fecode_initialize", itertools.product([1, 3, 4], [1, 2, 3], [False, True], [False, True]), code_initialize)

    def test_allocate_strings(self):
        self.cases("fecode_allocate_strings", itertools.product([0, 6, 8], [0, 2, 8], [False, True]), allocate_strings)

    def test_copy_properties(self):
        self.cases("fecode_copy", itertools.product([False, True], [False, True]), copy_properties)

    def test_track_assignment(self):
        self.cases("fekeytrack_assign", itertools.product([False, True], [False, True], [False, True]), track_assign)

    def test_select_callback(self):
        self.cases("fecode_select_callback", [-1, 0, 0.25, 0.5, 1, 2], select_callback)


if __name__ == "__main__":
    unittest.main()
