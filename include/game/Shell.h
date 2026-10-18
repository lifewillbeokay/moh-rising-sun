// AI-assisted scoped reconstruction from GR8E69; see docs/Script.md.
#ifndef GAME_SHELL_H
#define GAME_SHELL_H

// Method-only view of the input manager singleton.
class CInputManager {
  public:
    void SetBlockVibrations(bool, unsigned char);
    void SetFeedback(unsigned char, unsigned char, unsigned char, float, float);
};
extern CInputManager g_inputMgr;

#endif
