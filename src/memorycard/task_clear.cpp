// A fragment of the memory-card library's task classes: what each task clears. Class and method names come from the
// symbol table; member names are inferred from the code. These methods are virtual in the original and the classes
// derive from one another; these views only need the members each method touches.
extern "C" void *memset(void *, int, unsigned int);

namespace Realmc {
struct FindInfoStruct {
    unsigned char unknown00[4];
    int unknown04, unknown08, unknown0c, unknown10;
    char unknown14[4];
    char unknown18[2];
    void Clear();
};

void FindInfoStruct::Clear() {
    unknown04 = 0;
    unknown08 = 0;
    unknown0c = 0;
    unknown10 = 0;
    memset(unknown14, 0, sizeof(unknown14));
    memset(unknown18, 0, sizeof(unknown18));
}

// The common task fields, reached through the name GcTask in the symbols.
class GcTask {
  public:
    unsigned char unknown00[8];
    int status; // +0x08
    unsigned char unknown0c[0x14 - 0x0c];
    unsigned short slot; // +0x14
    int kind;   // +0x18
    int enable; // +0x1c
    void Init(bool);
};

void GcTask::Init(bool value) {
    slot = 0;
    kind = 27;
    enable = value;
    status = 0;
}

class TaskTrc {
  public:
    unsigned char unknown00[0x20];
    int unknown20, unknown24, unknown28; // +0x20
    void ClearCommon() {
        unknown20 = 0;
        unknown24 = 0;
        unknown28 = 0;
    }
    void Clear();
};

void TaskTrc::Clear() {
    ClearCommon();
}

class TaskTrcStartGame : public TaskTrc {
  public:
    unsigned char info[76]; // +0x2c
    int unknown78;
    void Clear();
};

void TaskTrcStartGame::Clear() {
    ClearCommon();
    memset(info, 0, sizeof(info));
    unknown78 = 0;
}

class TaskTrcGetCardInfo : public TaskTrc {
  public:
    unsigned char info[12]; // +0x2c
    void Clear();
};

void TaskTrcGetCardInfo::Clear() {
    ClearCommon();
    memset(info, 0, sizeof(info));
}

class TaskTrcSaveFile : public TaskTrc {
  public:
    unsigned char info[48]; // +0x2c
    int unknown5c, unknown60, unknown64, unknown68, unknown6c;
    int unknown70; // +0x70; 15.
    int unknown74, unknown78, unknown7c;
    void Clear();
};

void TaskTrcSaveFile::Clear() {
    ClearCommon();
    memset(info, 0, sizeof(info));
    unknown5c = 0;
    unknown60 = 0;
    unknown64 = 0;
    unknown68 = 0;
    unknown6c = 0;
    unknown70 = 15;
    unknown74 = 0;
    unknown78 = 0;
    unknown7c = 0;
}

class TaskTrcListFiles : public TaskTrc {
  public:
    unsigned char info[48]; // +0x2c
    void Clear();
};

void TaskTrcListFiles::Clear() {
    ClearCommon();
    memset(info, 0, sizeof(info));
    unknown24 = 0;
}

class TaskTrcLoadFile : public TaskTrc {
  public:
    unsigned char info[48]; // +0x2c
    int unknown5c, unknown60;
    void Clear();
};

void TaskTrcLoadFile::Clear() {
    ClearCommon();
    memset(info, 0, sizeof(info));
    unknown20 = 0;
    unknown60 = 15;
    unknown5c = 0;
}

class TaskTrcDeleteFile : public TaskTrc {
  public:
    unsigned char info[48]; // +0x2c
    int unknown5c, unknown60, unknown64;
    void Clear();
};

void TaskTrcDeleteFile::Clear() {
    ClearCommon();
    memset(info, 0, sizeof(info));
    unknown5c = 0;
    unknown60 = 15;
    unknown64 = 0;
}
}
