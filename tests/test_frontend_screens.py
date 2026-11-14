"""Isolated PPC behavior checks for screen events and co-op bookkeeping.

Requires the pinned original, compiled units/comparisons and optional Unicorn.
External services are stubbed; this checks observed state and callback order,
not gameplay or complete semantic equivalence. Private caches are tested both
cold and warm, and file-local tables retain their original symbol scopes.
"""
import itertools
import re
import struct
import unittest
from pathlib import Path
import test_explosion_grenade as ppc
from test_shortlist_partials import Run

ROOT = Path(__file__).resolve().parents[1]
EXACT = {'shell_accept_pad', 'handler_mp_skin', 'handler_special_features_events'}
OBJ, PKG, ENG, BUTTON, CORE, EXTRA, VT = [0x10020000, 0x10030000, 0x10040000, 0x10050000, 0x10060000, 0x100a0000, 0x100b0000]


def native_directory(unit):
    return ROOT / ('build/reconstruction/units' if unit in EXACT else 'build/comparisons') / (unit if unit in EXACT else 'compare_' + unit)


def addr(name): return ppc.symbols[name]['address']
def memory(r, ptr, size): return bytes(r.u.mem_read(ptr, size))
def byte(r, ptr, value): r.u.mem_write(ptr, bytes([value & 255]))
def signed(value): return value if value < 0x80000000 else value - 0x100000000


def private(file, name):
    scope = None
    for s in ppc.original.symbols():
        if s['type'] == 4: scope = s['name']
        if scope == file and s['name'] == name and s['section']: return s['address']
    raise AssertionError((file, name))


def hash_name(text):
    # A deterministic callback ID, not a reimplementation of FEHashUpper.
    value = 0
    for c in text: value = (value * 131 + ord(c)) & 0xffffffff
    return value


def common(r):
    sh = addr('g_Shell')
    r.wr(OBJ, sh, ENG, PKG); r.wr(OBJ + 0x4c, VT)
    r.wr(PKG + 0xc, EXTRA); r.cstring(EXTRA, 'screen.fng')
    r.wr(PKG + 0x28, 5); r.wr(PKG + 0x8c, BUTTON)
    r.wr(sh + 0x40, ENG); r.wr(sh + 0x4c, EXTRA + 0x100)
    r.wr(addr('g_GameSettings'), CORE)
    r.at('FEHashUpper__FPCc', lambda: (r.tracecall('hash', r.text(r.rr(3))), hash_name(r.text(r.rr(3))))[1])
    r.at('QueueMessage__7FEngineUlP8FEObjectP9FEPackageT2Ul', lambda: r.tracecall('message', *[r.rr(i) for i in range(3, 9)]))
    r.at('QueuePackageSwitch__7FEnginePCcP9FEPackage', lambda: r.tracecall('switch', r.rr(3), r.text(r.rr(4)), r.rr(5)))
    r.at('QueuePackagePush__7FEnginePCcP9FEPackageUl', lambda: r.tracecall('push', r.rr(3), r.text(r.rr(4)), r.rr(5), r.rr(6)))
    r.at('PlayShellSound__12ShellManager15EShellSoundType', lambda: r.tracecall('sound', r.rr(4)))
    r.at('SetObjectString__14PackageHandlerP9FEPackagePCcT2', lambda: r.tracecall('text', r.rr(4), r.text(r.rr(5)), r.text(r.rr(6))))
    r.at('SetScript__14PackageHandlerP9FEPackagePCcT2', lambda: (r.tracecall('script', r.rr(4), r.text(r.rr(5)), r.text(r.rr(6))), 0)[1])
    for n in ['EnableButton', 'DisableButton']:
        r.at(n + '__14PackageHandlerP9FEPackagePCc', lambda n=n: r.tracecall(n, r.rr(4), r.text(r.rr(5))))
    for n in ['HandleSaveGameEvent', 'HandleFEngQueries', 'HandleMovieEvent']:
        r.at(n + '__14PackageHandlerUiP8FEObject', lambda n=n: (r.tracecall(n, r.rr(4), r.rr(5)), 0)[1])
    r.at('SetProfileNameOnScreen__14PackageHandleri', lambda: r.tracecall('profile_name', r.rr(4)))
    r.at('GetCurrentLanguage__14PackageHandler', lambda: (r.tracecall('language'), 2)[1])
    r.string_stubs()
    r.at('strcasecmp', lambda: (r.text(r.rr(3)).lower() > r.text(r.rr(4)).lower()) - (r.text(r.rr(3)).lower() < r.text(r.rr(4)).lower()))
    def format_text():
        fmt = r.text(r.rr(4)); arg = [5]
        def replace(match):
            value = r.rr(arg[0]); arg[0] += 1
            return match.group() % (r.text(value) if match.group()[-1] == 's' else signed(value))
        text = re.sub(r'%[.0-9]*[sdc]', replace, fmt)
        r.cstring(r.rr(3), text); return len(text)
    r.at('sprintf', format_text)
    r.at('printf', lambda: (r.tracecall('printf', r.text(r.rr(3)), r.text(r.rr(4))), 0)[1])


