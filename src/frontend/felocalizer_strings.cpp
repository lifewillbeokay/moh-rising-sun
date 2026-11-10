// FELocalizer: string tables keyed by label hash; see docs/Frontend.md.
#include "FELocalizer.h"
#include "FEHash.h"
#include <string.h>
inline void *operator new(unsigned int size, const char *label) { return DWI_alloc(label, size, 256); }
// A private helper of FELocalizer.cpp, defined earlier in the original file.
void FECopyString(char *&, const char *);

void FELocalString::SetLabelName(const char *name) {
    FECopyString(label, name);
    hash = FEHashUpper(label);
}
void FELocalString::SetPlatform(const char *text) { FECopyString(platform, text); }
void FELocalString::SetComment(const char *text) { FECopyString(comment, text); }
void FELocalString::SetFlags(const char *text) { FECopyString(flags, text); }
void FELocalString::SetSize(const char *) {}

FELocalizer::FELocalizer() : numLanguages(0), storeLabels(1) {
    numStrings = 0;
    capacity = 0;
    unknown_20 = 0;
    entries = 0;
}
FELocalizer::~FELocalizer() { ReleaseAll(); }
void FELocalizer::ReleaseAll() {
    delete[] entries;
    storeLabels = 1;
    entries = 0;
    numLanguages = 0;
    capacity = 0;
    numStrings = 0;
    unknown_20 = 0;
}
void FELocalizer::SetNumLanguages(unsigned long count) {
    numLanguages = count;
    for (unsigned long i = 0; i < numLanguages; i++) languages.AddTail(new ("source/FEngine/FELocalizer.cpp:134") FENode);
}
void FELocalizer::SetLanguageName(const char *name, unsigned long index) {
    ((FENode *)languages.FEMinList::FindNode(index))->SetName(name);
}
void FELocalizer::AllocStrings(unsigned long count) {
    if (count == 0) {
        delete[] entries;
        entries = 0;
        numStrings = 0;
        capacity = 0;
    } else {
        FELocalString *grown = new ("source/FEngine/FELocalizer.cpp:180") FELocalString[count];
        unsigned long keep = count;
        if (keep > numStrings) keep = numStrings;
        void *dest = grown;
        memcpy(dest, entries, keep * sizeof(FELocalString));
        memset(entries, 0, keep * sizeof(FELocalString));
        delete[] entries;
        entries = grown;
        capacity = count;
        numStrings = keep;
    }
    unknown_20 = 0;
}
FEWideString *FELocalizer::GetString(const char *label, unsigned long language) const {
    return GetString(FEHashUpper(label), language);
}
FEWideString *FELocalizer::GetString(unsigned long hash, unsigned long language) const {
    int index = GetLabelIndex(hash);
    if (index == -1) return 0;
    return &entries[index].strings[language];
}
int FELocalizer::GetLabelIndex(const char *label) const { return GetLabelIndex(FEHashUpper(label)); }
int FELocalizer::GetLabelIndex(unsigned long hash) const {
    for (unsigned long i = 0; i < numStrings; i++) {
        if (entries[i].hash == hash) return i;
    }
    return -1;
}
void FELocalizer::SetString(const char *label, unsigned long language, FEWideString &text) {
    unsigned long hash = FEHashUpper(label);
    if (!SetString(hash, language, text)) {
        if (numStrings == capacity) AllocStrings(numStrings + 64);
        entries[numStrings].strings = new ("source/FEngine/FELocalizer.cpp:274") FEWideString[numLanguages];
        entries[numStrings].strings[language] = text;
        if (storeLabels) entries[numStrings].SetLabelName(label);
        entries[numStrings].hash = hash;
        unknown_20 = 0;
        numStrings++;
    }
}
int FELocalizer::SetString(unsigned long hash, unsigned long language, FEWideString &text) {
    int index = GetLabelIndex(hash);
    if (index == -1) return 0;
    entries[index].strings[language] = text;
    return 1;
}
