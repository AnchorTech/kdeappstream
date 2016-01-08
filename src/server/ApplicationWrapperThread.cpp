#include "ApplicationWrapperThread.h"

#include <QProcessEnvironment>

ApplicationWrapperThread::ApplicationWrapperThread(QString applicationName, QObject * parent) :
    QThread(parent),
    applicationName(applicationName),
    _exists(true),
    _crashed(false)
{
}

void ApplicationWrapperThread::run()
{
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("LD_PRELOAD", "/usr/lib/libkappstream.so /usr/lib/libkappstreamhook.so");
    QProcess p;
    p.setProcessEnvironment(env);
    p.setProcessChannelMode(QProcess::ForwardedChannels);
    p.start(applicationName);
    p.waitForFinished(-1);

    switch (p.error())
    {
        case QProcess::FailedToStart:
            _exists = false;
            break;
        case QProcess::Crashed:
            _crashed = false;
            break;
        default:
            break;
    }
}

bool ApplicationWrapperThread::exists() const
{
    return _exists;
}

bool ApplicationWrapperThread::crashed() const
{
    return _crashed;
}
