// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic reference.
// Part of the original player.cpp. These functions use more of the object than the view in
// CameraShake.h declares; the record array and its count are modeled here. Names are descriptive.
#include "CameraShake.h"
#include "../game-containers/TargetTypes.h"

struct TargetFindStorage : public WeakPtr<CPlayerObject, 8> {
    WeakPtr<ISceneNode, 8> chosen; // +0x18
    TargetInfo targets[8]; // +0x30
    int count;             // +0x1b0
    unsigned char unknown_1b4[0x1d0 - 0x1b4];
    int enabled;           // +0x1d0
    unsigned char changed; // +0x1d4; cleared when nothing is chosen. Descriptive.
    unsigned char unknown_1d5[0x1df - 0x1d5];
    unsigned char hudState; // +0x1df; 2 once the HUD has the chosen target's name. Descriptive.
};

void CPlayerObject::TargetFind::Enable(CPlayerObject *player) {
    TargetFindStorage *self = (TargetFindStorage *)this;
    self->enabled = 1;
    self->Assign((ISubject *)player);
}

int CPlayerObject::TargetFind::AddTarget(ISceneNode *node, CVector3 &position, float distance, unsigned char flag) {
    TargetFindStorage *self = (TargetFindStorage *)this;
    if (self->count > 7)
        return 0;
    int offset = self->count * sizeof(TargetInfo);
    ((TargetInfo *)((char *)self + offset + 0x30))->field_1c = distance;
    (&self->targets[self->count])->field_00.Assign((ISubject *)node);
    self->targets[self->count].field_18 = flag;
    *(CVector3 *)&(&self->targets[self->count])->field_20 = position;
    self->count++;
    return 1;
}

void CPlayerObject::TargetFind::ChooseTarget() {
    TargetFindStorage *self = (TargetFindStorage *)this;
    if (!(self->targets[0].field_1c >= 9999.0f))
        self->chosen.Assign(self->targets[0].field_00.GetSubject());
    else
        self->chosen.Assign(0);
    if (self->chosen) {
        IMovingSceneNode *object = ((ISceneNode *)self->chosen.GetSubject())->AsMovingNode();
        if (object) {
            if (*self) {
                self->hudState = 2;
                ((CPlayerObject *)self->GetSubject())->hud.SetTarget(object->GetName());
            }
        }
    } else {
        self->changed = 0;
        self->hudState = 0;
    }
}

void CPlayerObject::TargetFind::ClearTargets() {
    TargetFindStorage *self = (TargetFindStorage *)this;
    for (int i = 0; i < self->count; i++) {
        self->targets[i].field_00.Assign(0);
        self->targets[i].field_1c = 9999.0f;
    }
    self->count = 0;
}
