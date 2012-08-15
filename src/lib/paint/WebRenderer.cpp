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
    pd(new PaintDevice)
{
    t = new QTimer(this);
    //t->setInterval(100);
    t->setSingleShot(true);
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
    Widget w(widget, event->region(), event->rect());
    if (_render.isEmpty() || !(_render.first() == w))
    {
        //qDebug() << "dodane";
        _render.enqueue(w);
        //if (!t->isActive())
            t->start();
    //    event->setAccepted(true);
    }
    else
        qDebug() << "start render " << w.w;
    //else
    //    event->setAccepted(false);
}

void WebRenderer::dequeue(QWidget * widget)
{
    _render.removeAll(Widget(widget));
}

void WebRenderer::render()
{
    QPainter p;
    bool first = true;

    while (!_render.isEmpty())
    {
        //qDebug() << "try acquire";

        if (first)
        {
            p.begin(this->pd);
            first = false;
        }

        Widget w = _render.first();
        //if (w.w->isVisible())
        {
            qDebug() << "init render " << w.w;
            JSONBuilder::instance()->beginRender(w.w, w.region, w.rect);
            w.w->render(&p, QPoint(), QRegion(), QWidget::DrawWindowBackground);
            JSONBuilder::instance()->endRender();
        }
        _render.dequeue();
    }

    if (!first)
        p.end();

    JSONBuilder::instance()->finish();

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
