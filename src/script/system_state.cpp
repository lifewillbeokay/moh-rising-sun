// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptStartup.h"
extern EScriptSystemState g_script_system_state;
void BSUtilSetScriptingSystemState(EScriptSystemState state) { g_script_system_state = state; }
EScriptSystemState BSUtilGetScriptingSystemState() { return g_script_system_state; }
