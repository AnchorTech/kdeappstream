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

namespace KAppStream
{
    class PaintDevice;

    class JSONBuilder : public QObject
    {
            Q_OBJECT

            QSemaphore _sem;
            QString buffer;

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

            void flush();
            void color(const QColor & c);
            void ellipse(const QRect & r);
            void ellipse(const QRectF & r);
            void font(const QFont & f);
            void image(const QImage & i);
            void line(const QLine & l);
            void line(const QLineF & l);
            void pixmap(const QPixmap & pm);
            void pixmap(const QRectF & r, const QPixmap & pm, const QRectF & sr);
            void rect(const QRect & r);
            void rect(const QRectF & r);
            void state(const QPaintEngineState & s);
            void saveState();
            void pen(const QPen & p);
            void brush(const QBrush & b);
            void gradient(const QGradient & g);
            void transform(const QTransform & t);
        signals:
            void dataReady(const QString &data);
    };
}

#endif // JSONBUILDER_H
