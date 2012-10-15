#include "JSONBuilder.h"

#include "paint/ImagesBuffer.h"
#include "paint/PaintDevice.h"
#include "paint/PaintEngine.h"
#include "websocket/QWsSocket.h"

#include <QString>
#include <QWidget>
#include <QTimer>
#include <QDebug>
#include <QByteArray>
#include <QBuffer>
#include <QLocalSocket>
#include <QCoreApplication>
#include <QTabBar>
#include <QToolButton>
#include <QGradient>
#include <QPainter>
#include <QListWidget>
#include <QGraphicsView>

using namespace KAppStream;

JSONBuilder * JSONBuilder::m_instance = 0;

JSONBuilder::JSONBuilder(QObject * parent) :
    QObject(parent),
    _render_sem(1),
    _layout_sem(1)
{
    render_buffer.reserve(5237760);
    layout_buffer.reserve(2618880);
    connect(this, SIGNAL(bufferImage(QImage,qreal*)), ImagesBuffer::instance(), SLOT(addImage(QImage,qreal*)));
    connect(this, SIGNAL(bufferPixmap(QPixmap,qreal*)), ImagesBuffer::instance(), SLOT(addPixmap(QPixmap,qreal*)));
}

JSONBuilder * JSONBuilder::instance(QObject * parent)
{
    if (!m_instance)
        m_instance = new JSONBuilder(parent);
    return m_instance;
}

void JSONBuilder::beginRender(QWidget * widget, const QRect & rect)
{
    if (!widget)
        return;

    _render_sem.acquire();

    context << widget;

    QWidget * parent = widget->parentWidget();
    QSize s = widget->size();
    QPoint p = widget->pos();
    if (!parent)
        p = QPoint(0,0);
    else if (widget->windowType() & Qt::Window)
        p = parent->mapFromGlobal(p);

    render_buffer.append("{\"command\":\"draw\"")
          .append(",\"widget\":{")
          .append("\"id\":").append(QString::number((long long)widget).toAscii());

    if (parent)
        render_buffer.append(",\"z\":").append(QString::number(parent->children().indexOf(widget)).toAscii());

    render_buffer.append(",\"name\":\"").append(widget->metaObject()->className()).append("\"")
          .append(",\"flags\":").append(QString::number(widget->windowFlags()).toAscii())
          .append(",\"x\":").append(QString::number( p.x() ).toAscii())
          .append(",\"y\":").append(QString::number( p.y() ).toAscii())
          .append(",\"w\":").append(QString::number( s.width() ).toAscii())
          .append(",\"h\":").append(QString::number( s.height() ).toAscii())
            .append(",\"r\":{")
            .append("\"x\":").append(QString::number( rect.x() ).toAscii())
            .append(",\"y\":").append(QString::number( rect.y() ).toAscii())
            .append(",\"w\":").append(QString::number( rect.width() ).toAscii())
            .append(",\"h\":").append(QString::number( rect.height() ).toAscii())
          .append("}},\"render\":[");
}

void JSONBuilder::endRender()
{
    if (context.length())
    {
        context.removeFirst();
        if (render_buffer.length() > 0 && render_buffer[render_buffer.length()-1] == ',')
            render_buffer.remove(render_buffer.length()-1, 1);
        render_buffer.append("]},");
    }
    _render_sem.release();
}

void JSONBuilder::activateWindow(QWidget * window)
{
    _layout_sem.acquire();
    layout_buffer.append("{\"command\":\"activate\"")
          .append(",\"id\":").append(QString::number((long long)window).toAscii())
          .append("},");
    _layout_sem.release();
    this->finish();
}

void JSONBuilder::deactivateWindow(QWidget * window)
{
    _layout_sem.acquire();
    layout_buffer.append("{\"command\":\"deactivate\"")
          .append(",\"id\":").append(QString::number((long long)window).toAscii())
          .append("},");
    _layout_sem.release();
    this->finish();
}

void JSONBuilder::addChild(QWidget * child, QWidget * parent)
{
    _layout_sem.acquire();
    layout_buffer.append("{\"command\":\"addChild\"")
          .append(",\"id\":").append(QString::number((long long)parent).toAscii())
          .append(",\"child\":").append(QString::number((long long)child).toAscii())
          .append(",\"flags\":").append(QString::number(child->windowFlags()).toAscii())
          .append(",\"name\":\"").append(child->metaObject()->className()).append("\"")
          .append("},");
    _layout_sem.release();
    this->finish();
}

