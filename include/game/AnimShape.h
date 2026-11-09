// Target-derived animated-texture storage; see docs/research/broad-source-sweep.md.
#ifndef GAME_ANIM_SHAPE_H
#define GAME_ANIM_SHAPE_H
class SHAPE;
struct ShapeFile;
namespace EAGL {
class TAR;
}
class AnimShape {
  public:
    enum EState {
        StateDefault = '-',
        StateL = 'L',
        Statel = 'l',
        StateO = 'O',
        Stateo = 'o',
        StateP = 'P',
        Statep = 'p'
    };
    char name[5];
    unsigned char state, compressed, dirty;
    int crc;
    unsigned int updateFrame;
    int frameCount;
    float frameRate, frame;
    SHAPE *boundShape, *pendingShape;
    ShapeFile *shapeFile;
    struct TARListView {
        EAGL::TAR **begin, **end;
        unsigned char unknown08[4];
        EAGL::TAR **capacity;
    } tars;
    AnimShape *next;
    static AnimShape *ms_pShapes;
    static AnimShape *FindFromCRC(int);
    static void SetStateByCRC(int, EState);
    static void InitByCRC(int, EState, float);
    static void ResetByCRC(int);
    static void AdvanceByCRC(int);
    static void SetFrameByCRC(int, float);
    static void SetFrameRateByCRC(int, float);
    void AddToGlobalList();
    void RemoveFromGlobalList();
    void Decompress(int);
    void Update(float);
    void AdvanceFrames(float);
    void SetFrame(float);
    void StepToNextFrame();
    void RebindTARs();
};
#endif
