// AI-assisted reconstruction of the CODA allocation hook setters (coda.cpp); see docs/Sound.md.
namespace Snd {
extern void *(*CODANew)(unsigned int);
extern void (*CODADelete)(void *);

void CODASetNew(void *(*allocate)(unsigned int)) {
    CODANew = allocate;
}

void CODASetDelete(void (*release)(void *)) {
    CODADelete = release;
}
}
