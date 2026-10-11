# Camera projection and transforms

Twenty `CCamera` functions compile to **1,768 matching executable bytes** in
six fragments, with twenty-four generated read-only bytes. They use ProDG 3.8.1
with `-O2 -G0 -fno-exceptions -fno-implicit-templates`; this working profile
does not identify the original compiler release.

The starting point was kyleckroeger's [runtime research from PR #11](research/ps2/runtime-rules.md),
particularly its finding that `SetPerspective` takes half-angles. The contributor
used Claude for those notes. This source reconstruction and its independent
GameCube verification used Codex. No source was imported from the PS2 remake.

## Accepted fragments

| Manifest | Original text range (end exclusive) | Functions | Code bytes |
| --- | --- | ---: | ---: |
| `camera_get_transform` | `0x8012bdc4`–`0x8012bdf0` | 1 | 44 |
| `camera_axes` | `0x8012bdf0`–`0x8012bf40` | 4 | 320 |
| `camera_transforms` | `0x8012bf40`–`0x8012c188` | 8 | 584 |
| `camera_projection` | `0x8012c4a0`–`0x8012c5b0` | 5 | 272 |
| `camera_world_to_camera` | `0x8012c768`–`0x8012c910` | 1 | 424 |
| `camera_update_projection` | `0x8012c910`–`0x8012c98c` | 1 | 124 |

The functions are `GetTMLocalToWorld`, `PreTransform`, `Reset`, `Transform`,
`SetTMLocalToWorld`, `SetPosition`, `SetBasis`, `Move`, `Rotate`,
`Orthonormalize`, `SetPerspective`, `SetOrthographic`, `GetHFOV`, `GetVFOV`
and `UpdateCameraToClip`, `UpdateWorldToCamera`, plus the four vector getters `GetPosition`,
`GetRightward`, `GetForward` and `GetUpward`. Each manifest records individual original symbol
names, addresses and sizes. The adjacent `camera.cpp` and `draw_context.cpp`
compiler markers bound these global functions at `0x8012ba60`–`0x8012d26c`,
supporting the code map's inferred file ownership. These are reconstruction
fragments, not recovered historical source-file divisions.

## Storage and interface evidence

[Camera.h](../include/game/Camera.h) extends the existing
[scene-node declaration](SceneNode.md) with a scoped `CCamera` view.
The copy constructor at `0x8012ba60` installs the original table at
`0x802e8410`. Its slots 1–88 agree with `IMovingSceneNode`, including camera
overrides at slots 19 and 68–81, and inherited `AsMovingNode` at 34–35.
The table has no additional camera virtual slots before its next zero entry.
All examined `this` adjustments are zero. The inherited observer initialization
agrees with the existing 24-byte prefix.

| Offset | Descriptive field | Independent evidence |
| --- | --- | --- |
| `0x18`–`0x37` | Unknown bytes | No additional scene-node ownership or types asserted |
| `0x38`, `0x3c` | `nearClip`, `farClip` | Passed to the verified matrix projection routines; copied by the camera copy constructor |
| `0x40`, `0x44` | `projectionX`, `projectionY` | Both projection setters, FOV getters and projection-matrix update |
| `0x48`–`0x4f` | Unknown bytes | Preserved without a type claim |
| `0x50` | `localToWorld` | Named getter/setter, basis setters and 64-byte matrix assignment |
| `0x90` | `worldToCamera` | Original `UpdateWorldToCamera` writes its basis/position, and the copy constructor copies 64 bytes |
| `0xd0` | `cameraToClip` | Destination of both matrix projection calls and a 64-byte copy |
| `0x110` | `worldToClip` | Original `UpdateWorldToClip` writes all sixteen matrix entries |
| `0x150` | `perspective` | Set to 1/0 by perspective/orthographic setup and tested by projection update |
| `0x154`, `0x158`, `0x15c` | `worldToCameraValid`, `cameraToClipValid`, `worldToClipValid` | `AttemptUpdate` tests each before calling its corresponding update routine; those routines set their own flag |
| `0x160` | `field_160` | Both projection setters write 1; broader meaning remains unproven |

Field names, access control and the integer spelling of flags are reconstruction
choices, not recovered declarations. The flags occupy four bytes; these routines
do not establish their original signedness. The header ends at `0x164` and must
not be used to infer allocation size or construct camera objects. The original
constructor accesses later storage, including `0x2b0`, which remains outside
this view. Original tables, constructors and unrelated virtual overrides remain
binary context. `#pragma interface` avoids emitting a replacement camera table.

