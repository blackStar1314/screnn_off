#include "idle_monitor.h"
#include <QDebug>
#include <QTimer>
#include <Windows.h>
#include "system_keyborad_hook.h"
#include "system_mouse_hook.h"

IdleMonitor::IdleMonitor(QObject* parent)
    : QTimer(parent)
   , _lastIdleTime(GetTickCount64())
{
    connect(this, &IdleMonitor::timeout, this, &IdleMonitor::OnTimer);

#ifdef HOOK
    connect(&SystemKeyboardHook::Instance(), &SystemKeyboardHook::KeyBoardMessage, this,
        [this](int msg, LPKBDLLHOOKSTRUCT lpkbhks) {
            qDebug() << "keyboard msg : " << msg;
            _lastIdleTime.store(GetTickCount64());
        });
    connect(&SystemMouseHook::Instance(), &SystemMouseHook::MouseMessage, this,
        [this](int msg, LPMSLLHOOKSTRUCT lpmhks) {
            qDebug() << "mouse msg : " << msg;
            _lastIdleTime.store(GetTickCount64());
        });
#endif // HOOK

}

IdleMonitor::~IdleMonitor()
{
    stop();
}

void IdleMonitor::SetIdleTime(int duration)
{
#ifdef TEST
    _idleTime = std::chrono::seconds(duration);
#else
    _idleTime = std::chrono::minutes(duration);
#endif
}

void IdleMonitor::Start()
{
#ifdef HOOK
    Hook();
#endif
#ifdef TEST
    const auto timeElapsed = std::chrono::seconds(10);
#else
    const auto timeElapsed = std::chrono::minutes(1);
#endif
    start(std::chrono::duration_cast<std::chrono::milliseconds>(timeElapsed).count());
}

void IdleMonitor::Stop()
{
#ifdef HOOK
    UnHook();
#else
    stop();
#endif
}

#ifdef HOOK
bool IdleMonitor::Hook()
{
    return SystemMouseHook::Instance().Hook() && SystemKeyboardHook::Instance().Hook();
}

void IdleMonitor::UnHook()
{
    SystemMouseHook::Instance().UnHook();
    SystemKeyboardHook::Instance().UnHook();
}
#endif // HOOK


unsigned long IdleMonitor::GetLastIdleTime()
{
#ifdef INPUT_INFO
    LASTINPUTINFO lii;
    lii.cbSize = sizeof LASTINPUTINFO;
    ::GetLastInputInfo(&lii);
    _lastIdleTime.store(lii.dwTime);
#else
#ifdef HOOK
#else
#error No implement
#endif
#endif
    return _lastIdleTime.load();
}

void IdleMonitor::OnTimer()
{
    auto currentTime = GetTickCount();
    auto timeElapsed = currentTime - GetLastIdleTime();
    qDebug() << "OnTimer : " << timeElapsed;
    if (timeElapsed > std::chrono::duration_cast<std::chrono::milliseconds>(_idleTime).count()) {
        emit IdleTime(timeElapsed);
    }
}
