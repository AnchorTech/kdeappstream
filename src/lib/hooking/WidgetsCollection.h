#ifndef WIDGETSCOLLECTION_H
#define WIDGETSCOLLECTION_H

#include <QObject>
#include <QSemaphore>
#include <QSet>

class WidgetsCollection : public QObject
{
        Q_OBJECT

        static WidgetsCollection * m_instance;

        explicit WidgetsCollection(QObject *parent = 0);

        QSet<QWidget*> m_widgets;

        QSemaphore m_sem;

    public:

        static WidgetsCollection * instance(QObject * parent = 0);

        bool contains(QWidget * widget);

        bool add(QWidget * widget);

        bool remove(QWidget * widget);

};

#endif // WIDGETSCOLLECTION_H
