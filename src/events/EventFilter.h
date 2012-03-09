#ifndef EVENTFILTER_H
#define EVENTFILTER_H

#include <QObject>

namespace KAppStream
{
    class PaintDevice;

    class EventFilter : public QObject
    {
            Q_OBJECT

        public:

            explicit EventFilter(QObject * parent = 0);
            virtual ~EventFilter();
            bool eventFilter(QObject * recv, QEvent * e);

        signals:

            void render(QWidget *);
    };
}

#endif // EVENTFILTER_H
