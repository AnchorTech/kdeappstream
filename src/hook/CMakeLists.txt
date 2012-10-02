SET( KAPPSTREAM_HOOK_SRCS
     ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qwidget.cpp
   )

SET( INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel )

INCLUDE_DIRECTORIES( ${QT_INCLUDE}
                     ${KDE4_INCLUDES}
                     ${INCLUDE_DIRS} )

KDE4_ADD_LIBRARY( kappstreamhook SHARED ${KAPPSTREAM_HOOK_SRCS} )

TARGET_LINK_LIBRARIES( kappstreamhook
                       ${QT_QTCORE_LIBRARY}
                       ${DL_LIB} )

INSTALL( TARGETS kappstreamhook ${INSTALL_TARGETS_DEFAULT_ARGS} )
