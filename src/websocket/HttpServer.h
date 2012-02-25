#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <QTcpServer>

class HttpServer : public QTcpServer
{
    Q_OBJECT
public:
    HttpServer(int port, QObject *parent = 0);

private:
    int port;
protected:
    virtual void incomingConnection(int socket);

private slots:
    void discardClient();
    void readClient();
};

#endif // HTTPSERVER_H
