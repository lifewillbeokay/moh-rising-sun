#ifndef GAME_FLEXPROP_DATABASE_H
#define GAME_FLEXPROP_DATABASE_H

// Original static method symbols; no complete database class is inferred.
class FlexProp;
class FlexPropDatabase {
public:
    static void Rewind();
    static int GetNumProperties();
    static void GetPropertyByIndex(int, FlexProp &);
};
#endif
