// AI-assisted reconstruction from GR8E69; see docs/FlexProp.md.
#include "FlexProp.h"
#include "StringTable.h"

const char *FlexProp::GetString(int key) const {
    int *value = static_cast<int *>(GetDataPtr(key));
    if (value) {
        return g_pStringTable->LookupString(*value);
    } else {
        DebugMsg("WARNING: Couldn't find flexprop string field %x\n", key);
        return "";
    }
}
