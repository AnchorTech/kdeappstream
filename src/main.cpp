#include <QtGui/QApplication>

#include <QProcess>
#include <QDebug>
#include <QTimer>


int main(int argc, char *argv[])
{
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("LD_PRELOAD", "/usr/local/lib64/libkappstream.so");
    env.insert("GAMMARAY_UNSET_PRELOAD", "1");
    QProcess p;
    p.setProcessEnvironment(env);
    p.setProcessChannelMode(QProcess::ForwardedChannels);
    p.start("testapp");
    p.waitForFinished(-1);
    return p.exitCode();
}
