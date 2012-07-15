#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include "QWsServer.h"
#include "QWsSocket.h"

class WebsocketServer : public QObject
{
        Q_OBJECT

        QWsServer * server;
        QWsSocket * client;

        static WebsocketServer * m_instance;
        WebsocketServer(QObject * parent = 0);

    public:

        static WebsocketServer * instance(QObject * parent = 0);
        ~WebsocketServer();
        bool connectSocket();
        quint16 serverPort();

    public slots:

        void sendMessage(QString message);

    private slots:

        void onConnection();
        void onDisconnection();
        void onDataReceived(QString data);
        void readData();

    protected:

        void run();

    signals:

        void connected();
        void disconnected();
        void dataReceived(QString data);

};

#endif // SERVER_H
