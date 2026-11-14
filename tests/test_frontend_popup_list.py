"""Original/native PPC checks for popup lifecycle, settings, rendering and menus.

Requires the pinned target, compiled units and optional Unicorn. External game
services are stubbed: these are isolated state/ordering checks, not gameplay.
"""
import itertools
import struct
import unittest
from pathlib import Path
import test_explosion_grenade as ppc
from test_shortlist_partials import Run
from test_frontend_screens import common, addr, private, memory, hash_name, signed, OBJ, PKG, ENG, BUTTON, CORE, EXTRA, VT
from test_feng_animation_lists import copy_stubs

ROOT = Path(__file__).resolve().parents[1]
EXACT = {'savepopup_background', 'savepopup_update', 'savepopup_events',
         'favorites_remove_duplicates', 'savepopup_lifecycle'}


def directory(unit):
    return ROOT / ('build/reconstruction/units' if unit in EXACT else 'build/comparisons') / (unit if unit in EXACT else 'compare_' + unit)


def buttons(r, setting):
    r.u.mem_write(EXTRA, b'\xaa' * 32)
    r.call('SetUpButtonConfig__13CGameSettingsiPs', OBJ, setting, EXTRA + 2)
    result = struct.unpack('>14h', memory(r, EXTRA + 2, 28))
    expected = (18,16,17,21,2048,4,6,7,2,1024,1,2,-1,512) if setting == 1 else (
        (18,16,15,17,2048,1024,0,1,1,256,512,1,-1,16) if setting in [2,3] else
        (18,16,15,17,2048,512,0,1,256,1024,1,256,-1,16))
    assert result == expected
    assert memory(r, EXTRA, 2) == memory(r, EXTRA + 30, 2) == b'\xaa\xaa'
    return result


def restore(r, case):
    slot, player, replace_core = case
    profile, alternate = 0x10120000, 0x10140000
    source = bytes((i * 17 + slot) & 255 for i in range(0x954))
    r.wr(OBJ, CORE)
    r.u.mem_write(profile + 0x6c + slot * 0x954, source)
    r.u.mem_write(alternate + 0x31664, b'\x5a' * 0x954)
    calls = [0]
    def get_profile():
        r.tracecall('profile', r.rr(3), r.rr(4)); calls[0] += 1
        if replace_core and calls[0] == 1: r.wr(OBJ, alternate)
        return profile
    r.at('GetProfile__13CGameSettingsUi', get_profile)
    r.call('RestoreSaveData__13CGameSettingsUiUi', OBJ, slot, player)
    assert memory(r, OBJ + 0xc8, 0x954) == memory(r, CORE + 0x31664, 0x954) == source
    assert memory(r, profile + 0x69, 1) == bytes([slot])
    current = alternate if replace_core else CORE
    assert memory(r, current + 0x31fb8, 100) == memory(r, current + 0x31744, 100)
    return memory(r, OBJ, 0xa1c), memory(r, CORE + 0x31664, 0x9b8), memory(r, alternate + 0x31664, 0x9b8), r.trace


def delta(r, case):
    write, changed_indices, load_indices, special = case
    base = [float(i) for i in range(16)]; value = base[:]
    for i in changed_indices: value[i] += 0.25
    if special == 'nan': base[7] = value[7] = float('nan')
    if special == 'zero': base[0] = -0.0; value[0] = 0.0
    r.wf(EXTRA, *base); r.wf(EXTRA + 0x100, *value)
    r.at('SerializingOut__C13CGameSettings', lambda: int(write))
    index = [-1]
    def boolean():
        i = index[0]; index[0] += 1
        if not write: r.wr(r.rr(4), int(not load_indices) if i == -1 else int(i in load_indices))
        r.tracecall('bool', r.rd(r.rr(4)))
    def number():
        i = (r.rr(4) - EXTRA - 0x100) // 4
        if not write: r.wf(r.rr(4), 100.0 + i)
        r.tracecall('float', i, memory(r, r.rr(4), 4))
    r.at('SerializeValue__13CGameSettingsRb', boolean)
    r.at('SerializeValue__13CGameSettingsRf', number)
    r.call('SerializeDelta__13CGameSettingsRC7CMatrixR7CMatrix', OBJ, EXTRA, EXTRA + 0x100)
    if write and not changed_indices and special != 'nan': assert r.trace == [('bool', 1)]
    if not write:
        for i in range(16): assert r.rf(EXTRA + 0x100 + 4*i, 1)[0] == (100.0+i if i in load_indices else value[i])
    return memory(r, EXTRA + 0x100, 64), r.trace


