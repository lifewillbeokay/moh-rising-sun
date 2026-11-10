"""Original/native PowerPC checks for the broader property/card/texture batch.

Requires the pinned original, local native outputs and optional Unicorn 2.1.4.
External services are controlled stubs. These are isolated behavior checks,
not game or Dolphin runtime validation. Memory and return bits compare exactly.
"""

import itertools
import json
import random
import struct
import unittest
from pathlib import Path

import test_explosion_grenade as ppc
from test_shortlist_partials import Run

ROOT = Path(__file__).resolve().parents[1]
OBJ = 0x10020000


def initialize():
    if ppc.Uc is None:
        raise unittest.SkipTest("Optional Unicorn is unavailable")
    try:
        _, ppc.original = ppc.load_target()
    except (FileNotFoundError, ValueError):
        raise unittest.SkipTest("Pinned original is unavailable")
    ppc.symbols = {s["name"]: s for s in ppc.original.symbols() if s["section"]}


def native(unit, partial=False):
    folder = "comparisons" if partial else "reconstruction/units"
    return ROOT / "build" / folder / (("compare_" if partial else "") + unit)


def require(*paths):
    if any(not (p / "compiled.elf").is_file() for p in paths):
        raise unittest.SkipTest("Local native output is unavailable")


def compare_case(unit, case, body, partial=False):
    a, b = body(Run(), case), body(Run(native(unit, partial)), case)
    assert a == b, (unit, case, a, b)
    return a


def advance(r, case):
    state, count, frame, amount = case
    r.u.mem_write(OBJ, b"\xa5" * 60)
    r.u.mem_write(OBJ + 5, bytes([state]))
    r.wr(OBJ + 16, count)
    r.wf(OBJ + 24, frame)
    r.at("Decompress__9AnimShapei", lambda: r.tracecall("decompress", r.rr(3), r.rr(4)))
    r.fw(1, amount)
    r.call("AdvanceFrames__9AnimShapef", OBJ)
    return bytes(r.u.mem_read(OBJ, 60)), r.trace


def rebind(r, case):
    dirty, count = case
    r.u.mem_write(OBJ, b"\xa5" * 60)
    r.u.mem_write(OBJ + 7, bytes([dirty]))
    r.wr(OBJ + 28, 0x10030000, 0x10040000)
    r.wr(OBJ + 40, 0x10050000, 0x10050000 + 4 * count)
    r.wr(0x10050000, *[0x10060000 + 88 * i for i in range(count)])
    r.at(
        "SwapShape__Q24EAGL3TARPC5SHAPE", lambda: r.tracecall("swap", r.rr(3), r.rr(4))
    )
    r.call("RebindTARs__9AnimShape", OBJ)
    return bytes(r.u.mem_read(OBJ, 60)), r.trace


def invalidate(r, case):
    method, value = case
    r.u.mem_write(OBJ, b"\xa5" * 160)
    r.wr(ppc.symbols["g_hMemoryCard"]["address"], 0x10030000)
    r.at(
        "Cache__11CMemoryCardPQ211CMemoryCard6Device",
        lambda: r.tracecall("cache", r.rr(3), r.rr(4), r.rd(OBJ + 152)),
    )
    r.call(method + "__9CMCDeviceb", OBJ, value)
    return bytes(r.u.mem_read(OBJ, 160)), r.trace


class PartialSweepTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        initialize()
        require(
            *(
                native(u, True)
                for u in (
                    "animshape_advance",
                    "animshape_rebind",
                    "card_device_invalidation",
                )
            ),
            native("mc_getname"),
        )

    def test_advance_modes_and_boundaries(self):
        cases = itertools.product(
            map(ord, "LlOoPp-?"),
            (2, 10),
            (-1.0, -0.0, 0.75, 4.5, 9.999),
            (-10.0, -1.25, 0.0, 0.25, 2.0, 19.75),
        )
        for case in cases:
            compare_case("animshape_advance", case, advance, True)
        for state in map(ord, "LlOoPp-?"):
            compare_case(
                "animshape_advance", (state, 10, float("nan"), 1.0), advance, True
            )

    def test_rebind_dirty_and_empty_lists(self):
        for case in itertools.product((0, 1), (0, 1, 3)):
            compare_case("animshape_rebind", case, rebind, True)

    def test_device_invalidation_and_cache(self):
        for value in (0, 1):
            compare_case("mc_getname", ("Invalidate", value), invalidate)
            compare_case("card_device_invalidation", ("Cache", value), invalidate, True)


