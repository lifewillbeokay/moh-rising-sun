// A fragment of the original input code: polling any device for input. Names as in Shell.h, inferred from what these methods do.
#include "Shell.h"

extern "C" int PAD_scan(int);

int CInputManager::GetInputAnyDevice() const {
    if (PAD_scan(0xffff))
        return 1;
    for (int i = 0; i < GetNumDevices(); i++) {
        const CInputEvent *events = GetEventTable(i);
        if (events) {
            if (events[129].value)
                return 1;
            if (events[130].value)
                return 1;
            if (events[131].value)
                return 1;
            if (events[132].value)
                return 1;
        }
    }
    return 0;
}
