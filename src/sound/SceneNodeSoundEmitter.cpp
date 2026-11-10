// AI-assisted reconstruction of the scene-node sound emitter; see docs/Sound.md.
#include "GameSoundEmitter.h"
#include "PlacementNew.h"
#include "SceneNode.h"
#include "CameraShake.h"
extern "C" float sqrtf(float);
extern int g_bInMultiplayerMode;
int CalculateAzimuth(const CVector3 &, const CPlayerObject *);
// A player is a scene node; the shared CPlayerObject view declares no base class.
static inline const ISceneNode *PlayerNode(const CPlayerObject *player) { return (const ISceneNode *)player; }

inline SceneNodeSoundEmitter::SceneNodeSoundEmitter() {}
void SceneNodeSoundEmitter::SetSceneNode(ISceneNode *sceneNode) {
    node.Assign(sceneNode);
}
void SceneNodeSoundEmitter::Clone(SoundEmitter *out) const {
    if (out) {
        new (out) SceneNodeSoundEmitter;
        ((SceneNodeSoundEmitter *)out)->SetSceneNode(node.Get());
    }
}
float SceneNodeSoundEmitter::GetDistance() const {
    float distance = 3.4028235e38f;
    if (node) {
        CVector3 position;
        CVector3 listener;
        node->GetPosition(position);
        if (g_bInMultiplayerMode)
            PlayerNode(g_scene.FindClosestPlayer(position))->GetPosition(listener);
        else
            PlayerNode(g_scene.GetPlayer(0))->GetPosition(listener);
        position -= listener;
        distance = sqrtf(position.LengthSquared());
    }
    return distance;
}
int SceneNodeSoundEmitter::GetAzimuth() const {
    int azimuth = 0;
    if (node) {
        CVector3 position;
        node->GetPosition(position);
        if (g_bInMultiplayerMode)
            azimuth = CalculateAzimuth(position, g_scene.FindClosestPlayer(position));
        else
            azimuth = CalculateAzimuth(position, g_scene.GetPlayer(0));
    }
    return azimuth;
}
void *SceneNodeSoundEmitter::GetFilterDef() const {
    if (node && node->AsSoldierObject())
        return node->AsSoldierObject()->GetLipSyncFilterDef();
    return 0;
}
float PositionSoundEmitter::GetDistance() const {
    CVector3 listener;
    CVector3 position(x, y, z);
    if (g_bInMultiplayerMode)
        PlayerNode(g_scene.FindClosestPlayer(position))->GetPosition(listener);
    else
        PlayerNode(g_scene.GetPlayer(0))->GetPosition(listener);
    listener -= position;
    return sqrtf(listener.LengthSquared());
}
int PositionSoundEmitter::GetAzimuth() const {
    CVector3 position(x, y, z);
    int azimuth;
    if (g_bInMultiplayerMode)
        azimuth = CalculateAzimuth(position, g_scene.FindClosestPlayer(position));
    else
        azimuth = CalculateAzimuth(position, g_scene.GetPlayer(0));
    return azimuth;
}
