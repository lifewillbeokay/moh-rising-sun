// AI-assisted reconstruction of the sound manager's stream pool; see docs/Sound.md.
#include "SoundManager.h"
#include "SoundPlayOptions.h"
long long MathFunRandomI64(long long, long long);
extern "C" int SNDSTRM_create(SNDPLAYOPTS *, int, int, void *, int);
extern "C" int SNDSTRM_destroy(int);
extern "C" int SNDSTRM_purge(int);
extern "C" int SNDSTRM_status(int, int *);
extern "C" int printf(const char *, ...);

int cSoundMan::SelectGroupEntry(int *weights, int count) {
    int total = 0;
    int low = 0;
    int i = 0;
    if (count > 1) {
        for (; i < count; i++)
            total += weights[i];
        int pick = MathFunRandomI64(0, total);
        int sum = weights[0];
        i = 1;
        if (i < count) {
            do {
                if (pick >= low && pick <= sum)
                    break;
                low = sum;
                sum += weights[i];
                i++;
            } while (i < count);
        }
        return i - 1;
    }
    return 0;
}

int cSoundMan::GetSound(SoundEntryBank &entry, unsigned int alias) {
    int index = SearchForAlias(alias);
    if (index == -1)
        return 0;
    entry.SetSoundEntry(aliases[index].row, columns, columnCount);
    entry.field_d8 = bankMan.GetBankHandle(entry);
    if (entry.GetTypeID() == 0)
        entry.ResolveSlotNum();
    return 1;
}

int cSoundMan::InitStreams(unsigned char *buffer, int size, int count) {
    SNDPLAYOPTS opts;
    streamCount = count;
    streamBufferSize = size / (unsigned char)count;
    SNDplaysetdef(&opts);
    for (int i = 0; i < streamCount; i++) {
        streamBuffers[i] = buffer + i * streamBufferSize;
        int handle = SNDSTRM_create(&opts, 10, 150, streamBuffers[i], streamBufferSize);
        if (handle < 0)
            return -1;
        streams[i] = handle;
    }
    streamsReady = 1;
    return 0;
}

void cSoundMan::ShutdownStreams() {
    for (int i = 0; i < streamCount; i++) {
        SNDSTRM_destroy(streams[i]);
        streamBuffers[i] = 0;
        streams[i] = -12;
    }
    streamsReady = 0;
}

void cSoundMan::PurgeStreams() {
    for (int i = 0; i < streamCount; i++)
        SNDSTRM_purge(streams[i]);
}

int cSoundMan::GetFreeStream() {
    if (streamsReady != 1)
        return -1;
    for (int i = 0; i < streamCount; i++) {
        int status[4];
        SNDSTRM_status(streams[i], status);
        if (status[0] == 0)
            return streams[i];
    }
    printf("SOUND ERROR!!! CAN'T ALLOCATE STREAM!\n");
    return -1;
}
