// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original hierobject.cpp; member names are descriptive.
#include "SceneObjects.h"
#include "ScriptTimers.h"

// Method-only views. The animated object carries a second table pointer at +0x38, and the
// skeleton accessor is called through its slot at +0x10; the holder's other slot is a
// placeholder for that position only. The symbol establishes FindAttachPoint's nesting.
namespace EALA {
namespace Character {
class Skeleton {
  public:
    const CMatrix *FindAttachPoint(const char *);
};
}
}
class CSkeletonHolder {
  public:
    virtual void Slot8();
    virtual EALA::Character::Skeleton *GetSkeleton();
};

void CHierAnimObject::AttemptUpdate(float dt) {
    AttemptUpdateFromMotion(dt);
    ((IMovingSceneNode *)animObject)->SetTMLocalToWorld(transform);
    ((ISceneNode *)animObject)->AttemptUpdate(dt);
    CMatrix attach;
    attach = *((CSkeletonHolder *)((char *)animObject + 0x38))->GetSkeleton()->FindAttachPoint("JO_gun");
    for (CHierObject *child = firstChild; child; child = child->nextSibling) {
        child->SetAttachPointMatrix(attach);
        child->SetInheritedMatrix(transform);
        ((ISceneNode *)child)->AttemptUpdate(dt);
    }
    AttemptUpdateOfAttachedObjects();
}

void CHierAnimObject::CommitUpdate() {
    ((ISceneNode *)animObject)->CommitUpdate();
    CHierObject::CommitUpdate();
}

void CHierAnimObject::OnCollision(const CCollision &collision) {
    ((ISceneNode *)animObject)->OnCollision(collision);
    CHierObject::OnCollision(collision);
}

CVector3 CHierAnimObject::GetWorldLinearVelocity() const {
    CVector3 own = CHierObject::GetWorldLinearVelocity();
    CVector3 object = ((ISceneNode *)animObject)->GetWorldLinearVelocity();
    own += object;
    return own;
}

namespace EALA {
class Task {
  public:
    virtual void Execute() = 0;
};
}

// The task an animation runs when it ends: event -1 sends nothing. Member names are inferred.
class CHierAnimObject::StateDone : public EALA::Task {
  public:
    StateDone();
    virtual void Execute();
    int event;
    ISceneNode *owner;
};

CHierAnimObject::StateDone::StateDone() : event(-1), owner(0) {}

void CHierAnimObject::StateDone::Execute() {
    if (event >= 0)
        BSObjectTriggerEvent((BSObject *)owner->GetScriptObject(), event, 0, 0, true);
}

int CHierAnimObject::SetAnimationState(const char *name, int event) {
    int result;
    if (name) {
        ((StateDone *)doneState)->event = event;
        ((StateDone *)doneState)->owner = (ISceneNode *)root;
        ((CAnimObjectInterface *)animObject)->SetAnimationState(name, (StateDone *)doneState);
        result = 0;
    } else {
        result = 1;
    }
    return result;
}

int CHierAnimObject::FindAttachNode(int, CMatrix *matrix) {
    *matrix = *((CSkeletonHolder *)((char *)animObject + 0x38))->GetSkeleton()->FindAttachPoint("JO_gun");
    return 1;
}

int CHierAnimObject::AttachAnimatedObject() {
    if (!animObject)
        return 0;
    g_scene.ReOrder((ISceneNode *)this, (ISceneNode *)animObject);
    localTransform.GetSlot(0);
    animObject->SetAttached(this != 0);
    g_scene.ExcludeCollision(*(ISceneNode *)this, *(ISceneNode *)animObject, -1);
    return 1;
}

CAnimObject *CHierAnimObject::AsAnimObject() {
    return animObject;
}

const CAnimObject *CHierAnimObject::AsAnimObject() const {
    return animObject;
}
