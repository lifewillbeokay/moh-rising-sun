"""Isolated original/native PPC checks for the Realmc card-task batch.

Original target and optional Unicorn required. Card hardware, threading, allocation
and localization boundaries are stubbed; this is not an in-game save/load test.
"""
import itertools
import unittest
from pathlib import Path
import test_explosion_grenade as ppc
from test_shortlist_partials import Run
from test_frontend_screens import addr, private, memory, signed
from test_feng_animation_lists import copy_stubs

ROOT = Path(__file__).resolve().parents[1]
EXACT = {'gc_task_close', 'gc_task_seek', 'gc_task_attribute', 'mc_memvectors',
         'mc_block_calculator', 'mc_block_result'}
DRIVER, MANAGER, VT, DATA, FILE, FOUND = [0x10100000 + n * 0x10000 for n in range(6)]


def directory(unit):
    if unit in {'gc_task_trc_exists', 'gc_task_trc_info', 'gc_task_trc_list'}:
        unit = 'gc_trc_updates'
    return ROOT / ('build/reconstruction/units' if unit in EXACT else 'build/comparisons') / (unit if unit in EXACT else 'compare_' + unit)


def glob(name): return addr('_Q26Realmc11GCInterface.' + name)
def driver_symbol(name): return next(n for n in ppc.symbols if n.startswith(name + '__Q26Realmc8GCDriver'))
def task_symbol(name): return 'UpdateTask' + name + '__Q26Realmc11GCInterface'


def base(r):
    r.rw(13, addr('_SDA_BASE_')); r.rw(2, addr('_SDA2_BASE_')); copy_stubs(r)
    r.wr(glob('mpDriver'), DRIVER)
    r.u.mem_write(glob('mTaskMsg'), bytes([0xA5]) * 148)
    r.wr(MANAGER + 0x4c, VT)
    r.wr(glob('mReadFailed'), 0); r.wr(glob('mWriteFailed'), 0)


def setup_task(r, name, state=0, step=0, enabled=1, adjust=0):
    task = glob('mTask' + name)
    r.u.mem_write(task, bytes(32)); r.wr(task, MANAGER, 25, state, step, VT + 0x200, 0x10002, 17, enabled)
    def end():
        assert (r.rr(3), r.rr(4)) == (MANAGER + adjust, task)
        r.tracecall('end', memory(r, glob('mTaskMsg'), 148))
    r.slot(VT, 16, end, adjust)
    return task


MAPS = {
    'CardExists': {0:0}, 'GetCardInfo':{0:0},
    'Mount': {0:0, 2:6, 3:2, 6:4, 13:4, 16:5},
    'Unmount': {0:0, 3:2}, 'Open':{0:0, 4:11, 5:5, 6:14},
    'Close':{0:0}, 'Flush':{0:0}, 'Seek':{0:0, 11:23},
    'Read':{0:0, 2:6, 3:2, 4:10, 5:5, 6:14, 8:7, 9:9, 10:8, 11:23},
    'Write':{0:0, 2:6, 3:2, 4:10, 5:5, 6:14, 8:7, 9:9, 10:8, 11:23},
    'Delete':{0:0, 3:2, 4:11, 5:5, 10:8}, 'SetAttribute':{0:0, 3:2, 4:11, 10:8}}
MESSAGES = dict(zip(['CardExists','GetCardInfo','Mount','Unmount','Open','Close','Read','Write','Seek','Flush','Delete','SetAttribute'],[8,9,21,22,13,14,18,19,17,20,15,16]))
CALLS = {'GetCardInfo':'GetFreeCardSpace','Open':'OpenFile','Close':'CloseFile','Read':'ReadFile','Write':'WriteFile','Flush':'FlushWriteBuffer','Delete':'DeleteFile','SetAttribute':'SetAttributes'}


