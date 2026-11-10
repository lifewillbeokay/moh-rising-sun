// FEng message responses; class tags from symbols. See docs/Frontend.md.
#ifndef GAME_FE_MESSAGE_RESPONSE_H
#define GAME_FE_MESSAGE_RESPONSE_H
#pragma interface
#include "FEObjectPool.h"
// A response id, and a parameter that is a string for the ids HasStringParam tests.
class FEResponse {
  public:
    unsigned long id; // +0x0
    char *param;   // +0x4
    long value;    // +0x8
    FEResponse() : id(0), param(0), value(0) {}
    ~FEResponse();
    FEResponse &operator=(FEResponse &);
    void SetParam(const char *);
    void ReleaseParam();
    bool HasStringParam() const { return (unsigned long)(id - 512) <= 4 && id != 515; }
};
class FEMessageResponse : public FEMinNode {
  public:
    long message;          // +0xc
    unsigned long count;   // +0x10
    FEResponse *responses; // +0x14
    static FEMinList NodePool;
    FEMessageResponse() : message(0), count(0), responses(0) {}
    virtual ~FEMessageResponse();
    void *operator new(unsigned int);
    void operator delete(void *);
    void PurgeResponses();
    void SetCount(unsigned long);
    long FindResponse(unsigned long) const;
    long FindConditionBranchTarget(unsigned long) const;
};
#endif
