"""Isolated original/native PPC checks for save popups and card startup.

Pinned target and optional Unicorn required. External UI, input, audio and card
services are stubbed; these checks do not exercise gameplay or real card I/O.
"""
import itertools
import math
import struct
import unittest
from pathlib import Path
import test_explosion_grenade as ppc
from test_shortlist_partials import Run
from test_frontend_screens import common, addr, private, memory, hash_name, signed, OBJ, PKG, ENG, BUTTON, CORE, EXTRA, VT
from test_feng_animation_lists import copy_stubs
from test_save_flow import glob, gcglob, stub, CARD, DATA, HEAP

ROOT = Path(__file__).resolve().parents[1]
EXACT = {'savepopup_callbacks', 'savepopup_stack', 'memcardpopup_class', 'keyboardpopup_create',
         'languagescreen_instance', 'mcgc_init', 'mcgc_destructor', 'mc_scan_visibly'}


def directory(unit):
    unit = {'memcardpopup_ui':'memcardpopup_class', 'memcardpopup_push':'memcardpopup_class'}.get(unit, unit)
    return ROOT / ('build/reconstruction/units' if unit in EXACT else 'build/comparisons') / (unit if unit in EXACT else 'compare_' + unit)


def wide(r, pointer):
    text = []
    while True:
        c = struct.unpack('>H', memory(r, pointer, 2))[0]
        if not c: return ''.join(text)
        text.append(chr(c)); pointer += 2


def putwide(r, pointer, text): r.u.mem_write(pointer, text.encode('utf-16-be') + b'\0\0')


def base(r):
    common(r); copy_stubs(r)
    r.wr(OBJ, VT, ENG, EXTRA, PKG)
    r.at('__as__12FEWideStringPCs', lambda: r.tracecall('wide', r.rr(3), wide(r, r.rr(4))))
    r.at('__as__12FEWideStringPCc', lambda: r.tracecall('narrow', r.rr(3), r.text(r.rr(4))))
    r.at('USTR_length', lambda: len(wide(r, r.rr(3))))
    r.at('USTR_copy', lambda: putwide(r, r.rr(3), wide(r, r.rr(4))))
    r.at('SetString__14CSaveGamePopupP8FEStringPCUwb', lambda: r.tracecall('set', r.rr(3), r.rr(4), r.rr(5), r.rr(6)))
    for name in ['Push', 'Pop', 'Wait']:
        r.at(name + '__14CSaveGamePopup', lambda name=name: r.tracecall(name, r.rr(3)))


def text_wrap(r, case):
    text, wrap = case; base(r)
    if text is not None: putwide(r, EXTRA, text)
    r.wr(BUTTON + 0x1c, 0x123)
    # This is the function under test, not the usual display callback.
    r.hooks.pop(addr('SetString__14CSaveGamePopupP8FEStringPCUwb'))
    r.call('SetString__14CSaveGamePopupP8FEStringPCUwb', OBJ, BUTTON, EXTRA if text is not None else 0, wrap)
    assert r.rd(BUTTON + 0x1c) == 0x400123
    assert r.trace[0][0:2] == ('wide', BUTTON + 0x60)
    assert '\xa0' not in r.trace[0][2]
    return r.trace, memory(r, BUTTON, 0x70)


def popup_ui(r, case):
    warm, event, duplicate = case; base(r)
    guard = private('MemoryCardPopup.cpp', '_.tmp_0.3409'); table = private('MemoryCardPopup.cpp', 'uOptionEvents.3408')
    r.wr(guard, warm); values = [hash_name('Pad_Button%d' % i) for i in range(4)]
    if duplicate and warm: values = [values[0]] * 4
    r.wr(table, *values); r.wr(OBJ + 0x24, 0, 99)
    r.call('OnUIEvent__16CMemoryCardPopupUiP8FEObject', OBJ, hash_name(event), BUTTON)
    matches = [i for i, value in enumerate(values) if value == hash_name(event)]
    assert r.rd(OBJ + 0x28) == (matches[-1] if matches else 99)
    assert r.rd(OBJ + 0x24) == bool(matches) and r.rd(guard) == 1
    assert len(r.trace) == (0 if warm else 4)
    return memory(r, OBJ, 0x50), memory(r, table, 20), r.trace


