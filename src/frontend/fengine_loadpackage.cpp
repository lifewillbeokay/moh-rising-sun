// FEngine: loading and unloading packages.
#include "FEPackageReader.h"
#include "FEngine.h"
#include "ShellManager.h"
#include <string.h>

FEPackage *FEngine::LoadPackage(const void *data, bool isLibrary) {
    FEPackageReader reader;
    FEPackage *package = reader.Load(data, game, this, unknown_10, unknown_14, isLibrary);
    if (!package) return 0;
    return package;
}
int FEngine::UnloadPackage(FEPackage *package) {
    for (FEPackage *current = (FEPackage *)packages.head; current; current = (FEPackage *)current->next) {
        if (package == current) {
            int canDelete = game ? game->PackageWillUnload(current) : 1;
            ((FEPackageList *)&packages)->RemovePackage(package);
            FEMinNode *next;
            FEMinList *queue = &commandQueue;
            for (FEPackageCommand *command = (FEPackageCommand *)queue->head; command; command = (FEPackageCommand *)next) {
                next = command->next;
                if (command->package == package) {
                    commandQueue.RemNode(command);
                    delete command;
                }
            }
            if (current->unknown_18) {
                AddToIdleList(package);
            } else {
                for (FENode *node = (FENode *)current->referencedPackages.head; node; node = (FENode *)node->next) {
                    FEPackage *library = FindLibraryPackage(node->hash);
                    if (library) {
                        long references = library->priority - 1;
                        if (references <= 0) UnloadLibraryPackage(library);
                        else library->priority = references;
                    }
                }
                current->Shutdown(game);
                if (canDelete) delete current;
            }
            return 1;
        }
    }
    return 0;
}
