
# the next line sets up include and link directories and defines some variables that we will use.
# you can modify the behavior by setting some variables, e.g.
#   set(QT_USE_OPENGL TRUE)
# -> this will cause cmake to include and link against the OpenGL module

#finding qt

SET(QT_USE_QTNETWORK TRUE)
SET(QT_USE_QTWEBKIT TRUE)


find_package(Qt4 REQUIRED)


#REQUIRED
include(${QT_USE_FILE})

#Sources for the BRIDGE application
SET(BRIDGE_SRCS
	qextserialbase.cpp       
	qextserialport.cpp
	qextserialenumerator.cpp 
)

SET(BRIDGE_HEADERS 
	qextserialenumerator.h 
	qextserialbase.h       
	qextserialport.h
)


#POSIX/UNIX SPECIFIC FILES & DEFINES
IF (UNIX)
    ADD_DEFINITIONS(-D_TTY_POSIX_)
	SET(BRIDGE_SRCS ${BRIDGE_SRCS} posix_qextserialport.cpp)
	SET(BRIDGE_HEADERS ${BRIDGE_HEADERS} posix_qextserialport.h)
ENDIF(UNIX)

#WINDOWS SPECIFIC FILES	& DEFINES
IF (WIN32)
	SET(BRIDGE_SRCS ${BRIDGE_SRCS} 	win_qextserialport.cpp)
	SET(BRIDGE_HEADERS ${BRIDGE_HEADERS} win_qextserialport.h)
ENDIF(WIN32)



#This will generate moc_* for Qt
QT4_WRAP_CPP(BRIDGE_MOC_SRCS ${BRIDGE_HEADERS})


#include files
INCLUDE_DIRECTORIES(./ ${QT_INCLUDE_DIR})

# create an executable file named "BRIDGE" from the source files 
add_executable(cameraBridge ${BRIDGE_SRCS} ${BRIDGE_MOC_SRCS} main.cpp)

# Linking with libflow and Qt libraries
target_link_libraries(cameraBridge  ${QT_LIBRARIES})

#install application
install(TARGETS cameraBridge DESTINATION bin)
