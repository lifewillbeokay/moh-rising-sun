// AI-assisted reconstruction from GR8E69; see docs/Geometry.md.
#include "CQuaternion.h"
extern "C" float sinf(float),cosf(float);
void CQuaternion::SetFromEuler(float a,float b,float c) {
    float ha=a*0.5f,hb=b*0.5f,hc=c*0.5f;
    float cb=cosf(hb),cc=cosf(hc),ca=cosf(ha),sb=sinf(hb),sc=sinf(hc),sa=sinf(ha);
    float cbca=cb*ca, sbca=sb*ca, cbsa=cb*sa, sbsa=sb*sa;
    Set(cc*sbca-sc*cbsa,cc*sbsa+sc*cbca,cc*cbsa-sc*sbca,cc*cbca+sc*sbsa);
}