def notifications(r, case):
    sound, count, adjust = case
    header = EXTRA; nodes = [EXTRA + 0x100 + 0x20*i for i in range(count)]
    r.wr(OBJ + 0x1878, header); r.wr(header + 8, nodes[0] if count else header)
    for i, node in enumerate(nodes):
        target = BUTTON + 0x100*i; table = VT + 0x100*i
        r.wr(node + 0x10, target); r.wr(target, table)
        r.slot(table, 0x28 if sound else 0x20, lambda: r.tracecall('event', *[r.rr(j) for j in range(3,8)]), adjust)
    name = '_M_increment__Q24_STLt10_Rb_global1ZbPQ24_STL18_Rb_tree_node_base'
    r.at(name, lambda: nodes[nodes.index(r.rr(3))+1] if nodes.index(r.rr(3))+1 < count else header)
    r.call(('NotifySoundMessage' if sound else 'NotificationMessage')+'__14CFengInterfaceUlP8FEObjectUlUl', OBJ, 0x1234, PKG, 0x5678, ENG)
    assert r.trace == [('event', BUTTON + 0x100*i + adjust, 0x1234, 0x5678, PKG, ENG) for i in range(count)]
    return r.trace


def popup_background(r, ignored):
    r.call('EnableBackground__14CSaveGamePopup'); assert r.call('IsBackgroundEnabled__14CSaveGamePopup') == 1
    r.call('DisableBackground__14CSaveGamePopup'); assert r.call('IsBackgroundEnabled__14CSaveGamePopup') == 0
    return r.rd(private('SaveGameCore.cpp', 'g_bBackgroundEnabled'))


def popup_events(r, case):
    sound, adjust = case
    r.wr(OBJ, VT)
    r.slot(VT, 0x38 if sound else 0x30, lambda: r.tracecall('event', r.rr(3), r.rr(4), r.rr(5)), adjust)
    r.call(('ProcessSoundEvent' if sound else 'ProcessUIEvent')+'__14CSaveGamePopupUiUiP8FEObjectP9FEPackage', OBJ, 0x1234, 0xabcdef, BUTTON, PKG)
    assert r.trace == [('event', OBJ + adjust, 0x1234, BUTTON)]
    return r.trace


def popup_update(r, case):
    state, ending, delta = case
    r.wr(addr('g_GameFlow'), state, 0, ending); r.wr(OBJ + 4, ENG)
    r.at('CheckControllers__13CInputManagerb', lambda: r.tracecall('controllers', r.rr(3), r.rr(4)))
    r.at('Update__7FEnginel', lambda: r.tracecall('update', r.rr(3), signed(r.rr(4))))
    r.fw(1, delta); r.call('Update__14CSaveGamePopupf', OBJ)
    assert r.trace[-1] == ('update', ENG, int(delta * 20))
    assert len(r.trace) == (2 if not state and not ending else 1)
    return r.trace


def popup_lifecycle(r, count):
    next_alloc = [0x10180000]; allocations = []
    def allocate():
        at = next_alloc[0]; next_alloc[0] += 0x10000
        r.tracecall('allocate', r.text(r.rr(3)), r.rr(4), r.rr(5)); allocations.append(at)
        r.u.mem_write(at, b'\xa5' * r.rr(4)); return at
    r.at('DWI_alloc', allocate)
    r.at('TLT_LoadFileNormal__FPCcPii', lambda: (r.tracecall('load', r.text(r.rr(3)), r.rr(4), r.rr(5)), EXTRA)[1])
    r.at('__7FEngine', lambda: (r.tracecall('engine_ctor', r.rr(3)), r.rr(3))[1])
    def interface():
        r.tracecall('interface_ctor', r.rr(3), r.rr(4)); r.wr(r.rr(3), VT); return r.rr(3)
    r.at('__14CFengInterfaceP7FEngine', interface)
    r.slot(VT, 0xd8, lambda: r.tracecall('interface_dtor', r.rr(3), r.rr(4)))
    for name, nargs in [('SetExecution__7FEngineb',2),('SetNumJoyPads__7FEngineUc',2),('SetBigFilePtr__14CFengInterfacePc',2),('RegisterUIObject__14CFengInterfaceP9CUIObject',2),('TLT_ForceLoadFileNormalLocked__Fb',1),('UnloadPackage__7FEngineP9FEPackage',2),('_._7FEngine',2),('_._7CSprite',2),('MEM_free',1),('__builtin_delete',1)]:
        r.at(name, lambda n=name,c=nargs: r.tracecall(n,*[r.rr(i) for i in range(3,3+c)]))
    r.at('PushPackage__7FEnginePCcUcUl', lambda: (r.tracecall('push',r.rr(3),r.text(r.rr(4)),r.rr(5),r.rr(6)),PKG)[1])
    r.at('__7CSpritePCcQ27CSprite15ESpriteLoadType', lambda: (r.tracecall('sprite',r.rr(3),r.text(r.rr(4)),r.rr(5)),r.rr(3))[1])
    counter=private('SaveGameCore.cpp','g_uInstanceCount'); assets=private('SaveGameCore.cpp','g_pAssets')
    r.wr(counter,count);r.wr(assets,EXTRA);r.cstring(EXTRA+0x100,'popup.fng')
    r.call('__14CSaveGamePopupPCc',OBJ,EXTRA+0x100)
    assert r.rd(counter)==count+1
    assert [t[2] for t in r.trace if t[0]=='allocate']==[0x5270,0x18a0,0x24]
    before=memory(r,OBJ,32)
    r.call('_._14CSaveGamePopup',OBJ,3)
    assert r.rd(counter)==count and r.rd(assets)==(EXTRA if count else 0)
    return before,memory(r,OBJ,32),r.trace