def ordinary(r, case):
    name, result, enabled = case; base(r)
    adjust = [-4,0,8][result % 3]
    task = setup_task(r, name, enabled=enabled, adjust=adjust)
    previous_failure = int(result == 0 and enabled)
    if name in ['Read','Write']: r.wr(glob('m'+name+'Failed'),previous_failure)
    if name in ['Read','Write','Seek']: r.wr(task+32, FILE, DATA, 13)
    elif name in ['Close','Flush']: r.wr(task+32, FILE)
    elif name == 'Open': r.wr(task+32, DATA, 0x200); r.wr(DATA, DATA+128)
    elif name in ['Delete','SetAttribute']: r.cstring(task+32,'save.bin'); r.wr(task+68, 7)
    def operation():
        assert r.rr(3) == DRIVER
        if name in ['CardExists','GetCardInfo','Mount','Unmount','Open','Delete']:
            assert r.rr(4) == task+20
        if name == 'GetCardInfo': r.wr(r.rr(5), 8192); r.wr(r.rr(6), 6)
        if name == 'Open':
            assert (r.rr(5),r.rr(6)) == (DATA,0x200); assert r.rd(r.rr(7)) == 0; r.wr(r.rr(7),FILE)
        if name in ['Close','Flush','Seek','Read','Write']: assert r.rr(4) == FILE
        if name in ['Read','Write']:
            assert (r.rr(5),r.rr(6)) == (DATA,13); assert r.rd(r.rr(7)) == 0; r.wr(r.rr(7),7)
        if name == 'Seek': assert (r.rr(5),r.rr(6)) == (DATA,13)
        if name == 'Delete':
            assert r.rd(r.rr(5)) == task+32 and memory(r,r.rr(5)+4,44) == bytes(44)
        if name == 'SetAttribute':
            assert r.rd(r.rr(4)) == 0x10002 and (r.rr(5),r.rr(6)) == (task+32,7)
        r.tracecall('driver', name); return result
    r.at(driver_symbol(CALLS.get(name,name)), operation)
    r.call(task_symbol(name))
    status = MAPS[name].get(result, 2 if name=='CardExists' else 1)
    if enabled:
        assert (r.rd(glob('mTaskMsg')),r.rd(glob('mTaskMsg')+4)) == (MESSAGES[name],status)
        assert memory(r,glob('mTaskMsg')+112,32) == bytes(32)
    else: assert memory(r,glob('mTaskMsg'),148) == bytes([0xA5])*148
    if name in ['Read','Write']: assert r.rd(glob('m'+name+'Failed')) == bool(status or previous_failure)
    assert len(r.trace)==2 and r.trace[0]==('driver',name)
    return memory(r,task,32),memory(r,glob('mTaskMsg'),148),r.trace


def find_task(r, case):
    state, status, enabled, adjust = case; base(r)
    task=setup_task(r,'Find',state,7,enabled,adjust);r.cstring(task+32,'*.sav');r.wr(task+68,4)
    r.u.mem_write(FOUND,bytes([0x5A])*148);r.wr(FOUND,25,status)
    def find():
        assert (r.rr(3),r.rr(4))==(task+20,task+32)
        r.tracecall('find',r.rr(5));return FOUND
    r.at('Find__Q26Realmc11GCInterfaceRCQ26Realmc6CardIDPCcb',find)
    r.call(task_symbol('FindFile'))
    if state in [0,10,11]:
        assert r.trace[0]==('find',int(state!=11))
        assert r.rd(task+8)==(121 if status==11 else 11)
        assert r.rd(task+68)==4+(status!=11)
    return memory(r,task,72),memory(r,glob('mTaskMsg'),148),r.trace


def trc(r, case):
    name,state,step,mount_result,mounted,timer = case;base(r)
    task=setup_task(r,name,state,step,adjust=8);r.wr(task+32,33,mounted,44)
    mount=glob('mTaskTrcMount');r.u.mem_write(mount,bytes([0x66])*64);r.wr(mount,MANAGER);r.wr(mount+24,mount_result)
    r.wr(glob('mMsgTimer'),*(dict(off=(0,0,20,3),early=(1,19,20,3),equal=(1,20,20,3),late=(1,21,20,3),high=(1,0xffffffff,20,3),wrap=(1,1,0xfffffffe,3))[timer]))
    if name=='TrcGetCardInfo':r.wr(task+44,0x30004,1024,2)
    if name=='TrcListFiles':r.wr(task+44,DATA);r.cstring(DATA,'*.sav')
    r.slot(VT,8,lambda:r.tracecall('start',r.rr(3),r.rr(4)),8)
    def unmount():
        assert (r.rr(3),r.rr(4))==(DRIVER,task+20);r.tracecall('unmount');return 0
    r.at(driver_symbol('Unmount'),unmount)
    def query():
        assert (r.rr(3),r.rr(4),r.rr(5),r.rr(6))==(DRIVER,task+20,task+48,task+52)
        r.wr(r.rr(5),8192);r.wr(r.rr(6),4);r.tracecall('query');return mount_result
    r.at(driver_symbol('GetFreeCardSpace'),query)
    r.u.mem_write(FOUND,bytes([0x77])*148);r.wr(FOUND,25,11 if mount_result else 0)
    def find():
        assert (r.rr(3),r.rr(4))==(task+20,DATA);r.tracecall('find',r.rr(5));return FOUND
    r.at('Find__Q26Realmc11GCInterfaceRCQ26Realmc6CardIDPCcb',find)
    r.call(task_symbol(name))
    return memory(r,task,56),memory(r,mount,64),memory(r,glob('mTaskMsg'),148),memory(r,glob('mMsgTimer'),16),r.trace


