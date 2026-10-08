// AI-assisted reconstruction from GR8E69; see docs/Paths.md.
#include "AISplinePath.h"
CAISplinePath::CAISplinePath() {}
CAISplinePath::~CAISplinePath() { delete[] segments; segments = 0; }
