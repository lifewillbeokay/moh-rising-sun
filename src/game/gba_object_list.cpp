// The Game Boy Advance link's object list and packet check. Function names come from the symbol table; the
// member names and packet field meanings are inferred from this code.
struct GameObject {
    int type;
    int x;
    int y;
    int z;
};

extern GameObject GameObjects[64];
extern int gCurrentMapNumber;

unsigned long CalcCrc(unsigned long a, unsigned long b) {
    unsigned int sumA = 0;
    unsigned int sumB = 0;
    int i;
    for (i = 0; i < 32; i++)
        sumA += a >> i;
    for (i = 8; i < 32; i++)
        sumB += b >> i;
    return (sumA + sumB) & 15;
}

int ApplyXScaling(int x) {
    return (unsigned short)x;
}

int ApplyYScaling(int y) {
    return (unsigned short)y;
}

void AddObjectToList(int index, int type, int x, int y, int z) {
    if ((unsigned int)index <= 63 && (unsigned int)(type - 1) <= 14) {
        GameObjects[index].x = x;
        GameObjects[index].y = y;
        GameObjects[index].type = type;
        GameObjects[index].z = z;
    }
}

void InitObjectList() {
    for (int i = 63; i >= 0; i--)
        GameObjects[i].type = 0;
}

void ObjectList_SetCurrentGBAMap(int map) {
    gCurrentMapNumber = map & 15;
}
