// Scoped observer/target operation models; see docs/GameAlgorithms.md.
// ObserverLinkBase, ObserverLink, ObserverHead and TargetVectorStorage are
// descriptive reconstruction types, not recovered original class names.
#ifndef MOH_TARGET_CONTAINER_TYPES_H
#define MOH_TARGET_CONTAINER_TYPES_H
#include "../game/ObserverTypes.h"
class ISceneNode;
template <class T, int N> class WeakPtr : public IObserver {
  public:
    WeakPtr() {}
    WeakPtr(const WeakPtr &other) { Assign(other.subject); }
    WeakPtr &operator=(const WeakPtr &other) {
        Assign(other.subject);
        return *this;
    }
    // Descriptive inline accessors; script spatial callers corroborate these operations.
    operator bool() const { return subject != 0; }
    T *operator->() const { return static_cast<T *>(subject); }
    virtual void HandleEvent(ISubject *, ESubjectEvent event) {
        if (event & N)
            Assign(0);
    }
};
struct TargetVectorStorage {
    float x, y, z, w;
    TargetVectorStorage(const TargetVectorStorage &other)
        : x(other.x), y(other.y), z(other.z), w(1.0f) {}
    TargetVectorStorage operator=(const TargetVectorStorage &other) {
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }
};
struct TargetInfo {
    WeakPtr<ISceneNode, 8> field_00;
    unsigned char field_18;
    float field_1c;
    TargetVectorStorage field_20;
    bool operator<(const TargetInfo &other) const { return field_1c < other.field_1c; }
};

typedef char DestructiblePrefixCheck[sizeof(IDestructible) == 4 ? 1 : -1];
typedef char SubjectStorageCheck[sizeof(ISubject) == 8 ? 1 : -1];
typedef char ObserverStorageCheck[sizeof(IObserver) == 24 ? 1 : -1];
typedef char WeakSceneStorageCheck[sizeof(WeakPtr<ISceneNode, 8>) == 24 ? 1 : -1];
typedef char TargetVectorStorageCheck[sizeof(TargetVectorStorage) == 16 ? 1 : -1];
typedef char TargetInfoStorageCheck[sizeof(TargetInfo) == 48 ? 1 : -1];
#endif
