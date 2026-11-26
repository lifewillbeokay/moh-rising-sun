// AI-assisted reconstruction; see docs/research/card-trc.md.
#include "RealmcTrcUpdate.h"
namespace Realmc {
#ifdef DECOMP_COMPARE
void GCInterface::UpdateTaskTrcStartGame() {
    TaskTrcStart &task = mTaskTrcStartGame;
    if (!mMsgTimer.Ready()) return;
    switch (task.state) {
    case 0:
        task.Next(99); task.mounted = 0;
        // Fall through to mount.
    case 99:
        if (!task.step) {
            if (task.mounted) { mpDriver->Unmount(task.card); task.mounted = 0; }
            task.step = 103;
            mTaskTrcMount.Start(task.card, task.id);
        } else if (task.step == 103) {
            task.finalResult = mTaskTrcMount.result;
            if (mTaskTrcMount.result == 0 || mTaskTrcMount.result == 3) {
                task.mounted = 1; task.card = mTaskTrcMount.card; task.Next(24);
            } else { task.mounted = 0; task.Next(121); }
        }
        break;
    case 24:
        if (!task.step) {
            task.step = 25;
            mTaskTrcCheckSpace.Start(task.card, task.info, task.id, true);
        } else if (task.step == 25) {
            task.Next(121); task.finalResult = mTaskTrcCheckSpace.result;
            int choice = mTaskTrcCheckSpace.choice;
            if (choice == 1 || ((choice == 0 || choice == 4 || choice == 16) && mTaskTrcCheckSpace.result == 3))
                task.Next(99);
        }
        break;
    case 121:
        if (task.mounted) { mpDriver->Unmount(task.card); task.mounted = 0; }
        task.End();
        mTaskMsg.Set(1, task.finalResult);
        break;
    }
}

void GCInterface::UpdateTaskTrcCardExists() {
    switch (mTaskTrcCardExists.state) {
    case 0:
        mTaskTrcCardExists.mounted = 0;
        // Fall through to mount.
    case 99:
        mTaskTrcCardExists.state = 103;
        mTaskTrcCardExists.step = 0;
        mTaskTrcMount.Start(mTaskTrcCardExists.card, mTaskTrcCardExists.id);
        break;
    case 103:
        mTaskTrcCardExists.mounted = mTaskTrcMount.result == 0 || mTaskTrcMount.result == 3;
        mTaskTrcCardExists.result = mTaskTrcMount.result;
        mTaskTrcCardExists.state = 122;
        mTaskTrcCardExists.step = 0;
        // Fall through to completion.
    case 122:
        if (mTaskTrcCardExists.mounted) {
            mpDriver->Unmount(mTaskTrcCardExists.card);
            mTaskTrcCardExists.mounted = 0;
        }
        mTaskMsg.Set(2, mTaskTrcCardExists.result);
        mTaskMsg.details.card = mTaskTrcCardExists.card;
        mTaskTrcCardExists.manager->EndTask(&mTaskTrcCardExists);
        break;
    }
}

void GCInterface::UpdateTaskTrcGetCardInfo() {
    switch (mTaskTrcGetCardInfo.state) {
    case 0:
        mTaskTrcGetCardInfo.mounted = 0;
        // Fall through to mount.
    case 99:
        mTaskTrcGetCardInfo.state = 103;
        mTaskTrcGetCardInfo.step = 0;
        mTaskTrcMount.Start(mTaskTrcGetCardInfo.card, mTaskTrcGetCardInfo.id);
        break;
    case 103:
        mTaskTrcGetCardInfo.result = mTaskTrcMount.result;
        if (mTaskTrcMount.result != 0 && mTaskTrcMount.result != 3) {
            mTaskTrcGetCardInfo.state = 122;
            mTaskTrcGetCardInfo.step = 0;
            mTaskTrcGetCardInfo.mounted = 0;
            break;
        }
        mTaskTrcGetCardInfo.mounted = 1;
        mTaskTrcGetCardInfo.state = 21;
        mTaskTrcGetCardInfo.step = 0;
        // Fall through to query.
    case 21:
        if (!mpDriver->GetFreeCardSpace(mTaskTrcGetCardInfo.card,
                &mTaskTrcGetCardInfo.freeBytes, &mTaskTrcGetCardInfo.freeFiles))
            mTaskTrcGetCardInfo.result = 0;
        mTaskTrcGetCardInfo.state = 122;
        mTaskTrcGetCardInfo.step = 0;
        // Fall through to completion.
    case 122:
        if (mTaskTrcGetCardInfo.mounted) {
            mpDriver->Unmount(mTaskTrcGetCardInfo.card);
            mTaskTrcGetCardInfo.mounted = 0;
        }
        mTaskMsg.Set(3, mTaskTrcGetCardInfo.result);
        mTaskMsg.details.card = mTaskTrcGetCardInfo.infoCard;
        mTaskMsg.details.freeBytes = mTaskTrcGetCardInfo.freeBytes;
        mTaskMsg.details.freeFiles = mTaskTrcGetCardInfo.freeFiles;
        mTaskTrcGetCardInfo.manager->EndTask(&mTaskTrcGetCardInfo);
        break;
    }
}

void GCInterface::UpdateTaskTrcLoadFile() {
    TaskTrcLoad &task = mTaskTrcLoadFile;
    if (!mMsgTimer.Ready()) return;
    switch (task.state) {
    case 0:
        task.Next(99); task.mounted = 0;
        // Fall through to the initial mount.
    case 99:
        if (!task.step) {
            task.result = 27;
            if (task.mounted) { mpDriver->Unmount(task.card); task.mounted = 0; }
            task.step = 103;
            mTaskTrcMount.Start(task.card, 4);
        } else if (task.step == 103) {
            if (mTaskTrcMount.result != 0 && mTaskTrcMount.result != 3) {
                task.Next(121); task.result = mTaskTrcMount.result; task.mounted = 0;
            } else { task.Next(98); task.mounted = 1; }
        }
        break;
    case 98:
        if (CheckCard(task.card) == 3) { task.Next(116); return; }
        if (!task.step) {
            mTaskMsg.Prompt(23, 1);
            mTaskMsg.args[1] = mTaskMsg.LcGetSlotString(task.card.slot);
            Options1(mTaskMsg, 512, task.id);
            task.step = 95; mUserMsg = UserMessage_None;
        } else if (task.step == 95 && mUserMsg) task.Next(79);
        break;
    case 79: {
        OpenFileDescriptor *file = 0;
        task.opened = 0;
        int result = mpDriver->OpenFile(task.card, &task.info, (FileOpenMode)1, &file);
        switch (result) {
        case 0:
            mTaskMsg.Set(13, 0); task.opened = 1; task.unknown20 = (int)file;
            if (mpDriver->GetOpenFileSize(file) != -1) {
                task.Next(75); mTaskMsg.result = 0;
                mTaskMsg.details.file = task.Handle(); mTaskMsg.details.fileName = task.info.name;
            } else { task.Next(112); task.result = 14; }
            break;
        case 4: task.Next(121); task.result = 10; break;
        case 6: task.Next(112); task.result = 14; break;
        default: task.Next(112); task.result = 1; break;
        }
        break;
    }
    case 75:
        if (!task.step) {
            mTaskMsg.Prompt(24, 1);
            mTaskMsg.args[1] = mTaskMsg.LcGetSlotString(task.card.slot);
            Options1(mTaskMsg, 0, task.id);
            mMsgTimer.Start(); task.step = 74;
        } else if (task.step == 74) {
            task.step = 75; mTaskMsg.Set(26, 0);
            mReadFailed = 0; mUserMsg = UserMessage_None;
        } else if (task.step == 75) {
            if (mReadFailed) { task.Next(112); task.result = 15; }
            else if (mUserMsg == 2) task.Next(77);
        }
        break;
    case 77:
        if (!task.step) {
            task.step = 78; mTaskMsg.Set(28, 0); mUserMsg = UserMessage_None;
        } else if (task.step == 78) {
            if (mUserMsg == 6) { task.Next(121); task.result = 0; }
            else if (mUserMsg == 7) { task.Next(112); task.result = 14; }
        }
        break;
    case 112:
        if (CheckCard(task.card) == 3) { task.Next(116); return; }
        if (!task.step) {
            if (task.opened) { mpDriver->CloseFile(task.Handle()); task.opened = 0; }
            if (task.result == 14) {
                mTaskMsg.Prompt(22, 3);
                mTaskMsg.args[1] = task.info.Label(); mTaskMsg.args[2] = task.info.text28;
                mTaskMsg.args[3] = mTaskMsg.LcGetSlotString(task.card.slot);
                Options3(mTaskMsg, 37, task.id);
            } else {
                mTaskMsg.Prompt(25, 1);
                mTaskMsg.args[1] = mTaskMsg.LcGetSlotString(task.card.slot);
                Options1(mTaskMsg, 5, task.id);
            }
            task.step = 113; mUserMsg = UserMessage_None;
        } else if (task.step == 113 && mUserMsg) {
            switch (Choice(task.result == 14 ? 37 : 5, task.id)) {
            case 1: task.Next(99); break;
            case 4: task.Next(121); task.result = 20; break;
            case 32: task.Next(85); break;
            }
        }
        break;
    case 85:
        if (CheckCard(task.card) == 3) { task.Next(116); return; }
        if (!task.step) {
            mTaskMsg.Prompt(28, 0); mTaskMsg.LC_SetOptions(36, 6, 0);
            task.step = 86; mUserMsg = UserMessage_None;
        } else if (task.step == 86 && mUserMsg) {
            int choice = Choice(36, task.id);
            if (choice == 4) { task.Next(121); task.result = 20; }
            else if (choice == 32) task.Next(87);
        }
        break;
    case 87:
        mTaskMsg.Prompt(29, 1);
        mTaskMsg.args[1] = mTaskMsg.LcGetSlotString(task.card.slot);
        Options1(mTaskMsg, 0, task.id); mMsgTimer.Start();
        task.driverResult = mpDriver->DeleteFile(task.card, &task.info);
        if (!task.driverResult) { task.Next(121); task.result = 21; }
        else { task.Next(114); task.result = task.driverResult == 5 ? 5 : 1; }
        break;
    case 114:
        if (CheckCard(task.card) == 3) { task.Next(116); return; }
        if (!task.step) {
            mTaskMsg.Prompt(task.result == 5 ? 14 : 30, 1);
            mTaskMsg.args[1] = mTaskMsg.LcGetSlotString(task.card.slot);
            Options1(mTaskMsg, 5, task.result == 5 ? task.id : 6);
            task.step = 115; mUserMsg = UserMessage_None;
        } else if (task.step == 115 && mUserMsg) {
            if (Choice(5, task.id) == 4) { task.Next(121); task.result = 1; }
            else task.Next(99);
        }
        break;
    case 116:
        if (!task.step) {
            mpDriver->Unmount(task.card); task.mounted = 0; task.step = 103;
            mTaskTrcMount.Start(task.card, task.id);
        } else if (task.step == 103) {
            task.Next(121);
            if (mTaskTrcMount.result != 0 && mTaskTrcMount.result != 3) {
                task.result = 20; task.mounted = 0;
            } else { task.result = 24; task.mounted = 1; }
        }
        break;
    case 121:
        if (task.opened) {
            task.opened = 0;
            if (mpDriver->CloseFile(task.Handle())) task.Next(112);
        }
        // Completion still runs when CloseFile changes the state above.
        if (task.mounted) { mpDriver->Unmount(task.card); task.mounted = 0; }
        mTaskMsg.Set(4, task.result); task.End();
        break;
    }
}

void GCInterface::UpdateTaskTrcSaveFile() {
    TaskTrcSave &task = mTaskTrcSaveFile;
    if (!mMsgTimer.Ready()) return;
    switch (task.state) {
    case 0:
        task.Next(99); task.mounted = 0;
        // Fall through to mount.
    case 99:
        if (!task.step) {
            task.result = 27;
            if (task.mounted) { mpDriver->Unmount(task.card); task.mounted = 0; }
            task.step = 103; mTaskTrcMount.Start(task.card, task.id);
        } else if (task.step == 103) {
            if (mTaskTrcMount.result != 0 && mTaskTrcMount.result != 3) {
                task.Next(121); task.result = mTaskTrcMount.result; task.mounted = 0;
            } else { task.Next(24); task.mounted = 1; }
        }
        break;
    case 24:
        if (!task.step) {
            task.step = 25;
            mTaskTrcCheckSpace.Start(task.card, task.info, task.id, false);
        } else if (task.step == 25) {
            switch (mTaskTrcCheckSpace.choice) {
            case 1: task.Next(99); task.result = mTaskTrcCheckSpace.result; break;
            case 0:
                task.Next(mTaskTrcCheckSpace.result == 3 ? 116 : mTaskTrcCheckSpace.exists ? 60 : 108);
                task.result = mTaskTrcCheckSpace.result;
                break;
            case 4: task.Next(121); task.result = 20; break;
            default: task.Next(121); task.result = 1; break;
            }
        }
        break;
    case 60:
        if (CheckCard(task.card) == 3) { task.Next(116); return; }
        if (!task.step) {
            mTaskMsg.Prompt(18, 4);
            mTaskMsg.args[1] = task.info.Label(); mTaskMsg.args[2] = task.info.text24;
            mTaskMsg.args[3] = task.info.text28;
            mTaskMsg.args[4] = mTaskMsg.LcGetSlotString(task.card.slot);
            Options4(mTaskMsg, 12, task.id); task.step = 61; mUserMsg = UserMessage_None;
        } else if (task.step == 61 && mUserMsg) {
            int choice = Choice(12, task.id);
            if (choice == 4) { task.Next(121); task.result = 20; }
            else if (choice == 8) task.Next(108);
        }
        break;
    case 108:
        if (CheckCard(task.card) == 3) { task.Next(116); return; }
        if (!task.step) {
            mTaskMsg.Prompt(19, 1); mTaskMsg.args[1] = mTaskMsg.LcGetSlotString(task.card.slot);
            Options1(mTaskMsg, 512, task.id); task.step = 95; mUserMsg = UserMessage_None;
        } else if (task.step == 95 && mUserMsg) task.Next(79);
        break;
    case 79:
        if (!task.step) {
            mTaskMsg.Prompt(20, 1); mTaskMsg.args[1] = mTaskMsg.LcGetSlotString(task.card.slot);
            Options1(mTaskMsg, 0, task.id); mMsgTimer.Start(); task.step = 79;
        } else if (task.step == 79) {
            if (mTaskTrcStartGame.useTemporary) task.info.name = mTaskTrcStartGame.temporaryName;
            task.driverResult = mpDriver->OpenFile(task.card, &task.info, (FileOpenMode)512, &task.handle);
            if (!task.driverResult) {
                task.Next(66); task.result = 0; task.opened = 1;
                mTaskMsg.Set(13, 0); mTaskMsg.details.file = task.handle;
                mTaskMsg.details.fileName = task.info.name;
            } else {
                task.result = task.driverResult == 5 ? 5 : 1;
                if (mTaskTrcStartGame.useTemporary) {
                    task.info.name = mTaskTrcStartGame.temporaryName;
                    mpDriver->DeleteFile(task.card, &task.info);
                }
                task.Next(121);
            }
        }
        break;
    case 66:
        if (!task.step) {
            task.step = 67; mTaskMsg.Set(27, 0); mWriteFailed = 0; mUserMsg = UserMessage_None;
        } else if (task.step == 67) {
            if (mWriteFailed) task.result = 1;
            else if (mUserMsg == 1) task.result = 0;
            else return;
            task.Next(121);
        }
        break;
    case 121:
        if (task.opened) { mpDriver->CloseFile(task.handle); task.opened = 0; }
        if (!task.result) {
            if (mTaskTrcStartGame.useTemporary) {
                task.info.name = task.originalName; mpDriver->DeleteFile(task.card, &task.info);
                task.info.name = mTaskTrcStartGame.temporaryName;
                mpDriver->RenameFile(task.card, &task.info, task.originalName);
            }
            task.Next(122);
        } else if (task.result == 20 || task.result == 24) task.Next(122);
        else task.Next(69);
        break;
    case 69:
        if (CheckCard(task.card) == 3) { task.Next(116); return; }
        if (!task.step) {
            if (task.driverResult == 5) task.result = 5;
            mTaskMsg.Prompt(task.driverResult == 5 ? 14 : 21, 1);
            mTaskMsg.args[1] = mTaskMsg.LcGetSlotString(task.card.slot);
            Options1(mTaskMsg, 5, task.id); task.step = 70; mUserMsg = UserMessage_None;
        } else if (task.step == 70 && mUserMsg) {
            task.choice = Choice(5, task.id); task.Next(task.choice == 4 ? 122 : 99);
        }
        break;
    case 116:
        if (!task.step) {
            mpDriver->Unmount(task.card); task.mounted = 0; task.step = 103;
            mTaskTrcMount.Start(task.card, task.id);
        } else if (task.step == 103) {
            task.Next(121);
            if (mTaskTrcMount.result != 0 && mTaskTrcMount.result != 3) {
                task.result = 20; task.mounted = 0;
            } else { task.result = 24; task.mounted = 1; }
        }
        break;
    case 122:
        if (task.mounted) { mpDriver->Unmount(task.card); task.mounted = 0; }
        mTaskMsg.Set(5, task.result); task.End();
        break;
    }
}

void GCInterface::UpdateTaskTrcDeleteFile() {
    TaskTrcDelete &task = mTaskTrcDeleteFile;
    if (!mMsgTimer.Ready()) return;
    switch (task.state) {
    case 0:
        task.Next(99); task.mounted = 0;
        // Fall through to mount.
    case 99:
        if (!task.step) {
            task.result = 27;
            if (task.mounted) { mpDriver->Unmount(task.card); task.mounted = 0; }
            task.step = 103; mTaskTrcMount.Start(task.card, 5);
        } else if (task.step == 103) {
            if (!mTaskTrcMount.result) { task.Next(85); task.mounted = 1; }
            else if (mTaskTrcMount.result == 3) { task.Next(116); task.mounted = 1; }
            else { task.Next(122); task.result = mTaskTrcMount.result; task.mounted = 0; }
        }
        break;
    case 85:
        if (CheckCard(task.card) == 3) { task.Next(116); return; }
        if (!task.step) {
            mTaskMsg.Prompt(28, 0); mTaskMsg.LC_SetOptions(36, task.id, 0);
            task.step = 86; mUserMsg = UserMessage_None;
        } else if (task.step == 86 && mUserMsg) {
            task.choice = Choice(36, task.id);
            if (task.choice == 4) { task.Next(121); task.result = 20; }
            else if (task.choice == 32) task.Next(87);
        }
        break;
    case 87:
        mTaskMsg.Prompt(29, 1); mTaskMsg.args[1] = mTaskMsg.LcGetSlotString(task.card.slot);
        Options1(mTaskMsg, 0, task.id); mMsgTimer.Start();
        task.driverResult = mpDriver->DeleteFile(task.card, &task.info);
        task.Next(121);
        if (task.driverResult == 3 || task.driverResult == 4) task.result = 10;
        else if (task.driverResult == 0 || task.driverResult == 5) task.result = task.driverResult;
        else task.result = 1;
        break;
    case 121:
        task.Next(task.result == 0 || task.result == 20 || task.result == 24 ? 122 : 114);
        break;
    case 114:
        if (CheckCard(task.card) == 3) { task.Next(116); return; }
        if (!task.step) {
            mTaskMsg.Prompt(task.result == 5 ? 14 : 30, 1);
            mTaskMsg.args[1] = mTaskMsg.LcGetSlotString(task.card.slot);
            Options1(mTaskMsg, 5, task.result == 5 ? 5 : task.id);
            task.step = 115; mUserMsg = UserMessage_None;
        } else if (task.step == 115 && mUserMsg) {
            task.choice = Choice(5, task.id);
            if (task.choice == 4) { task.Next(122); task.result = 20; }
            else task.Next(99);
        }
        break;
    case 116:
        if (!task.step) {
            mpDriver->Unmount(task.card); task.mounted = 0; task.step = 103;
            mTaskTrcMount.Start(task.card, task.id);
        } else if (task.step == 103) {
            task.Next(121);
            if (mTaskTrcMount.result != 0 && mTaskTrcMount.result != 3) {
                task.result = 20; task.mounted = 0;
            } else { task.result = 24; task.mounted = 1; }
        }
        break;
    case 122:
        if (task.mounted) { mpDriver->Unmount(task.card); task.mounted = 0; }
        mTaskMsg.Set(6, task.result); task.End();
        break;
    }
}

void GCInterface::UpdateTaskTrcListFiles() {
    if (!mMsgTimer.Ready()) return;
    GCMessage *found;
    switch (mTaskTrcListFiles.state) {
    case 0:
        mTaskTrcListFiles.state = 99;
        mTaskTrcListFiles.step = 0;
        mTaskTrcListFiles.result = 27;
        mTaskTrcListFiles.mounted = 0;
        // Fall through to mount.
    case 99:
        if (mTaskTrcListFiles.step == 0) {
            mTaskTrcListFiles.step = 103;
            mTaskTrcMount.Start(mTaskTrcListFiles.card, 7);
        } else if (mTaskTrcListFiles.step == 103) {
            mTaskTrcListFiles.result = mTaskTrcMount.result;
            mTaskTrcListFiles.step = 0;
            if (mTaskTrcMount.result == 0) {
                mTaskTrcListFiles.mounted = 1;
                mTaskTrcListFiles.state = 12;
            } else mTaskTrcListFiles.state = 121;
        }
        break;
    case 12:
        if (mTaskTrcListFiles.step == 0) {
            found = Find(mTaskTrcListFiles.card, mTaskTrcListFiles.info.name, true);
            if (found->result == 11) {
                mTaskTrcListFiles.state = 121;
                mTaskTrcListFiles.step = 0;
                mTaskTrcListFiles.result = 11;
            } else {
                mTaskMsg = *found;
                mTaskTrcListFiles.step = 11;
            }
        } else if (mTaskTrcListFiles.step == 11) {
            found = Find(mTaskTrcListFiles.card, mTaskTrcListFiles.info.name, false);
            if (found->result == 11) {
                mTaskTrcListFiles.state = 121;
                mTaskTrcListFiles.step = 0;
            } else mTaskMsg = *found;
        }
        break;
    case 121:
        if (mTaskTrcListFiles.mounted) {
            mpDriver->Unmount(mTaskTrcListFiles.card);
            mTaskTrcListFiles.mounted = 0;
        }
        mTaskMsg.Set(7, mTaskTrcListFiles.result);
        mTaskTrcListFiles.manager->EndTask(&mTaskTrcListFiles);
        break;
    }
}

void GCInterface::UpdateTaskTrcMountCard() {
    TaskTrcMount &task = mTaskTrcMount;
    TaskTrcMountOperation &operation = mTaskTrcMountOperation;
    switch (task.state) {
    case 0: task.Next(99); break;
    case 99:
        if (task.mounted) { mpDriver->Unmount(task.card); task.mounted = 0; }
        task.Next(100); operation.Start(task.card, task.parentID);
        break;
    case 100:
        task.mounted = operation.mounted; task.keepMounted = operation.success;
        task.choice = operation.choice; task.card = operation.card;
        if (CheckCard(task.card) == 3) task.removed = 1;
        if (task.choice == 2) {
            mTaskMsg.Prompt(26, 0); mTaskMsg.LC_SetOptions(68, task.parentID, 0);
            task.Next(63); mUserMsg = UserMessage_None;
        } else task.Next(task.choice == 1 ? 99 : task.choice == 4 ? 121 : 101);
        break;
    case 63:
        if (!mUserMsg) {
            if (CheckCard(task.card) == 3) { task.Next(116); task.result = 3; }
            return;
        }
        task.choice = Choice(68, task.parentID);
        if (task.choice != 64) { task.Next(121); return; }
        mTaskMsg.Prompt(27, 1); mTaskMsg.args[1] = mTaskMsg.LcGetSlotString(task.card.slot);
        Options1(mTaskMsg, 0, task.parentID); mMsgTimer.Start();
        task.formatResult = mpDriver->FormatCard(task.card); task.Next(59);
        break;
    case 59:
        if (!mMsgTimer.Ready()) return;
        mMsgTimer.active = 0;
        if (task.formatResult == 3) task.Next(116);
        else if (!task.formatResult) { task.Next(121); task.choice = 0; task.keepMounted = 1; }
        else {
            mTaskMsg.Prompt(14, 1); mTaskMsg.args[1] = mTaskMsg.LcGetSlotString(task.card.slot);
            Options1(mTaskMsg, 5, task.parentID); task.Next(104); mUserMsg = UserMessage_None;
        }
        break;
    case 104:
        if (!mUserMsg) {
            if (CheckCard(task.card) == 3) task.Next(99);
            return;
        }
        task.choice = Choice(5, task.parentID); task.Next(101);
        break;
    case 101: task.Next(task.choice == 1 ? 99 : 121); break;
    case 116:
        mTaskMsg.Prompt(35, 1); task.result = 1;
        mTaskMsg.args[1] = mTaskMsg.LcGetSlotString(task.card.slot);
        Options1(mTaskMsg, 512, task.id); task.Next(117); mUserMsg = UserMessage_None;
        break;
    case 117:
        if (mUserMsg) task.Next(99);
        break;
    case 121:
        if (task.mounted && !task.keepMounted) { mpDriver->Unmount(task.card); task.mounted = 0; }
        if (!task.choice) task.result = task.removed ? 3 : 0;
        else if (task.choice == 4) task.result = 20;
        else if (!operation.foundCard) task.result = operation.wrongDevice ? 6 : 2;
        else if (operation.ioError) task.result = 5;
        else task.result = task.keepMounted ? 1 : 4;
        task.End();
        break;
    }
}

void GCInterface::UpdateTaskTrcMountCardOperation() {
    TaskTrcMountOperation &task = mTaskTrcMountOperation;
    switch (task.state) {
    case 0: {
        unsigned short first = task.card.slot, last = task.card.slot;
        if (task.parentID == 1 && mTaskTrcStartGame.scanBoth) { first = 0; last = 1; }
        for (int retry = 0; retry < 5 && !task.foundCard; retry++) {
            CardID card = {first, 0};
            while (card.slot <= last && !task.foundCard) {
                int result = mpDriver->CardExists(card);
                if (!result) { task.foundCard = 1; task.card.slot = card.slot; }
                else if (result == 2) { task.wrongDevice = 1; task.card.slot = card.slot; }
                card.slot++;
            }
        }
        if (task.wrongDevice) {
            task.driverResult = 2; mTaskMsg.Prompt(12, 1);
            mTaskMsg.args[1] = mTaskMsg.LcGetSlotString(task.card.slot);
            Options1(mTaskMsg, 5, task.parentID); task.Next(105); mUserMsg = UserMessage_None;
        } else if (task.foundCard) task.Next(99);
        else {
            task.driverResult = 3; mTaskMsg.Prompt(11, 1);
            mTaskMsg.args[1] = mTaskMsg.LcGetSlotString(task.card.slot);
            Options1(mTaskMsg, 5, task.parentID); task.Next(105); mUserMsg = UserMessage_None;
        }
        break;
    }
    case 99:
        task.driverResult = mpDriver->Mount(task.card); mUserMsg = UserMessage_None;
        switch (task.driverResult) {
        case 2:
            mTaskMsg.Prompt(12, 1); task.wrongDevice = 1;
            mTaskMsg.args[1] = mTaskMsg.LcGetSlotString(task.card.slot);
            Options1(mTaskMsg, 5, 5); task.Next(106);
            break;
        case 3:
            task.Next(121); task.choice = 1; task.foundCard = 0;
            break;
        case 5: case 16:
            mTaskMsg.Prompt(14, 1); task.ioError = 1;
            mTaskMsg.args[1] = mTaskMsg.LcGetSlotString(task.card.slot);
            Options1(mTaskMsg, 5, 5); task.Next(106);
            break;
        case 6: case 13:
            mTaskMsg.Prompt(13, 1); task.mounted = 1;
            mTaskMsg.args[1] = mTaskMsg.LcGetSlotString(task.card.slot);
            Options1(mTaskMsg, 7, 5); task.Next(106);
            break;
        default:
            task.success = 1; task.Next(121); task.mounted = 1; task.choice = 0;
            break;
        }
        break;
    case 105:
        if (mUserMsg) { task.choice = Choice(5, task.parentID); task.Next(121); }
        else if (task.driverResult == 2 && mpDriver->CardExists(task.card) == 3) {
            task.Next(121); task.choice = 1;
        }
        break;
    case 106:
        if (!mUserMsg) {
            if (mpDriver->CardExists(task.card) == 3) { task.Next(121); task.choice = 1; }
            return;
        }
        switch (task.driverResult) {
        case 2: case 5: case 16: task.choice = Choice(5, task.parentID); break;
        case 6: case 13: task.choice = Choice(7, task.parentID); break;
        }
        task.Next(121);
        // User acknowledgement completes this state immediately.
    case 121: task.End(); break;
    }
}

void GCInterface::UpdateTaskTrcCheckSpace() {
    TaskTrcCheckSpace &task = mTaskTrcCheckSpace;
    const char *originalName = task.info.name;
    switch (task.state) {
    case 0:
        task.choice = 0;
        if (task.parentID == 1) {
            task.totalFiles = mTaskTrcStartGame.requiredFiles;
            task.totalBlocks = mTaskTrcStartGame.requiredBlocks;
        } else {
            task.totalBlocks = mpDriver->GetFileBlocks(task.card, &task.info, 0); task.totalFiles = 1;
        }
        task.neededBlocks = task.totalBlocks; task.neededFiles = task.totalFiles;
        if (mTaskTrcStartGame.useTemporary) {
            task.Next(92); task.temporaryBlocks = mTaskTrcStartGame.temporaryBlocks; task.temporaryFiles = 1;
        } else task.Next(18);
        break;
    case 92:
        task.info.name = mTaskTrcStartGame.temporaryName;
        mpDriver->DeleteFile(task.card, &task.info); task.Next(18);
        // Fall through to query the newly available space.
    case 18:
        task.info.name = originalName;
        if (mpDriver->GetFreeCardSpace(task.card, (int *)&task.freeBlocks, (int *)&task.freeFiles)) {
            task.Next(121); task.choice = 1; return;
        }
        task.Next(10);
        // Fall through to the first matching file.
    case 10:
        task.choice = 128; task.found = Find(task.card, task.info.name, true);
        task.Next(task.found->result == 11 ? 24 : 13);
        break;
    case 11:
        task.found = Find(task.card, task.info.name, false);
        task.Next(task.found->result == 11 ? 24 : 13);
        break;
    case 13:
        if (!strcmp(task.found->details.foundName, task.info.name)) task.exists = 1;
        if (task.enable) {
            mTaskMsg.id = 23; mTaskMsg.details = task.found->details;
            task.Next(5); mUserMsg = UserMessage_None;
        } else if (task.exists) task.Next(109);
        break;
    case 5:
        if (!mUserMsg) return;
        task.choice = Choice(384, task.parentID); task.Next(task.choice == 128 ? 109 : 11);
        break;
    case 109: {
        unsigned int sector = mpDriver->GetSectorSize(task.card);
        task.reclaimedBlocks += (task.found->details.foundSize + sector - 1) / sector;
        task.reclaimedFiles++; task.Next(11);
        break;
    }
    case 24: {
        task.choice = 0;
        task.neededBlocks = task.reclaimedBlocks < task.neededBlocks ? task.neededBlocks - task.reclaimedBlocks : 0;
        task.neededFiles = task.reclaimedFiles < task.neededFiles ? task.neededFiles - task.reclaimedFiles : 0;
        if (CheckCard(task.card) == 3) { task.Next(121); task.result = 3; }
        // The retail code continues the capacity calculation after the card check.
        bool enough = false;
        if (task.neededBlocks <= task.freeBlocks && task.neededFiles <= task.freeFiles) {
            if (!mTaskTrcStartGame.useTemporary) enough = true;
            else {
                task.freeBlocks -= task.neededBlocks; task.freeFiles -= task.neededFiles;
                enough = task.temporaryBlocks <= task.freeBlocks && task.temporaryFiles <= task.freeFiles;
            }
        }
        if (enough) { task.Next(121); task.result = 0; return; }
        if (task.info.unknown2c) {
            mTaskMsg.Prompt(17, 2); mTaskMsg.args[1] = mTaskMsg.LcGetSlotString(task.card.slot);
            mTaskMsg.args[2] = task.info.Label(); Options2(mTaskMsg, 21, task.parentID);
        } else if (!mTaskTrcStartGame.useTemporary) {
            mTaskMsg.Prompt(15, 4); mTaskMsg.args[1] = mTaskMsg.LcGetSlotString(task.card.slot);
            mTaskMsg.args[2] = task.info.Label(); mTaskMsg.args[3] = task.totalFiles;
            mTaskMsg.args[4] = task.totalBlocks; OptionsSpace(mTaskMsg, 21, task.parentID);
        } else {
            mTaskMsg.Prompt(16, 7); mTaskMsg.args[1] = mTaskMsg.LcGetSlotString(task.card.slot);
            mTaskMsg.args[2] = task.info.Label(); mTaskMsg.args[3] = task.totalFiles;
            mTaskMsg.args[4] = task.totalBlocks; mTaskMsg.args[5] = task.info.text24;
            mTaskMsg.args[6] = task.temporaryFiles; mTaskMsg.args[7] = task.temporaryBlocks;
            OptionsTemporary(mTaskMsg, 21, task.parentID);
        }
        task.Next(28); mUserMsg = UserMessage_None;
        break;
    }
    case 28:
        if (!mUserMsg) {
            if (CheckCard(task.card) == 3) { task.Next(121); task.result = 3; }
            return;
        }
        task.choice = Choice(21, task.parentID); task.Next(121);
        // Fall through to completion.
    case 121:
        switch (task.choice) {
        case 0: break;
        case 4: task.result = 9; break;
        case 16: task.result = 26; break;
        default: task.result = 1; break;
        }
        task.End();
        break;
    }
}

#endif
}
