"""Isolated original/native checks for the maintained shortlist partials.

Uses the optional Unicorn fixture from test_explosion_grenade. External callees
are stubbed; these checks do not exercise the game runtime. Integer state, memory
and call order compare exactly; float traces round to six decimal places.
"""

import itertools
import struct
import unittest
from pathlib import Path
import test_explosion_grenade as ppc

ROOT = Path(__file__).resolve().parents[1]
BASE = ROOT / "build/comparisons"


class Run(ppc.Run):
    def __init__(self, candidate=None):
        super().__init__(candidate)
        self.entries = dict(ppc.symbols)
        if candidate:
            linked = ppc.Elf32((BASE / candidate / "compiled.elf").read_bytes())
            self.entries.update(
                {s["name"]: s for s in linked.symbols() if s["type"] == 2}
            )

    def call(self, name, *args):
        # A later function in a partial unit may move; enter its native symbol.
        self.rw(1, 0x10FF0000)
        for i, value in enumerate(args, 3):
            self.rw(i, value)
        self.u.reg_write(ppc.R.UC_PPC_REG_LR, 0x10001000)
        self.u.emu_start(self.entries[name]["address"], 0x10001000, count=100000)
        assert self.u.reg_read(ppc.R.UC_PPC_REG_PC) == 0x10001000
        return self.rr(3)

    def cstring(self, address, text):
        self.u.mem_write(address, text.encode() + b"\0")

    def string_stubs(self):
        self.at("strlen", lambda: len(self.text(self.rr(3))))
        self.at(
            "strcmp",
            lambda: (self.text(self.rr(3)) > self.text(self.rr(4)))
            - (self.text(self.rr(3)) < self.text(self.rr(4))),
        )

        def copy():
            dest = self.rr(3)
            self.u.mem_write(dest, self.text(self.rr(4)).encode() + b"\0")
            return dest

        def move():
            dest = self.rr(3)
            self.u.mem_write(dest, bytes(self.u.mem_read(self.rr(4), self.rr(5))))
            return dest

        def find():
            at = self.text(self.rr(3)).find(self.text(self.rr(4)))
            return 0 if at < 0 else self.rr(3) + at

        self.at("strcpy", copy)
        self.at("memmove", move)
        self.at("strstr", find)

    def callback(self, address, fn):
        self.hooks[address] = fn
        return address

    def script(self, args):
        stack, slots, table = 0x10010000, 0x10011000, 0x10012000
        self.wr(ppc.symbols["g_iCurrentBIFIndex"]["address"], 0)
        self.wr(ppc.symbols["g_pBuiltInFunctions"]["address"], table)
        self.u.mem_write(table + 10, struct.pack(">hh", len(args), len(args)))
        self.wr(slots, 0xABCDEF01, *args)
        self.wr(stack, slots + 4 * len(args))
        return stack, slots


def compare(unit, case, body):
    original = ppc.normalized(body(Run(), case))
    compiled = ppc.normalized(body(Run("compare_" + unit), case))
    assert original == compiled, (unit, case, original, compiled)
    return original


def elf_hash(name):
    h = 0
    for char in name:
        h = (h * 16 + ord(char)) & 0xFFFFFFFF
        g = h & 0xF0000000
        if g:
            h ^= g >> 24
        h &= ~g
    return h & 255


