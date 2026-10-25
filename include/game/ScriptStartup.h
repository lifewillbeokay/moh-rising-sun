// AI-assisted original interfaces; see docs/Script.md and docs/TriggerObject.md.
#ifndef GAME_SCRIPT_STARTUP_H
#define GAME_SCRIPT_STARTUP_H
// Enum tag is recovered; these two value names describe observed startup phases.
enum EScriptSystemState { SCRIPT_SYSTEM_STARTUP = 1, SCRIPT_SYSTEM_RUNNING = 2 };
void BSUtilSetScriptingSystemState(EScriptSystemState);
EScriptSystemState BSUtilGetScriptingSystemState();
void BSUtilInitStartupAllocator(int);
void BSInitBSObjectStartupAllocator(int);
struct BSObject;
class BSGO_Basic;
class TriggerObject;
int CreateBSObject(const char *, BSObject **, BSGO_Basic *, TriggerObject *, void **);
#endif
