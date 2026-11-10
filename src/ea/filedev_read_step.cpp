// A fragment of EA's file device (filedev.cpp): the asynchronous disc read. AyncDVDRead runs on the read thread after
// each DVD transfer, copies what it brought and queues the next. The states are 1, a short read
// through the bounce buffer; 2, a misaligned head; 3, a run read in place; 4, a chunk of at most 16 KB through the
// bounce buffer; 5, finished. DVDFileInfo is named by the mangled symbols; the request record is inferred.
struct DVDFileInfo;
extern "C" {
int DVDReadAsyncPrio(DVDFileInfo *, void *, long, long, void (*)(long, DVDFileInfo *), long);
void DCInvalidateRange(void *, unsigned long);
void MEM_copy(void *, const void *, int);
}

struct READREQUEST {
    int state;
    int dest;
    int offset;
    int size;
    int destAlignedDown;
    int destAlignedUp;
    int destEnd;
    int destEndAligned;
    int offsetAligned;
    int offsetAlignedUp;
    int offsetEnd;
    int offsetEndAligned;
    int misalign;
    int headLength;
    int queued;
    unsigned char unknown3c[0x60 - 0x3c]; // Includes the request fields written by the requesting thread.
    char temp[32768];
};

// A file static in the original, defined outside this fragment.
extern READREQUEST gCurRead;

void AyncDVDCallback(long, DVDFileInfo *); // A file-local function in the original, defined outside this fragment.

static void QEndOp() {
    gCurRead.state = 5;
    AyncDVDCallback(0, 0);
}

static void AyncDVDRead(DVDFileInfo *info) {
    int state = gCurRead.state;
    {
        READREQUEST *read = &gCurRead;
        if (state == 2) {
            char *head = read->temp;
            MEM_copy((void *)read->dest, read->misalign + head, read->headLength);
            read->offset += read->headLength;
            read->dest += read->headLength;
        } else if (state == 3) {
            DCInvalidateRange((void *)read->dest, read->destEndAligned - read->dest);
            read->offset += read->destEndAligned - read->dest;
            read->dest += read->destEndAligned - read->dest;
        } else if (state == 4) {
            int length = read->destEnd - read->dest;
            if (length > 16384)
                length = 16384;
            MEM_copy((void *)read->dest, read->temp, length);
            read->offset += length;
            read->dest += length;
        } else if (state == 1) {
            char *head = read->temp;
            MEM_copy((void *)read->dest, read->misalign + head, read->size);
            gCurRead.state = 1;
            QEndOp();
            return;
        }
    }
    {
        READREQUEST *read = &gCurRead;
        int dest = read->dest;
        if (dest == read->destAlignedUp) {
            int end = read->destEndAligned;
            if (dest < end) {
                gCurRead.state = 3;
                read->queued = DVDReadAsyncPrio(info, (void *)dest, end - dest, read->offset, AyncDVDCallback, 2);
                return;
            }
        }
    }
    {
        READREQUEST *read = &gCurRead;
        int end = read->destEnd;
        int dest = read->dest;
        if (end > dest) {
            int length = (end - dest + 31) & ~31;
            if (length > 16384)
                length = 16384;
            gCurRead.state = 4;
            read->queued += DVDReadAsyncPrio(info, read->temp, length, read->offset, AyncDVDCallback, 2);
            return;
        }
    }
    QEndOp();
}
