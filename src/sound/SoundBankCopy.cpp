// AI-assisted reconstruction from GR8E69; see docs/Sound.md.
#include "SoundManager.h"
#include "PlacementNew.h"
SoundEntryBank::SoundEntryBank(const SoundEntryBank &o) : SoundEntry(o), field_d8(o.field_d8), field_dc(o.field_dc) {}
void SoundEntryBank::Clone(SoundEntry *out) const {
    if (out) new(out) SoundEntryBank(*this);
}
