// A fragment of the memory-card library's public interface (public.cpp): the trc file calls fill in a task and hand it to its manager.
#include "RealmcPublic.h"
namespace Realmc {
void GCInterface::TrcLoadFile(const CardID &id, const FileInfo &file) {
    MUTEX_lock(mMutex);
    CardID card = id;
    TaskTrcFile *task = &mTaskTrcLoadFile;
    task->Start();
    task->Card() = card;
    task->info = file;
    task->manager->StartTask(task);
    MUTEX_unlock(mMutex);
}

void GCInterface::TrcSaveFile(const CardID &id, const FileInfo &file) {
    MUTEX_lock(mMutex);
    CardID card = id;
    TaskTrcFile *task = &mTaskTrcSaveFile;
    task->Start();
    task->Card() = card;
    task->info = file;
    task->saveName = task->info.name;
    task->manager->StartTask(task);
    MUTEX_unlock(mMutex);
}

void GCInterface::TrcListFiles(const CardID &id, const FileInfo &file) {
    MUTEX_lock(mMutex);
    CardID card = id;
    TaskTrcFile *task = &mTaskTrcListFiles;
    task->Start();
    task->Card() = card;
    task->info = file;
    task->fileCount = 4;
    task->manager->StartTask(task);
    MUTEX_unlock(mMutex);
}

void GCInterface::TrcDeleteFile(const CardID &id, const FileInfo &file) {
    MUTEX_lock(mMutex);
    CardID card = id;
    TaskTrcFile *task = &mTaskTrcDeleteFile;
    task->Start();
    task->Card() = card;
    task->info = file;
    task->manager->StartTask(task);
    MUTEX_unlock(mMutex);
}
}
