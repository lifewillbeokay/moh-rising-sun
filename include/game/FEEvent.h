// Event names and 12-byte layout from IssueScriptMessages; see docs/research/feng-packages.md.
#ifndef GAME_FE_EVENT_H
#define GAME_FE_EVENT_H
#pragma interface
struct FEEvent {
    unsigned long message;
    unsigned long target;
    unsigned long time;
};
class FEEventList {
  public:
    long count;
    FEEvent *events;
    FEEventList() : count(0), events(0) {}
    ~FEEventList() { if (events) delete[] events; events = 0; }
    void operator=(FEEventList &);
    void SetCount(long);
};
#endif
