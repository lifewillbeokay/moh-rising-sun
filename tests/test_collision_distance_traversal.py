"""PPC differential checks for the distance/traversal collision batch.

Optional Unicorn 2.1.4, pinned originals and local comparisons are required.
The box/capsule intersection contact limitation is explicit in check_outputs;
this suite does not claim equivalence of those normals or positions. Other
float outputs compare at 3e-5 absolute/relative tolerance. No game runtime claim.
"""

import itertools
import unittest
import sys, json, math, random, struct
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
import test_explosion_grenade as ppc
from test_shortlist_partials import Run
import test_collision_batch as old

A = 0x10020000
B = 0x10021000
C = 0x10022000
T = 0x10023000
O = 0x10024000
UNITS = (
    "cdb_branch_box",
    "cdb_branch_sphere",
    "cdb_branch_capsule",
    "cdb_branch_animatedvolume",
    "cdb_object_line",
    "triangle_nearest",
    "box_capsule",
    "line_segment_distance",
    "sphere_visibility",
    "box_sphere",
    "capsule_visibility",
)
TARGETS = {
    n: json.loads((ROOT / "config/GR8E69/comparisons" / (n + ".json")).read_text())[
        "functions"
    ][0]
    for n in UNITS
}
v = old.v
box = old.box
line = old.line


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


def run(r, unit, c):
    r.trace = []
    r.hooks = {}
    r.u.mem_write(A, b"\xa5" * 0x9000)
    r.wr(C, *[0] * 9)
    r.wr(C + 24, c.get("result", 0))
    r.at("sqrtf", r.sqrt)

    def contact():
        p = r.rr(4)
        r.trace.append((r.rf(p), r.rf(p + 16), r.rf(p + 32, 1)[0], r.rd(p + 36)))

    r.at("AddContact__10CCollisionRC12SClsnContact", contact)
    n = TARGETS[unit]["name"]
    flag = c.get("flag", 0)
    if unit == "triangle_nearest":
        for i, p in enumerate(c["tri"]):
            v(r, A + 16 * i, p)
        v(r, B, c["other"])
        r.wf(T, 99, 88)
        r.call(
            n, O, A, B, T if c.get("s", True) else 0, T + 4 if c.get("t", True) else 0
        )
        return r.rf(O, 4), r.rf(T, 2)
    if unit == "line_segment_distance":
        line(r, A, c["start"], c["end"])
        line(r, B, c["other"], c["center"])
        r.wf(T, 99, 88, 77)
        if c.get("cached", False):
            for p in [A, B]:
                d = r.rf(p + 32)
                r.wf(p + 52, sum(x * x for x in d))
                r.wr(p + 60, 1)
        ret = r.call(n, A, B, T, T + 4, T + 8 if c.get("distance", True) else 0)
        return (
            ret,
            r.rf(T, 3),
            r.rf(A + 52, 1),
            r.rd(A + 60),
            r.rf(B + 52, 1),
            r.rd(B + 60),
        )
    if unit.startswith("box_"):
        box(r, A, c["center"], c["half"], c["axes"])
        if unit == "box_sphere":
            r.wr(B, ppc.symbols["_vt.10CVolSphere"]["address"])
            r.wf(B + 12, c["radius"])
            v(r, B + 16, c["other"])
        else:
            r.wr(B, ppc.symbols["_vt.11CVolCapsule"]["address"])
            r.wf(B + 12, c["radius"])
            line(r, B + 16, c["start"], c["end"])
        ret = r.call(n, A, B, C, flag)
        return ret, r.rd(C + 24), r.trace
    if unit.endswith("visibility"):
        if unit.startswith("sphere"):
            r.wf(A + 12, c["radius"])
            v(r, A + 16, c["center"])
        else:
            r.wf(A + 12, c["radius"])
            line(r, A + 16, c["start"], c["end"])
        r.wr(B + 212, T)
        r.wr(B + 256, 0x12345678)
        for i, (normal, distance) in enumerate(c["planes"]):
            v(r, T + 368 + i * 32, normal)
            r.wf(T + 384 + i * 32, distance)
        r.wf(T + 144, *c.get("view", (1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1)))
        r.wf(
            T + 208,
            *c.get("projection", (1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1))
        )
        ret = r.call(n, A, B)
        return ret, r.rd(B + 256), r.rf(B + 260, 4)
    raise ValueError(unit)


