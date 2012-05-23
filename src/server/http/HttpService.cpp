#include "HttpService.h"
#include "HttpServer.h"

HttpService::HttpService(QObject * parent) :
    QObject(parent)
{
    server = new HttpServer(8080, this);
}

HttpService::~HttpService()
{
    if (server)
        delete server;
}
