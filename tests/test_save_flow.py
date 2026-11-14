"""Original/native PPC checks for save flow and GameCube card operations.

Pinned original and optional Unicorn required. Game, audio, timing and card I/O
services are stubbed; these exercise state transitions, not gameplay or real cards.
"""
import binascii
import itertools
import struct
import unittest
from pathlib import Path
import test_explosion_grenade as ppc
from test_shortlist_partials import Run
from test_frontend_screens import addr, private, memory, signed, OBJ, PKG, ENG, BUTTON, CORE, EXTRA, VT
from test_feng_animation_lists import copy_stubs

ROOT = Path(__file__).resolve().parents[1]
EXACT = {'mc_display_reply', 'mc_wait', 'mc_cache_device', 'mcgc_results'}
CARD, MESSAGE, DATA, PROFILE, HEAP = 0x10100000, 0x10110000, 0x10120000, 0x10140000, 0x10160000


def directory(unit):
    return ROOT / ('build/reconstruction/units' if unit in EXACT else 'build/comparisons') / (unit if unit in EXACT else 'compare_' + unit)


def glob(name): return private('MemoryCard.cpp', name)
def gcglob(name): return private('MemoryCardGC.cpp', name)


def stub(r, name, fn):
    r.at(name, fn)
    # Calls to another function in the partial unit use its native entry.
    r.hooks[r.entries[name]['address']] = fn


def base(r):
    copy_stubs(r); r.string_stubs()
    r.wr(OBJ + 4, VT); r.wr(addr('g_GameSettings'), CORE)
    r.wr(glob('g_bMinimalInteraction'), 0); r.wr(glob('g_bWaitingForReply'), 0)
    r.wr(glob('g_bIsBusy'), 0); r.wr(glob('g_pScratchpad'), DATA)
    r.wr(glob('g_uGameDataSize'), 0xc598)
    r.at('Instance__16CMemoryCardPopup', lambda: PKG)
    for name in ['Push', 'Pop', 'Wait']:
        r.at(name + '__14CSaveGamePopup', lambda name=name: r.tracecall(name, r.rr(3)))
    r.at('CancelReplyRequest__11CMemoryCard', lambda: r.tracecall('cancel', r.rr(3)))
    r.at('THREAD_yield', lambda: r.tracecall('yield', r.rr(3)))


def reply(r, case):
    minimal, adjust = case; base(r)
    r.wr(glob('g_bMinimalInteraction'), minimal)
    r.at('RequestReply__16CMemoryCardPopup', lambda: r.tracecall('request', r.rr(3)))
    r.slot(VT, 0x80, lambda: (r.tracecall('default', r.rr(3)), 3)[1], adjust)
    r.slot(VT, 0x78, lambda: r.tracecall('reply', r.rr(3), r.rr(4)), adjust)
    r.call('RequestReply__11CMemoryCard', OBJ)
    assert r.rd(glob('g_bWaitingForReply')) == (not minimal)
    assert r.trace == ([('default', OBJ + adjust), ('reply', OBJ + adjust, 3)] if minimal else [('request', PKG)])
    return r.trace


def wait(r, case):
    minimal, waiting, replied, adjust = case; base(r)
    r.wr(glob('g_bMinimalInteraction'), minimal); r.wr(glob('g_bWaitingForReply'), waiting); r.wr(PKG + 0x24, replied)
    r.at('GetReply__16CMemoryCardPopup', lambda: (r.tracecall('get', r.rr(3)), 2)[1])
    r.slot(VT, 0x78, lambda: r.tracecall('reply', r.rr(3), r.rr(4)), adjust)
    r.call('Wait__11CMemoryCard', OBJ)
    assert r.trace[-1] == ('yield', 1)
    assert r.rd(glob('g_bWaitingForReply')) == (0 if not minimal and waiting and replied else waiting)
    return r.trace


def display(r, ignored):
    base(r)
    r.at('Display__16CMemoryCardPopupPCUwN41', lambda: r.tracecall('display', *[r.rr(i) for i in range(3, 9)]))
    r.call('Display__11CMemoryCardPCUwN41', OBJ, EXTRA, 0, BUTTON, CORE, 0)
    assert r.trace == [('display', PKG, EXTRA, 0, BUTTON, CORE, 0)]
    return r.trace


