// EA's MAD coefficient decoder (maddeca.cpp). Function and table names come from the symbol table; parameter and local
// names are inferred. It reads one block's run-length coded coefficients into idctinput and returns the run total, where 1
// means only the DC term was present.
extern const unsigned char *maddataptr;
extern unsigned int madshiftreg;
extern int madbitcount;
extern int madvlctbl1[];
extern int madvlctbl2[];
extern int madvlctbl3[];
extern int madquant[];
extern int idctinput[];
extern const int zigzag[];

static void discardbits(int count) {
    unsigned int shifted = madshiftreg << count;
    int left = madbitcount - count;
    madshiftreg = shifted;
    madbitcount = left;
    if (left <= 15) {
        const unsigned char *data = maddataptr;
        int bits = (data[1] << 8 | data[0]) << (16 - left);
        madbitcount = left + 16;
        maddataptr = data + 2;
        madshiftreg = shifted | bits;
    }
}

extern "C" int madvlcdecode() {
    idctinput[0] = (((int)madshiftreg) >> 24) * madquant[0];
    discardbits(8);
    int *clear = idctinput + 1;
    for (int i = 0; i <= 8; i++) {
        clear[0] = 0;
        clear[1] = 0;
        clear[2] = 0;
        clear[3] = 0;
        clear[4] = 0;
        clear[5] = 0;
        clear[6] = 0;
        clear += 7;
    }
    int position = 1;
    for (;;) {
        unsigned int entry = madvlctbl1[madshiftreg >> 23];
        int bits = entry & 0xff;
        if (bits > 9) {
            if ((entry & 32) == 0) {
                if ((entry & 16) == 0) {
                    discardbits(9);
                    entry = madvlctbl2[(unsigned char)(madshiftreg >> 24)];
                    bits = entry & 0xff;
                } else {
                    discardbits(6);
                    entry = madvlctbl3[(unsigned char)(madshiftreg >> 24)];
                    bits = entry & 0xff;
                }
            } else {
                if (entry & 16) {
                    discardbits(2);
                    return position;
                }
                discardbits(6);
                entry = madshiftreg;
                bits = 16;
            }
        }
        discardbits(bits);
        position += entry >> 16 & 63;
        idctinput[zigzag[position]] = ((int)entry >> 22) * madquant[zigzag[position]];
        ++position;
    }
}