def events(r, cls, event, param=0):
    return r.call('HandleEvent__' + str(len(cls)) + cls + 'UiP8FEObjectUl', OBJ, hash_name(event), BUTTON, param)


def slider(r, case):
    common(r)
    r.at('FindScript__C8FEObjectUl', lambda: (r.tracecall('script', r.rr(3), r.rr(4)), EXTRA)[1])
    r.at('FindTrack__C8FEScript18FEKeyTrack_Indices', lambda: (r.tracecall('track', r.rr(3), r.rr(4)), EXTRA + 0x100)[1])
    r.at('GetKeyAt__10FEKeyTrackl', lambda: (r.tracecall('key', r.rr(3), r.rr(4)), EXTRA + 0x200)[1])
    r.wf(EXTRA + 0x118, *case[:3]); r.wf(EXTRA + 0x210, *case[3:])
    r.call('Initialize__7CSliderP8FEObject', OBJ, BUTTON)
    return memory(r, OBJ, 0x20), r.trace


def sequence(r, case):
    values, count, direction, current, missing = case
    r.wr(EXTRA, *values)
    return r.call('GetNextEntryInSequence__22HandlerCoopGameOptionsPiUiii', OBJ, 0 if missing else EXTRA, count, direction, current)


def skin(r, case):
    online, count, skin, team, unlocked = case
    file = 'handlerOnlineOptions.cpp' if online else 'handlerMPBots.cpp'
    table = private(file, 'skinLock'); r.wr(private(file, 'NumLockSkins'), count)
    for i, row in enumerate([(3, 2, 0), (9, 1, 1), (7, 14, 1), (8, 2, 0)]): r.wr(table + 12*i, *row)
    r.at('HasCheat__C13CGameSettingsi', lambda: (r.tracecall('cheat', r.rr(4)), unlocked)[1])
    if online: ret = r.call('IsSkinUnlocked__20HandlerOnlineOptionsi', OBJ, skin)
    else: ret = r.call('IsSkinUnlocked__13HandlerMPBotsii', skin, team)
    return ret, r.trace


