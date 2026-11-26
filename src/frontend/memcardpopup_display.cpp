// AI-assisted GR8E69 reconstruction; see docs/research/save-popups.md.
#include "SaveGamePopups.h"
#ifdef DECOMP_COMPARE
void CMemoryCardPopup::Display(const unsigned short *text, const unsigned short *a, const unsigned short *b, const unsigned short *c, const unsigned short *d) {
    displayed = 1; SetString(message, text, true);
    selectable[3] = selectable[0] = selectable[1] = selectable[2] = 0;
    if (labels[0]) SetString(labels[0], a, false);
    if (labels[1]) SetString(labels[1], b, false);
    if (labels[2]) SetString(labels[2], c, false);
    if (labels[3]) SetString(labels[3], d, false);
    selectable[0] = a != 0; selectable[1] = b != 0; selectable[2] = c != 0; selectable[3] = d != 0;
    if (!waitingForReply) HideSelections();
}
#endif
