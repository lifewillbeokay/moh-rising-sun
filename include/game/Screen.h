#ifndef SCREEN_H
#define SCREEN_H
// A scoped view of CScreen. Method names come from the original symbols; the compiler
// places the table pointer at +100, after these members.
class CColor;

class CDrawContext;
class CScreen {
public:
    void Init();
    int GetVSyncs();
    int GetDepthBufferMax() const;
    void Clear();
    // Table order, from the original table; both pure slots are unnamed.
    virtual ~CScreen();
    virtual void SetCurrent();
    virtual void PureSlot2() = 0;
    virtual int Flip();
    virtual void Wait();
    virtual void SetClear(bool clear);
    virtual void SetClearColor(const CColor &color);
    virtual int GetClear() const;
    virtual const CColor &GetClearColor() const;
    virtual int GetTMViewToScreen() const;
    virtual CDrawContext *GetOrthographicDrawContext() const = 0;

    int clear;
    unsigned int clearColor;
    unsigned char unknown08[0x18 - 0x08];
    int depthFormat;
    unsigned char unknown1c[0x64 - 0x1c];

    static CScreen *m_spScreen;
    static unsigned int m_sWidth;
    static int m_sHeight;
    static int m_sVideoMode;
    static float m_sWidthScale;
    static float m_sHeightScale;
    static float m_2dXScale;
    static float m_2dYScale;
    static float m_2dXDisp;
    static float m_2dYDisp;
    static volatile int m_sFrameNum;
};
#endif
