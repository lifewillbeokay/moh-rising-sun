// EA's MAD movie codec (rcmp_mad_codec.cpp). Class and method names come from the symbol table. Member, local and helper
// names are inferred from the code. The chunk tags are 'MADk', 'MADm' and 'MADe': the first two take a frame from
// CreateKorM and the last from CreateE; what each kind of frame means is inferred from those names.
#include "RcmpMadCodec.h"

extern "C" void MEM_fill(void *, int, int);
extern "C" void REAL_abortmessage(const char *, ...);
void MAD_initdecode(const unsigned short *, int, int);
void MAD_decodemacroblock(const unsigned char *, const unsigned char *, const unsigned char *, unsigned char *,
                          unsigned char *, unsigned char *, int);

static unsigned int GetChunkType(RCMP::CHUNK *chunk) {
    if (chunk) {
        unsigned char *tag = (unsigned char *)chunk->next;
        return tag[0] << 24 | tag[1] << 16 | tag[2] << 8 | tag[3];
    }
    return 0x4d41446b;
}

struct MAD_SHAPE {
    unsigned char kind;
    unsigned char unknown01[3];
    short width;  // +0x04
    short height; // +0x06
    unsigned char unknown08[4];
    unsigned int flags; // +0x0c
    int pixelOffset;    // +0x10
};

static inline int ReadLong(const unsigned char *bytes) { return bytes[3] << 24 | bytes[2] << 16 | bytes[1] << 8 | bytes[0]; }

static inline unsigned char *ShapePixels(MAD_SHAPE *shape) {
    if (shape->flags & 0x10000000)
        return (unsigned char *)shape + shape->pixelOffset;
    return (unsigned char *)shape + 16;
}

// A new link holds 3, a destroyed one 7 and a removed one 11, as markers.
struct FrameLink {
    FrameLink *next;
    FrameLink *previous;
    FrameLink() {
        next = (FrameLink *)3;
        previous = (FrameLink *)3;
    }
    ~FrameLink() {
        next = (FrameLink *)7;
        previous = (FrameLink *)7;
    }
    void Clear() {
        next = this;
        previous = this;
    }
    void Remove() {
        FrameLink *after = next;
        FrameLink *before = previous;
        before->next = after;
        after->previous = before;
        next = (FrameLink *)11;
        previous = (FrameLink *)11;
    }
};

// A list's head is a link pointing at itself. The constructor clears it twice, which the code really does.
struct FrameList : FrameLink {
    FrameList() {
        Clear();
        Clear();
    }
};

class MAD_FRAME {
  public:
    int unknown00;
    MAD_SHAPE *shape;
    FrameLink link; // +0x08
    int refs;       // +0x10
    MAD_FRAME(unsigned int, unsigned int);
    ~MAD_FRAME();
    static void *operator new(unsigned int size) { return RCMP::rcmp_sys.allocate("", size, 0, 0, (int)RCMP::rcmp_sys.field8); }
    static void operator delete(void *p) { RCMP::rcmp_sys.release(p); }
};

static inline void AddTail(FrameLink *list, FrameLink *link) {
    FrameLink *last = list->previous;
    last->next = link;
    list->previous = link;
    link->previous = last;
    link->next = list;
}

static inline MAD_FRAME *FrameOf(FrameLink *link) { return link ? (MAD_FRAME *)((char *)link - 8) : 0; }

MAD_FRAME::MAD_FRAME(unsigned int width, unsigned int height) {
    refs = 0;
    unknown00 = 1;
    shape = (MAD_SHAPE *)RCMP::rcmp_sys.allocate("MAD_FRAME::m_Shape", (width * ((height << 1) + height) >> 1) + 16, 0, 16, (int)RCMP::rcmp_sys.field8);
    MEM_fill(shape, 0, 16);
    shape->kind = 108;
    shape->width = width;
    shape->height = height;
}

