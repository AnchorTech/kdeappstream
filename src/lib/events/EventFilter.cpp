#include "EventFilter.h"

#include "WebRenderer.h"
#include "websocket/WebsocketServer.h"
#include "paint/JSONBuilder.h"

#include <QEvent>
#include <QDebug>
#include <QWidget>
#include <QPainter>
#include <QApplication>
#include <QPaintEvent>

using namespace KAppStream;

bool hasConnection = false;

EventFilter::EventFilter(QObject *parent) :
    QObject(parent)
{
    WebRenderer::instance(this);
}

EventFilter::~EventFilter()
{
}

void EventFilter::connected()
{
    hasConnection = true;
    qDebug() << "connection";

}

void EventFilter::disconnected()
{
    hasConnection = false;
    qDebug() << "no connection";
}

bool EventFilter::eventFilter(QObject * recv, QEvent * e)
{
    QWidget * w = dynamic_cast<QWidget*>(recv);

    if (w)
    {
        switch (e->type())
        {
            case QEvent::None:
                qDebug() << "QEvent::None" << recv;
                break;
            case QEvent::AccessibilityDescription:
                qDebug() << "QEvent::AccessibilityDescription" << recv;
                break;
            case QEvent::AccessibilityHelp:
                qDebug() << "QEvent::AccessibilityHelp" << recv;
                break;
            case QEvent::AccessibilityPrepare:
                qDebug() << "QEvent::AccessibilityPrepare" << recv;
                break;
            case QEvent::ActionAdded:
                qDebug() << "QEvent::ActionAdded" << recv;
                break;
            case QEvent::ActionChanged:
                qDebug() << "QEvent::ActionChanged" << recv;
                break;
            case QEvent::ActionRemoved:
                qDebug() << "QEvent::ActionRemoved" << recv;
                break;
            case QEvent::ActivationChange:
                qDebug() << "QEvent::ActivationChange" << recv << e;
                break;
            case QEvent::ApplicationActivate:
                qDebug() << "QEvent::ApplicationActivate ApplicationActivated" << recv;
                break;
            case QEvent::ApplicationDeactivate:
                qDebug() << "QEvent::ApplicationDeactivate" << recv;
                break;
            case QEvent::ApplicationFontChange:
                qDebug() << "QEvent::ApplicationFontChange" << recv;
                break;
            case QEvent::ApplicationLayoutDirectionChange:
                qDebug() << "QEvent::ApplicationLayoutDirectionChange" << recv;
                break;
            case QEvent::ApplicationPaletteChange:
                qDebug() << "QEvent::ApplicationPaletteChange" << recv;
                break;
            case QEvent::ApplicationWindowIconChange:
                qDebug() << "QEvent::ApplicationWindowIconChange" << recv;
                break;
            case QEvent::ChildAdded:
                qDebug() << "QEvent::ChildAdded" << (long long) recv;
                {
                    QChildEvent * ce = (QChildEvent*)e;
                    QWidget * cw = dynamic_cast<QWidget*>(ce->child());
                    if (cw)
                        JSONBuilder::instance()->addChild(cw, w);
                }
                break;
            case QEvent::ChildPolished:
                qDebug() << "QEvent::ChildPolished" << (long long) recv;
                break;
            case QEvent::ChildRemoved:
                qDebug() << "QEvent::ChildRemoved" << (long long) recv << e;
                {
                    QChildEvent * ce = (QChildEvent*)e;
                    QWidget * cw = dynamic_cast<QWidget*>(ce->child());
                    if (cw)
                    {
                        WebRenderer::instance()->dequeue(cw);
                        JSONBuilder::instance()->removeChild(cw, w);
                    }
                }
                break;
            case QEvent::Clipboard:
                qDebug() << "QEvent::Clipboard" << recv;
                break;
            case QEvent::Close:
                qDebug() << "QEvent::Close" << recv;
                WebRenderer::instance()->dequeue(w);
                break;
            case QEvent::CloseSoftwareInputPanel:
                qDebug() << "QEvent::CloseSoftwareInputPanel" << recv;
                break;
            case QEvent::ContentsRectChange:
                qDebug() << "QEvent::ContentsRectChange" << recv;
                break;
            case QEvent::ContextMenu:
                qDebug() << "QEvent::ContextMenu" << recv;
                break;
            case QEvent::CursorChange:
                qDebug() << "QEvent::CursorChange" << recv;
                break;
            case QEvent::DeferredDelete:
                qDebug() << "QEvent::DeferredDelete" << recv;
                break;
            case QEvent::DragEnter:
                qDebug() << "QEvent::DragEnter" << recv;
                break;
            case QEvent::DragLeave:
                qDebug() << "QEvent::DragLeave" << recv;
                break;
            case QEvent::DragMove:
                qDebug() << "QEvent::DragMove" << recv;
                break;
            case QEvent::Drop:
                qDebug() << "QEvent::Drop" << recv;
                break;
            case QEvent::EnabledChange:
                qDebug() << "QEvent::EnabledChange" << recv;
                break;
            case QEvent::Enter:
                qDebug() << "QEvent::Enter" << recv;
                break;
            case QEvent::EnterWhatsThisMode:
                qDebug() << "QEvent::EnterWhatsThisMode" << recv;
                break;
            case QEvent::FileOpen:
                qDebug() << "QEvent::FileOpen" << recv;
                break;
            case QEvent::FocusIn:
                qDebug() << "QEvent::FocusIn" << recv;
                break;
            case QEvent::FocusOut:
                qDebug() << "QEvent::FocusOut" << recv;
                break;
            case QEvent::FontChange:
                qDebug() << "QEvent::FontChange" << recv;
                break;
            case QEvent::GrabKeyboard:
                qDebug() << "QEvent::GrabKeyboard" << recv;
                break;
            case QEvent::GrabMouse:
                qDebug() << "QEvent::GrabMouse" << recv;
                break;
            case QEvent::GraphicsSceneContextMenu:
                qDebug() << "QEvent::GraphicsSceneContextMenu" << recv;
                break;
            case QEvent::GraphicsSceneDragEnter:
                qDebug() << "QEvent::GraphicsSceneDragEnter" << recv;
                break;
            case QEvent::GraphicsSceneDragLeave:
                qDebug() << "QEvent::GraphicsSceneDragLeave" << recv;
                break;
            case QEvent::GraphicsSceneDragMove:
                qDebug() << "QEvent::GraphicsSceneDragMove" << recv;
                break;
            case QEvent::GraphicsSceneDrop:
                qDebug() << "QEvent::GraphicsSceneDrop" << recv;
                break;
            case QEvent::GraphicsSceneHelp:
                qDebug() << "QEvent::GraphicsSceneHelp" << recv;
                break;
            case QEvent::GraphicsSceneHoverEnter:
                qDebug() << "QEvent::GraphicsSceneHoverEnter" << recv;
                break;
            case QEvent::GraphicsSceneHoverLeave:
                qDebug() << "QEvent::GraphicsSceneHoverLeave" << recv;
                break;
            case QEvent::GraphicsSceneHoverMove:
                qDebug() << "QEvent::GraphicsSceneHoverMove" << recv;
                break;
            case QEvent::GraphicsSceneMouseDoubleClick:
                qDebug() << "QEvent::GraphicsSceneMouseDoubleClick" << recv;
                break;
            case QEvent::GraphicsSceneMouseMove:
                qDebug() << "QEvent::GraphicsSceneMouseMove" << recv;
                break;
            case QEvent::GraphicsSceneMousePress:
                qDebug() << "QEvent::GraphicsSceneMousePress" << recv;
                break;
            case QEvent::GraphicsSceneMouseRelease:
                qDebug() << "QEvent::GraphicsSceneMouseRelease" << recv;
                break;
            case QEvent::GraphicsSceneMove:
                qDebug() << "QEvent::GraphicsSceneMove" << recv;
                break;
            case QEvent::GraphicsSceneResize:
                qDebug() << "QEvent::GraphicsSceneResize" << recv;
                break;
            case QEvent::GraphicsSceneWheel:
                qDebug() << "QEvent::GraphicsSceneWheel" << recv;
                break;
            case QEvent::Hide:
                qDebug() << "QEvent::Hide" << recv;
                WebRenderer::instance()->dequeue(w);
                JSONBuilder::instance()->hideWidget(w);
                break;
            case QEvent::HideToParent:
                qDebug() << "QEvent::HideToParent" << recv;
                WebRenderer::instance()->dequeue(w);
                JSONBuilder::instance()->hideWidget(w);
                break;
            case QEvent::HoverEnter:
                qDebug() << "QEvent::HoverEnter" << recv;
                break;
            case QEvent::HoverLeave:
                qDebug() << "QEvent::HoverLeave" << recv;
                break;
            case QEvent::HoverMove:
                qDebug() << "QEvent::HoverMove" << recv;
                break;
            case QEvent::IconDrag:
                qDebug() << "QEvent::IconDrag" << recv;
                break;
            case QEvent::IconTextChange:
                qDebug() << "QEvent::IconTextChange" << recv;
                break;
            case QEvent::InputMethod:
                qDebug() << "QEvent::InputMethod" << recv;
                break;
            case QEvent::KeyPress:
                qDebug() << "QEvent::KeyPress" << recv;
                break;
            case QEvent::KeyRelease:
                qDebug() << "QEvent::KeyRelease" << recv;
                break;
            case QEvent::LanguageChange:
                qDebug() << "QEvent::LanguageChange" << recv;
                break;
            case QEvent::LayoutDirectionChange:
                qDebug() << "QEvent::LayoutDirectionChange" << recv;
                break;
            case QEvent::LayoutRequest:
                qDebug() << "QEvent::LayoutRequest" << recv;
                break;
            case QEvent::Leave:
                qDebug() << "QEvent::Leave" << recv;
                break;
            case QEvent::LeaveWhatsThisMode:
                qDebug() << "QEvent::LeaveWhatsThisMode" << recv;
                break;
            case QEvent::LocaleChange:
                qDebug() << "QEvent::LocaleChange" << recv;
                break;
            case QEvent::NonClientAreaMouseButtonDblClick:
                qDebug() << "QEvent::NonClientAreaMouseButtonDblClick" << recv;
                break;
            case QEvent::NonClientAreaMouseButtonPress:
                qDebug() << "QEvent::NonClientAreaMouseButtonPress" << recv;
                break;
            case QEvent::NonClientAreaMouseButtonRelease:
                qDebug() << "QEvent::NonClientAreaMouseButtonRelease" << recv;
                break;
            case QEvent::NonClientAreaMouseMove:
                qDebug() << "QEvent::NonClientAreaMouseMove" << recv;
                break;
            case QEvent::MacSizeChange:
                qDebug() << "QEvent::MacSizeChange" << recv;
                break;
            case QEvent::MenubarUpdated:
                qDebug() << "QEvent::MenubarUpdated" << recv;
                break;
            case QEvent::MetaCall:
                qDebug() << "QEvent::MetaCall" << recv;
                break;
            case QEvent::ModifiedChange:
                qDebug() << "QEvent::ModifiedChange" << recv;
                break;
            case QEvent::MouseButtonDblClick:
                qDebug() << "QEvent::MouseButtonDblClick" << recv;
                break;
            case QEvent::MouseButtonPress:
                qDebug() << "QEvent::MouseButtonPress" << recv;
                break;
            case QEvent::MouseButtonRelease:
                qDebug() << "QEvent::MouseButtonRelease" << recv;
                break;
            case QEvent::MouseMove:
                qDebug() << "QEvent::MouseMove" << recv;
                break;
            case QEvent::MouseTrackingChange:
                qDebug() << "QEvent::MouseTrackingChange" << recv;
                break;
            case QEvent::Move:
                qDebug() << "QEvent::Move" << recv;
                break;
            case QEvent::Paint:
                {
                    QPaintEvent * pe = dynamic_cast<QPaintEvent*>(e);
                    WebRenderer::instance()->queue(w, pe);
                    //return pe->isAccepted();
                }
                break;
            case QEvent::PaletteChange:
                qDebug() << "QEvent::PaletteChange" << recv;
                break;
            case QEvent::ParentAboutToChange:
                qDebug() << "QEvent::ParentAboutToChange" << recv;
                break;
            case QEvent::ParentChange:
                qDebug() << "QEvent::ParentChange" << recv;
                break;
            case QEvent::Polish:
                qDebug() << "QEvent::Polish" << recv;
                break;
            case QEvent::PolishRequest:
                qDebug() << "QEvent::PolishRequest" << recv;
                break;
            case QEvent::QueryWhatsThis:
                qDebug() << "QEvent::QueryWhatsThis" << recv;
                break;
            case QEvent::RequestSoftwareInputPanel:
                qDebug() << "QEvent::RequestSoftwareInputPanel" << recv;
                break;
            case QEvent::Resize:
                {
                    QResizeEvent * re = (QResizeEvent*)e;
                    JSONBuilder::instance()->resize(w, re->oldSize(), re->size());
                }
                break;
            case QEvent::Shortcut:
                qDebug() << "QEvent::Shortcut" << recv;
                break;
            case QEvent::ShortcutOverride:
                qDebug() << "QEvent::ShortcutOverride" << recv;
                break;
            case QEvent::Show:
                qDebug() << "QEvent::Show" << recv;
                JSONBuilder::instance()->showWidget(w);
                break;
            case QEvent::ShowToParent:
                qDebug() << "QEvent::ShowToParent" << recv;
                break;
            case QEvent::SockAct:
                qDebug() << "QEvent::SockAct" << recv;
                break;
            case QEvent::StateMachineSignal:
                qDebug() << "QEvent::StateMachineSignal" << recv;
                break;
            case QEvent::StateMachineWrapped:
                qDebug() << "QEvent::StateMachineWrapped" << recv;
                break;
            case QEvent::StatusTip:
                qDebug() << "QEvent::StatusTip" << recv;
                break;
            case QEvent::StyleChange:
                qDebug() << "QEvent::StyleChange" << recv;
                break;
            case QEvent::TabletMove:
                qDebug() << "QEvent::TabletMove" << recv;
                break;
            case QEvent::TabletPress:
                qDebug() << "QEvent::TabletPress" << recv;
                break;
            case QEvent::TabletRelease:
                qDebug() << "QEvent::TabletRelease" << recv;
                break;
            case QEvent::OkRequest:
                qDebug() << "QEvent::OkRequest" << recv;
                break;
            case QEvent::TabletEnterProximity:
                qDebug() << "QEvent::TabletEnterProximity" << recv;
                break;
            case QEvent::TabletLeaveProximity:
                qDebug() << "QEvent::TabletLeaveProximity" << recv;
                break;
            case QEvent::Timer:
                qDebug() << "QEvent::Timer" << recv;
                break;
            case QEvent::ToolBarChange:
                qDebug() << "QEvent::ToolBarChange" << recv;
                break;
            case QEvent::ToolTip:
                qDebug() << "QEvent::ToolTip" << recv;
                break;
            case QEvent::ToolTipChange:
                qDebug() << "QEvent::ToolTipChange" << recv;
                break;
            case QEvent::UngrabKeyboard:
                qDebug() << "QEvent::UngrabKeyboard" << recv;
                break;
            case QEvent::UngrabMouse:
                qDebug() << "QEvent::UngrabMouse" << recv;
                break;
            case QEvent::UpdateLater:
                qDebug() << "QEvent::UpdateLater" << recv;
                break;
            case QEvent::UpdateRequest:
                qDebug() << "QEvent::UpdateRequest" << recv;
                break;
            case QEvent::WhatsThis:
                qDebug() << "QEvent::WhatsThis" << recv;
                break;
            case QEvent::WhatsThisClicked:
                qDebug() << "QEvent::WhatsThisClicked" << recv;
                break;
            case QEvent::Wheel:
                qDebug() << "QEvent::Wheel" << recv;
                break;
            case QEvent::WinEventAct:
                qDebug() << "QEvent::WinEventAct" << recv;
                break;
            case QEvent::WindowActivate:
                qDebug() << "QEvent::WindowActivate" << recv;
                break;
            case QEvent::WindowBlocked:
                qDebug() << "QEvent::WindowBlocked" << recv;
                break;
            case QEvent::WindowDeactivate:
                qDebug() << "QEvent::WindowDeactivate" << recv << e;
                break;
            case QEvent::WindowIconChange:
                qDebug() << "QEvent::WindowIconChange" << recv;
                break;
            case QEvent::WindowStateChange:
                qDebug() << "QEvent::WindowStateChange" << recv << e;
                break;
            case QEvent::WindowTitleChange:
                qDebug() << "QEvent::WindowTitleChange" << recv;
                break;
            case QEvent::WindowUnblocked:
                qDebug() << "QEvent::WindowUnblocked" << recv;
                break;
            case QEvent::ZOrderChange:
                qDebug() << "QEvent::ZOrderChange" << recv;
                break;
            case QEvent::KeyboardLayoutChange:
                qDebug() << "QEvent::KeyboardLayoutChange" << recv;
                break;
            case QEvent::DynamicPropertyChange:
                qDebug() << "QEvent::DynamicPropertyChange" << recv;
                break;
            case QEvent::TouchBegin:
                qDebug() << "QEvent::TouchBegin" << recv;
                break;
            case QEvent::TouchUpdate:
                qDebug() << "QEvent::TouchUpdate" << recv;
                break;
            case QEvent::TouchEnd:
                qDebug() << "QEvent::TouchEnd" << recv;
                break;
            case QEvent::WinIdChange:
                qDebug() << "QEvent::WinIdChange" << recv;
                break;
            case QEvent::Gesture:
                qDebug() << "QEvent::Gesture" << recv;
                break;
            case QEvent::GestureOverride:
                qDebug() << "QEvent::GestureOverride" << recv;
                break;
        }
    }
    return false;
}
