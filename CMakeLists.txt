PROJECT( kde-appstream )

CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )

FIND_PACKAGE( KDE4 REQUIRED )
FIND_LIBRARY( DL_LIB NAMES ${CMAKE_DL_LIBS} )
MESSAGE( STATUS "using dl library: ${DL_LIB}" )

INCLUDE( KDE4Defaults )

SET( CMAKE_PACKAGE_QTGUI TRUE )
SET( QT_USE_QTXML TRUE )

SET( KAS_COMMON_SRCS
	./src/websocket/HttpServer.cpp
	./src/websocket/QWsServer.cpp
	./src/websocket/QWsSocket.cpp
        ./src/main.cpp
   )

SET( KASLIB_COMMON_SRCS
        ./src/hooking/AbstractFunctionOverwriter.cpp
        ./src/hooking/FunctionOverwriterFactory.cpp
        ./src/hooking/Probe.cpp
        ./src/hooking/UnixFunctionOverwriter.cpp
        ./src/events/EventDispather.cpp
        ./src/events/EventFilter.cpp
        ./src/paint/JSONBuilder.cpp
        ./src/paint/PaintDevice.cpp
        ./src/paint/PaintEngine.cpp
        ./src/paint/WebRenderer.cpp
	./src/websocket/QWsServer.cpp
	./src/websocket/QWsSocket.cpp
	./src/websocket/WebsocketServer.cpp
	./src/websocket/WebsocketThread.cpp
   )

SET( TEST_COMMON_SRCS
        ./src/test/main.cpp
        ./src/test/TestWindow.cpp
   )

SET( INCLUDE_DIRS
        ./src
        ./src/paint
        ./src/test
   )

SET(data_dir ${DATA_INSTALL_DIR}/kdeappstream/data/)
INSTALL( FILES "./src/websocket/index.html" DESTINATION ${data_dir} )

INCLUDE_DIRECTORIES( ${QT_INCLUDE}
                     ${KDE4_INCLUDES}
                     ${INCLUDE_DIRS} )

KDE4_ADD_UI_FILES( TEST_COMMON_SRCS ./src/test/TestWindow.ui )

ADD_DEFINITIONS( -Wall )
ADD_DEFINITIONS(-DQT_SHARED )
ADD_DEFINITIONS(-DQT_TEST )

KDE4_ADD_EXECUTABLE( testapp ${TEST_COMMON_SRCS} )
KDE4_ADD_EXECUTABLE( kappstream-server ${KAS_COMMON_SRCS} )
KDE4_ADD_LIBRARY( kappstream SHARED ${KASLIB_COMMON_SRCS} )

TARGET_LINK_LIBRARIES( kappstream
                       ${QT_QTCORE_LIBRARY}
                       ${QT_QTGUI_LIBRARY}
                       ${QT_QTNETWORK_LIBRARY}
                       ${QT_QTTEST_LIBRARY}
                       ${QT_QTSCRIPT_LIBRARY}
                       ${DL_LIB} )

TARGET_LINK_LIBRARIES( kappstream-server
                       ${QT_QTCORE_LIBRARY}
                       ${QT_QTGUI_LIBRARY}
                       ${KDE4_KDEUI_LIBS}
                       ${KDE4_KPARTS_LIBS} )

TARGET_LINK_LIBRARIES( testapp
                       ${QT_QTCORE_LIBRARY}
                       ${QT_QTGUI_LIBRARY}
                       ${KDE4_KDEUI_LIBS}
                       ${KDE4_KPARTS_LIBS} kappstream )

INSTALL( TARGETS testapp ${INSTALL_TARGETS_DEFAULT_ARGS} )
INSTALL( TARGETS kappstream ${INSTALL_TARGETS_DEFAULT_ARGS} )
INSTALL( TARGETS kappstream-server ${INSTALL_TARGETS_DEFAULT_ARGS} )
