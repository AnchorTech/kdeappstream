#include "ImagesHostServer.h"

#include <QString>
#include <QStringList>
#include <QRegExp>
#include <QImage>
#include <QImageWriter>
#include <QDateTime>
#include <QBuffer>

ImagesHostServer * ImagesHostServer::m_instance = 0;

ImagesHostServer::ImagesHostServer(QObject * parent) :
    QTcpServer(parent),
    m_sem(1)
{
    qDebug() << "Listening for images fetching:" << this->listen(QHostAddress::Any);
}

ImagesHostServer * ImagesHostServer::instance(QObject * parent)
{
    if (!m_instance)
        m_instance = new ImagesHostServer(parent);
    return m_instance;
}

void ImagesHostServer::incomingConnection(int socket)
{
    qDebug() << "IncomingConnection!" << socket;
    QTcpSocket * s = new QTcpSocket(this);
    connect(s, SIGNAL(readyRead()), this, SLOT(readClient()));
    connect(s, SIGNAL(disconnected()), this, SLOT(discardClient()));
    s->setSocketDescriptor(socket);
}

void ImagesHostServer::readClient()
{
    qDebug() << "readClient!";
    QTcpSocket * socket = (QTcpSocket*)sender();
    if (socket->canReadLine())
    {
        QStringList tokens = QString(socket->readLine()).split(QRegExp("[ \r\n][ \r\n]*"));
        if (tokens[0] == "GET")
        {
            QStringList resources = tokens[1].split(QRegExp("\\?"));
            qDebug() << resources;
            if (resources.count() == 1)
            {
                qlonglong t = 0;
                qint64 key = 0;
                QStringList args = resources[0].split(QRegExp("(\\\\|/|_|\\.)"), QString::SkipEmptyParts);
                qDebug() << args;
                if (args.count() == 3)
                {
                    t = args[0].toLongLong();
                    key = args[1].toLongLong();
                    this->sendStatus(socket, 200);
                    this->sendImage(socket, IDImagePair(t, key));
                    goto close_socket;
                }
            }
        }
    }
    this->sendStatus(socket, 400);
close_socket:
    socket->close();
    if (socket->state() == QTcpSocket::UnconnectedState)
        delete socket;
}

void ImagesHostServer::discardClient()
{
    qDebug() << "discardClient!";
    QTcpSocket* socket = (QTcpSocket*)sender();
    socket->deleteLater();
}

void ImagesHostServer::sendStatus(QIODevice * device, int status)
{
    switch (status)
    {
        case 200:
            device->write("HTTP/1.0 200 Ok\r\n"
                          "Content-Type: image/png; charset=\"utf-8\"\r\n"
                          "\r\n");
            break;
        case 400:
        default:
            device->write("HTTP/1.0 400 Bad Request\r\n"
                          "Content-Type: text/html; charset=\"utf-8\"\r\n"
                          "\r\n");
    }
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

        QByteArray ba;
        QBuffer buf(&ba);
        image.save(&buf, "PNG");
        device->write(ba);
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
