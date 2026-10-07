#include "BPD.h"
#include "offset_ptr_impl.h"

template void offsetPtr<void>(void *&, int);
template void offsetPtr<unsigned long>(unsigned long *&, int);
template void offsetPtr<float>(float *&, int);
template void offsetPtr<PropPlane4>(PropPlane4 *&, int);
template void offsetPtr<BPDLight>(BPDLight *&, int);