def object_lists(r, case):
    operation, selected, requested, count = case
    common(r); r.cstring(EXTRA, 'Item')
    def object_at(name):
        index = int(name[4:])
        return BUTTON + 0x100*index if 0 < index <= count else 0
    def find():
        name = r.text(r.rr(4)); r.tracecall('find', name); return object_at(name)
    def script():
        name, action = r.text(r.rr(5)), r.text(r.rr(6)); r.tracecall('script', name, action)
        p = object_at(name)
        if p: r.wr(p + 0x54, EXTRA + (0x100 if action == 'Show' else 0x200))
        return 0 if p else -1
    for i in range(1, count+1):
        r.wr(BUTTON + i*0x100 + 0x54, EXTRA + (0x100 if i == selected else 0x200))
    r.wr(EXTRA + 0x12c, EXTRA + 0x300); r.cstring(EXTRA + 0x300, 'sHoW')
    r.wr(EXTRA + 0x22c, EXTRA + 0x310); r.cstring(EXTRA + 0x310, 'Hide')
    r.at('FindObjectByName__9FEPackagePCc', find); r.at('SetScript__14PackageHandlerP9FEPackagePCcT2', script)
    name = {'set':'SetObjectListIndex__14PackageHandlerPcUi','next':'SetNextObjectListIndex__14PackageHandlerPc','prev':'SetPrevObjectListIndex__14PackageHandlerPc','get':'GetObjectListIndex__14PackageHandlerPc'}[operation]
    ret = r.call(name, OBJ, EXTRA, requested)
    return ret, memory(r, BUTTON, 0x600), r.trace


def join_records(r, case):
    operation, ports, filled, port, device = case
    common(r); sh = addr('g_Shell')
    for i in range(2):
        r.wr(sh + 0x3bd8 + i*12, PKG if filled[i] else 0, 0xaaaaaaaa, 0x1234)
        byte(r, sh + 0x3bdc + i*12, ports[i])
    r.u.mem_write(CORE + 0x321e0, b'\x07\x09\x0b\x0d')
    r.at('GetDeviceIndexFromControlMask__12ShellManagerUl', lambda: (r.tracecall('device_from_mask', r.rr(4)), device)[1])
    r.at('FindDeviceIDFromPortNum__C13CInputManagerUc', lambda: (r.tracecall('device_from_port', r.rr(4)), (r.rr(4)+1) & 3)[1])
    r.at('GetPortNum__C14CDeviceManagerUc', lambda: (r.tracecall('port_from_device', r.rr(4)), port)[1])
    if operation == 'reset': r.call('ResetCoopJoinInfoFromPortNum__12ShellManagerUc', sh, port)
    elif operation == 'set': r.call('SetCoopJoinInfo__12ShellManagerP9FEPackageUl', sh, PKG, 0x12345678)
    else: r.call('DoCoopJoinInfoUpdating__12ShellManager', sh)
    return memory(r, sh + 0x3bd8, 36), memory(r, CORE + 0x321e0, 8), r.trace


def accept_pad(r, case):
    present, index = case
    r.wr(OBJ + 0x40, ENG if present else 0); byte(r, ENG + 0x526c, index)
    return r.call('GetIndexPadLastPressedAccept__12ShellManager', OBJ)


def start_mp(r, case):
    players, flags = case
    common(r); sh = addr('g_Shell'); byte(r, sh + 0x18a0, players)
    r.wr(CORE + 0x32074, flags); byte(r, CORE + 0x321e5, 0xa5)
    r.call('SetStartMPDataFromShell__12ShellManager', sh)
    return memory(r, CORE + 0x3201c, 4), r.rd(CORE + 0x32074), memory(r, CORE + 0x321e5, 1), memory(r, addr('g_GameFlow'), 20)


def special(r, case):
    common(r)
    for event in case: events(r, 'HandlerSpecialFeatures', event)
    state = [r.rd(private('handlerSpecialFeatures.cpp', name)) for name in ['uSETUPMENU.3279','_.tmp_0.3280','uPLAYCREDITS.3281','_.tmp_1.3282']]
    return state, r.trace


