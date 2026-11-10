// A fragment of the original draw_context.cpp: the colour scale and offset that the EAGL render methods read by name.
// Names come from the symbol table.
#include "Color.h"
namespace EAGL {
class DynamicLoader {
  public:
    static const char *RegisterVar(const char *, void *);
};
}

class CNormColor {
  public:
    float r, g, b, a;
};

namespace GlobalColorModifier {
extern CNormColor defaultScale;
extern CNormColor defaultOffset;
extern CNormColor scale;
extern CNormColor offset;
void SetScale(const CNormColor &);
void SetOffset(const CNormColor &);
void SetScale(const CColor &);
void SetDefaults();

void Init() {
    SetDefaults();
    EAGL::DynamicLoader::RegisterVar("GlobalColorScale", &scale);
    EAGL::DynamicLoader::RegisterVar("GlobalColorOffset", &offset);
}

void SetScale(const CNormColor &color) { scale = color; }

void SetOffset(const CNormColor &color) {
    offset.r = color.r;
    offset.g = color.g;
    offset.b = color.b;
    offset.a = color.a;
}
}
