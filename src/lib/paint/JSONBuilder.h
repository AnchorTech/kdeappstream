#ifndef JSONBUILDER_H
#define JSONBUILDER_H

#include <QRect>
#include <QRectF>
#include <QLine>
#include <QLineF>
#include <QObject>
#include <QQueue>
#include <QSemaphore>
#include <QPixmap>
#include <QPaintEngineState>

#include "paint/ImagesHostServer.h"

class QWsSocket;

namespace KAppStream
{
    class PaintDevice;

    class JSONBuilder : public QObject
    {
            Q_OBJECT

            QSemaphore _render_sem;
            QByteArray render_buffer;

            QSemaphore _layout_sem;
            QByteArray layout_buffer;

            QList<QWidget*> context;

            struct State
            {
                    State()
                    {
                        state = 0;
                    }

                    QPaintEngine::DirtyFlags state;

                    QBrush backgroundBrush;
                    Qt::BGMode backgroundMode;
                    QBrush brush;
                    QPointF brushOrigin;
                    Qt::ClipOperation clipOperation;
                    QPainterPath clipPath;
                    QRegion clipRegion;
                    QPainterPath prevClipPath;
                    QPainter::CompositionMode compositionMode;
                    QFont font;
                    bool isClipEnabled;
                    qreal opacity;
                    QPen pen;
                    QPainter::RenderHints renderHints;
                    QTransform transform;

            } cur_state;

            static JSONBuilder * m_instance;

            JSONBuilder(QObject * parent = 0);

        public:

            static JSONBuilder * instance(QObject * parent = 0);

            void beginRender(QWidget * widget, const QRect & rect);
            void endRender();

            void activateWindow(QWidget * window);
            void deactivateWindow(QWidget * window);

            void addChild(QWidget * child, QWidget * parent);
            void removeChild(QWidget * child, QWidget * parent);

            void hideWidget(QWidget * widget);
            void showWidget(QWidget * widget);
            void titleChange(QWidget * widget, const QString & title);
            void move(QWidget * widget, const QPoint & pos);
            void ZOrderChange(QWidget * widget);

            void finish();
            void flush(QWsSocket * device);
            void ellipse(const QRect & r);
            void ellipse(const QRectF & r);
            void image(const QRectF & rectangle, const QImage & image, const QRectF & sr, Qt::ImageConversionFlags flags);
            void image(const QImage & i, const QPointF & p = QPointF(0,0));
            void line(const QLine & l);
            void line(const QLineF & l);
            void path(const QPainterPath & path);
            void pixmap(const QRectF & r, const QPixmap & pm, const QRectF & sr);
            void pixmap(const QPixmap & pm, const QPointF & p = QPointF(0,0));
            void points(const QPointF * points, int pointCount);
            void points(const QPoint * points, int pointCount);
            void polygon(const QPointF * points, int pointCount, QPaintEngine::PolygonDrawMode mode);
            void polygon(const QPoint * points, int pointCount, QPaintEngine::PolygonDrawMode mode);
            void rect(const QRect & r);
            void rect(const QRectF & r);
            void text(const QPointF & p, const QTextItem & textItem);
            void tiledPixmap(const QRectF & rect, const QPixmap & pixmap, const QPointF & p);
            void resize(QWidget * w, const QSize & oldSize, const QSize & newSize);
            void state(const QPaintEngineState & s);
            void saveState();

        private:

            void clip();
            void color(const QColor & c);
            void font(const QFont & f);
            void opacity(qreal op);
            void pen(const QPen & p);
            void brush(const QBrush & b);
            void bbrush(const QBrush & b);
            void gradient(const QGradient & g);
            void transform(const QTransform & t);
            void saveStatePriv();
            void image(const QByteArray & id, const QPointF & p);
            void pixmap(const QByteArray & id, const QPointF & p);

        signals:

            void readyRead();
            void bufferImage(const QImage & image, qreal * id);
            void bufferPixmap(const QPixmap & pixmap, qreal * id);
    };
}

#endif // JSONBUILDER_H