def popup_push(r, mask):
    base(r); r.wr(OBJ + 0x18, 7); r.wr(OBJ + 0x20, 2, 3, 4)
    for i in range(5):
        p = BUTTON + i * 0x100; r.wr(OBJ + 0x2c + i * 4, p if mask & (1 << i) else 0); r.wr(p + 0x1c, 0x55)
    r.call('OnPush__16CMemoryCardPopup', OBJ)
    assert [r.rd(OBJ + x) for x in [0x18, 0x20, 0x24, 0x28]] == [0] * 4
    assert r.trace == [('narrow', BUTTON + i * 0x100 + 0x60, '') for i in range(5) if mask & (1 << i)]
    return memory(r, OBJ, 0x50), memory(r, BUTTON, 0x500), r.trace


def popup_display(r, case):
    labels, choices, waiting = case; base(r); r.wr(OBJ + 0x2c, BUTTON); r.wr(OBJ + 0x20, waiting)
    for i in range(4): r.wr(OBJ + 0x30 + i * 4, BUTTON + 0x100 * (i+1) if labels & (1 << i) else 0)
    options = [EXTRA + i * 16 if choices & (1 << i) else 0 for i in range(4)]
    r.at('HideSelections__16CMemoryCardPopup', lambda: r.tracecall('hide', r.rr(3)))
    r.call('Display__16CMemoryCardPopupPCUwN41', OBJ, DATA, *options)
    assert r.rd(OBJ + 0x18) == 1
    assert [r.rd(OBJ + 0x40 + i * 4) for i in range(4)] == [bool(x) for x in options]
    assert r.trace == [('set', OBJ, BUTTON, DATA, 1)] + [('set', OBJ, BUTTON + 0x100*(i+1), options[i], 0) for i in range(4) if labels & (1 << i)] + ([] if waiting else [('hide', OBJ)])
    return memory(r, OBJ, 0x50), r.trace


def keyboard_create(r, case):
    warm, waits = case; base(r)
    guard = private('SaveGameKeyboardPopup.cpp', '_.tmp_0.3535'); message = private('SaveGameKeyboardPopup.cpp', 'message.3534')
    response = private('SaveGameKeyboardPopup.cpp', 'g_eResponse'); r.wr(guard, warm); r.wr(message, hash_name('Reset'))
    r.wr(response, 9); r.wr(addr('g_PauseScreen') + 0x18, 1)
    r.at('BeginSaving__10CPauseMenu', lambda: r.tracecall('begin', r.rr(3)))
    count = [0]
    def wait():
        count[0] += 1; r.tracecall('wait', r.rr(3), r.rd(response))
        if count[0] == waits: r.wr(response, 1)
    r.at('Wait__14CSaveGamePopup', wait)
    r.call('CreateName__22CSaveGameKeyboardPopup', OBJ)
    assert count[0] == waits and r.rd(addr('g_PauseScreen') + 0x18) == 0 and r.rd(response) == 1
    return r.trace, r.rd(guard), r.rd(message)