def coop_split(r, case):
    common(r); sh = addr('g_Shell')
    r.u.mem_write(sh + 0x18a0, b'\xf5\xe9\xd3\xc8'); r.wr(sh + 0x3bc8, 7); r.wr(sh + 0x3bf0, 3, 5)
    r.wr(addr('g_hMemoryCard'), EXTRA + 0x200)
    r.at('ResetCoopJoinInfo__12ShellManagerSc', lambda: r.tracecall('reset_join', r.rr(3), r.rr(4)))
    r.at('OnEnterInteractiveMode__11CMemoryCard', lambda: r.tracecall('interactive', r.rr(3)))
    if case: ret = r.call('Init__20HandlerCoop4WaySplitPCc', OBJ, EXTRA)
    else: r.call('GotoNextScreen__20HandlerCoop4WaySplit', OBJ); ret = None
    return ret, memory(r, sh, 0x3c00), memory(r, CORE + 0x3201c, 4), r.rd(OBJ + 0x50), r.rd(addr('g_inputMgr')+0x23f8), r.rd(addr('_16HandlerMPProfile.m_lastProfile')), r.trace


def level_start(r, case):
    common(r); r.wr(CORE + 0x32074, 0xdabcdef0)
    r.at('SetNextMissionAndLevel__13CGameSettingsUiUii', lambda: r.tracecall('level', r.rr(4), r.rr(5), r.rr(6)))
    r.call('HandleCoopLevelStartEvent__14PackageHandlerUiP8FEObject', OBJ, hash_name(case), BUTTON)
    return memory(r, CORE + 0x3201c, 4), r.rd(CORE + 0x32074), r.rd(addr('g_GameFlow') + 12), r.trace


def two_player(r, case):
    event, sku, shell_level, single, top = case
    common(r); sh = addr('g_Shell'); r.wr(addr('g_GameSettings')+0xb0, sku); r.wr(addr('g_GameFlow')+4, shell_level)
    byte(r, sh + 0x18a0, 0xb1); byte(r, sh + 0x18a1, 9); byte(r, CORE + 0x321e5, 6)
    r.cstring(EXTRA+0x200, 'player1'); r.cstring(EXTRA+0x300, 'player2')
    r.at('GetProfile__13CGameSettingsUi', lambda: (r.tracecall('profile', r.rr(4)), EXTRA+0x200+r.rr(4)*0x100)[1])
    r.at('WasLastGameSinglePlayer__13CGameSettings', lambda: (r.tracecall('single'), single)[1])
    r.at('IsCoopOnTop__12ShellManagerUc', lambda: (r.tracecall('top', r.rr(4)), top)[1])
    ret = events(r, 'Handler_2P_Menu', event, 0x109)
    return ret, memory(r, sh+0x18a0, 32), r.rd(PKG+0x28), r.trace


def coop_options(r, case):
    event, button, flags, exists, swap, slot, coop = case
    common(r); sh = addr('g_Shell'); r.wr(sh+0x18b8, coop); r.wr(sh+0x18a8, 1)
    r.wr(addr('_22HandlerCoopGameOptions.mSaveSlot'), slot); r.wr(CORE+0x32074, flags)
    r.wr(BUTTON+0x10, hash_name(button), EXTRA+0x300); r.cstring(EXTRA+0x300, button)
    r.wr(addr('g_GameSettings')+0x18, 7)
    r.at('LevelExistsOnCurrentDisc__Fii', lambda: (r.tracecall('disc', r.rr(3), r.rr(4)), exists)[1])
    r.at('SwapDiscs__F15GcSwapDiscsMode', lambda: (r.tracecall('swap', r.rr(3)), swap)[1])
    r.at('RestoreSaveData__13CGameSettingsUiUi', lambda: r.tracecall('restore', r.rr(4), r.rr(5)))
    r.at('SetStartMPDataFromShell__12ShellManager', lambda: r.tracecall('start'))
    def next_value():
        r.tracecall('sequence', r.rr(5), r.rr(6), r.rr(7), memory(r, r.rr(4), 8))
        return 1 if r.rr(7) == 0 else 0
    r.at('GetNextEntryInSequence__22HandlerCoopGameOptionsPiUiii', next_value)
    r.at('SetOptionsProperties__22HandlerCoopGameOptions', lambda: r.tracecall('properties', r.rd(CORE+0x32074)))
    ret = events(r, 'HandlerCoopGameOptions', event)
    # A second dispatch also exercises the warm enable-input cache.
    events(r, 'HandlerCoopGameOptions', 'unhandled')
    return ret, r.rd(CORE+0x32074), r.rd(addr('_22HandlerCoopGameOptions.mSaveSlot')), r.trace


