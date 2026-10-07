# Camera shake

`src/player/` reconstructs nine functions (564 executable bytes) of the player's
camera shake from the pinned GR8E69 executable. Claude Code assisted the analysis
and verification.

## Declarations

`include/game/CameraShake.h` declares `CPlayerObject::CCameraShake` (36 bytes: eight
floats, then its table pointer at `+32`, because the class declares data before its
virtual destructor; the table is `_vt.Q213CPlayerObject12CCameraShake` at
`0x802e5030`) and a scoped `CPlayerObject` view: a flag word at `+0xd4c`, the main
shake at `+0xd58`, the background shake at `+0xd7c` and three motion-shake floats at
`+0xda0`. Field names are descriptive.

| Manifest | Functions | Code bytes | Generated data |
| --- | --- | ---: | --- |
| `camera_shake_init` | `CCameraShake` constructor | 56 | 4 bytes at `0x8029f478` |
| `camera_shake_evaluate` | `SetShake(intensity, rampTime)`, `Evaluate` | 240 | 16 bytes at `0x8029f4d4` |
| `player_camera_shake` | `SetCameraShake`, `StartCameraShake`, `StopCameraShake`, `StartBackgroundCameraShake`, `StopBackgroundCameraShake`, `DoMotionShake` | 268 | 12 bytes at `0x8029f1f8` |

## Behavior

- **`SetShake(intensity, rampTime, fadeTime, duration)`** (not accepted, see below):
  a duration outside 0-100000 prints "Specified duration must be between 0 and %f"
  into a local buffer and becomes 0. The target is **twice** the intensity. The
  duration is raised to at least the ramp time; the ramp time and fade time to at
  least 1 (update ticks). The elapsed time restarts, the scale becomes
  `1 / current` (or 1 when the current intensity is not positive), and the rate is
  `(target - current) / rampTime`.
- **`SetShake(intensity, rampTime)`** uses a fade time of 0 (raised to 1) and the
  duration 100000, which `Update` treats as "no automatic stop".
- **`Evaluate(x, y)`** adds a random value in `±0.25 * current` to `y` and then to
  `x` (`MathFunRandomReal`); while fading (`rate < 0`) both are multiplied by
  `scale * current`.
- **`Update(step)`** (not accepted): while the rate is non-zero it adds
  `rate * step` and settles on the target when it passes it. Settled, and unless the
  duration is 100000, it counts elapsed time and, once past the duration, starts
  `SetShake(0, fadeTime)`.
- The player wrappers forward to the main or background controller; the `Stop`
  forms ramp to 0 over the given time. `DoMotionShake(amount, rate)` stores both,
  clears a timer, and sets bit 25 of the flag word for a positive amount.

`SetShake`'s four-argument form and `Update` compile to the original instructions
except for register and block-order details (`Update`) and constant-pool alignment:
the four-argument form uses an eight-byte conversion constant, and the original pool
continues into `Update`'s constants, so it can be accepted only together with
`Update`.

## Verification

ProDG 3.8.1 with `-O2 -G0 -fno-exceptions -fno-implicit-templates` reproduces every
accepted fragment and its constants. The complete rebuilt analysis image is
identical. Runtime behavior has not been tested.