def cache(r, case):
    states, adjust = case; base(r); r.wr(CARD, VT + 0x200); r.wr(PKG + 0x18, 7)
    queue = list(states)
    r.slot(VT + 0x200, 0x48, lambda: r.tracecall('issue', r.rr(3)), adjust)
    r.slot(VT + 0x200, 0x50, lambda: (r.tracecall('check', r.rr(3)), queue.pop(0))[1], adjust)
    r.at('Wait__11CMemoryCard', lambda: r.tracecall('wait', r.rr(3), r.rd(glob('g_bIsBusy'))))
    r.call('Cache__11CMemoryCardPQ211CMemoryCard6Device', OBJ, CARD)
    assert not queue and r.rd(PKG + 0x18) == r.rd(glob('g_bIsBusy')) == 0
    assert len([x for x in r.trace if x[0] == 'wait']) == len(states) - 1
    return r.trace


def crc(r, case):
    length, corrupt, reflection = case; base(r)
    data = bytes((i * 37 + 11) & 255 for i in range(length)); r.u.mem_write(DATA, data + bytes(12))
    r.wr(glob('g_uGameDataSize'), length); r.wr(addr('_14SPlayerProfile.VERSION'), 411)
    def allocate():
        r.tracecall('allocate', r.text(r.rr(3)), r.rr(4), r.rr(5)); return HEAP
    r.at('DWI_alloc', allocate); r.at('__builtin_delete', lambda: r.tracecall('free', r.rr(3)))
    # The false-reflection branch is reached by a controlled CRC callback.
    if not reflection:
        original_reset = addr('Reset__4CCRC')
        def reset():
            r.wr(HEAP + 0x10, 0); r.wr(HEAP + 0x18, 0xffffffff)
        r.hooks[original_reset] = reset
    checksum = binascii.crc32(data)
    r.wr(DATA + length, checksum ^ (1 if corrupt == 'crc' else 0), 411 + (corrupt == 'version'))
    value = r.call('ScratchpadDataOkay__C11CMemoryCard', OBJ)
    if reflection: assert value == (corrupt == 'none')
    assert r.trace == [('allocate', 'source/system/crc.h:241', 28, 1024), ('free', HEAP)]
    return value, memory(r, DATA, length + 12), memory(r, HEAP, 28), r.trace


def save_load(r, case):
    load, states, success, declined, written, device = case; base(r)
    contents = bytes((i * 11 + 7) & 255 for i in range(0xc598))
    r.u.mem_write(PROFILE, contents); r.cstring(PROFILE, 'PROFILE'); r.wr(PROFILE + 0xc, device)
    r.u.mem_write(DATA, contents); r.wr(CORE, 6)
    r.at('GetDeviceCount__C11CMemoryCard', lambda: 3)
    r.at('GetDevice__C11CMemoryCardUi', lambda: CARD + 0x100*r.rr(4))
    for i in range(3):
        r.wr(CARD + 0x100*i, VT + 0x200 + 0x100*i)
        r.slot(VT + 0x200 + 0x100*i, 0x18, lambda i=i: int(i == 1))
    r.at('GenerateScratchpadCRC__Fv', lambda: r.tracecall('crc', memory(r, DATA, 0x20)))
    r.at('Wait__11CMemoryCard', lambda: r.tracecall('wait', r.rd(glob('g_bIsBusy'))))
    r.at('Set_Language__13CGameSettings10ELanguages', lambda: r.tracecall('language', r.rr(4)))
    queue = list(states)
    r.slot(VT, 0x40 if load else 0x28, lambda: r.tracecall('issue', r.text(r.rr(4)), r.rr(5)))
    r.slot(VT, 0x48 if load else 0x30, lambda: queue.pop(0))
    r.slot(VT, 0x50 if load else 0x38, lambda: success)
    r.slot(VT, 0x88, lambda: declined); r.slot(VT, 0x90, lambda: written)
    if load:
        result = r.call('Load__11CMemoryCardPCcUiR14SPlayerProfile', OBJ, PROFILE, 2, PROFILE)
        assert result == success
        if success: assert r.rd(PROFILE + 0xc) == 2
    else:
        result = signed(r.call('Save__11CMemoryCardR14SPlayerProfile', OBJ, PROFILE))
        assert result == ((1 if written else 2) if success else (-2 if declined else -1))
        if device == 0xbad: assert r.rd(PROFILE + 0xc) == 1
    assert not queue and r.rd(glob('g_bIsBusy')) == 0
    assert len([t for t in r.trace if t[0] == 'issue']) == 1 + states.count(4)
    return result, memory(r, PROFILE, 0xc598), memory(r, DATA, 0xc598), r.trace


