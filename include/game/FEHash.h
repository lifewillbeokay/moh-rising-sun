#ifndef GAME_FE_HASH_H
#define GAME_FE_HASH_H

// Original symbols; signed char input and unsigned hash bits follow GR8E69.
// See docs/FEHash.md for the character conversion and overflow behavior.
char FEUpperCase(char);
unsigned int FEHashUpper(const char *);
#endif