MAD_FRAME::~MAD_FRAME() {
    if (shape) {
        RCMP::rcmp_sys.release(shape);
        shape = 0;
    }
}

class MAD_CODEC_INTERNAL : public RCMP::CODEC {
  public:
    MAD_CODEC_INTERNAL();
    ~MAD_CODEC_INTERNAL();
    int Init(RCMP::DECODER *, RCMP::CHUNK *);
    RCMP::FRAME *GetFrame(unsigned int);
    unsigned int GetCurrentFrameNumber();
    float GetFrameRate();
    void ReleaseFrame(RCMP::FRAME *);
    void GetNextChunk(RCMP::CHUNK **);
    void ReleaseChunk(RCMP::CHUNK *);
    RCMP::FRAME *DecodeChunk(RCMP::CHUNK *);
    MAD_FRAME *CreateKorM();
    MAD_FRAME *CreateE();
    float frameRate;      // +0x04
    int rate;             // +0x08
    int firstChunk;       // +0x0c
    int width;            // +0x10
    int height;           // +0x14
    MAD_FRAME *held;      // +0x18
    unsigned int frame;   // +0x1c
    FrameList used;       // +0x20
    FrameList free;       // +0x28
    RCMP::DECODER *decoder; // +0x30
};

MAD_CODEC_INTERNAL::MAD_CODEC_INTERNAL() {
    firstChunk = 1;
}

MAD_CODEC_INTERNAL::~MAD_CODEC_INTERNAL() {
    while (used.next != &used) {
        FrameLink *link = used.next;
        link->Remove();
        delete FrameOf(link);
    }
    while (free.next != &free) {
        FrameLink *link = free.next;
        link->Remove();
        delete FrameOf(link);
    }
}

int MAD_CODEC_INTERNAL::Init(RCMP::DECODER *newDecoder, RCMP::CHUNK *) {
    frame = 0;
    decoder = newDecoder;
    held = 0;
    return 2;
}

unsigned int MAD_CODEC_INTERNAL::GetCurrentFrameNumber() { return frame; }

float MAD_CODEC_INTERNAL::GetFrameRate() { return frameRate; }

void MAD_CODEC_INTERNAL::GetNextChunk(RCMP::CHUNK **chunk) {
    *chunk = decoder->GetChunk();
    frame++;
}

void MAD_CODEC_INTERNAL::ReleaseChunk(RCMP::CHUNK *chunk) {
    if (chunk)
        decoder->ReleaseChunk(chunk);
}

RCMP::FRAME *MAD_CODEC_INTERNAL::DecodeChunk(RCMP::CHUNK *chunk) {
    MAD_FRAME *result = 0;
    unsigned char *header = (unsigned char *)chunk->next;
    int i;
    unsigned char *lastY = 0;
    unsigned char *lastU = 0;
    unsigned char *lastV = 0;
    if (held) {
        lastY = ShapePixels(held->shape);
        int chromaEnd = 5 * height;
        lastU = lastY + width * height;
        lastV = lastY + width * chromaEnd / 4;
    }
    if (firstChunk) {
        rate = ReadLong(header + 12);
        frameRate = 1000.0f / ((float)rate * 1.52590219e-05f);
        width = header[17] << 8 | header[16];
        height = header[19] << 8 | header[18];
        int count = decoder->idata.value + 1;
        for (i = 0; i < count; i++) {
            unsigned int w = width;
            unsigned int h = height;
            MAD_FRAME *created = new MAD_FRAME(w, h);
            FrameLink *link = created ? &created->link : 0;
            FrameLink *first = free.next;
            free.next = link;
            first->previous = link;
            link->next = first;
            link->previous = &free;
        }
        firstChunk = 0;
    }
    if (GetChunkType(chunk) == 0x4d41446b) {
        if (held)
            ReleaseFrame((RCMP::FRAME *)held);
        held = 0;
        result = CreateKorM();
        MAD_initdecode((const unsigned short *)(header + 24), 0, (signed char)header[21]);
    } else if (GetChunkType(chunk) == 0x4d41446d) {
        result = CreateKorM();
        MAD_initdecode((const unsigned short *)(header + 24), 1, (signed char)header[21]);
    } else if (GetChunkType(chunk) == 0x4d414465) {
        result = CreateE();
        MAD_initdecode((const unsigned short *)(header + 24), 1, (signed char)header[21]);
    }
    unsigned char *newY = ShapePixels(result->shape);
    int chromaEnd = 5 * height;
    unsigned char *newU = newY + width * height;
    unsigned char *newV = newY + width * chromaEnd / 4;
    for (i = 0; i < height; i += 16) {
        for (int x = 0; x < width; x += 16) {
            int row = i * width;
            int luma = row + x;
            int chroma = row / 4 + x / 2;
            MAD_decodemacroblock(lastY + luma, lastU + chroma, lastV + chroma, newY + luma, newU + chroma, newV + chroma, width);
        }
    }
    if (GetChunkType(chunk) == 0x4d41446d) {
        if (held)
            ReleaseFrame((RCMP::FRAME *)held);
        held = result;
    } else if (GetChunkType(chunk) == 0x4d41446b) {
        held = result;
    }
    return (RCMP::FRAME *)result;
}

