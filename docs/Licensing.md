# Licensing

Original contributions to this project are dedicated to the public domain under
[CC0 1.0 Universal](../LICENSE), to the extent their contributors hold copyright
and related rights in those contributions. This includes project-written tooling,
documentation, configuration and original reconstruction work, subject to the
third-party exclusions below. CC0 includes a fallback license where its waiver
cannot take full effect; the unmodified legal text is in the root `LICENSE`.

CC0 is also used by the [Metroid Prime](https://github.com/PrimeDecomp/prime/blob/main/LICENSE)
and [Battle for Bikini Bottom](https://github.com/bfbbdecomp/bfbb/blob/main/LICENSE)
decompilation projects. The official text is available from
[Creative Commons](https://creativecommons.org/publicdomain/zero/1.0/legalcode.en).

## Third-party material

The root dedication does not relicense imported source, headers, library code or
their adaptations. Their existing terms and notices continue to apply, including
to generated code from third-party templates. A file's presence here, or its
matching the original executable, does not establish permission to reuse it under
CC0. Where a reference supplies no license, this project grants no rights in that
reference's material.

| Material | Applicable notices and provenance |
| --- | --- |
| Lua 4.0.1 in `src/lua/` | [Lua copyright and permission notice](../src/lua/COPYRIGHT); [integration details](Lua.md) |
| Newlib sources and headers | [COPYING.NEWLIB](../src/newlib/COPYING.NEWLIB), per-file notices and [NOTICE](../src/newlib/NOTICE); [SN-derived fragments](../src/newlib/NOTICE.SN) retain their own notices |
| GCC runtime in `src/libgcc/` | [GNU GPL v2](../src/libgcc/COPYING), the linking exception in [libgcc2.c](../src/libgcc/libgcc2.c), and [NOTICE](../src/libgcc/NOTICE) |
| GCC-derived compiler support headers | [Compiler-header notice](../include/newlib-compiler/NOTICE) and [GPL text](../include/newlib-compiler/COPYING); the runtime exception is not asserted for unrelated headers |
| STLport algorithms and headers | Per-file copyright/permission notices, [STLport provenance](STLport.md) and [container notice](../src/stlport/CONTAINERS_NOTICE); the GCC `new`/`exception` headers have a separate [GPL text](../src/stlport/COPYING.GCC) |
| Dolphin SDK reconstructions and headers | [SDK notice](../src/dolphin/NOTICE) and [provenance](Dolphin.md); the Prime-derived subset retains [LICENSE.PrimeDecomp](../src/dolphin/LICENSE.PrimeDecomp). The pinned dolsdk2004 reference has no repository-wide license grant |
| Network and Ethernet reconstructions and headers | [Network notice](../src/dolphin/NETWORK_NOTICE) and [provenance](Network.md); the BFBB-derived subset retains [LICENSE.bfbb](../src/dolphin/LICENSE.bfbb). The pinned Cuyler36 references have no repository-wide license grant |
| EAGL animation adaptations | [NFS Most Wanted attribution](../src/eagl_anim/NOTICE), [upstream CC0 license](../src/eagl_anim/LICENSE.nfsmw) and [target evidence](Animation.md) |
| EAGL loader adaptations | [NFS Most Wanted attribution](../src/eagl_loading/NOTICE), [upstream CC0 license](../src/eagl_loading/LICENSE.nfsmw) and [target evidence](Loading.md) |
| Frontline property-reader adaptations | Upstream CC0 1.0, the matching [project license](../LICENSE), and [pinned source attribution and target changes](research/broad-source-sweep.md#frontline-reuse-and-property-readers) |
| Frontline collision adaptations | Upstream CC0 1.0, the matching [project license](../LICENSE), and [pinned accepted-source/draft attribution and target differences](research/collision-batch.md), including the [distance/traversal follow-up](research/collision-distance-traversal.md) |
| Frontline semantic references in the frontend, AI, audio and rendering batch | Upstream CC0 1.0, the matching [project license](../LICENSE), and [pinned attribution](research/rising-sun-only-batch.md) |
| RSA MD5 implementation | The copyright and permission notice in [md5.c](../src/dolphin/eth/md5.c) |

Per-file notices and the provenance manifests under `config/GR8E69/` identify the
sources and adaptations more precisely than directory names alone. Project-written
glue does not replace the terms of the libraries it uses. Preserve all applicable
copyright, permission and attribution notices when reusing these components.

## Original game and tools

This dedication grants no rights in the original game, executable code, assets,
Nintendo SDK material, trademarks or third-party compiler/tool binaries. Those
rights remain with their respective holders. Bring your own game copy; original
images, binaries and assets remain outside the repository. Downloaded build tools
retain their own licenses.

## Contributions

Submit original contributions under the project's CC0 dedication, only for rights
you are entitled to dedicate. For third-party material or adaptations, preserve
the applicable upstream terms and record the source, revision and modifications.
Identify any licensing uncertainty during review. AI-assisted contributions follow
the same requirements; see [CONTRIBUTING.md](../CONTRIBUTING.md).