void JSONBuilder::removeChild(QWidget * child, QWidget * parent)
{
    _layout_sem.acquire();
    layout_buffer.append("{\"command\":\"removeChild\"")
          .append(",\"id\":").append(QString::number((long long)parent).toAscii())
          .append(",\"child\":").append(QString::number((long long)child).toAscii())
          .append("},");
    _layout_sem.release();
    this->finish();
}

void JSONBuilder::resize(QWidget * w, const QSize & oldSize, const QSize & newSize)
{
    if (w->windowType() & Qt::Window)
    {
        _layout_sem.acquire();
        layout_buffer.append("{\"command\":\"resize\"")
              .append(",\"id\":").append(QString::number((long long)w).toAscii())
              .append(",\"old\":")
                .append("{\"w\":").append(QString::number(oldSize.width()).toAscii())
                .append(",\"h\":").append(QString::number(oldSize.height()).toAscii())
              .append("},\"new\":")
                .append("{\"w\":").append(QString::number(newSize.width()).toAscii())
                .append(",\"h\":").append(QString::number(newSize.height()).toAscii())
              .append("}},");
        _layout_sem.release();
        this->finish();
    }
}

void JSONBuilder::hideWidget(QWidget * widget)
{
    _layout_sem.acquire();
    layout_buffer.append("{\"command\":\"hide\"")
          .append(",\"id\":").append(QString::number((long long)widget).toAscii())
          .append("},");
    _layout_sem.release();
    this->finish();
}

void JSONBuilder::showWidget(QWidget * widget)
{
    _layout_sem.acquire();
    layout_buffer.append("{\"command\":\"show\"")
          .append(",\"id\":").append(QString::number((long long)widget).toAscii());
    if (widget->isWindow())
        layout_buffer.append(",\"modality\":").append(QString::number((long long)widget->windowModality()).toAscii());
    layout_buffer.append("},");
    _layout_sem.release();
    this->finish();
}

void JSONBuilder::titleChange(QWidget * widget, const QString & title)
{
    QString tmp = title;
    _layout_sem.acquire();
    layout_buffer.append("{\"command\":\"title\"")
          .append(",\"id\":").append(QString::number((long long)widget).toAscii())
          .append(",\"text\":\"").append(tmp.replace("\\", "\\\\").replace("\"", "\\\"").toUtf8())
          .append("\"},");
    _layout_sem.release();
    this->finish();
}

void JSONBuilder::move(QWidget * widget, const QPoint & pos)
{
    _layout_sem.acquire();
    layout_buffer.append("{\"command\":\"move\"")
          .append(",\"id\":").append(QString::number((long long)widget).toAscii())
          .append(",\"x\":").append(QString::number(pos.x()).toAscii())
          .append(",\"y\":").append(QString::number(pos.y()).toAscii())
          .append("},");
    _layout_sem.release();
    this->finish();
}

void JSONBuilder::ZOrderChange(QWidget * widget)
{
    QWidget * parent = widget->parentWidget();
    if (!parent)
        return;
    _layout_sem.acquire();
    layout_buffer.append("{\"command\":\"zorder\"")
          .append(",\"id\":").append(QString::number((long long)widget).toAscii())
          .append(",\"z\":").append(QString::number(parent->children().indexOf(widget)).toAscii())
          .append("},");
    _layout_sem.release();
    this->finish();
}

void JSONBuilder::finish()
{
    emit readyRead();
}

void JSONBuilder::flush(QWsSocket * device)
{
    static int i = 0;
    if (device)
    {
        _render_sem.acquire();
        if (render_buffer.length())
        {
            if (render_buffer[render_buffer.length()-1] == ',')
                render_buffer.remove(render_buffer.length()-1, 1);
            device->write(QString('[' + render_buffer + ']'));
            ++i;
        }
        render_buffer.clear();
        _render_sem.release();

        _layout_sem.acquire();
        if (layout_buffer.length())
        {
            if (layout_buffer[layout_buffer.length()-1] == ',')
                layout_buffer.remove(layout_buffer.length()-1, 1);
            device->write(QString('[' + layout_buffer + ']'));
            ++i;
        }
        layout_buffer.clear();
        _layout_sem.release();
    }
}

void JSONBuilder::ellipse(const QRect & r)
{
    saveStatePriv();
    render_buffer.append("{\"t\":\"ellipse\"")
          .append(",\"x\":").append(QString::number(r.x()))
          .append(",\"y\":").append(QString::number(r.y()))
          .append(",\"w\":").append(QString::number(r.width()))
          .append(",\"h\":").append(QString::number(r.height()))
          .append("},");
}

