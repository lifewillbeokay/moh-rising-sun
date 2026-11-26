"""Original/native PPC checks for the complete Realmc TRC update batch.

External driver, localization, timer input and task-manager services are stubbed.
These checks exercise state transitions and messages, not physical memory cards.
"""
import itertools
import unittest
from pathlib import Path
import test_explosion_grenade as ppc
from test_shortlist_partials import Run
from test_frontend_screens import memory
from test_card_tasks import base, glob, driver_symbol, task_symbol, DRIVER, MANAGER, VT, DATA, FILE, FOUND

ROOT = Path(__file__).resolve().parents[1]
NATIVE = ROOT / 'build/comparisons/compare_gc_trc_updates'
START, MOUNT, OP, SPACE = 'TrcStartGame', 'TrcMount', 'TrcMountOperation', 'TrcCheckSpace'
SIZES = {'TrcStartGame':124,'TrcCardExists':44,'TrcGetCardInfo':56,'TrcLoadFile':100,
         'TrcSaveFile':128,'TrcDeleteFile':104,'TrcListFiles':124,'TrcMount':64,
         'TrcMountOperation':72,'TrcCheckSpace':148}
HANDLERS = {'TrcMount':'TrcMountCard','TrcMountOperation':'TrcMountCardOperation'}
STATES = {
 'TrcStartGame':[(0,7),(99,0),(99,103),(99,7),(24,0),(24,25),(24,7),(121,0),(999,0)],
 'TrcLoadFile':[(0,7),(99,0),(99,103),(99,7),(98,0),(98,95),(98,7),(79,0),
                (75,0),(75,74),(75,75),(75,7),(77,0),(77,78),(77,7),
                (112,0),(112,113),(112,7),(85,0),(85,86),(85,7),(87,0),
                (114,0),(114,115),(114,7),(116,0),(116,103),(116,7),(121,0),(999,0)],
 'TrcSaveFile':[(0,7),(99,0),(99,103),(99,7),(24,0),(24,25),(24,7),(60,0),(60,61),(60,7),
                (108,0),(108,95),(108,7),(79,0),(79,79),(79,7),(66,0),(66,67),(66,7),
                (121,0),(69,0),(69,70),(69,7),(116,0),(116,103),(116,7),(122,0),(999,0)],
 'TrcDeleteFile':[(0,7),(99,0),(99,103),(99,7),(85,0),(85,86),(85,7),(87,0),(121,0),
                  (114,0),(114,115),(114,7),(116,0),(116,103),(116,7),(122,0),(999,0)],
 'TrcMount':[(x,7) for x in [0,99,100,63,59,104,101,116,117,121,999]],
 'TrcMountOperation':[(x,7) for x in [0,99,105,106,121,999]],
 'TrcCheckSpace':[(x,7) for x in [0,92,18,10,11,13,5,109,24,28,121,999]]}


