#include "HttpService.h"
#include "HttpServer.h"

HttpService::HttpService(QObject * parent) :
    QObject(parent)
{
    qDebug() << "Starting http server";
    server = new HttpServer(8080, this);
}

HttpService::~HttpService()
{
    qDebug() << "Closing http server";
    if (server)
        delete server;
}
