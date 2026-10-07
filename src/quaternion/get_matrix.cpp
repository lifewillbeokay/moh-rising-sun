// AI-assisted reconstruction from GR8E69; see docs/Geometry.md.
#include "CQuaternion.h"
void CQuaternion::GetMatrix(CMatrix& m) const {
    float tx=x+x,ty=y+y,tz=z+z,tw=w+w;
    float xx=tx*x,yy=ty*y,zz=tz*z,xy=tx*y,xz=tx*z,yz=ty*z,wx=tw*x,wy=tw*y,wz=tw*z;
    m.row[0].x=1-(yy+zz);
    m.row[1].y=1-(xx+zz);
    m.row[2].z=1-(xx+yy);
    m.row[0].y=xy-wz;
    m.row[1].x=xy+wz;
    m.row[0].z=xz+wy;
    m.row[2].x=xz-wy;
    m.row[1].z=yz-wx;
    m.row[2].y=yz+wx;
    m.row[0].w=0;
    m.row[1].w=0;
    m.row[2].w=0;
    m.row[3].x=0;
    m.row[3].y=0;
    m.row[3].z=0;
    m.row[3].w=1;
}
