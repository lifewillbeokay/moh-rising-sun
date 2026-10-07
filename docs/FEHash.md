# Front-end uppercase hash

The [NFS Underground symbol map](research/ea-shared-code.md) supplied a useful
same-era reference: its `FEHashUpper(const char *)` has the same name and 100-byte
size. That is corroboration only. Both bodies here were independently
reconstructed from the pinned GR8E69 instructions with Codex assistance.

| Function | Address | Code bytes |
| --- | --- | ---: |
| `FEUpperCase(char)` | `0x800495f0` | 24 |
| `FEHashUpper(const char *)` | `0x800499a4` | 100 |

`FEUpperCase` converts only ASCII `a` through `z` by subtracting 32. Every other
byte is preserved. It does not use the C library locale or alter accented bytes.
The original caller sign-extends the argument, and the helper sign-extends its
converted result; the working compiler profile explicitly selects signed `char`.

The hash starts at `0xffffffff`. For each nonzero input byte it performs
`hash += hash << 5`, then adds the uppercased result converted to an unsigned byte.
Arithmetic wraps in 32-bit unsigned storage. The multiply/add order, signed
argument and unsigned result conversion match the original instructions.
Null and empty inputs both return `0xffffffff`; the terminator is not hashed.
The public return declaration preserves those bits, without claiming an original
typedef or broader front-end string ownership contract.

Both complete generated text sections match with ProDG 3.8.1 and
`-O2 -G0 -fno-exceptions -fno-implicit-templates -fsigned-char`. They emit no data.
Only these 124 bytes earn new source credit; the many callers remain independent
work. The earlier dependency scan counted 116 unfinished callers of the hash,
which is a prioritization measure rather than additional progress.

No source or implementation was imported from the reference repository. The
complete-image reconstruction, test suite and snapshot checks apply as described
in [Progress.md](Progress.md). Runtime behavior remains untested.
