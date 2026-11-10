// AI-assisted scoped reconstruction from GR8E69; see docs/Script.md.
#ifndef GAME_SHELL_H
#define GAME_SHELL_H

struct KEYMAP;

// An input event record; +4 and +8 are cleared together by Reset.
struct CInputEvent {
    int field00;
    int field04;
    int value;
};

// A controller's state: its pad port, input state at +2 and event table at +0x60.
class CGcDevice {
  public:
    void Reset();
    void Update();
    void SetActuatorFX(unsigned char, unsigned char, float, float);
    void UpdateActuators(float);
    const void *GetInputState() const;
    CInputEvent *GetEventTable();
    const CInputEvent *GetEventTable() const;

    signed char port;
    unsigned char unknown01[0x8ec - 1];

    static unsigned char MAX_NUM_PORTS;
};

// The device manager at the start of the input manager.
class CDeviceManager {
  public:
    CDeviceManager();
    void SetPort(unsigned char, signed char);
    int GetNumDevices() const;
    int GetNumPorts() const;
    const void *GetDeviceState(unsigned char) const;
    void UpdateActuators(float);
    int SetActuatorState(unsigned char, unsigned char, unsigned char, float, float);
    CInputEvent *GetEventTable(unsigned char);
    const CInputEvent *GetEventTable(unsigned char) const;
    int IsActive(signed char) const;
    int GetPortNum(unsigned char) const;
    int GetCurrentState(unsigned char);

    CGcDevice devices[4];       // +0x0000
    unsigned char numDevices;   // +0x23b0
    int stateChanged;           // +0x23b4
    int field23b8;              // +0x23b8
    int field23bc;              // +0x23bc
};

// The input manager singleton; members are named by what these methods do with them.
class CInputManager : public CDeviceManager {
  public:
    CInputManager();
    void Init();
    void Shutdown();
    int CheckControllers(bool);
    void SetController(unsigned char, signed char);
    void SetKeymap(unsigned char, KEYMAP *);
    int GetValue(unsigned char, unsigned short) const;
    int GetInputAnyDevice() const;
    bool IsReady(signed char) const;
    int IsActive(signed char) const;
    void SetFeedback(unsigned char, unsigned char, unsigned char, float, float);
    void Reset();
    void SetBlockVibrations(bool, unsigned char);
    void SetMasterBlockVibrations(bool, unsigned char);
    void SetSPLockedPort(unsigned char);
    int FindDeviceIDFromPortNum(unsigned char) const;
    void SetBadControllerPort(unsigned char);

    KEYMAP *keymap;
    KEYMAP *portKeymaps[4];
    int blockVibrations[4];
    int masterBlockVibrations[4];
    int padOption;              // +0x23f4; the profile's byte at +0x28 sets it. Meaning unknown.
    int controllerChecksEnabled; // +0x23f8
    signed char controllers[4];  // +0x23fc
    signed char field2400;
    int field2404;
    signed char field2408;
    int badControllerSet;
    signed char badControllerPort;
};
extern CInputManager g_inputMgr;

#endif
