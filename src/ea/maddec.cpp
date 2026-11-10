// EA's MAD macroblock decoder (maddec.cpp). Function and table names come from the symbol table. Parameter, local and
// helper names are inferred from the code. The tables are defined outside this unit. Macroblocks hold their coefficients
// in 16.16 fixed point, with luma blocks in a 16-wide array and chroma in 8-wide ones.
extern "C" int fixedmult(int, int);
extern "C" int madvlcdecode();
extern "C" void idctcompute(int *, int);

extern const unsigned char *maddataptr;
extern unsigned int madshiftreg;
extern int madbitcount;
extern int initflag;
extern int motionframe;

extern int madvlctbl1[];
extern int madvlctbl2[];
extern int madvlctbl3[];
extern int madvlctbl4[];
extern int madquant[];
extern int idctinput[];
extern int idctprescale[];
extern unsigned char clipbiastbl[];
extern int luma[];
extern int chroma[];
extern const int encodetbl1[];
extern const int encodetbl2[];
extern const int quanttbl[];

static inline int Entry(int value, int length) { return value << 22 | (value << 6 & 0x3f0000) | length; }

static void madinit() {
    int i, j, length, code, value, count, base;
    for (i = -256; i <= 254; i++) {
        value = i;
        if (i < -128)
            value = -128;
        else if (i > 127)
            value = 127;
        clipbiastbl[i & 0x1ff] = value - 128;
    }
    madvlctbl1[0] = 15;
    for (i = 1; i <= 7; i++)
        madvlctbl1[i] = 31;
    for (i = 0; i <= 7; i++)
        madvlctbl1[i + 8] = 47;
    for (i = 0; i <= 127; i++)
        madvlctbl1[i + 256] = 63;
    for (i = 1; i <= 94; i++) {
        length = encodetbl1[i * 4];
        code = encodetbl1[i * 4 + 3];
        value = encodetbl1[i * 4 + 1];
        if (code & 0xfc00) {
            count = 1 << (9 - length);
            value = Entry(value, length);
            base = code >> 7;
            for (j = 0; j < count; j++)
                madvlctbl1[base + j] = value;
        } else {
            count = 1 << (14 - length);
            value = Entry(value, length - 6);
            base = code >> 2;
            for (j = 0; j < count; j++)
                madvlctbl3[base + j] = value;
        }
    }
    for (i = 0; i <= 127; i++) {
        int bits = encodetbl2[i * 4];
        code = encodetbl2[i * 4 + 3];
        value = encodetbl2[i * 4 + 1];
        length = bits + 8;
        if (!(code & 0x8000)) {
            count = 1 << (17 - length);
            value = Entry(value, bits - 1);
            base = code >> 7;
            for (j = 0; j < count; j++)
                madvlctbl2[base + j] = value;
        } else {
            count = 1 << (14 - length);
            value = Entry(value, bits + 2);
            base = code >> 10;
            for (j = 0; j < count; j++)
                madvlctbl3[base + j] = value;
        }
    }
    for (i = 0; i <= 31; i++)
        madvlctbl4[i] = 1;
    for (i = 0; i <= 15; i++) {
        value = i + 1;
        madvlctbl4[i + 32] = value << 22 | 6;
        madvlctbl4[i + 48] = (i - 16) << 22 | 6;
        i = value - 1;
    }
    initflag = 1;
}

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

static int getdelta() {
    int entry = madvlctbl4[madshiftreg >> 26];
    discardbits(entry & 0xff);
    return entry >> 22;
}

static void dcblock(int *block, int stride) {
    for (int i = 0; i <= 7; i++) {
        block[0] = idctinput[0];
        block[1] = idctinput[0];
        block[2] = idctinput[0];
        block[3] = idctinput[0];
        block[4] = idctinput[0];
        block[5] = idctinput[0];
        block[6] = idctinput[0];
        block[7] = idctinput[0];
        block += stride;
    }
}

static void getluma(const unsigned char *pixels, int stride, int *block, int delta) {
    for (int i = 0; i <= 7; i++) {
        block[0] = (pixels[0] + delta) << 16;
        block[1] = (pixels[1] + delta) << 16;
        block[2] = (pixels[2] + delta) << 16;
        block[3] = (pixels[3] + delta) << 16;
        block[4] = (pixels[4] + delta) << 16;
        block[5] = (pixels[5] + delta) << 16;
        block[6] = (pixels[6] + delta) << 16;
        block[7] = (pixels[7] + delta) << 16;
        pixels += stride;
        block += 16;
    }
}

static void getchroma(const unsigned char *pixels, int stride, int *block, int delta) {
    for (int i = 0; i <= 7; i++) {
        block[0] = (pixels[0] + delta) << 16;
        block[1] = (pixels[1] + delta) << 16;
        block[2] = (pixels[2] + delta) << 16;
        block[3] = (pixels[3] + delta) << 16;
        block[4] = (pixels[4] + delta) << 16;
        block[5] = (pixels[5] + delta) << 16;
        block[6] = (pixels[6] + delta) << 16;
        block[7] = (pixels[7] + delta) << 16;
        pixels += stride;
        block += 8;
    }
}