def gcbase(r, waiting=0, adjust=0):
    base(r); r.wr(OBJ + 8, ENG); r.wr(ENG, VT + 0x400)
    r.wr(OBJ + 0x88, 9, 8, 7, EXTRA, CARD, BUTTON, 3)
    r.wr(CORE + 0x5c, 7); r.wr(CARD, VT + 0x800); r.wr(CARD + 4, ENG)
    r.wr(CARD + 8, 0x00010002); r.wr(EXTRA, VT + 0xa00)
    r.wr(gcglob('g_pMCInstance'), OBJ); r.wr(gcglob('g_bVerbose'), 1)
    r.at('Instance__9MohLoader', lambda: CORE)
    r.at('TIMER_elapsedticks', lambda: 123); r.at('TIMER_getfrequency', lambda: 10000)
    r.slot(VT + 0x400, 0xa8, lambda: (r.tracecall('poll', r.rr(3), signed(r.rr(4))), MESSAGE)[1], adjust)
    r.slot(VT + 0x400, 0xb0, lambda: r.tracecall('send', r.rr(3), r.rr(4), r.rr(5)), adjust)
    for slot, name, nargs in [(0x30,'load',2),(0x38,'save',2),(0x58,'info',1),(0x78,'read',3),(0x80,'write',3),(0xa0,'find',3),(0xe8,'mount',1),(0xf0,'unmount',1)]:
        r.slot(VT + 0x400, slot, lambda name=name,nargs=nargs: r.tracecall(name, *[r.rr(i) for i in range(3,4+nargs)]), adjust)
    for name in ['MUSIC_Pause__Fb', 'SOUND_PauseAll__Fb']:
        r.at(name, lambda name=name: r.tracecall(name, r.rr(3)))
    stub(r, 'DisplayTRCMessage__13CMemoryCardGCPCQ26Realmc7Message', lambda: r.tracecall('trc', r.rr(3), r.rr(4)))
    r.at('RequestReply__11CMemoryCard', lambda: r.tracecall('request', r.rr(3), r.rd(OBJ + 0xa0)))
    r.at('WaitingForReply__C11CMemoryCard', lambda: waiting)
    r.at('OSResetSystem', lambda: r.tracecall('reset', r.rr(3), r.rr(4), r.rr(5)))
    r.at('GetScratchpad__C11CMemoryCard', lambda: (r.tracecall('scratch'), DATA)[1])
    r.at('GetScratchpadSize__C11CMemoryCard', lambda: (r.tracecall('size'), 0xc5a0)[1])
    r.at('GetDevice__9CMCDeviceUi', lambda: (r.tracecall('device', r.rr(3)), CARD)[1])
    r.at('sprintf', lambda: (r.tracecall('diagnostic', r.text(r.rr(4)), signed(r.rr(5))), 0)[1])


