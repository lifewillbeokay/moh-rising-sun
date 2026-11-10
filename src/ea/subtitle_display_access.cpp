// A fragment of the original subtitle display. Names come from the symbol table; members are inferred from the code. The file shows unreduced loops, so it builds with -fno-strength-reduce.
#include "Rcmp.h"
#include "TARExtension.h"
struct FONT;
namespace RCMP {
class AV_SUBTITLE {
  public:
    unsigned char unknown00[0x30];
    char *wrapped;
};
class AV_SUBTITLE_ARRAY {
  public:
    AV_SUBTITLE *GetSubtitle(unsigned int);
    unsigned int GetNumberOfSubtitle();
};
struct ShapeTable {
    unsigned char unknown00[8];
    int count; // +0x08
    int unknown0c;
    struct Entry {
        int unknown00;
        int offset;
    } entries[1]; // +0x10; each shape's offset from the table.
};
class VD_SUBTITLE {
  public:
    VD_SUBTITLE();
    ~VD_SUBTITLE();
    void DeleteTars();
    void CreateTars();
    void DeleteWrappedStrings();
    void CreateWrappedStrings();
    void Init(void *, bool, FONT *, void *, AV_SUBTITLE_ARRAY *);
    void GetPos(float *, float *);
    void SetScale(float, float);
    void GetScale(float *, float *);
    float x, y;
    float scaleX, scaleY;     // +0x08
    void *data;               // +0x10
    FONT *font;               // +0x14
    ShapeTable *shapes;       // +0x18
    AV_SUBTITLE_ARRAY *array; // +0x1c
    EAGL::TAR **tars;         // +0x20
    unsigned int tarCount;    // +0x24
    bool active;              // +0x28
};

void VD_SUBTITLE::GetPos(float *outX, float *outY) {
    *outX = x;
    *outY = y;
}

void VD_SUBTITLE::SetScale(float newX, float newY) {
    scaleX = newX;
    scaleY = newY;
}

void VD_SUBTITLE::GetScale(float *outX, float *outY) {
    *outX = scaleX;
    *outY = scaleY;
}
}
