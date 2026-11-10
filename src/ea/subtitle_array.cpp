// A fragment of the original subtitle code: the array of timed subtitles. Names come from the symbol table; members are
// inferred from the code.
#include "Rcmp.h"
namespace RCMP {
class AV_SUBTITLE {
  public:
    int posX;
    int posY;
    unsigned int width;
    unsigned int height;
    unsigned int strID;       // +0x10
    unsigned int frameNumber; // +0x14
    int justifyX;             // +0x18
    int justifyY;             // +0x1c
    unsigned int shapeIndex;  // +0x20
    unsigned char a, r, g, b; // +0x24
    unsigned int unknown28;
    unsigned int unknown2c;
    char *wrapped;            // +0x30
    ~AV_SUBTITLE() {}
    static void operator delete[](void *p) { rcmp_sys.release(p); }
};

class AV_SUBTITLE_ARRAY {
  public:
    AV_SUBTITLE *subtitles;
    unsigned int count; // +0x04
    AV_SUBTITLE_ARRAY();
    ~AV_SUBTITLE_ARRAY();
    AV_SUBTITLE *GetSubtitle(unsigned int);
    AV_SUBTITLE *FindSubtitle(unsigned int);
    unsigned int GetNumberOfSubtitle();
    static void operator delete(void *p) { rcmp_sys.release(p); }
};

AV_SUBTITLE_ARRAY::AV_SUBTITLE_ARRAY() {
    subtitles = 0;
    count = 0;
}

AV_SUBTITLE_ARRAY::~AV_SUBTITLE_ARRAY() {
    if (subtitles) {
        delete[] subtitles;
        subtitles = 0;
    }
    count = 0;
}

AV_SUBTITLE *AV_SUBTITLE_ARRAY::GetSubtitle(unsigned int index) {
    if (count < index)
        return 0;
    return &subtitles[index];
}
}
