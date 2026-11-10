#ifndef RCMP_H
#define RCMP_H

// RCMP, the movie player. Class and method names come from the original symbols; member names
// follow what the code does with them. Codec slot names other than the destructor are inferred. The original table has
// pure virtuals in those slots; they are declared plainly here so that a unit does not own the table.
namespace RCMP {

class DECODER;
class STREAMER;
class FRAME;

class CHUNK {
public:
    CHUNK();
    ~CHUNK();
    static void operator delete(void *p);

    void *data;
    void *next;
    unsigned int size;
};

typedef void (*GETCHUNKFUNC)(DECODER *, STREAMER *, CHUNK **);
typedef void (*RELEASECHUNKFUNC)(DECODER *, STREAMER *, CHUNK *);

class CODEC_IDATA {
public:
    CODEC_IDATA();
    CODEC_IDATA(STREAMER *, GETCHUNKFUNC, RELEASECHUNKFUNC, unsigned int);

    STREAMER *streamer;
    GETCHUNKFUNC getChunk;
    RELEASECHUNKFUNC releaseChunk;
    unsigned int value;
};

class CODEC {
public:
    virtual ~CODEC() {}
    static void *operator new(unsigned int size);
    static void operator delete(void *p);
    virtual int Init(DECODER *, CHUNK *);
    virtual FRAME *GetFrame(unsigned int);
    virtual unsigned int GetCurrentFrameNumber();
    virtual float GetFrameRate();
    virtual void ReleaseFrame(FRAME *);
};

// The memory hooks; a virtual table pointer follows them at +12.
class RCMP_SYSTEM {
public:
    RCMP_SYSTEM();
    virtual ~RCMP_SYSTEM();
    void SetREALDefaults();

    void *(*allocate)(const char *, int, int, int, int);
    int (*release)(void *);
    void *field8;
};

extern RCMP_SYSTEM rcmp_sys;

inline void CHUNK::operator delete(void *p) { rcmp_sys.release(p); }
inline CHUNK::~CHUNK() {}
inline void CODEC::operator delete(void *p) { rcmp_sys.release(p); }

class DECODER {
public:
    DECODER(const CODEC_IDATA *);
    virtual ~DECODER();
    static void operator delete(void *p) { rcmp_sys.release(p); }
    int ChooseCodec(CODEC *, CHUNK *);
    void FreeChosenCodec();
    unsigned int GetCurrentFrameNumber();
    float GetFrameRate();
    FRAME *GetFrame(unsigned int);
    void ReleaseFrame(FRAME *);
    CHUNK *GetChunk();
    void ReleaseChunk(CHUNK *);

    CHUNK *pending;
    CODEC_IDATA idata;
    CODEC *codec;
};

class AUDIO_PLAYER {
public:
    AUDIO_PLAYER(int, int);
    ~AUDIO_PLAYER();
    static void operator delete(void *p) { rcmp_sys.release(p); }
    int SetSpeed(unsigned int);
    int StartSound();
    int SetVol(unsigned int);

    float speed;
    void *buffer;
    int field08;
    int stream;
    int request;
};

}

#endif
