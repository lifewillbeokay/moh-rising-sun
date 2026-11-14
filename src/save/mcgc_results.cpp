// AI-assisted GR8E69 reconstruction; see docs/research/save-flow.md.
#include "MemoryCardGC.h"
int CMemoryCardGC::SaveSuccessful() const { return saveSuccess; }
int CMemoryCardGC::LoadSuccessful() const { return loadSuccess; }
int CMemoryCardGC::DefaultReply() const { return defaultReply; }
int CMemoryCardGC::DeclinedToSave() const { return 0; }
