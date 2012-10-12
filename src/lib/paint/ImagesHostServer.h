#ifndef IMAGESHOSTSERVER_H
#define IMAGESHOSTSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QSemaphore>
#include <QCryptographicHash>
#include <QImage>

class ImagesHostServer : public QTcpServer
{
        Q_OBJECT

        static ImagesHostServer * m_instance;

        uint port;
        QSemaphore m_sem;
        QMap<QByteArray, QImage> m_data;
        QSet<QByteArray> m_cached;

        explicit ImagesHostServer(QObject * parent = 0);

    public:

        static ImagesHostServer * instance(QObject * parent = 0);

        QByteArray hostImage(const QImage & image);

        static inline QByteArray fastmd5( const QImage & image )
        {
            QCryptographicHash formats( QCryptographicHash::Md5 );
            formats.addData((const char*)image.bits(), image.byteCount());
            return formats.result();
        }

    protected slots:

        virtual void incomingConnection(int socket);
        void sendStatus(QIODevice * device, int status);
        void sendImage(QIODevice * device, QByteArray id);

    private slots:

        void discardClient();
        void readClient();

};

#endif // IMAGESHOSTSERVER_H
