// AI-assisted scoped reconstruction from GR8E69; see docs/CameraShake.md.
#ifndef GAME_CAMERA_SHAKE_H
#define GAME_CAMERA_SHAKE_H
#pragma interface

// Scoped view of CPlayerObject: the nested shake controller and the members its
// camera-shake wrappers use.
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

    // Member-only view: the shake controllers and motion-shake values the wrappers
    // use. The prefix, base classes and table pointer are not declared.
    struct Flags {
        unsigned int unknown_31_26 : 6;
        unsigned int motionShake : 1;   // Bit 25: set by DoMotionShake for a positive amount.
        unsigned int unknown_low : 25;
    };
    unsigned char unknown_0[0xd4c];
    Flags flags;
    unsigned char unknown_d50[0xd58 - 0xd50];
    CCameraShake cameraShake;
    CCameraShake backgroundShake;
    float motionShakeAmount, motionShakeRate, motionShakeTime;

    void SetCameraShake(float, float, float, float);
    void StartCameraShake(float, float);
    void StopCameraShake(float);
    void StartBackgroundCameraShake(float, float);
    void StopBackgroundCameraShake(float);
    void DoMotionShake(float, float);
};

#endif
