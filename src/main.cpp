#include <QtGui/QApplication>

#include <QProcess>
#include <QDebug>
#include <QTimer>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("LD_PRELOAD", "../lib/libkappstream_lib.so");
    env.insert("GAMMARAY_UNSET_PRELOAD", "1");
    QProcess p;
    p.setProcessEnvironment(env);
    p.setProcessChannelMode(QProcess::ForwardedChannels);
    p.start("testapp");

    app.exec();
}
