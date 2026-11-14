// The two volume sliders. Names come from the symbol table; which knob is which follows
// SoundGetVolumes's parameter names, themselves inferred.
#ifndef GAME_VOLUME_KNOBS_H
#define GAME_VOLUME_KNOBS_H
class FEObject;
enum EVolumeKnob { VolumeKnobMusic, VolumeKnobSfx };
// Two vectors the handlers' inline constructors zero; their meaning is not known.
struct CSliderVector {
    float x, y, z;
    CSliderVector() { x = y = z = 0.0f; }
};
class CSlider {
  public:
    void Initialize(FEObject *);
    void SetValue(float);
    union { unsigned char unknown00[4]; FEObject *object; };
    CSliderVector vectorA; // +0x04
    CSliderVector vectorB; // +0x10
    union { unsigned char unknown1c[4]; float value; };
};
class CVolumeKnobs {
  public:
    void Setup(EVolumeKnob, FEObject *);
    void Increase(EVolumeKnob);
    void Decrease(EVolumeKnob);
    CSlider musicSlider;
    CSlider sfxSlider; // +0x20
};
#endif
