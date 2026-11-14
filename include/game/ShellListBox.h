// Scoped dListBox.cpp storage reused from the accepted ListBox fragments.
#ifndef GAME_SHELL_LIST_BOX_H
#define GAME_SHELL_LIST_BOX_H
#include "profile.h"
#include <vector>
class FEngine;
extern char s_xsNull[];
class ListBox {
  public:
    class Row {
      public:
        ~Row();
        const char *Field(int index) const { return index < fieldCount ? fields[index] : s_xsNull; }
        char **fields;    // +0x00
        int fieldCount;   // +0x04
        unsigned char unknown08[4];
        long id;          // +0x0c
    };
    ListBox(FEngine *);
    ~ListBox();
    void Clear();
    int AddRow(Row *);
    void RemoveRow(Row *);
    void ScrollUp();
    void ScrollDown();
    Row *GetSelectedRow();
    void SetSelectedRowById(long);
    Row *GetRowByIndex(long);
    int GetRowIndexById(long);
    int GetNumRows();
    int GetNumScreenRows();
    int GetRowFieldWidth(unsigned char);
    int CountRows();
    Row *RowAt(long index) {
        if (index < 0 || index >= (int)order.size())
            return 0;
        int row = order[index];
        if (row < 0 || row >= (int)rows.size())
            return 0;
        return rows[row];
    }
    char prefix[16];         // +0x00
    int screenRows;          // +0x10; counted on demand when not positive.
    int top;                 // +0x14
    int selected;            // +0x18; relative to top.
    int fieldWidths[32];     // +0x1c; negative until counted.
    FEngine *engine;         // +0x9c
    unsigned char unknowna0[4];
    _STL::vector<Row *> rows;  // +0xa4
    _STL::vector<int> order;   // +0xb4; indices into rows, in display order.
};

#endif
