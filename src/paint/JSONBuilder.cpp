#include "JSONBuilder.h"

#include "paint/PaintDevice.h"
#include "paint/PaintEngine.h"

#include <QString>
#include <QWidget>
#include <QTimer>
#include <QDebug>
#include <QByteArray>
#include <QBuffer>

using namespace KAppStream;

JSONBuilder * JSONBuilder::m_instance = 0;

JSONBuilder::JSONBuilder(QObject * parent) :
    QObject(parent),
    pd(new PaintDevice),
    _sem(1)
{
    QTimer * t = new QTimer(this);
    t->setInterval(500);
    connect(t, SIGNAL(timeout()), this, SLOT(render()));
    t->start();
}

JSONBuilder * JSONBuilder::instance(QObject * parent)
{
    if (!m_instance)
        m_instance = new JSONBuilder(parent);
    return m_instance;
}

void JSONBuilder::queue(QWidget * widget)
{
    if (!_sem.tryAcquire(1))
        return;
    if (widget->isVisible() && !_render.contains(widget))
    {
        _render.enqueue(widget);
        qDebug() << widget;
    }
    _sem.release();
}

QString JSONBuilder::color(const QColor & c)
{
    return QString("\"color\":\"%1\"").arg(c.name());
}

QString JSONBuilder::ellipse(const QRect & r)
{
    return QString("\"ellipse\":{\"x\":%1,\"y\":%2,\"w\":%3,\"h\":%4}").arg(r.x()).arg(r.y()).arg(r.width()).arg(r.height());
}

QString JSONBuilder::ellipse(const QRectF & r)
{
    return QString("\"ellipse\":{\"x\":%1,\"y\":%2,\"w\":%3,\"h\":%4}").arg(r.x()).arg(r.y()).arg(r.width()).arg(r.height());
}

QString JSONBuilder::font(const QFont & f)
{
    QStringList result;
    result.removeAll("");
    return "\"font\":{" + result.join(",") + "}";
}

QString JSONBuilder::image(const QImage & i)
{
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    i.save(&buffer, "PNG");
    return QString("\"image\":\"data:image/png;base64," + byteArray.toBase64() + "\"");
}

QString JSONBuilder::line(const QLine & l)
{
    return QString("\"line\":{\"xs\":%1,\"ys\":%2,\"xe\":%3,\"ye\":%4}").arg(l.x1()).arg(l.y1()).arg(l.x2()).arg(l.y2());
}

QString JSONBuilder::line(const QLineF & l)
{
    return QString("\"line\":{\"xs\":%1,\"ys\":%2,\"xe\":%3,\"ye\":%4}").arg(l.x1()).arg(l.y1()).arg(l.x2()).arg(l.y2());
}

QString JSONBuilder::pixmap(const QRectF & r, const QPixmap & pm, const QRectF & sr)
{
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    pm.copy(sr.toRect()).save(&buffer, "PNG");
    return QString("\"pixmap\":{\"x\":%1,\"y\":%2,\"w\":%3,\"h\":%4,\"data\":\"data:image/png;base64," + byteArray.toBase64() + "\"}")
            .arg(r.x())
            .arg(r.y())
            .arg(r.width())
            .arg(r.height());
}

QString JSONBuilder::pixmap(const QPixmap & pm)
{
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    pm.save(&buffer, "PNG");
    return QString("\"pixmap\":\"data:image/png;base64," + byteArray.toBase64() + "\"");
}

QString JSONBuilder::rect(const QRect & r)
{
    return QString("\"rect\":{\"x\":%1,\"y\":%2,\"w\":%3,\"h\":%4}").arg(r.x()).arg(r.y()).arg(r.width()).arg(r.height());
}

QString JSONBuilder::rect(const QRectF & r)
{
    return QString("\"rect\":{\"x\":%1,\"y\":%2,\"w\":%3,\"h\":%4}").arg(r.x()).arg(r.y()).arg(r.width()).arg(r.height());
}

