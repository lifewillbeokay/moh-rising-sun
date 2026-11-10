// ShellManager's map from packages to their handlers. Only the units that use it need the container
// headers, so ShellManager.h declares the member as this type when this header comes first.
#ifndef GAME_SHELL_HANDLER_MAP_H
#define GAME_SHELL_HANDLER_MAP_H
#include "profile.h"
#include <map>
class FEPackage;
class PackageHandler;
typedef _STL::map<FEPackage *, PackageHandler *> ShellHandlerMap;
#include "PackageHandlers.h"
#include "GameSettings.h"
#include "Shell.h"
#endif
