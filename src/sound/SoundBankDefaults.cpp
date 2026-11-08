// AI-assisted reconstruction from GR8E69; see docs/Sound.md.
#include "SoundManager.h"
SoundEntryBank::~SoundEntryBank() {}
int SoundEntryBank::GetTypeID() const { return 0; }
void SoundEntryBank::UpdatePlayStatus() {}
int SoundEntryBank::CheckFade() { return 0; }
