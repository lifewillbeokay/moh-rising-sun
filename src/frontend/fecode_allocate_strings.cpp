// AI-assisted reconstruction from GR8E69; see docs/research/feng-animation-lists.md.
#ifdef DECOMP_COMPARE
#include "FEngElements.h"
#include <string.h>

void FECodeListBox::AllocateStrings(unsigned long count, unsigned long length) {
    short *oldBlock = stringBlock;
    short **oldStrings = strings;
    unknown_8c = stringsInUse = unknown_90 = 0;
    stringBlock = 0;
    strings = 0;
    if (!count || !length) {
        // This empty walk is present in the target's zero-capacity path.
        for (unsigned long row = 0; row < numRows; ++row)
            for (unsigned long column = 0; column < numColumns; ++column) {}
    } else {
        stringBlock = new ("source/FEngine/FECodeListBox.cpp:500") short[count * length];
        strings = new ("source/FEngine/FECodeListBox.cpp:504") short *[count];
        memset(stringBlock, 0, count * length * sizeof(short));
        for (unsigned long i = 0; i < count; ++i) strings[i] = stringBlock + i * length;
        unknown_8c = count;
        unknown_90 = length;
        if (oldBlock && oldStrings) {
            for (unsigned long row = 0; row < numRows; ++row) {
                for (unsigned long column = 0; column < numColumns; ++column) {
                    FEListBoxCell &cell = cells[row * numColumns + column];
                    if (cell.type == 2) {
                        const short *source = cell.text;
                        cell.text = AllocateString();
                        CopyString<short>(cell.text, source, length);
                    }
                }
            }
        }
    }
    if (oldBlock) delete[] oldBlock;
    if (oldStrings) delete[] oldStrings;
}

#endif