void JSONBuilder::ellipse(const QRectF & r)
{
    saveStatePriv();
    render_buffer.append("{\"t\":\"ellipse\"")
          .append(",\"x\":").append(QString::number(r.x()))
          .append(",\"y\":").append(QString::number(r.y()))
          .append(",\"w\":").append(QString::number(r.width()))
          .append(",\"h\":").append(QString::number(r.height()))
          .append("},");
}

void JSONBuilder::image(const QRectF & r, const QImage & image, const QRectF & sr, Qt::ImageConversionFlags flags)
{
    this->image(image.copy(sr.toRect()).scaled(r.size().toSize()), r.topLeft());
}

void JSONBuilder::image(const QImage & image, const QPointF & p)
{
        QByteArray id = ImagesHostServer::instance()->hostImage(image);
        this->image(id, p);
}

void JSONBuilder::image(const QByteArray & id, const QPointF & p)
{
    saveStatePriv();
    render_buffer.append("{\"t\":\"image\"")
          .append(",\"data\":\"").append(id.data())
          .append("\",\"x\":").append(QString::number(p.x()).toAscii())
          .append(",\"y\":").append(QString::number(p.y()).toAscii())
          .append("},");
}

void JSONBuilder::line(const QLine & l)
{
    saveStatePriv();
    render_buffer.append("{\"t\":\"line\"")
          .append(",\"xs\":").append(QString::number(l.x1()))
          .append(",\"ys\":").append(QString::number(l.y1()))
          .append(",\"xe\":").append(QString::number(l.x2()))
          .append(",\"ye\":").append(QString::number(l.y2()))
          .append("},");
}

void JSONBuilder::line(const QLineF & l)
{
    saveStatePriv();
    render_buffer.append("{\"t\":\"line\"")
          .append(",\"xs\":").append(QString::number(l.x1()))
          .append(",\"ys\":").append(QString::number(l.y1()))
          .append(",\"xe\":").append(QString::number(l.x2()))
          .append(",\"ye\":").append(QString::number(l.y2()))
          .append("},");
}

void JSONBuilder::path(const QPainterPath & path)
{
    saveStatePriv();
    render_buffer.append("{\"t\":\"path\"")
            .append(",\"data\":[");

    for (int i = 0; i < path.elementCount(); ++i)
    {
        QPainterPath::Element e = path.elementAt(i);
        render_buffer.append("{\"t\":").append(QString::number(e.type).toAscii());
        render_buffer.append(",\"p\":[[")
                .append(QString::number(e.x).toAscii())
                .append(",")
                .append(QString::number(e.y).toAscii())
              .append("],");

        if (e.type == QPainterPath::CurveToElement)
        {
            for (e = path.elementAt(++i); i < path.elementCount(); ++i)
            {
                render_buffer.append("[")
                        .append(QString::number(e.x).toAscii())
                        .append(",")
                        .append(QString::number(e.y).toAscii())
                        .append("],");
                e = path.elementAt(i);
                if (e.type != QPainterPath::CurveToDataElement)
                {
                    --i;
                    break;
                }
            }
        }

        if (render_buffer[render_buffer.length()-1] == ',')
            render_buffer.remove(render_buffer.length()-1, 1);

        render_buffer.append("]},");
    }

    if (render_buffer[render_buffer.length()-1] == ',')
        render_buffer.remove(render_buffer.length()-1, 1);

    render_buffer.append("],\"fill\":").append(QString::number(path.fillRule()).toAscii());
    render_buffer.append("},");
}

void JSONBuilder::pixmap(const QRectF & r, const QPixmap & pm, const QRectF & sr)
{
    this->pixmap(pm.copy(sr.toRect()).scaled(r.size().toSize()), r.topLeft());
}

void JSONBuilder::pixmap(const QPixmap & pm, const QPointF & p)
{
    QByteArray id = ImagesHostServer::instance()->hostImage(pm.toImage());
    this->pixmap(id, p);
}

void JSONBuilder::pixmap(const QByteArray & id, const QPointF & p)
{
    // issue: http://www.qtcentre.org/threads/20106-QPixmap-Rotating-but-not-translating

    QTransform t = cur_state.transform;
    cur_state.transform = QTransform();
    cur_state.state |= QPaintEngine::DirtyTransform;
    saveStatePriv();

    render_buffer.append("{\"t\":\"image\"")
          .append(",\"data\":\"").append(id.data())
          .append("\",\"x\":").append(QString::number(p.x()).toAscii())
          .append(",\"y\":").append(QString::number(p.y()).toAscii())
          .append("},");

    cur_state.transform = t;
    cur_state.state |= QPaintEngine::DirtyTransform;
    saveStatePriv();
}