def poll(r, case):
    operation, msgid, result, trc, waiting, valid, adjust = case; gcbase(r, waiting, adjust)
    r.wr(MESSAGE, msgid, result, trc); r.wr(MESSAGE + 0x54, BUTTON + 0x20)
    r.at('ScratchpadDataOkay__C11CMemoryCard', lambda: valid)
    stub(r, 'IsOurs__C13CMemoryCardGCRCQ46Realmc7Message10DetailInfo8FileInfo', lambda: valid)
    r.wr(MESSAGE + 0x40, DATA); r.cstring(DATA, 'PROFILE')
    r.slot(VT + 0x800, 0x10, lambda: (r.tracecall('index', r.rr(3)), 2)[1], adjust)
    r.slot(VT + 0xa00, 0x10, lambda: r.tracecall('execute', r.rr(3), r.text(r.rr(4)), r.rr(5)), adjust)
    if msgid == -1:
        r.slot(VT + 0x400, 0xa8, lambda: (r.tracecall('poll', r.rr(3), signed(r.rr(4))), 0)[1], adjust)
    ret = r.call('Check'+operation+'__13CMemoryCardGC', OBJ)
    assert r.trace[0] == ('poll', ENG + adjust, 12)
    if waiting and not (operation == 'Start' and msgid == 1 and result == 26): assert ret == 1
    if not waiting and operation in ('Save','Load') and msgid == (5 if operation=='Save' else 4): assert ret == (4 if result==24 else 2)
    if not waiting and operation == 'Scan' and msgid == 22: assert ret == 2 and r.rd(OBJ + 0x94) == r.rd(OBJ + 0x98) == 0
    return ret, memory(r, OBJ + 0x88, 28), r.rd(CORE + 0x5c), r.trace


def issue(r, case):
    operation, warm = case; gcbase(r)
    r.cstring(DATA, 'PROFILE'); info = bytes(range(48)); r.u.mem_write(OBJ + 0x58, info)
    r.wr(gcglob('_.tmp_1.3257'), warm); r.u.mem_write(gcglob('fileInfo.3256'), b'\xaa'*48)
    r.call('Issue'+operation+'__13CMemoryCardGC'+('RQ211CMemoryCard8ScanTaskPCQ211CMemoryCard6Device' if operation=='Scan' else 'PCcUi'), OBJ, EXTRA if operation=='Scan' else DATA, CARD if operation=='Scan' else 1)
    if operation == 'Save':
        at=gcglob('fileInfo.3256'); assert r.rd(at)==r.rd(at+20)==DATA and r.rd(at+24)==7
    return memory(r, OBJ, 0xa4), memory(r, gcglob('fileInfo.3256'), 48), r.rd(CORE + 0x5c), r.trace


def process_reply(r, case):
    active, replyid, adjust = case; gcbase(r, adjust=adjust); r.wr(OBJ + 0x90, active)
    r.at('Display__11CMemoryCardPCUwN41', lambda: r.tracecall('display', r.rr(3), memory(r, r.rr(4), 4), *[r.rr(i) for i in range(5,9)]))
    r.at('DebugMsg__FPCce', lambda: r.tracecall('debug', r.text(r.rr(3))))
    r.call('ProcessReply__13CMemoryCardGCUi', OBJ, replyid)
    assert r.trace[0] == ('display', OBJ, b'\x00 \x00\x00', 0, 0, 0, 0)
    if replyid < 4: assert r.trace[-1] == ('send', ENG + adjust, replyid + 10, 0)
    return r.rd(CORE + 0x5c), r.trace


def trc_display(r, count):
    base(r); r.wr(MESSAGE + 0xc, DATA, count); r.wr(MESSAGE + 0x24, 1,2,3,4)
    r.at('Display__11CMemoryCardPCUwN41', lambda: r.tracecall('display', *[r.rr(i) for i in range(3,9)]))
    r.call('DisplayTRCMessage__13CMemoryCardGCPCQ26Realmc7Message', OBJ, MESSAGE)
    assert r.trace == [('display', OBJ, DATA, *[i+1 if count>i else 0 for i in range(4)])]
    return r.trace


def ours(r, case):
    length, mismatch = case; base(r)
    r.wr(addr('COMPANY_CODE'), EXTRA); r.wr(addr('GAME_CODE'), EXTRA + 4)
    r.u.mem_write(EXTRA, b'EA\x00\x00GR8E'); r.cstring(DATA, 'a'*length); r.wr(MESSAGE, DATA)
    code=bytearray(b'EAGR8E')
    if mismatch >= 0: code[mismatch] ^= 1
    r.u.mem_write(MESSAGE + 12, bytes(code))
    value=r.call('IsOurs__C13CMemoryCardGCRCQ46Realmc7Message10DetailInfo8FileInfo', OBJ, MESSAGE)
    assert value == (mismatch < 0 and length < 9)
    return value


