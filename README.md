# Medal of Honor: Rising Sun

An early matching decompilation of **Medal of Honor: Rising Sun** for GameCube, targeting the USA release **GR8E69, Disc 1, revision 0**. The initial game-code target is the disc's `MOH3RDVD.ELF`.

[![Verified progress snapshot](https://github.com/lifewillbeokay/moh-rising-sun/actions/workflows/progress.yml/badge.svg)](https://github.com/lifewillbeokay/moh-rising-sun/actions/workflows/progress.yml)

**See [current matching progress and unfinished functions on decomp.dev](https://decomp.dev/lifewillbeokay/moh-rising-sun).** The project combines reconstructed game-specific C++ with compatible public library reconstructions, including Lua, Dolphin SDK, Newlib, GCC runtime, STLport and network helpers. Accepted source is checked against the original executable as part of a complete rebuilt analysis-image comparison. The remainder stays original binary context. AI-assisted contributions are welcome; see [CONTRIBUTING.md](CONTRIBUTING.md).

Progress counts verified, nonoverlapping source-built function bytes against all bytes in the original executable sections (`.init` and `.text`). Generated data and BSS are verified separately and earn no code-progress credit; neither do stripped functions, original context or debug metadata. Exact counts and input hashes live in the [verified snapshot](progress/GR8E69.snapshot.json). See [how progress is verified and published](docs/Progress.md) and the [reuse index](docs/ReuseMap.md) for library provenance, evidence and next work.

## Verified starting point

- `MOH3RDVD.ELF`: 3,628,980 bytes; SHA-1 `dd797412be7d631e78c98f194a1bb47981174d75`.
- 15,507 symbol entries, including 9,883 function entries and 954 source-file entries. Entries are not necessarily unique functions or complete original source units.
- Preserved DWARF 1 information covers seven Nintendo GBA-library compilation units. It is not full game-wide type information.
- The original-object relink matches the complete 2,860,576-byte DOL derived from the ELF, including its header. Every original allocated ELF file byte, entry point, and BSS extent is checked separately.
- Accepted game and library source passes that same complete-image comparison. Code and generated data are both checked; reconstructed game code and restored library code are reported separately. The [project manifest](config/GR8E69/project.json) lists accepted units and fragments.

This comparison is against a derived analysis image. It does **not** mean the original ELF's debug/symbol tables have been reproduced, the on-disc boot DOL has been replaced, or the game has been tested in an emulator. The boot DOL is a separate 206,016-byte program containing references to the game ELFs.

## Join the project — AI-assisted contributions welcome

Want to help bring Rising Sun back to readable source? Human and AI-assisted
contributions are welcome: reconstruct game functions, investigate compiler
behavior, improve verification tools, document evidence, or test the loading path.
You do not need to use AI to contribute. AI-generated suggestions receive the same
review and byte-verification requirements as other changes.

Start with the [next work](#next-work) below and [contribution guide](CONTRIBUTING.md).
Open an issue to coordinate a source unit or investigation, then send a focused
pull request with your evidence, verification results and any AI assistance used.
Pseudonymous contributors are welcome. Preserve upstream credit and license notices.

Bring your own game copy for local verification. This repository contains source,
configuration and tooling; please do not upload game images, executable binaries,
assets, compiler binaries or complete debug dumps to commits, issues or PRs.

## Research notes

The [research notes index](docs/research/README.md) collects contributor investigations
and their GameCube comparisons. Thanks to kyleckroeger for sharing PS2 research on
FlexProp names, particle recipes, script instructions, class descriptions, gameplay
data, interactive music, runtime timing and camera behavior. These notes provide
leads for reconstruction; each topic separates its evidence and open questions
from verified matching source.

## Progress integration

Explore the [live decomp.dev map](https://decomp.dev/lifewillbeokay/moh-rising-sun)
to find unfinished functions. The remaining code is divided into grey boxes:
named file groups where supported by symbols, unknown-file groups, shared entry
points and unidentified bytes. Inferred ownership is labeled and earns no matching
credit. See [code-map evidence and starting points](docs/CodeMap.md) and the
[reuse index and ranked next work](docs/ReuseMap.md). The new
[direct dependency map](docs/Dependencies.md) ranks shared helpers by unfinished
callers and preserves unresolved indirect calls.

The [progress workflow](.github/workflows/progress.yml) publishes an objdiff v2
`GR8E69_report` artifact compatible with decomp.dev. CI validates source hashes
against a locally verified snapshot; it does not rebuild the game. Source changes
require a fresh local complete-image verification before their progress can be
published. See [progress reporting and registration](docs/Progress.md).

## Local setup

Requires Python 3.9+ on macOS ARM64 or Linux x86-64 (including WSL2). On macOS, source compilation also uses existing Rosetta support; on Linux, wibo runs the Windows compilers natively. Tools are downloaded into ignored `build/tools/` and `build/compiler/` with pinned SHA-256 checksums. ProDG 3.8.1 with `-O2 -G0` is the working MathFun profile; the restored Lua library adds `-ffast-math` and uses native unused-function stripping. The SDK units use CodeWarrior GC/1.2.5n; EXI uses `-O3,p`, the other SDK units use `-O4,p`, with `-opt nopeep` for the documented fragments. Newlib libc uses ProDG 3.8.1 with `-O2 -G8 -fsigned-char -DMB_CAPABLE`; math uses `-O2 -G1024 -fno-builtin -msafe-sda` and the language profiles in [Newlib.md](docs/Newlib.md). GCC runtime helpers use `-O2 -G8`. The script interpreter handlers use ProDG 3.9.3; see [Script.md](docs/Script.md) for the compiler comparison and accepted scope. The original compiler releases remain unconfirmed.

```sh
python3 tools/import_disc.py "/path/to/Medal of Honor - Rising Sun (USA) (Disc 1).nkit.iso"
python3 tools/setup.py
python3 tools/audit.py
python3 tools/baseline.py
python3 tools/reconstruct.py
python3 -m unittest discover -s tests -v
```

The importer reads the standard GameCube filesystem table, including the provided uncompressed NKit v01 image. It extracts only four pinned executables into `orig/GR8E69/`, checks their sizes and SHA-1/SHA-256 digests, and leaves the input image untouched. This does not recover or verify a full retail-disc image. Other compressed formats and other revisions are not currently supported.

Run the scripts from any directory; all outputs remain inside this checkout. `baseline.py` and `reconstruct.py` recreate their own build directories, so keep experiments under `scratch/`.

## Outputs

| Path | Contents |
| --- | --- |
| `config/GR8E69/target.json` | Executable identities and hashes |
| `config/GR8E69/baseline.json` | Entry point, SDA bases and comparison layout |
| `build/audit/summary.json` | Section and symbol statistics |
| `build/audit/symbols.json` | Complete local symbol inventory |
| `build/audit/dwarf.txt` | Local debug-information dump |
| `build/audit/elf-config/` | Experimental dtk symbol/split exports |
| `build/baseline/report.json` | Verified baseline result, with zero source credit |
| `build/baseline/relinked.dol` | Rebuilt analysis image |
| `build/audit/dependencies.json` | Local direct-branch dependency graph and research ranking (`python3 tools/dependencies.py`) |
| `src/matrix/`, `include/game/CMatrix.h`, `include/game/CVector3.h` | Reconstructed matrix and [vector](docs/Matrix.md#cvector3-layout) functions and their shared layouts |
| `src/camera/`, `include/game/Camera.h` | Reconstructed [camera projection and transform functions](docs/Camera.md), guided by contributed runtime research |
| `src/sys_memory.cpp` | Four game allocation operators; original heap routines remain external |
| `src/stlport/` | Tree, container, sorting and heap functions using an attributed STLport 4.5.3 subset |
| `src/eagl/` | Reconstructed rendering state and material drawing fragments; [evidence and remaining work](docs/Rendering.md) |
| `src/eagl_anim/` | Reconstructed animation decoders, channels and support interfaces; [reference and evidence](docs/Animation.md) |
| `src/eagl_loading/` | Reconstructed EAGL symbol pools and dynamic-loader fragments; [reference and evidence](docs/Loading.md) |
| `src/particles/`, `src/flexprop/`, `src/string_crc.cpp` | Reconstructed [particle recipes, state, pool and clock helpers](docs/ParticleRecipes.md), [FlexProp lookup and string CRC](docs/FlexProp.md) |
| `src/scene/`, `include/game/SceneNode.h` | Recovered [scene-node virtual-slot order and base-class defaults](docs/SceneNode.md) |
| `src/script/` | Reconstructed [all opcode handlers, script timers, message registrations, event lookup and music built-ins](docs/Script.md), guided by contributed PS2 research |
| `src/bpd/`, `src/endian/` | Reconstructed [BPD/property conversion, scalar wrappers and lighting-volume point test](docs/BPD.md) |
| `src/lighting/`, `include/game/Light.h` | Reconstructed [light-volume selection and transitions, scene-light transforms and animated-light lifecycle](docs/LightVolumes.md) |
| `src/bullets/`, `include/game/Bullet.h` | Reconstructed [bullet defaults, accessors and collision helpers](docs/Bullets.md) |
| `src/triggers/` | Reconstructed [trigger-object queries, positions and point tests](docs/TriggerObject.md) |
| `src/sound/` | Reconstructed [ambient-track and sound accessors](docs/Sound.md) |
| `src/player/` | Reconstructed [camera-shake initialization, evaluation and player wrappers](docs/CameraShake.md) |
| `src/objectives/` | Reconstructed [objective initialization, status and visibility helpers](docs/Objectives.md) |
| `src/observers/` | Reconstructed observers, weak pointers and destruction queue; [evidence](docs/Observers.md) |
| `src/MathFun.cpp` | Reconstructed functions from the original MathFun unit |
| `src/lua/` | Restored Lua 4.0.1 source units, headers and copyright notice |
| `src/dolphin/`, `include/dolphin-sdk/` | Accepted SDK source units, reference headers and attribution |
| `src/newlib/`, `include/newlib/` | Accepted Newlib units, supporting headers and attribution |
| `src/libgcc/` | Verified GCC 2.95.2 runtime helpers, target configuration and notices |
| `config/GR8E69/MathFun.json` | Function ranges, external references and compiler profile |
| `config/GR8E69/project.json` | Accepted unit list, source provenance and fixed progress denominator |
| `build/reconstruction/report.json` | Full-image result and source/tool fingerprints |
| `build/reconstruction/units/` | Per-unit compiler, linker and generated-section output |
| `build/reconstruction/rebuilt.dol` | Analysis image containing all accepted compiled source |
| `progress/GR8E69.snapshot.json` | Verification metadata, source hashes and unfinished-code map; no original bytes |
| `build/progress/report.json` | Generated objdiff v2 progress for decomp.dev |

The automatic dtk ELF split export emits warnings for this executable's symbol ordering. Treat it as research output; it is not accepted source ownership. The baseline uses a separate original-object split and retains every compared byte.

## Next work

1. Extend the [animation formats](docs/Animation.md) using the shared delta-decoder interfaces and attributed NFS reference, and the [rendering reconstruction](docs/Rendering.md) through its remaining lighting blocks, startup registration and PCode interpreter. The [reuse index](docs/ReuseMap.md) ranks other candidates, including matrix, container, string and allocation helpers. Preserve the documented limits of shared storage views; establish missing types and constant ownership before adding callers that depend on them.
2. Distinguish the original compiler release using larger functions. Five tested SN/ProDG versions match the accepted fragment, so its byte match alone cannot identify the original release. Nintendo libraries may use different compilers.
3. Extend the network-library subset by recovering the older target's behavior and storage; the remaining TCP, UDP, PPP and Ethernet reference routines differ. Continue shared game-code reconstruction alongside library restoration. Only compiled source bytes that pass complete-image verification count toward progress.
4. Inspect Disc 2 and test the game's executable-loading path before claiming complete game coverage or a runnable replacement disc.

See [the initial audit](docs/initial-audit.md) for evidence and limitations.

## License

Original project contributions are dedicated under [CC0 1.0 Universal](LICENSE),
to the extent contributors hold the relevant rights. Third-party source and
adaptations retain their existing terms and notices; the dedication grants no
rights in the original game or its assets. See [licensing scope and third-party
notices](docs/Licensing.md).

## Tooling

Uses [decomp-toolkit](https://github.com/encounter/decomp-toolkit) 1.8.4, [gc-wii-binutils](https://github.com/encounter/gc-wii-binutils) 2.42-2, and [wibo](https://github.com/decompals/wibo) 1.0.3. The project layout follows the separation of originals, source and build output documented by [dtk-template](https://github.com/encounter/dtk-template); its standard CodeWarrior build configuration has not been adopted for the SN-compiled game code.