def loader_get_addr(r, case):
    mode, callback_valid = case
    loader, hdr, symtab, sections, strings, elf, chain = [
        0x10020000 + i * 0x1000 for i in range(7)
    ]
    typep, namep, result = 0x10030000, 0x10030100, 0x10030200
    r.string_stubs()
    r.cstring(typep, "kind")
    r.cstring(namep, "entry")
    r.cstring(strings, "entry\0\x7fkind")
    r.wr(loader, hdr if mode != "null" else 0)
    r.wr(hdr + 8, strings)
    r.wr(hdr + 16, 1, symtab, sections, elf)
    r.u.mem_write(elf + 48, struct.pack(">H", 3))
    r.wr(sections + 40 + 16, 0x100ABC00)
    r.u.mem_write(
        symtab, struct.pack(">IIIBBH", 0, 0x30, 0, 0, 1 if mode == "absolute" else 0, 1)
    )
    r.u.mem_write(hdr + 32, b"\xff" * 1024)
    r.wr(hdr + 32 + 4 * elf_hash("entry"), 0)
    r.wr(hdr + 0x420, chain)
    r.wr(chain, 0xFFFFFFFF)
    if mode == "wrong_type":
        r.cstring(strings, "entry\0\x7fother")
    if mode == "empty":
        r.wr(hdr + 32 + 4 * elf_hash("entry"), 0xFFFFFFFF)
    r.wr(result, 0xAAAAAAAA)

    def callback():
        assert r.text(r.rr(3)) == "entry"
        r.wr(r.rr(4), int(callback_valid))
        r.tracecall("callback")
        return 0x100CAFE0

    r.wr(loader + 0x1C, r.callback(0x80001800, callback))
    ret = r.call(
        "GetAddr__CQ24EAGL13DynamicLoaderPCcT1RPv", loader, typep, namep, result
    )
    if mode == "absolute":
        assert ret == 1 and r.rd(result) == 0x30
    elif mode == "relative":
        assert ret == 1 and r.rd(result) == 0x100ABC30
    elif mode == "null":
        assert ret == 0 and r.rd(result) == 0 and not r.trace
    else:
        assert ret == int(callback_valid) and r.rd(result) == 0x100CAFE0
    return ret, r.rd(result), r.trace


def symbol_search(r, case):
    mode = case
    pool, table, entry, name = 0x10020000, 0x10021000, 0x10022000, 0x10023000
    valid = 0x10024000
    r.string_stubs()
    r.cstring(name, "a")
    r.wr(pool, 4, 4, 0 if mode == "no_table" else table)
    r.wr(valid, 99)
    r.wr(entry, 0x12345678)
    r.cstring(entry + 4, "a")
    r.wr(table, 0, entry if mode == "direct" else 0, 0, 0)
    if mode == "collision":
        r.wr(table + 4, entry + 0x100, entry)
        r.wr(entry + 0x100, 0)
        r.cstring(entry + 0x104, "other")
    ret = r.call("Search__Q24EAGL10SymbolPoolPCcRb", pool, name, valid)
    found = mode in ("direct", "collision")
    assert ret == (0x12345678 if found else 0) and r.rd(valid) == int(found)
    return (
        ret,
        r.rd(valid),
        bytes(r.u.mem_read(pool, 20)),
        bytes(r.u.mem_read(table, 16)),
    )


def loader_initialize(r, case):
    relocated, extra_type = case
    loader, data, reloc = 0x10020000, 0x10030000, 0x10040000
    r.string_stubs()
    blob = bytearray(0x600)
    struct.pack_into(
        "<16sHHIIIIIHHHHHH",
        blob,
        0,
        b"\x7fELF" + b"\0" * 12,
        1,
        20,
        1,
        0,
        0,
        0x40,
        0,
        52,
        0,
        0,
        40,
        6,
        1,
    )
    shnames = b"\0.shstrtab\0.strtab\0.symtab\0.rel\0.extra\0"
    strings = b"\0__kind:::entry\0other\0"
    blob[0x180 : 0x180 + len(shnames)] = shnames
    blob[0x200 : 0x200 + len(strings)] = strings
    sections = [
        (0, 0, 0, 0, 0),
        (1, 3, 0x180, len(shnames), 0),
        (11, 3, 0x200, len(strings), 0),
        (19, 2, 0x300, 48, 2),
        (27, 9, 0x400, 8, 3),
        (32, extra_type, 0x440, 4, 0),
    ]
    for i, (name, typ, off, size, link) in enumerate(sections):
        struct.pack_into(
            "<10I",
            blob,
            0x40 + i * 40,
            name,
            typ,
            0,
            0,
            off,
            size,
            link,
            1 if typ == 9 else 0,
            4,
            16 if typ == 2 else 0,
        )
    for i, (name, value, section) in enumerate(
        [(0, 0, 0), (1, 0x34, 2), (16, 0x55, 0xFFF1)]
    ):
        struct.pack_into("<IIIBBH", blob, 0x300 + 16 * i, name, value, 4, 0, 0, section)
    split = 0x280 if relocated else len(blob)
    r.u.mem_write(data, bytes(blob[:split]))
    if relocated:
        r.u.mem_write(reloc, bytes(blob[split:]))
    r.wr(loader + 16, data, split, reloc if relocated else 0)
    allocations = []

    def alloc():
        size = r.rr(3)
        address = 0x10080000 + len(allocations) * 0x1000
        allocations.append((address, size))
        r.u.mem_write(address, b"\xa5" * max(size, 1))
        return address

    r.wr(
        ppc.symbols["_12EAGLInternal.EAGLMalloc"]["address"],
        r.callback(0x80001800, alloc),
    )
    r.at("PrintMessage__4EAGLiPCce", lambda: r.tracecall("message", r.text(r.rr(4))))
    head = ppc.symbols["_4EAGL.hashhead"]["address"]
    old = 0x10070000
    r.wr(head, old)
    r.wr(old + 4, 0xAAAAAAAA)
    r.call("Initialize__Q24EAGL13DynamicLoaderPFPCcRb_Pv", loader, 0x12345678)
    assert [n for _, n in allocations] == [1072, 12, 12]
    h = allocations[0][0]
    assert r.rd(loader) == h and r.rd(head) == h and r.rd(old + 4) == h
    assert r.rd(h) == old and r.rd(h + 4) == 0 and r.rd(h + 0x42C) == 0x12345678
    assert bytes(r.u.mem_read(data + 0x201, 12)) == b"entry\0\x7fkind\0"
    state = [bytes(r.u.mem_read(a, n)) for a, n in allocations]
    return (
        state,
        bytes(r.u.mem_read(loader, 48)),
        bytes(r.u.mem_read(data, split)),
        bytes(r.u.mem_read(reloc, len(blob) - split)) if relocated else b"",
        r.trace,
    )


