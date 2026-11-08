"""Original/native comparison checks for the explosion/grenade partial sources.

Requires the pinned original, tools/comparisons.py output and optional unicorn
(tested with 2.1.4). Public CI skips these checks. External callees are stubbed;
this is not a game/runtime test. Float traces round to six decimal places while
returned integers, byte snapshots, flags and call ordering compare exactly.
"""

import math
import struct
import sys
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
from audit import load_target
from formats import Elf32

try:
    from unicorn import Uc, UC_ARCH_PPC, UC_MODE_32, UC_MODE_BIG_ENDIAN, UC_HOOK_CODE
    from unicorn import ppc_const as R
except ImportError:
    Uc = None

BASE = ROOT / "build/comparisons"
original = None
symbols = {}


class Run:
    def __init__(self, candidate=None):
        self.u = Uc(UC_ARCH_PPC, UC_MODE_32 | UC_MODE_BIG_ENDIAN)
        self.u.mem_map(0x80000000, 0x1000000)
        self.u.mem_map(0x10000000, 0x1000000)
        for sec in original.sections:
            if sec["flags"] & 2 and sec["size"] and sec["type"] == 1:
                self.u.mem_write(sec["address"], original.contents(sec))
        self.candidate_ranges = []
        if candidate:
            e = Elf32((BASE / candidate / "compiled.elf").read_bytes())
            self.candidate_ranges = [
                (s["address"], s["address"] + s["size"])
                for s in e.symbols()
                if s["type"] == 2
            ]
            for sec in e.sections:
                if sec["flags"] & 2 and sec["size"]:
                    self.u.mem_write(sec["address"], e.contents(sec))
        self.previous_pc = None
        self.trace = []
        self.hooks = {}
        self.u.hook_add(UC_HOOK_CODE, self.hook)
        self.u.reg_write(R.UC_PPC_REG_MSR, 0x2000)

    def wr(self, a, *v):
        self.u.mem_write(
            a, struct.pack(">" + "I" * len(v), *[x & 0xFFFFFFFF for x in v])
        )

    def wf(self, a, *v):
        self.u.mem_write(a, struct.pack(">" + "f" * len(v), *v))

    def rd(self, a):
        return struct.unpack(">I", self.u.mem_read(a, 4))[0]

    def rf(self, a, n=3):
        return struct.unpack(">" + "f" * n, self.u.mem_read(a, 4 * n))

    def rr(self, n):
        return self.u.reg_read(getattr(R, "UC_PPC_REG_" + str(n)))

    def rw(self, n, v):
        self.u.reg_write(getattr(R, "UC_PPC_REG_" + str(n)), v & 0xFFFFFFFF)

    def fr(self, n):
        return struct.unpack(
            ">d",
            struct.pack(">Q", self.u.reg_read(getattr(R, "UC_PPC_REG_FPR" + str(n)))),
        )[0]

    def fw(self, n, v):
        self.u.reg_write(
            getattr(R, "UC_PPC_REG_FPR" + str(n)),
            struct.unpack(">Q", struct.pack(">d", v))[0],
        )

    def at(self, name, fn):
        self.hooks[symbols[name]["address"]] = fn

    def slot(self, vt, offset, fn, adj=0):
        address = 0x80001000 + len(self.hooks) * 4
        self.hooks[address] = fn
        self.u.mem_write(vt + offset, struct.pack(">hhI", adj, 0, address))

    def hook(self, u, a, size, data):
        # A longer partial may occupy the next original function's entry. Only
        # an actual call to that entry should trigger its stub; sequential
        # execution still belongs to the isolated candidate's native code.
        fallthrough = self.previous_pc == a - 4
        self.previous_pc = a
        if a in self.hooks and not (
            fallthrough
            and any(start <= a < end for start, end in self.candidate_ranges)
        ):
            ret = self.hooks[a]()
            if ret is not None:
                self.rw(3, ret)
            u.reg_write(R.UC_PPC_REG_PC, u.reg_read(R.UC_PPC_REG_LR))

    def call(self, name, *args):
        self.rw(1, 0x10FF0000)
        for i, v in enumerate(args, 3):
            self.rw(i, v)
        self.u.reg_write(R.UC_PPC_REG_LR, 0x10001000)
        self.u.emu_start(symbols[name]["address"], 0x10001000, count=20000)
        assert self.u.reg_read(R.UC_PPC_REG_PC) == 0x10001000
        return self.rr(3)

    def sqrt(self):
        # sqrtf returns a single-precision value in the floating-point register.
        value = struct.unpack(">f", struct.pack(">f", math.sqrt(self.fr(1))))[0]
        self.fw(1, value)

    def tracecall(self, label, *args):
        self.trace.append((label, *args))

    def text(self, a):
        out = b""
        while self.u.mem_read(a, 1) != b"\0":
            out += bytes(self.u.mem_read(a, 1))
            a += 1
        return out.decode()


