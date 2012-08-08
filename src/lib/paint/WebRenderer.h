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
                typedef struct QueueNode
                    {
                            QueueNode(const Widget & widget) :
                                next(0),
                                prev(0),
                                node(widget)
                            {}

                            QueueNode * next;
                            QueueNode * prev;

                            Widget node;

                    } QueueNode;

                QueueNode * start;
                QueueNode * end;
                QSemaphore _sem;

                public:

                _RenderQueue() :
                    start(0),
                    end(0),
                    _sem(1)
                {}

                void enqueue(Widget & node)
                {
                    _sem.acquire();
                    QueueNode * tmp = new QueueNode(node);
                    if (end)
                    {
                        end->next = tmp;
                        end = end->next;
                    }
                    else
                    {
                        start = end = tmp;
                    }
                    _sem.release();
                }

                Widget dequeue()
                {
                    _sem.acquire();
                    if (start)
                    {
                        Widget result = start->node;
                        QueueNode * tmp = start;
                        start = start->next;
                        if (tmp == end)
                            end = start;
                        delete tmp;
                        _sem.release();
                        return result;
                    }
                    else
                    {
                        Q_ASSERT(start == 0);
                        return Widget(0);
                    }
                }

                Widget & first()
                {
                    return start->node;
                }

                bool isEmpty()
                {
                    _sem.acquire();
                    if (start)
                    {
                        _sem.release();
                        return false;
                    }
                    _sem.release();
                    return true;
                }

                bool contains(const Widget & widget)
                {
                    _sem.acquire();
                    QueueNode * tmp = start;
                    while (tmp)
                    {
                        if (widget == tmp->node)
                        {
                            _sem.release();
                            return true;
                        }
                        tmp = tmp->next;
                    }
                    _sem.release();
                    return false;
                }

                void removeAll(const Widget & widget)
                {
                    _sem.acquire();
                    QueueNode * tmp = start;
                    while (tmp)
                    {
                        if (tmp->node == widget)
                        {
                            if (tmp == start)
                            {
                                start = tmp->next;
                                delete tmp;
                                if (!start)
                                {
                                    end = 0;
                                    break;
                                }
                                else
                                {
                                    start->prev = 0;
                                }
                            }
                            else if (tmp == end)
                            {
                                end = tmp->prev;
                                delete tmp;
                                break;
                            }
                            else
                            {
                                QueueNode * tmp2 = tmp;
                                if (tmp->prev)
                                    tmp->prev->next = tmp->next;
                                if (tmp->next)
                                    tmp->next->prev = tmp->prev;
                            }
                            continue;
                        }
                        tmp = tmp->next;
                    }
                    _sem.release();
                }

            } RenderQueue;

            Q_OBJECT

            PaintDevice * pd;
            static WebRenderer * m_instance;
            RenderQueue _render;
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
