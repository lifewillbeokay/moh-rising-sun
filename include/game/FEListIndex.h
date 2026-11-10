#ifndef GAME_FE_LIST_INDEX_H
#define GAME_FE_LIST_INDEX_H
// Descriptive helper for the signed remainder operation repeated in list boxes.
// For a negative multiple it returns count, rather than zero, as the target does.
static inline long FEWrapIndex(long index, long count) {
    if (index >= 0) return index % count;
    long remainder = -index % count;
    return count > 1 ? count - remainder : 0;
}
#endif