def keyboard(r, case):
    actions, text, cursor, length = case
    common(r); string = EXTRA+0x200; r.cstring(string, text); r.wr(OBJ+0x50, cursor)
    r.slot(VT, 0x30, lambda: (r.tracecall('get_string'), string)[1])
    r.slot(VT, 0x38, lambda: (r.tracecall('length'), length)[1])
    def clear(): r.tracecall('clear'); r.cstring(string, '')
    def back(): r.tracecall('back'); r.cstring(r.rr(4), r.text(r.rr(4))[:-1])
    def add(): r.tracecall('add', r.rr(5)); r.cstring(r.rr(4), r.text(r.rr(4)) + chr(r.rr(5) & 255))
    r.at('ClearStringField__22HandlerVirtualKeyboard', clear)
    r.at('RemovePreviousLetter__22HandlerVirtualKeyboardPc', back)
    r.at('AddLetterToString__22HandlerVirtualKeyboardPcc', add)
    for event, key in actions:
        r.wr(BUTTON+0x10, hash_name(key), EXTRA+0x300); r.cstring(EXTRA+0x300, key)
        events(r, 'HandlerVirtualKeyboard', event)
    return memory(r, string, 24), r.rd(OBJ+0x50), r.trace


def briefing(r, case):
    screen, history, coop, found, setup = case
    common(r); r.cstring(EXTRA, screen); r.wr(OBJ+0x50, history)
    r.wr(addr('g_bCoopMultiplayer'), coop); byte(r, CORE+0x31665, 7)
    def localized():
        r.tracecall('localize', r.text(r.rr(4)), r.rr(5)); return EXTRA+0x200 if found else 0
    r.at('GetString__C11FELocalizerPCcUl', localized); r.wr(EXTRA+0x200, EXTRA+0x300)
    r.u.mem_write(EXTRA+0x300, 'brief'.encode('utf-16-be')+b'\0\0')
    r.at('FindObjectByName__9FEPackagePCc', lambda: (r.tracecall('find', r.text(r.rr(4))), BUTTON+int(r.text(r.rr(4)).split()[1])*0x100)[1])
    def assign(wide):
        r.tracecall('assign', r.rr(3), r.rr(4) if wide else r.text(r.rr(4)))
        r.wr(r.rr(3), EXTRA+0x300 if wide else 0); return r.rr(3)
    r.at('__as__12FEWideStringPCs', lambda: assign(True)); r.at('__as__12FEWideStringPCc', lambda: assign(False))
    events(r, 'HandlerPauseBriefingOverlay', 'Setup Menu' if setup else 'other')
    return r.rd(OBJ+0x50), memory(r, BUTTON, 0xd00), r.trace


