#include "WebRenderer.h"
#include "JSONBuilder.h"
#include "PaintDevice.h"

#include <QString>
#include <QWidget>
#include <QTimer>
#include <QDebug>
#include <QByteArray>
#include <QBuffer>
#include <QPainter>
#include <QFrame>
#include <QStyle>
#include <QApplication>
#include <QPaintEvent>

using namespace KAppStream;

WebRenderer * WebRenderer::m_instance = 0;

WebRenderer::WebRenderer(QObject * parent) :
    QObject(parent),
    pd(new PaintDevice),
    _sem(1)
{
    t = new QTimer(this);
    t->setInterval(10);
    //t->setSingleShot(true);
    connect(t, SIGNAL(timeout()), this, SLOT(render()));
    t->start();
}

WebRenderer * WebRenderer::instance(QObject * parent)
{
    if (!m_instance)
        m_instance = new WebRenderer(parent);
    return m_instance;
}

void WebRenderer::queue(QWidget * widget, QPaintEvent * event)
{
    if (!widget->isVisible())
    {
        qDebug() << "not visible";
        return;
    }
    //_sem.acquire();
    Widget w(widget, event->region(), event->rect());
    if (!_render.contains(w))
    {
        _render.enqueue(w);
        if (!t->isActive())
            t->start(10);
    }
    else
        qDebug() << "fail";
    //_sem.release();
}

void WebRenderer::dequeue(QWidget * widget)
{
    qDebug() << "remove " << (long long) widget;
    _render.removeAll(Widget(widget));
}

void WebRenderer::render()
{
    QPainter p;
    bool first = true;
int i = 10;
    while (--i)
    {
        qDebug() << "try acquire";
        if (!_sem.tryAcquire())
            break;
        if (_render.isEmpty())
        {
            qDebug() << "its empty";
            _sem.release();
            break;
        }

        if (first)
        {
            p.begin(this->pd);
            first = false;
        }

        Widget w = _render.dequeue();
        if (w.w->isVisible())
        {
            _sem.release();
            JSONBuilder::instance()->beginRender(w.w, w.region, w.rect);
            w.w->render(&p, QPoint(), QRegion(), QWidget::DrawWindowBackground);
            JSONBuilder::instance()->endRender();
        }
        _sem.release();
    }

    if (!first)
        p.end();
    JSONBuilder::instance()->finish();
    //t->start();

    //            static bool f = true;
    //            QFrame * frame = dynamic_cast<QFrame*>(w);
    //            if (f && frame)
//            {
//                qDebug() << QApplication::style() << frame->frameRect() << frame->rect();
//                for (int i = 0; i < 60; ++i)
//                {
//                    QIcon ic = QApplication::style()->standardIcon((QStyle::StandardPixmap)i);
//                    if (!ic.isNull())
//                    {
//                        if (ic.availableSizes().count() > 0)
//                        {

//                            QImage im(ic.availableSizes()[ic.availableSizes().count()-1], QImage::Format_ARGB32);
//                            im.fill(0xFFFFFFFF);
//                            QPainter p(&im);
//                            p.drawPixmap(QPoint(0,0), ic.pixmap(ic.availableSizes()[ic.availableSizes().count()-1]));
//                            p.end();
//                            im.save("/home/coder89/dupa" + QString::number(i) + ".jpg");
//                        }
//                        else
//                            qDebug() << "no sizes available!";
//                    }
//                    else
//                        qDebug() << "is null";
//                }
//                f = false;
//            }
}
