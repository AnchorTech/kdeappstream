#ifndef IMAGESHOSTSERVER_H
#define IMAGESHOSTSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QSemaphore>

class IDImagePair
{
    public:

        qlonglong t;
        qint64 key;

        IDImagePair(qlonglong _t, qint64 _key) :
            t(_t),
            key(_key)
        {}

        IDImagePair(const IDImagePair & id) :
            t(id.t),
            key(id.key)
        {}

        QString toString()
        {
            return QString("%1_%2.png").arg(t).arg(key);
        }

        bool operator ==(const IDImagePair & pair) const
        {
            return (key == pair.key);
        }

        bool operator !=(const IDImagePair & pair) const
        {
            return !(this->operator ==(pair));
        }

        bool operator <(const IDImagePair & pair) const
        {
            if (pair.key == key)
                return (t < pair.t);
            return (key < pair.key);
        }

};

class ImagesHostServer : public QTcpServer
{
        Q_OBJECT

        static ImagesHostServer * m_instance;

        uint port;
        QSemaphore m_sem;
        QMap<IDImagePair, QImage> m_data;

        explicit ImagesHostServer(QObject * parent = 0);

    public:

        static ImagesHostServer * instance(QObject * parent = 0);

        IDImagePair hostImage(const QImage & image);

    protected slots:

        virtual void incomingConnection(int socket);
        void sendStatus(QIODevice * device, int status);
        void sendImage(QIODevice * device, IDImagePair id);

    private slots:

        void discardClient();
        void readClient();

};

#endif // IMAGESHOSTSERVER_H
