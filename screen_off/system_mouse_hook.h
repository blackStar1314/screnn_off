#pragma once
#include <Windows.h>
#include <QObject>

class SystemMouseHook : public QObject
{
    Q_OBJECT
public:
    static SystemMouseHook& Instance();
    ~SystemMouseHook();
    bool Hook();
    void UnHook();
private:
    SystemMouseHook();
    SystemMouseHook(const SystemMouseHook&) = delete;
    SystemMouseHook& operator= (const SystemMouseHook&) = delete;

    inline const HHOOK GetHHook() const { return _hhk; }

    static LRESULT CALLBACK LowLevelMouseProc(int, WPARAM, LPARAM);
signals:
    void MouseMessage(int, LPMSLLHOOKSTRUCT);
private:
    const static int kIdHook = static_cast<int>(WH_MOUSE_LL);
    HHOOK _hhk;
};
