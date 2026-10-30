// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptRuntime.h"
int BSFileLoadClass(const char *, bool, BSClass_struct **);
int BSFileGetClass(const char *name, BSClass_struct **result) {
    return BSFileLoadClass(name, true, result);
}
