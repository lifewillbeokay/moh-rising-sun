// AI-assisted reconstruction of the sound server's default mutex hooks (sserver.c); see docs/Sound.md.
void SNDI_mutexlock();
void SNDI_mutexunlock();
namespace Snd {
void DefaultMutexLockFn() {
    SNDI_mutexlock();
}

void DefaultMutexUnlockFn() {
    SNDI_mutexunlock();
}
}
