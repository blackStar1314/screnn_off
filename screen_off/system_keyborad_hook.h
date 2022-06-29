#pragma once
#include <Windows.h>
#include <QObject>

class SystemKeyboardHook : public QObject
{
    Q_OBJECT
public:
    static SystemKeyboardHook& Instance();
    ~SystemKeyboardHook();
    bool Hook();
    void UnHook();
private:
    SystemKeyboardHook();
    SystemKeyboardHook(const SystemKeyboardHook&) = delete;
    SystemKeyboardHook& operator= (const SystemKeyboardHook&) = delete;

    inline const HHOOK GetHHook() const { return _hhk; }

    static LRESULT CALLBACK LowLevelKeyboardProc(int, WPARAM, LPARAM);
signals:
    void KeyBoardMessage(int, LPKBDLLHOOKSTRUCT);
private:
    const static int kIdHook = static_cast<int>(WH_KEYBOARD_LL);
    HHOOK _hhk;
};