def public_query(r, case):
    info, adjust=case;base(r);name='TrcGetCardInfo' if info else 'TrcCardExists'
    task=setup_task(r,name,state=91,step=7);r.wr(DATA,0x30004)
    r.u.mem_write(task+32,bytes([0xAA])*(24 if info else 12))
    r.at('MUTEX_lock',lambda:r.tracecall('lock',r.rr(3)))
    r.at('MUTEX_unlock',lambda:r.tracecall('unlock',r.rr(3)))
    r.slot(VT+0x200,16,lambda:r.tracecall('clear',r.rr(3)),adjust)
    r.slot(VT,8,lambda:r.tracecall('start',r.rr(3),r.rr(4)),adjust)
    r.call(name+'__Q26Realmc11GCInterfaceRCQ26Realmc6CardID',DRIVER,DATA)
    assert [x[0] for x in r.trace]==['lock','clear','clear','start','unlock']
    assert [r.rd(task+x) for x in [8,20,24,28]]==[0,0x30004,27,1]
    if info:assert memory(r,task+44,12)==memory(r,DATA,4)+bytes(8)
    return memory(r,task,56 if info else 44),r.trace


def memvectors(r, case):
    operation, size=case;base(r)
    slots=[private('memvectors.cpp','_6Realmc.'+n) for n in ['gpMemAlloc','gpMemFree','gpMemAllocSize','gpMemFreeSize']]
    for i,p in enumerate(slots):
        callback=VT+0x500+i*4;r.wr(p,callback)
        def cb(i=i):
            count=5 if i in [0,2] else (1 if i==1 else 2)
            r.tracecall(i,*[r.rr(n) for n in range(3,3+count)]);return DATA+0x100
        r.hooks[callback]=cb
    names={n:next(s for s in ppc.symbols if s.startswith(n+'__6Realmc')) for n in ['SetMemVectors','SetSizeMemVectors','DefaultSizeAlloc','DefaultSizeFree','AllocateMem','AllocateMemSize','FreeMem','FreeMemSize']}
    if operation.startswith('Set'):
        r.call(names[operation],DATA,FILE);index=2 if operation=='SetSizeMemVectors' else 0
        assert [r.rd(x) for x in slots[index:index+2]]==[DATA,FILE]
    else:
        result=r.call(names[operation],DATA,size,3,4,5)
        i={'DefaultSizeAlloc':0,'AllocateMem':0,'AllocateMemSize':2,'DefaultSizeFree':1,'FreeMem':1,'FreeMemSize':3}[operation]
        assert r.trace[0][0]==i
        if 'Alloc' in operation:assert result==DATA+0x100
    return [r.rd(p) for p in slots],r.trace


def block(r, case):
    operation,initial,adjust=case;base(r);r.wr(DATA,VT,DRIVER,initial,2,99,1)
    r.slot(VT,8,lambda:r.tracecall('clear',r.rr(3)),adjust)
    def blocks():
        assert (r.rr(3),r.rr(4),r.rr(5),r.rr(6))==(DRIVER,FILE,FOUND,0)
        r.tracecall('blocks');return 7
    r.at(driver_symbol('GetFileBlocks'),blocks)
    n=next(n for n in ppc.symbols if n.startswith(operation+'__Q26Realmc18BlockCalculatorImp'))
    result=r.call(n,DATA,FILE,FOUND)
    if operation=='Init':assert r.rd(DATA+4)==FILE and r.rd(DATA+16)==FOUND
    if operation=='Clear':assert [r.rd(DATA+x) for x in [8,12,20]]==[0,0,0]
    if operation=='SetFileInfo':assert [r.rd(DATA+x) for x in [8,12,20]]==[(initial+7)&0xffffffff,3,1]
    if operation=='GetResult':assert result==initial&0xffffffff
    return memory(r,DATA,24),r.trace