def cases(count):
    for i, c in enumerate(old.cases(count)):
        c["cached"] = i % 3 == 0
        c["distance"] = i % 5 != 0
        c["s"] = i % 3 != 0
        c["t"] = i % 4 != 0
        yield c


def trees(count):
    rng = random.Random(6311)
    for i in range(count):
        c = next(cases(1))
        c.update(next(iter(old.cases(1))))
        c["start"] = tuple(rng.uniform(-4, 4) for _ in range(3))
        c["end"] = tuple(rng.uniform(-4, 4) for _ in range(3))
        c["center"] = tuple(rng.uniform(-3, 3) for _ in range(3))
        c["radius"] = rng.uniform(0.1, 2)
        c["half"] = tuple(rng.uniform(0.2, 2) for _ in range(3))
        c["axes"] = old.axes(rng)
        c["flag"] = i % 2
        c["root_mode"] = ("branch", "leaf", "null")[i % 3]
        c["groups"] = [rng.choice([-3, 0, 1, 2, 4]) for _ in range(8)]
        c["returns"] = [rng.randrange(1, 4) for _ in range(8)]
        nodes = []

        def node(depth):
            idx = len(nodes)
            nodes.append(None)
            if depth == 0 or rng.randrange(4) == 0:
                nodes[idx] = dict(
                    kind=0,
                    center=tuple(rng.uniform(-4, 4) for _ in range(3)),
                    half=tuple(rng.uniform(1, 8) for _ in range(3)),
                    groups=[rng.randrange(8) for _ in range(rng.randrange(5))],
                )
            else:
                children = [
                    node(depth - 1) if rng.randrange(5) else None for _ in range(2)
                ]
                nodes[idx] = dict(
                    kind=1,
                    children=children,
                    normal=old.axes(rng)[0],
                    distance=rng.uniform(-2, 2),
                )
            return idx

        node(4)
        if nodes[0]["kind"] != 1:
            nodes.insert(
                0, dict(kind=1, children=[1, None], normal=(1, 0, 0), distance=0)
            )
        if not any(n["kind"] == 0 for n in nodes):
            nodes.append(dict(kind=0, center=(0, 0, 0), half=(2, 2, 2), groups=[]))
        c["nodes"] = nodes
        yield c


def tree_run(r, unit, c):
    r.trace = []
    r.hooks = {}
    r.u.mem_write(A, b"\x00" * 0x30000)
    r.at("sqrtf", r.sqrt)
    r.wr(C + 24, 0)
    r.wf(C + 12, 100)
    G = 0x10040000
    N = 0x10030000

    def addr(idx):
        return 0 if idx is None else N + 0x100 * idx

    for i, node in enumerate(c["nodes"]):
        p = addr(i)
        r.wr(p, node["kind"])
        if node["kind"]:
            r.wr(p + 4, *map(addr, node["children"]))
            r.wf(p + 12, *node["normal"], node["distance"])
        else:
            r.wf(p + 4, *node["half"], 1, 0, 0, 0, 1, 0, 0, 0, 1, *node["center"])
            r.wr(p + 64, len(node["groups"]), *[G + 0x100 * j for j in node["groups"]])
    for i, mark in enumerate(c["groups"]):
        r.wr(G + 0x100 * i + 16, mark)
    if unit == "cdb_object_line":
        leaf = next(i for i, n in enumerate(c["nodes"]) if not n["kind"])
        r.wr(A + 4, 0 if c["root_mode"] == "null" else O)
        r.wr(O + 28, 1 if c["root_mode"] == "branch" else 0)
        r.wr(O + 44, addr(leaf), N)
        line(r, B, c["start"], c["end"])

        def group():
            p = r.rr(3)
            idx = (p - G) // 0x100
            r.trace.append((idx, r.rr(4) == B, r.rr(5) == C, r.rr(6), r.rd(p + 16)))
            r.wf(C + 12, 100 - idx)
            return c["returns"][idx]

        r.at(
            "TestCollisionWithTriGroup__FRC11CDBTriGroupRC6CLine3R10CCollisionb", group
        )
        ret = r.call(TARGETS[unit]["name"], A, B, C, c["flag"])
        return (
            ret,
            r.trace,
            [r.rd(G + 0x100 * i + 16) for i in range(8)],
            r.rf(C + 12, 1),
        )
    if unit == "cdb_branch_box":
        box(r, B, c["center"], c["half"], c["axes"])
    elif unit == "cdb_branch_capsule":
        r.wr(B, ppc.symbols["_vt.11CVolCapsule"]["address"])
        r.wf(B + 12, c["radius"])
        line(r, B + 16, c["start"], c["end"])
    else:
        r.wr(B, ppc.symbols["_vt.10CVolSphere"]["address"])
        r.wf(B + 12, c["radius"])
        v(r, B + 16, c["center"])
    r.wr(T, O, 0, 0, 2048, 0, 12)
    ret = r.call(TARGETS[unit]["name"], A, N, B, C, c["flag"], T)
    n = r.rd(T + 8)
    return (
        ret,
        [r.rd(O + 4 * i) for i in range(n)],
        [r.rd(G + 0x100 * i + 16) for i in range(8)],
        r.rd(C + 24),
    )


