// AI-assisted reconstruction from GR8E69. A fragment of the original source/BOT/BotArea.cpp;
// function and variable names come from the symbol table, member names are inferred.
#include "BotArea.h"

void AddLink(CAreaPathNode *, int, CAreaPathNode *, int);

void SetJumpConnections2(float height, CAreaPathNode *from, CAreaPathNode *to, int edge) {
    static float MAX_JUMP_HEIGHT = 1.0f;
    if (!(height <= 0.0f)) {
        AddLink(from, edge, to, 3);
    } else if (!(__builtin_fabsf(height) >= MAX_JUMP_HEIGHT)) {
        AddLink(from, edge, to, 2);
    }
}
