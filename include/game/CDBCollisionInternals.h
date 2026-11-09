#ifndef GAME_CDBCOLLISIONINTERNALS_H
#define GAME_CDBCOLLISIONINTERNALS_H
// Names and types from cdbgeom.cpp's local symbols; enumerators are not recovered.
#include "CDBCollisionAxis.h"
#include "CVector3.h"
extern bool separate(float, float, float, float, EBoxTriSeperatingAxis, float &, EBoxTriSeperatingAxis &);
extern bool separate(float, float, float, EBoxTriSeperatingAxis, float &, EBoxTriSeperatingAxis &);
extern bool separate(float, float, EBoxTriSeperatingAxis, float &, EBoxTriSeperatingAxis &);
extern bool boxline_testaxis(const CVector3 &, const CVector3 &, const CVector3 &, float, float &, float &);
#endif
