// FEng wide strings; see docs/Frontend.md. Class tags and methods are from symbols, member names descriptive.
#ifndef GAME_FENG_STRINGS_H
#define GAME_FENG_STRINGS_H

extern "C" void *DWI_alloc(const char *, int, int);
inline void *operator new[](unsigned int size, const char *label) { return DWI_alloc(label, size, 256); }

unsigned long GetStringLength(const short *);
template <class T> void CopyString(short *, const T *);
template <class T> void CopyString(short *, const T *, unsigned long);

class FEWideString {
  public:
    FEWideString();
    FEWideString(const short *);
    FEWideString(const FEWideString &);
    ~FEWideString();
    FEWideString &operator=(const FEWideString &);
    FEWideString &operator=(const short *);
    FEWideString &operator=(const char *);
    unsigned long Length() const;
    void SetLength(unsigned long);
    char *Convert(char *) const;
    short *AllocateString(unsigned long);
    void ConvertFromSpecial(char *);

    short *text;           // +0x0
    unsigned long capacity; // +0x4
};

#endif