def device_issue(r, label):
    gcbase(r)
    r.slot(VT + 0x400, 0xc8, lambda: DATA if label is not None else 0)
    r.slot(VT + 0x400, 0x28, lambda: r.tracecall('info', r.rr(3), r.rr(4)))
    r.u.mem_write(CARD + 0x10, b'\xaa'*128)
    if label is not None: r.u.mem_write(DATA, label.encode('utf-16-be') + b'\0\0')
    def wide_length(p):
        n=0
        while memory(r,p+2*n,2) != b'\0\0': n+=1
        return n
    def wide_copy():
        dest,src,count=[r.rr(i) for i in range(3,6)]; size=min(count,wide_length(src))
        r.u.mem_write(dest,memory(r,src,2*size)+bytes(2*(count-size))); return dest
    r.at('wcsncpy',wide_copy); r.at('wcslen',lambda:wide_length(r.rr(3)))
    r.call('IssueCache__9CMCDevice',CARD)
    assert r.rd(CARD+0xc)==0
    expected=(' ' if label is None else label.replace('\xa0',' ')).encode('utf-16-be')+b'\0\0'
    assert memory(r,CARD+0x10,len(expected))==expected
    return memory(r,CARD,160),r.trace


def device_poll(r, case):
    msgid,trc,verbose,status,free,total=case;gcbase(r)
    r.wr(gcglob('g_bVerbose'),verbose);r.wr(CARD+0xc,status);r.wr(MESSAGE,msgid,0,trc);r.wr(MESSAGE+0x38,free,total)
    r.at('Invalidate__9CMCDeviceb',lambda:r.tracecall('invalidate',r.rr(3),r.rr(4)))
    if msgid == -1:r.slot(VT+0x400,0xa8,lambda:0)
    ret=r.call('CheckCache__9CMCDevice',CARD)
    assert ret==(2 if msgid==3 else 0)
    if msgid==3:assert r.rd(CARD+0x90)==free and r.rd(CARD+0x94)==min(free,total)
    return ret,memory(r,CARD,160),r.rd(OBJ+0xa0),r.trace


def results(r, value):
    base(r);r.wr(OBJ+0x88,value,value+1,value+2);r.wr(OBJ+0xa0,value+3)
    ret=[r.call(name+'__C13CMemoryCardGC',OBJ) for name in ['SaveSuccessful','LoadSuccessful','DefaultReply','DeclinedToSave']]
    assert ret==[value&0xffffffff,(value+1)&0xffffffff,(value+3)&0xffffffff,0]
    return ret


def verbosity(r, quiet):
    r.call('TellTheGamecubeToShutUp__13CMemoryCardGCb',OBJ,quiet)
    assert r.rd(gcglob('g_bVerbose'))==(not quiet)
    return r.rd(gcglob('g_bVerbose'))


def startup(r, case):
    sku,language,valid,already,state=case;base(r)
    r.at('Instance__9MohLoader',lambda:BUTTON);r.wr(BUTTON+0x54,valid)
    # skuType is +0xb0 in CGameSettings, as read at 0x802f8178.
    r.wr(addr('g_GameSettings')+0xb0,sku);r.wr(CORE,language);r.u.mem_write(CORE+0x321e0,b'\x02')
    r.wr(glob('wasAlreadyHere.4236'),already)
    r.at('Instance__23CSaveGameLanguageScreen',lambda:PKG)
    r.at('SelectLanguage__23CSaveGameLanguageScreen',lambda:r.tracecall('select'))
    r.at('LOCALE_setstate',lambda:r.tracecall('locale',r.rr(3),r.rr(4),r.rr(5)))
    queue=[0,state]
    r.slot(VT,0x18,lambda:r.tracecall('start'))
    r.slot(VT,0x20,lambda:queue.pop(0))
    r.at('Wait__11CMemoryCard',lambda:r.tracecall('wait',r.rd(glob('g_bMinimalInteraction'))))
    r.at('Update__Q211CMemoryCard6Deviceb',lambda:r.tracecall('update',r.rr(3),r.rd(glob('g_bMinimalInteraction'))))
    ret=r.call('OnStartup__11CMemoryCard',OBJ)
    assert ret==(1 if already or state==2 else 0)
    assert r.rd(glob('wasAlreadyHere.4236'))==already
    if not already:assert not queue and r.rd(glob('g_bMinimalInteraction'))==0
    if sku==1 and not valid and language:assert r.rd(CORE+4+2*0xc598+0x2c)&0x80000000
    return ret,memory(r,CORE+4+2*0xc598,0x40),r.trace


