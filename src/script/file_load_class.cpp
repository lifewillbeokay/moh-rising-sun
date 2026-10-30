// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptRuntime.h"
BSClass_struct *GetClassFromDatabase(const char *);
int AddClassToDatabase(BSClass_struct *);
int BSFileReadClass(const char *, bool, bool, BSClass_struct **);

int BSFileLoadClass(const char *name, bool, BSClass_struct **result) {
    *result = GetClassFromDatabase(name);
    if (*result) {
        int status = 0;
        if (!(*result)->loaded) status = BSFileReadClass(name, true, false, result);
        return status;
    }
    BSFileReadClass(name, true, true, result);
    return AddClassToDatabase(*result);
}
