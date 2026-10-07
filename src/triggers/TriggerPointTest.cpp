// AI-assisted reconstruction from GR8E69; see docs/TriggerObject.md.
#include "TriggerObject.h"
int IsPointInFlexTrigger(const CVector3 &, FlexProp *);
int IsPointInTrigger(const CVector3 &point, TriggerObject &trigger) { return IsPointInFlexTrigger(point, &trigger.properties.flex); }
