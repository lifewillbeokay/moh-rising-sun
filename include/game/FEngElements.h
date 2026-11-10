// FEng's drawable objects, as FEPackageReader::CreateObject builds them. Class names and table
// symbols are from the symbol table; member names are inferred. The constructors taking a type are
// inferred from the store to FEObject::type repeated before the reader's common store.
#ifndef GAME_FENG_ELEMENTS_H
#define GAME_FENG_ELEMENTS_H
#include "FEngLists.h"
#include "FEngStrings.h"
#include "FEListBox.h"
#include "FEColor.h"
#pragma interface

// Called when a string's label hash is read; the class name is inferred, FEString::pLabelCallback is a symbol.
class FEStringLabelCallback {
  public:
    virtual void LabelRead(class FEString *);
};
class FEString : public FEObject {
  public:
    FEString(unsigned long objectType) : label(0), labelHash(-1) {
        unknown_68 = 0;
        unknown_6c = 0;
        unknown_70 = 0;
        type = objectType;
    }
    FEString(const FEString &, bool);
    virtual ~FEString();
    void SetLabel(const char *);
    static FEStringLabelCallback *pLabelCallback;
    char *label;              // +0x58
    long labelHash;           // +0x5c
    FEWideString text;        // +0x60
    unsigned long unknown_68; // +0x68
    unsigned long unknown_6c; // +0x6c
    unsigned long unknown_70; // +0x70
};
class FEImage : public FEObject {
  public:
    FEImage(unsigned long objectType) : unknown_58(0) { type = objectType; }
    virtual ~FEImage();
    unsigned long unknown_58; // +0x58
};
class FEMovie : public FEObject {
  public:
    FEMovie(unsigned long objectType) : unknown_58(0) { type = objectType; }
    virtual ~FEMovie();
    unsigned long unknown_58; // +0x58
};
class FEColoredImage : public FEObject {
  public:
    FEColoredImage(unsigned long objectType) : unknown_58(0) { type = objectType; }
    virtual ~FEColoredImage();
    unsigned long unknown_58; // +0x58
};
class FEAnimImage : public FEObject {
  public:
    FEAnimImage(unsigned long objectType) : unknown_58(0) { type = objectType; }
    virtual ~FEAnimImage();
    unsigned long unknown_58; // +0x58
};
class FESimpleImage : public FEObject {
  public:
    FESimpleImage(unsigned long objectType) { type = objectType; }
    virtual ~FESimpleImage();
};
// 196 bytes: CreateObject and Clone both allocate that much. Member names are inferred from use.
class FECodeListBox : public FEObject {
  public:
    typedef void (*SelectCallback)(FECodeListBox *);
    static SelectCallback mpDefaultCallback;
    FECodeListBox();
    FECodeListBox(const FECodeListBox &, bool);
    virtual ~FECodeListBox();
    void FillAllCells();
    FEObject *Clone(bool);
    void CopyProperties(const FECodeListBox &);
    void SetTotalNumColumns(unsigned long);
    void SetTotalNumRows(unsigned long);
    void ScrollSelection(long, long);
    int ScrollSelection(long, unsigned long &, unsigned long &, unsigned long, unsigned long, bool);
    int CheckMovement(long, long, long, long, long);
    int MakeMove(long, unsigned long &, unsigned long &, unsigned long, unsigned long);
    void Initialize(unsigned long, unsigned long);
    void AllocateStrings(unsigned long, unsigned long);
    void Update(float);
    static void DefaultSelectCallback(FECodeListBox *);
    short *AllocateString();
    void DeallocateString(short *);
    unsigned long CalculateCurrentFromTarget(unsigned long, unsigned long, unsigned long);
    long GetRealColumn(long) const;
    long GetRealRow(long) const;
    void SetCellColor(unsigned long, unsigned long, unsigned long, unsigned long, unsigned long);
    void SetCellScale(unsigned long, unsigned long, const FEPoint &, unsigned long, unsigned long);
    void SetCellJustification(unsigned long, unsigned long, unsigned long, unsigned long, unsigned long);
    FEGameInterface *game;       // +0x58
    unsigned long numColumns;    // +0x5c
    unsigned long numRows;       // +0x60
    unsigned long listFlags;     // +0x64; bit 8 wraps.
    unsigned long totalColumns;  // +0x68
    unsigned long totalRows;     // +0x6c
    unsigned long currentColumn; // +0x70
    unsigned long currentRow;    // +0x74
    unsigned long targetColumn;  // +0x78
    unsigned long targetRow;     // +0x7c
    float unknown_80;            // +0x80
    float unknown_84;            // +0x84
    FEListBoxCell *cells;        // +0x88
    unsigned long unknown_8c;    // +0x8c
    unsigned long unknown_90;    // +0x90
    unsigned long stringsInUse;  // +0x94
    short **strings;             // +0x98
    short *stringBlock;          // +0x9c
    float blink;                 // +0xa0
    float blinkRate;             // +0xa4
    FEColor selectColor;         // +0xa8
    SelectCallback selectCallback; // +0xb8
    unsigned long unknown_bc;    // +0xbc
    unsigned long unknown_c0;    // +0xc0
};
#endif