def stateless(r, case):
    sparse, fps, previous, time, maskbits, companion, direct = case
    obj, anim, times, indices, out, mask = [0x10020000 + i * 0x1000 for i in range(6)]
    r.wr(obj + 12, anim)
    r.u.mem_write(obj + 16, struct.pack(">HBBI", previous, int(fps), 2, 0x12345678))
    r.wr(anim, 0, 0, times if sparse else 0, indices, 0x10027000 if companion else 0)
    r.u.mem_write(anim + 20, struct.pack(">HBB", 3, 2, 2))
    r.u.mem_write(times, struct.pack(">3H", 2, 5, 9))
    r.u.mem_write(indices, bytes([1, 3, 0, 2]))
    # Distinct frames exercise actual interpolation, followed by two constants.
    values = [
        0x0000,
        0x7D00,
        0x7E00,
        0x7F00,
        0xFD00,
        0xFE00,
        0xFF00,
        0x0000,
        0x7E00,
        0x7D00,
        0xFE00,
        0x7F00,
        0x7D00,
        0x7E00,
        0x7F00,
        0x0000,
        0xFE00,
        0xFD00,
        0x7E00,
        0x7F00,
        0x0000,
        0xFD00,
        0xFE00,
        0x7F00,
        0x0000,
        0x7D00,
        0x7E00,
        0x7F00,
        0xFD00,
        0xFE00,
        0xFF00,
        0x0000,
    ]
    r.u.mem_write(anim + 24, struct.pack(">32H", *values))
    r.wf(out, *([-77.0] * 48))
    r.wr(mask, maskbits or 0, *([0] * 7))

    def f3():
        assert r.rr(3) == 0x10027000
        r.tracecall("f3", r.fr(1), r.rr(4), r.rr(5), r.rr(6), r.rr(7), r.fr(2))
        return 0

    r.at("EvalSQTfast__Q28EAGLAnim13FnStatelessF3fPfPCQ28EAGLAnim8BoneMaskbif", f3)
    r.fw(1, time)
    if direct:
        r.fw(2, 0.25)
        result = r.call(
            "EvalSQTMask__Q28EAGLAnim12FnStatelessQfPfPCQ28EAGLAnim8BoneMaskbif",
            obj,
            out,
            mask,
            1,
            previous,
        )
    else:
        result = r.call(
            "EvalSQT__Q28EAGLAnim12FnStatelessQfPfPCQ28EAGLAnim8BoneMask",
            obj,
            out,
            mask if maskbits is not None else 0,
        )
    assert result == 1
    if maskbits == 4:
        # Skipping the first constant bone does not consume its packed values.
        assert r.rf(out + 2 * 48 + 16, 4) == (0.0, 0.25, 0.5, 1.0)
    # Scale/translation slots stay untouched; only quaternion slots are outputs.
    for bone in range(4):
        assert (
            r.rf(out + bone * 48, 4) == (-77.0,) * 4
            and r.rf(out + bone * 48 + 32, 4) == (-77.0,) * 4
        )
    return result, r.rf(out, 48), bytes(r.u.mem_read(obj + 16, 8)), r.trace


