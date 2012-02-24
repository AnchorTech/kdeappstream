#include "PaintEngine.h"

using namespace KAppStream;

PaintEngine::PaintEngine() :
    QPaintEngine()
{
}

PaintEngine::~PaintEngine()
{}

bool PaintEngine::begin(QPaintDevice * pdev)
{
    return false;
}

void PaintEngine::drawEllipse(const QRectF & rect)
{}

void PaintEngine::drawEllipse(const QRect & rect)
{}

void PaintEngine::drawImage(const QRectF & rectangle, const QImage & image, const QRectF & sr, Qt::ImageConversionFlags flags)
{}

void PaintEngine::drawLines(const QLineF * lines, int lineCount)
{}

void PaintEngine::drawLines(const QLine * lines, int lineCount)
{}

void PaintEngine::drawPath(const QPainterPath & path)
{}

void PaintEngine::drawPixmap(const QRectF & r, const QPixmap & pm, const QRectF & sr)
{}

void PaintEngine::drawPoints(const QPointF * points, int pointCount)
{}

void PaintEngine::drawPoints(const QPoint * points, int pointCount)
{}

void PaintEngine::drawPolygon(const QPointF * points, int pointCount, PolygonDrawMode mode)
{}

void PaintEngine::drawPolygon(const QPoint * points, int pointCount, PolygonDrawMode mode)
{}

void PaintEngine::drawRects (const QRectF * rects, int rectCount)
{}

void PaintEngine::drawRects (const QRect * rects, int rectCount)
{}

void PaintEngine::drawTextItem(const QPointF & p, const QTextItem & textItem)
{}

void PaintEngine::drawTiledPixmap(const QRectF & rect, const QPixmap & pixmap, const QPointF & p)
{}

bool PaintEngine::end()
{
    return false;
}

PaintEngine::Type PaintEngine::type() const
{
    return QPaintEngine::SVG;
}

void PaintEngine::updateState (const QPaintEngineState & state)
{}
