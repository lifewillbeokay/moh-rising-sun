// A fragment of the original device_cmn.cpp: the device's list of render contexts and its overrides.
// Names come from the symbol table; instance member names are inferred from this code.
namespace EAGLInternal {
extern void *(*EAGLMalloc)(unsigned int, const char *);
extern void (*EAGLFree)(void *, unsigned int);
class DevicePrivate;
}

namespace EAGL {
class Device;
class RenderContextBase {
  public:
    struct Link {
        RenderContextBase *next;
    };
    unsigned char unknown00[4];
    int skipped;  // +0x04; passed over by DeleteRenderContext's search when set.
    Link *link;   // +0x08
    // Table pointer at +0x0c; the destructor's slot shows three virtuals before it.
    virtual void Unknown0();
    virtual void Unknown1();
    virtual void Unknown2();
    virtual ~RenderContextBase();
};
class RenderContext : public RenderContextBase {
  public:
    RenderContext(Device *);
    static void *operator new(unsigned int n) { return EAGLInternal::EAGLMalloc(n, 0); }
    unsigned char unknown10[240 - 16];
};
}

namespace EAGLInternal {
class DevicePrivate {
  public:
    void SetCurrentRenderContext(EAGL::RenderContext *);
    static EAGL::Device *gpCurrentDevice;
    static EAGL::RenderContext *gpCurrentRenderContext;
    static EAGL::RenderContextBase *gpCurrentContext;
    int unknown00;
};
}

namespace EAGL {
class Device {
  public:
    RenderContext *NewRenderContext();
    void DeleteRenderContext(RenderContext *);
    void AddRenderContext(RenderContextBase *);
    void RemoveRenderContext(RenderContextBase *);
    RenderContext *GetCurrentRenderContext() const;
    RenderContextBase *GetCurrentContext() const;
    static void SetNewOverride(void *(*)(unsigned int, const char *));
    static void SetDeleteOverride(void (*)(void *, unsigned int));
    static Device *Get();
    unsigned char unknown00[4];
    EAGLInternal::DevicePrivate devicePrivate; // +0x04
    unsigned char unknown08[4];
    RenderContextBase *contexts;               // +0x0c
};

RenderContext *Device::NewRenderContext() {
    RenderContext *context = new RenderContext(this);
    AddRenderContext(context);
    devicePrivate.SetCurrentRenderContext(context);
    return context;
}

void Device::DeleteRenderContext(RenderContext *context) {
    RemoveRenderContext(context);
    if (EAGLInternal::DevicePrivate::gpCurrentRenderContext == context) {
        RenderContextBase *next = contexts;
        while (next && next->skipped)
            next = next->link->next;
        EAGLInternal::DevicePrivate::gpCurrentRenderContext = (RenderContext *)next;
    }
}

void Device::AddRenderContext(RenderContextBase *context) {
    context->link->next = contexts;
    contexts = context;
}

void Device::RemoveRenderContext(RenderContextBase *context) {
    RenderContextBase *previous = contexts;
    if (context == previous) {
        contexts = context->link->next;
        if (context)
            delete context;
    } else {
        while (previous && previous->link->next != context)
            previous = previous->link->next;
        RenderContextBase::Link *link = previous->link;
        RenderContextBase *removed = link->next;
        link->next = removed->link->next;
        if (removed)
            delete removed;
    }
    if (EAGLInternal::DevicePrivate::gpCurrentContext == context)
        EAGLInternal::DevicePrivate::gpCurrentContext = 0;
}

RenderContext *Device::GetCurrentRenderContext() const { return EAGLInternal::DevicePrivate::gpCurrentRenderContext; }

RenderContextBase *Device::GetCurrentContext() const { return EAGLInternal::DevicePrivate::gpCurrentContext; }
}

void EAGLInternal::DevicePrivate::SetCurrentRenderContext(EAGL::RenderContext *context) {
    gpCurrentRenderContext = context;
    gpCurrentContext = context;
}

namespace EAGL {
void Device::SetNewOverride(void *(*function)(unsigned int, const char *)) { EAGLInternal::EAGLMalloc = function; }

void Device::SetDeleteOverride(void (*function)(void *, unsigned int)) { EAGLInternal::EAGLFree = function; }

Device *Device::Get() { return EAGLInternal::DevicePrivate::gpCurrentDevice; }
}
