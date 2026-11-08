"""Public reporting safeguards run without the original game or compilers."""
import copy
import json
import sys
import tempfile
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
from progress_report import export, fingerprints


class PublicProgress(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)
        self.config = self.root / "config/GR8E69"
        self.config.mkdir(parents=True)
        (self.root / "src").mkdir()
        (self.root / "src/example.c").write_text("int example(void) { return 1; }\n")
        self.manifest = {"id": "example", "source": "src/example.c",
                         "category": "reconstructed_game",
                         "functions": [{"name": "example", "address": "0x80001000", "size": 8}]}
        self.write("example.json", self.manifest)
        self.write("project.json", {"units": ["example.json"],
                                   "progress": {"code_bytes": 100, "executable_sections": {".text": 100}}})
        self.write("baseline.json", {"sections": {".text": "0x80001000"}})
        self.write("comparisons.json", {"schema_version": 1, "units": []})
        (self.root / "tools").mkdir()
        self.tool_hash = "a" * 64
        (self.root / "tools/toolchain.json").write_text(json.dumps({"objdiff": {
            "version": "test", "platforms": {"test": {"sha256": self.tool_hash}}}}))
        self.snapshot = {"schema_version": 3, "verification": "local-complete-image-match",
                         "input_sha256": fingerprints(self.root),
                         "units": [{"id": "example", "category": "reconstructed_game",
                                    "functions": 1, "code_bytes": 8}],
                         "progress": {"matching_code_bytes": 8, "total_executable_code_bytes": 100,
                                      "matching_functions": 1,
                                      "categories": {"reconstructed_game": 8, "restored_library": 0}},
                         "code_map": {"schema_version": 1,
                                      "sections": [{"name": ".text", "address": "0x80001000", "size": 100}],
                                      "units": [{"name": "Unknown file/example", "ranges": [
                                          {"kind": "function", "name": "unfinished", "address": "0x80001008", "size": 92}]}]}}

        self.snapshot["original_elf_sha256"] = "b" * 64
        self.snapshot["comparisons"] = {"schema_version": 1,
            "original_elf_sha256": self.snapshot["original_elf_sha256"],
            "input_sha256": fingerprints(self.root), "objdiff_version": "test",
            "objdiff_sha256": self.tool_hash, "compiler_executables": {}, "units": []}

    def refresh(self):
        self.snapshot["input_sha256"] = fingerprints(self.root)
        self.snapshot["comparisons"]["input_sha256"] = fingerprints(self.root)

    def add_comparison(self, exact=False):
        f = {"name": "unfinished", "address": "0x80001008", "size": 92, "binding": 1}
        self.write("comparisons.json", {"schema_version": 1, "units": ["partial.json"]})
        self.partial = {"id": "partial", "source": "src/example.c", "category": "reconstructed_game",
                        "profile_from": "example.json", "evidence": "test", "context_units": [],
                        "sections": [{"name": ".text", "address": f["address"], "size": 92}],
                        "functions": [f], "comparison_functions": ["unfinished"]}
        self.write("partial.json", self.partial)
        self.result = dict(f, compiled_address=f["address"], compiled_size=92,
                           target_sha256="a"*64, compiled_sha256=("a" if exact else "c")*64,
                           differing_bytes=0 if exact else 8,
                           status="exact_unlinked" if exact else "partial",
                           objdiff_percent=100 if exact else 90, fuzzy_match_percent=100 if exact else 90)
        self.snapshot["comparisons"]["units"] = [{"id": "partial", "functions": [self.result],
            **{k: "d"*64 for k in ("object_sha256", "linked_sha256", "target_object_sha256", "diff_object_sha256", "diff_sha256")}}]
        self.refresh()

    def write(self, name, value):
        (self.config / name).write_text(json.dumps(value))

    def test_context_preserves_whole_executable_denominator(self):
        report = export(self.snapshot, self.root)
        self.assertEqual(report["version"], 2)
        self.assertEqual(report["measures"]["matched_code_percent"], 8)
        self.assertEqual(sum(int(u["measures"]["total_code"]) for u in report["units"]), 100)
        self.assertEqual(report["units"][-1]["measures"]["matched_code"], "0")
        self.assertNotIn("total_functions", report["measures"])
        self.assertNotIn(str(self.root), json.dumps(report))

    def test_partial_score_does_not_earn_exact_or_linked_credit(self):
        self.add_comparison()
        report = export(self.snapshot, self.root)
        self.assertEqual(report["measures"]["matched_code"], "8")
        self.assertEqual(report["measures"]["complete_code"], "8")
        self.assertAlmostEqual(report["measures"]["fuzzy_match_percent"], 90.8)
        self.assertEqual(sum(int(u["measures"]["total_code"]) for u in report["units"]), 100)
        self.assertFalse(report["units"][-1]["metadata"]["complete"])

    def test_exact_unlinked_credit_is_separate(self):
        self.add_comparison(exact=True)
        report = export(self.snapshot, self.root)
        self.assertEqual(report["measures"]["matched_code"], "100")
        self.assertEqual(report["measures"]["complete_code"], "8")
        self.assertEqual(report["measures"]["fuzzy_match_percent"], 100)

    def test_promotion_moves_credit_to_linked_without_double_counting(self):
        self.add_comparison(exact=True)
        self.write("promoted.json", {"id": "promoted", "source": "src/example.c",
                                   "category": "reconstructed_game", "functions": self.partial["functions"]})
        self.write("project.json", {"units": ["example.json", "promoted.json"],
                                   "progress": {"code_bytes": 100, "executable_sections": {".text": 100}}})
        self.write("comparisons.json", {"schema_version": 1, "units": []})
        self.snapshot["comparisons"]["units"] = []
        self.snapshot["units"].append({"id": "promoted", "category": "reconstructed_game",
                                       "functions": 1, "code_bytes": 92})
        self.snapshot["progress"].update(matching_code_bytes=100, matching_functions=2,
                                         categories={"reconstructed_game": 100, "restored_library": 0})
        self.snapshot["code_map"]["units"] = []
        self.refresh()
        report = export(self.snapshot, self.root)
        self.assertEqual(report["measures"]["matched_code"], "100")
        self.assertEqual(report["measures"]["complete_code"], "100")
        self.assertEqual(sum(int(u["measures"]["total_code"]) for u in report["units"]), 100)

    def test_false_exact_and_nonfinite_scores_are_rejected(self):
        for change in ({"status": "exact_unlinked"}, {"objdiff_percent": float("nan")},
                       {"fuzzy_match_percent": 100}, {"differing_bytes": -1}, {"differing_bytes": 0}):
            with self.subTest(change=change):
                self.add_comparison()
                self.result.update(change)
                with self.assertRaisesRegex(ValueError, "measurement"):
                    export(self.snapshot, self.root)

    def test_objdiff_100_is_capped_when_bytes_differ(self):
        self.add_comparison()
        self.result.update(objdiff_percent=100, fuzzy_match_percent=99.99)
        self.assertLess(export(self.snapshot, self.root)["measures"]["fuzzy_match_percent"], 100)

    def test_comparison_must_cover_the_complete_mapped_function(self):
        self.add_comparison()
        self.partial["functions"][0]["size"] = 88
        self.result["size"] = 88
        self.write("partial.json", self.partial)
        self.refresh()
        with self.assertRaisesRegex(ValueError, "complete mapped"):
            export(self.snapshot, self.root)

    def test_comparisons_cannot_double_count_linked_code(self):
        self.add_comparison()
        self.partial["functions"][0]["address"] = "0x80001000"
        self.write("partial.json", self.partial)
        self.refresh()
        with self.assertRaisesRegex(ValueError, "Overlapping"):
            export(self.snapshot, self.root)

    def test_comparison_cannot_select_private_compiler_flags(self):
        self.add_comparison()
        self.partial["compiler_flags"] = ["-O3"]
        self.write("partial.json", self.partial)
        self.refresh()
        with self.assertRaisesRegex(ValueError, "reuse"):
            export(self.snapshot, self.root)

    def test_stale_comparison_receipt_is_rejected(self):
        self.add_comparison()
        self.snapshot["comparisons"]["input_sha256"] = {}
        with self.assertRaisesRegex(ValueError, "Stale"):
            export(self.snapshot, self.root)

    def test_missing_or_duplicate_comparison_receipt_is_rejected(self):
        self.add_comparison()
        for records in ([], self.snapshot["comparisons"]["units"] * 2):
            self.snapshot["comparisons"]["units"] = records
            with self.assertRaisesRegex(ValueError, "inventory"):
                export(self.snapshot, self.root)

    def test_changed_source_is_stale(self):
        (self.root / "src/example.c").write_text("changed\n")
        with self.assertRaisesRegex(ValueError, "Stale"):
            export(self.snapshot, self.root)

    def test_added_source_is_stale(self):
        (self.root / "src/extra.c").write_text("new\n")
        with self.assertRaisesRegex(ValueError, "Stale"):
            export(self.snapshot, self.root)

    def test_removed_source_is_stale(self):
        (self.root / "src/example.c").unlink()
        with self.assertRaisesRegex(ValueError, "Stale"):
            export(self.snapshot, self.root)

    def test_inflated_snapshot_total_is_rejected(self):
        self.snapshot["progress"]["matching_code_bytes"] += 1
        with self.assertRaisesRegex(ValueError, "totals"):
            export(self.snapshot, self.root)

    def test_overlapping_functions_are_rejected(self):
        self.manifest["functions"].append(copy.deepcopy(self.manifest["functions"][0]))
        self.write("example.json", self.manifest)
        self.snapshot["input_sha256"] = fingerprints(self.root)
        self.snapshot["units"][0].update(functions=2, code_bytes=16)
        with self.assertRaisesRegex(ValueError, "Overlapping"):
            export(self.snapshot, self.root)

    def test_duplicate_units_are_rejected(self):
        self.snapshot["units"].append(copy.deepcopy(self.snapshot["units"][0]))
        with self.assertRaisesRegex(ValueError, "Duplicate"):
            export(self.snapshot, self.root)

    def test_map_cannot_double_count_matched_source(self):
        item = self.snapshot["code_map"]["units"][0]["ranges"][0]
        item.update(address="0x80001000", size=100)
        with self.assertRaisesRegex(ValueError, "coverage"):
            export(self.snapshot, self.root)

    def test_missing_map_bytes_are_rejected(self):
        self.snapshot["code_map"]["units"][0]["ranges"][0]["size"] -= 4
        with self.assertRaisesRegex(ValueError, "coverage"):
            export(self.snapshot, self.root)

    def test_map_cannot_move_the_original_section(self):
        self.snapshot["code_map"]["sections"][0]["address"] = "0x80002000"
        with self.assertRaisesRegex(ValueError, "pinned"):
            export(self.snapshot, self.root)

    def test_unidentified_bytes_are_not_reported_as_functions(self):
        self.snapshot["code_map"]["units"][0]["ranges"][0]["kind"] = "unidentified"
        unit = export(self.snapshot, self.root)["units"][-1]
        self.assertFalse(unit["functions"])
        self.assertEqual(unit["sections"][0]["size"], "92")
        self.assertEqual(unit["measures"]["matched_code"], "0")


if __name__ == "__main__":
    unittest.main()
