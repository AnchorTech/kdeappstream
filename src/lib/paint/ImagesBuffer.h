#ifndef IMAGESBUFFER_H
#define IMAGESBUFFER_H

#include <QThread>
#include <QMap>
#include <QPixmap>
#include <QImage>
#include <QSemaphore>

class ImagesBuffer : public QThread
{
        Q_OBJECT

        static ImagesBuffer * m_instance;

        QSemaphore m_sem;
        QMap<qreal,QByteArray*> m_map;

        explicit ImagesBuffer(QObject * parent = 0);

    public:

        static ImagesBuffer * instance(QObject * parent = 0);

    protected:

        virtual void run();

    public slots:

        void addPixmap(const QPixmap & pixmap, qreal * id);
        void addImage(const QImage & image, qreal * id);
        void sendData(qreal id, QIODevice * output);

};

#endif // IMAGESBUFFER_H
