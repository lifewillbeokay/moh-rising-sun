// AI-assisted GR8E69 reconstruction; see docs/research/save-popups.md.
#include "SaveGamePopups.h"
extern "C" { unsigned int USTR_length(const unsigned short *); void USTR_copy(unsigned short *, const unsigned short *); }
#ifdef DECOMP_COMPARE
void CSaveGamePopup::SetString(FEString *string, const unsigned short *text, bool wrap) {
    unsigned short buffer[1024];
    buffer[0] = 0;
    unsigned int length = 0;
    if (text) { length = USTR_length(text); USTR_copy(buffer, text); }
    if (wrap) {
        unsigned int lastBreak = 0, lastSpace = 0;
        for (unsigned int i = 0; i < length; ++i) {
            if (i - lastBreak > 37 && lastSpace) {
                buffer[lastSpace] = '\n'; lastBreak = lastSpace;
            }
            if (buffer[i] == ' ') lastSpace = i;
        }
    }
    // Nonbreaking spaces become ordinary spaces only after wrapping.
    for (unsigned int i = 0; i < length; ++i)
        if (buffer[i] == 0xa0) buffer[i] = ' ';
    string->text = (const short *)buffer;
    string->flags |= 0x400000;
}
#endif