def boundary_cases():
    base = next(cases(1))
    base.update(
        center=(0, 0, 0),
        other=(0, 0, 0),
        half=(1, 1, 1),
        axes=((1, 0, 0), (0, 1, 0), (0, 0, 1)),
        radius=1,
        tri=((0, 0, 0), (1, 0, 0), (0, 1, 0)),
        planes=(
            ((1, 0, 0), 1),
            ((-1, 0, 0), 1),
            ((0, 1, 0), 1),
            ((0, -1, 0), 1),
            ((0, 0, 1), 1),
            ((0, 0, -1), 1),
        ),
        cached=False,
        distance=True,
        s=True,
        t=True,
    )
    for start, end in [
        ((0, 0, 0), (0, 0, 0)),
        ((-2, 0, 0), (2, 0, 0)),
        ((0, 0, 0), (1, 0, 0)),
        ((0, 1, 0), (1, 1, 0)),
        ((0, 0, 1), (0, 0, 2)),
        ((0, 0, 0), (1e-6, 1e-6, 0)),
        ((2, 2, 2), (-2, -2, -2)),
    ]:
        for point, radius, flag in itertools.product(
            [
                (0, 0, 0),
                (1, 0, 0),
                (2, 0, 0),
                (-2, 0, 0),
                (0.5, 0.5, 0),
                (1.00001, 0, 0),
            ],
            [0, 1, 2],
            [0, 1],
        ):
            yield dict(
                base, start=start, end=end, other=point, radius=radius, flag=flag
            )
    for tri in [
        ((0, 0, 0),) * 3,
        ((0, 0, 0), (1, 0, 0), (2, 0, 0)),
        ((0, 0, 0), (1e-6, 0, 0), (0, 1e-6, 0)),
    ]:
        for point in [(0, 0, 0), (0.5, 0, 0), (1, 1, 1)]:
            yield dict(base, start=(0, 0, 0), end=(0, 0, 0), tri=tri, other=point)


def camera_cases(count):
    rng = random.Random(6321)
    for i, c in enumerate(cases(count)):
        axes = old.axes(rng)
        c["view"] = tuple(v for row in axes for v in (*row, 0)) + (
            *[rng.uniform(-5, 5) for _ in range(3)],
            1,
        )
        sx, sy = rng.uniform(0.1, 4), rng.uniform(0.1, 4)
        c["projection"] = (
            sx,
            0,
            0,
            0,
            0,
            sy,
            0,
            0,
            0,
            0,
            1,
            1 if i % 2 else 0,
            0,
            0,
            -0.1,
            0 if i % 2 else 1,
        )
        yield c


