#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <QTcpServer>

class HttpServer : public QTcpServer
{
        Q_OBJECT

        uint port;
        bool disabled;

    public:

        HttpServer(uint port, QObject * parent = 0);

    public slots:

        void pause()
        {
            disabled = true;
        }

        void resume()
        {
            disabled = false;
        }

    protected:

        virtual void incomingConnection(int socket);

        void sendStatus(QTextStream & os, int status);
        void sendIndex(QTextStream & os);
        void sendCanvas(QTextStream & os, QString applicationName);

    private slots:

        void discardClient();
        void readClient();

};

#endif // HTTPSERVER_H
