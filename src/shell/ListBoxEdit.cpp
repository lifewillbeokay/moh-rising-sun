// A fragment of the original dListBox.cpp: rows on a shell list. Names come from the symbol table; member
// names are inferred from this code.
#include "profile.h"
#include <vector>
#include <string.h>

class FEngine;
extern char s_xsNull[];
extern const char s_DefaultPrefix[]; // The file's private default.
enum EShellSoundType { ShellSoundScroll = 10 };
class ShellManager {
  public:
    void PlayShellSound(EShellSoundType);
};
extern ShellManager g_Shell;

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

int ListBox::AddRow(Row *row) {
    for (Row **it = rows.begin(); it != rows.end(); it++) {
        if (strcmp(row->Field(0), (*it)->Field(0)) == 0) {
            delete row;
            return 0;
        }
    }
    rows.push_back(row);
    order.push_back(rows.size() - 1);
    return 1;
}

void ListBox::RemoveRow(Row *row) {
    if (row) {
        int rowIndex = 0;
        while (rowIndex < (int)rows.size() && rows[rowIndex] != row)
            rowIndex++;
        int orderIndex = 0;
        while (orderIndex < (int)order.size() && order[orderIndex] != rowIndex)
            orderIndex++;
        rows.erase(&rows[rowIndex]);
        int *orderEntry = order.begin();
        orderEntry += orderIndex;
        order.erase(orderEntry);
        for (int i = 0; i < (int)order.size(); i++) {
            if (order[i] > rowIndex)
                order[i]--;
        }
        if (selected + top >= (int)rows.size())
            ScrollUp();
    }
}
