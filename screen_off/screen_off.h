#pragma once

#include <QtWidgets/QDialog>
#include <memory>
#include "ui_screen_off.h"

class IdleMonitor;
class QSystemTrayIcon;
class screen_off : public QDialog
{
    Q_OBJECT

public:
    screen_off(QWidget *parent = nullptr);
    ~screen_off();

private:
    void ChangeMonitorState();
    void changeEvent(QEvent* e) override;
    void closeEvent(QCloseEvent* e) override;
    void CreateSystemTrayIcon();
    static void DelMonitorPower(bool off);
private slots:
    void OnIdleTimeValueChanged(int value);
    void OnClicked();
    void OnIdleTime(int);
    void OnKeyBoardEvent(int vkCode, int keyStore);

private:
    Ui::screen_offClass ui;

    std::shared_ptr<IdleMonitor> _idleMontor;
    bool                         _monitorState;
    QSystemTrayIcon* _tray;
};