Method names, parameter types and constness come from original symbols. FOV
getters return through the floating-point result register; mutation methods
retain the existing void interface. The transform wrappers pass vectors by reference. The four vector getters
construct row copies and use the recovered vector assignment that returns a
value; they preserve the destination fourth word. Each getter has its own
generated 1.0 constant, together occupying `0x802a768c`–`0x802a769c`. The vector type is described in [Matrix.md](Matrix.md#cvector3-layout).

## Preserved behavior

`SetPerspective` stores the tangent of each supplied half-angle, in radians.
`GetHFOV` and `GetVFOV` return twice the corresponding arctangent. This supports
the contributed interpretation of the player's 35-degree setting as a 70-degree
horizontal field of view. `SetOrthographic` stores its two inputs directly.
Both setters invalidate camera-to-clip and world-to-clip, leave world-to-camera
validity untouched, set their projection mode, and write 1 to `field_160`.
The getters do not branch on projection mode.

`UpdateCameraToClip` calls the already reconstructed `CMatrix::Perspective` or
`Orthographic`. The perspective branch multiplies the stored far-clip value by
**100.0f**, whereas the orthographic branch passes it unchanged. The complete
generated constant at `0x802a76bc` is verified. The update sets only the
camera-to-clip validity flag; no new invalidation or normalization is introduced.

Camera transform mutations invalidate world-to-clip and world-to-camera.
`Reset` calls `GetSlot(0)` to restore the identity matrix; `SetBasis` calls
`SetRight`, `SetFront`, then `SetUp`. `Move` calls `Translate`. Pre- and
post-transform copy the current matrix into the original shared
`CMatrix::s_TempMat` before multiplying in the observed operand order. Its
original symbol identifies 64 bytes at `0x802fbf00`; it remains external BSS
with no reconstructed-data credit. A new local temporary would change this
shared-storage behavior and is not substituted.

The matrix operations already have accepted source except for `Rotate`, which remains an external original body.
`Orthonormalize` now has separately verified matrix source. Their camera wrappers
earn credit only for their own verified bytes. The matrix header now declares
the original `Rotate(const CVector3 &, float)` method and shared temporary;
neither declaration earns source credit.

## Verification and next work

Verification compares all generated allocated code and data, every function's
boundaries and external dependencies, then the complete rebuilt analysis image.
No generated functions are stripped, no instruction patches or assembly bodies
are used, and original context earns no new credit. The separate original-object
baseline also matches; runtime and emulator behavior remain untested.

All 55 local tests pass. The earlier camera batch preserved all 753 then-existing unit records. The
latest geometry batch additionally checks preservation of all 803 prior records
when extending the camera and matrix declarations.
The complete 2,860,576-byte image retains SHA-1
`6abed07aefb9be8cb2cd3c4e0fa53a1fde8db04d`. The public snapshot was refreshed after
local verification; CI validates that snapshot rather than rebuilding the game.

`UpdateWorldToClip`, update dispatch and the remaining pitch/roll/yaw routines are
useful follow-ups. Their local vectors can now use the
[`CVector3` declaration](Matrix.md#cvector3-layout); each body still needs its own
comparison. Frustum members and
`field_160` also need further analysis; the current storage view deliberately
leaves them unresolved.

## World-to-camera update

`UpdateWorldToCamera` copies the local-to-world right, front, up and position
vectors before writing the destination. It transposes the saved basis with an
up/front axis swap: the three output rows are `(right.x, up.x, front.x)`,
`(right.y, up.y, front.y)` and `(right.z, up.z, front.z)`. Translation is the negative
dot product of position with right, up and front, in that order. The function calls
the already reconstructed matrix setters and sets only `worldToCameraValid` to one.
It does not normalize the basis or perform a general matrix inversion.

The matrix setters preserve each fourth component; the reconstruction does not
reset those destination words. The saved vectors, component-pointer view of right,
and default-constructed translation retain the original temporary lifetimes and
floating-point operation order. `DotComponents` is a descriptive inline helper,
not a recovered original name. No additional camera or matrix storage was inferred.
The generated four-byte `1.0f` at `0x802a76b8` is fully compared and earns no code
credit. This fragment was independently reconstructed with Codex assistance.

The same batch adds the script far-clip setter described in [Script.md](Script.md).
That wrapper reuses the existing camera prefix and invalidation flags; neither
the external global camera storage nor the remaining update routines earn credit.
