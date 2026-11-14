// AI-assisted reconstruction from GR8E69; see docs/research/frontend-popup-list.md.
#include "PackageHandlers.h"
#include "ShellListBox.h"
#include <string.h>
void HandlerOnlineFavorites::RemoveDuplicates() {
    for (int i = 0; i < listBox->GetNumRows(); ++i) {
        ListBox::Row *row = listBox->GetRowByIndex(i);
        if (!row) continue;
        for (int j = i + 1; j < listBox->GetNumRows(); ++j) {
            ListBox::Row *other = listBox->GetRowByIndex(j);
            if (other && !strcmp(row->Field(0), other->Field(0))) {
                listBox->RemoveRow(other);
                delete other;
                --j;
            }
        }
    }
}
