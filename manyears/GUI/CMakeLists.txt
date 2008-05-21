
# the next line sets up include and link directories and defines some variables that we will use.
# you can modify the behavior by setting some variables, e.g.
#   set(QT_USE_OPENGL TRUE)
# -> this will cause cmake to include and link against the OpenGL module

#finding qt

SET(QT_USE_QTNETWORK TRUE)

find_package(Qt4 REQUIRED)


#REQUIRED
include(${QT_USE_FILE})

SET(AUDIOVIEWER_SRCS 
   audioview.cpp
   TrackAudioWidget.cpp
   ImageView.cpp 
   FDReader.cpp 
   SourceInfo.cpp 
)

SET(AUDIOVIEWER_HEADERS 
   audioview.h
   TrackAudioWidget.h 
   ImageView.h
   FDReader.h
)

#This will generate moc_* for Qt
QT4_WRAP_CPP(AUDIOVIEWER_MOC_SRCS ${AUDIOVIEWER_HEADERS})

#include files
INCLUDE_DIRECTORIES(./ ${FLOWDESIGNER_INCLUDE_DIR} ${QT_INCLUDE_DIR} ${LIBXML2_INCLUDE_DIR}  )

# create an executable file named "AUDIOVIEWER" from the source files 
add_executable(audioviewer ${AUDIOVIEWER_SRCS} main.cpp ${AUDIOVIEWER_MOC_SRCS})

# Linking with libflow and Qt libraries
target_link_libraries(audioviewer  ${QT_QTCORE_LIBRARY} ${QT_QTNETWORK_LIBRARY} ${QT_QTGUI_LIBRARY} -L${FLOWDESIGNER_LIB_DIR} -lflow ${LIBXML2_LIBRARIES})

#install application
install(TARGETS audioviewer DESTINATION bin)
