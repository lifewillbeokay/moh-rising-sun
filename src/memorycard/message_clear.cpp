// A fragment of the memory-card library's task classes: clearing a message. Class and method names come from the symbol
// table; member names are inferred from the code.
namespace Realmc {
class GCMessage {
  public:
    unsigned int id;
    int unknown04;
    unsigned char unknown08[0x70 - 8];
    int args[8]; // +0x70; the last is reset first.
    void Clear();
};

void GCMessage::Clear() {
    id = 0;
    unknown04 = 0;
    args[0] = 0;
    args[1] = 0;
    args[2] = 0;
    args[3] = 0;
    args[4] = 0;
    args[5] = 0;
    args[6] = 0;
    args[7] = 0;
}
}