def duplicates(r, values):
    common(r); r.wr(OBJ+0x50,EXTRA)
    rows=[]
    for i,text in enumerate(values):
        if text is None: rows.append(0);continue
        row=EXTRA+0x100+i*0x100; rows.append(row)
        r.wr(row,row+0x20,1 if text else 0);r.wr(row+0x20,row+0x40);r.cstring(row+0x40,text)
    r.at('GetNumRows__7ListBox', lambda: len(rows))
    r.at('GetRowByIndex__7ListBoxl', lambda: rows[r.rr(4)])
    def remove():
        row=r.rr(4);r.tracecall('remove',row);rows.remove(row)
    r.at('RemoveRow__7ListBoxPQ27ListBox3Row', remove)
    r.at('_._Q27ListBox3Row',lambda:r.tracecall('delete',r.rr(3),r.rr(4)))
    r.call('RemoveDuplicates__22HandlerOnlineFavorites',OBJ)
    return rows,r.trace


def selections(r, flags):
    common(r);r.wr(OBJ+0x40,*flags)
    for name in ['_.tmp_1.3429','_.tmp_2.3431','_.tmp_3.3433','_.tmp_4.3438']:r.wr(private('MemoryCardPopup.cpp',name),0)
    for _ in range(2):
        r.call('ShowSelections__16CMemoryCardPopup',OBJ)
        r.call('HideSelections__16CMemoryCardPopup',OBJ)
    ids=[t[2] for t in r.trace if t[0]=='message']
    expected=[hash_name('Enable Option '+str(i)) for i,f in enumerate(flags) if f]+[hash_name('Enable_All_Inputs')]
    if flags==(1,0,0,0):expected+=[hash_name('Label0')]
    expected+=[hash_name('Disable_All_Inputs')]
    assert ids==expected*2
    hashes=[t[1] for t in r.trace if t[0]=='hash'];assert len(hashes)==len(set(hashes))
    return r.trace


def joypad(r, case):
    blocked,ready,pressed,x,y,pad=case
    r.wr(addr('g_inputMgr')+0x240c,blocked);r.wr(OBJ+0x18ac,123,456)
    table=addr('padFlags');keys=[]
    for i in range(9):
        keys.append(struct.unpack('>H',memory(r,table+8*i+2,2))[0])
    r.at('IsReady__C13CInputManagerSc',lambda:(r.tracecall('ready',signed(r.rr(4))),ready)[1])
    def get():
        key=r.rr(5);r.tracecall('key',r.rr(4),key)
        return x if key==0x81 else y if key==0x82 else int(pressed>=0 and key==keys[pressed])
    r.at('GetValue__C13CInputManagerUcUs',get)
    result=r.call('GetJoyPadMask__12ShellManagerUc',OBJ,pad)
    if blocked or not ready:assert result==0 and r.rd(OBJ+0x18ac)==123
    else:
        expected=r.rd(table+8*pressed+4) if pressed>=0 else 4 if x < -63 else 8 if x>63 else 1 if y < -63 else 2 if y>63 else 0
        assert result==expected
        assert r.rd(OBJ+0x18ac)==x&0xffffffff and r.rd(OBJ+0x18b0)==y&0xffffffff
    return result,memory(r,OBJ+0x18ac,8),r.trace


