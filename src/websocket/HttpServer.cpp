#include "HttpServer.h"
#include <QTcpSocket>
#include <QFile>
#include <QStringList>
#include <QRegExp>

HttpServer::HttpServer(int port, QObject *parent) :
    QTcpServer(parent)
{
    this->port=port;
    listen(QHostAddress::Any,port);
}

void HttpServer::incomingConnection(int socket)
{
    QTcpSocket* s = new QTcpSocket(this);
    connect(s, SIGNAL(readyRead()), this, SLOT(readClient()));
    connect(s, SIGNAL(disconnected()), this, SLOT(discardClient()));
    s->setSocketDescriptor(socket);
}

void HttpServer::readClient()
{
    QTcpSocket* socket = (QTcpSocket*)sender();
    if (socket->canReadLine()) {
        QStringList tokens = QString(socket->readLine()).split(QRegExp("[ \r\n][ \r\n]*"));
        if (tokens[0] == "GET") {
            QTextStream os(socket);
            os.setAutoDetectUnicode(true);
                                os << "HTTP/1.0 200 Ok\r\n"
                                    "Content-Type: text/html; charset=\"utf-8\"\r\n"
                                    "\r\n";
            QFile file("index.html");
            if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
                qDebug() << "Can't read index"; //Error
            QTextStream in(&file);
            while(!in.atEnd())
            {
                QString line = in.readLine();
                os << line;
            }
            socket->close();
            if (socket->state() == QTcpSocket::UnconnectedState) {
                delete socket;
            }
        }
    }
}
void HttpServer::discardClient()
{
    QTcpSocket* socket = (QTcpSocket*)sender();
    socket->deleteLater();
}
