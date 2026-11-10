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

VD_SUBTITLE::VD_SUBTITLE()
    : x(0.0f), y(0.0f), scaleX(1.0f), scaleY(1.0f), data(0), font(0), shapes(0), array(0), tars(0), tarCount(0), active(0) {}

VD_SUBTITLE::~VD_SUBTITLE() {
    DeleteTars();
    DeleteWrappedStrings();
}

void VD_SUBTITLE::DeleteTars() {
    if (tars) {
        for (unsigned int i = 0; i < tarCount; i++) {
            if (tars[i])
                delete tars[i];
        }
        if (tars)
            delete[] tars;
        tarCount = 0;
        tars = 0;
    }
}

void VD_SUBTITLE::CreateTars() {
    if (shapes) {
        int count = shapes->count;
        tarCount = count;
        tars = new EAGL::TAR *[count];
        for (int i = 0; i < count; i++) {
            ShapeTable *table = shapes;
            const SHAPE *shape = (const SHAPE *)((char *)table + (&table->entries[i])->offset);
            tars[i] = new (EAGLInternal::EAGLMalloc(sizeof(EAGL::TAR), 0)) EAGL::TAR(shape);
        }
    }
}

void VD_SUBTITLE::DeleteWrappedStrings() {
    if (array) {
        for (unsigned int i = 0; i < array->GetNumberOfSubtitle(); i++) {
            AV_SUBTITLE *subtitle = array->GetSubtitle(i);
            if (subtitle->wrapped) {
                rcmp_sys.release(subtitle->wrapped);
                subtitle->wrapped = 0;
            }
        }
    }
}
}