def slider(r, case):
    position,base,slope=case
    r.wr(OBJ,BUTTON);r.wf(OBJ+4,*base,*slope);r.wr(BUTTON+0x54,EXTRA);r.wr(BUTTON+0x1c,0xa1234567)
    r.at('FindTrack__C8FEScript18FEKeyTrack_Indices',lambda:(r.tracecall('track',r.rr(3),r.rr(4)),EXTRA+0x100)[1])
    r.at('SetKeyValueAt__10FEKeyTracklR9FEVector3',lambda:r.tracecall('value',r.rr(3),r.rr(4),r.rf(r.rr(5))))
    r.fw(1,position);r.call('SetValue__7CSliderf',OBJ)
    assert r.rf(OBJ+0x1c,1)[0]==position
    assert r.rd(BUTTON+0x1c)==0xa1634567
    return memory(r,OBJ,32),r.trace


def render(r, case):
    mode,index,parent=case
    copy_stubs(r)
    r.u.mem_write(BUTTON+0x20,struct.pack('>HH',parent,0));r.wr(BUTTON+0x2c,EXTRA)
    r.wr(EXTRA,17,90,160,220);r.wf(EXTRA+0x1c,1.25,-3.5,5.0,0.1,0.2,0.3,0.4,2,3,4)
    for i in range(4):r.wf(OBJ+0xa0+i*96,*[float(j+i) for j in range(16)],0.5,0.25,0.75,1.0)
    def transform():r.tracecall('transform',r.rr(4),r.rr(5),r.rr(6))
    r.at('BuildScale__Q24EAGL9Transformffff',lambda:r.tracecall('scale',r.rr(3),*[r.fr(i) for i in range(1,5)]))
    r.at('PrependQuatTrans__Q24EAGL9TransformPC6COORD4T1',lambda:r.tracecall('quat',r.rr(3),r.rf(r.rr(4),4),r.rf(r.rr(5),4)))
    r.at('AppendTranslate__Q24EAGL9Transformfff',lambda:r.tracecall('translate',r.rr(3),*[r.fr(i) for i in range(1,4)]))
    r.at('PostMult__Q24EAGL9TransformRCQ24EAGL9Transform',lambda:r.tracecall('parent',r.rr(3),r.rr(4)))
    if mode=='matrix':r.call('GenerateXformMatrix__12ShellManagerPQ24EAGL9TransformP9FEObjDataUs',OBJ,ENG,EXTRA,parent)
    elif mode=='color':
        # Stub at the native entry too, as the earlier partial may change size.
        name='GenerateXformMatrix__12ShellManagerPQ24EAGL9TransformP9FEObjDataUs';r.callback(r.entries[name]['address'],transform)
        r.call('GenerateRenderContext__12ShellManagerUsP8FEObject',OBJ,index,BUTTON)
    else:
        ret=r.call('GetContextTransform__12ShellManagerUsR9FEMatrix4',OBJ,index,ENG)
        assert ret==1 and memory(r,ENG,64)==memory(r,OBJ+0xa0+96*index,64)
    return memory(r,OBJ+0xa0,384),memory(r,ENG,64),r.trace


def render_list(r, count):
    r.wr(OBJ,VT)
    r.slot(VT,0x78,lambda:r.tracecall('render',r.rr(3),r.rr(4)),8)
    for i in range(count):r.wr(EXTRA+8*i,BUTTON+0x100*i,99)
    r.call('RenderObjectList__12ShellManagerP17FEObjectListEntryUl',OBJ,EXTRA,count)
    assert r.trace==[('render',OBJ+8,BUTTON+0x100*i) for i in reversed(range(count))]
    return r.trace


def render_dispatch(r, kind):
    r.wr(BUTTON+0x18,kind)
    for name in ['RenderImage__12ShellManagerP7FEImage','RenderString__12ShellManagerP8FEString','RenderSimpleImage__12ShellManagerP13FESimpleImage']:
        r.at(name,lambda n=name:r.tracecall(n,r.rr(3),r.rr(4)))
    r.at('printf',lambda:r.tracecall('warning',r.text(r.rr(3))))
    r.call('RenderObject__12ShellManagerP8FEObject',OBJ,BUTTON)
    return r.trace


