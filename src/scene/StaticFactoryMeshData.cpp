// A fragment of the original staticobjfactory.cpp: loading a mesh record's data. Names come from the
// symbol table; the record's member names are inferred from this code.
void *TLT_LoadFromRepository(const char *, int *, bool *);

// The level file's mesh fields: a frame count and that many file names.
struct LevelFileFields_ {
    int unknown00;
    int frameCount;      // +0x04
    const char *files[1]; // +0x08; extent is frameCount.
};

class CAnyStaticMesh {
  public:
    void InitMesh(void *, bool);
    void InitAnim(int, int);
    void InitFrame(int, void *, bool);
};

static void InitMeshData(CAnyStaticMesh &mesh, const LevelFileFields_ *fields, int rate) {
    int size;
    bool cached;
    if (fields->frameCount > 1) {
        mesh.InitAnim(fields->frameCount, rate);
        for (int i = 0; i < fields->frameCount; i++) {
            void *data = TLT_LoadFromRepository(fields->files[i], &size, &cached);
            mesh.InitFrame(i, data, !cached);
        }
    } else {
        void *data = TLT_LoadFromRepository(fields->files[0], &size, &cached);
        mesh.InitMesh(data, !cached);
    }
}
