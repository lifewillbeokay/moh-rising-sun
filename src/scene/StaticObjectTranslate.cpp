// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original static_obj.cpp; member names are descriptive.
#include "SceneObjects.h"
extern "C" float sqrtf(float);

// A motion's header, followed by its key frames; names descriptive.
struct MotionData {
    int unknown_0;
    int rate;
    int loop;
    int hasMotion;
    MotionFrame frames[1];
};

static inline void SetSub(CVector3 &r, const CVector3 &a, const CVector3 &b) {
    r.x = a.x - b.x;
    r.y = a.y - b.y;
    r.z = a.z - b.z;
}

static inline CVector3 KeyPosition(const MotionFrame *frames, int i) { return frames[i].position; }

void CStaticObject::ModifyMotionData(MotionData *data) {
    if (data) {
        int frameCount = data->loop;
        motionLoop = frameCount;
        motionRate = (float)data->rate;
        bool hasMotion = true;
        if (!data->hasMotion)
            hasMotion = false;
        flags.hasMotion = hasMotion;
        MotionFrame *frames = data->frames;
        motionFrames = frames;
        motionTimer = 0;
        motionKey = 0;
        if (dumpAge != 0.0f && flags.hasMotion) {
            int last;
            if (frameCount <= 8)
                last = frameCount - 1;
            else
                last = 9;
            CVector3 start = KeyPosition(frames, 0);
            CVector3 end = KeyPosition(frames, last);
            CVector3 delta;
            SetSub(delta, end, start);
            float length = sqrtf(delta.LengthSquared());
            motionRate *= length * (60.0f / (motionRate * (float)(last + 1))) / dumpAge;
        }
    } else {
        motionFrameCount = 0;
        motionSpeed = 0;
        motionLoop = 0;
        motionTimer = 0;
        motionKey = 0;
        motionRate = 0.0f;
        flags.hasMotion = 0;
        motionFrames = 0;
    }
    flags.motionPlaying = 0;
}

void CStaticObject::TranslateFromScript(const CVector3 &rate, const CVector3 &distance, EBSEventEnum event) {
    scriptMoveEvent = event;
    flags.scriptMoving = 1;
    if (distance.x == 0.0f && distance.y == 0.0f && distance.z == 0.0f)
        scriptMoveTime = 3.4028235e38f;
    else
        scriptMoveTime = CalculateMovementTime(rate, distance);
    velocity = rate;
    pathPosition = transform.GetPos();
}
