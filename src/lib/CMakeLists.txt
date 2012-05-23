SET( KAPPSTREAM_LIB_SRCS
     ${CMAKE_CURRENT_SOURCE_DIR}/hooking/AbstractFunctionOverwriter.cpp
     ${CMAKE_CURRENT_SOURCE_DIR}/hooking/FunctionOverwriterFactory.cpp
     ${CMAKE_CURRENT_SOURCE_DIR}/hooking/Probe.cpp
     ${CMAKE_CURRENT_SOURCE_DIR}/hooking/UnixFunctionOverwriter.cpp
     ${CMAKE_CURRENT_SOURCE_DIR}/events/EventDispather.cpp
     ${CMAKE_CURRENT_SOURCE_DIR}/events/EventFilter.cpp
     ${CMAKE_CURRENT_SOURCE_DIR}/paint/JSONBuilder.cpp
     ${CMAKE_CURRENT_SOURCE_DIR}/paint/PaintDevice.cpp
     ${CMAKE_CURRENT_SOURCE_DIR}/paint/PaintEngine.cpp
     ${CMAKE_CURRENT_SOURCE_DIR}/paint/WebRenderer.cpp
     ${CMAKE_CURRENT_SOURCE_DIR}/websocket/QWsServer.cpp
     ${CMAKE_CURRENT_SOURCE_DIR}/websocket/QWsSocket.cpp
     ${CMAKE_CURRENT_SOURCE_DIR}/websocket/WebsocketServer.cpp
     ${CMAKE_CURRENT_SOURCE_DIR}/websocket/WebsocketThread.cpp
   )

SET( INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/event
    ${CMAKE_CURRENT_SOURCE_DIR}/hooking
    ${CMAKE_CURRENT_SOURCE_DIR}/paint
    ${CMAKE_CURRENT_SOURCE_DIR}/websocket )

INCLUDE_DIRECTORIES( ${QT_INCLUDE}
                     ${KDE4_INCLUDES}
                     ${INCLUDE_DIRS} )

KDE4_ADD_LIBRARY( kappstream SHARED ${KAPPSTREAM_LIB_SRCS} )

TARGET_LINK_LIBRARIES( kappstream
                       ${QT_QTCORE_LIBRARY}
                       ${QT_QTGUI_LIBRARY}
                       ${QT_QTNETWORK_LIBRARY}
                       ${QT_QTTEST_LIBRARY}
                       ${QT_QTSCRIPT_LIBRARY}
                       ${DL_LIB} )

INSTALL( TARGETS kappstream ${INSTALL_TARGETS_DEFAULT_ARGS} )