def normalized(v):
    if isinstance(v, float):
        return "NaN" if math.isnan(v) else round(v, 6)
    if isinstance(v, (tuple, list)):
        return tuple(normalized(x) for x in v)
    return v


def compare(candidate, case, fn):
    a = normalized(fn(Run(), case))
    b = normalized(fn(Run(candidate), case))
    assert a == b, (candidate, case, a, b)
    return a


obj, node, vt, world, vol, wvt, vvt = [0x10020000 + i * 0x1000 for i in range(7)]


def height(r, c):
    typ, z = c
    r.wr(obj + 0x38, typ)
    r.wf(obj + 0x20, 11, 12, z, 99)
    r.wr(node, vt)

    def pos():
        assert r.rr(3) == node + 4
        r.wf(r.rr(4), 1, 2, 3)
        r.tracecall("position")

    r.slot(vt, 0xA8, pos, 4)
    result = r.call("ShouldHeightCheckBeFlipped__10CExplosionR10ISceneNode", obj, node)
    assert result == int(21 <= typ <= 23 and z - 3 > 2), (c, result)
    return result, r.trace


def ray(r, c):
    state, matrix_values = c
    p, a, b, m = 0x10030000, 0x10030100, 0x10030200, 0x10030300
    r.wf(obj + 0x20, 10, 20, 30, 99)
    r.wf(p, 1, 2, 3, 77)
    r.wf(a, 2, -1, 4, 55)
    r.wf(b, -2, 3, 1, 88)
    r.wf(m, *matrix_values)
    r.wr(symbols["g_scene"]["address"] + 0x18, world)
    r.wr(world, wvt)
    r.wr(vol, vvt)

    def line():
        r.tracecall("line", r.rf(r.rr(4)), r.rf(r.rr(5)))

    r.at("Set__6CLine3RC8CVector3T1", line)
    ctor = "__10CCollision"

    def collision():
        r.wr(r.rr(3) + 16, 0xFFFFFFFF)
        return r.rr(3)

    r.at(ctor, collision)
    r.at("_._10CCollision", lambda: r.tracecall("destroy_collision"))

    def bound():
        assert r.rr(3) == world - 4 and r.rr(4) == 0
        return vol

    def test():
        assert r.rr(3) == vol + 4 and r.rr(6) == 0
        assert r.rd(r.rr(5) + 16) == 0x7FFFFFFF
        r.tracecall("test")
        return state

    r.slot(wvt, 0x90, bound, -4)
    r.slot(vvt, 0x68, test, 4)
    result = r.call("TestRay__10CExplosionRC8CVector3N21RC7CMatrix", obj, p, a, b, m)
    assert result == int(state != 1)

    # Row-vector transform followed by the appropriate world position.
    def point(v, offset):
        return tuple(
            sum(v[k] * matrix_values[k * 4 + j] for k in range(3))
            + matrix_values[12 + j]
            + offset[j]
            for j in range(3)
        )

    assert normalized(r.trace[0]) == normalized(
        ("line", point((2, -1, 4), (10, 20, 30)), point((-2, 3, 1), (1, 2, 3)))
    )
    return result, r.trace


props, light, lvt, source, script = [0x10040000 + i * 0x1000 for i in range(5)]


