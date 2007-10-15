PROJECT (ManyEars)

#Look for minimum cmake version
cmake_minimum_required(VERSION 2.4.0)

SET(CMAKE_BUILD_TYPE Release )
SET(CMAKE_VERBOSE_MAKEFILE ON)

#Let's find FlowDesigner
#Hard coded paths right now...

SET (FLOWDESIGNER_BIN_DIR "/usr/local/flowdesigner/bin/")
SET (FLOWDESIGNER_INCLUDE_DIR "/usr/local/flowdesigner/include/flowdesigner/")
SET (FLOWDESIGNER_LIB_DIR "/usr/local/flowdesigner/lib/")
SET (FLOWDESIGNER_TOOLBOX_DIR "/usr/local/flowdesigner/lib/flowdesigner/toolbox/")

#Add the macros directory for cmake scripts
SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/macros)

#Look for dependencies
find_package(FFTW3 REQUIRED)

#finding qt
find_package(Qt4 REQUIRED)

#STANDARD PERL MODULE
find_package(Perl REQUIRED)

#set the prefix
SET(CMAKE_INSTALL_PREFIX "${FLOWDESIGNER_TOOLBOX_DIR}/ManyEars")

#Adding subdirectories
ADD_SUBDIRECTORY(src)

IF (HAVE_QT4)
	ADD_SUBDIRECTORY(GUI)
ENDIF(HAVE_QT4)

#WIN32 DLL INSTALLATION
IF(WIN32)

	SET(FFTW3_DLL
 		C:/Dev-Cpp/bin/fftw3.dll
	)
	INSTALL(FILES ${FFTW3_DLL} DESTINATION bin)

ENDIF (WIN32)