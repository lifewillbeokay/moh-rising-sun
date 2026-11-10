// FEWideString: FEng's wide-character string; characters are 16 bits, with the top bit
// marking an escape code. See docs/Frontend.md.
#include "FEngStrings.h"
#include <string.h>
struct FEEscapeSequence { const char *name; short code; };
extern FEEscapeSequence SpecialColorCharacters[];
void GenerateEscapeSequence(char *, const char *, unsigned long); // File-local in the original.

FEWideString::FEWideString() : text(0), capacity(0) { capacity = Length(); }
FEWideString::FEWideString(const short *s) : text(0), capacity(0) {
    *this = s;
    capacity = Length();
}
FEWideString::FEWideString(const FEWideString &s) : text(0), capacity(0) {
    *this = s;
    capacity = Length();
}
FEWideString::~FEWideString() {
    if (text) delete[] text;
}
FEWideString &FEWideString::operator=(const FEWideString &s) {
    if (s.text) {
        text = AllocateString(GetStringLength(s.text) + 1);
        CopyString(text, (const short *)s.text);
    }
    return *this;
}
FEWideString &FEWideString::operator=(const short *s) {
    if (!s) return *this;
    text = AllocateString(GetStringLength(s) + 1);
    CopyString(text, s);
    return *this;
}
FEWideString &FEWideString::operator=(const char *s) {
    if (!s) return *this;
    unsigned long length = strlen(s);
    text = AllocateString(length + 1);
    for (unsigned long i = 0; i < length; i++) text[i] = (unsigned char)s[i];
    text[length] = 0;
    return *this;
}
unsigned long FEWideString::Length() const { return GetStringLength(text); }
void FEWideString::SetLength(unsigned long length) {
    if (length > Length()) {
        capacity = length;
        short *buffer = new ("source/FEngine/FEWideString.cpp:320") short[length + 1];
        CopyString(buffer, (const short *)text);
        if (text) delete[] text;
        text = buffer;
    }
}
char *FEWideString::Convert(char *out) const {
    if (!out) return 0;
    *out = 0;
    if (!text) return out;
    unsigned long i = 0;
    while (text[i]) {
        out[i] = text[i];
        i++;
    }
    out[i] = 0;
    return out;
}
short *FEWideString::AllocateString(unsigned long length) {
    if (length > capacity) SetLength(length);
    return text;
}
void FEWideString::ConvertFromSpecial(char *out) {
    if (!text) {
        *out = 0;
        return;
    }
    unsigned long length = 0;
    for (unsigned long i = 0; text[i]; i++) {
        int c = (unsigned short)text[i];
        if (c & 0x8000) {
            FEEscapeSequence *entry = SpecialColorCharacters;
            if (entry->name) {
                short code = c & ~0xfe;
                do {
                    if (entry->code == code) {
                        char *p = out + length;
                        GenerateEscapeSequence(p, entry->name, c & 0xff);
                        length += strlen(p);
                        break;
                    }
                    entry++;
                } while (entry->name);
            }
            if (!entry->name) out[length++] = text[i];
        } else out[length++] = text[i];
    }
    out[length] = 0;
}
