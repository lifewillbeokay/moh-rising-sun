// AI-assisted scoped reconstruction from GR8E69; see docs/Objectives.md.
#ifndef GAME_OBJECTIVES_H
#define GAME_OBJECTIVES_H

// The user message table: an array of string records, a parallel CRC table and a count.
// Member names follow what the methods do with them.
struct CUserMessageRecord {
    const char *text;
    unsigned short length;
};

class CUserMessageTable {
  public:
    CUserMessageTable();
    ~CUserMessageTable();
    void Unload();
    void Load(char *, char *, char *);
    int Loaded() const;
    const char *GetString(int);
    int GetIndexFromCRC(unsigned int);
    const char *GetStringFromCRC(unsigned int);
    int GetStringLength(int);

    CUserMessageRecord *records;
    void *field04;
    void *field08;
    unsigned int *crcTable;
    int count;
};

#include "PopUpMessage.h"
class CFont;
class CSprite;

// The pop-up message handler: viewports of four message records, the fonts and sprites its loop
// draws with, and two message tables. Names of the members other than the tables are inferred.
class PopUpMessageHandler {
public:
    PopUpMessageHandler(char *, char *, char *);
    ~PopUpMessageHandler();
    void LoadSubtitles(char *, char *, char *);
    void ShutdownPopUpMessageHandler();
    unsigned short GetOldest(unsigned char viewport);
    short GetNewest(unsigned char viewport);
    short CheckForEmptySlot(unsigned char viewport);
    void UpdateScroll(unsigned char viewport, short id);
    void Clear();
    void AddPopUpMessage(char *, unsigned char, unsigned short, unsigned short, unsigned char, float, unsigned char, unsigned char, unsigned char, unsigned char, unsigned char, int, short);

    PopUpMessage popups[5][4];
    unsigned char unknown640[4];
    CFont *font644;                  // +0x644
    CFont *font648;                  // +0x648
    CSprite *sprite64c;              // +0x64c
    CSprite *sprite650;              // +0x650
    int settings654;                 // +0x654: copied from g_GameSettings at construction
    short slots658[4];               // +0x658: all -1 at construction
    CUserMessageTable tables[2];     // +0x660: messages, subtitles
    unsigned char unknown688[0x6c4 - 0x688];
    float lineHeight;                // +0x6c4
};
extern PopUpMessageHandler *g_pPopUpMessageHandler;
void appendMessageToPrompt(const char *, int);
void displayPromptBuffer(short, float);

// One of ten 20-byte objective records. Names are descriptive.
struct PlayerObjective {
    const char *prompt;
    unsigned char promptLength;
    int completed;
    int bonus;
    int shown;
};

// Scoped view through +208; the complete size is not established.
class CPlayerObjectives {
  public:
    PlayerObjective objectives[10];
    int mainCount;
    int bonusCount;
    int field_d0;
    CPlayerObjectives();
    void Reset();
    void AddObjective(unsigned int, unsigned int, bool, bool, bool);
    void ShowObjective(unsigned int);
    void ChangeObjectivePrompt(unsigned int, unsigned int);
    void SetObjectiveStatus(unsigned int, bool);
    int GetObjectiveStatus(unsigned int);
    int GetNumCompletedObjectives();
    int CheckIfAllMainObjectivsAreCompleted();
    int GetNumBonusObjectives();
    int GetNumCompletedBonusObjectives();
    void UpdateUI(class FEPackage *);
    void Serialize();
};

// Original shared storage; this declaration does not allocate a new object.
extern CPlayerObjectives g_Objectives;

#endif