def card_update(r, case):
    busy, refresh, before, now, probed = case
    r.wr(ppc.symbols["g_hMemoryCard"]["address"], OBJ)
    for name, values in (("gs_statusBefore", before), ("gs_statusNow", now)):
        r.wr(ppc.symbols[name]["address"], *values)
    r.wr(ppc.symbols["gs_bChangeOfState"]["address"], 7)
    r.at("IsBusy__C11CMemoryCard", lambda: busy)

    def probe():
        slot = r.rr(3)
        r.tracecall("probe", slot, r.rr(4), r.rr(5))
        return probed[slot]

    r.at("CARDProbeEx", probe)
    r.at("Cache__9CMCDeviceb", lambda: r.tracecall("cache", r.rr(3), r.rr(4)))
    r.call("Update__Q211CMemoryCard6Deviceb", refresh)
    memory = tuple(
        bytes(r.u.mem_read(ppc.symbols[n]["address"], size))
        for n, size in (
            ("gs_statusBefore", 8),
            ("gs_statusNow", 8),
            ("gs_bChangeOfState", 4),
        )
    )
    changed = r.call("ChangeOfState__Q211CMemoryCard6Device")
    return memory, r.trace, changed, r.rd(ppc.symbols["gs_bChangeOfState"]["address"])


def card_text(r, case):
    status, found = case
    r.u.mem_write(OBJ, b"\xa5" * 160)
    r.wr(OBJ + 12, status)
    r.wr(OBJ + 144, 37, 101)
    r.wr(ppc.symbols["g_pPopUpMessageHandler"]["address"], 0x10040000)
    r.cstring(0x10050000, "%u/%u")

    def lookup():
        r.tracecall("lookup", r.rr(3), r.rr(4))
        return 0x10050000 if found else 0

    def sprintf():
        text = r.text(r.rr(4))
        if text != " ":
            text = "%s/%s" % (r.rr(5), r.rr(6))
        r.tracecall("format", text)
        r.cstring(r.rr(3), text)
        return len(text)

    r.at("GetString__17CUserMessageTablei", lookup)
    r.at("sprintf", sprintf)
    result = r.call("GetStatusText__C9CMCDevice", OBJ)
    return r.text(result), r.trace, bytes(r.u.mem_read(OBJ, 160))


class ExactSweepTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        initialize()
        cls.readers = sorted((ROOT / "config/GR8E69").glob("property_read_*.json"))
        require(
            *(native(p.stem) for p in cls.readers),
            native("property_dispatch"),
            native("card_device_update"),
            native("card_device_queries")
        )

    def test_property_reader_fields_signedness_and_flags(self):
        rng = random.Random(0x47523845)
        records = [bytes([v]) * 192 for v in (0, 0x7F, 0x80, 0xFF)]
        records += [bytes(rng.randrange(256) for _ in range(192)) for _ in range(4)]
        for manifest in self.readers:
            unit = json.loads(manifest.read_text())
            name = unit["functions"][0]["name"]
            runners = (Run(), Run(native(unit["id"])))
            for record in records:
                for r in runners:
                    r.u.mem_write(OBJ, record)
                for field in range(-2, 64):
                    results = [r.call(name, OBJ, field) for r in runners]
                    self.assertEqual(
                        results[0], results[1], (name, field, record.hex())
                    )
                for r in runners:
                    self.assertEqual(bytes(r.u.mem_read(OBJ, len(record))), record)

    def test_property_dispatch_type_ids(self):
        for runner in (Run(), Run(native("property_dispatch"))):
            for name in ppc.symbols:
                if name.startswith("_LoadPropertyData__"):
                    runner.at(
                        name,
                        lambda n=name: (
                            runner.tracecall(n, runner.rr(3), runner.rr(4)),
                            123,
                        )[1],
                    )
            for type_id in range(-1, 17):
                runner.trace.clear()
                runner.wr(OBJ + 12, type_id)
                result = runner.call(
                    "LoadPropertyData__FP18xyzProperty_Structi", OBJ, 51
                )
                self.assertEqual(result, 123 if 1 <= type_id <= 14 else 0)
                if 1 <= type_id <= 14:
                    type_name = [
                        "core",
                        "mechanic",
                        "enemy",
                        "powerup",
                        "wayPoint",
                        "pathPoint",
                        "envMod",
                        "mechanicEnvMod",
                        "projectileGenerator",
                        "projectileGeneratorTarget",
                        "cameraShaker",
                        "playerPathController",
                        "animatedLight",
                        "fogParams",
                    ][type_id - 1]
                    self.assertEqual(len(runner.trace), 1)
                    self.assertIn("MOH_" + type_name + "_Struct", runner.trace[0][0])
                    self.assertEqual(runner.trace[0][1:], (OBJ, 51))
                else:
                    self.assertEqual(runner.trace, [])

    def test_card_probe_transitions(self):
        pairs = [(-1, -1), (-3, 0), (0, -3), (0, 0), (1, 2)]
        for case in itertools.product((0, 1), (0, 1), pairs[:2], pairs, pairs):
            compare_case("card_device_update", case, card_update)

    def test_card_status_text(self):
        for case in itertools.product(range(-1, 7), (False, True)):
            compare_case("card_device_queries", case, card_text)


if __name__ == "__main__":
    unittest.main()
