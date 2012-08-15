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
}

WebRenderer * WebRenderer::instance(QObject * parent)
{
    if (!m_instance)
        m_instance = new WebRenderer(parent);
    return m_instance;
}

QWidget * dupa2 = 0;

void WebRenderer::queue(QWidget * widget, QPaintEvent * event)
{
    if (dupa2 != widget)
    {
        Widget w(widget, event->region(), event->rect());
        _render.enqueue(w);
        t->start();
        //if (dupa)
    //    event->setAccepted(true);
    }
    //else
    //    qDebug() << "start render " << w.w;
    //else
    //    event->setAccepted(false);
}

void WebRenderer::dequeue(QWidget * widget)
{
    _render.removeAll(Widget(widget));
}

void WebRenderer::render()
{
    qDebug() << "timer start";

    while (!_render.isEmpty())
    {
        Widget w = _render.first();
        if (w.w->isVisible())
        {
            JSONBuilder::instance()->beginRender(w.w, w.region, w.rect);
            dupa2 = w.w;
            w.w->render(pd, QPoint(), QRegion(), QWidget::DrawWindowBackground);
            dupa2 = 0;
            JSONBuilder::instance()->endRender();
        }
        _render.dequeue();
    }

    JSONBuilder::instance()->finish();

    qDebug() << "timer end";


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
