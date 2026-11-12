// AI-assisted GR8E69 reconstruction; see docs/research/feng-lifecycle.md.
#ifdef DECOMP_COMPARE
#include "FEPackageReader.h"
#include "FEngine.h"
#include "FETypeNode.h"
#include "FEScript.h"
int FEPackageReader::ReadScriptTags(FETag *tag, unsigned long size) {
    char *end = (char *)tag + size;
    long index = -1;
    FEScript *script = 0;
    FEKeyTrack *track = 0;
    signed char offset = 0;
    for (; (char *)tag < end; tag = (FETag *)((char *)(tag + 1) + FESwapShort(tag->size))) {
        unsigned char *bytes = (unsigned char *)(tag + 1);
        switch (FESwapShort(tag->id)) {
        case 0x6e53:
            script = new FEScript;
            script->time = 0;
            if (keepExisting) script->SetName((const char *)bytes);
            index = -1; offset = 0;
            break;
        case 0x4953: script = object->FindScript(FETagLong(tag, 0)); track = 0; break;
        case 0x6853:
            script->hash = FETagLong(tag, 0);
            script->unknown_c = FETagLong(tag, 1); script->unknown_14 = FETagLong(tag, 2);
            script->SetTrackCount(FETagLong(tag, 3)); break;
        case 0x6c53: script->unknown_c = FETagLong(tag, 0); break;
        case 0x6653: if (script) script->unknown_14 = FETagLong(tag, 0); break;
        case 0x6353: script->unknown_18 = FETagLong(tag, 0); break;
        case 0x4946:
            track = script->tracks + ++index;
            track->unknown_0 = bytes[0]; track->valueSize = bytes[1];
            track->unknown_2 = bytes[2]; track->flags = bytes[3];
            track->lastKeyTime = FETagLong(tag, 1); track->dataOffset = offset;
            offset += track->valueSize >> 2;
            break;
        case 0x6f54: track->dataOffset = bytes[0]; break;
        case 0x7454: if (script) track->unknown_2 = bytes[0]; break;
        case 0x6154: if (script) track->flags = bytes[0]; break;
        case 0x6254:
            track->baseKey.time = FETagLong(tag, 0);
            for (unsigned long i = 0; i < (unsigned long)(track->valueSize >> 2); ++i)
                track->baseKey.value.word[i] = FETagLong(tag, i + 1);
            break;
        case 0x644b: {
            unsigned long stride = track->valueSize + 4;
            unsigned long count = FESwapShort(tag->size) / stride;
            unsigned long *values = (unsigned long *)bytes;
            if (track->keys.isReference) track->keys.ReferenceList(0);
            unsigned long i = 0;
            do {
                FEKeyNode *key = i ? new FEKeyNode : &track->baseKey;
                key->time = FESwap(values[0]);
                for (unsigned long word = 0; word < (stride >> 2) - 1; ++word)
                    key->value.word[word] = FESwap(values[word + 1]);
                if (i) track->keys.AddNode(track->keys.tail, key);
                ++i; values = (unsigned long *)((char *)values + stride);
            } while (i < count);
            break;
        }
        case 0x5645: {
            unsigned long count = FESwapShort(tag->size) / 12;
            script->events.SetCount(count);
            FEEvent *event = script->events.events;
            unsigned long *value = (unsigned long *)bytes;
            do {
                event->message = FESwap(value[0]); event->target = FESwap(value[1]);
                event->time = FESwap(value[2]); ++event; value += 3;
            } while (--count);
            break;
        }
        case 0x6954:
            if (script) {
                unsigned short kind = FESwapShort(*(unsigned short *)bytes);
                track = script->FindTrack((FEKeyTrack_Indices)kind);
                if (!track) {
                    FEKeyTrack *replacement = new FEKeyTrack[script->trackCount + 1];
                    FETypeNode *type = engine->typeLib.FindType(object->type);
                    FEFieldNode *field = (FEFieldNode *)type->fields.FindNode(kind);
                    unsigned long source = 0;
                    for (unsigned long i = 0; i <= (unsigned long)script->trackCount; ++i) {
                        FEKeyTrack *old = script->tracks;
                        FEKeyTrack &out = replacement[i];
                        if (!old || source >= (unsigned long)script->trackCount ||
                            (field && field->offset / 4 <= old[source].dataOffset)) {
                            out.unknown_0 = field->keyType; out.valueSize = field->size;
                            out.unknown_2 = 1; out.flags = 0; out.lastKeyTime = script->unknown_c;
                            out.dataOffset = field->offset >> 2;
                            field = 0; track = &out;
                        } else out = old[source++];
                    }
                    if (script->tracks) delete[] script->tracks;
                    script->tracks = replacement; ++script->trackCount;
                }
            }
            break;
        }
    }
    if (!isReference) object->scriptList.AddNode(object->scriptList.tail, script);
    if (script->hash == 0x1744b3) object->currentScript = script;
    return 1;
}
#endif
