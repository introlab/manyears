
# the next line sets up include and link directories and defines some variables that we will use.
# you can modify the behavior by setting some variables, e.g.
#   set(QT_USE_OPENGL TRUE)
# -> this will cause cmake to include and link against the OpenGL module

#finding qt

SET(QT_USE_QTNETWORK TRUE)
SET(QT_USE_QTWEBKIT TRUE)
SET(QT_USE_PHONON TRUE)


find_package(Qt4 REQUIRED)

IF (QT_PHONON_FOUND)
 ADD_DEFINITIONS(-D_HAVE_PHONON_)
ENDIF(QT_PHONON_FOUND)

#REQUIRED
include(${QT_USE_FILE})

#Sources for the AudioViewer application
SET(AUDIOVIEWER_SRCS
   main.cpp
   FDReader.cpp
)

SET(AUDIOVIEWER_HEADERS 
   FDReader.h
)

#Shared sources between AudioViewer application and Probes used in FlowDesigner
SET(AUDIOVIEWERSHARED_SRCS 
   audioview.cpp
   TrackAudioWidget.cpp
   ImageView.cpp
   SNCRZ30Cam.cpp
)

SET(AUDIOVIEWERSHARED_HEADERS 
   audioview.h
   TrackAudioWidget.h 
   ImageView.h
   SNCRZ30Cam.h
)

#Sources for Probes used in FlowDesigner
SET(AUDIOVIEWERPROBES_SRCS 
   QtProbeAudioViewer.cpp
)

SET(AUDIOVIEWERPROBES_HEADERS 
   QtProbeAudioViewer.h
)

#This will generate moc_* for Qt
QT4_WRAP_CPP(AUDIOVIEWER_MOC_SRCS ${AUDIOVIEWER_HEADERS})
#This will generate moc_* for Qt
QT4_WRAP_CPP(AUDIOVIEWERSHARED_MOC_SRCS ${AUDIOVIEWERSHARED_HEADERS})
#This will generate moc_* for Qt
QT4_WRAP_CPP(AUDIOVIEWERPROBES_MOC_SRCS ${AUDIOVIEWERPROBES_HEADERS})

#Qtflow includes
SET (FLOWDESIGNER_QTFLOW_INCLUDE_DIR "${FLOWDESIGNER_INCLUDE_DIR}/qtflow/")

#include files
INCLUDE_DIRECTORIES(./ ../src ${FLOWDESIGNER_INCLUDE_DIR} ${QT_INCLUDE_DIR} ${LIBXML2_INCLUDE_DIR} ${FLOWDESIGNER_QTFLOW_INCLUDE_DIR} )

# create an executable file named "AUDIOVIEWER" from the source files 
add_executable(audioviewer ${AUDIOVIEWER_SRCS} ${AUDIOVIEWER_MOC_SRCS} ${AUDIOVIEWERSHARED_SRCS} ${AUDIOVIEWERSHARED_MOC_SRCS})

# Linking with libflow and Qt libraries

target_link_libraries(audioviewer  ${QT_LIBRARIES} -L${FLOWDESIGNER_LIB_DIR} -lflow ${LIBXML2_LIBRARIES} libmanyears-dynamic)

#install application
install(TARGETS audioviewer DESTINATION bin)

#Create a library, used for probes
ADD_DEFINITIONS(-DINSTALL_PREFIX=\"${FD_PREFIX}\")
add_library(libmanyears-probe-dynamic SHARED ${AUDIOVIEWERSHARED_SRCS} ${AUDIOVIEWERSHARED_MOC_SRCS} ${AUDIOVIEWERPROBES_SRCS} ${AUDIOVIEWERPROBES_MOC_SRCS})

set_target_properties(libmanyears-probe-dynamic PROPERTIES OUTPUT_NAME manyears.probe 

)

IF (APPLE)
	set_target_properties(libmanyears-probe-dynamic PROPERTIES
	BUILD_WITH_INSTALL_RPATH ON
	INSTALL_NAME_DIR ${FLOWDESIGNER_TOOLBOX_DIR}/ManyEars/lib)
ENDIF(APPLE)


target_link_libraries(libmanyears-probe-dynamic ${QT_LIBRARIES} -L${FLOWDESIGNER_LIB_DIR} -lflow -lqtflow ${LIBXML2_LIBRARIES} libmanyears-dynamic)
install(TARGETS libmanyears-probe-dynamic DESTINATION ${FLOWDESIGNER_TOOLBOX_DIR}/ManyEars/lib)
