// A fragment of the original VolumeKnobs.cpp: the two volume sliders. Names come from the symbol table;
// which knob is which follows SoundGetVolumes's parameter names, themselves inferred.
void SoundGetVolumes(float &, float &);
void SoundSetVolumes(float, float);

#include "VolumeKnobs.h"
enum EShellSoundType { ShellSoundUnknown6 = 6, ShellSoundUnknown7 = 7 };

class ShellManager {
  public:
    void PlayShellSound(EShellSoundType);
};
extern ShellManager g_Shell;

void CVolumeKnobs::Setup(EVolumeKnob knob, FEObject *object) {
    float sfx, music;
    SoundGetVolumes(sfx, music);
    if (knob == VolumeKnobMusic) {
        musicSlider.Initialize(object);
        musicSlider.SetValue(music);
    } else {
        sfxSlider.Initialize(object);
        sfxSlider.SetValue(sfx);
    }
}

void CVolumeKnobs::Increase(EVolumeKnob knob) {
    float sfx, music;
    SoundGetVolumes(sfx, music);
    float *volume = &sfx;
    if (knob == VolumeKnobMusic)
        volume = &music;
    *volume += 0.1f;
    if (!(*volume <= 1.0f))
        *volume = 1.0f;
    else
        g_Shell.PlayShellSound(ShellSoundUnknown6);
    SoundSetVolumes(sfx, music);
    if (knob == VolumeKnobMusic)
        musicSlider.SetValue(*volume);
    else
        sfxSlider.SetValue(*volume);
}

void CVolumeKnobs::Decrease(EVolumeKnob knob) {
    float sfx, music;
    SoundGetVolumes(sfx, music);
    float *volume = &sfx;
    if (knob == VolumeKnobMusic)
        volume = &music;
    *volume -= 0.1f;
    if (!(*volume >= 0.0f))
        *volume = 0.0f;
    else
        g_Shell.PlayShellSound(ShellSoundUnknown7);
    SoundSetVolumes(sfx, music);
    if (knob == VolumeKnobMusic)
        musicSlider.SetValue(*volume);
    else
        sfxSlider.SetValue(*volume);
}
