// AI-assisted GR8E69 reconstruction; see docs/research/feng-lifecycle.md.
#ifdef DECOMP_COMPARE
#include "FELocalizer.h"
#include <string.h>
int CompareUpper(const short *, const char *);
int CompareUpperSubst(const short *, const char *);
int WideToCharString(char *, const short *);
int AtoI(const char *);
int FELocalizer::ReadLocaleData(void *buffer, unsigned long size) {
    short wide[2048]; char text[2048], label[256];
    unsigned long offset = 0;
    int index = 0;
    FEWideString value;
    char *data = (char *)buffer;
    if (!ReadNextString(wide, data, offset, size) || CompareUpper(wide, "[HEADER]")) return 0;
    do { if (!ReadNextString(wide, data, offset, size)) return 0; }
    while (CompareUpperSubst(wide, "Languages"));
    WideToCharString(text, wide);
    int digit = 9;
    while (text[digit] && (unsigned long)(text[digit] - '0') > 9) ++digit;
    SetNumLanguages(AtoI(text + digit));
    for (unsigned long language = 0; language < numLanguages; ++language)
        if (ReadNextString(wide, data, offset, size) && WideToCharString(text, wide)) SetLanguageName(text, language);
    int more;
    do {
        wide[0] = 0;
        more = ReadNextString(wide, data, offset, size);
        if (!wide[0]) continue;
        if (!CompareUpperSubst(wide, "[LABEL]")) {
            WideToEscapeString(label, wide + 8);
            value = (const char *)0;
            SetString(label, 0, value);
            index = GetLabelIndex(label);
        } else if (!CompareUpperSubst(wide, "[PLATFORM]")) {
            WideToEscapeString(text, wide + 11); entries[index].SetPlatform(text);
        } else if (!CompareUpperSubst(wide, "[COMMENT]")) {
            WideToEscapeString(text, wide + 10); entries[index].SetComment(text);
        } else if (!CompareUpperSubst(wide, "[FLAGS]")) {
            WideToEscapeString(text, wide + 8); entries[index].SetFlags(text);
        } else if (!CompareUpperSubst(wide, "[MAX_SIZE]")) {
            WideToEscapeString(text, wide + 11); entries[index].SetSize(text);
        } else {
            int language = 0;
            for (FENode *node = (FENode *)languages.head; node; node = (FENode *)node->next, ++language) {
                if (!CompareUpperSubst(wide + 1, node->name)) {
                    value = wide + 3 + strlen(node->name);
                    entries[index].strings[language] = value;
                    break;
                }
            }
        }
    } while (more);
    return 1;
}
#endif
