#include "Event.h"

int Activate::_type = QEvent::registerEventType();
int MousePressEvent::_type = QEvent::registerEventType();
int MouseReleaseEvent::_type = QEvent::registerEventType();
int MouseMoveEvent::_type = QEvent::registerEventType();
int MouseDbClickEvent::_type = QEvent::registerEventType();
int MouseWheelEvent::_type = QEvent::registerEventType();
int EnterEvent::_type = QEvent::registerEventType();
int LeaveEvent::_type = QEvent::registerEventType();
int CloseEvent::_type = QEvent::registerEventType();
