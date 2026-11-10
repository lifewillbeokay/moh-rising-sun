// AI-assisted reconstruction of source/Character/State.cpp; see docs/Animation.md.
#include "profile.h"
#include "CharacterTasks.h"
extern "C" int printf(const char *, ...);
typedef _STL::map<_STL::string, EALA::Resource::Pointer<EALA::Character::State>, StringCompare> StateMap;
extern StateMap g_mapStates;
namespace EALA {
namespace Character {

void *State::operator new(unsigned int, void *where) { return where; }

void State::operator delete(void *) {}

State::~State() {
    delete names;
    names = 0;
}

unsigned int State::SizeOf(unsigned int substates) { return substates * 4 + 40; }

void State::UnloadAll() { g_mapStates.clear(); }

} // namespace Character
} // namespace EALA