def load_game(r, case):
    mission, level, valid, shell_level, current, exists, swap, coop, device = case
    common(r); sh=addr('g_Shell'); save=EXTRA+0x300
    r.wr(sh+0x18b8, coop); r.wr(sh+0x18a8, 1); r.wr(addr('g_GameFlow')+4, shell_level)
    r.u.mem_write(save, bytes([mission, level, 0xa9, 0x57])); r.u.mem_write(CORE+0x31664, bytes(current))
    r.at('GetSelection__C19HandlerSaveSlotBase', lambda: (r.tracecall('selection'), 3)[1])
    r.at('GetSaveData__C13CGameSettingsUiUi', lambda: (r.tracecall('save', r.rr(4), r.rr(5)), save)[1])
    r.at('IsValidLoadGameType__19HandlerSaveSlotBasei', lambda: (r.tracecall('valid', r.rr(4)), valid)[1])
    r.at('LevelExistsOnCurrentDisc__Fii', lambda: (r.tracecall('disc', r.rr(3), r.rr(4)), exists)[1])
    r.at('SwapDiscs__F15GcSwapDiscsMode', lambda: (r.tracecall('swap', r.rr(3)), swap)[1])
    r.at('RestoreSaveData__13CGameSettingsUiUi', lambda: r.tracecall('restore', r.rr(4), r.rr(5)))
    r.at('SetNextMissionAndLevel__13CGameSettingsUiUii', lambda: r.tracecall('level', r.rr(4), r.rr(5), r.rr(6)))
    r.at('UnpauseGame__10CPauseMenu', lambda: r.tracecall('unpause'))
    r.at('GetIndexPadLastPressedAccept__12ShellManager', lambda: (r.tracecall('accept'), device)[1])
    r.at('GetPortNum__C14CDeviceManagerUc', lambda: (r.tracecall('port', r.rr(4)), 2)[1])
    r.at('RecordPackageMarker__7FEnginePCc', lambda: r.tracecall('marker', r.text(r.rr(4))))
    r.call('OnAccept__15HandlerLoadGame', OBJ)
    return memory(r, addr('g_GameFlow'), 20), memory(r, CORE+0x321e0, 8), r.rd(addr('_22HandlerCoopGameOptions.mSaveSlot')), r.trace


def control_masks(r, case):
    coop, count, split, devices, hashes = case
    common(r); sh=addr('g_Shell'); byte(r, sh+0x18a1, 0xab)
    for i, name in enumerate(hashes):
        p=PKG+i*0x100; r.wr(p, p+0x100 if i+1 < len(hashes) else 0)
        r.wr(p+0x10, hash_name(name)); r.wr(p+0x28, [0x1234, 0x103, 0x208, 0x50][i]); byte(r,p+0xac,i)
    r.wr(ENG+0xf4, PKG if hashes else 0)
    r.at('GetNumCoopPlayers__12ShellManager', lambda: (r.tracecall('count'), count)[1])
    r.at('FindNode__C6FEListPCc', lambda: (r.tracecall('find',r.text(r.rr(4))), PKG if r.text(r.rr(4))==split else 0)[1])
    r.at('FindDeviceIDFromPortNum__C13CInputManagerUc', lambda: (r.tracecall('device',r.rr(4)), devices[r.rr(4)])[1])
    name = ('DoCoopPackageControlMaskUpdating' if coop else 'DoPackageControlMaskUpdating')+'__12ShellManager'
    r.call(name, sh); r.call(name, sh)
    return memory(r, PKG, len(hashes)*0x100), memory(r, sh+0x18a0, 4), r.trace


