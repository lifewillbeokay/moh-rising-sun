// AI-assisted reconstruction from GR8E69. Fragments of the original Behavior.cpp;
// method names come from the symbol table, member names are inferred.
#include "AIBehavior.h"

CBehavior::CBehavior() {
    aiObject = 0;
    framesSinceUpdate = 0;
    enabled = 1;
}

CBehavior::~CBehavior() {}

int CBehavior::Update(int frames, bool force) {
    framesSinceUpdate += frames;
    if (force || framesSinceUpdate > GetHeuristicUpdateFrequency()) {
        framesSinceUpdate = 0;
        return UpdateHeuristicScore();
    }
    return 0;
}

void CBehavior::ReleaseResources() {}
int CBehavior::GetType() { return 0; }
unsigned int CBehavior::GetCRC() { return 0; }
void CBehavior::Enable() { enabled = 1; }
void CBehavior::Disable() { enabled = 0; }
BSObject *CBehavior::GetScriptObject() { return 0; }