def commit(r, c):
    typ, timer, flags, velocity, with_light = c
    r.u.mem_write(obj, b"\xa5" * 0x280)
    r.wr(obj, vt)
    r.wr(obj + 0xC4, typ, props)
    r.wf(props + 0x60, 8)
    r.wf(obj + 0xB0, timer)
    r.wr(obj + 0x25C, flags)
    r.wf(obj + 0xA0, 10, 20, 30, 99)
    r.wf(obj + 0x140, *velocity, 77)
    r.wr(obj + 0x254, light if with_light else 0)
    r.wr(light, lvt)
    r.wr(symbols["_7CMatrix.s_ClassInit"]["address"], 1)
    r.slot(vt, 0x2E0, lambda: r.fw(1, 17))
    r.slot(vt, 0x2E8, lambda: 23)
    r.slot(vt, 0x2F0, lambda: source)
    r.slot(vt, 0xF8, lambda: script)

    def destroy():
        assert r.rr(3) == obj and r.rr(4) == 0
        r.tracecall("destroy")

    r.slot(vt, 8, destroy)

    def getmatrix():
        r.tracecall("getmatrix")
        r.wf(r.rr(4), 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 10, 20, 30, 1)

    def setmatrix():
        r.tracecall("light", r.rf(r.rr(4) + 48))

    r.slot(vt, 0x98, getmatrix)
    r.slot(lvt, 0x238, setmatrix)

    def explosion():
        assert (
            r.rr(4) == symbols["g_scene"]["address"]
            and r.rr(6) == 23
            and r.rr(7) == source
            and r.rr(8) == 0
            and r.rr(9) == 1
        )
        r.tracecall("explosion", r.rf(r.rr(5)), r.fr(1), r.fr(2), r.fr(3))
        return r.rr(3)

    r.at(
        "__10CExplosionR6CSceneRC8CVector3fff17EScriptBulletTypeP10ISceneNodebT8",
        explosion,
    )
    r.at("_._10CExplosion", lambda: r.tracecall("destroy_explosion"))

    def trans():
        r.wf(r.rr(3) + 48, *r.rf(r.rr(4)), 1)

    def effect():
        r.tracecall("effect", r.rf(r.rr(4) + 48), r.rr(5))

    r.at("BuildTrans__7CMatrixRC8CVector3", trans)
    r.at("CreateExplosionEffect__13CThrownBulletRC7CMatrixb", effect)

    def crc():
        r.tracecall("alias", r.text(r.rr(3)))
        return 12345

    r.at("GetStringCRC__FPCc", crc)

    def sound():
        e = r.rr(3)
        assert (
            r.rd(e + 36) == 12345
            and r.rd(e + 32) == symbols["_vt.15SoundAliasEvent"]["address"]
        )
        assert r.u.mem_read(e + 4, 28) == b"\0" * 28
        emitter = r.rd(e)
        assert r.rd(emitter) == symbols["_vt.20PositionSoundEmitter"]["address"]
        r.tracecall("sound", r.rf(emitter + 4))
        return 7

    r.at("SendEvent__15SoundAliasEvent", sound)

    def msg():
        assert [r.rr(i) for i in range(3, 8)] == [141, script, 0, 0, obj]
        r.tracecall("message")
        return 1

    r.at("BSSendMessage__FUsP8BSObjectP13TriggerObjectT1Pv", msg)
    r.at("DebugMsg__FPCce", lambda: r.tracecall("warning", r.text(r.rr(3)), r.rr(4)))
    r.at("sqrtf", r.sqrt)
    before = bytes(r.u.mem_read(obj, 0x280))
    r.call("CommitUpdate__13CThrownBullet", obj)
    after = bytes(r.u.mem_read(obj, 0x280))
    if flags & 0x01000000:
        assert before == after and not r.trace
    elif typ not in [0, 1, 2, 4, 5, 6, 7, 8, 9, 10, 11]:
        assert r.trace[0][0] == "warning"
    else:
        moving = not (timer <= 0) and (not (flags & 0x10000000) or flags & 0x08000000)
        if moving:
            assert r.rf(obj + 0x50) == (10, 20, 30) and r.rf(obj + 0x150) == velocity
            expected = flags
            if typ not in [0, 6, 8]:
                if typ != 4:
                    expected = (flags & ~0x20000000) | (
                        0 if flags & 0x80000000 else 0x20000000
                    )
                expected &= ~0xC0000000
            assert r.rd(obj + 0x25C) == expected
        else:
            expected_sound = typ in [0, 6, 8] or not (flags & 0x08000000)
            assert any(x[0] == "sound" for x in r.trace) == expected_sound
            assert any(x[0] == "explosion" for x in r.trace) == (
                expected_sound and typ != 5
            )
            assert any(x[0] == "destroy" for x in r.trace)
    return after, r.trace


def obstruction(r, c):
    kind, crouch, headheight, flipped, outcomes, position = c
    player, soldier = 0x10060000, 0x10061000
    r.wr(node, vt)
    r.wf(obj + 0x20, 0, 0, 0, 99)
    r.wf(player + 0x18C, 0.75)
    r.slot(vt, 0xA8, lambda: r.wf(r.rr(4), *position))
    r.slot(vt, 0x170, lambda: player if kind == "player" else 0)
    r.slot(vt, 0x160, lambda: soldier if kind == "soldier" else 0)
    r.wr(symbols["_7CMatrix.s_ClassInit"]["address"], 0)
    r.at(
        "InitClass__7CMatrix",
        lambda: r.wr(symbols["_7CMatrix.s_ClassInit"]["address"], 1),
    )
    for name, offset in [
        ("SetRight", 0),
        ("SetFront", 16),
        ("SetUp", 32),
        ("SetPos", 48),
    ]:
        r.at(
            name + "__7CMatrixRC8CVector3",
            lambda offset=offset: r.wf(r.rr(3) + offset, *r.rf(r.rr(4)), 1),
        )
    r.at("sqrtf", r.sqrt)
    r.at("IsPlayerCrouching__C13CPlayerObject", lambda: int(crouch))
    r.at(
        "GetLookMatrix__14CSoldierObjectR7CMatrix",
        lambda: r.wf(r.rr(4) + 48, 0, 0, position[2] + headheight, 1),
    )
    r.at("ShouldHeightCheckBeFlipped__10CExplosionR10ISceneNode", lambda: int(flipped))

    def ray_body():
        r.tracecall(
            "ray",
            r.rf(r.rr(4)),
            r.rf(r.rr(5)),
            r.rf(r.rr(6)),
            tuple(r.rf(r.rr(7) + off) for off in (0, 16, 32, 48)),
        )
        return int(outcomes[len(r.trace) - 1])

    r.at("TestRay__10CExplosionRC8CVector3N21RC7CMatrix", ray_body)
    result = r.call("CheckForObstructions__10CExplosionR10ISceneNode", obj, node)
    if position[:2] == (0.0, 0.0):
        assert result == 0 and not r.trace
    else:
        assert result == int(all(outcomes))
        assert len(r.trace) == next((j + 1 for j, v in enumerate(outcomes) if not v), 3)
    return result, r.trace