def task_end(r, case):
    count, public=case;base(r);r.wr(MANAGER,FILE);r.wr(MANAGER+4,*[DATA+i*16 for i in range(16)]);r.wr(MANAGER+68,count,5)
    def check():assert (r.rr(3),r.rr(4))==(MANAGER,FILE);r.tracecall('public');return public
    r.at('IsPublicTrcTask__Q26Realmc11TaskManagerPQ26Realmc4Task',check)
    r.call('EndTask__Q26Realmc11TaskManagerPQ26Realmc4Task',MANAGER,FILE)
    assert r.rd(MANAGER)==(DATA+(count-1)*16 if count>0 else 0)
    assert r.rd(MANAGER+72)==5-public
    return memory(r,MANAGER,76),r.trace


def singleton(r, task):
    base(r);r.wr(MANAGER,*range(20));before=memory(r,MANAGER,80)
    r.call('TrcSingletonAssert__Q26Realmc11TaskManagerPQ26Realmc4Task',MANAGER,task)
    assert memory(r,MANAGER,80)==before
    return before


def create(r, case):
    result,busy=case;base(r);r.wr(FILE+8,0x10002,DATA,8192);r.wr(FILE+0xd8,77);r.wr(FILE+0xe0,1)
    calls=[0]
    def card_create():
        assert (r.rr(3),r.rr(4),r.rr(5),r.rr(6))==(1,DATA,8192,FILE+0x48)
        calls[0]+=1;r.tracecall('create');return -1 if calls[0]<=busy else result
    r.at('CARDCreate',card_create);r.at('THREAD_yield',lambda:r.tracecall('yield',r.rr(3)))
    actual=r.call(driver_symbol('NGCSportsBioCreate'),DRIVER,FILE)
    assert signed(actual)==result
    assert signed(r.rd(FILE+0xd8))=={0:0,-128:-1,-3:-2,-7:-4,-8:-5,-9:-6,-12:-7,-5:-8}.get(result,-10)
    assert r.rd(FILE+0xe0)==bool(result)
    return memory(r,FILE,228),r.trace