def keyboard_ui(r, case):
    event, initial, selected = case; base(r)
    r.cstring(OBJ + 0x20, initial); r.wr(OBJ + 0x2c, 5); response = private('SaveGameKeyboardPopup.cpp', 'g_eResponse'); r.wr(response, 0)
    r.u.mem_write(CORE + 0x321e0, bytes([selected])); r.wr(CORE + 4 + selected*0xc598 + 0x2c, 0x1234)
    r.at('AddToNameString__22CSaveGameKeyboardPopupPcc', lambda: r.tracecall('add', r.rr(3), r.rr(4), r.rr(5)))
    r.at('BackspaceNameString__22CSaveGameKeyboardPopupPc', lambda: r.tracecall('backspace', r.rr(3), r.rr(4)))
    r.at('CopyNameIntoSettings__13CGameSettingsPCci', lambda: r.tracecall('copy_name', r.rr(3), r.text(r.rr(4)), r.rr(5)))
    r.at('GetString__C11FELocalizerPCcUl', lambda: (r.tracecall('localize', r.text(r.rr(4)), r.rr(5)), EXTRA + 0x200)[1])
    def convert():
        r.tracecall('convert', r.rr(3)); r.cstring(r.rr(4), 'PLAYER 1'); return r.rr(4)
    r.at('Convert__C12FEWideStringPc', convert)
    r.at('LocalizeString__22CSaveGameKeyboardPopupPCcT1', lambda: r.tracecall('label', r.text(r.rr(4)), r.text(r.rr(5))))
    r.at('FindObjectByName__9FEPackagePCc', lambda: (r.tracecall('find', r.rr(3), r.text(r.rr(4))), BUTTON)[1])
    r.at('printf', lambda: (r.tracecall('printf', r.text(r.rr(3))), 0)[1])
    r.call('OnUIEvent__22CSaveGameKeyboardPopupUiP8FEObject', OBJ, hash_name(event), BUTTON)
    chars = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890.@'
    if event in list(chars) + ['Underscore']:
        assert ('add', OBJ, OBJ+0x20, ord('_' if event == 'Underscore' else event)) in r.trace
    assert r.rd(response) == (event == 'Game Name Aborted' or (event == 'Text Entry Done' and bool(initial)))
    if event == 'Text Entry Setup': assert r.text(OBJ+0x20) == 'PLAYER 1' and r.rd(OBJ+0x2c) == 0
    assert r.trace[-2] == ('find', PKG, 'Profile Name text') and r.rd(BUTTON+0x1c) & 0x400000
    return memory(r, OBJ, 0x30), memory(r, CORE+4+selected*0xc598, 0x40), r.trace, r.rd(response)


def language_instance(r, warm):
    guard = private('SaveGameLanguageScreen.cpp', '_.tmp_0.3349'); obj = private('SaveGameLanguageScreen.cpp', 'p.3348')
    r.wr(guard, warm)
    r.at('__23CSaveGameLanguageScreen', lambda: r.tracecall('construct', r.rr(3)))
    callback = [0]
    def atexit(): callback[0] = r.rr(3); r.tracecall('atexit'); return 0
    r.at('atexit', atexit)
    r.at('_._23CSaveGameLanguageScreen', lambda: r.tracecall('destroy', r.rr(3), r.rr(4)))
    assert r.call('Instance__23CSaveGameLanguageScreen') == obj
    assert r.call('Instance__23CSaveGameLanguageScreen') == obj
    if not warm:
        r.entries['cleanup'] = {'address': callback[0]}; r.call('cleanup')
    assert r.trace == ([] if warm else [('construct', obj), ('atexit',), ('destroy', obj, 2)])
    return r.rd(guard), r.trace


def language_select(r, case):
    frames, waits = case; base(r)
    response = private('SaveGameLanguageScreen.cpp', 'g_eResponse'); r.wr(response, 9); r.wr(OBJ+0x4c, frames); count = [0]
    def wait():
        count[0] += 1; r.tracecall('wait', r.rd(OBJ+0x4c))
        if count[0] == waits: r.wr(response, 1)
    r.at('Wait__14CSaveGamePopup', wait)
    r.at('PlayMovie__12ShellManagerP16_ShellMoviePathsb', lambda: r.tracecall('movie', r.rr(3), r.rr(4), r.rr(5)))
    r.call('SelectLanguage__23CSaveGameLanguageScreen', OBJ)
    assert count[0] == waits
    movies = [x for x in r.trace if x[0] == 'movie']
    assert len(movies) == (frames+waits)//900
    if movies: assert movies[0] == ('movie', addr('g_Shell'), addr('g_Shell')+0x3a48, 1)
    return r.rd(OBJ+0x4c), r.rd(response), r.trace


