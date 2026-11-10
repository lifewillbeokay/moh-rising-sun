// Each controller port's GBA link thread. Names of the functions, GbaThreadParam and the globals are symbols;
// members are inferred from what these functions do with them. See docs/Frontend.md.
#ifndef GAME_GBA_THREAD_H
#define GAME_GBA_THREAD_H
// Scoped views of the SDK types these functions use; the SDK's os.h does not build with this compiler.
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned int u32;
typedef long s32;
typedef void *OSMessage;
struct OSContext;
struct OSThread { u8 unknown_0[792]; };
struct OSAlarm { u8 unknown_0[40]; };
struct OSMessageQueue { u8 unknown_0[32]; };
typedef void (*OSAlarmHandler)(OSAlarm *, OSContext *);
extern "C" {
void GBAInit(void);
s32 GBAGetStatus(s32 chan, u8 *status);
s32 GBARead(s32 chan, u8 *dst, u8 *status);
s32 GBAWrite(s32 chan, u8 *src, u8 *status);
void OSCreateAlarm(OSAlarm *);
void OSSetAlarm(OSAlarm *, long long, OSAlarmHandler);
OSThread *OSGetCurrentThread(void);
long OSResumeThread(OSThread *);
long OSSuspendThread(OSThread *);
int OSDisableInterrupts(void);
int OSRestoreInterrupts(int);
void OSInitMessageQueue(OSMessageQueue *, OSMessage *, long);
int OSSendMessage(OSMessageQueue *, OSMessage, long);
int OSCreateThread(OSThread *, void *(*)(void *), void *, void *, u32, long, unsigned short);
unsigned long OSGetTick(void);
}
// What each side knows about a link, compared whole by the open handshake. Name inferred.
struct SGbaContext {
    u8 created;  // +0x0
    u8 chan;     // +0x1
    u8 unknown_2;
    u8 valid;    // +0x3
    u32 hostId;  // +0x4
    u32 session; // +0x8
    u32 unknown_c;
    u8 unknown_10[16];
};
struct GbaThreadParam;
typedef void (*GbaThreadCallback)(GbaThreadParam *, void *);
struct GbaThreadParam {
    OSMessageQueue queue;       // +0x000
    OSMessage message;          // +0x020
    GbaThreadCallback callback; // +0x024
    SGbaContext context;        // +0x028
    u32 lastSession;            // +0x048
    unsigned char *callbackArg; // +0x04c
    long unknown_50;            // +0x050
    int unknown_54;             // +0x054
    u8 unknown_58[0x5c - 0x58];
    int fileLength;             // +0x05c; cleared when the cartridge reports the boot code.
    u8 unknown_60[4];
    int initialCode;            // +0x064; the word the cartridge sent.
    u8 data[512];               // +0x068; copied from the shared buffer.
    u8 unknown_268[0x268 - 0x268];
    s8 chan;                    // +0x268
    u8 unknown_269;
    u8 unknown_26a;
    u8 state;                   // +0x26b
    u8 status;                  // +0x26c
    u8 unknown_26d[2];
    u8 initialCodeRead;         // +0x26f
    u8 linked;                  // +0x270
    u8 unknown_271[3];
};
extern long HostID;
extern GbaThreadParam Param[4];
extern SGbaContext GbaContext[4];
extern long SESSION_ID;
void GbaThreadInitGbaContext(GbaThreadParam *, int);
void GbaThreadAlarmHandler(OSAlarm *, OSContext *);
void GbaThreadSleep(long long);
long GbaThreadReadInitialCode(GbaThreadParam *);
long GbaThreadWriteInitialCode(GbaThreadParam *);
long GbaThreadReadContext(GbaThreadParam *);
void GbaThreadInit(long, GbaThreadParam *, OSThread *, unsigned char *, unsigned long, long, GbaThreadCallback, unsigned char *, long);
#endif
