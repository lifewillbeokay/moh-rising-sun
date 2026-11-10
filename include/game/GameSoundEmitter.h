// AI-assisted view of the sound emitter that follows a scene node (GameSoundEmitter.cpp); see docs/Sound.md.
#ifndef GAME_GAME_SOUND_EMITTER_H
#define GAME_GAME_SOUND_EMITTER_H
#pragma interface
#include "SoundEvents.h"
#include "../game-containers/TargetTypes.h"
#include "SceneNode.h"
class SceneNodeSoundEmitter : public SoundEmitter {
  public:
    WeakPtr<ISceneNode, 8> node; // +4; the name is inferred.
    inline SceneNodeSoundEmitter();
    virtual ~SceneNodeSoundEmitter() {}
    virtual void Clone(SoundEmitter *) const;
    virtual float GetDistance() const;
    virtual int GetAzimuth() const;
    virtual void *GetObjRef() const { return node.Get(); }
    virtual int GetType() { return 0; }
    virtual void *GetFilterDef() const;
    void SetSceneNode(ISceneNode *);
};
#endif
