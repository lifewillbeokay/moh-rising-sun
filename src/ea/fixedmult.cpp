// A 16.16 fixed point multiply with rounding, used by the MAD decoder. The name comes from the symbol table.
extern "C" int fixedmult(int a, int b) { return (int)(((long long)a * (long long)b + 0x8000) >> 16); }
