// AI-assisted reconstruction of the sound system front end's vector mode switch; see docs/Sound.md.
#include "SndSystem.h"
extern "C" int SNDSYS_vectortoreal();
namespace Snd {
int System::VectorToReal() {
    SNDSYS_vectortoreal();
    return 0;
}
}
