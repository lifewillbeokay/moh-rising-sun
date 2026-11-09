// AI-assisted reconstruction using CC0 Frontline references and GR8E69
// evidence. Provenance and validation:
// docs/research/collision-distance-traversal.md.

#include "CDBGeom.h"
#include "CLine3.h"
#include "CollisionMath.h"

#ifdef DECOMP_COMPARE
extern EClsnState TestCollisionWithTriGroup(const CDBTriGroup &, const CLine3 &,
                                            CCollision &, bool);
// Descriptive local traversal frame: the target uses a 48-byte stride.
struct CollisionLineFrame {
  CVector3 start, end;
  const CDBBranchNode *node;
} __attribute__((aligned(16)));
EClsnState CCDBObject::TestCollision(const CLine3 &line, CCollision &collision,
                                     bool flag) const {
  CDB *db = database;
  if (!db)
    return (EClsnState)1;
  CDBTriGroup *groupStorage[2048];
  dwi::fast_vec<CDBTriGroup *> groups(groupStorage, 2048);
  int result = 1;
  if (db->branchCount > 0) {
    CollisionLineFrame stack[20];
    stack[0].node = db->branchRoot;
    stack[0].start = line.start;
    stack[0].end = line.end;
    int count = 1;
    while (count != 0) {
      const CollisionLineFrame &frame = stack[--count];
      const CDBBranchNode *node = frame.node;
      CVector3 start(frame.start), end(frame.end);
      CVector3 normal(node->normal.x, node->normal.y, node->normal.z);
      float d0 = CollisionDot(normal, start) - node->distance;
      float d1 = CollisionDot(normal, end) - node->distance;
      const CDBBranchNode *first = 0, *second = 0;
      CVector3 frontStart, frontEnd, backStart, backEnd;
      if (!(d0 < 0.0f)) {
        first = node->front;
        if (!(d1 >= 0.0f)) {
          second = node->back;
          float t = d0 / (d0 - d1);
          CVector3 mid;
          CollisionSub(mid, end, start);
          mid *= t;
          mid += start;
          frontStart = start;
          frontEnd = mid;
          backStart = mid;
          backEnd = end;
        } else {
          frontStart = start;
          frontEnd = end;
        }
      } else {
        first = node->back;
        if (!(d1 < 0.0f)) {
          second = node->front;
          float t = -d0 / (d1 - d0);
          CVector3 mid;
          CollisionSub(mid, end, start);
          mid *= t;
          mid += start;
          backStart = start;
          backEnd = mid;
          frontStart = mid;
          frontEnd = end;
        } else {
          backStart = start;
          backEnd = end;
        }
      }
      if (first) {
        if (first->type == 1) {
          if (first == node->front) {
            stack[count].start = frontStart;
            stack[count].end = frontEnd;
          } else {
            stack[count].start = backStart;
            stack[count].end = backEnd;
          }
          stack[count++].node = first;
        } else {
          const CDBLeafNode &leaf = *(const CDBLeafNode *)first;
          CCDBVolBox box(leaf.box);
          int leafResult = 1;
          if (box.TestCollision(line, collision, false) != 1) {
            for (int i = 0; i < leaf.groupCount; ++i) {
              CDBTriGroup *group = leaf.groups[i];
              if (group->mark > 0) {
                group->mark = -group->mark;
                groups.push_back(group);
                int r =
                    TestCollisionWithTriGroup(*group, line, collision, flag);
                if (r > leafResult)
                  leafResult = r;
              }
            }
          }
          if (leafResult > result)
            result = leafResult;
        }
      }
      if (second) {
        if (second->type == 1) {
          if (second == node->front) {
            stack[count].start = frontStart;
            stack[count].end = frontEnd;
          } else {
            stack[count].start = backStart;
            stack[count].end = backEnd;
          }
          stack[count++].node = second;
        } else {
          const CDBLeafNode &leaf = *(const CDBLeafNode *)second;
          CCDBVolBox box(leaf.box);
          int leafResult = 1;
          if (box.TestCollision(line, collision, false) != 1) {
            for (int i = 0; i < leaf.groupCount; ++i) {
              CDBTriGroup *group = leaf.groups[i];
              if (group->mark > 0) {
                group->mark = -group->mark;
                groups.push_back(group);
                int r =
                    TestCollisionWithTriGroup(*group, line, collision, flag);
                if (r > leafResult)
                  leafResult = r;
              }
            }
          }
          if (leafResult > result)
            result = leafResult;
        }
      }
    }
  } else {
    const CDBLeafNode &leaf = *db->leafRoot;
    CCDBVolBox box(leaf.box);
    int leafResult = 1;
    if (box.TestCollision(line, collision, false) != 1) {
      for (int i = 0; i < leaf.groupCount; ++i) {
        CDBTriGroup *group = leaf.groups[i];
        if (group->mark > 0) {
          group->mark = -group->mark;
          groups.push_back(group);
          int r = TestCollisionWithTriGroup(*group, line, collision, flag);
          if (r > leafResult)
            leafResult = r;
        }
      }
    }
    if (leafResult > result)
      result = leafResult;
  }
  CDBTriGroup **end = groups.end();
  for (CDBTriGroup **p = groups.begin(); p != end; ++p)
    (*p)->mark = -(*p)->mark;
  return (EClsnState)result;
}

#endif
