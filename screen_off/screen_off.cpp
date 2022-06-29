#include "screen_off.h"
#include <QDebug>
#include <QCloseEvent>
#include <QMenu>
#include <QSystemTrayIcon>
#include <Windows.h>
#include "idle_monitor.h"

screen_off::screen_off(QWidget *parent)
    : QDialog(parent)
    , _idleMontor(std::make_shared<IdleMonitor>())
    , _monitorState(false)
{
    ui.setupUi(this);
    auto flag = windowFlags();
    flag |= Qt::WindowMinimizeButtonHint;
    flag |= Qt::WindowCloseButtonHint;
    setWindowFlags(flag);

    CreateSystemTrayIcon();

    this->setFixedSize(QSize(width(), height()));
    _idleMontor->SetIdleTime(1);

    QObject::connect(ui.m_idleTimeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(OnIdleTimeValueChanged(int)));
    QObject::connect(ui.m_mintorState, SIGNAL(clicked()), this, SLOT(OnClicked()));
    QObject::connect(_idleMontor.get(), SIGNAL(IdleTime(int)), this, SLOT(OnIdleTime(int)));
    }

screen_off::~screen_off()
{}

void screen_off::ChangeMonitorState() 
{
    ui.m_mintorState->setText(_monitorState ? QString(u8"停止") : QString(u8"启动"));
    ui.m_idleTimeSpinBox->setDisabled(_monitorState);
    if (_monitorState) {
        _idleMontor->Start();
    }
    else {
        _idleMontor->Stop();
    } 
}

void screen_off::changeEvent(QEvent* e)
{
    if (e->WindowStateChange && this->windowState() == Qt::WindowMinimized) {
        auto flags = windowFlags();
        flags |= Qt::Tool;
        this->setWindowFlags(flags);
        this->hide();
        _tray->show();
        //_tray->showMessage(windowTitle(), u8"666666");
        e->ignore();
    }
}

void screen_off::closeEvent(QCloseEvent* e)
{
    this->close();
}

void screen_off::CreateSystemTrayIcon()
{
    auto showAction = new QAction(QStringLiteral("显示"));
    auto hidAction = new QAction(QStringLiteral("隐藏"));

    QObject::connect(showAction, &QAction::triggered, this, [this]() {
        auto flags = windowFlags();
        flags ^= Qt::Tool;
        this->setWindowFlags(flags);
        this->showNormal();
        });
    QObject::connect(hidAction, &QAction::triggered, this, [this]() {
        this->showMinimized();
        });

    auto menu = new QMenu(this);
    menu->addAction(showAction);
    menu->addAction(hidAction);

    _tray = new QSystemTrayIcon(this);
    _tray->setIcon(QIcon("screen_off.ico"));
    _tray->setContextMenu(menu);
}

void screen_off::DelMonitorPower(bool off)
{
    ::SendMessageW(::GetShellWindow(), WM_SYSCOMMAND, SC_MONITORPOWER, off ? 2 : -1);
}

void screen_off::OnIdleTimeValueChanged(int value)
{
    _idleMontor->SetIdleTime(value);
}

void screen_off::OnClicked()
{
    _monitorState = !_monitorState; // 取反
    ChangeMonitorState();
}

void screen_off::OnIdleTime(int timeElapsed)
{
    qDebug() << "On Idle time : " << timeElapsed;
    LockWorkStation();
    DelMonitorPower(true);
}

void screen_off::OnKeyBoardEvent(int vkCode, int keyStore)
{
}
