// FEngine: dispatching the message queue.
#include "FEngine.h"
#include "ShellManager.h"
#include "FEMessageResponse.h"

void FEngine::ProcessMessageQueue() {
    FEMessageNode *node;
    for (node = (FEMessageNode *)messageQueue.RemHead(); node; node = (FEMessageNode *)messageQueue.RemHead()) {
        if (debugMessages) game->DebugMessageProcessed(node->message, node->to, node->from, node->package, node->mask);
        switch ((unsigned long)node->to) {
        case 0: {
            FEList *list = &packages;
            for (FEPackage *package = (FEPackage *)list->head; package; package = (FEPackage *)package->next) {
                ProcessGlobalMessage(package, node->message, node->mask);
                FEMsgTargetList *targets = package->GetMessageTargets(node->message);
                if (targets) {
                    unsigned long count = targets->count;
                    unsigned long message = node->message;
                    for (unsigned long i = 0; i < count; i++) ProcessObjectMessage(targets->targets[i], package, message, node->mask);
                }
            }
            break;
        }
        case (unsigned long)-1:
            game->NotificationMessage(node->message, node->from, node->mask, (unsigned long)node->package);
            break;
        case (unsigned long)-2: {
            FEList *list = &packages;
            for (FEPackage *package = (FEPackage *)list->head; package; package = (FEPackage *)package->next) {
                ProcessGlobalMessage(package, node->message, node->mask);
            }
            break;
        }
        case (unsigned long)-3:
            ProcessGlobalMessage(node->package, node->message, node->mask);
            break;
        case (unsigned long)-4: {
            FEList *list = &packages;
            FEPackage *package = (FEPackage *)list->head;
            while (package && package != node->package) package = (FEPackage *)package->next;
            if (package) {
                ProcessGlobalMessage(package, node->message, node->mask);
                FEMsgTargetList *targets = package->GetMessageTargets(node->message);
                if (targets) {
                    unsigned long count = targets->count;
                    unsigned long message = node->message;
                    for (unsigned long i = 0; i < count; i++) ProcessObjectMessage(targets->targets[i], package, message, node->mask);
                }
            }
            break;
        }
        case (unsigned long)-5:
            game->NotifySoundMessage(node->message, node->from, node->mask, (unsigned long)node->package);
            break;
        case (unsigned long)-6:
            switch (node->message) {
            case 0x5d4ce32d:
                resetRequested = 0;
                break;
            case 0x59bed120:
                resetRequested = 1;
                break;
            }
            break;
        default:
            ProcessObjectMessage(node->to, node->package, node->message, node->mask);
            break;
        }
        delete node;
    }
}