def static_init(r, flags):
    obj = 0x10020000
    r.u.mem_write(obj, b"\xa5" * 0x2F0)
    r.wr(obj + 0x218, flags)

    def quat():
        assert r.rr(3) == obj + 0x1B0 and r.rr(4) == obj + 0x50
        r.wf(r.rr(3), 1, 2, 3, 4)

    r.at("SetFromMatrix__11CQuaternionRC7CMatrix", quat)
    r.call("Init__13CStaticObject", obj)
    expected = (flags | 0x80000000) & ~0x71800000
    assert (
        r.rd(obj + 0x218) == expected
        and r.rd(obj + 0x214) == 0xFFFFFFFF
        and r.rd(obj + 0x12C) == 0
    )
    assert (
        r.rf(obj + 0x190) == (0.0, 0.0, 0.0)
        and r.rf(obj + 0x110, 1) == (0.5,)
        and r.rf(obj + 0x1E4, 1) == (1.0,)
    )
    return bytes(r.u.mem_read(obj, 0x2F0))


def static_attempt(r, case):
    motion, rate, dt, world_present = case
    obj, volume, local, vt, nodes = (
        0x10020000,
        0x10021000,
        0x10022000,
        0x10023000,
        0x10024000,
    )
    r.u.mem_write(obj, b"\xa5" * 0x2F0)
    r.wr(obj, vt)
    r.wr(obj + 0x20, nodes, nodes + 16, nodes + 32, nodes + 48)
    r.wr(obj + 0x25C, motion)
    r.wf(obj + 0x264, rate)
    r.wf(obj + 0x26C, 2.0)
    r.wf(obj + 0x190, 1, 2, 3, 99)
    r.wf(obj + 0x150, 4, 5, 6, 77)
    r.wr(obj + 0x134, local)
    r.wr(obj + 0x13C, volume if world_present else 0)
    r.wr(volume, vt + 0x1000)

    def slerp():
        r.tracecall("slerp", r.fr(1))
        r.wf(r.rr(3), 1, 0, 0, 0)

    r.at("Slerp__11CQuaternionT0T0f", slerp)
    r.at("GetMatrix__C11CQuaternionR7CMatrix", lambda: r.tracecall("matrix"))
    r.at("SetPos__7CMatrixRC8CVector3", lambda: r.tracecall("position", r.rf(r.rr(4))))
    r.at("Translate__7CMatrixRC8CVector3", lambda: r.tracecall("delta", r.rf(r.rr(4))))

    def transform():
        assert r.rr(3) == volume - 4 and r.rr(4) == local and r.rr(5) == obj + 0x50
        r.tracecall("volume")

    def extents():
        r.wf(r.rr(4), -1, -2, -3)
        r.wf(r.rr(5), 1, 2, 3)

    r.slot(vt + 0x1000, 0x18, transform, -4)
    r.slot(vt + 0x1000, 0x88, extents, 4)
    r.slot(vt, 0xA8, lambda: r.wf(r.rr(4), 7, 8, 9))
    r.fw(1, dt)
    r.call("AttemptAnimMotionUpdate__13CStaticObjectf", obj)
    return bytes(r.u.mem_read(obj, 0x2F0)), bytes(r.u.mem_read(nodes, 64)), r.trace


