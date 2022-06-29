#include "system_mouse_hook.h"

SystemMouseHook& SystemMouseHook::Instance()
{
    static SystemMouseHook instance;
    return instance;
}

SystemMouseHook::~SystemMouseHook()
{
    UnHook();
}

bool SystemMouseHook::Hook()
{
    _hhk = SetWindowsHookExW(kIdHook,
        LowLevelMouseProc,
        GetModuleHandleW(NULL),
        0);
    return _hhk != nullptr;
}

void SystemMouseHook::UnHook()
{
    if (_hhk) {
        UnhookWindowsHookEx(_hhk);
        _hhk = nullptr;
    }
}

SystemMouseHook::SystemMouseHook()
    : _hhk(nullptr)
{}

LRESULT SystemMouseHook::LowLevelMouseProc(int code, WPARAM wParam, LPARAM lParam)
{
    // https://docs.microsoft.com/en-us/previous-versions/windows/desktop/legacy/ms644986(v=vs.85)
    if (code >= 0) {
        emit SystemMouseHook::Instance().MouseMessage(wParam, reinterpret_cast<LPMSLLHOOKSTRUCT>(lParam));
    }
    return CallNextHookEx(SystemMouseHook::Instance().GetHHook(), code, wParam, lParam);
}
