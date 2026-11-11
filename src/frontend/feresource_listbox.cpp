// AI-assisted GR8E69 reconstruction; see docs/research/feng-packages.md.
#ifdef DECOMP_COMPARE
#include "FEListBox.h"
void ResourceConnector::ConnectListBoxResources(FEListBox *list) {
    list->currentColumn = list->currentRow = 0;
    unsigned long rows = list->numRows, columns = list->numColumns;
    for (unsigned long row = 0; row < rows; ++row) {
        for (unsigned long column = 0; column < columns; ++column) {
            long index = list->cells[list->currentRow * list->numColumns + list->currentColumn].unknown_14;
            if (index == -1) {
                FEListBoxCell *cell = list->GetPCellData(list->currentColumn, list->currentRow);
                cell->unknown_c = 0; cell->unknown_14 = -1; cell->unknown_10 = 0;
            } else {
                FEResourceRequest &resource = (*requests)[index];
                void *data = resource.data, *handle = resource.handle;
                FEListBoxCell *cell = list->GetPCellData(list->currentColumn, list->currentRow);
                cell->unknown_c = (long)handle; cell->unknown_14 = index; cell->unknown_10 = (long)data;
            }
            list->IncrementCellByColumn();
        }
    }
}
#endif