def tank_weapon(r, case):
    slot, crc, created, projectile = case
    obj, weapon, props = 0x10020000, 0x10022000, 0x10023000
    r.u.mem_write(obj, b"\xa5" * 0xB84)
    r.wr(weapon + 0x300, props)
    r.u.mem_write(props + 14, struct.pack(">h", projectile))
    r.wr(weapon + 0x31C, 99)

    def create():
        assert (
            r.rr(3) == ppc.symbols["g_WeaponFactory"]["address"]
            and r.rr(4) == crc
            and r.rr(5) == obj
            and r.rr(6) == 999
        )
        r.tracecall("create")
        return weapon if created else 0

    r.at("CreateWeaponByCRC__14CWeaponFactoryiP10ISceneNodei", create)
    result = r.call("AddWeapon__11CTankObjectii", obj, slot, crc)
    expected = crc != 0xFB9630C9 and created
    assert result == int(expected)
    assert r.rd(obj + 0xB78 + 4 * slot) == (weapon if expected else 0)
    assert r.rd(weapon + 0x31C) == ((4 if projectile else 10) if expected else 99)
    return result, bytes(r.u.mem_read(obj, 0xB84)), r.rd(weapon + 0x31C), r.trace


def expression(r, case):
    current, pending, requested, time = case
    node, vt, soldier = 0x10020000, 0x10021000, 0x10022000
    args = [requested, struct.unpack(">I", struct.pack(">f", time))[0]]
    stack, slots = r.script(args)
    r.wr(node, vt)
    r.slot(vt, 0x160, lambda: soldier)
    # Layout is asserted against the original accesses below in this fixture.
    offset = EXPRESSION_OFFSET
    r.u.mem_write(soldier + offset, b"\xa5" * 32)
    r.wr(soldier + offset, current)
    r.wr(soldier + offset + 4, pending)
    r.call("BIFunc_SetExpression__FPPiPv", stack, node)
    assert r.rd(stack) == slots and r.rd(slots) == 0xFFFFFFFF
    return bytes(r.u.mem_read(soldier + offset, 32)), bytes(r.u.mem_read(slots, 12))


def alias_looping(r, case):
    sound, looping = case
    stack, slots = r.script([sound])
    r.at("GetStringCRC__FPCc", lambda: 7)

    def ctor():
        r.u.mem_write(r.rr(3), b"\0" * 0xD8)
        return r.rr(3)

    def get():
        r.wr(r.rr(4) + 12, looping)
        r.tracecall("get", r.rr(5))

    r.at("__10SoundEntry", ctor)
    r.at("_._10SoundEntry", lambda: r.tracecall("destroy"))
    r.at("GetSound__9cSoundManR14SoundEntryBankUi", get)
    r.call("BIFunc_IsAliasSoundLooping__FPPiPv", stack, 0)
    assert (
        r.rd(slots) == (looping if sound not in (0, 7) else 0) and r.rd(stack) == slots
    )
    return r.rd(slots), r.trace


def soldier_info(r, case):
    is_float = case
    script, bsgo, bvt, node = 0x10020000, 0x10021000, 0x10022000, 0x10023000
    stack, slots = r.script([script, 5, 0x12345678, 0x3F800000])
    r.wr(script + 32, bsgo)
    r.wr(bsgo, bvt)
    r.slot(bvt, 40, lambda: node)

    def info():
        assert [r.rr(i) for i in (3, 4, 5)] == [5, 0x12345678, node] and r.fr(1) == 1.0
        r.wr(r.rr(6), int(is_float))
        r.wr(r.rr(7), 123)
        r.wf(r.rr(8), 2.5)

    r.at("GetObjInfo__FiP8BSObjectfP10ISceneNodeRbRiRf", info)
    r.call("BIFunc_Soldier_GetObjInfo__FPPiPv", stack, 0)
    assert r.rd(stack) == slots and r.rd(slots) == (0x40200000 if is_float else 123)
    return bytes(r.u.mem_read(slots, 20))


def vibration(r, case):
    multiplayer, is_player = case
    script, bsgo, bvt, node, nvt, player, pvt = [
        0x10020000 + i * 0x1000 for i in range(7)
    ]
    stack, slots = r.script([3, 0x40000000, 0x3F000000, 5])
    r.wr(ppc.symbols["g_bInMultiplayerMode"]["address"], int(multiplayer))
    r.wr(ppc.symbols["g_pBSObject"]["address"], script)
    r.wr(script + 32, bsgo)
    r.wr(bsgo, bvt)
    r.wr(node, nvt)
    r.wr(player, pvt)
    r.slot(bvt, 40, lambda: node)
    r.slot(nvt, 0x170, lambda: player if is_player else 0)
    r.slot(nvt, 0xA8, lambda: r.wf(r.rr(4), 1, 2, 3))
    r.slot(pvt, PLAYER_INDEX_SLOT, lambda: 2)
    r.at("FindClosestPlayer__6CSceneRC8CVector3", lambda: player)
    r.at("GetPlayer__C6CScenei", lambda: player)

    def feedback():
        assert [r.rr(i) for i in (4, 5, 6)] == [2, 3, 5]
        assert r.fr(1) == 0.5 and r.fr(2) == 2.0
        r.tracecall("feedback")

    r.at("SetFeedback__13CInputManagerUcUcUcff", feedback)
    r.call("BIFunc_PlayVibrationEvent__FPPiPv", stack, 0)
    assert r.rd(stack) == slots and r.trace == [("feedback",)]
    return bytes(r.u.mem_read(slots, 20)), r.trace


