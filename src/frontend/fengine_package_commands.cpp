// AI-assisted GR8E69 reconstruction; see docs/research/feng-lifecycle.md.
#ifdef DECOMP_COMPARE
#include "FEngine.h"
#include "Shell.h"
void FEngine::ProcessPackageCommands() {
    FEPackage *previous = 0;
    const FEPackage *parent = 0;
    FEPackageCommand *command;
    while ((command = (FEPackageCommand *)commandQueue.RemHead()) != 0) {
        FEPackage *source = command->package;
        int priority = source ? source->priority : -1;
        if ((command->type & 1) && priority >= 0) {
            FEPackage *replace = source;
            if (!(command->type & 2)) {
                ((FEPackageList *)&packages)->ReplaceParentLinks(source, source->parent);
                replace = previous;
            } else parent = source->parent;
            if (command->package->parent) ((FEPackage *)command->package->parent)->controlMask |= command->mask;
            --priority; UnloadPackage(command->package);
            previous = replace;
        }
        if (command->type & 2) {
            FEPackage *pushed = PushPackage(command->name, priority + 1, command->mask);
            bool setPort = false;
            if (pushed && !(command->type & 1) && priority >= 0) {
                pushed->parent = command->package;
                command->package->controlMask &= ~command->mask;
                setPort = true;
            } else if (command->type & 1) {
                pushed->parent = parent;
                ((FEPackageList *)&packages)->ReplaceParentLinks(previous, pushed);
                setPort = true;
            }
            if (setPort) {
                unsigned long port = command->mask >> 1;
                if (port == 4) port = 3;
                else if (port != 2 && port != 1 && port != 0) port = 255;
                if (port != 255) pushed->groupId = g_inputMgr.GetPortNum(port);
            }
        }
        if (command->type & 4) {
            FEPackage *from = command->package, *to = (FEPackage *)from->parent;
            if (to) {
                unsigned long transfer = from->controlMask & command->mask;
                from->controlMask &= ~transfer; to->controlMask |= transfer;
                QueueMessage(0x334c5493, 0, to, (FEObject *)-4, command->mask);
            }
        }
        if (command->type & 8) {
            FEPackage *from = command->package, *to = (FEPackage *)packages.head;
            while (to && to->parent != from) to = (FEPackage *)to->next;
            if (to) {
                unsigned long transfer = from->controlMask & command->mask;
                from->controlMask &= ~transfer; to->controlMask |= transfer;
                QueueMessage(0x334c5493, 0, to, (FEObject *)-4, command->mask);
            }
        }
        delete command;
    }
}
#endif
