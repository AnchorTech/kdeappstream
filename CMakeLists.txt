PROJECT( kde-appstream )

CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )

FIND_PACKAGE( KDE4 REQUIRED )
INCLUDE( KDE4Defaults )

SET( CMAKE_PACKAGE_QTGUI TRUE )
SET( QT_USE_QTXML TRUE )

SET( KAS_COMMON_SRCS
	./src/main.cpp
	./src/TestWindow.cpp
	./src/paint/PaintDevice.cpp
	./src/paint/PaintEngine.cpp
   )

KDE4_ADD_UI_FILES( KAS_COMMON_SRCS ./src/TestWindow.ui )

SET( INCLUDE_DIRS
	./src
	./src/paint
   )

INCLUDE_DIRECTORIES( ${QT_INCLUDE}
                     ${KDE4_INCLUDES}
		     ${INCLUDE_DIRS} )

ADD_DEFINITIONS( -Wall )

KDE4_ADD_EXECUTABLE( kde-appstream ${KAS_COMMON_SRCS} )

TARGET_LINK_LIBRARIES( kde-appstream
	               ${QT_LIBRARIES}
		       ${KDE4_KDEUI_LIBS}
		       ${KDE4_KPARTS_LIBS} )

INSTALL( TARGETS kde-appstream ${INSTALL_TARGETS_DEFAULT_ARGS} )