void JSONBuilder::points(const QPointF * points, int pointCount)
{
    saveStatePriv();
    render_buffer.append("{\"t\":\"points\"")
            .append(",\"data\":[");
    while (pointCount--)
    {
        render_buffer.append("[").append(QString::number(points->x()).toAscii()).append(",").append(QString::number(points->y()).toAscii()).append("],");
        ++points;
    }
    if (render_buffer[render_buffer.length()-1] == ',')
        render_buffer.remove(render_buffer.length()-1, 1);
    render_buffer.append("]},");
}

void JSONBuilder::points(const QPoint * points, int pointCount)
{
    saveStatePriv();
    render_buffer.append("{\"t\":\"points\"")
            .append(",\"data\":[");
    while (pointCount--)
    {
        render_buffer.append("[").append(QString::number(points->x()).toAscii()).append(",").append(QString::number(points->y()).toAscii()).append("],");
        ++points;
    }
    if (render_buffer[render_buffer.length()-1] == ',')
        render_buffer.remove(render_buffer.length()-1, 1);
    render_buffer.append("]},");
}

void JSONBuilder::polygon(const QPointF * points, int pointCount, QPaintEngine::PolygonDrawMode mode)
{
    saveStatePriv();
    render_buffer.append("{\"t\":\"polygon\"")
            .append(",\"mode\":").append(QString::number(mode))
            .append(",\"data\":[");
    while (pointCount--)
    {
        render_buffer.append("[").append(QString::number(points->x()).toAscii()).append(",").append(QString::number(points->y()).toAscii()).append("],");
        ++points;
    }
    if (render_buffer[render_buffer.length()-1] == ',')
        render_buffer.remove(render_buffer.length()-1, 1);
    render_buffer.append("]},");
}

void JSONBuilder::polygon(const QPoint * points, int pointCount, QPaintEngine::PolygonDrawMode mode)
{
    saveStatePriv();
    render_buffer.append("{\"t\":\"polygon\"")
            .append(",\"mode\":").append(QString::number(mode))
            .append(",\"data\":[");
    while (pointCount--)
    {
        render_buffer.append("[").append(QString::number(points->x()).toAscii()).append(",").append(QString::number(points->y()).toAscii()).append("],");
        ++points;
    }
    if (render_buffer[render_buffer.length()-1] == ',')
        render_buffer.remove(render_buffer.length()-1, 1);
    render_buffer.append("]},");
}

void JSONBuilder::rect(const QRect & r)
{
    saveStatePriv();
    render_buffer.append("{\"t\":\"rect\"")
          .append(",\"x\":").append(QString::number(r.x()))
          .append(",\"y\":").append(QString::number(r.y()))
          .append(",\"w\":").append(QString::number(r.width()))
          .append(",\"h\":").append(QString::number(r.height()))
          .append("},");
}

void JSONBuilder::rect(const QRectF & r)
{
    saveStatePriv();
    render_buffer.append("{\"t\":\"rect\"")
          .append(",\"x\":").append(QString::number(r.x()))
          .append(",\"y\":").append(QString::number(r.y()))
          .append(",\"w\":").append(QString::number(r.width()))
          .append(",\"h\":").append(QString::number(r.height()))
          .append("},");
}

void JSONBuilder::text(const QPointF & p, const QTextItem & textItem)
{
    saveStatePriv();
    render_buffer.append("{\"t\":\"text\"")
            .append(",\"data\":{")
            .append("\"text\":\"").append(textItem.text().replace("\\", "\\\\").replace("\"", "\\\"").toUtf8()).append("\"")
            .append(",\"ascent\":").append(QString::number(textItem.ascent()).toAscii())
            .append(",\"descent\":").append(QString::number(textItem.descent()).toAscii())
            .append(",\"x\":").append(QString::number(p.x()).toAscii())
            .append(",\"y\":").append(QString::number(p.y()).toAscii())
            .append(",");
    font(textItem.font());
    render_buffer.append("}},");
}

void JSONBuilder::tiledPixmap(const QRectF & r, const QPixmap & pm, const QPointF & p)
{
    //    QTransform t = cur_state.transform;
    //    cur_state.transform = QTransform();
    //    cur_state.state |= QPaintEngine::DirtyTransform;
    //    saveStatePriv();

    //    QByteArray id = ImagesHostServer::instance()->hostImage(pm.toImage());
    //    buffer.append("{\"t\":\"pixmap\"")
    //          .append(",\"data\":\"").append(id.data())
    //          .append("\",\"x\":").append(QString::number(p.x()).toAscii())
    //          .append(",\"y\":").append(QString::number(p.y()).toAscii())
    //          .append(",\"r\":{\"x\":").append(QString::number(r.x()).toAscii())
    //          .append(",\"y\":").append(QString::number(r.y()).toAscii())
    //          .append(",\"w\":").append(QString::number(r.width()).toAscii())
    //          .append(",\"h\":").append(QString::number(r.height()).toAscii())
    //          .append("}},");

    //    cur_state.transform = t;
    //    cur_state.state |= QPaintEngine::DirtyTransform;
    //    saveStatePriv();

    QPixmap pixmap(r.size().toSize());
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.drawTiledPixmap(QRectF(QPointF(), r.size()), pm, p);
    painter.end();
    this->image(pixmap.toImage(), r.topLeft());
}

