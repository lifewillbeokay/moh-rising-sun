// AI-assisted GR8E69 reconstruction; see docs/research/save-popups.md.
#include "profile.h"
#include "SaveGamePopups.h"
#include <list>
extern _STL::list<CSaveGamePopup *> g_listPopups;
void CSaveGamePopup::Push() { g_listPopups.push_front(this); unknown_14 = 0; OnPush(); }
void CSaveGamePopup::Pop() { g_listPopups.pop_front(); }
