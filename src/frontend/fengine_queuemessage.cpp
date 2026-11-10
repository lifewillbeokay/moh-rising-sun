// FEngine.cpp: queued messages and package commands.
#include "FEngObjects.h"
#include "FEngStrings.h"
#include <string.h>
class FEJoyPad { public: FEJoyPad(); unsigned char state[136]; };
class FEListBox { public: void ScrollSelection(long, long); };
class FECodeListBox { public: void ScrollSelection(long, long); };
inline void *operator new(unsigned int size, const char *label) { return DWI_alloc(label, size, 256); }

void FEngine::QueueMessage(unsigned long message, FEObject *from, FEPackage *package, FEObject *to, unsigned long mask) {
    FEMessageNode *node = new ("source/FEngine/FEngine.cpp:1679") FEMessageNode;
    node->message = message;
    node->from = from;
    node->package = package;
    node->to = to;
    node->mask = mask;
    if (debugMessages) game->DebugMessageQueued(message, from, package, to, mask);
    messageQueue.AddTail(node);
}
void FEngine::SendMessageToGame(unsigned long message, FEObject *object, FEPackage *package, unsigned long mask) {
    game->NotificationMessage(message, object, mask, (unsigned long)package);
}
void FEngine::QueuePackageSwitch(const char *name, FEPackage *package) {
    FEPackageCommand *command = new ("source/FEngine/FEngine.cpp:1767") FEPackageCommand;
    command->package = package;
    command->mask = package->controlMask;
    command->type = 3;
    command->SetName(name);
    commandQueue.AddTail(command);
    if (package) package->controlMask = 0;
}
void FEngine::QueuePackagePush(const char *name, FEPackage *package, unsigned long mask) {
    FEPackageCommand *command = new ("source/FEngine/FEngine.cpp:1784") FEPackageCommand;
    command->package = package;
    command->mask = mask;
    command->type = 2;
    command->SetName(name);
    commandQueue.AddTail(command);
    if (package) package->controlMask &= ~mask;
}
void FEngine::QueuePackagePop(FEPackage *package) {
    FEPackageCommand *command = new ("source/FEngine/FEngine.cpp:1801") FEPackageCommand;
    command->package = package;
    command->mask = package->controlMask;
    command->type = 1;
    commandQueue.AddTail(command);
    package->controlMask = 0;
}
void FEngine::QueuePackageUserTransfer(FEPackage *package, bool give, unsigned long mask) {
    FEPackageCommand *command = new ("source/FEngine/FEngine.cpp:1819") FEPackageCommand;
    command->package = package;
    command->mask = package->controlMask & mask;
    command->type = give ? 8 : 4;
    commandQueue.AddTail(command);
}
