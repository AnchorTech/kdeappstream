#include "ImagesHostThread.h"
#include "ImagesHostServer.h"

ImagesHostThread * ImagesHostThread::m_instance = 0;

ImagesHostThread::ImagesHostThread(QObject *parent) :
    QThread(parent),
    m_server(new ImagesHostServer())
{
}

void ImagesHostThread::run()
{
    this->exec();
}

ImagesHostThread * ImagesHostThread::instance(QObject * parent)
{
    if (!m_instance)
        m_instance = new ImagesHostThread(parent);
    return m_instance;
}

void ImagesHostThread::hostImage(const uchar * data, qlonglong length)
{
    m_server->hostImage(data, length);
}