void JSONBuilder::state(const QPaintEngineState & s)
{
    QPaintEngine::DirtyFlags f = s.state();
    if (f & QPaintEngine::DirtyBackground)
        cur_state.backgroundBrush = s.backgroundBrush();
    if (f & QPaintEngine::DirtyBackgroundMode)
        cur_state.backgroundMode = s.backgroundMode();
    if (f & QPaintEngine::DirtyBrush)
        cur_state.brush = s.brush();
    if (f & QPaintEngine::DirtyBrushOrigin)
        cur_state.brushOrigin = s.brushOrigin();
    if (f & QPaintEngine::DirtyClipPath || f & QPaintEngine::DirtyClipRegion)
    {
        cur_state.clipOperation = s.clipOperation();
        cur_state.clipPath = s.clipPath();
        cur_state.clipRegion = s.clipRegion();
        if ( f & QPaintEngine::DirtyClipRegion &&
             s.clipRegion().rectCount() == 1 &&
             s.clipRegion().boundingRect() == context.first()->rect())
        { }
        else if (s.clipOperation() != Qt::NoClip)
        {
            cur_state.clipOperation = s.clipOperation();
            if (f & QPaintEngine::DirtyClipPath)
                cur_state.clipPath = s.clipPath();
            if (f & QPaintEngine::DirtyClipRegion)
                cur_state.clipRegion = s.clipRegion();
        }
    }
    if (f & QPaintEngine::DirtyCompositionMode)
        cur_state.compositionMode = s.compositionMode();
    if (f & QPaintEngine::DirtyFont)
        cur_state.font = s.font();
    if (f & QPaintEngine::DirtyClipEnabled)
        cur_state.isClipEnabled = s.isClipEnabled();
    if (f & QPaintEngine::DirtyOpacity)
        cur_state.opacity = s.opacity();
    if (f & QPaintEngine::DirtyPen)
        cur_state.pen = s.pen();
    if (f & QPaintEngine::DirtyHints)
        cur_state.renderHints = s.renderHints();
    if (f & QPaintEngine::DirtyTransform)
        cur_state.transform = s.transform();
    cur_state.state |= f & ~(QPaintEngine::DirtyHints);
}

void JSONBuilder::saveStatePriv()
{
    QPaintEngine::DirtyFlags f = cur_state.state;
    if (f & QPaintEngine::AllDirty)
    {
        render_buffer.append("{\"t\":\"state\"")
              .append(",\"data\":{");

        if (f & QPaintEngine::DirtyPen)
        {
            pen(cur_state.pen);
            render_buffer.append(",");
        }

        if (f & QPaintEngine::DirtyBrush)
        {
            brush(cur_state.brush);
            render_buffer.append(",");
        }

        if (f & QPaintEngine::DirtyFont)
        {
            font(cur_state.font);
            render_buffer.append(",");
        }

        if (f & QPaintEngine::DirtyBrushOrigin)
        {
            render_buffer.append("\"brushorigin\":{\"x\":").append(QString::number(cur_state.brushOrigin.x()))
                  .append(",\"y\":").append(QString::number(cur_state.brushOrigin.y()))
                  .append("},");
        }

        if (f & QPaintEngine::DirtyTransform)
        {
            transform(cur_state.transform);
            render_buffer.append(",");
        }

        if (f & QPaintEngine::DirtyCompositionMode)
        {
            render_buffer.append("\"composition\":");
            switch (cur_state.compositionMode)
            {
                case QPainter::CompositionMode_SourceAtop:
                    render_buffer.append("\"source-atop\":");
                    break;
                case QPainter::CompositionMode_SourceIn:
                    render_buffer.append("\"source-in\":");
                    break;
                case QPainter::CompositionMode_SourceOut:
                    render_buffer.append("\"source-out\":");
                    break;
                case QPainter::CompositionMode_SourceOver:
                    render_buffer.append("\"source-over\":");
                    break;
                case QPainter::CompositionMode_DestinationAtop:
                    render_buffer.append("\"destination-atop\":");
                    break;
                case QPainter::CompositionMode_DestinationIn:
                    render_buffer.append("\"destination-in\":");
                    break;
                case QPainter::CompositionMode_DestinationOut:
                    render_buffer.append("\"destination-out\":");
                    break;
                case QPainter::CompositionMode_DestinationOver:
                    render_buffer.append("\"destination-over\":");
                    break;
                case QPainter::CompositionMode_Lighten:
                    render_buffer.append("\"lighter\":");
                    break;
                case QPainter::CompositionMode_Darken:
                    render_buffer.append("\"darker\":");
                    break;
                case QPainter::CompositionMode_Xor:
                    render_buffer.append("\"xor\":");
                    break;
                case QPainter::CompositionMode_Source:
                    render_buffer.append("\"copy\":");
                    break;
                default:
                    render_buffer.append("\"source-over\":");
                    break;
            }
            render_buffer.append(",");
        }

        if (f & (QPaintEngine::DirtyBackground | QPaintEngine::DirtyBackgroundMode))
        {
            bbrush(cur_state.backgroundBrush);
            render_buffer.append(",");
        }

        if (f & QPaintEngine::DirtyOpacity)
        {
            opacity(cur_state.opacity);
            render_buffer.append(",");
        }

        if (cur_state.isClipEnabled)
        {
            this->clip();
        }

        if (render_buffer[render_buffer.length()-1] == ',')
            render_buffer.remove(render_buffer.length()-1, 1);
        render_buffer.append("}},");

        cur_state.state = 0;
    }
}

