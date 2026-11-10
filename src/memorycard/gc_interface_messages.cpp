// AI-assisted reconstruction of the memory-card library's GameCube interface: messages, the card check and clearing the task; see docs/Memory.md.
#include "RealmcInterface.h"
namespace Realmc {
void GCInterface::SetTrcMessageFile(const void *file) {
    MUTEX_lock(mMutex);
    locale.SetTrcFile(file);
    MUTEX_unlock(mMutex);
}
void GCInterface::SendMessage(UserMessage message, int value) {
    MUTEX_lock(mMutex);
    mUserMsg = message;
    switch (message) {
    case 8:
        mNoCheckingCardWarning = !value;
        break;
    case 9:
        mMsgTimer.active = 0;
        break;
    }
    MUTEX_unlock(mMutex);
}
int GCInterface::ConvertUmsgToOption(UserMessage message, int options, TaskID task) {
    int found[4];
    char text[128];
    if (task != 1)
        options &= ~0x10;
    int count = 0;
    int bit = 1;
    while (options) {
        if (options & 1)
            found[count++] = bit;
        bit <<= 1;
        options >>= 1;
    }
    if (message == 10 && count > 0)
        return found[0];
    if (message == 11 && count > 1)
        return found[1];
    if (message == 12 && count > 2)
        return found[2];
    if (message == 13 && count > 3)
        return found[3];
    sprintf(text, "Error in GCInterface::ConvertUmsgToOption: received invalid user message, %d\n", message);
    return 0;
}
int GCInterface::CheckCard(const CardID &) {
    if (!mpDriver->IsCardPresent())
        return 3;
    return 0;
}
void GCInterface::ClearTask() {
    mTaskManager.current = 0;
    mTaskMsg.Reset();
}
} // namespace Realmc
