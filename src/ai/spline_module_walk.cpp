// AI-assisted reconstruction from GR8E69; see docs/Paths.md.
#include "AISplinePath.h"
void CAISplinePathModule::StartSplinePathWalkForward(CAISplinePath *path) {
    StopSplinePathWalk();
    areaPathFinding->StopAStarPathWalk();
    traversal.PrepareForTraversalForward(path, true, areaPathFinding);
    locomotion->walkType = 1;
    WalkNextSplinePathPoint();
}
void CAISplinePathModule::WalkNextSplinePathPoint() {
    CVector3 point;
    traversal.GetNextPointForward(physics->position, physics->axes.axis1, &point, scriptObject, this, areaPathFinding);
    locomotion->objectivePosition = point;
}
void CAISplinePathModule::StopSplinePathWalk() { locomotion->walkType = 0; }
void CAISplinePathModule::ContinueSplinePathWalk() {
    switch (traversal.state) {
    case 2: case 4: case 6:
        traversal.ContinueTraversalForwardAStar(physics, areaPathFinding);
        // Fall through to resume spline walking after A-star setup.
    case 1: case 3: case 5:
        locomotion->walkType = 1;
        break;
    }
}
void CAISplinePathModule::LoopSplinePathWalk() {
    StopSplinePathWalk();
    areaPathFinding->StopAStarPathWalk();
    traversal.RestartForwardTraversal(areaPathFinding);
    locomotion->walkType = 1;
    WalkNextSplinePathPoint();
}
int CAISplinePathModule::GetWalkType() { return 1; }
void CAISplinePathModule::StopWalk() { StopSplinePathWalk(); }