QString JSONBuilder::state(const QPaintEngineState & s)
{
    QStringList result;

    QPaintEngine::DirtyFlags f = s.state();
    if (f & QPaintEngine::DirtyPen)
        result += pen(s.pen());
    if (f & QPaintEngine::DirtyBrush)
        result += brush(s.brush());
    if (f & QPaintEngine::DirtyBrushOrigin)
        result += QString("\"origin\":{\"x\":%1,\"y\":%2}").arg(s.brushOrigin().x()).arg(s.brushOrigin().y());
    if (f & QPaintEngine::DirtyFont)
        result += font(s.font());
    if (f & QPaintEngine::DirtyBackground || f & QPaintEngine::DirtyBackgroundMode)
    {
        QStringList t;
        if (f & QPaintEngine::DirtyBackground)
            t += brush(s.brush());
        if (f & QPaintEngine::DirtyBackgroundMode)
            t += QString("\"mode\":%1").arg(s.backgroundMode());
        result += QString("\"bg\":{%1}").arg(t.join(","));
    }
    if (f & QPaintEngine::DirtyTransform)
        result += transform(s.transform());
    if (f & QPaintEngine::DirtyClipRegion)
        result += "";
    if (f & QPaintEngine::DirtyClipPath)
        result += "";
    if (f & QPaintEngine::DirtyHints)
    {
        QStringList tmp;
        tmp += "\"aliasing\":" + (s.renderHints() & QPainter::Antialiasing);
        tmp += "\"testAliasing\":" + (s.renderHints() & QPainter::TextAntialiasing);
        tmp += "\"smoothPixmapTransform\":" + (s.renderHints() & QPainter::SmoothPixmapTransform);
        tmp += "\"highQualityAntialiasing\":" + (s.renderHints() & QPainter::HighQualityAntialiasing);
        tmp += "\"nonCosmeticDefaultPen\":" + (s.renderHints() & QPainter::NonCosmeticDefaultPen);
        result += "\"hints\":{" + tmp.join(",") + "}";
    }
    if (f & QPaintEngine::DirtyCompositionMode)
        result += QString("\"mode\":%1").arg(s.compositionMode());
    if (f & QPaintEngine::DirtyClipEnabled)
        result += "";
    if (f & QPaintEngine::DirtyOpacity)
        result += QString("\"opacity\":%1").arg(s.opacity());
    result.removeAll("");
    return "\"state\":{" + result.join(",") + "}";
}

QString JSONBuilder::pen(const QPen & p)
{
    QStringList result;

    // If brush defined (instead color)
    QBrush b = p.brush();
    if (b.style() != Qt::NoBrush && b.style() != Qt::SolidPattern)
        result += brush(p.brush());
    else
        result += color(p.color());

    // If Penstyle different than SolidLine
    if (!p.isSolid())
    {
        result += QString("\"style\":%1").arg(p.style());
        result += QString("\"cap\":%1").arg(p.capStyle());
        QStringList tmp;
        foreach (qreal d, p.dashPattern())
            tmp += QString::number(d);
        result += QString("\"pattern\":[%1]").arg(tmp.join(","));
        result += QString("\"offset\":%1").arg(p.dashOffset());
    }

    result += QString("\"join\":%1").arg(p.joinStyle());
    result += QString("\"miter\":%1").arg(p.miterLimit());
    result += QString("\"width\":%1").arg(p.width());

    result.removeAll("");
    return "\"pen\":{" + result.join(",") + "}";
}

QString JSONBuilder::brush(const QBrush & b)
{
    QStringList result;

    switch (b.style())
    {
        case Qt::NoBrush:
            return "";
        case Qt::SolidPattern:
            result += color(b.color());
            break;
        case Qt::LinearGradientPattern:
        case Qt::RadialGradientPattern:
        case Qt::ConicalGradientPattern:
            if (b.gradient())
                result += gradient(*(b.gradient()));
            break;
        case Qt::TexturePattern:
            {
                QImage img = b.textureImage();
                if (!img.isNull())
                    result += image(img);
                else
                {
                    QPixmap pix = b.texture();
                    if (!pix.isNull())
                        result += pixmap(pix);
                }
            }
            break;
        default:
            result += color(b.color());
            result += QString("\"style\":%1").arg(b.style());
            break;
    }

    if (!b.transform().isIdentity())
        result += "\"transform\":" + transform(b.transform());

    result.removeAll("");
    return "\"brush\":{" + result.join(",") + "}";
}

QString JSONBuilder::gradient(const QGradient & g)
{
    QStringList result;
    result += QString("\"type\":%1").arg(g.type());

    QStringList tmp;
    foreach (QGradientStop stop, g.stops())
        tmp += QString("[%1, %2]").arg(stop.first).arg(stop.second.name());
    result += "\"stops\":[" + tmp.join(",") + "]";

    result.removeAll("");
    return QString("\"gradient\":{") + result.join(",") + "}";
}

QString JSONBuilder::transform(const QTransform & t)
{
    return QString("\"transform\":[ [%1, %2, %3], [%4, %5, %6], [%7, %8, %9] ]")
            .arg(t.m11())
            .arg(t.m12())
            .arg(t.m13())
            .arg(t.m21())
            .arg(t.m22())
            .arg(t.m23())
            .arg(t.m31())
            .arg(t.m32())
            .arg(t.m33());
}

void JSONBuilder::render()
{
    _sem.acquire();
    QQueue<QWidget*> tmp = _render;
    if (!tmp.isEmpty())
    {
        QPainter p(pd);
        do
        {
            qDebug() << tmp.first();
            tmp.dequeue()->render(&p, QPoint(), QRegion(), QWidget::DrawWindowBackground);
        }
        while (!tmp.isEmpty());
        p.end();
    }
    _render.clear();
    _sem.release();
}
