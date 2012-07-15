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

using namespace KAppStream;

WebRenderer * WebRenderer::m_instance = 0;

WebRenderer::WebRenderer(QObject * parent) :
    QObject(parent),
    pd(new PaintDevice),
    _sem(1)
{
    QTimer * t = new QTimer(this);
    t->setInterval(500);
    connect(t, SIGNAL(timeout()), this, SLOT(render()));
    t->start();
}

WebRenderer * WebRenderer::instance(QObject * parent)
{
    if (!m_instance)
        m_instance = new WebRenderer(parent);
    return m_instance;
}

void WebRenderer::queue(QWidget * widget)
{
    _sem.acquire();
    if (widget->isVisible() && !_render.contains(widget))
        _render.enqueue(widget);
    _sem.release();
}

void WebRenderer::render()
{
    if (!_sem.tryAcquire())
        return;
    QQueue<QWidget*> tmp = _render;
    _render.clear();
    _sem.release();
    if (!tmp.isEmpty())
    {
        QPainter p(this->pd);
        do
        {
            QWidget * w = tmp.first();
            JSONBuilder::instance()->beginRender(w);
            w->render(&p, QPoint(), QRegion(), QWidget::DrawWindowBackground);
            tmp.dequeue();
            JSONBuilder::instance()->endRender();
        }
        while (!tmp.isEmpty());
        p.end();
        JSONBuilder::instance()->finish();
    }
}
