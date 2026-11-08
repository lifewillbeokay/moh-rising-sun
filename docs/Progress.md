# Progress reports and decomp.dev

The project exports the [objdiff report v2 schema](https://github.com/encounter/objdiff/blob/0c48d711c7bd51f791b353d7d85ba948b277e2f2/objdiff-core/protos/report.proto)
using its own byte-verification tooling. GitHub Actions uploads `report.json` in
an artifact named `GR8E69_report`, as required by the
[decomp.dev integration guide](https://decomp.wiki/tools/decomp-dev).
There is no decomp.dev-specific repository metadata file.

## Verification boundary

The public workflow validates a **locally verified snapshot**. It does not have
the original executables and does not rerun the proprietary compiler toolchain.
`progress/GR8E69.snapshot.json` records hashes of every source file, header,
configuration file and build script, plus verified unit counts and object hashes.
Snapshot schema 3 includes a selected executable-symbol map for unfinished
code and source-pinned comparison receipts, generated from the pinned original
during capture.
Changes, additions and removals in those directories make CI fail until someone
with the pinned original completes verification and refreshes the snapshot.
Documentation-only changes do not require rebuilding.

To refresh after an accepted source/tooling change, on a supported local machine:

```sh
python3 tools/reconstruct.py
python3 tools/comparisons.py
python3 -m unittest discover -s tests -v
python3 tools/progress_report.py --capture
```

Capture recompiles registered comparisons with pinned tools, then records their
complete-function measurements and artifact hashes. It also rechecks the complete
rebuilt image against its pinned identity, original
load layout, source ranges and compiled unit hashes/bytes. It emits only selected
public metadata, never original bytes or local filesystem paths. Commit the
snapshot with the source change. Maintainers must review snapshot updates;
fingerprints detect stale inputs, not fabricated assertions or a malicious verifier.

CI runs the tests, checks the complete input inventory and generates the objdiff
report. Anyone can reproduce that public export without a game image:

```sh
python3 tools/progress_report.py
```

## Counting rules

The overall denominator is every allocated executable byte in the pinned
`MOH3RDVD.ELF`: 2,492,032 bytes. Only nonoverlapping, verified source-built function
bytes earn credit. Restored library code and reconstructed game code are distinct
categories. Accepted units may be fragments of an original translation unit.
Their categories describe accepted ranges, so 100% within an accepted category
does not mean all game code or all library code is done.

Unreconstructed code is represented by named file groups, explicitly unassigned
function groups, shared entry-point groups, and unidentified code/padding ranges.
See [the code map](CodeMap.md) for evidence and limitations. Unmeasured ranges are
generated
inventory units with zero matching credit. Registered comparison functions are
removed from these inventory groups and displayed in source comparison units.
Their ranges plus linked source and comparison source must partition the complete
executable with no gaps or overlaps. Inferred file
groups are labeled; their game vs. library ownership is not exhaustively
classified. Data, BSS and original context earn no code credit. Unknown total
function/unit/data counts are omitted rather than inferred from overlapping
symbol entries, synthetic navigation groups or partial source boundaries.
The report distinguishes three measures:

- `matched_code`: complete byte-exact functions, including exact-unlinked source.
- `complete_code`: source functions linked into the verified image.
- `fuzzy_match_percent`: pinned objdiff similarity, weighted by **target function
  size**, plus 100% for exact source and 0% for unmeasured ranges. This is not an
  estimate of completed code or proof of semantic equivalence.

Accepted game/library fragments and unlinked game/library comparisons have
separate categories. Data is checked but does not affect these code measures.
The snapshot's original `progress` block continues to describe the exact linked
build; its `comparisons` receipt adds the unlinked measurements used by the exporter.
This is progress on the initial Disc 1 executable, not both discs or the whole
game. Runtime remains untested.

## Partial source workflow

1. Put readable source in `src/` and shared interfaces in `include/`. Guard
   unfinished bodies and comparison-only helpers with `DECOMP_COMPARE` when
   necessary to keep existing exact units unchanged. Record evidence and open
   questions in a topic document.
2. Add a manifest to the `comparisons.json` registry. It uses the exact-build
   manifest's sections, functions, externals and discard inventories, with a
   `profile_from` reference to an accepted unit instead of private compiler flags.
   Declare **all** retained functions, complete original ranges, generated data,
   absent inline bodies and dependencies. `comparison_functions` selects whole
   functions to measure. Other emitted functions must be previously linked,
   byte-exact context from explicit `context_units`; they earn no second credit.
3. Run `python3 tools/comparisons.py`. Local output in `build/comparisons/`
   includes native objects, native links, byte counts/hashes, compiler logs and
   objdiff JSON. Inspect it while iterating. A nonexact function is a successful
   comparison result, not a failed match-build requirement.
4. Run the complete source build and tests, then capture the public snapshot.
   Capture reproduces comparisons, pins their inputs/tool versions/hashes and
   publishes selected metadata only. CI validates those receipts and exports
   the report; it does not run the proprietary compiler or possess originals.

The initial implementation supports ProDG profiles, a contiguous original text
range, complete named function coverage and **exact generated data**. Code size
may differ, provided all retained code is accounted for. Unchanged linked context
must retain its original addresses and bytes; a size change that moves that
context fails until the source is split into an independently evidenced fragment.
Data/BSS mismatches, unknown dependencies or unplaced output require further
reconstruction or a separately reviewed tooling extension. They cannot be waived
as partial matches. Comparison units cannot overlap each other or borrow another
unit's storage outside their explicit context inventory.

Objdiff 3.8.3 computes instruction similarity. It does not accept this executable's
full symbol metadata or native linked addresses directly. The tool therefore
wraps complete, unchanged original and natively linked ranges in local diagnostic
objects, checks their bytes and symbols, and compares those with objdiff. Native
objects and instructions are never patched. The native link still resolves every
retained dependency, and full-function byte/address/size checks independently
determine exactness. Nonexact functions are capped at 99.99 in the exported
fuzzy field so an objdiff score of 100 cannot imply exact bytes; raw scores
remain in the receipt.
Raw object/diff files contain original bytes and stay ignored.

To promote a function, review behavior, types, boundaries and data ownership,
then configure a normal exact unit or fragment in `project.json`. Enable the
reviewed body for that build without enabling unfinished neighbors. Remove its
name from `comparison_functions`; if the comparison still emits it, add its
accepted unit to `context_units`. Remove an empty comparison registration.
Require the exact unit's entire allocated output and the whole rebuilt image to
match. Keep the remaining source measurable; do not discard an unfinished target
function or clip a constant pool to force promotion. No automatic promotion occurs
when a comparison reaches 100%.

## Registering the project

The project is registered at
<https://decomp.dev/lifewillbeokay/moh-rising-sun>.

Once a successful default-branch workflow has uploaded its report, a repository
admin can add the repository at <https://decomp.dev/manage/new>, naming the game
**Medal of Honor: Rising Sun** and platform **GameCube**. Repository creation and
report publication do not themselves register a project on decomp.dev. Its GitHub
app is optional for faster updates and PR comments; the site also polls reports.
