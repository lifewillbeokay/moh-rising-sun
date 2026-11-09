"""Original/native PPC checks for the Frontline-derived collision batch.

The pinned original and optional Unicorn 2.1.4 are required. sqrtf and contact
collection are controlled callbacks; nearest-point and segment-distance callees
execute original code. No game runtime claim. Integer state compares exactly;
float results use 3e-5 absolute/relative tolerance. Capsule intersections have
an explicitly narrower contract: see check_outputs and the research notes.
"""

import itertools
import json
import math
import random
import sys
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
import test_explosion_grenade as ppc
from test_shortlist_partials import Run

UNITS = (
    "cdb_box_ctor",
    "cdb_box_sphere",
    "cdb_box_line",
    "cdb_triangle_line",
    "cdb_triangle_box",
    "cdb_triangle_sphere",
    "cdb_box_triangle",
    "cdb_triangle_capsule",
    "box_plane_partial",
    "box_visibility",
    "box_box_partial",
    "cdb_separate1",
    "cdb_separate2",
    "cdb_separate3",
    "cdb_boxline_axis",
    "box_box_axis",
)
EXACT = {"cdb_separate1", "cdb_separate2", "cdb_separate3"}


def native(unit):
    return (
        ROOT
        / "build"
        / ("reconstruction/units" if unit in EXACT else "comparisons")
        / (unit if unit in EXACT else "compare_" + unit)
    )


def initialize():
    if ppc.Uc is None:
        raise unittest.SkipTest("Optional Unicorn is unavailable")
    try:
        _, ppc.original = ppc.load_target()
    except (FileNotFoundError, ValueError):
        raise unittest.SkipTest("Pinned original is unavailable")
    ppc.symbols = {s["name"]: s for s in ppc.original.symbols() if s["section"]}
    if any(not (native(n) / "compiled.elf").is_file() for n in UNITS):
        raise unittest.SkipTest("Local native collision output is unavailable")


A = 0x10020000
B = 0x10021000
C = 0x10022000
T = 0x10023000
NAMES = {
    n: json.loads(
        (
            ROOT
            / "config/GR8E69"
            / ((n + ".json") if n in EXACT else ("comparisons/" + n + ".json"))
        ).read_text()
    )["functions"][0]["name"]
    for n in UNITS
}


def v(r, p, a):
    r.wf(p, *a, 1)


def box(r, p, center, half=(1, 1, 1), axes=((1, 0, 0), (0, 1, 0), (0, 0, 1))):
    r.wr(p, ppc.symbols["_vt.7CVolBox"]["address"])
    r.wf(p + 4, *half)
    for i, a in enumerate((*axes, center)):
        v(r, p + 16 * (i + 1), a)


def line(r, p, start, end):
    v(r, p, start)
    v(r, p + 16, end)
    v(r, p + 32, [e - s for e, s in zip(end, start)])
    r.wf(p + 48, 99, 88)
    r.wr(p + 56, 0, 0)