////////////// HELPER FUNCTIONS BELOW - NOT COLON SEPARATED /////////////////

void JSONBuilder::clip()
{
    QPainterPath r;
    if (!cur_state.clipRegion.isEmpty())
    {
        r.addRegion(cur_state.clipRegion);
    }

    switch (cur_state.clipOperation)
    {
        case Qt::ReplaceClip:
            if (!r.isEmpty())
                cur_state.prevClipPath = r;
            if (!cur_state.clipPath.isEmpty())
                cur_state.prevClipPath = cur_state.clipPath;
            break;
        case Qt::IntersectClip:
            if (!r.isEmpty())
                cur_state.prevClipPath.intersects(r);
            if (!cur_state.clipPath.isEmpty())
                cur_state.prevClipPath.intersects(cur_state.clipPath);
            break;
        case Qt::UniteClip:
            if (!r.isEmpty())
                cur_state.prevClipPath = cur_state.prevClipPath.united(r);
            if (!cur_state.clipPath.isEmpty())
                cur_state.prevClipPath = cur_state.prevClipPath.united(cur_state.clipPath);
            break;
        default:
            cur_state.prevClipPath = QPainterPath();
    }

    QPainterPath & path = cur_state.prevClipPath;

    render_buffer.append("\"clip\":{");

    if (!path.isEmpty())
    {
        render_buffer.append("\"data\":[");

        for (int i = 0; i < path.elementCount(); ++i)
        {
            QPainterPath::Element e = path.elementAt(i);
            render_buffer.append("{\"t\":").append(QString::number(e.type).toAscii());
            render_buffer.append(",\"p\":[[")
                    .append(QString::number(e.x).toAscii())
                    .append(",")
                    .append(QString::number(e.y).toAscii())
                    .append("],");

            if (e.type == QPainterPath::CurveToElement)
            {
                for (e = path.elementAt(++i); i < path.elementCount(); ++i)
                {
                    render_buffer.append("[")
                            .append(QString::number(e.x).toAscii())
                            .append(",")
                            .append(QString::number(e.y).toAscii())
                            .append("],");
                    e = path.elementAt(i);
                    if (e.type != QPainterPath::CurveToDataElement)
                    {
                        --i;
                        break;
                    }
                }
            }

            if (render_buffer[render_buffer.length()-1] == ',')
                render_buffer.remove(render_buffer.length()-1, 1);

            render_buffer.append("]},");
        }

        if (render_buffer[render_buffer.length()-1] == ',')
            render_buffer.remove(render_buffer.length()-1, 1);

        render_buffer.append("],\"fill\":").append(QString::number(path.fillRule()).toAscii());
    }

    render_buffer.append("}");
    cur_state.clipPath = QPainterPath();
    cur_state.clipRegion = QRegion();
}

void JSONBuilder::color(const QColor & c)
{
    render_buffer.append("\"color\":\"")
          .append(c.name())
          .append("\"");
}

