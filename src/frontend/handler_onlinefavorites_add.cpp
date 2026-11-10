// HandlerOnlineFavorites' list update and SetFavorite; see docs/Frontend.md.
#include "PackageHandlers.h"
#include "GameSettings.h"
#include <string.h>
class ListBox {
  public:
    void Update();
};
void HandlerOnlineFavorites::UpdateFavListBox() {
    if (listBox) {
        listBox->Update();
    }
}
void HandlerOnlineFavorites::SetFavorite(const char *name) {
    for (int i = 0; i < 16; i++) {
        if (strcmp(g_GameSettings.GetProfile(0)->names[i + 1], name) == 0) {
            return;
        }
        if (strlen(g_GameSettings.GetProfile(0)->names[i + 1]) == 0) {
            strcpy(g_GameSettings.GetProfile(0)->names[i + 1], name);
            return;
        }
    }
}
