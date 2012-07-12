#ifndef WEBSOCKETSERVICE_H
#define WEBSOCKETSERVICE_H

#include <QThread>

class WebSocketService : public QThread
{
        Q_OBJECT

        int port;

    public:

        explicit WebSocketService(int port, QObject * parent = 0);

    signals:

    public slots:

    protected:

        virtual void run();

};

#endif // WEBSOCKETSERVICE_H
