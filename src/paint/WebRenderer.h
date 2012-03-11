#ifndef WEBRENDERER_H
#define WEBRENDERER_H

#include <QObject>
#include <QSemaphore>
#include <QQueue>

namespace KAppStream
{
    class PaintDevice;

    class WebRenderer : public QObject
    {
            Q_OBJECT

            PaintDevice * pd;
            static WebRenderer * m_instance;
            QQueue<QWidget*> _render;
            QSemaphore _sem;
            QString buffer;

            explicit WebRenderer(QObject * parent = 0);

        public:

            static WebRenderer * instance(QObject * parent = 0);
            void queue(QWidget * widget);

        public slots:

            void render();
    };
}

#endif // WEBRENDERER_H
