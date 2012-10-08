#include "ImagesBuffer.h"

#include <QBuffer>
#include <QDebug>

ImagesBuffer * ImagesBuffer::m_instance = 0;

ImagesBuffer::ImagesBuffer(QObject *parent) :
    QThread(parent),
    m_sem(1)
{
}

ImagesBuffer * ImagesBuffer::instance(QObject * parent)
{
    if (!m_instance)
        m_instance = new ImagesBuffer(parent);
    return m_instance;
}

void ImagesBuffer::run()
{
    this->exec();

    QMap<qreal, QByteArray*>::iterator it = m_map.begin();

    while (it != m_map.end())
    {
        delete it.value();
        ++it;
    }
}

void ImagesBuffer::addPixmap(const QPixmap & pixmap, qreal * id)
{
    m_sem.acquire();

    if (!m_map.contains(pixmap.cacheKey()))
    {
        QByteArray * array = new QByteArray();
        QBuffer buf(array);
        pixmap.toImage().save(&buf, "PNG");
        m_map.insert(pixmap.cacheKey(), array);
    }

    *id = pixmap.cacheKey();

    qDebug() << "Count: " << m_map.count();

    m_sem.release();
}

void ImagesBuffer::addImage(const QImage & image, qreal * id)
{
    m_sem.acquire();

    if (!m_map.contains(image.cacheKey()))
    {
        QByteArray * array = new QByteArray();
        QBuffer buf(array);
        image.save(&buf, "PNG");
        m_map.insert(image.cacheKey(), array);
    }

    *id = image.cacheKey();

    qDebug() << "Count: " << m_map.count();

    m_sem.release();
}

void ImagesBuffer::sendData(qreal id, QIODevice * output)
{
    m_sem.acquire();

    if (output && m_map.contains(id))
    {
        output->write(*m_map[id]);
    }

    m_sem.release();
}
