// AI-assisted reconstruction; the header name is a project choice. The one definition
// of global placement new, so that a file can include any headers that need it.
#ifndef GAME_PLACEMENTNEW_H
#define GAME_PLACEMENTNEW_H
// The compiler's <new>, when a unit using containers has already included it, defines its own.
#ifndef __NEW__
inline void *operator new(unsigned int, void *p) { return p; }
#endif
#endif
