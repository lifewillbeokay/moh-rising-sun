// A fragment of the original compartment code: instance types. Names come from the symbol table; member names
// are inferred from this code.
#include "Compartment.h"

struct PrimerInstanceType {
    unsigned char unknown00[8];
    int type; // +0x08
};
struct PrimerInstanceTable {
    int count;
    PrimerInstanceType *entries[1]; // +0x04; extent is count.
};
class CPTHeader {
  public:
    unsigned char unknown00[52];
    PrimerInstanceTable *instanceTypes; // +0x34
};

int CCompartment::GetInstanceType(int index) {
    return primerTable->instanceTypes->entries[index]->type;
}
