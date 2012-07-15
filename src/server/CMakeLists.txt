SET( KAPPSTREAM_SERVER_SRCS 
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/HttpService.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/HttpServer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ApplicationWrapperThread.cpp )

SET( KAPPSTREAM_SERVER_HEADERS 
    ${CMAKE_CURRENT_SOURCE_DIR}/HttpService.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/HttpServer.h  
    ${CMAKE_CURRENT_SOURCE_DIR}/ApplicationWrapperThread.h )

SET( INCLUDE_DIRS
    ${COMMON_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR} )

SET( KAPPSTREAM_SERVER_DATA_DIR
    ${DATA_INSTALL_DIR}/kdeappstream/data/ )

INCLUDE_DIRECTORIES( ${QT_INCLUDE}
                     ${KDE4_INCLUDES}
                     ${INCLUDE_DIRS} )

KDE4_ADD_EXECUTABLE( kappstream-server ${KAPPSTREAM_SERVER_SRCS} )

TARGET_LINK_LIBRARIES( kappstream-server
                       ${QT_QTCORE_LIBRARY}
                       ${QT_QTGUI_LIBRARY}
                       ${KDE4_KDEUI_LIBS}
                       ${KDE4_KPARTS_LIBS}
                       )#kappstream-database )

INSTALL( FILES ${CMAKE_CURRENT_SOURCE_DIR}/index.html DESTINATION ${KAPPSTREAM_SERVER_DATA_DIR} )
INSTALL( FILES ${CMAKE_CURRENT_SOURCE_DIR}/canvas.html DESTINATION ${KAPPSTREAM_SERVER_DATA_DIR} )
INSTALL( TARGETS kappstream-server ${INSTALL_TARGETS_DEFAULT_ARGS} )
