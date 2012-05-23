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
        QStringList tokens = QString(socket->readLine()).split(QRegExp("[ \r\n][ \r\n]*"));
        if (tokens[0] == "GET")
        {
            QTextStream os(socket);
            os.setAutoDetectUnicode(true);
            os << "HTTP/1.0 200 Ok\r\n"
                  "Content-Type: text/html; charset=\"utf-8\"\r\n"
                  "\r\n";

            KStandardDirs sd;
            QStringList sl = sd.findDirs("data", "kdeappstream/data/");
            if (!sl.count())
            {
                qDebug() << "File not found.";
                return;
            }

            foreach (QString dir, sl)
            {
                QFile file(dir + "/index.html");
                if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
                    continue;

                bool sessionIdUnset = false;
                QRegExp headTag("<head>");
                QTextStream in(&file);
                while(!in.atEnd())
                {
                    QString line = in.readLine();
                    if (sessionIdUnset &&
                            headTag.exactMatch(line))
                    {
                        line.replace(headTag, "<head>"
                                     "<script type=\"text/javascript\">"
                                     "var sessionID = " + QUuid::createUuid().toString() + ";</script>");
                        sessionIdUnset = true;
                    }
                    os << line << '\n';
                }
                socket->close();

                if (socket->state() == QTcpSocket::UnconnectedState)
                    delete socket;
                return;
            }

            qDebug() << "Can't read index";
        }
    }
}

void HttpServer::discardClient()
{
    QTcpSocket* socket = (QTcpSocket*)sender();
    socket->deleteLater();
}
