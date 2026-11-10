// AI-assisted reconstruction from GR8E69. Fragments of the original AreaPathNode.cpp;
// method names come from the symbol table.
#include "AISplinePath.h"

void CAreaPathNode::ResetOutOfArea() { MarkAsInArea(); }
void CAreaPathNode::MarkAsOutOfArea() { inArea = 0; }
void CAreaPathNode::MarkAsInArea() { inArea = 1; }
