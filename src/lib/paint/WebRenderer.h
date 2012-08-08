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
            typedef struct _Widget
            {
                QWidget * w;

                QRegion region;

                QRect rect;

                _Widget(QWidget * _w)
                {
                    w = _w;
                }

                _Widget(QWidget * _w, const QRegion & _region, const QRect & _rect)
                {
                    w = _w;
                    region = _region;
                    rect = _rect;
                }

                bool operator ==(const _Widget & widget)
                {
                    return (this == &widget || this->w == widget.w);
                }

            } Widget;

            Q_OBJECT

            PaintDevice * pd;
            static WebRenderer * m_instance;
            QQueue<Widget> _render;
            QSemaphore _sem;
            QString buffer;
            QTimer * t;

            explicit WebRenderer(QObject * parent = 0);

        public:

            static WebRenderer * instance(QObject * parent = 0);
            void queue(QWidget * widget, QPaintEvent * event);
            void dequeue(QWidget * widget);

        public slots:

            void render();

    };
}

#endif // WEBRENDERER_H
