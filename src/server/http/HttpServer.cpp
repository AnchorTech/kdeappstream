#include "HttpServer.h"
#include <QTcpSocket>
#include <QFile>
#include <QStringList>
#include <QRegExp>
#include <kstandarddirs.h>
#include <QUuid>

HttpServer::HttpServer(uint _port, QObject * parent) :
    QTcpServer(parent),
    port(_port),
    disabled(false)
{
    listen(QHostAddress::Any, port);
}

void HttpServer::incomingConnection(int socket)
{
    QTcpSocket * s = new QTcpSocket(this);
    connect(s, SIGNAL(readyRead()), this, SLOT(readClient()));
    connect(s, SIGNAL(disconnected()), this, SLOT(discardClient()));
    s->setSocketDescriptor(socket);
}

void HttpServer::readClient()
{
    QTcpSocket * socket = (QTcpSocket*)sender();
    if (socket->canReadLine())
    {
        QTextStream os(socket);
        os.setAutoDetectUnicode(true);

        QStringList tokens = QString(socket->readLine()).split(QRegExp("[ \r\n][ \r\n]*"));
        if (tokens[0] == "GET")
        {
            QStringList resources = tokens[1].split(QRegExp("\\?"));
            if (resources.count() == 2)
            {
                if (resources[0] == "/canvas.html")
                {
                    QStringList args = resources[1].split(QRegExp("&(amp){0}"));
                    foreach (QString arg, args)
                    {
                        QStringList kv = arg.split("=");
                        if (kv.count() == 2 && kv[0] == "app")
                        {
                            this->sendCanvas(os, kv[1]);
                            goto close_socket;
                        }
                    }
                    this->sendStatus(os, 400);
                    goto close_socket;
                }
                else
                {
                    this->sendIndex(os);
                }
            }
            else
            {
                this->sendIndex(os);
            }
        }
        else
        {
            this->sendStatus(os, 400);
        }
    }
close_socket:
    socket->close();
    if (socket->state() == QTcpSocket::UnconnectedState)
        delete socket;
}

void HttpServer::discardClient()
{
    QTcpSocket* socket = (QTcpSocket*)sender();
    socket->deleteLater();
}

void HttpServer::sendStatus(QTextStream & os, int status)
{
    switch (status)
    {
        case 200:
            os << "HTTP/1.0 200 Ok\r\n"
                  "Content-Type: text/html; charset=\"utf-8\"\r\n"
                  "\r\n";
            break;
        case 400:
        default:
            os << "HTTP/1.0 400 Bad Request\r\n"
                  "Content-Type: text/html; charset=\"utf-8\"\r\n"
                  "\r\n";
    }
}

void HttpServer::sendIndex(QTextStream & os)
{
    this->sendStatus(os, 200);

    KStandardDirs sd;
    QStringList sl = sd.findDirs("data", "kdeappstream/data/");
    if (!sl.count())
    {
        qDebug() << "File not found.";
        os << "<html><head></head><body>Server internal error.</body></head>";
        return;
    }

    foreach (QString dir, sl)
    {
        QFile file(dir + "/index.html");
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
            continue;

        QString headTag("<head>");
        QTextStream in(&file);
        while(!in.atEnd())
        {
            QString line = in.readLine();
            if (line.contains(headTag))
            {
                line.replace(headTag, "<head>"
                             "<script type=\"text/javascript\">"
                             "var sessionID = '" + QUuid::createUuid().toString() + "';</script>");
                os << line << '\n';
                break;
            }
            os << line << '\n';
        }
        os << in.readAll();
        return;
    }
    qDebug() << "Can't read index";
}

void HttpServer::sendCanvas(QTextStream & os, QString applicationName)
{
    this->sendStatus(os, 200);

    KStandardDirs sd;
    QStringList sl = sd.findDirs("data", "kdeappstream/data/");
    if (!sl.count())
    {
        qDebug() << "File not found.";
        os << "<html><head></head><body>Server internal error.</body></head>";
        return;
    }

    foreach (QString dir, sl)
    {
        QFile file(dir + "/canvas.html");
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
            continue;

        QString headTag("<head>");
        QTextStream in(&file);
        while(!in.atEnd())
        {
            QString line = in.readLine();
            if (line.contains(headTag))
            {
                line.replace(headTag, "<head>"
                             "<script type=\"text/javascript\">"
                             "var sessionID = '" + QUuid::createUuid().toString() + "';</script>");
                os << line << '\n';
                break;
            }
            os << line << '\n';
        }
        os << in.readAll();
        return;
    }
    qDebug() << "Can't read index";
}
