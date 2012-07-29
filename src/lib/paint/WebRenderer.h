#ifndef WEBRENDERER_H
#define WEBRENDERER_H

#include <QObject>
#include <QSemaphore>
#include <QQueue>
#include <QRegion>
#include <QRect>
#include <QPaintEvent>

namespace KAppStream
{
    class PaintDevice;

    class WebRenderer : public QObject
    {
            Q_OBJECT

            PaintDevice * pd;
            static WebRenderer * m_instance;
            QQueue<QWidget*> _render;
            QQueue<QRegion> _regions;
            QQueue<QRect> _rects;
            QSemaphore _sem;
            QString buffer;

            explicit WebRenderer(QObject * parent = 0);

        public:

            static WebRenderer * instance(QObject * parent = 0);
            void queue(QWidget * widget, QPaintEvent * event);

        public slots:

            void render();

    };
}

#endif // WEBRENDERER_H
