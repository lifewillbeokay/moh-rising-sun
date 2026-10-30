#ifndef GAME_SCRIPT_MACHINE_H
#define GAME_SCRIPT_MACHINE_H

// Scoped interpreter interface. Startup and shutdown are reconstructed; global
// storage remains external. The stack holds values and 32-bit target addresses.
extern int *g_piStackTop;
extern int *g_piIP;
extern int *g_piFrame;
extern int *g_piGlobals;
extern int **g_piCodeBase;
extern int g_iStopThread;
extern void *g_pObj;

// The signed argument occupies the high half of the instruction word.
inline int BSInstructionArgument() {
    return static_cast<short>(*g_piIP >> 16);
}

#endif
