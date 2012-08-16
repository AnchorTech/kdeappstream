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

                bool operator ==(const _Widget & widget) const
                {
                    return (this == &widget || this->w == widget.w);
                }

            } Widget;

            typedef class _RenderQueue
            {
                QQueue<Widget> queue;

                QSemaphore _sem;

                public:

                _RenderQueue() :
                    _sem(1)
                {}

                void enqueue(Widget & node)
                {
                    _sem.acquire();
                    queue.enqueue(node);
                    _sem.release();
                }

                Widget dequeue()
                {
                    _sem.acquire();
                    Widget result = queue.dequeue();
                    _sem.release();
                    return result;
                }

                Widget & first()
                {
                    _sem.acquire();
                    Widget result = queue.first();
                    _sem.release();
                    return result;
                }

                bool isEmpty()
                {
                    _sem.acquire();
                    bool result = queue.isEmpty();
                    _sem.release();
                    return result;
                }

                bool contains(const Widget & widget)
                {
                    _sem.acquire();
                    bool result = queue.contains(widget);
                    _sem.release();
                    return result;
                }

                void removeAll(const Widget & widget)
                {
                    _sem.acquire();
                    queue.removeAll(widget);
                    _sem.release();
                }

            } RenderQueue;

            Q_OBJECT

            PaintDevice * pd;
            static WebRenderer * m_instance;
            RenderQueue _render;
            QString buffer;
            QTimer * t;
            QSemaphore renderSemaphore;
            QWidget * currentRenderingWidget;

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
