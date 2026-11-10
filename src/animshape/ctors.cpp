// A fragment of the original anim_texture.cpp: animated texture construction, the name's parsed settings
// and copying. Class, method and static names come from the symbol table; member names are inferred from
// this code. Only what these functions use is declared.
#include "profile.h"
#include <map>
#include <vector>
#include <string.h>

extern "C" char *SHAPE_name(void *shape, int index, char *name);
extern "C" int SHAPE_depth(void *shape);
extern "C" int SHAPE_size(void *shape);
int HexStrToInt(const char *);

struct ShapeFile {
    unsigned char unknown00[8];
    int count;
    unsigned char unknown0c[8];
    int firstOffset;
};

struct SHAPE;
namespace EAGL {
class TAR {
public:
    void SwapShape(const SHAPE *);
};
}

class AnimShape {
public:
    enum EState { StateDefault = '-' };
    struct InitInfo {
        int state;
        float frameRate;
    };
    static AnimShape *FindFromCRC(int);
    static void InitByCRC(int, EState, float);
    void RebindTARs();
    void UnRegisterTAR(EAGL::TAR &);
    AnimShape(ShapeFile &);
    AnimShape(const AnimShape &);
    ~AnimShape();
    AnimShape &operator=(const AnimShape &);
    void Init(ShapeFile &);
    void AddToGlobalList();
    void RemoveFromGlobalList();

    char name[5];
    unsigned char state, compressed, dirty;
    int crc;
    int updateFrame;
    int frameCount;
    float frameRate, frame;
    SHAPE *boundShape, *pendingShape;
    ShapeFile *shapeFile;
    _STL::vector<EAGL::TAR *> tars;
    static _STL::map<int, InitInfo> ms_initMap;
};

AnimShape::AnimShape(ShapeFile &file) {
    Init(file);
    AddToGlobalList();
}

AnimShape::AnimShape(const AnimShape &other) {
    *this = other;
    AddToGlobalList();
}

