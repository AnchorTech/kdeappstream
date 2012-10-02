#include <QApplication>

#include <stdlib.h>
#include <X11/Xlib.h>

QT_BEGIN_NAMESPACE

extern void qt_get_net_virtual_roots();
extern void qt_get_net_supported();

int QApplication::x11ProcessEvent(XEvent* event)
{
    Q_D(QApplication);
    QScopedLoopLevelCounter loopLevelCounter(d->threadData);

//#ifdef ALIEN_DEBUG
//    //qDebug() << "QApplication::x11ProcessEvent:" << event->type;
//#endif
//    switch (event->type) {
//    case ButtonPress:
//        pressed_window = event->xbutton.window;
//        X11->userTime = event->xbutton.time;
//        // fallthrough intended
//    case ButtonRelease:
//        X11->time = event->xbutton.time;
//        break;
//    case MotionNotify:
//        X11->time = event->xmotion.time;
//        break;
//    case XKeyPress:
//        X11->userTime = event->xkey.time;
//        // fallthrough intended
//    case XKeyRelease:
//        X11->time = event->xkey.time;
//        break;
//    case PropertyNotify:
//        X11->time = event->xproperty.time;
//        break;
//    case EnterNotify:
//    case LeaveNotify:
//        X11->time = event->xcrossing.time;
//        break;
//    case SelectionClear:
//        X11->time = event->xselectionclear.time;
//        break;
//    default:
//        break;
//    }
//#ifndef QT_NO_XFIXES
//    if (X11->use_xfixes && event->type == (X11->xfixes_eventbase + XFixesSelectionNotify)) {
//        XFixesSelectionNotifyEvent *req =
//            reinterpret_cast<XFixesSelectionNotifyEvent *>(event);
//        X11->time = req->selection_timestamp;
//        if (req->selection == ATOM(_NET_WM_CM_S0))
//            X11->compositingManagerRunning = req->owner;
//    }
//#endif

//    QETWidget *widget = (QETWidget*)QWidget::find((WId)event->xany.window);

//    if (wPRmapper) {                                // just did a widget reparent?
//        if (widget == 0) {                        // not in std widget mapper
//            switch (event->type) {                // only for mouse/key events
//            case ButtonPress:
//            case ButtonRelease:
//            case MotionNotify:
//            case XKeyPress:
//            case XKeyRelease:
//                widget = qPRFindWidget(event->xany.window);
//                break;
//            }
//        }
//        else if (widget->testAttribute(Qt::WA_WState_Reparented))
//            qPRCleanup(widget);                // remove from alt mapper
//    }

//    QETWidget *keywidget=0;
//    bool grabbed=false;
//    if (event->type==XKeyPress || event->type==XKeyRelease) {
//        keywidget = (QETWidget*)QWidget::keyboardGrabber();
//        if (keywidget) {
//            grabbed = true;
//        } else if (!keywidget) {
//            if (d->inPopupMode()) // no focus widget, see if we have a popup
//                keywidget = (QETWidget*) (activePopupWidget()->focusWidget() ? activePopupWidget()->focusWidget() : activePopupWidget());
//            else if (QApplicationPrivate::focus_widget)
//                keywidget = (QETWidget*)QApplicationPrivate::focus_widget;
//            else if (widget)
//                keywidget = (QETWidget*)widget->window();
//        }
//    }

//#ifndef QT_NO_IM
//    // Filtering input events by the input context. It has to be taken
//    // place before any other key event consumers such as eventfilters
//    // and accelerators because some input methods require quite
//    // various key combination and sequences. It often conflicts with
//    // accelerators and so on, so we must give the input context the
//    // filtering opportunity first to ensure all input methods work
//    // properly regardless of application design.

//    if(keywidget && keywidget->isEnabled() && keywidget->testAttribute(Qt::WA_InputMethodEnabled)) {
//        // block user interaction during session management
//	if((event->type==XKeyPress || event->type==XKeyRelease) && qt_sm_blockUserInput)
//	    return true;

//        // for XIM handling
//	QInputContext *qic = keywidget->inputContext();
//	if(qic && qic->x11FilterEvent(keywidget, event))
//	    return true;

//	// filterEvent() accepts QEvent *event rather than preexpanded
//	// key event attribute values. This is intended to pass other
//	// QInputEvent in future. Other non IM-related events should
//	// not be forwarded to input contexts to prevent weird event
//	// handling.
//	if ((event->type == XKeyPress || event->type == XKeyRelease)) {
//	    int code = -1;
//	    int count = 0;
//	    Qt::KeyboardModifiers modifiers;
//	    QEvent::Type type;
//	    QString text;
//            KeySym keySym;

//            qt_keymapper_private()->translateKeyEventInternal(keywidget, event, keySym, count,
//                                                              text, modifiers, code, type, false);

//	    // both key press/release is required for some complex
//	    // input methods. don't eliminate anything.
//	    QKeyEventEx keyevent(type, code, modifiers, text, false, qMax(qMax(count, 1), text.length()),
//                                 event->xkey.keycode, keySym, event->xkey.state);
//	    if(qic && qic->filterEvent(&keyevent))
//		return true;
//	}
//    } else
//#endif // QT_NO_IM
//        {
//            if (XFilterEvent(event, XNone))
//                return true;
//        }

//    if (qt_x11EventFilter(event))                // send through app filter
//        return 1;

//    if (event->type == MappingNotify) {
//        // keyboard mapping changed
//        XRefreshKeyboardMapping(&event->xmapping);

//        QKeyMapper::changeKeyboard();
//        return 0;
//    }
//#ifndef QT_NO_XKB
//    else if (X11->use_xkb && event->type == X11->xkb_eventbase) {
//        XkbAnyEvent *xkbevent = (XkbAnyEvent *) event;
//        switch (xkbevent->xkb_type) {
//        case XkbStateNotify:
//            {
//                XkbStateNotifyEvent *xkbstateevent = (XkbStateNotifyEvent *) xkbevent;
//                if ((xkbstateevent->changed & XkbGroupStateMask) != 0) {
//                    qt_keymapper_private()->xkb_currentGroup = xkbstateevent->group;
//                    QKeyMapper::changeKeyboard();
//                }
//                break;
//            }
//        default:
//            break;
//        }
//    }
//#endif

//    if (!widget) {                                // don't know this windows
//        QWidget* popup = QApplication::activePopupWidget();
//        if (popup) {

//            /*
//              That is more than suboptimal. The real solution should
//              do some keyevent and buttonevent translation, so that
//              the popup still continues to work as the user expects.
//              Unfortunately this translation is currently only
//              possible with a known widget. I'll change that soon
//              (Matthias).
//            */

//            // Danger - make sure we don't lock the server
//            switch (event->type) {
//            case ButtonPress:
//            case ButtonRelease:
//            case XKeyPress:
//            case XKeyRelease:
//                do {
//                    popup->close();
//                } while ((popup = qApp->activePopupWidget()));
//                return 1;
//            }
//        }
//        return -1;
//    }

//    if (event->type == XKeyPress || event->type == XKeyRelease)
//        widget = keywidget; // send XKeyEvents through keywidget->x11Event()

//    if (app_do_modal)                                // modal event handling
//        if (!qt_try_modal(widget, event)) {
//            if (event->type == ClientMessage && !widget->x11Event(event))
//                x11ClientMessage(widget, event, true);
//            return 1;
//        }


//    if (widget->x11Event(event))                // send through widget filter
//        return 1;
//#if !defined (QT_NO_TABLET)
//    if (!qt_xdnd_dragging) {
//        QTabletDeviceDataList *tablets = qt_tablet_devices();
//        for (int i = 0; i < tablets->size(); ++i) {
//            QTabletDeviceData &tab = tablets->operator [](i);
//            if (event->type == tab.xinput_motion
//            || event->type == tab.xinput_button_release
//            || event->type == tab.xinput_button_press
//            || event->type == tab.xinput_proximity_in
//            || event->type == tab.xinput_proximity_out) {
//                widget->translateXinputEvent(event, &tab);
//                return 0;
//            }
//        }
//    }
//#endif

//#ifndef QT_NO_XRANDR
//    if (X11->use_xrandr && event->type == (X11->xrandr_eventbase + RRScreenChangeNotify)) {
//        // update Xlib internals with the latest screen configuration
//        X11->ptrXRRUpdateConfiguration(event);

//        // update the size for desktop widget
//        int scr = X11->ptrXRRRootToScreen(X11->display, event->xany.window);
//        QDesktopWidget *desktop = QApplication::desktop();
//        QWidget *w = desktop->screen(scr);
//        QSize oldSize(w->size());
//        w->data->crect.setWidth(DisplayWidth(X11->display, scr));
//        w->data->crect.setHeight(DisplayHeight(X11->display, scr));
//        QResizeEvent e(w->size(), oldSize);
//        QApplication::sendEvent(w, &e);
//        if (w != desktop)
//            QApplication::sendEvent(desktop, &e);
//    }
//#endif // QT_NO_XRANDR

//#ifndef QT_NO_XFIXES
//    if (X11->use_xfixes && event->type == (X11->xfixes_eventbase + XFixesSelectionNotify)) {
//        XFixesSelectionNotifyEvent *req = reinterpret_cast<XFixesSelectionNotifyEvent *>(event);

//        // compress all XFixes events related to this selection
//        // we don't want to handle old SelectionNotify events.
//        qt_xfixes_selection_event_data xfixes_event;
//        xfixes_event.selection = req->selection;
//        for (XEvent ev;;) {
//            if (!XCheckIfEvent(X11->display, &ev, &qt_xfixes_scanner, (XPointer)&xfixes_event))
//                break;
//        }

//        if (req->selection == ATOM(CLIPBOARD)) {
//            if (qt_xfixes_clipboard_changed(req->owner, req->selection_timestamp)) {
//                emit clipboard()->changed(QClipboard::Clipboard);
//                emit clipboard()->dataChanged();
//            }
//        } else if (req->selection == XA_PRIMARY) {
//            if (qt_xfixes_selection_changed(req->owner, req->selection_timestamp)) {
//                emit clipboard()->changed(QClipboard::Selection);
//                emit clipboard()->selectionChanged();
//            }
//        }
//    }
//#endif // QT_NO_XFIXES

    switch (event->type) {

//    case ButtonRelease:                        // mouse event
//        if (!d->inPopupMode() && !QWidget::mouseGrabber() && pressed_window != widget->internalWinId()
//            && (widget = (QETWidget*) QWidget::find((WId)pressed_window)) == 0)
//            break;
//        // fall through intended
//    case ButtonPress:
//        if (event->xbutton.root != RootWindow(X11->display, widget->x11Info().screen())
//            && ! qt_xdnd_dragging) {
//            while (activePopupWidget())
//                activePopupWidget()->close();
//            return 1;
//        }
//        if (event->type == ButtonPress)
//            qt_net_update_user_time(widget->window(), X11->userTime);
//        // fall through intended
//    case MotionNotify:
//#if !defined(QT_NO_TABLET)
//        if (!qt_tabletChokeMouse) {
//#endif
//            if (widget->testAttribute(Qt::WA_TransparentForMouseEvents)) {
//                QPoint pos(event->xbutton.x, event->xbutton.y);
//                pos = widget->d_func()->mapFromWS(pos);
//                QWidget *window = widget->window();
//                pos = widget->mapTo(window, pos);
//                if (QWidget *child = window->childAt(pos)) {
//                    widget = static_cast<QETWidget *>(child);
//                    pos = child->mapFrom(window, pos);
//                    event->xbutton.x = pos.x();
//                    event->xbutton.y = pos.y();
//                }
//            }
//            widget->translateMouseEvent(event);
//#if !defined(QT_NO_TABLET)
//        } else {
//            qt_tabletChokeMouse = false;
//        }
//#endif
//        break;

//    case XKeyPress:                                // keyboard event
//        qt_net_update_user_time(widget->window(), X11->userTime);
//        // fallthrough intended
//    case XKeyRelease:
//        {
//            if (keywidget && keywidget->isEnabled()) { // should always exist
//                // qDebug("sending key event");
//                qt_keymapper_private()->translateKeyEvent(keywidget, event, grabbed);
//            }
//            break;
//        }

//    case GraphicsExpose:
//    case Expose:                                // paint event
//        widget->translatePaintEvent(event);
//        break;

//    case ConfigureNotify:                        // window move/resize event
//        if (event->xconfigure.event == event->xconfigure.window)
//            widget->translateConfigEvent(event);
//        break;

//    case XFocusIn: {                                // got focus
//        if ((widget->windowType() == Qt::Desktop))
//            break;
//        if (d->inPopupMode()) // some delayed focus event to ignore
//            break;
//        if (!widget->isWindow())
//            break;
//        if (event->xfocus.detail != NotifyAncestor &&
//            event->xfocus.detail != NotifyInferior &&
//            event->xfocus.detail != NotifyNonlinear)
//            break;
//        setActiveWindow(widget);
//        if (X11->focus_model == QX11Data::FM_PointerRoot) {
//            // We got real input focus from somewhere, but we were in PointerRoot
//            // mode, so we don't trust this event.  Check the focus model to make
//            // sure we know what focus mode we are using...
//            qt_check_focus_model();
//        }
//    }
//        break;

    case XFocusOut:                                // lost focus
        if ((widget->windowType() == Qt::Desktop))
            break;
        if (!widget->isWindow())
            break;
        if (event->xfocus.mode == NotifyGrab) {
            qt_xfocusout_grab_counter++;
            break;
        }
        if (event->xfocus.detail != NotifyAncestor &&
            event->xfocus.detail != NotifyNonlinearVirtual &&
            event->xfocus.detail != NotifyNonlinear)
            break;
        if (!d->inPopupMode() && widget == QApplicationPrivate::active_window) {
            XEvent ev;
            bool focus_will_change = false;
            if (XCheckTypedEvent(X11->display, XFocusIn, &ev)) {
                // we're about to get an XFocusIn, if we know we will
                // get a new active window, we don't want to set the
                // active window to 0 now
                QWidget *w2 = QWidget::find(ev.xany.window);
                if (w2
                    && w2->windowType() != Qt::Desktop
                    && !d->inPopupMode() // some delayed focus event to ignore
                    && w2->isWindow()
                    && (ev.xfocus.detail == NotifyAncestor
                        || ev.xfocus.detail == NotifyInferior
                        || ev.xfocus.detail == NotifyNonlinear))
                    focus_will_change = true;

                XPutBackEvent(X11->display, &ev);
            }
            if (!focus_will_change)
                setActiveWindow(0);
        }
        break;

//    case EnterNotify: {                        // enter window
//        if (QWidget::mouseGrabber() && (!d->inPopupMode() || widget->window() != activePopupWidget()))
//            break;
//        if ((event->xcrossing.mode != NotifyNormal
//             && event->xcrossing.mode != NotifyUngrab)
//            || event->xcrossing.detail == NotifyVirtual
//            || event->xcrossing.detail == NotifyNonlinearVirtual)
//            break;
//        if (event->xcrossing.focus &&
//            !(widget->windowType() == Qt::Desktop) && !widget->isActiveWindow()) {
//            if (X11->focus_model == QX11Data::FM_Unknown) // check focus model
//                qt_check_focus_model();
//            if (X11->focus_model == QX11Data::FM_PointerRoot) // PointerRoot mode
//                setActiveWindow(widget);
//        }

//        if (qt_button_down && !d->inPopupMode())
//            break;

//        QWidget *alien = widget->childAt(widget->d_func()->mapFromWS(QPoint(event->xcrossing.x,
//                                                                            event->xcrossing.y)));
//        QWidget *enter = alien ? alien : widget;
//        QWidget *leave = 0;
//        if (qt_last_mouse_receiver && !qt_last_mouse_receiver->internalWinId())
//            leave = qt_last_mouse_receiver;
//        else
//            leave = QWidget::find(curWin);

//        // ### Alien: enter/leave might be wrong here with overlapping siblings
//        // if the enter widget is native and stacked under a non-native widget.
//        QApplicationPrivate::dispatchEnterLeave(enter, leave);
//        curWin = widget->internalWinId();
//        qt_last_mouse_receiver = enter;
//        if (!d->inPopupMode() || widget->window() == activePopupWidget())
//            widget->translateMouseEvent(event); //we don't get MotionNotify, emulate it
//    }
//        break;
    case LeaveNotify: {                        // leave window
        QWidget *mouseGrabber = QWidget::mouseGrabber();
        if (mouseGrabber && !d->inPopupMode())
            break;
        if (curWin && widget->internalWinId() != curWin)
            break;
        if ((event->xcrossing.mode != NotifyNormal
            && event->xcrossing.mode != NotifyUngrab)
            || event->xcrossing.detail == NotifyInferior)
            break;
        if (!(widget->windowType() == Qt::Desktop))
            widget->translateMouseEvent(event); //we don't get MotionNotify, emulate it

        QWidget* enter = 0;
        QPoint enterPoint;
        XEvent ev;
        while (XCheckMaskEvent(X11->display, EnterWindowMask | LeaveWindowMask , &ev)
               && !qt_x11EventFilter(&ev)) {
            QWidget* event_widget = QWidget::find(ev.xcrossing.window);
            if(event_widget && event_widget->x11Event(&ev))
                break;
            if (ev.type == LeaveNotify
                || (ev.xcrossing.mode != NotifyNormal
                    && ev.xcrossing.mode != NotifyUngrab)
                || ev.xcrossing.detail == NotifyVirtual
                || ev.xcrossing.detail == NotifyNonlinearVirtual)
                continue;
            enter = event_widget;
            if (enter)
                enterPoint = enter->d_func()->mapFromWS(QPoint(ev.xcrossing.x, ev.xcrossing.y));
            if (ev.xcrossing.focus &&
                enter && !(enter->windowType() == Qt::Desktop) && !enter->isActiveWindow()) {
                if (X11->focus_model == QX11Data::FM_Unknown) // check focus model
                    qt_check_focus_model();
                if (X11->focus_model == QX11Data::FM_PointerRoot) // PointerRoot mode
                    setActiveWindow(enter);
            }
            break;
        }

        if ((! enter || (enter->windowType() == Qt::Desktop)) &&
            event->xcrossing.focus && widget == QApplicationPrivate::active_window &&
            X11->focus_model == QX11Data::FM_PointerRoot // PointerRoot mode
            ) {
            setActiveWindow(0);
        }

        if (qt_button_down && !d->inPopupMode())
            break;

        if (!curWin)
            QApplicationPrivate::dispatchEnterLeave(widget, 0);

        if (enter) {
            QWidget *alienEnter = enter->childAt(enterPoint);
            if (alienEnter)
                enter = alienEnter;
        }

        QWidget *leave = qt_last_mouse_receiver ? qt_last_mouse_receiver : widget;
        QWidget *activePopupWidget = qApp->activePopupWidget();

        if (mouseGrabber && activePopupWidget && leave == activePopupWidget)
            enter = mouseGrabber;
        else if (enter != widget && mouseGrabber) {
            if (!widget->rect().contains(widget->d_func()->mapFromWS(QPoint(event->xcrossing.x,
                                                                            event->xcrossing.y))))
                break;
        }

        QApplicationPrivate::dispatchEnterLeave(enter, leave);
        qt_last_mouse_receiver = enter;

        if (enter && QApplicationPrivate::tryModalHelper(enter, 0)) {
            QWidget *nativeEnter = enter->internalWinId() ? enter : enter->nativeParentWidget();
            curWin = nativeEnter->internalWinId();
            static_cast<QETWidget *>(nativeEnter)->translateMouseEvent(&ev); //we don't get MotionNotify, emulate it
        } else {
            curWin = 0;
            qt_last_mouse_receiver = 0;
        }
    }
        break;

//    case UnmapNotify:                                // window hidden
//        if (widget->isWindow()) {
//            Q_ASSERT(widget->testAttribute(Qt::WA_WState_Created));
//            widget->d_func()->topData()->waitingForMapNotify = 0;

//            if (widget->windowType() != Qt::Popup && !widget->testAttribute(Qt::WA_DontShowOnScreen)) {
//                widget->setAttribute(Qt::WA_Mapped, false);
//                if (widget->isVisible()) {
//                    widget->d_func()->topData()->spont_unmapped = 1;
//                    QHideEvent e;
//                    QApplication::sendSpontaneousEvent(widget, &e);
//                    widget->d_func()->hideChildren(true);
//                }
//            }

//            if (!widget->d_func()->topData()->validWMState && X11->deferred_map.removeAll(widget))
//                widget->doDeferredMap();
//        }
//        break;

//    case MapNotify:                                // window shown
//        if (widget->isWindow()) {
//            // if we got a MapNotify when we were not waiting for it, it most
//            // likely means the user has already asked to hide the window before
//            // it ever being shown, so we try to withdraw a window after sending
//            // the QShowEvent.
//            bool pendingHide = widget->testAttribute(Qt::WA_WState_ExplicitShowHide) && widget->testAttribute(Qt::WA_WState_Hidden);
//            widget->d_func()->topData()->waitingForMapNotify = 0;

//            if (widget->windowType() != Qt::Popup) {
//                widget->setAttribute(Qt::WA_Mapped);
//                if (widget->d_func()->topData()->spont_unmapped) {
//                    widget->d_func()->topData()->spont_unmapped = 0;
//                    widget->d_func()->showChildren(true);
//                    QShowEvent e;
//                    QApplication::sendSpontaneousEvent(widget, &e);

//                    // show() must have been called on this widget in
//                    // order to reach this point, but we could have
//                    // cleared these 2 attributes in case something
//                    // previously forced us into WithdrawnState
//                    // (e.g. kdocker)
//                    widget->setAttribute(Qt::WA_WState_ExplicitShowHide, true);
//                    widget->setAttribute(Qt::WA_WState_Visible, true);
//                }
//            }
//            if (pendingHide) // hide the window
//                XWithdrawWindow(X11->display, widget->internalWinId(), widget->x11Info().screen());
//        }
//        break;

//    case ClientMessage:                        // client message
//        return x11ClientMessage(widget,event,False);

//    case ReparentNotify: {                      // window manager reparents
//        // compress old reparent events to self
//        XEvent ev;
//        while (XCheckTypedWindowEvent(X11->display,
//                                      widget->effectiveWinId(),
//                                      ReparentNotify,
//                                      &ev)) {
//            if (ev.xreparent.window != ev.xreparent.event) {
//                XPutBackEvent(X11->display, &ev);
//                break;
//            }
//        }
//        if (widget->isWindow()) {
//            QTLWExtra *topData = widget->d_func()->topData();

//            // store the parent. Useful for many things, embedding for instance.
//            topData->parentWinId = event->xreparent.parent;

//            // the widget frame strut should also be invalidated
//            widget->data->fstrut_dirty = 1;

//            // work around broken window managers... if we get a
//            // ReparentNotify before the MapNotify, we assume that
//            // we're being managed by a reparenting window
//            // manager.
//            //
//            // however, the WM_STATE property may not have been set
//            // yet, but we are going to assume that it will
//            // be... otherwise we could try to map again after getting
//            // an UnmapNotify... which could then, in turn, trigger a
//            // race in the window manager which causes the window to
//            // disappear when it really should be hidden.
//            if (topData->waitingForMapNotify && !topData->validWMState) {
//                topData->waitingForMapNotify = 0;
//                topData->validWMState = 1;
//            }

//            if (X11->focus_model != QX11Data::FM_Unknown) {
//                // toplevel reparented...
//                QWidget *newparent = QWidget::find(event->xreparent.parent);
//                if (! newparent || (newparent->windowType() == Qt::Desktop)) {
//                    // we don't know about the new parent (or we've been
//                    // reparented to root), perhaps a window manager
//                    // has been (re)started?  reset the focus model to unknown
//                    X11->focus_model = QX11Data::FM_Unknown;
//                }
//            }
//        }
//        break;
//    }
//    case SelectionRequest: {
//        XSelectionRequestEvent *req = &event->xselectionrequest;
//        if (! req)
//            break;

//        if (ATOM(XdndSelection) && req->selection == ATOM(XdndSelection)) {
//            X11->xdndHandleSelectionRequest(req);

//        } else if (qt_clipboard) {
//            QClipboardEvent e(reinterpret_cast<QEventPrivate*>(event));
//            QApplication::sendSpontaneousEvent(qt_clipboard, &e);
//        }
//        break;
//    }
//    case SelectionClear: {
//        XSelectionClearEvent *req = &event->xselectionclear;
//        // don't deliver dnd events to the clipboard, it gets confused
//        if (! req || (ATOM(XdndSelection) && req->selection == ATOM(XdndSelection)))
//            break;

//        if (qt_clipboard && !X11->use_xfixes) {
//            QClipboardEvent e(reinterpret_cast<QEventPrivate*>(event));
//            QApplication::sendSpontaneousEvent(qt_clipboard, &e);
//        }
//        break;
//    }

//    case SelectionNotify: {
//        XSelectionEvent *req = &event->xselection;
//        // don't deliver dnd events to the clipboard, it gets confused
//        if (! req || (ATOM(XdndSelection) && req->selection == ATOM(XdndSelection)))
//            break;

//        if (qt_clipboard) {
//            QClipboardEvent e(reinterpret_cast<QEventPrivate*>(event));
//            QApplication::sendSpontaneousEvent(qt_clipboard, &e);
//        }
//        break;
//    }
//    case PropertyNotify:
//        // some properties changed
//        if (event->xproperty.window == QX11Info::appRootWindow(0)) {
//            // root properties for the first screen
//            if (!X11->use_xfixes && event->xproperty.atom == ATOM(_QT_CLIPBOARD_SENTINEL)) {
//                if (qt_check_clipboard_sentinel()) {
//                    emit clipboard()->changed(QClipboard::Clipboard);
//                    emit clipboard()->dataChanged();
//                }
//            } else if (!X11->use_xfixes && event->xproperty.atom == ATOM(_QT_SELECTION_SENTINEL)) {
//                if (qt_check_selection_sentinel()) {
//                    emit clipboard()->changed(QClipboard::Selection);
//                    emit clipboard()->selectionChanged();
//                }
//            } else if (QApplicationPrivate::obey_desktop_settings) {
//                if (event->xproperty.atom == ATOM(RESOURCE_MANAGER))
//                    qt_set_x11_resources();
//                else if (event->xproperty.atom == ATOM(_QT_SETTINGS_TIMESTAMP))
//                    qt_set_x11_resources();
//            }
//        }
//        if (event->xproperty.window == QX11Info::appRootWindow()) {
//            // root properties for the default screen
//            if (event->xproperty.atom == ATOM(_QT_INPUT_ENCODING)) {
//                qt_set_input_encoding();
//            } else if (event->xproperty.atom == ATOM(_NET_SUPPORTED)) {
//                qt_get_net_supported();
//            } else if (event->xproperty.atom == ATOM(_NET_VIRTUAL_ROOTS)) {
//                qt_get_net_virtual_roots();
//            } else if (event->xproperty.atom == ATOM(_NET_WORKAREA)) {
//                qt_desktopwidget_update_workarea();

//                // emit the workAreaResized() signal
//                QDesktopWidget *desktop = QApplication::desktop();
//                int numScreens = desktop->numScreens();
//                for (int i = 0; i < numScreens; ++i)
//                    emit desktop->workAreaResized(i);
//            }
//        } else if (widget) {
//            widget->translatePropertyEvent(event);
//        }  else {
//            return -1; // don't know this window
//        }
//        break;

    default:
        break;
    }

    return 0;
}

QT_END_NAMESPACE
