#ifndef IMAGESHOSTTHREAD_H
#define IMAGESHOSTTHREAD_H

#include <QThread>
#include <QTcpServer>
#include <QTcpSocket>

class ImagesHostServer;

class ImagesHostThread : public QThread
{
        Q_OBJECT

        ImagesHostServer * m_server;

        static ImagesHostThread * m_instance;

        explicit ImagesHostThread(QObject * parent = 0);

    public:

        static ImagesHostThread * instance(QObject * parent = 0);

    signals:

    public slots:

        void hostImage(const uchar * data, qlonglong length);

    protected:

        virtual void run();

};

#endif // IMAGESHOSTTHREAD_H