def run(r, unit, case):
    r.trace = []
    r.hooks = {}
    r.u.mem_write(A, b"\xa5" * 0x5000)
    r.wr(C, *[0] * 9)
    r.wr(C + 24, case.get("result", 0))
    r.wr(C + 16, case.get("line", 0) << 31)
    r.wr(T, 0, 0, 0, 7)
    r.at("sqrtf", r.sqrt)
    r.at("IsSeeThru__12CSurfaceTypeUsb", lambda: case.get("see", 0))

    def contact():
        p = r.rr(4)
        result = [
            r.rf(p),
            r.rf(p + 16) if unit != "cdb_triangle_box" else (),
            r.rf(p + 32, 1)[0],
            r.rd(p + 36),
        ]
        r.trace.append(result)
        if unit == "box_box_partial" and hasattr(r, "axis_pointer"):
            r.covered_axes.add(r.rd(r.axis_pointer))

    r.at("AddContact__10CCollisionRC12SClsnContact", contact)
    flag = case.get("flag", 1)
    if unit in ("cdb_box_ctor",):
        r.wf(
            B,
            *case["half"],
            *[x for row in (*case["axes"], case["center"]) for x in row]
        )
        r.call(NAMES[unit], A, B)
        return bytes(r.u.mem_read(A, 80))
    if unit.startswith("cdb_triangle_"):
        for i, a in enumerate(case["tri"]):
            v(r, A + i * 16, a)
        if unit.endswith("sphere"):
            r.wf(B + 12, case["radius"])
            v(r, B + 16, case["center"])
        elif unit.endswith("box"):
            box(r, B, case["center"], case["half"], case["axes"])
        elif unit.endswith("line"):
            line(r, B, case["start"], case["end"])
        else:
            r.wr(B, ppc.symbols["_vt.11CVolCapsule"]["address"])
            r.wf(B + 12, case["radius"])
            line(r, B + 16, case["start"], case["end"])
        ret = r.call(NAMES[unit], A, B, C, T, flag)
    elif unit.startswith("cdb_box_") or unit.startswith("box_"):
        box(r, A, case["center"], case["half"], case["axes"])
        if unit == "cdb_box_sphere":
            r.wf(B + 12, case["radius"])
            v(r, B + 16, case["other"])
        elif unit == "cdb_box_line":
            line(r, B, case["start"], case["end"])
        elif unit == "cdb_box_triangle":
            for i, a in enumerate(case["tri"]):
                v(r, B + i * 16, a)
        elif unit == "box_box_partial":
            box(r, B, case["other"], case["other_half"], case["other_axes"])
        elif unit == "box_plane_partial":
            v(r, B, case["normal"])
            r.wf(B + 16, case["distance"])
        elif unit == "box_visibility":
            r.wr(B + 212, T)
            r.wr(B + 256, 0xAABBCCDD)
            for i, (normal, distance) in enumerate(case["planes"]):
                v(r, T + 368 + i * 32, normal)
                r.wf(T + 384 + i * 32, distance)
            ret = r.call(NAMES[unit], A, B)
            return ret, r.rd(B + 256)
        ret = r.call(NAMES[unit], A, B, C, flag)
    return (
        ret,
        r.rd(C + 24),
        r.rf(C + 12, 1),
        r.trace,
        (
            bytes(
                r.u.mem_read(B + (16 if unit == "cdb_triangle_capsule" else 0) + 48, 16)
            )
            if unit in ("cdb_triangle_line", "cdb_triangle_capsule", "cdb_box_line")
            else ()
        ),
    )


def eq(a, b):
    if type(a) != type(b):
        return False
    if isinstance(a, float):
        return (
            (math.isnan(a) and math.isnan(b))
            or a == b
            or math.isclose(a, b, rel_tol=3e-5, abs_tol=3e-5)
        )
    if isinstance(a, (tuple, list)):
        return len(a) == len(b) and all(eq(x, y) for x, y in zip(a, b))
    return a == b


def axes(rng):
    q = [rng.uniform(-1, 1) for _ in range(4)]
    n = math.sqrt(sum(x * x for x in q))
    w, x, y, z = [v / n for v in q]
    return (
        (1 - 2 * (y * y + z * z), 2 * (x * y + w * z), 2 * (x * z - w * y)),
        (2 * (x * y - w * z), 1 - 2 * (x * x + z * z), 2 * (y * z + w * x)),
        (2 * (x * z + w * y), 2 * (y * z - w * x), 1 - 2 * (x * x + y * y)),
    )


