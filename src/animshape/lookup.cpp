// AI-assisted reconstruction from GR8E69; see docs/research/broad-source-sweep.md.
#include "AnimShape.h"

#include <string.h>
static void encodeName(char *out, int value) {
    char alphabet[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ~!";
    out[0] = alphabet[value & 63];
    out[1] = alphabet[(value >> 6) & 63];
    out[2] = alphabet[(value >> 12) & 63];
    out[3] = alphabet[(value >> 18) & 63];
    out[4] = 0;
}
static int HexStrToInt(const char *text) {
    int value = 0;
    while (*text) {
        int c = (signed char)*text;
        value <<= 4;
        if (c >= '0' && c <= '9')
            value += c - '0';
        else if (c >= 'a' && c <= 'f')
            value += c - 'a' + 10;
        else if (c >= 'A' && c <= 'F')
            value += c - 'A' + 10;
        ++text;
    }
    return value;
}
AnimShape *AnimShape::FindFromCRC(int crc) {
    char encoded[5];
    AnimShape *shape = ms_pShapes;
    encodeName(encoded, crc);
    while (shape && strncmp(encoded, shape->name, 5))
        shape = shape->next;
    return shape;
}