@unittest.skipIf(ppc.Uc is None, 'Optional Unicorn not installed')
class ScreenTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        try:
            _, ppc.original=ppc.load_target()
            ppc.symbols={s['name']:s for s in ppc.original.symbols() if s['section']}
        except (FileNotFoundError, ValueError) as exc: raise unittest.SkipTest(str(exc))

    def cases(self, unit, cases, fn):
        directory=native_directory(unit)
        if not (directory/'compiled.elf').exists(): self.skipTest('Build '+unit+' first')
        for case in cases:
            with self.subTest(case=case):
                self.assertEqual(ppc.normalized(fn(Run(),case)), ppc.normalized(fn(Run(directory),case)))

    def test_slider(self):
        self.cases('slider_initialize', [(1,2,3,4,5,6),(-2,0,4,9,-8,1)], slider)

    def test_sequence(self):
        self.cases('handler_coop_sequence', [(v,n,d,c,p) for v in [(4,8,2),(4,4,2)] for n,d,c,p in itertools.product([0,1,3],[-1,0,1],[2,4,8,99],[0,1])], sequence)

    def test_skin(self):
        for online in [0,1]:
            self.cases('handler_online_skin' if online else 'handler_mp_skin', [(online,*c) for c in itertools.product([-1,0,1,4],[-1,0,1,2,14,15,16,29,30],[-1,0,1],[0,1])], skin)

    def test_object_lists(self):
        self.cases('handler_object_lists', itertools.product(['set','next','prev','get'],[1,2,3],[0,1,3,4],[3]), object_lists)

    def test_join_records(self):
        cases=itertools.product(['reset','set'],[(-1,-1),(0,2),(2,2)],[(0,0),(1,0),(1,1)],[0,2,255],[0,3,255])
        self.cases('shell_coop_join_records', cases, join_records)

    def test_join_update(self):
        self.cases('shell_coop_join_update', [('update',ports,(1,1),0,0) for ports in [(-1,-1),(0,-1),(-1,2),(0,2)]], join_records)

    def test_accept(self):
        self.cases('shell_accept_pad',itertools.product([0,1],[0,3,127,128,255]),accept_pad)

    def test_start(self):
        self.cases('shell_start_mp',itertools.product([0,3,255],[0,0xffffffff,0x12345678]),start_mp)

    def test_special(self):
        self.cases('handler_special_features_events',[('other','Setup Menu','Play Credits Movie','Setup Menu')],special)

    def test_split(self):
        self.cases('handler_coop_split',[0,1],coop_split)

    def test_level_start(self):
        self.cases('handler_coop_level_start',['Select Coop Level %02d'%i for i in range(13)],level_start)

    def test_two_player(self):
        self.cases('handler_two_player_events',itertools.product(['other','Setup Two Player Menu','Load Saved Game','Goto Options'],[0,1],[0,1],[0,1],[0,1]),two_player)

    def test_options(self):
        cases=[(e,b,f,1,0,-1,0) for e,b,f in itertools.product(['other','Pad_Left','Pad_Right','Pad_Back'],['MapSelectionButton','FFSelectionButton','SplitButton','ShareHealthButton','ShareAmmoButton','Unknown'],[0,0x12345678,0xffffffff])]
        cases += [('Start Coop Multiplayer','none',0,*c) for c in itertools.product([0,1],[0,1],[-1,3],[0,1])]
        self.cases('handler_coop_options_events',cases,coop_options)

    def test_keyboard(self):
        actions=[[('Text Entry Setup','A')],[('Clear String Field','A')],[('Clear Profile Name','A')],[('other','A')],[('Key Pressed','A'),('Key Pressed','BACKSPACE'),('Key Pressed','Z')]]
        self.cases('handler_virtual_keyboard_events',itertools.product(actions,['','old','ab\0tail'],[0,1],[0,4,8]),keyboard)

    def test_briefing(self):
        self.cases('handler_briefing_events',itertools.product(['BriefingOverlay.fng','HistoryOverlay.fng','unknown'],[0,1],[0,1],[0,1],[0,1]),briefing)

    def test_load(self):
        cases=[(m,l,1,s,c,1,0,coop,device) for (m,l),s,c,coop,device in itertools.product([(0,1),(1,0),(1,4),(5,4)],[0,1],[(1,4),(5,3)],[0,1],[-1,3])]
        cases += [(m,4,v,1,(1,4),e,s,1,3) for m,v,e,s in itertools.product([1,5],[0,1],[0,1],[0,1])]
        self.cases('handler_loadgame_accept',cases,load_game)

    def test_masks(self):
        lists=[[],['other'],['MPJoin.fng','MPWaitingJoin.fng','4WaySplit.fng','Coop4WaySplit.fng'],['MPWaitingJoin.fng','MPJoin.fng','MPJoin.fng','other']]
        self.cases('shell_control_masks',itertools.product([0,1],[0,1,2],['none','4WaySplit.fng','Coop4WaySplit.fng'],[(0,1,2,3),(-1,2,0,1)],lists),control_masks)


if __name__=='__main__': unittest.main()