@unittest.skipIf(ppc.Uc is None,'Optional Unicorn not installed')
class SaveFlowTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        try:
            _,ppc.original=ppc.load_target();ppc.symbols={s['name']:s for s in ppc.original.symbols() if s['section']}
        except (FileNotFoundError,ValueError) as exc:raise unittest.SkipTest(str(exc))

    def cases(self,unit,cases,fn):
        native=directory(unit)
        if not (native/'compiled.elf').exists():self.skipTest('Build '+unit+' first')
        for case in cases:
            with self.subTest(case=case):self.assertEqual(ppc.normalized(fn(Run(),case)),ppc.normalized(fn(Run(native),case)))

    def test_reply(self):self.cases('mc_display_reply',itertools.product([0,1],[-4,0,8]),reply)
    def test_display(self):self.cases('mc_display_reply',[None],display)
    def test_wait(self):self.cases('mc_wait',itertools.product([0,1],[0,1],[0,1],[-4,0,8]),wait)
    def test_cache(self):self.cases('mc_cache_device',itertools.product([[2],[0,1,0,2]],[-4,0,8]),cache)
    def test_verify_crc(self):self.cases('mc_verify_crc',[(n,c,reflection) for n,c,reflection in itertools.product([0,1,4,17,128],['none','crc','version'],[0,1])],crc)
    def test_save_load(self):self.cases('mc_save_load',itertools.product([0,1],[[2],[0,1,4,0,2]],[0,1],[0,1],[0,1],[0,0xbad]),save_load)
    def test_gc_poll(self):
        cases=[]
        for operation,ids in [('Start',[-1,0,1,23,24,99]),('Save',[-1,0,5,13,19,24,27]),('Load',[-1,0,4,13,18,24,26,28]),('Scan',[-1,0,9,12,21,22,25,99])]:
            for msg in ids:
                for result,trc,waiting,valid,adjust in itertools.product([0,24,26] if msg in [1,4,5,9,21,25] else [0],range(10,37) if msg==24 else [0],[0,1],[0,1] if msg in [25,28] else [1],[-4,0,8]):
                    cases.append((operation,msg,result,trc,waiting,valid,adjust))
        self.cases('mcgc_operations',cases,poll)
    def test_gc_issue(self):self.cases('mcgc_operations',itertools.product(['Save','Load','Scan'],[0,1]),issue)
    def test_process_reply(self):self.cases('mcgc_operations',itertools.product([0,1],[0,1,2,3,4,0xffffffff],[-4,0,8]),process_reply)
    def test_trc_display(self):self.cases('mcgc_operations',[-1,0,1,2,3,4,5,99],trc_display)
    def test_is_ours(self):self.cases('mcgc_operations',itertools.product([0,1,8,9,32],range(-1,6)),ours)
    def test_device_issue(self):self.cases('mcgc_operations',[None,'','A','Memory\xa0Card','\xa0\xa0','x'*62],device_issue)
    def test_device_poll(self):self.cases('mcgc_operations',[(24,t,v,s,10,20) for t,v,s in itertools.product(range(10,37),[0,1],[0,1])] + [(i,0,1,0,f,t) for i,f,t in itertools.product([-1,3,99],[0,10],[0,20])],device_poll)
    def test_results(self):self.cases('mcgc_results',[-1,0,1,100],results)
    def test_verbosity(self):self.cases('mcgc_operations',[0,1],verbosity)
    def test_startup(self):self.cases('mc_startup',itertools.product([0,1],[0,2,3,6,7,9],[0,1],[0,1],[2,3]),startup)


if __name__=='__main__':unittest.main()