def state_case(r, case):
    name, state, step, config = case
    base(r); r.string_stubs()
    r.cstring(DATA, 'save.dat'); r.cstring(DATA+64,'temporary.dat'); r.cstring(DATA+128,'other.dat')
    for kind,size in SIZES.items():
        p=glob('mTask'+kind)
        # Nonzero unknowns expose accidental clearing of unrelated fields.
        r.u.mem_write(p,bytes([0x33])*size)
        r.wr(p,MANAGER,1 if kind==START else 4 if kind=='TrcLoadFile' else 5,0,7,VT+0x100,0x10000,17,0,FILE,0,0)
        if kind in [START,'TrcLoadFile','TrcSaveFile','TrcDeleteFile','TrcListFiles',SPACE]:
            r.wr(p+44,DATA,100,DATA+200,0,0,0,0,0,0,DATA+220,DATA+240,0)
    start,mount,op,space=[glob('mTask'+n) for n in [START,MOUNT,OP,SPACE]]
    r.wr(start+92,3,0,config.get('scan_both',0),DATA+64,2,1,config.get('temporary',0),19)
    r.wr(mount+24,config.get('mount_result',0));r.wr(mount+44,config.get('choice',0),config.get('driver_result',0),5,config.get('keep',0),config.get('removed',0))
    r.wr(op+44,config.get('choice',0),config.get('parent',5),config.get('driver_result',0),config.get('found',0),config.get('wrong',0),config.get('success',0),config.get('io_error',0))
    r.wr(op+36,config.get('mounted',0))
    r.wr(space+24,config.get('space_result',0));r.wr(space+92,config.get('parent',5),config.get('choice',0),FOUND,
         config.get('free_blocks',8),config.get('reclaimed_blocks',0),config.get('needed_blocks',3),
         config.get('free_files',8),config.get('reclaimed_files',0),config.get('needed_files',2),
         3,2,1,1,config.get('exists',0))
    r.wr(space+28,config.get('notify',0));r.wr(space+88,config.get('label_mode',0))
    load=glob('mTaskTrcLoadFile');save=glob('mTaskTrcSaveFile');delete=glob('mTaskTrcDeleteFile')
    r.wr(load+96,config.get('driver_result',0));r.wr(save+100,FILE,0,DATA,config.get('driver_result',0),config.get('choice',0),0,0)
    r.wr(delete+96,config.get('driver_result',0),config.get('choice',0))
    task=glob('mTask'+name);r.wr(task+8,state,step)
    if name in [START,'TrcLoadFile','TrcSaveFile','TrcDeleteFile',MOUNT]:
        r.wr(task+36,config.get('mounted',0),config.get('opened',0))
    r.wr(task+24,config.get('result',17))
    r.wr(glob('mReadFailed'),config.get('read_failed',0));r.wr(glob('mWriteFailed'),config.get('write_failed',0))
    r.wr(glob('mUserMsg'),config.get('user',0))
    r.wr(glob('mMsgTimer'),*(config.get('timer',(0,7,20,3))))
    r.u.mem_write(FOUND,bytes([0x55])*148);r.wr(FOUND,25,config.get('find_result',0));r.wr(FOUND+64,DATA if config.get('same_name',1) else DATA+128,config.get('found_size',17))
    # Preserve all message bytes at every service boundary, including the vptr.
    def message():return memory(r,glob('mTaskMsg'),148)
    adjust=config.get('adjust',8)
    def manager_call(label):
        assert r.rr(3)==MANAGER+adjust
        target=r.rr(4);kind=next(n for n in SIZES if glob('mTask'+n)==target)
        r.tracecall(label,kind,memory(r,target,SIZES[kind]),message())
    r.slot(VT,8,lambda:manager_call('start'),adjust);r.slot(VT,16,lambda:manager_call('end'),adjust)
    def check():
        r.tracecall('check',r.rd(r.rr(3)),message());return config.get('card_result',0)
    r.at('CheckCard__Q26Realmc11GCInterfaceRCQ26Realmc6CardID',check)
    def choice():
        r.tracecall('choice',r.rr(3),r.rr(4),r.rr(5),message());return config.get('choice',0)
    r.at('ConvertUmsgToOption__Q26Realmc11GCInterfaceQ26Realmc11UserMessageiQ26Realmc6TaskID',choice)
    def slot():
        assert r.rr(3)==glob('mTaskMsg');r.tracecall('slot',r.rr(4),message());return DATA+512+r.rr(4)*16
    r.at('LcGetSlotString__Q26Realmc9GCMessagei',slot)
    def options():
        assert r.rr(3)==glob('mTaskMsg')
        r.tracecall('options',r.rr(4),r.rr(5),r.text(r.rr(6)) if r.rr(6) else None,message())
    r.at('LC_SetOptions__Q26Realmc9GCMessageiiPc',options)
    def card_call(label):
        assert r.rr(3)==DRIVER
        card=r.rd(r.rr(4));r.tracecall(label,card,message())
        return config.get({'CardExists':'probe_result','Mount':'mount_result','Unmount':'unmount_result','FormatCard':'format_result'}[label],0)
    for label in ['CardExists','Mount','Unmount','FormatCard']:
        r.at(driver_symbol(label),lambda label=label:card_call(label))
    def file_call(label):
        assert r.rr(3)==DRIVER
        r.tracecall(label,r.rd(r.rr(4)),memory(r,r.rr(5),48),message())
        if label=='OpenFile':
            r.tracecall('mode',r.rr(6));r.wr(r.rr(7),FILE)
        if label=='RenameFile':r.tracecall('rename_to',r.text(r.rr(6)))
        return config.get('open_result' if label=='OpenFile' else 'delete_result',0)
    for label in ['OpenFile','DeleteFile','RenameFile']:r.at(driver_symbol(label),lambda label=label:file_call(label))
    def close():
        assert (r.rr(3),r.rr(4))==(DRIVER,FILE);r.tracecall('close',message());return config.get('close_result',0)
    r.at(driver_symbol('CloseFile'),close)
    def size():
        assert (r.rr(3),r.rr(4))==(DRIVER,FILE);r.tracecall('size',message());return config.get('file_size',100)
    r.at(driver_symbol('GetOpenFileSize'),size)
    def free():
        assert r.rr(3)==DRIVER
        r.tracecall('free',r.rd(r.rr(4)),message());r.wr(r.rr(5),config.get('free_blocks',8));r.wr(r.rr(6),config.get('free_files',8))
        return config.get('free_result',0)
    r.at(driver_symbol('GetFreeCardSpace'),free)
    def blocks():
        assert (r.rr(3),r.rr(6))==(DRIVER,0);r.tracecall('blocks',r.rd(r.rr(4)),memory(r,r.rr(5),48));return 3
    r.at(driver_symbol('GetFileBlocks'),blocks)
    r.at(driver_symbol('GetSectorSize'),lambda:(r.tracecall('sector',r.rd(r.rr(4))),config.get('sector_size',8))[1])
    def find():
        r.tracecall('find',r.rd(r.rr(3)),r.text(r.rr(4)),r.rr(5),message());return FOUND
    r.at('Find__Q26Realmc11GCInterfaceRCQ26Realmc6CardIDPCcb',find)
    r.call(task_symbol(HANDLERS.get(name,name)))
    return ([(n,memory(r,glob('mTask'+n),size)) for n,size in SIZES.items()],message(),
            memory(r,glob('mMsgTimer'),16),[r.rd(glob(n)) for n in ['mReadFailed','mWriteFailed','mUserMsg']],r.trace)


