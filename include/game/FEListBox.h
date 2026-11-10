// FEng's list box object; see docs/Frontend.md. Class tags come from symbols, member names describe the code.
#ifndef GAME_FE_LIST_BOX_H
#define GAME_FE_LIST_BOX_H
#pragma interface
#include "FEngStrings.h"
#include "FEngObjects.h"

struct FEPoint {
    float x, y;
    FEPoint(float px, float py) { x = px; y = py; }
};

// A column's or row's size and the cumulative position RecalculateCummulative gives it.
struct FEListEntryData {
    float size;       // +0x0
    float cumulative; // +0x4
    unsigned long unknown_8;
};

// One cell; 48 bytes per the allocations and InitializeCell.
struct FEListBoxCell {
    // The array constructors in both list-box classes initialize this subset.
    FEListBoxCell() : resourceIndex(0), unknown_4(1.0f, 1.0f), unknown_c(0),
        unknown_10(0), unknown_14(0), type(0), text(0), unknown_24_bits(-1) {}
    long resourceIndex;  // +0x00
    FEPoint unknown_4;   // +0x04
    long unknown_c;      // +0x0c
    long unknown_10;     // +0x10
    long unknown_14;     // +0x14
    long type;           // +0x18; 2 for a string.
    long unknown_1c;     // +0x1c
    union {
        short *text;     // +0x20; SetCellString allocates it.
        float unknown_20;
    };
    union {
        float unknown_24; // +0x24
        long unknown_24_bits; // Constructor's all-ones sentinel; meaning unknown.
    };
    float unknown_28;    // +0x28
    float unknown_2c;    // +0x2c
};

class FEListBox : public FEObject {
  public:
    unsigned long listFlags;     // +0x58; 1 initialized, 2 scrolling, 4 no wrap.
    unsigned long numColumns;  // +0x5c
    unsigned long numRows;     // +0x60
    float unknown_64;          // +0x64
    float unknown_68;          // +0x68
    float scrollX;             // +0x6c
    float scrollY;             // +0x70
    FEListEntryData *columns;  // +0x74
    FEListEntryData *rows;     // +0x78
    float unknown_7c;          // +0x7c
    float unknown_80;          // +0x80
    unsigned long currentColumn; // +0x84
    unsigned long currentRow;  // +0x88
    FEListBoxCell *cells;      // +0x8c
    float targetX;             // +0x90
    float targetY;             // +0x94
    float unknown_98;          // +0x98
    float unknown_9c;          // +0x9c
    float blink;               // +0xa0
    float blinkRate;           // +0xa4
    FEListBox();
    virtual ~FEListBox();
    void Terminate();
    void SetNumColumns(unsigned long);
    void SetNumRows(unsigned long);
    void SetCellType(unsigned long);
    void SetCellString(const short *);
    void IncrementCellByColumn();
    void SetAutoWrap(bool);
    static void InitializeListEntry(FEListEntryData *, unsigned long);
    static void InitializeCell(FEListBoxCell *, unsigned long);
    void CleanupColumns();
    void CleanupRows();
    void CleanupCells();
    void RecalculateCummulative();
    void CompleteScroll();
    void ScrollSelection(long, long);
    void Update(float);
    FEListBoxCell *GetPCellData(unsigned long, unsigned long);
};
#endif
