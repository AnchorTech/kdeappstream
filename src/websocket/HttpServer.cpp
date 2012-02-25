#include "HttpServer.h"
#include <QTcpSocket>
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
            // os << index.html
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
