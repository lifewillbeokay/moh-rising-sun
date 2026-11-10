// A text file split into "[section]" blocks, with "key = value" lookups in the current
// section. Member names follow what src/game/configfile.cpp does with them.
#ifndef GAME_CONFIG_FILE_H
#define GAME_CONFIG_FILE_H

struct CConfigSection {
    CConfigSection() : name(0), start(0), end(0) {}
    char *name;
    char *start;
    char *end;
};

class CConfigFile {
public:
    CConfigFile(const char *fileName);
    ~CConfigFile();
    int SetSection(const char *name);
    char *GetVar(const char *key);
    int AsInt(const char *key, int &value);
    int AsBool(const char *key, bool &value);
    int AsString(const char *key, char *buffer, int size);

    CConfigSection sections[64];
    int count;
    CConfigSection *current;
    char *data;
};

#endif