RCMP::FRAME *MAD_CODEC_INTERNAL::GetFrame(unsigned int wanted) {
    RCMP::CHUNK *chunk;
    GetNextChunk(&chunk);
    if (!firstChunk) {
        int skip = wanted - frame;
        if (skip > 2) {
            do {
                ReleaseChunk(chunk);
                GetNextChunk(&chunk);
            } while (GetChunkType(chunk) != 0x4d41446b);
        } else if (skip > 0) {
            for (int i = 0; i < skip; i++) {
                if (GetChunkType(chunk) == 0x4d414465) {
                    ReleaseChunk(chunk);
                    GetNextChunk(&chunk);
                }
            }
        }
    }
    RCMP::CHUNK *current = chunk;
    if (current) {
        RCMP::FRAME *result = DecodeChunk(current);
        ReleaseChunk(chunk);
        return result;
    }
    if (held) {
        ReleaseFrame((RCMP::FRAME *)held);
        held = (MAD_FRAME *)current;
    }
    return 0;
}

MAD_FRAME *MAD_CODEC_INTERNAL::CreateKorM() {
    if (free.next != &free) {
        FrameLink *link = free.next;
        link->Remove();
        MAD_FRAME *created = FrameOf(link);
        AddTail(&used, created ? &created->link : 0);
        created->refs = 2;
        return created;
    }
    REAL_abortmessage("MAD_CODEC_INTERNAL::CreateIorP() out of frames did you call CODEC::ReleaseFrame( FRAME *Frame )\n");
    return 0;
}

MAD_FRAME *MAD_CODEC_INTERNAL::CreateE() {
    if (free.next != &free) {
        FrameLink *link = free.next;
        link->Remove();
        MAD_FRAME *created = FrameOf(link);
        AddTail(&used, created ? &created->link : 0);
        created->refs = 1;
        return created;
    }
    REAL_abortmessage("MAD_CODEC_INTERNAL::CreateB() out of frames did you call CODEC::ReleaseFrame( FRAME *Frame )\n");
    return 0;
}

void MAD_CODEC_INTERNAL::ReleaseFrame(RCMP::FRAME *released) {
    MAD_FRAME *frame = (MAD_FRAME *)released;
    frame->refs = frame->refs - 1;
    if (frame->refs > 0)
        return;
    FrameLink *link = &frame->link;
    link->Remove();
    AddTail(&free, frame ? link : 0);
}

namespace RCMP {
CODEC *MAD_CODEC_create() { return new MAD_CODEC_INTERNAL; }
}
