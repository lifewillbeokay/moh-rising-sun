# Agent instructions

Read README.md and CONTRIBUTING.md before changing this project. AI-assisted contributions are allowed.

Keep the original game and generated research under ignored directories. Do not alter the supplied disc image. Verify the pinned target before analysis or building.

Report original-object baseline success, individual function matches, complete source-build results and runtime tests separately. Do not count original bytes as reconstructed source.

Preserve symbol evidence and unknowns. Do not invent types, original names or source boundaries to get a match. Keep experiments in scratch/.

Run checks appropriate to the change. The original-object baseline command is `python3 tools/baseline.py`; the source build is `python3 tools/reconstruct.py`. Validation tests are `python3 -m unittest discover -s tests -v`. Read `docs/MathFun.md`, `docs/Lua.md`, `docs/Dolphin.md` `docs/Newlib.md` and `docs/Libgcc.md` before extending their respective units; working compiler profiles are not proof of the original compiler release.

Exact progress counts verified matching source-built function bytes against all 2,492,032 original executable section bytes. Linked progress counts only source ranges in the verified image; fuzzy similarity is reported separately. Preserve the separate reconstructed-game and restored-library categories, upstream attribution and licenses. Never count original context, data bytes, discarded functions or overlapping ranges toward code progress.

Read docs/ParticleRecipes.md and docs/FlexProp.md before extending particle recipe, FlexProp or string-CRC interfaces. Read docs/Loading.md before extending EAGL loader or symbol-pool interfaces. Read docs/Animation.md before extending EAGL animation formats and docs/Rendering.md before extending rendering, property-parser or TAR interfaces. Read docs/Network.md before extending the network-library subset. Read docs/STLport.md before extending STL templates and docs/Memory.md before extending allocation wrappers. Read docs/Matrix.md before extending game matrix/vector declarations or routines. Use tools/dependencies.py and docs/Dependencies.md to prioritize shared helpers; direct branch counts do not establish class layouts, runtime reachability or source credit.

Check the repository-local commit identity before committing; never inherit a personal identity without authorization. Preserve contributors' pseudonymity and upstream attribution. Follow docs/Progress.md when changing source, headers, configuration or tools: regenerate the public snapshot only after complete local verification. CI validates a snapshot and must not be described as rebuilding the game.

Read docs/Script.md before extending the behaviour-script interpreter, class/state views or built-in function interface. Its runtime storage views do not establish complete class allocations or on-disc formats; check PS2 research claims independently against GR8E69.

Read docs/BPD.md before extending BPD/property-record conversion, endian helpers or light-volume interfaces. Preserve conversion order and unknown fields; the prefix views do not establish complete object allocations or GameCube file layouts.

Maintain useful partial reconstructions in normal source with registered comparisons; matching incompleteness alone is not a review failure. Time-box compiler matching experiments and publish reproducible measurements and unresolved assumptions. Keep exploratory variants in scratch/. Follow docs/Progress.md's partial-source workflow: partial, exact-unlinked and linked-verified are distinct. Never substitute mismatching code into the verified build or count similarity as exact coverage. Whole exact functions may be promoted independently of unfinished neighbors, with complete data/dependency checks and a fresh full-image verification. Use the shared unit profile and DECOMP_COMPARE body guards; do not introduce function-specific flags or patches.
