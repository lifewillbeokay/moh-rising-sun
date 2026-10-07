// AI-assisted reconstruction from GR8E69; see docs/Matrix.md.
#include "CMatrix.h"
extern "C" float asinf(float),atan2f(float,float);
void CMatrix::ToEulerXYZ(float& x,float& y,float& z) const {
    if (!(row[0].z>=1.0f)) {
        if (!(row[0].z<=-1.0f)) {
            x=asinf(-row[1].z);
            y=atan2f(-row[0].z,row[2].z);
            z=atan2f(row[0].y,row[0].x);
        }
        else {
            x=-1.570796326794896619f;
            y=-atan2f(row[1].x,row[1].y);
            z=0.0f;
        }
    }
    else {
        x=1.570796326794896619f;
        y=atan2f(row[1].x,row[1].y);
        z=0.0f;
    }
}