def language_ui(r, case):
    event, selected, warm = case; base(r)
    f = 'SaveGameLanguageScreen.cpp'; r.wr(OBJ+0x4c, 701); r.wr(private(f, 'g_eResponse'), 0)
    hashes = [101, 103, 107, 109, 113]; r.wr(OBJ+0x20, *hashes); r.wr(OBJ+0x34, 0, 2, 3, 6, 7); r.wr(BUTTON+0x10, hashes[selected])
    for symbol, number, guard, text in [('uLANGUAGE_SELECTED',3365,1,'Language Selected'), ('uSELECTLANGUAGETEXT',3367,2,'Select Language text'), ('uUPDATESCREEN',3369,3,'Update Screen'), ('uCONFIRM',3371,4,'Confirm text')]:
        r.wr(private(f, symbol+'.'+str(number)), hash_name(text)); r.wr(private(f, '_.tmp_%d.%d' % (guard, number+1)), warm)
    r.wr(addr('g_pMiscTextDatabase'), DATA)
    r.at('Set_Language__13CGameSettings10ELanguages', lambda: r.tracecall('language', r.rr(4)))
    r.at('GetLocaleID__23CSaveGameLanguageScreeni', lambda: (r.tracecall('locale_index', r.rr(4)), 30+r.rr(4))[1])
    r.at('GetButtonName__23CSaveGameLanguageScreeni', lambda: 10+r.rr(4))
    r.at('LOCALE_setstate', lambda: r.tracecall('locale', r.rr(3), r.rr(4), r.rr(5)))
    r.at('LOCALE_getstr', lambda: (r.tracecall('get_string', r.rr(3), r.rr(4)), DATA+0x100+16*r.rr(4))[1])
    r.at('FindObjectByHash__9FEPackageUl', lambda: (r.tracecall('find_hash', r.rr(3), r.rr(4)), BUTTON)[1])
    r.call('OnUIEvent__23CSaveGameLanguageScreenUiP8FEObject', OBJ, hash_name(event), BUTTON)
    assert r.rd(OBJ+0x4c) == 0 and r.rd(private(f, 'g_eResponse')) == (event == 'Language Selected')
    if event == 'Language Selected': assert r.trace[-1] == ('language', [0,2,3,6,7][selected])
    if event == 'Update Screen': assert len([x for x in r.trace if x[0] == 'set']) == 7 and ('locale_index', selected) in r.trace
    return memory(r, OBJ, 0x50), r.trace


def card_init(r, size):
    base(r); r.u.mem_write(OBJ+0x58, b'\xaa'*48)
    r.at('GetMiscTextDatabase__C11CMemoryCard', lambda: DATA)
    r.at('LOCALE_getstr', lambda: (r.tracecall('locale', r.rr(3), r.rr(4)), DATA+16*r.rr(4))[1])
    def wcstombs():
        r.tracecall('wcstombs', r.rr(4), r.rr(5)); r.cstring(r.rr(3), 'SAVE %d'); return 7
    r.at('wcstombs', wcstombs)
    r.at('MEM_clear', lambda: r.u.mem_write(r.rr(3), bytes(r.rr(4))))
    r.at('GetScratchpadSize__C11CMemoryCard', lambda: size)
    r.wr(addr('FILENAME_ALL_FILES__6Realmc') if 'FILENAME_ALL_FILES__6Realmc' in ppc.symbols else addr('_6Realmc.FILENAME_ALL_FILES'), EXTRA)
    r.call('Init__13CMemoryCardGC', OBJ)
    assert r.text(gcglob('comment1.3245')) == 'SAVE -103' and r.rd(OBJ+0x5c) == size
    assert r.rd(OBJ+0x84) == 0
    return memory(r, OBJ+0x58, 48), memory(r, gcglob('comment1.3245'), 32), r.trace


