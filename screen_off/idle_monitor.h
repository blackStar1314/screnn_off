#pragma once
#include <QTimer> 
#include <chrono>

class IdleMonitor : public QTimer 
{
    Q_OBJECT
public:
    IdleMonitor(QObject* parent = Q_NULLPTR);
    ~IdleMonitor();

    void SetIdleTime(int);
    void Start();
    void Stop();
private:
#ifdef HOOK
    bool Hook();
    void UnHook();
#endif // HOOK

signals:
    void IdleTime(int);

private:
    unsigned long long GetLastIdleTime();
    void OnTimer();

    std::atomic<unsigned long long> _lastIdleTime;
#ifdef TEST 
    std::chrono::seconds         _idleTime;
#else
    std::chrono::minutes         _idleTime;
#endif // TEST
};
