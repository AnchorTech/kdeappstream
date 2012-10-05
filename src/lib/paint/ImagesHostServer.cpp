#include "ImagesHostServer.h"

#include <QString>
#include <QStringList>
#include <QRegExp>
#include <QImage>
#include <QImageWriter>
#include <QDateTime>

ImagesHostServer * ImagesHostServer::m_instance = 0;

ImagesHostServer::ImagesHostServer(QObject * parent) :
    QTcpServer(parent),
    m_sem(1)
{
    this->listen(QHostAddress::Any);
    this->setMaxPendingConnections(1);
}

ImagesHostServer * ImagesHostServer::instance(QObject * parent)
{
    if (!m_instance)
        m_instance = new ImagesHostServer(parent);
    return m_instance;
}

void ImagesHostServer::incomingConnection(int socket)
{
    QTcpSocket * s = new QTcpSocket(this);
    connect(s, SIGNAL(readyRead()), this, SLOT(readClient()));
    connect(s, SIGNAL(disconnected()), this, SLOT(discardClient()));
    s->setSocketDescriptor(socket);
}

void ImagesHostServer::readClient()
{
    QTcpSocket * socket = (QTcpSocket*)sender();
    if (socket->canReadLine())
    {
        QStringList tokens = QString(socket->readLine()).split(QRegExp("[ \r\n][ \r\n]*"));
        if (tokens[0] == "GET")
        {
            QStringList resources = tokens[1].split(QRegExp("\\?"));
            if (resources.count() == 1)
            {
                qlonglong t = 0;
                qint64 key = 0;
                QStringList args = resources[0].split(QRegExp("&(amp){0}"));
                foreach (QString arg, args)
                {
                    QStringList kv = arg.split("=");
                    if (kv.count() == 2)
                    {
                        if (kv[0] == "t")
                            t = kv[0].toLongLong();
                        else if (kv[0] == "k")
                            key = kv[0].toLongLong();
                    }

                    if (t && key)
                        break;
                }

                if (t && key)
                    this->sendImage(socket, IDImagePair(t, key));

                goto close_socket;
            }
        }
    }
close_socket:
    socket->close();
    if (socket->state() == QTcpSocket::UnconnectedState)
        delete socket;
}

void ImagesHostServer::discardClient()
{
    QTcpSocket* socket = (QTcpSocket*)sender();
    socket->deleteLater();
}

void ImagesHostServer::sendImage(QIODevice * device, IDImagePair id)
{
    qDebug() << "Reading image of given ID";

    m_sem.acquire();

    if (m_data.contains(id))
    {
        QImage image = m_data[id];
        m_data.remove(id);
        m_sem.release();

        QImageWriter writer(device, "png");
        writer.write(image);
    }
    else
    {
        qDebug() << "Cannot find image of given ID";
        m_sem.release();
    }
}

IDImagePair ImagesHostServer::hostImage(const QImage & image)
{
    QImage img = image.copy();

    m_sem.acquire();

    IDImagePair id(QDateTime::currentDateTime().toMSecsSinceEpoch(), image.cacheKey());

    if (m_data.contains(id))
        m_data[id] = img;
    else
        m_data.insert(id, img);

    while (m_data.count() > 100)
        m_data.remove(m_data.begin().key());

    m_sem.release();

    return id;
}