class ExplosionGrenadeComparisons(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        global original, symbols
        if Uc is None:
            raise unittest.SkipTest(
                "Install optional unicorn to execute isolated PPC checks"
            )
        names = (
            "explosion_height",
            "explosion_ray",
            "explosion_obstructions",
            "thrown_bullet_commit",
        )
        if not (ROOT / "orig/GR8E69/MOH3RDVD.ELF").exists() or not all(
            (BASE / ("compare_" + name) / "compiled.elf").exists() for name in names
        ):
            raise unittest.SkipTest("Run comparisons.py with the pinned original first")
        _, original = load_target()
        symbols = {s["name"]: s for s in original.symbols() if s["section"]}

    def test_height_types_and_thresholds(self):
        # 48 cases, including equality, unordered comparison and signed adjustment.
        for typ in [-1, 0, 20, 21, 22, 23, 24, 0x7FFFFFFF]:
            for z in [4.999, 5, 5.001, 100, float("nan"), float("inf")]:
                with self.subTest(typ=typ, z=z):
                    compare("compare_explosion_height", (typ, z), height)

    def test_ray_transforms_and_collision_results(self):
        # 10 cases; translated/scaled/rotated offsets and raw collision results.
        mats = [
            (1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1),
            (0, 2, 0, 0, -3, 0, 0, 0, 0, 0, 4, 0, 5, 6, 7, 1),
        ]
        for state in [-1, 0, 1, 2, 3]:
            for mat in mats:
                with self.subTest(state=state, matrix=mat):
                    compare("compare_explosion_ray", (state, mat), ray)

    def test_commit_types_timers_flags_and_light(self):
        # 560 cases; lifetime ordering and writes outside the changed fields too.
        for typ in range(-1, 13):
            for timer in [-1.0, 0.0, 1.0, float("nan")]:
                for flags in [0, 0x80000000, 0xF0000000, 0x18000000, 0x01000000]:
                    for with_light in [False, True]:
                        with self.subTest(
                            typ=typ, timer=timer, flags=flags, light=with_light
                        ):
                            compare(
                                "compare_thrown_bullet_commit",
                                (typ, timer, flags, (3.0, 4.0, 0.0), with_light),
                                commit,
                            )

    def test_bazooka_backoff_thresholds(self):
        # 36 more cases exercising zero, threshold, full 3D and unordered speed.
        velocities = [
            (0.0, 0.0, 0.0),
            (0.0, 0.0, 0.0005),
            (0.0, 0.0, 0.001),
            (0.0, 0.0, 0.0011),
            (3.0, 4.0, 12.0),
            (float("nan"), 0.0, 0.0),
        ]
        for typ in [0, 6, 8]:
            for velocity in velocities:
                for with_light in [False, True]:
                    with self.subTest(typ=typ, velocity=velocity, light=with_light):
                        compare(
                            "compare_thrown_bullet_commit",
                            (typ, 0.0, 0, velocity, with_light),
                            commit,
                        )

    def test_obstruction_samples_and_early_returns(self):
        # 96 cases, with ray/height bodies stubbed to isolate this caller.
        for kind, crouch, headheight in [
            ("other", False, 2.0),
            ("player", False, 2.0),
            ("player", True, 2.0),
            ("soldier", False, 2.4),
            ("soldier", False, -1.0),
            ("soldier", False, float("nan")),
        ]:
            for flipped in [False, True]:
                for outcomes in [
                    (True, True, True),
                    (False, True, True),
                    (True, False, True),
                    (True, True, False),
                ]:
                    for position in [(3.0, 4.0, 10.0), (0.0, 0.0, 10.0)]:
                        case = (kind, crouch, headheight, flipped, outcomes, position)
                        with self.subTest(case=case):
                            compare("compare_explosion_obstructions", case, obstruction)


if __name__ == "__main__":
    unittest.main()
