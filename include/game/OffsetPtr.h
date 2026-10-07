#ifndef GAME_OFFSET_PTR_H
#define GAME_OFFSET_PTR_H

// Original template interface. Six property-related instantiations are rebuilt;
// other pointee types remain original code. See docs/BPD.md.
template <class T> void offsetPtr(T *&, int);
#endif
