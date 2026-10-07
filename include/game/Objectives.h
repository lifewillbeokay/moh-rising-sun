// AI-assisted scoped reconstruction from GR8E69; see docs/Objectives.md.
#ifndef GAME_OBJECTIVES_H
#define GAME_OBJECTIVES_H

// Method-only view of the user message table.
class CUserMessageTable {
  public:
    const char *GetString(int);
    int GetStringLength(int);
};

// Scoped view: the message table at +1632 is all the objectives code reads.
class PopUpMessageHandler {
  public:
    unsigned char unknown_0[1632];
    CUserMessageTable messages;
};
extern PopUpMessageHandler *g_pPopUpMessageHandler;

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
};

#endif
