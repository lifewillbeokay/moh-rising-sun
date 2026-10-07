# Direct dependency map and next shared-code targets

Run `python3 tools/dependencies.py` to generate
`build/audit/dependencies.json` from the pinned original. The graph remains
Git-ignored. It contains symbol names, branch sites, target evidence, candidate
dependencies and rankings, with no source-progress credit. Accepted status comes
from the current manifests; this analysis command does not verify their builds.

The initial scan found **9,849 nonoverlapping function ranges**, **31,677 direct
dependency sites**, **105 unresolved direct sites** and **4,777 register-indirect
call sites**. A range may contain several overlapping function symbols; aliases
are preserved together instead of duplicating instructions or inventing unique
ownership. Direct dependency sites include recursive calls and candidate
inter-function branches, not just ordinary calls.

Rankings count distinct unfinished caller ranges, not call sites. Recursive
self-calls do not increase that count. Caller-byte totals are unique within each
row but overlap across rows; they are not a prediction of matching coverage.

## Useful next work packets

Counts below are the earlier checkpoint following the container, Lua and network-helper additions. They will change as
manifests grow; regenerate the report for current counts. A small function with many callers is a type/contract research lead,
not necessarily the easiest source match.

| Target | Address | Code bytes | Unfinished callers | Work to unlock |
| --- | --- | ---: | ---: | --- |
| `DWI_alloc` | `0x801a8670` | 212 | 139 | Follow the small-allocation path and main heap call; establish flag and failure behavior. Allocation operators now provide a verified caller. |
| `FEHashUpper` | `0x800499a4` | 100 | 116 | Establish exact character conversion, signedness and hash recurrence; find callers' string ownership without replacing the algorithm with a generic hash. |
| `DWI_free` | `0x801a8894` | 64 | 75 | Recover `freeSmall`'s result contract and the fallback to `MEM_free`. |
| `GetStringCRC` | `0x801acb98` | 80 | 60 | Identify the CRC table/algorithm and signedness before selecting a public implementation. |

The CMatrix work packet now has 23 accepted functions and a shared
64-byte matrix representation, including initialization, assignment and
multiplication. See [the matrix and vector layout evidence](Matrix.md).
Their earlier unfinished-caller counts were 173, 127 and 75; these overlapping
counts do not represent newly reconstructed callers. Further matrix work can reuse the recovered vector construction and assignment
behavior, while checking each caller's aliasing and temporary lifetimes. The small string and allocation helpers above remain useful
independent work packets. The larger library path remains
[STLport container instantiations](STLport.md). The builtin/pointer subset is
now accepted; remaining custom comparators and game value types require further
evidence. These are independent research tracks; a pointer-container match
does not establish the pointed-to game class layout.

## Evidence and limitations

The scanner decodes aligned PowerPC I-form and B-form branches inside sized
executable function symbols. It handles signed displacements, absolute branches,
link bits and conditional branch options. A destination must equal an original
function entry to become a named dependency. Cross-function interior destinations
and unknown destinations remain unresolved. Ordinary intra-function control flow
is omitted. Unlinked inter-function branches are labeled `branch`, not proven
tail calls.

This is not control-flow reachability analysis. A syntactic branch can be on a
path never executed. Register-indirect calls are counted without inferring their
targets; virtual dispatch, callbacks and indirect tail branches are not resolved.
The report does not recover data references, vtables, C++ types, or dynamic call
frequencies. Source file ownership is not inferred from call relationships.

Synthetic tests cover backward/absolute/conditional branches, address wrap,
indirect calls, returns, aliases, repeated calls, recursive calls, unresolved
destinations, and invalid function bounds. They run without original game files.
The graph is a prioritization aid; source acceptance still requires the complete
verification described in [Progress.md](Progress.md).
