// AI-assisted view of the sound system's C++ front end (ssys.cpp); see docs/Sound.md. The classes
// and method names come from the symbol table; only the type names of the enums are recovered.
#ifndef GAME_SND_SYSTEM_H
#define GAME_SND_SYSTEM_H
namespace Snd {
// Only the type names are recovered; the enumerators stand for observed values.
enum OutputMode { OutputMode_0 = 0, OutputMode_1 = 1, OutputMode_2 = 2, OutputMode_3 = 3, OutputMode_4 = 4, OutputMode_5 = 5, OutputMode_8 = 8 };
enum Device { Device_Main = 0, Device_Fast = 2 };
extern void *pheap;
extern OutputMode outputmode;
extern int heapsize;
class System {
  public:
    static int SetMaxBanks(int);
    static int CapOutputMode(OutputMode, bool *);
    static int SetOutputMode(OutputMode);
    static int SetVoices(Device, int);
    static int SetSndLoadsIopModules(bool);
    static int Init(int);
    static int Restore();
    static int VectorToReal();
};
class Memory {
  public:
    static int SetHeap(Device, void *, int);
    static int SetHeapFastRam(void *, int);
};
} // namespace Snd
#endif
