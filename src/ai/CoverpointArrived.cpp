// AI-assisted reconstruction from GR8E69. Fragments of the original CoverpointBehavior.cpp;
// method names come from the symbol table, member names are inferred.
#include "SceneObjects.h"
#include "SceneNode.h"
#include "Weapon.h"
#include "AIBehavior.h"
#include "ScriptTimers.h"

void CCoverpointBehavior::Arrived() { arrived = 1; }
void CCoverpointBehavior::EndCoverpoint() { ended = 1; }