def card_start(r, case):
    blocks, adjust = case; base(r); r.wr(OBJ+8, CARD); r.wr(CARD, VT+0x100); r.wr(HEAP, VT+0x200)
    r.u.mem_write(OBJ+0xc, b'\xaa'*76); source = bytes((i*13+7)&255 for i in range(48)); r.u.mem_write(OBJ+0x58, source)
    r.at('Instance__9MohLoader', lambda: BUTTON); r.wr(BUTTON+0x5c, 7)
    r.at('Init__13CMemoryCardGC', lambda: r.tracecall('init', r.rr(3), r.rd(BUTTON+0x5c)))
    r.at('MEM_clear', lambda: r.u.mem_write(r.rr(3), bytes(r.rr(4))))
    r.at('MEM_copy', lambda: r.u.mem_write(r.rr(3), memory(r, r.rr(4), r.rr(5))))
    r.slot(VT+0x100, 0xd8, lambda: (r.tracecall('calculator', r.rr(3)), HEAP)[1], adjust)
    r.slot(VT+0x200, 8, lambda: r.tracecall('clear', r.rr(3)), adjust)
    r.slot(VT+0x200, 0x18, lambda: r.tracecall('file', r.rr(3), memory(r,r.rr(4),4), r.rr(5)), adjust)
    r.slot(VT+0x200, 0x10, lambda: (r.tracecall('blocks', r.rr(3)), blocks)[1], adjust)
    r.slot(VT+0x100, 0x18, lambda: r.tracecall('start', r.rr(3), r.rr(4)), adjust)
    r.at('TIMER_elapsedticks', lambda: (r.tracecall('timer'), 99)[1])
    r.call('IssueStart__13CMemoryCardGC', OBJ)
    assert memory(r, OBJ+0xc, 48) == source and r.rd(OBJ+0x90) == 1 and r.rd(BUTTON+0x5c) == 0
    assert [r.rd(OBJ+x) for x in [0x3c,0x4c,0x50]] == [blocks&0xffffffff,1,blocks&0xffffffff]
    assert r.trace[-1] == ('timer',)
    return memory(r,OBJ,0xa4), r.trace


def card_destructor(r, case):
    flag, adjust = case; base(r); r.wr(OBJ+8, CARD); r.wr(CARD, VT+0x100); r.wr(gcglob('g_pMCInstance'), OBJ)
    r.slot(VT+0x100, 0x10, lambda: r.tracecall('release', r.rr(3)), adjust)
    r.at('_._11CMemoryCard', lambda: r.tracecall('base_destroy', r.rr(3), r.rr(4)))
    r.call('_._13CMemoryCardGC', OBJ, flag)
    assert r.rd(gcglob('g_pMCInstance')) == 0
    assert r.trace == [('release', CARD+adjust), ('base_destroy', OBJ, flag)]
    return r.trace, memory(r,OBJ,0x10)