static inline unsigned char Clip(int value) { return clipbiastbl[(value >> 16) & 0x1ff]; }

static void setluma(const int *block, unsigned char *pixels, int stride) {
    for (int i = 0; i <= 15; i++) {
        pixels[0] = Clip(block[0]);
        pixels[1] = Clip(block[1]);
        pixels[2] = Clip(block[2]);
        pixels[3] = Clip(block[3]);
        pixels[4] = Clip(block[4]);
        pixels[5] = Clip(block[5]);
        pixels[6] = Clip(block[6]);
        pixels[7] = Clip(block[7]);
        pixels[8] = Clip(block[8]);
        pixels[9] = Clip(block[9]);
        pixels[10] = Clip(block[10]);
        pixels[11] = Clip(block[11]);
        pixels[12] = Clip(block[12]);
        pixels[13] = Clip(block[13]);
        pixels[14] = Clip(block[14]);
        pixels[15] = Clip(block[15]);
        block += 16;
        pixels += stride;
    }
}

static void setchroma(const int *block, unsigned char *pixels, int stride) {
    for (int i = 0; i <= 7; i++) {
        pixels[0] = Clip(block[0]);
        pixels[1] = Clip(block[1]);
        pixels[2] = Clip(block[2]);
        pixels[3] = Clip(block[3]);
        pixels[4] = Clip(block[4]);
        pixels[5] = Clip(block[5]);
        pixels[6] = Clip(block[6]);
        pixels[7] = Clip(block[7]);
        block += 8;
        pixels += stride;
    }
}

void MAD_initdecode(const unsigned short *data, int motion, int quality) {
    if (!initflag)
        madinit();
    const unsigned char *bytes = (const unsigned char *)data;
    madshiftreg = (bytes[1] << 8 | bytes[0]) << 16 | (bytes[3] << 8 | bytes[2]);
    madbitcount = 32;
    maddataptr = bytes + 4;
    motionframe = motion;
    madquant[0] = fixedmult(quanttbl[0] << 16, idctprescale[0]);
    for (int i = 1; i <= 63; i++)
        madquant[i] = fixedmult(quality * quanttbl[i] << 13, idctprescale[i]);
}

void MAD_decodemacroblock(const unsigned char *lastY, const unsigned char *lastU, const unsigned char *lastV,
                          unsigned char *newY, unsigned char *newU, unsigned char *newV, int stride) {
    int chromaStride = stride >> 1;
    int mode;
    if (!motionframe) {
        mode = 0;
    } else if (!(madshiftreg & 0xc0000000)) {
        mode = 0;
        discardbits(2);
    } else {
        if ((int)madshiftreg < 0) {
            mode = 1023;
            discardbits(1);
        } else {
            mode = madshiftreg >> 24;
            discardbits(8);
        }
        int dx = getdelta();
        int dy = getdelta();
        lastY += dy * stride + dx;
        dx >>= 1;
        dy >>= 1;
        lastU += dy * chromaStride + dx;
        lastV += dy * chromaStride + dx;
    }
    if ((mode & 1) == 0) {
        if (madvlcdecode() == 1)
            dcblock(luma, 16);
        else
            idctcompute(luma, 16);
    } else {
        int delta = getdelta() * 2 - 128;
        getluma(lastY, stride, luma, delta);
    }
    if ((mode & 2) == 0) {
        if (madvlcdecode() == 1)
            dcblock(luma + 8, 16);
        else
            idctcompute(luma + 8, 16);
    } else {
        int delta = getdelta() * 2 - 128;
        getluma(lastY + 8, stride, luma + 8, delta);
    }
    if ((mode & 4) == 0) {
        if (madvlcdecode() == 1)
            dcblock(luma + 128, 16);
        else
            idctcompute(luma + 128, 16);
    } else {
        int delta = getdelta() * 2 - 128;
        getluma(lastY + stride * 8, stride, luma + 128, delta);
    }
    if ((mode & 8) == 0) {
        if (madvlcdecode() == 1)
            dcblock(luma + 136, 16);
        else
            idctcompute(luma + 136, 16);
    } else {
        int delta = getdelta() * 2 - 128;
        getluma(lastY + stride * 8 + 8, stride, luma + 136, delta);
    }
    if ((mode & 16) == 0) {
        if (madvlcdecode() == 1)
            dcblock(chroma, 8);
        else
            idctcompute(chroma, 8);
    } else {
        int delta = getdelta() * 2 - 128;
        getchroma(lastU, chromaStride, chroma, delta);
    }
    if ((mode & 32) == 0) {
        if (madvlcdecode() == 1)
            dcblock(chroma + 64, 8);
        else
            idctcompute(chroma + 64, 8);
    } else {
        int delta = getdelta() * 2 - 128;
        getchroma(lastV, chromaStride, chroma + 64, delta);
    }
    setluma(luma, newY, stride);
    setchroma(chroma, newU, chromaStride);
    setchroma(chroma + 64, newV, chromaStride);
}
