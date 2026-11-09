// AI-assisted reconstruction using CC0 Frontline references and GR8E69
// evidence. Provenance and validation:
// docs/research/collision-distance-traversal.md.

#include "AnimatedVolume.h"
#include "CDBGeom.h"
#include "Capsule.h"
#include "CollisionMath.h"
#include "Sphere.h"

#ifdef DECOMP_COMPARE
EClsnState CCDBObject::TestCDBBranchNodeCollision(
    const CDBBranchNode &root, const CAnimatedVolume &volume,
    CCollision &collision, bool, dwi::fast_vec<CDBTriGroup *> &groups) const {
  const CDBBranchNode *storage[20];
  dwi::fast_vec<const CDBBranchNode *> stack(storage, 20, 11);
  int result = 1;
  stack.push_back(&root);
  while (stack.count != 0) {
    const CDBBranchNode *node = stack.back();
    stack.pop_back();
    CVector3 normal(node->normal.x, node->normal.y, node->normal.z);
    float d = CollisionDot(normal, volume.center) - node->distance;
    float side = d - volume.radius;
    if (!(side > 0.0f)) {
      side = d + volume.radius;
      if (!(side <= 0.0f))
        side = 0.0f;
    }

    if (node->front && !(side < 0.0f)) {
      if (node->front->type == 1)
        stack.push_back(node->front);
      else {
        const CDBLeafNode &leaf = *(const CDBLeafNode *)node->front;
        CCDBVolBox box(leaf.box);
        int r = box.TestCollision(volume, collision, false);
        if (r != 1) {
          for (int i = 0; i < leaf.groupCount; ++i) {
            CDBTriGroup *group = leaf.groups[i];
            if (group->mark > 0) {
              group->mark = -group->mark;
              groups.push_back(group);
            }
          }
        }
        if (r > result)
          result = r;
      }
    }
    if (node->back && !(side > 0.0f)) {
      if (node->back->type == 1)
        stack.push_back(node->back);
      else {
        const CDBLeafNode &leaf = *(const CDBLeafNode *)node->back;
        CCDBVolBox box(leaf.box);
        int r = box.TestCollision(volume, collision, false);
        if (r != 1) {
          for (int i = 0; i < leaf.groupCount; ++i) {
            CDBTriGroup *group = leaf.groups[i];
            if (group->mark > 0) {
              group->mark = -group->mark;
              groups.push_back(group);
            }
          }
        }
        if (r > result)
          result = r;
      }
    }
  }
  return (EClsnState)result;
}

#endif