def scan(r, case):
    index, available, text, adjust = case; base(r); r.wr(OBJ+4, VT); r.wr(glob('g_bIsBusy'), 9)
    r.at('Instance__16CMemoryCardPopup', lambda: PKG)
    r.at('GetDeviceCount__C11CMemoryCard', lambda: len(available))
    r.at('GetDevice__C11CMemoryCardUi', lambda: CARD + 0x100*r.rr(4))
    r.wr(addr('g_pPopUpMessageHandler'), HEAP)
    if text is not None: r.cstring(DATA, text)
    r.at('GetString__17CUserMessageTablei', lambda: (r.tracecall('message', r.rr(3), r.rr(4)), DATA if text is not None else 0)[1])
    for i,a in enumerate(available):
        r.wr(CARD+i*0x100, VT+0x100+i*0x100); putwide(r, EXTRA+i*0x100, 'Card '+str(i))
        r.slot(VT+0x100+i*0x100,0x18,lambda a=a:a,adjust)
        r.slot(VT+0x100+i*0x100,8,lambda i=i:EXTRA+i*0x100,adjust)
    r.at('Display__16CMemoryCardPopupPCUwN41', lambda: r.tracecall('display',r.rr(3),wide(r,r.rr(4)),*[r.rr(i) for i in range(5,9)]))
    r.slot(VT,0x68,lambda:r.tracecall('scan',r.rr(3),r.rr(4),r.rr(5)),adjust)
    checks=[0]
    def check(): checks[0]+=1;return 2 if checks[0]%2==0 else 0
    r.slot(VT,0x70,check,adjust)
    ticks=[0]
    def tick(): ticks[0]+=250;return ticks[0]
    r.at('TIMER_gettick',tick);r.at('TIMER_getfrequency',lambda:1000)
    r.at('Wait__11CMemoryCard',lambda:r.tracecall('wait',r.rd(glob('g_bIsBusy'))))
    r.at('CancelReplyRequest__11CMemoryCard',lambda:r.tracecall('cancel',r.rr(3)))
    r.call('ScanVisibly__11CMemoryCardRQ211CMemoryCard8ScanTaski',OBJ,BUTTON,index)
    visits=[i for i in range(len(available)) if (index<0 or i==index) and available[i]]
    assert [x[3] for x in r.trace if x[0]=='scan']==[CARD+i*0x100 for i in visits]
    assert len([x for x in r.trace if x[0]=='wait'])==5*len(visits)
    assert r.rd(glob('g_bIsBusy'))==0 and r.rd(PKG+0x18)==0
    return r.trace


def stack(r, case):
    count, adjust = case
    header=EXTRA; nodes=[EXTRA+0x100+i*16 for i in range(count)]; new=HEAP
    r.wr(private('SaveGameCore.cpp','g_listPopups')+4,header)
    r.wr(header, nodes[0] if nodes else header, nodes[-1] if nodes else header)
    for i,n in enumerate(nodes):r.wr(n,nodes[i+1] if i+1<count else header,nodes[i-1] if i else header,BUTTON+i*0x100)
    r.wr(OBJ,VT);r.wr(OBJ+0x14,29)
    r.at('__builtin_new',lambda:(r.tracecall('new',r.rr(3)),new)[1]);r.at('__builtin_delete',lambda:r.tracecall('delete',r.rr(3)))
    r.slot(VT,0x40,lambda:r.tracecall('on_push',r.rr(3)),adjust)
    before=memory(r,header,8),memory(r,EXTRA+0x100,count*16)
    r.call('Push__14CSaveGamePopup',OBJ)
    assert r.rd(header)==new and r.rd(new+8)==OBJ and r.rd(OBJ+0x14)==0
    r.call('Pop__14CSaveGamePopup',OBJ)
    assert (memory(r,header,8),memory(r,EXTRA+0x100,count*16))==before
    assert r.trace==[('new',12),('on_push',OBJ+adjust),('delete',new)]
    return r.trace


def draw(r, case):
    displayed, sprite, frame, mode, scale = case; base(r)
    r.wr(OBJ+0x10,BUTTON if sprite else 0,frame,displayed)
    r.wf(addr('_7CScreen.m_sWidthScale'),scale);r.wf(addr('_7CScreen.m_sHeightScale'),0.75)
    r.wr(addr('_7CScreen.m_sVideoMode'),mode)
    r.at('Render__7FEngine',lambda:r.tracecall('render',r.rr(3)))
    def trig(label,fn):
        r.tracecall(label,r.fr(1));r.fw(1,struct.unpack('>f',struct.pack('>f',fn(r.fr(1))))[0])
    r.at('cosf',lambda:trig('cos',math.cos));r.at('sinf',lambda:trig('sin',math.sin))
    r.at('DrawPoly__7CSpriteP15_spritepolyvertiQ27CSprite13EPolyPrimType',lambda:r.tracecall('poly',r.rr(3),memory(r,r.rr(4),96),r.rr(5),r.rr(6)))
    r.call('Draw__14CSaveGamePopup',OBJ)
    active=displayed and sprite
    assert r.rd(OBJ+0x14) == ((0 if frame+1>32 else frame+1) if active else frame)
    assert len([x for x in r.trace if x[0]=='poly'])==(2 if active else 0)
    for x in r.trace:
        if x[0]=='poly':
            assert x[3:]==(4,5)
            for i in range(4):assert x[2][i*24+20:i*24+24]==b'\x80\x80\x80\xff'
    return r.rd(OBJ+0x14),r.trace


