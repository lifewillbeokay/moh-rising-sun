// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "Bullet.h"

// Maps script bullet types to weapon classes. Cases are grouped by result in
// result order; types 26 and 27 return 0 explicitly, separate from the default.
int GetClassFromBulletType(EScriptBulletType type) {
    switch (type) {
    case 26: case 27:
        return 0;
    case 11: case 17: case 18: case 25:
        return 1;
    case 1: case 4: case 6: case 7: case 8: case 9:
        return 2;
    case 0: case 14: case 15: case 19: case 20:
        return 3;
    case 12:
        return 4;
    case 5: case 13: case 16:
        return 5;
    case 2:
        return 6;
    case 3: case 21: case 22: case 23: case 24:
        return 7;
    }
    return 0;
}
