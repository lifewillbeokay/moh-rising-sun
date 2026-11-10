// AI-assisted reconstruction from GR8E69. Views of the bot area classes in source/BOT/BotArea.cpp;
// class names come from the symbol table, sizes from array strides, and member names are
// inferred. See docs/Behaviors.md.
#ifndef GAME_BOT_AREA_H
#define GAME_BOT_AREA_H
#include "AISplinePath.h"
struct JumpData {
    unsigned char unknown00[48];
};
class CBotArea {
  public:
    JumpData *GetJumpData();
    signed char jumpIndex; // +0x00; negative for none.
    unsigned char unknown01[16 - 1];
};
class CAreaEdge {
  public:
    void GetEdgePoints(CVector3 &, CVector3 &);
    signed char coverPoints; // +0x00; a cover edge has a positive count.
    unsigned char unknown01[0x0c - 1];
    void *coverData;         // +0x0c; a cover edge has this set.
    unsigned short vertex0;  // +0x10
    unsigned short vertex1;  // +0x12
    unsigned char unknown14[64 - 0x14];
};
#endif
