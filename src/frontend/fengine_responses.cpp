// AI-assisted GR8E69 reconstruction; see docs/research/feng-lifecycle.md.
#ifdef DECOMP_COMPARE
#include "FEngine.h"
#include "FEMessageResponse.h"
#include "FEScript.h"
void FEngine::ProcessResponses(FEMessageResponse *list, FEObject *object, FEPackage *package, unsigned long mask) {
    unsigned long count = list->count;
    for (unsigned long i = 0; i < count; ++i) {
        FEResponse &response = list->responses[i];
        unsigned long value = (unsigned long)response.param;
        switch (response.id) {
        case 0: {
            FEScript *script = object->FindScript(value);
            if (script) { object->SetCurrentScript(script); script->time = 0; }
            break;
        }
        case 1: {
            FEObject *target = (FEObject *)response.value;
            if (response.value != -4 && response.value != -1) target = package->FindObjectByGUID(response.value);
            QueueMessage(value, object, package, target, mask); break;
        }
        case 2: QueueMessage(value, object, package, (FEObject *)-1, mask); break;
        case 3: QueueMessage(value, object, package, (FEObject *)-5, mask); break;
        case 0x100: {
            FEObject *target = value ? package->FindObjectByGUID(value) : 0;
            if (target || !value) package->SetCurrentButton(this, target, target != 0);
            break;
        }
        case 0x101: resetRequested = value == 1; break;
        case 0x102: RecordLastPackageButton(package->hash, package->currentButton ? package->currentButton->guid : 0); break;
        case 0x103: {
            FEObject *target = 0;
            unsigned long guid = RecallLastPackageButton(package->hash);
            if (guid) target = package->FindObjectByGUID(guid);
            if (!target && value) target = package->FindObjectByGUID(value);
            if (target || !value) package->SetCurrentButton(this, target, target != 0);
            break;
        }
        case 0x104: break;
        case 0x105: QueuePackageUserTransfer(package, true, mask); break;
        case 0x106: QueuePackageUserTransfer(package, true, 255); break;
        case 0x107: QueuePackageUserTransfer(package, false, mask); break;
        case 0x108: QueuePackageUserTransfer(package, false, 255); break;
        case 0x200: QueuePackageSwitch(response.param, package); break;
        case 0x201: QueuePackagePush(response.param, package, package->controlMask); break;
        case 0x202:
            for (unsigned long bit = 0; bit < 8; ++bit)
                if (mask & (1 << bit)) QueuePackagePush(response.param, package, 1 << bit);
            break;
        case 0x203: QueuePackagePop(package); break;
        case 0x204: QueuePackagePush(response.param, package, 0); break;
        case 0x2c0: RecordPackageMarker(package->name); break;
        case 0x2c1: {
            char *name = RecallPackageMarker();
            if (name) QueuePackageSwitch(name, package);
            break;
        }
        case 0x2c2: ClearPackageMarkers(); break;
        case 0x300: if (object->currentScript->hash != value) i = list->FindConditionBranchTarget(i); break;
        case 0x301: if (object->currentScript->hash == value) i = list->FindConditionBranchTarget(i); break;
        case 0x500: i = list->FindConditionBranchTarget(i); break;
        }
    }
}
#endif
