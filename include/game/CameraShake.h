// AI-assisted scoped reconstruction from GR8E69; see docs/CameraShake.md and docs/Paths.md.
#ifndef GAME_CAMERA_SHAKE_H
#define GAME_CAMERA_SHAKE_H
#pragma interface
class CAISplinePath;

// Scoped member view of CPlayerObject for shake and path controls. This does not
// establish the complete allocation, base classes or virtual interface.
class CPlayerObject {
  public:
    // 36 bytes: eight floats, then the table pointer at +32 (the class declares its
    // data before its virtual destructor). Field names are descriptive.
    class CCameraShake {
      public:
        float current;     // Intensity now.
        float target;      // Twice the requested intensity.
        float scale;       // 1 / intensity at the last SetShake (1 when none).
        float field_0c;
        float fadeTime;    // Ramp time used when the duration expires.
        float duration;    // 100000 means no automatic stop.
        float elapsed;
        float rate;        // Intensity change per update step; 0 when settled.
        CCameraShake();
        virtual ~CCameraShake();
        void SetShake(float, float, float, float);
        void SetShake(float, float);
        void Evaluate(float &, float &);
        void Update(float);
    };

    // Member-only view; unknown bytes retain their original ownership.
    struct Flags {
        unsigned int unknown_31_26 : 6;
        unsigned int motionShake : 1;   // Bit 25: set by DoMotionShake for a positive amount.
        unsigned int unknown_low : 25;
    };
    unsigned char unknown_0[0xd44];
    unsigned int pathFlags; // +0xd44; bit 19 enables path movement.
    unsigned char unknown_d48[4];
    Flags flags;
    unsigned char unknown_d50[0xd58 - 0xd50];
    CCameraShake cameraShake;
    CCameraShake backgroundShake;
    float motionShakeAmount, motionShakeRate, motionShakeTime;

    unsigned char unknown_dac[0xe00 - 0xdac];
    CAISplinePath *movePath, *lookPath; // +0xe00, +0xe04
    float pathParameter, pathRate; // +0xe08, +0xe0c
    void MoveOnPath(CAISplinePath *, CAISplinePath *, float);
    void StopPath();

    void SetCameraShake(float, float, float, float);
    void StartCameraShake(float, float);
    void StopCameraShake(float);
    void StartBackgroundCameraShake(float, float);
    void StopBackgroundCameraShake(float);
    void DoMotionShake(float, float);
};

#endif
