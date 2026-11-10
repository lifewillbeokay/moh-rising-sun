#ifndef GAME_EAGL_RENDER_CONTEXT_PRIVATE_BASE_H
#define GAME_EAGL_RENDER_CONTEXT_PRIVATE_BASE_H
// AI-assisted runtime view; see docs/Rendering.md. The class and method names come from the symbol
// table; member names are inferred from use. The destructor is the table's only virtual: it is
// inlined by the derived destructor and also has one out-of-line copy, so it is an inline body here.
#pragma interface
namespace EAGL {
class RenderContext;
class ViewPort;
}
namespace EAGLInternal {
class RenderContextPrivateBase {
  public:
    unsigned int unknown_0;
    EAGL::RenderContext *context;
    EAGL::ViewPort *currentViewPort;
    EAGL::ViewPort *firstViewPort;
    inline RenderContextPrivateBase(EAGL::RenderContext *);
    virtual ~RenderContextPrivateBase() {}
    void SetCurrentViewPort(EAGL::ViewPort *);
};
}
#endif
