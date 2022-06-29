#include "system_keyborad_hook.h"
#include <QDebug>

SystemKeyboardHook& SystemKeyboardHook::Instance()
{
    static SystemKeyboardHook instance;
    return instance;
}

SystemKeyboardHook::~SystemKeyboardHook()
{
    UnHook();
}

bool SystemKeyboardHook::Hook()
{
    _hhk = SetWindowsHookExW(kIdHook,
        LowLevelKeyboardProc,
        GetModuleHandleW(NULL),
        0);
    return _hhk != nullptr;
}

void SystemKeyboardHook::UnHook()
{
    if (_hhk) {
        UnhookWindowsHookEx(_hhk);
        _hhk = nullptr;
    }
}

SystemKeyboardHook::SystemKeyboardHook()
    : _hhk(nullptr)
{

}

LRESULT SystemKeyboardHook::LowLevelKeyboardProc(int code, WPARAM wParam, LPARAM lParam)
{
    // https://docs.microsoft.com/en-us/previous-versions/windows/desktop/legacy/ms644984(v=vs.85)
    if (code >= 0) {
        emit SystemKeyboardHook::Instance().KeyBoardMessage(wParam, reinterpret_cast<LPKBDLLHOOKSTRUCT>(lParam));
    }
    return CallNextHookEx(SystemKeyboardHook::Instance().GetHHook(), code, wParam, lParam);
}
