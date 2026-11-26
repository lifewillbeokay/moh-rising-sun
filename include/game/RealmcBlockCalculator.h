// AI-assisted scoped view; virtual slots and offsets come from blockcalculator.cpp.
#ifndef GAME_REALMC_BLOCK_CALCULATOR_H
#define GAME_REALMC_BLOCK_CALCULATOR_H
#include "RealmcDriver.h"
namespace Realmc {
class BlockCalculator {
public:
    virtual void Clear() = 0;
    virtual int GetResult() = 0;
    virtual void SetFileInfo(const CardID &, const FileInfo &) = 0;
    virtual ~BlockCalculator();
};
class BlockCalculatorImp : public BlockCalculator {
public:
    virtual void Clear();
    virtual int GetResult();
    virtual void SetFileInfo(const CardID &, const FileInfo &);
    virtual ~BlockCalculatorImp();
    GCDriver *driver; // +4
    int blocks, files; // +8, +12
    unsigned int limit; // +16
    int hasInfo; // +20
    void Init(GCDriver *, unsigned int);
};
}
#endif