def checksum(r, case):
    length,error,corrupt=case;base(r);r.wr(FILE+0xd0,length+4);r.wr(DRIVER+0x2c0,DATA)
    r.wr(addr('_Q26Realmc8GCDriver.MEMCARD_SECTOR_SIZE'),8)
    counts=[0];total=[0]
    def read():
        assert (r.rr(3),r.rr(4),r.rr(7))==(DRIVER,FILE,0)
        counts[0]+=1;count=r.rr(6);dest=r.rr(5);r.tracecall('read',bool(dest),count)
        if counts[0]==error:return 5
        if dest:r.wr(dest,total[0]+corrupt)
        else:
            data=bytes((128+counts[0]+i)%256 for i in range(count));r.u.mem_write(DATA,data)
            total[0]+=sum(x-256 if x>=128 else x for x in data)
        return 0
    r.at(driver_symbol('ReadFile'),read)
    result=r.call(driver_symbol('VerifyHeaderChecksum'),DRIVER,FILE)
    assert result == (15 if length == 0 else 5 if 0 < error <= (length+7)//8+1 else 6 if corrupt else 0)
    return result,r.trace


def write_file(r, case):
    count,position,offset,capacity,write_error,read_error,busy,bio,output=case;base(r)
    r.wr(DRIVER+0x2b4,8);r.wr(DRIVER+0x2c0,DATA)
    r.wr(FILE+0xc8,position,offset,0,0,0,0,not bio)
    r.u.mem_write(DATA,bytes([0x55])*8);r.u.mem_write(FOUND,bytes(range(64)));r.wr(FOUND+128,99)
    r.at(driver_symbol('ConvertFileHandleToDescriptor'),lambda:FILE)
    r.at(driver_symbol('NGCSportsBioWrite'),lambda:(r.tracecall('bio',r.rr(4),r.rr(5),r.rr(6),r.rr(7)),3)[1])
    r.at(driver_symbol('ConvertCardResult'),lambda:0 if signed(r.rr(4))==0 else 5)
    r.at(driver_symbol('GetFileBlocks'),lambda:capacity)
    counters={'write':0,'read':0}
    def io(which,err):
        assert (r.rr(3),r.rr(4),r.rr(5))==(FILE+0x48,DATA,8)
        counters[which]+=1;r.tracecall(which,signed(r.rr(6)),memory(r,DATA,8))
        if counters[which]<=busy:return -1
        if which=='read':r.u.mem_write(DATA,bytes([0x99])*8)
        return err
    r.at('CARDWrite',lambda:io('write',write_error));r.at('CARDRead',lambda:io('read',read_error))
    r.at('THREAD_yield',lambda:r.tracecall('yield',r.rr(3)))
    def clear():
        assert (r.rr(3),r.rr(4))==(DATA,8);r.u.mem_write(DATA,bytes(8));r.tracecall('clear')
    r.at('MEM_clear',clear)
    result=r.call(driver_symbol('WriteFile'),DRIVER,FILE,FOUND,count,FOUND+128 if output else 0)
    if output and not bio:assert r.rd(FOUND+128)==count&0xffffffff
    return result,memory(r,FILE,228),memory(r,DATA,8),r.rd(FOUND+128),r.trace


@unittest.skipIf(ppc.Uc is None,'Optional Unicorn not installed')
class CardTaskTests(unittest.TestCase):
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

    def test_find(self):self.cases('gc_task_find',itertools.product([0,10,11,121,7],[0,5,11],[0,1],[-4,8]),find_task)
    def test_trc_exists(self):self.cases('gc_task_trc_exists',[('TrcCardExists',s,7,m,b,'off') for s,m,b in itertools.product([0,99,103,122,7],[0,3,5],[0,1])],trc)
    def test_trc_info(self):self.cases('gc_task_trc_info',[('TrcGetCardInfo',s,7,m,b,'off') for s,m,b in itertools.product([0,99,103,21,122,7],[0,3,5],[0,1])],trc)
    def test_trc_list(self):self.cases('gc_task_trc_list',[('TrcListFiles',s,p,m,b,t) for (s,p),m,b,t in itertools.product([(0,7),(99,0),(99,103),(99,7),(12,0),(12,11),(12,7),(121,0),(7,0)],[0,3,5],[0,1],['off','early','equal','late','high','wrap'])],trc)
    def test_public_queries(self):self.cases('mc_public_card_queries',itertools.product([0,1],[-4,0,8]),public_query)
    def test_memvectors(self):self.cases('mc_memvectors',itertools.product(['SetMemVectors','SetSizeMemVectors','DefaultSizeAlloc','DefaultSizeFree','AllocateMem','AllocateMemSize','FreeMem','FreeMemSize'],[0,1,4096]),memvectors)
    def test_block(self):self.cases('mc_block_calculator',itertools.product(['Init','Clear','SetFileInfo'],[0,3,-1],[-4,0,8]),block)
    def test_block_result(self):self.cases('mc_block_result',[('GetResult',n,0) for n in [0,3,-1]],block)
    def test_task_end(self):self.cases('mc_task_end',itertools.product([-1,0,1,3,16],[0,1]),task_end)
    def test_singleton(self):self.cases('mc_task_singleton',[0,FILE],singleton)
    def test_create(self):self.cases('gc_driver_create_verify',itertools.product([0,-2,-3,-4,-5,-6,-7,-8,-9,-10,-11,-12,-13,-14,-128],[0,2]),create)
    def test_checksum(self):self.cases('gc_driver_create_verify',itertools.product([0,1,7,8,9,24],[0,1,2,4],[0,1]),checksum)
    def test_buffered_write(self):
        cases=[(n,p,o,cap,0,0,0,0,out) for n,p,o,cap,out in itertools.product([0,1,8,13,24],[0,3,8],[-1,0,8],[0,8],[0,1])]
        cases += [(16,0,0,cap,we,re,busy,bio,out) for cap,we,re,busy,bio,out in itertools.product([0,8],[0,-5],[0,-5],[0,2],[0,1],[0,1])]
        self.cases('gc_driver_write',cases,write_file)


for name,unit in [('CardExists','exists'),('GetCardInfo','info'),('Mount','mount'),('Unmount','unmount'),('Open','open'),('Close','close'),('Read','read'),('Write','write'),('Seek','seek'),('Flush','flush'),('Delete','delete'),('SetAttribute','attribute')]:
    def test(self,name=name,unit=unit):
        self.cases('gc_task_'+unit,[(name,result,enabled) for result,enabled in itertools.product([-1]+list(range(17))+[99],[0,1])],ordinary)
    setattr(CardTaskTests,'test_'+unit,test)

if __name__=='__main__':unittest.main()
