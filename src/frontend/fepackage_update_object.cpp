// AI-assisted GR8E69 reconstruction; see docs/research/feng-lifecycle.md.
#ifdef DECOMP_COMPARE
#include "FEngine.h"
#include "FEScript.h"
#include "FEngElements.h"
void FEPackage::UpdateObject(FEObject *object, long elapsed) {
    if (object->flags & 0x1c00000) object->flags |= 0x2000000;
    else object->flags &= uHoldDirtyFlags | 0xfdffffff;
    FEScript *script = object->currentScript;
    long oldTime = script->time, end = script->unknown_c;
    script->time += lastDeltaTime;
    if (script->time < 0) script->time = 0;
    if (script->time >= end && executing) {
        if (script->unknown_18) {
            UpdateObjectTracks(object, script);
            long remainder = script->time - end;
            script->time = 0;
            if (script->events.count) IssueScriptMessages(engine, object, script, oldTime, end);
            script = (FEScript *)script->unknown_18;
            object->SetCurrentScript(script); script->time = remainder;
            if (script->events.count) IssueScriptMessages(engine, object, script, 0, remainder);
        } else {
            switch (script->unknown_14 & 3) {
            case 0:
                if (script->events.count) IssueScriptMessages(engine, object, script, oldTime, end);
                script->time = script->unknown_c + 1; break;
            case 1:
                if ((long)script->unknown_c < 1) script->time = 0;
                else {
                    if (script->events.count) IssueScriptMessages(engine, object, script, oldTime, end);
                    script->time %= (long)script->unknown_c;
                    if (script->events.count) IssueScriptMessages(engine, object, script, 0, script->time);
                    object->SetupMoveToTracks();
                }
                break;
            case 2:
                if ((long)script->unknown_c < 1) script->time = 0;
                else script->time %= (long)script->unknown_c * 2;
                break;
            }
        }
    } else if (script->time < end && executing && script->events.count) {
        switch (script->unknown_14 & 3) {
        case 0: IssueScriptMessages(engine, object, script, oldTime, script->time); break;
        case 1:
            if (script->time < oldTime) {
                IssueScriptMessages(engine, object, script, oldTime, end);
                IssueScriptMessages(engine, object, script, 0, script->time);
            } else IssueScriptMessages(engine, object, script, oldTime, script->time);
            break;
        case 2:
            if (oldTime < end) IssueScriptMessages(engine, object, script, oldTime, script->time);
            else {
                IssueScriptMessages(engine, object, script, oldTime - end, 0);
                IssueScriptMessages(engine, object, script, 0, script->time);
            }
            break;
        }
    }
    if (!(executing && oldTime == script->time && oldTime == (long)script->unknown_c + 1 && !(object->flags & 0x400000)))
        UpdateObjectTracks(object, script);
    switch (object->type) {
    case 4: ((FEListBox *)object)->Update((float)elapsed); break;
    case 5: UpdateGroup((FEGroup *)object, elapsed); break;
    case 6: ((FECodeListBox *)object)->Update((float)elapsed); break;
    case 7: if (executing) ((FEMovie *)object)->unknown_58 += elapsed; break;
    }
    if (executing == 1 && oldTime == script->time && oldTime == (long)script->unknown_c + 1 && !(object->flags & 0x400000))
        object->flags &= uHoldDirtyFlags | 0xfe7fffff;
    object->flags &= uHoldDirtyFlags | 0xffbfffff;
    if (object->flags & 0x1c00000) object->flags |= 0x2000000;
}
#endif