def bonus(r, case):
    event,unlocked,coop,level_shell,last_single,missing,selected=case
    common(r);r.at('printf',lambda:r.tracecall('printf',r.text(r.rr(3))))
    r.wr(addr('g_Shell')+0x18b8,coop);r.wr(addr('g_Shell')+0x18a8,2)
    r.wr(addr('g_GameFlow')+4,level_shell)
    r.at('WasLastGameSinglePlayer__13CGameSettings',lambda:last_single)
    r.cstring(EXTRA+0x300,'profile');r.at('GetNameFromSettings__13CGameSettingsi',lambda:EXTRA+0x300)
    r.at('HasCheat__C13CGameSettingsi',lambda:(r.tracecall('has',r.rr(4)),int(r.rr(4) in unlocked))[1])
    r.at('GetCheatByIndex__19HandlerBonusOptionsi',lambda:(r.tracecall('get',r.rr(4)),r.rr(4)%2)[1])
    r.at('ToggleCheatByIndex__19HandlerBonusOptionsi',lambda:(r.tracecall('toggle',r.rr(4)),r.rr(4)%2)[1])
    def find():
        text=r.text(r.rr(4));r.tracecall('find',text);return 0 if missing and ('group' in text or 'text' in text) else BUTTON
    r.at('FindObjectByName__9FEPackagePCc',find)
    r.at('SetCurrentButton__9FEPackageP7FEngineP8FEObjectb',lambda:r.tracecall('select',r.rr(3),r.rr(4),r.rr(5),r.rr(6)))
    r.wr(OBJ+0x50,*range(100,112));r.wr(BUTTON+0x10,selected)
    r.call('HandleEvent__19HandlerBonusOptionsUiP8FEObjectUl',OBJ,0xc407210 if event=='toggle' else hash_name(event),BUTTON,0)
    return r.trace


@unittest.skipIf(ppc.Uc is None,'Optional Unicorn not installed')
class PopupListTests(unittest.TestCase):
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

    def test_buttons(self):self.cases('settings_button_config',[-5,0,1,2,3,4,100],buttons)
    def test_restore(self):self.cases('settings_restore_slot',itertools.product([0,1,19],[0,3],[False,True]),restore)
    def test_matrix_delta(self):
        cases=[(True,c,[],s) for c,s in itertools.product([[],[0],[15],[2,7,13]],['normal','nan','zero'])]
        cases += [(False,[],c,'normal') for c in [[],[0],[15],[2,7,13],list(range(16))]]
        self.cases('settings_matrix_delta',cases,delta)
    def test_notifications(self):self.cases('fenginterface_notifications',itertools.product([False,True],[0,1,3],[-4,0,8]),notifications)
    def test_background(self):self.cases('savepopup_background',[None],popup_background)
    def test_popup_events(self):self.cases('savepopup_events',itertools.product([False,True],[-4,0,8]),popup_events)
    def test_popup_update(self):self.cases('savepopup_update',itertools.product([0,1],[0,1],[-1.25,0,0.125,2.5]),popup_update)
    def test_popup_lifecycle(self):self.cases('savepopup_lifecycle',[0,1,5],popup_lifecycle)
    def test_duplicates(self):self.cases('favorites_remove_duplicates',[[],['a'],['a','a','a'],['a','b','a','b'],['',''],[None,'a',None,'a']],duplicates)
    def test_selections(self):self.cases('memorycard_selections',itertools.product([0,1],repeat=4),selections)
    def test_joypad(self):self.cases('shell_joypad_mask',itertools.product([0,1],[0,1],[-1,0,8],[-64,-63,0,63,64],[-64,0,64],[0,255]),joypad)
    def test_slider(self):self.cases('slider_value',itertools.product([-1.0,0.0,0.5,2.0],[(1,2,3),(-4,2,-6)],[(0,0,0),(4,-2,8)]),slider)
    def test_render_contexts(self):self.cases('shell_render_contexts',itertools.product(['matrix','color','copy'],[1,2],[0,1,3]),render)
    def test_render_list(self):self.cases('shell_render_contexts',[0,1,5],render_list)
    def test_render_dispatch(self):self.cases('shell_render_contexts',range(-1,15),render_dispatch)
    def test_bonus_menu(self):
        cases=[('Setup Menu',u,c,s,l,m,100) for u,c,s,l,m in itertools.product([[],[0],[1,5,11],list(range(12))],[0,1],[0,1],[0,1],[0,1])]
        cases += [('toggle',[],0,0,0,0,i) for i in [99,100,105,111,112]]+[('other',[],0,0,0,0,100)]
        self.cases('bonusoptions_menu',cases,bonus)


if __name__=='__main__':unittest.main()
