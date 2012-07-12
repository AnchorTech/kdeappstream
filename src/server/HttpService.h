#ifndef HTTPSERVICE_H
#define HTTPSERVICE_H

#include <QObject>

class HttpServer;

class HttpService : public QObject
{
        Q_OBJECT

        HttpServer * server;

    public:

        explicit HttpService(QObject * parent = 0);
        ~HttpService();

    signals:

    public slots:

};

#endif // HTTPSERVICE_H
