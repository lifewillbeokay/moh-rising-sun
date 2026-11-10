// ShellManager's package callbacks and message routing; see docs/Frontend.md.
#include "ShellHandlerMap.h"
void ShellManager::PackageWasLoaded(FEPackage *package) {
    PackageHandler *handler = factory->CreatePackageHandler(package->name);
    if (handler) {
        handler->package = package;
        handler->engine = engine;
        handler->shell = this;
        handler->Init(package->name);
        handlers.insert(_STL::make_pair(package, handler));
    }
    package->Update(engine, 0);
}
int ShellManager::PackageWillUnload(FEPackage *package) {
    ShellHandlerMap::iterator it = handlers.find(package);
    if (it != handlers.end()) {
        PackageHandler *handler = (*it).second;
        handlers.erase(it);
        factory->DestroyPackageHandler(handler);
    }
    return 1;
}
void ShellManager::NotificationMessage(unsigned long message, FEObject *object, unsigned long param, unsigned long package) {
    ShellHandlerMap::iterator it = handlers.find((FEPackage *)package);
    if (it != handlers.end()) {
        (*it).second->HandleEvent(message, object, param);
    }
}
void ShellManager::NotifySoundMessage(unsigned long message, FEObject *object, unsigned long param, unsigned long package) {
    ShellHandlerMap::iterator it = handlers.find((FEPackage *)package);
    if (it != handlers.end()) {
        (*it).second->HandleSoundEvent(message, object);
    }
}
