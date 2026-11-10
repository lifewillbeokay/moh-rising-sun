#ifndef GAME_EAGL_MATERIAL_H
#define GAME_EAGL_MATERIAL_H
#include "EAGLTransform.h"
#include "EAGLGeoPrimState.h"
#include "TARExtension.h"
#include "TevStage.h"
#include "EAGLRenderContext.h"
#include <dolphin/gx/GXGeometry.h>
#include <dolphin/gx/GXLighting.h>
#include <dolphin/gx/GXTransform.h>
#include <dolphin/gx/GXDispList.h>
#include <dolphin/os/OSCache.h>
namespace EAGL {
class GeoPrim;
class Device {
public:
    unsigned int unknown_0;
    EAGLInternal::DevicePrivate privateData; // +4
    static Device* Get();
    RenderContext* GetCurrentRenderContext() const;
};

class RenderMethod {
public:
    static void IncrementArrays(int);
    static void SetArray(GXAttr,void*,unsigned char);
};
}
static inline GXColor WhiteColour() {
    GXColor c={255};
    c.g=255;
    c.b=255;
    c.a=255;
    return c;
}
namespace EAGLInternal {
struct ParameterSlot {
    unsigned int unknown;
    void* value;
};
// Descriptive storage fields. Twenty Render callers reserve 192 bytes for this
// record; ProcessPCode initializes the remaining execution state. Unknown words
// retain explicit byte storage. Do not infer the original constructor or names.
struct PCodeData {
    ParameterSlot* parameters;
    unsigned char* variationCode;
    unsigned int* identifier;
    int block;
    void** volatileData;
    void** lastVolatileSource;
    int numVolatiles;
    int* noUploadsConstantSize;
    int* noUploadsVariableSize;
    unsigned char* defaultPCode;
    void* displayList;
    unsigned int displayListSize;
    bool textureDirty[8];
    unsigned char unknown_50[0x28];
    int positionIndex8;
    int normalIndex8;
    unsigned char unknown_80[8];
    int colourIndex8;
    unsigned char unknown_8c[4];
    int textureIndex8[8];
    bool resetState;
    bool formatDirty;
    bool fastDisplayList;
    bool drawArrays;
};
// Original per-material static objects are 116 bytes. Unused fields stay unknown.
struct StaticData {
    int attributesPerVertex;
    EAGL::GeoPrimState* state;
    unsigned int vertexFormat;
    unsigned char unknown_0c[0x28];
    int positionIndex8;
    int normalIndex8;
    unsigned char unknown_3c[8];
    int colourIndex8;
    unsigned char unknown_48[4];
    int textureIndex8[8];
    unsigned int preserveVertexFormat;
    unsigned int vertexColours;
};
}
namespace EAGLInternal {
extern int CurrentVariation;
void ProcessPCode(PCodeData&,EAGL::GeoPrim*,int);
}
extern unsigned char* gpDisplayList;
extern int gDisplayListSize;
static inline EAGL::RenderContextExtensionBase* ContextExtension(EAGL::RenderContext* context) {
    return (EAGL::RenderContextExtensionBase*)((unsigned char*)context+0x10);
}
// GX hardware FIFO aliases for its byte, halfword and word ports.
union FifoPort { unsigned char byte; unsigned short half; unsigned int word; };
// Native SN hardware-address declaration; allocates no executable RAM.
volatile FifoPort GXWGFifo __attribute__((address(0xcc008000)));
#define FIFO8 GXWGFifo.byte
#define FIFO16 GXWGFifo.half
#define FIFO32 GXWGFifo.word
static inline void CallList(const void* list, unsigned int bytes, bool fast) {
    if (fast) {FIFO8=0x40;FIFO32=(unsigned int)list;FIFO32=bytes;}
    else GXCallDisplayList((void*)list,bytes);
}
#endif
