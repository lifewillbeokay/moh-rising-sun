// HandlerMovieMenu, the movie menus: construction; see docs/Frontend.md.
#include "PackageHandlers.h"
#include <stdio.h>
HandlerMovieMenu::HandlerMovieMenu() {
    char name[16];
    for (int i = 0; i < 11; i++) {
        sprintf(name, "button %.2d", i + 1);
        buttonHashes[i] = FEHashUpper(name);
    }
}