def cases(count):
    rng = random.Random(6162)
    for i in range(count):
        base = dict(
            center=tuple(rng.uniform(-2, 2) for _ in range(3)),
            half=tuple(rng.uniform(0.2, 3) for _ in range(3)),
            axes=axes(rng),
            other=tuple(rng.uniform(-2, 2) for _ in range(3)),
            other_half=tuple(rng.uniform(0.2, 3) for _ in range(3)),
            other_axes=axes(rng),
            tri=tuple(tuple(rng.uniform(-3, 3) for _ in range(3)) for j in range(3)),
            start=tuple(rng.uniform(-3, 3) for _ in range(3)),
            end=tuple(rng.uniform(-3, 3) for _ in range(3)),
            radius=rng.uniform(0.01, 3),
            normal=axes(rng)[0],
            distance=rng.uniform(-3, 3),
            flag=i % 2,
            line=(i // 2) % 2,
            see=(i // 4) % 2,
            result=(i // 8) % 4,
        )
        base["planes"] = tuple((axes(rng)[0], rng.uniform(-5, 5)) for _ in range(6))
        yield base


def crosses_triangle(case):
    """Independent double-precision intersection check for the ill-conditioned regime."""

    def sub(a, b):
        return [x - y for x, y in zip(a, b)]

    def dot(a, b):
        return sum(x * y for x, y in zip(a, b))

    def cross(a, b):
        return [
            a[1] * b[2] - a[2] * b[1],
            a[2] * b[0] - a[0] * b[2],
            a[0] * b[1] - a[1] * b[0],
        ]

    a, b, c = case["tri"]
    e0 = sub(b, a)
    e1 = sub(c, a)
    direction = sub(case["end"], case["start"])
    p = cross(direction, e1)
    det = dot(e0, p)
    if abs(det) < 1e-12:
        return False
    offset = sub(case["start"], a)
    u = dot(offset, p) / det
    q = cross(offset, e0)
    v = dot(direction, q) / det
    t = dot(e1, q) / det
    return 0 <= u <= 1 and 0 <= v and u + v <= 1 and 0 <= t <= 1


def check_outputs(test, unit, case, original, native_result):
    if unit != "cdb_triangle_capsule" or not crosses_triangle(case):
        test.assertTrue(
            eq(original, native_result), (unit, case, original, native_result)
        )
        return
    # Known partial limitation: the distance quadratic cancels at an intersection.
    # Do not assert equivalence of normals or separations in that regime. Check
    # classification, contact positions and untouched state, and independently
    # bound both implementations' error from the geometric zero-distance answer.
    a, b = original, native_result
    test.assertEqual(a[:2], b[:2])
    test.assertEqual(a[2], b[2])
    test.assertEqual(a[4], b[4])
    test.assertEqual(len(a[3]), len(b[3]))
    for x, y in zip(a[3], b[3]):
        test.assertTrue(eq(x[1], y[1]), (unit, case, x, y))
        test.assertEqual(x[3], y[3])
        for contact in (x, y):
            test.assertLessEqual(abs(contact[2] + case["radius"]), 0.003)
            magnitude = math.sqrt(sum(v * v for v in contact[0]))
            test.assertTrue(magnitude == 0 or abs(magnitude - 1) < 3e-5)


def observe_box_axes(r):
    r.covered_axes = set()
    address = ppc.symbols["BoxBoxTest__Fff5EAxisRfT3R5EAxis"]["address"]

    def observe(u, a, size, data):
        r.axis_pointer = r.rr(6)

    r.u.hook_add(ppc.UC_HOOK_CODE, observe, begin=address, end=address)


class CollisionBatchTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        initialize()

    def test_randomized_complete_volume_bodies(self):
        for unit in UNITS[:11]:
            runners = (Run(), Run(native(unit)))
            if unit == "box_box_partial":
                for r in runners:
                    observe_box_axes(r)
            for case in cases(2000):
                a, b = (run(r, unit, case) for r in runners)
                check_outputs(self, unit, case, a, b)
            if unit == "box_box_partial":
                self.assertEqual(runners[0].covered_axes, set(range(1, 16)))
                self.assertEqual(runners[1].covered_axes, runners[0].covered_axes)

    def test_contact_boundaries_and_degenerate_geometry(self):
        base = next(cases(1))
        base.update(
            center=(0, 0, 0),
            half=(1, 1, 1),
            axes=((1, 0, 0), (0, 1, 0), (0, 0, 1)),
            other_half=(1, 1, 1),
            other_axes=((1, 0, 0), (0, 1, 0), (0, 0, 1)),
            normal=(0, 0, 1),
            tri=((-2, -2, 0), (2, -2, 0), (0, 2, 0)),
            radius=0.5,
            result=0,
            see=0,
        )
        for unit in UNITS[:11]:
            runners = (Run(), Run(native(unit)))
            for distance, flag, line_flag in itertools.product(
                (-3.0, -1.0, -0.5, -1e-6, 0.0, 1e-6, 0.5, 1.0, 3.0), (0, 1), (0, 1)
            ):
                case = dict(
                    base,
                    other=(distance, 0, 0),
                    center=(0, 0, distance),
                    distance=distance,
                    start=(0.25, 0.25, -1),
                    end=(0.25, 0.25, 1),
                    flag=flag,
                    line=line_flag,
                )
                a, b = (run(r, unit, case) for r in runners)
                check_outputs(self, unit, case, a, b)
            if unit in ("cdb_triangle_line", "cdb_triangle_capsule", "cdb_box_line"):
                for start, end in (
                    ((0, 0, 1), (0, 0, 1)),
                    ((-3, 0, 1), (3, 0, 1)),
                    ((-3, 0, 0), (3, 0, 0)),
                    ((0, 0, -1), (0, 0, 0)),
                ):
                    for flag in (0, 1):
                        case = dict(
                            base,
                            start=start,
                            end=end,
                            flag=flag,
                            other=(0, 0, 0),
                            distance=0.0,
                        )
                        a, b = (run(r, unit, case) for r in runners)
                        check_outputs(self, unit, case, a, b)

    def test_separating_axis_helpers(self):
        values = (-3.0, -0.0, 0.0, 0.1, 2.0, float("nan"), float("inf"), -float("inf"))
        for unit, count in (
            ("cdb_separate1", 1),
            ("cdb_separate2", 2),
            ("cdb_separate3", 3),
        ):
            runners = (Run(), Run(native(unit)))
            for params in itertools.product(values, repeat=count):
                for best in (-10.0, -1.0, 0.0, 3.4028234663852886e38):
                    outputs = []
                    for r in runners:
                        for i, value in enumerate((1.0, *params), 1):
                            r.fw(i, value)
                        r.wf(A, best)
                        r.wr(B, 99)
                        ret = r.call(NAMES[unit], 7, A, B)
                        outputs.append((ret, r.rf(A, 1), r.rd(B)))
                    self.assertTrue(eq(*outputs), (unit, params, best, outputs))

    def test_box_axis_helper(self):
        runners = (Run(), Run(native("box_box_axis")))
        for extent, projection, best in itertools.product(
            (0.0, 1.0, 3.0),
            (-4.0, -1.0, -0.0, 0.0, 0.5, 1.0, 4.0, float("nan")),
            (-10.0, 0.0, 3.4028234663852886e38),
        ):
            outputs = []
            for r in runners:
                r.fw(1, extent)
                r.fw(2, projection)
                r.wf(A, best, 17.0)
                r.wr(B, 99)
                ret = r.call(NAMES["box_box_axis"], 7, A, A + 4, B)
                outputs.append((ret, r.rf(A, 2), r.rd(B)))
            self.assertTrue(eq(*outputs), (extent, projection, best, outputs))

    def test_box_line_axis_clipping(self):
        runners = (Run(), Run(native("cdb_boxline_axis")))
        for offset, direction, near, far in itertools.product(
            (-2.0, -1.0, 0.0, 1.0, 2.0),
            (-2.0, -1e-6, 0.0, 1e-6, 2.0),
            (-3.0, 0.0, 2.0),
            (-1.0, 1.0, 3.0),
        ):
            outputs = []
            for r in runners:
                v(r, A, (1, 0, 0))
                v(r, B, (offset, 0, 0))
                v(r, T, (direction, 1, 0))
                r.wf(C, near, far)
                r.fw(1, 1.0)
                ret = r.call(NAMES["cdb_boxline_axis"], A, B, T, C, C + 4)
                outputs.append((ret, r.rf(C, 2)))
            self.assertTrue(eq(*outputs), (offset, direction, near, far, outputs))
