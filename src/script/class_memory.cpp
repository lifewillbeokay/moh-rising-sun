#include "ScriptRuntime.h"
int BSUtilComputeClassMemoryRequirements(BSClass_struct *scriptClass) {
    int size = 0;
    for (int i = 0; i < scriptClass->levelCount; ++i) {
        size += sizeof(BSMachineThread_struct);
        size += scriptClass->levels[i].contextWordCount * 4;
    }
    return size;
}