def frame_wait(r, case):
    background,shell,silent,count,counter,adjust=case[:6];base(r)
    insert_during_draw = len(case) > 6 and case[6]
    r.wr(private('SaveGameCore.cpp','g_bBackgroundEnabled'),background);r.wr(addr('g_bDisableSound'),silent);r.wf(addr('g_frameDel'),0.125)
    r.wr(addr('g_GameFlow')+4,shell);r.wr(addr('_7CScreen.m_sWidth'),640);r.wr(addr('_7CScreen.m_sHeight'),448)
    header=HEAP;nodes=[HEAP+0x100+i*16 for i in range(count)]
    r.wr(private('SaveGameCore.cpp','g_listPopups')+4,header);r.wr(header,nodes[0],nodes[-1])
    inserted = [False]
    def draw_popup():
        r.tracecall('draw_popup', r.rr(3))
        if insert_during_draw and not inserted[0]:
            # A new front node must not move this traversal's captured end.
            inserted[0] = True
            node, obj = HEAP+0x500, BUTTON+0x500
            r.wr(node, nodes[0], header, obj); r.wr(header, node); r.wr(nodes[0]+4, node)
            r.wr(obj, VT+0x700); r.slot(VT+0x700, 0x18, draw_popup, adjust)
    for i,n in enumerate(nodes):
        obj=BUTTON+i*0x100;table=VT+i*0x100;r.wr(n,nodes[i+1] if i+1<count else header,nodes[i-1] if i else header,obj);r.wr(obj,table)
        r.slot(table,0x10,lambda:r.tracecall('update_popup',r.rr(3),r.fr(1)),adjust)
        r.slot(table,0x18,draw_popup,adjust)
    r.at('Gameloop_Draw__Ff',lambda:r.tracecall('background',r.fr(1)))
    r.at('Update__13CInputManagerf',lambda:r.tracecall('input',r.rr(3),r.fr(1)))
    r.at('SetRect__9CViewportffff',lambda:r.tracecall('rect',r.rr(3),*[r.fr(i) for i in range(1,5)]))
    r.at('Set__8CFrustumRC7CCameraR7CScreenR9CViewport',lambda:r.tracecall('frustum',*[r.rr(i) for i in range(3,7)]))
    r.at('Transform__8CFrustumRC7CMatrix',lambda:r.tracecall('transform',r.rr(3),r.rr(4)))
    r.at('GetOrthographicDrawContext__C11CScreenEAGL',lambda:(r.tracecall('context',r.rr(3)),CARD)[1])
    r.wr(CARD+0x224,VT+0x800);r.slot(VT+0x800,0x10,lambda:r.tracecall('begin',r.rr(3),r.rr(4)),adjust);r.slot(VT+0x800,0x18,lambda:r.tracecall('end',r.rr(3)),adjust)
    for cls in ['10CPauseMenu','12ShellManager']:
        for op in ['UpdateUnplugged','DrawUnplugged']:
            r.at(op+'__'+cls,lambda op=op,cls=cls:r.tracecall(op+cls,r.rr(3)))
    sound=addr('g_SoundMan');r.wr(sound+0x86f8,counter);r.wf(sound+0x86f4,-1)
    for op in ['SNDSYS_entercritical','SNDSYS_leavecritical','SoundSync__Fv','Update__7GBALinkv','WaitScreen__Fv']:
        r.at(op,lambda op=op:r.tracecall(op))
    r.at('ServiceStreams__9cSoundMan',lambda:r.tracecall('streams',r.rr(3)))
    r.at('Update__13SoundSchedulef',lambda:r.tracecall('schedule',r.rr(3),r.fr(1)))
    r.at('SNDPROFILE_iopcpu',lambda:(r.tracecall('cpu'),r.fw(1,0.25))[1])
    r.at('SYNCTASK_run',lambda:r.tracecall('sync',r.rr(3)))
    r.hooks.pop(addr('Wait__14CSaveGamePopup'))
    r.call('Wait__14CSaveGamePopup',OBJ)
    assert [x[1] for x in r.trace if x[0]=='draw_popup']==[BUTTON+i*0x100+adjust for i in reversed(range(count))]
    assert ('transform',addr('g_camera')+0x170,addr('g_camera')+0x50) in r.trace
    assert r.rd(sound+0x86f8)==((counter+(not silent))&0xffffffff)
    return r.trace,memory(r,sound+0x86f4,8)


