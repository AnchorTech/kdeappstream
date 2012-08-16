#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include "QWsServer.h"
#include "QWsSocket.h"
#include <QSemaphore>

class WebsocketServer : public QObject
{
        Q_OBJECT

        QWsServer * server;
        QWsSocket * client;
        QSemaphore connectionSemaphore;

        static WebsocketServer * m_instance;
        WebsocketServer(QObject * parent = 0);

    public:

        static WebsocketServer * instance(QObject * parent = 0);
        ~WebsocketServer();
        bool connectSocket();
        quint16 serverPort();
        bool waitForConnected(int wait);

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