def configs():
    # Sweep one field at a time first, then selected interacting conditions.
    yield {}
    values={'user':[1,2,6,7,10],'choice':[1,2,4,8,16,32,64,128,512],
      'result':[0,1,3,5,14,15,20,24],'mount_result':[2,3,5,6,13,16],
      'card_result':[3],'opened':[1],'mounted':[1],'temporary':[1],
      'read_failed':[1],'write_failed':[1],'driver_result':[2,3,5,6,13,16],
      'open_result':[4,5,6,15],'file_size':[-1],'delete_result':[3,4,5,15],'close_result':[5],
      'free_result':[3],'find_result':[11],'exists':[1],'notify':[1],'same_name':[0],
      'scan_both':[1],'parent':[1],'found':[1],'wrong':[1],'success':[1],'io_error':[1],
      'keep':[1],'removed':[1],'free_blocks':[0,2,3],'free_files':[0,1,2],
      'reclaimed_blocks':[2,3,4],'reclaimed_files':[1,2,3],'label_mode':[1],
      'timer':[(1,19,20,3),(1,20,20,3),(1,21,20,3),(1,0xffffffff,20,3),(1,1,0xfffffffe,3)],
      'adjust':[-4,0]}
    for key,vs in values.items():
        for value in vs:yield {key:value}
    for choice,result in itertools.product([0,1,2,4,8,16,32,64,128,512],[0,3,5,14]):
        yield dict(user=10,choice=choice,result=result,mounted=1,opened=1,temporary=1,driver_result=result)
    for result in [0,2,3,5,6,13,16]:
        yield dict(user=10,driver_result=result,choice=4)
        yield dict(user=0,driver_result=result,probe_result=3)
    for flags in itertools.product([0,1],repeat=4):
        yield dict(choice=2,found=flags[0],wrong=flags[1],io_error=flags[2],keep=flags[3],mounted=1)
    for parent,probe in itertools.product([1,5],[0,2,3]):
        yield dict(parent=parent,scan_both=1,probe_result=probe)
    for blocks,files,temp,label,card in itertools.product([0,3,4],[0,2,3],[0,1],[0,1],[0,3]):
        yield dict(free_blocks=blocks,free_files=files,temporary=temp,label_mode=label,card_result=card)


@unittest.skipIf(ppc.Uc is None,'Optional Unicorn not installed')
class CardTrcTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        try:
            _,ppc.original=ppc.load_target();ppc.symbols={s['name']:s for s in ppc.original.symbols() if s['section']}
        except (FileNotFoundError,ValueError) as exc:raise unittest.SkipTest(str(exc))

    def cases(self,name):
        if not (NATIVE/'compiled.elf').exists():self.skipTest('Build TRC comparisons first')
        for (state,step),config in itertools.product(STATES[name],configs()):
            case=(name,state,step,config)
            with self.subTest(case=case):
                self.assertEqual(ppc.normalized(state_case(Run(),case)),ppc.normalized(state_case(Run(NATIVE),case)))

for name in STATES:
    def test(self,name=name):self.cases(name)
    setattr(CardTrcTests,'test_'+name,test)

if __name__=='__main__':unittest.main()