EXPRESSION_OFFSET = 0xF7C  # The original stores match SceneObjects.h's scoped view.
PLAYER_INDEX_SLOT = 0x2C0


class ShortlistComparisons(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        if ppc.Uc is None:
            raise unittest.SkipTest("Install optional unicorn for isolated PPC checks")
        if (
            not (ROOT / "orig/GR8E69/MOH3RDVD.ELF").exists()
            or not (BASE / "compare_scene_tank_add_weapon/compiled.elf").exists()
        ):
            raise unittest.SkipTest("Run comparisons.py with the pinned original first")
        _, ppc.original = ppc.load_target()
        ppc.symbols = {s["name"]: s for s in ppc.original.symbols() if s["section"]}

    def cases(self, unit, cases, body):
        for case in cases:
            with self.subTest(case=case):
                compare(unit, case, body)

    def test_loader_lookup(self):
        self.cases(
            "loading_DynamicLoaderGetAddr",
            itertools.product(
                ["null", "absolute", "relative", "wrong_type", "empty"], [False, True]
            ),
            loader_get_addr,
        )

    def test_symbol_search(self):
        self.cases(
            "loading_SymbolPoolSearch",
            ["no_table", "direct", "collision", "missing"],
            symbol_search,
        )

    def test_loader_initialize(self):
        self.cases(
            "loading_DynamicLoaderInitialize",
            itertools.product([False, True], [0, 4, 5, 8, 10, 11, 123]),
            loader_initialize,
        )

    def test_stateless_evaluation(self):
        self.cases(
            "FnStatelessQEval",
            (
                (s, f, p, t, m, c, False)
                for s, f, p, t, m, c in itertools.product(
                    [False, True],
                    [False, True],
                    [0, 1, 2],
                    [-1.0, 0.0, 0.5, 1.0, 2.0, 5.0],
                    [None, 0, 4, 5, 10, 15],
                    [False, True],
                )
            ),
            stateless,
        )

    def test_stateless_masked_entry(self):
        self.cases(
            "FnStatelessQEval",
            (
                (False, False, p, 0.0, m, False, True)
                for p, m in itertools.product([0, 1, 2], [0, 4, 5, 10, 15])
            ),
            stateless,
        )

    def test_static_initialization(self):
        self.cases(
            "scene_static_init", [0, 0xFFFFFFFF, 0xAAAAAAAA, 0x55555555], static_init
        )

    def test_static_animation_attempt(self):
        self.cases(
            "scene_static_anim_attempt",
            itertools.product(
                [0, 1], [0.0, 1.0], [-1.0, 0.0, 0.25, 2.0, float("nan")], [False, True]
            ),
            static_attempt,
        )

    def test_tank_weapon(self):
        self.cases(
            "scene_tank_add_weapon",
            itertools.product(range(3), [0xFB9630C9, 123], [False, True], [-1, 0, 1]),
            tank_weapon,
        )

    def test_expression(self):
        self.cases(
            "script_set_expression",
            itertools.product([-1, 0, 1], [-1, 0], [0, 1], [0.0, 2.0]),
            expression,
        )

    def test_alias_looping(self):
        self.cases(
            "script_alias_looping",
            itertools.product([0, 7, 123], [0, 1]),
            alias_looping,
        )

    def test_soldier_query(self):
        self.cases("script_soldier_obj_info", [False, True], soldier_info)

    def test_vibration(self):
        self.cases(
            "script_vibration",
            itertools.product([False, True], [False, True]),
            vibration,
        )


if __name__ == "__main__":
    unittest.main()