void JSONBuilder::font(const QFont & f)
{
    render_buffer.append("\"font\":\"");

    switch (f.style())
    {
        case QFont::StyleItalic:
            render_buffer.append("italic ");
            break;
        case QFont::StyleOblique:
            render_buffer.append("oblique ");
            break;
        default:
            render_buffer.append("normal ");
            break;
    }

    switch (f.capitalization())
    {
        case QFont::SmallCaps:
            render_buffer.append("small-caps ");
            break;
        default:
            render_buffer.append("normal ");
            break;
    }

    switch (f.weight())
    {
        case QFont::Light:
            render_buffer.append("lighter ");
            break;
        case QFont::DemiBold:
            render_buffer.append("600 ");
            break;
        case QFont::Bold:
            render_buffer.append("700 ");
            break;
        case QFont::Black:
            render_buffer.append("900 ");
            break;
        default:
            render_buffer.append("400 ");
            break;
    }

    if (f.pixelSize() != -1)
        render_buffer.append(QString::number(f.pixelSize()).toAscii()).append("px ");
    else if (f.pointSize() != -1)
        render_buffer.append(QString::number(f.pointSize()).toAscii()).append("pt ");
    else if (f.pointSizeF() != -1)
        render_buffer.append(QString::number(f.pointSizeF()).toAscii()).append("pt ");
    else
        render_buffer.append("medium ");

    render_buffer.append(f.family().toAscii());
    foreach (QString s, QFont::substitutes(f.family()))
        render_buffer.append(",").append(s.toAscii());
    render_buffer.append(",").append(f.defaultFamily().toAscii());
    render_buffer.append("\"");
}

void JSONBuilder::opacity(qreal op)
{
    render_buffer.append("\"opacity\":").append(QString::number(op).toAscii());
}

void JSONBuilder::pen(const QPen & p)
{
    render_buffer.append("\"pen\":{");

    // If brush defined (instead color)
    QBrush b = p.brush();
    if (b.style() != Qt::NoBrush && b.style() != Qt::SolidPattern)
        brush(p.brush());
    else
        color(p.color());

    if (p.style() != Qt::SolidLine)
    {
        qreal w = p.widthF();
        render_buffer.append(",\"dash\":{");
        {
            render_buffer.append("\"offset\":").append(QString::number(p.dashOffset()));
            render_buffer.append(",\"pattern\":[");
            {
                foreach (qreal d, p.dashPattern())
                    render_buffer.append(QString::number(d * w)).append(',');
                if (render_buffer[render_buffer.length()-1] == ',')
                    render_buffer.remove(render_buffer.length()-1, 1);
            }
            render_buffer.append("]");
        }
        render_buffer.append("}");
    }

    int cap = p.capStyle();
    switch (cap)
    {
        case Qt::RoundCap:
            render_buffer.append(",\"cap\":\"round\"");
            break;
        case Qt::SquareCap:
            render_buffer.append(",\"cap\":\"square\"");
            break;
        default:
            render_buffer.append(",\"cap\":\"butt\"");
    }

    render_buffer.append(",\"join\":").append(QString::number(p.joinStyle()));
    if (p.joinStyle() == Qt::MiterJoin)
        render_buffer.append(",\"miter\":").append(QString::number(p.miterLimit()));
    render_buffer.append(",\"width\":").append(QString::number(p.widthF()));
    render_buffer.append('}');
}

void JSONBuilder::brush(const QBrush & b)
{

    render_buffer.append("\"brush\":{")
          .append("\"style\":").append(QString::number(b.style()))
          .append(",");

    switch (b.style())
    {
        case Qt::NoBrush:
            break;
        case Qt::SolidPattern:
            color(b.color());
            break;
        case Qt::LinearGradientPattern:
        case Qt::RadialGradientPattern:
        case Qt::ConicalGradientPattern:
            if (b.gradient())
                gradient(*(b.gradient()));
            break;
        case Qt::TexturePattern:
            {
                QImage img = b.textureImage();
                if (!img.isNull())
                {
                    QByteArray byteArray;
                    QBuffer buf(&byteArray);
                    img.save(&buf, "PNG");
                    render_buffer.append("\"texture\":\"data:image/png;base64,").append(byteArray.toBase64()).append("\"");
                }
                else
                {
                    QPixmap pix = b.texture();
                    if (!pix.isNull())
                    {
                        QByteArray byteArray;
                        QBuffer buf(&byteArray);
                        pix.save(&buf, "PNG");
                        render_buffer.append("\"texture\":\"data:image/png;base64,").append(byteArray.toBase64()).append("\"");
                    }
                }
            }
            break;
        default:
            color(b.color());
            break;
    }

    if (b.style() != Qt::NoBrush && !b.transform().isIdentity())
        transform(b.transform());

    if (render_buffer[render_buffer.length()-1] == ',')
        render_buffer.remove(render_buffer.length()-1, 1);

    render_buffer.append('}');
}

