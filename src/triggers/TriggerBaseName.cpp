// AI-assisted reconstruction from GR8E69; see docs/TriggerObject.md.
#include <string.h>

void GetBaseName(const char *source, char *destination) {
    const char *back = strrchr(source, '\\');
    if (!back)
        back = source;
    else
        ++back;
    const char *forward = strrchr(source, '/');
    if (!forward)
        forward = source;
    else
        ++forward;
    if (forward > back)
        back = forward;
    strcpy(destination, back);
    char *dot = strchr(destination, '.');
    if (dot)
        *dot = 0;
}