@unittest.skipIf(ppc.Uc is None,'Optional Unicorn not installed')
class SavePopupTests(unittest.TestCase):
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

    def test_wrap(self):self.cases('savepopup_setstring',itertools.product([None,'','a','a'*80,' '*45,'hello world '*12,'a\xa0b '*25,'A'*37+' '+'B'*40,'語文 '+ 'x '*70],[0,1]),text_wrap)
    def test_ui(self):self.cases('memcardpopup_ui',itertools.product([0,1],['Pad_Button%d'%i for i in range(4)]+['Other'],[0,1]),popup_ui)
    def test_push(self):self.cases('memcardpopup_push',range(32),popup_push)
    def test_display(self):self.cases('memcardpopup_display',itertools.product([0,5,10,15],range(16),[0,1]),popup_display)
    def test_keyboard_create(self):self.cases('keyboardpopup_create',itertools.product([0,1],[1,3]),keyboard_create)
    def test_keyboard_ui(self):self.cases('keyboardpopup_ui',itertools.product(list('ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890.@')+['Underscore','Backspace','Clear Profile Name','Text Entry Done','Text Entry Setup','Game Name Aborted','Other'],['','TEST'],[0,3]),keyboard_ui)
    def test_instance(self):self.cases('languagescreen_instance',[0,1],language_instance)
    def test_select(self):self.cases('languagescreen_select',itertools.product([0,898,899,900],[1,3]),language_select)
    def test_language_ui(self):self.cases('languagescreen_ui',itertools.product(['Language Selected','Update Screen','Other'],range(5),[0,1]),language_ui)
    def test_card_init(self):self.cases('mcgc_init',[0,16,0xc5a0],card_init)
    def test_card_start(self):self.cases('mcgc_issue_start',itertools.product([0,1,32],[-4,0,8]),card_start)
    def test_card_destructor(self):self.cases('mcgc_destructor',itertools.product([0,1,2,3],[-4,0,8]),card_destructor)
    def test_scan(self):self.cases('mc_scan_visibly',itertools.product([-1,0,1,2],[(0,0),(1,0),(0,1),(1,1)],[None,'Scanning %s','Plain text','%s + %s'],[-4,0,8]),scan)
    def test_stack(self):self.cases('savepopup_stack',itertools.product([0,1,3],[-4,0,8]),stack)
    def test_draw(self):self.cases('savepopup_draw',itertools.product([0,1],[0,1],[0,1,16,32,33],[0,1],[0.75,1.0]),draw)
    def test_wait_list_insertion(self):self.cases('savepopup_wait',[(1,1,0,3,20,a,True) for a in [-4,0,8]],frame_wait)
    def test_wait(self):self.cases('savepopup_wait',itertools.product([0,1],[0,1],[0,1],[1,3],[0,19,20,-20],[-4,0,8]),frame_wait)


if __name__=='__main__':unittest.main()