void JSONBuilder::bbrush(const QBrush & b)
{

    render_buffer.append("\"bbrush\":{")
          .append("\"style\":").append(QString::number(b.style()))
          .append(",");

    switch (b.style())
    {
        case Qt::NoBrush:
            break;
        case Qt::SolidPattern:
            color(b.color());
            break;
        case Qt::LinearGradientPattern:
        case Qt::RadialGradientPattern:
        case Qt::ConicalGradientPattern:
            if (b.gradient())
                gradient(*(b.gradient()));
            break;
        case Qt::TexturePattern:
            {
                QImage img = b.textureImage();
                if (!img.isNull())
                {
                    QByteArray byteArray;
                    QBuffer buf(&byteArray);
                    img.save(&buf, "PNG");
                    render_buffer.append("\"image\":\"data:image/png;base64,").append(byteArray.toBase64()).append("\"");
                }
                else
                {
                    QPixmap pix = b.texture();
                    if (!pix.isNull())
                    {
                        QByteArray byteArray;
                        QBuffer buf(&byteArray);
                        pix.save(&buf, "PNG");
                        render_buffer.append("\"image\":\"data:image/png;base64,").append(byteArray.toBase64()).append("\"");
                    }
                }
            }
            break;
        default:
            color(b.color());
            break;
    }

    if (b.style() != Qt::NoBrush && !b.transform().isIdentity())
        transform(b.transform());

    if (render_buffer[render_buffer.length()-1] == ',')
        render_buffer.remove(render_buffer.length()-1, 1);

    render_buffer.append('}');
}

void JSONBuilder::gradient(const QGradient & g)
{
    render_buffer.append("\"gradient\":{");
    render_buffer.append("\"type\":").append(QString::number(g.type()).toAscii());
    if (g.type() == QGradient::LinearGradient)
    {
        const QLinearGradient & lg = (const QLinearGradient&)(g);
        render_buffer.append(",\"xs\":").append(QString::number(lg.start().x()).toAscii());
        render_buffer.append(",\"ys\":").append(QString::number(lg.start().y()).toAscii());
        render_buffer.append(",\"xe\":").append(QString::number(lg.finalStop().x()).toAscii());
        render_buffer.append(",\"ye\":").append(QString::number(lg.finalStop().y()).toAscii());
    }
    else if (g.type() == QGradient::RadialGradient)
    {
        const QRadialGradient & rg = (const QRadialGradient&)(g);
        render_buffer.append(",\"xc\":").append(QString::number(rg.center().x()).toAscii());
        render_buffer.append(",\"yc\":").append(QString::number(rg.center().y()).toAscii());
        render_buffer.append(",\"xf\":").append(QString::number(rg.focalPoint().x()).toAscii());
        render_buffer.append(",\"yf\":").append(QString::number(rg.focalPoint().y()).toAscii());
    }
    else if (g.type() == QGradient::ConicalGradient)
    {
        const QConicalGradient & cg = (const QConicalGradient&)(g);
        render_buffer.append(",\"xc\":").append(QString::number(cg.center().x()).toAscii());
        render_buffer.append(",\"yc\":").append(QString::number(cg.center().y()).toAscii());
        render_buffer.append(",\"a\":").append(QString::number(cg.angle()).toAscii());
    }
    render_buffer.append(",\"stops\":[");
    {
        foreach (QGradientStop stop, g.stops())
            render_buffer.append("[")
                    .append(QString::number(stop.first)).append(",\"")
                    .append(stop.second.name()).append("\"],");
    }
    if (render_buffer[render_buffer.length()-1] == ',')
        render_buffer.remove(render_buffer.length()-1, 1);
    render_buffer.append("],\"spread\":").append(QString::number(g.spread()).toAscii());
    render_buffer.append(",\"mode\":").append(QString::number(g.coordinateMode()).toAscii());
    render_buffer.append("}");
}

void JSONBuilder::transform(const QTransform & t)
{
    render_buffer.append("\"transform\":[[")
          .append(QString::number(t.m11())).append(",")
          .append(QString::number(t.m12())).append(",")
          .append(QString::number(t.m13())).append("],[")
          .append(QString::number(t.m21())).append(",")
          .append(QString::number(t.m22())).append(",")
          .append(QString::number(t.m23())).append("],[")
          .append(QString::number(t.m31())).append(",")
          .append(QString::number(t.m32())).append(",")
            .append(QString::number(t.m33())).append("]]");
}
