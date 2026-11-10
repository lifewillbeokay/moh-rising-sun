// AI-assisted reconstruction of the frustum volume's copying and destructor; see docs/Rendering.md.
#include "VolFrustum.h"
CVolFrustum::CVolFrustum(const CVolFrustum &other) {
    for (int i = 0; i < 6; i++)
        planes[i] = other.planes[i];
}
CVolFrustum::~CVolFrustum() {
}
CVolFrustum &CVolFrustum::operator=(const CVolFrustum &other) {
    for (int i = 0; i < 6; i++)
        planes[i] = other.planes[i];
    return *this;
}
