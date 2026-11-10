// AI-assisted reconstruction from GR8E69. Fragments of the original CoverpointBehavior.cpp;
// method names come from the symbol table, member names are inferred.
#include "SceneObjects.h"
#include "SceneNode.h"
#include "Weapon.h"
#include "AIBehavior.h"
#include "ScriptTimers.h"

CCoverpointBehavior::CCoverpointBehavior()
    : heuristicScore(0.0f), coverScript(0), arrived(0), stopping(0), ended(0), coverPoint(0), soldier(0), field30(0.0f), forced(0) {}

CCoverpointBehavior::~CCoverpointBehavior() {}
float CCoverpointBehavior::GetCurrentHeuristicScore() { return heuristicScore; }
int CCoverpointBehavior::GetHeuristicUpdateFrequency() { return coverScript ? 1 : 60; }

