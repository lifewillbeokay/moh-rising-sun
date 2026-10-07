// AI-assisted reconstruction from GR8E69; see docs/FlexProp.md.
#include "StringTable.h"
#include "Endian.h"

StringTableImpl::StringTableImpl(char *base)
    : data(reinterpret_cast<StringTableData *>(base)) {
    ChangeEndian(data->count);
    for (int i = 0; i < data->count; ++i) {
        char *&entry = data->strings[i];
        ChangeEndian(entry);
        entry = base + reinterpret_cast<unsigned int>(entry);
    }
}