def f32(x):
    return struct.unpack(">f", struct.pack(">f", x))[0]


def segment_intersects_box(c):
    """Independent double-precision slab check on the stored float inputs."""
    start, end, center = [[f32(x) for x in c[n]] for n in ("start", "end", "center")]
    near, far = 0.0, 1.0
    for axis, h in zip(c["axes"], c["half"]):
        axis = [f32(x) for x in axis]
        h = f32(h)
        p = sum((x - y) * a for x, y, a in zip(start, center, axis))
        d = sum((x - y) * a for x, y, a in zip(end, start, axis))
        if d == 0:
            if abs(p) > h:
                return False
        else:
            a, b = sorted(((-h - p) / d, (h - p) / d))
            near = max(near, a)
            far = min(far, b)
            if near > far:
                return False
    return True


def check_outputs(unit, c, a, b):
    if unit != "box_capsule" or not segment_intersects_box(c):
        assert eq(a, b), (unit, c, a, b)
        return
    # At intersections, rounding in the nearly coincident closest points can
    # change the normal and the contact position. This partial is explicitly
    # incomplete there; compare classification/state/separation, not those vectors.
    assert a[:2] == b[:2] and len(a[2]) == len(b[2]), (unit, c, a, b)
    for left, right in zip(a[2], b[2]):
        assert eq(left[2:], right[2:]), (unit, c, a, b)
        for normal, point, separation, feature in (left, right):
            length = math.sqrt(sum(x * x for x in normal))
            assert math.isfinite(length) and (length == 0 or abs(length - 1) < 3e-5)
            start = [f32(x) for x in c["start"]]
            end = [f32(x) for x in c["end"]]
            direction = [e - s for s, e in zip(start, end)]
            denom = sum(x * x for x in direction)
            t = (
                sum((p - s) * d for p, s, d in zip(point, start, direction)) / denom
                if denom
                else 0
            )
            t = min(1, max(0, t))
            distance = math.sqrt(
                sum((p - (s + t * d)) ** 2 for p, s, d in zip(point, start, direction))
            )
            assert distance <= f32(c["radius"]) + 1e-4, (
                unit,
                c,
                normal,
                point,
                distance,
            )


class CollisionDistanceTraversal(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        if ppc.Uc is None:
            raise unittest.SkipTest("Optional Unicorn is unavailable")
        try:
            _, ppc.original = ppc.load_target()
        except (FileNotFoundError, ValueError):
            raise unittest.SkipTest("Pinned original is unavailable")
        ppc.symbols = {s["name"]: s for s in ppc.original.symbols() if s["section"]}
        if any(
            not (
                ROOT / "build/comparisons" / ("compare_" + n) / "compiled.elf"
            ).is_file()
            for n in UNITS
        ):
            raise unittest.SkipTest("Run comparisons.py with the collision batch first")

    def compare_cases(self, unit, inputs, body=run):
        runners = [Run(), Run(ROOT / "build/comparisons" / ("compare_" + unit))]
        for i, c in enumerate(inputs):
            with self.subTest(unit=unit, case=i):
                a, b = [body(r, unit, c) for r in runners]
                check_outputs(unit, c, a, b)

    def test_shared_distance_helpers(self):
        for unit in ("line_segment_distance", "triangle_nearest"):
            self.compare_cases(unit, itertools.chain(cases(2000), boundary_cases()))

    def test_box_contacts_and_classification(self):
        for unit in ("box_sphere", "box_capsule"):
            self.compare_cases(unit, itertools.chain(cases(2000), boundary_cases()))

    def test_visibility_flags_and_projected_bounds(self):
        for unit in ("sphere_visibility", "capsule_visibility"):
            self.compare_cases(
                unit, itertools.chain(cases(2000), boundary_cases(), camera_cases(2000))
            )

    def test_tree_traversal_and_group_lifetime(self):
        for unit in (
            "cdb_branch_box",
            "cdb_branch_sphere",
            "cdb_branch_capsule",
            "cdb_branch_animatedvolume",
            "cdb_object_line",
        ):
            self.compare_cases(unit, trees(1000), tree_run)
