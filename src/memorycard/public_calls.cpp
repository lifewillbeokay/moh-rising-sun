// A fragment of the memory-card library's public interface (public.cpp): each call fills in a static task under the mutex and hands it to the task's manager.
#include "RealmcPublic.h"
namespace Realmc {
void GCInterface::CardExists(const CardID &id) {
    MUTEX_lock(mMutex);
    CardID card = id;
    mTaskCardExists.Init(true);
    mTaskCardExists.Card() = card;
    mTaskCardExists.args[0] = 0;
    mTaskCardExists.manager->StartTask(&mTaskCardExists);
    MUTEX_unlock(mMutex);
}

void GCInterface::GetCardInfo(const CardID &id) {
    MUTEX_lock(mMutex);
    CardID card = id;
    mTaskGetCardInfo.Init(true);
    mTaskGetCardInfo.Card() = card;
    mTaskGetCardInfo.manager->StartTask(&mTaskGetCardInfo);
    MUTEX_unlock(mMutex);
}

void GCInterface::Mount(const CardID &id) {
    MUTEX_lock(mMutex);
    CardID card = id;
    mTaskMount.Init(true);
    mTaskMount.Card() = card;
    mTaskMount.manager->StartTask(&mTaskMount);
    MUTEX_unlock(mMutex);
}

void GCInterface::Unmount(const CardID &id) {
    MUTEX_lock(mMutex);
    CardID card = id;
    mTaskUnmount.Init(true);
    mTaskUnmount.Card() = card;
    mTaskUnmount.manager->StartTask(&mTaskUnmount);
    MUTEX_unlock(mMutex);
}

void GCInterface::OpenFile(const CardID &id, const FileInfo &info, FileOpenMode mode) {
    MUTEX_lock(mMutex);
    CardID card = id;
    mTaskOpen.Init(true);
    mTaskOpen.Card() = card;
    mTaskOpen.args[0] = (int)&info;
    mTaskOpen.args[1] = mode;
    mTaskOpen.manager->StartTask(&mTaskOpen);
    MUTEX_unlock(mMutex);
}

void GCInterface::CloseFile(OpenFileDescriptor *file) {
    MUTEX_lock(mMutex);
    mTaskClose.Init(true);
    mTaskClose.args[0] = (int)file;
    mTaskClose.manager->StartTask(&mTaskClose);
    MUTEX_unlock(mMutex);
}

void GCInterface::Read(OpenFileDescriptor *file, void *buffer, int size) {
    MUTEX_lock(mMutex);
    mTaskRead.Init(true);
    mTaskRead.args[0] = (int)file;
    mTaskRead.args[1] = (int)buffer;
    mTaskRead.args[2] = size;
    mTaskRead.args[3] = 0;
    mTaskRead.manager->StartTask(&mTaskRead);
    MUTEX_unlock(mMutex);
}

void GCInterface::Write(OpenFileDescriptor *file, void *buffer, int size) {
    MUTEX_lock(mMutex);
    mTaskWrite.Init(true);
    mTaskWrite.args[0] = (int)file;
    mTaskWrite.args[1] = (int)buffer;
    mTaskWrite.args[2] = size;
    mTaskWrite.args[3] = 0;
    mTaskWrite.manager->StartTask(&mTaskWrite);
    MUTEX_unlock(mMutex);
}

void GCInterface::Seek(OpenFileDescriptor *file, int offset, SeekFrom from) {
    MUTEX_lock(mMutex);
    mTaskSeek.Init(true);
    mTaskSeek.args[0] = (int)file;
    mTaskSeek.args[1] = offset;
    mTaskSeek.args[2] = from;
    mTaskSeek.manager->StartTask(&mTaskSeek);
    MUTEX_unlock(mMutex);
}

void GCInterface::Flush(OpenFileDescriptor *file) {
    MUTEX_lock(mMutex);
    mTaskFlush.Init(true);
    mTaskFlush.args[0] = (int)file;
    mTaskFlush.manager->StartTask(&mTaskFlush);
    MUTEX_unlock(mMutex);
}

void GCInterface::DeleteFile(const CardID &id, const char *, const char *name) {
    MUTEX_lock(mMutex);
    CardID card = id;
    mTaskDelete.Init(true);
    mTaskDelete.Card() = card;
    strncpy(mTaskDelete.name, name, 36);
    mTaskDelete.manager->StartTask(&mTaskDelete);
    MUTEX_unlock(mMutex);
}

void GCInterface::SetFileAttribute(const CardID &id, const char *, const char *name, FileAttribute attribute) {
    MUTEX_lock(mMutex);
    CardID card = id;
    mTaskSetAttribute.Init(true);
    mTaskSetAttribute.Card() = card;
    strncpy(mTaskSetAttribute.name, name, 36);
    mTaskSetAttribute.result = attribute;
    mTaskSetAttribute.manager->StartTask(&mTaskSetAttribute);
    MUTEX_unlock(mMutex);
}

void GCInterface::FindFile(const CardID &id, const char *, const char *name) {
    MUTEX_lock(mMutex);
    CardID card = id;
    mTaskFind.Init(true);
    mTaskFind.Card() = card;
    strncpy(mTaskFind.name, name, 36);
    mTaskFind.result = 0;
    mTaskFind.manager->StartTask(&mTaskFind);
    MUTEX_unlock(mMutex);
}
}
