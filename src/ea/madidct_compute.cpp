// EA's MAD inverse transform (madidct.cpp). Function and table names come from the symbol table; parameter and local names
// are inferred. A fast 8-point transform on 16.16 fixed point: it runs each column and then each row.
extern "C" void idctcompute(int *, int);
extern "C" void IdctColumn(const int *, int *); // File-local in the original, defined outside this fragment.
extern "C" void IdctRow(const int *, int *);
extern int idctinput[];
extern int work[];

extern "C" void idctcompute(int *out, int stride) {
    IdctColumn(idctinput, work);
    IdctColumn(idctinput + 8, work + 1);
    IdctColumn(idctinput + 16, work + 2);
    IdctColumn(idctinput + 24, work + 3);
    IdctColumn(idctinput + 32, work + 4);
    IdctColumn(idctinput + 40, work + 5);
    IdctColumn(idctinput + 48, work + 6);
    IdctColumn(idctinput + 56, work + 7);
    IdctRow(work, out);
    IdctRow(work + 8, out + stride);
    IdctRow(work + 16, out + stride * 2);
    IdctRow(work + 24, out + stride * 3);
    IdctRow(work + 32, out + stride * 4);
    IdctRow(work + 40, out + stride * 5);
    IdctRow(work + 48, out + stride * 6);
    IdctRow(work + 56, out + stride * 7);
}
