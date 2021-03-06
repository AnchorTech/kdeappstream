#ifndef EVENT_H
#define EVENT_H

#include <QEvent>
#include <QPoint>
#include <QWidget>

class Event : public QEvent
{
    public:

        Event(QEvent::Type type) :
            QEvent(type)
        {}
};

class Activate : public Event
{
        static int _type;

    public:

        Activate() :
            Event((QEvent::Type)_type)
        {}

        static QEvent::Type eventType()
        {
            return (QEvent::Type) _type;
        }
};

class MouseEvent : public Event
{
        QPoint _pos;
        QPoint _globalPos;
        Qt::MouseButton _button;
        Qt::MouseButtons _buttons;
        Qt::KeyboardModifiers _modifiers;

    public:

        MouseEvent(QEvent::Type type, const QPoint & pos, const QPoint & globalPos, Qt::MouseButton button, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers) :
            Event(type),
            _pos(pos),
            _globalPos(globalPos),
            _button(button),
            _buttons(buttons),
            _modifiers(modifiers)
        {}

        Qt::MouseButton button() const
        {
            return _button;
        }

        Qt::MouseButtons buttons() const
        {
            return _buttons;
        }

        Qt::KeyboardModifiers modifiers() const
        {
            return _modifiers;
        }

        QPoint pos() const
        {
            return _pos;
        }

        QPoint globalPos() const
        {
            return _globalPos;
        }
};

class MousePressEvent : public MouseEvent
{
        static int _type;

    public:

        MousePressEvent(const QPoint & pos, const QPoint & globalPos, Qt::MouseButton button, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers) :
            MouseEvent((QEvent::Type)_type, pos, globalPos, button, buttons, modifiers)
        {}

        static QEvent::Type eventType()
        {
            return (QEvent::Type) _type;
        }
};

class MouseReleaseEvent : public MouseEvent
{
        static int _type;

    public:

        MouseReleaseEvent(const QPoint & pos, const QPoint & globalPos, Qt::MouseButton button, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers) :
            MouseEvent((QEvent::Type)_type, pos, globalPos, button, buttons, modifiers)
        {}

        static QEvent::Type eventType()
        {
            return (QEvent::Type) _type;
        }
};

class MouseMoveEvent : public MouseEvent
{
        static int _type;
        const QPoint _oldPos;

    public:

        MouseMoveEvent(const QPoint & pos, const QPoint & globalPos, const QPoint & oldPos, Qt::MouseButton button, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers) :
            MouseEvent((QEvent::Type)_type, pos, globalPos, button, buttons, modifiers),
            _oldPos(oldPos)
        {}

        QPoint oldPos() const
        {
            return _oldPos;
        }

        static QEvent::Type eventType()
        {
            return (QEvent::Type) _type;
        }
};

class MouseDbClickEvent : public MouseEvent
{
        static int _type;

    public:

        MouseDbClickEvent(const QPoint & pos, const QPoint & globalPos, Qt::MouseButton button, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers) :
            MouseEvent((QEvent::Type)_type, pos, globalPos, button, buttons, modifiers)
        {}

        static QEvent::Type eventType()
        {
            return (QEvent::Type) _type;
        }
};

class MouseWheelEvent : public MouseEvent
{
        static int _type;
        int _delta;
        Qt::Orientation _orientation;

    public:

        MouseWheelEvent(const QPoint & pos, const QPoint & globalPos, int delta, Qt::Orientation orientation, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers) :
            MouseEvent((QEvent::Type)_type, pos, globalPos, (Qt::MouseButton) 0, buttons, modifiers),
            _delta(delta),
            _orientation(orientation)
        {}

        int delta()
        {
            return _delta;
        }

        Qt::Orientation orientation()
        {
            return (Qt::Orientation) _orientation;
        }

        static QEvent::Type eventType()
        {
            return (QEvent::Type) _type;
        }
};

class EnterEvent : public Event
{
        static int _type;

        QPoint _pos;

    public:

        EnterEvent(const QPoint & pos) :
            Event((QEvent::Type)_type),
            _pos(pos)
        {}

        QPoint pos() const
        {
            return _pos;
        }

        static QEvent::Type eventType()
        {
            return (QEvent::Type) _type;
        }
};

class LeaveEvent : public Event
{
        static int _type;

        QPoint _pos;

    public:

        LeaveEvent(const QPoint & pos) :
            Event((QEvent::Type)_type),
            _pos(pos)
        {}

        QPoint pos() const
        {
            return _pos;
        }

        static QEvent::Type eventType()
        {
            return (QEvent::Type) _type;
        }
};

class CloseEvent : public Event
{
        static int _type;

    public:

        CloseEvent() :
            Event((QEvent::Type)_type)
        {}

        static QEvent::Type eventType()
        {
            return (QEvent::Type) _type;
        }
};

class ResizeEvent : public Event
{
        static int _type;
        int w;
        int h;

    public:

        ResizeEvent(int w, int h) :
            Event((QEvent::Type)_type),
            w(w),
            h(h)
        {}

        ResizeEvent(const QSize & s) :
            Event((QEvent::Type)_type),
            w(s.width()),
            h(s.height())
        {}

        int width() const
        {
            return w;
        }

        int height() const
        {
            return h;
        }

        static QEvent::Type eventType()
        {
            return (QEvent::Type) _type;
        }
};

#endif // EVENT_H
