// FELocalizer: string tables keyed by label hash, one column per language.
// Class and method names are from the symbol table. Member names are inferred from their use.
#ifndef GAME_FELOCALIZER_H
#define GAME_FELOCALIZER_H

#include "FEngLists.h"
#include "FEngStrings.h"

class FELocalString {
  public:
    FELocalString() {
        label = 0;
        platform = 0;
        comment = 0;
        flags = 0;
        size = 0;
        hash = 0xffffffff;
        strings = 0;
    }
    ~FELocalString() {
        if (label) delete[] label;
        if (platform) delete[] platform;
        if (comment) delete[] comment;
        if (flags) delete[] flags;
        if (size) delete[] size;
        delete[] strings;
    }
    void SetLabelName(const char *);
    void SetPlatform(const char *);
    void SetComment(const char *);
    void SetFlags(const char *);
    void SetSize(const char *);

    char *label;           // +0x0
    char *platform;        // +0x4
    char *comment;         // +0x8
    char *flags;           // +0xc
    char *size;            // +0x10; SetSize ignores its argument.
    unsigned long hash;    // +0x14; FEHashUpper of the label.
    FEWideString *strings; // +0x18; one per language.
};

class FELocalizer {
  public:
    FELocalizer();
    ~FELocalizer();
    void ReleaseAll();
    void SetNumLanguages(unsigned long);
    void SetLanguageName(const char *, unsigned long);
    void AllocStrings(unsigned long);
    FEWideString *GetString(const char *, unsigned long) const;
    FEWideString *GetString(unsigned long, unsigned long) const;
    int GetLabelIndex(const char *) const;
    int GetLabelIndex(unsigned long) const;
    void SetString(const char *, unsigned long, FEWideString &);
    int SetString(unsigned long, unsigned long, FEWideString &);
    int ReadLocaleData(void *, unsigned long);
    void WideToEscapeString(char *, const short *);
    int ReadString(short *, char *, unsigned long &, unsigned long);
    int ReadNextString(short *, char *, unsigned long &, unsigned long);

    unsigned long numLanguages; // +0x0
    FEList languages;           // +0x4; one named node per language.
    int storeLabels;            // +0x14; SetString keeps label text when set.
    unsigned long numStrings;   // +0x18
    unsigned long capacity;     // +0x1c
    int unknown_20;             // +0x20; cleared whenever the table changes.
    FELocalString *entries;     // +0x24
};

#endif
