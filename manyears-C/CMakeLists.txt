PROJECT (INTROLAB-ManyEars)

cmake_minimum_required(VERSION 2.6)

########################
# Configurations
########################
SET(CPACK_PACKAGE_VERSION_MAJOR "1")
SET(CPACK_PACKAGE_VERSION_MINOR "1")
SET(CPACK_PACKAGE_VERSION_PATCH "4-2")
SET(CPACK_PACKAGE_VERSION ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH})


#OSX Architecture, will build for 32 bits systems
#set(CMAKE_OSX_ARCHITECTURES "i386")

#SET THE BUILD TYPE HERE, Release, Debug
SET(CMAKE_BUILD_TYPE Release)


#Additional modules
SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/macros)

#Here we are assuming SSE3 support
IF (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX )
    MESSAGE( STATUS "Setting flags for GNU GCC")
    ADD_DEFINITIONS(-DUSE_SIMD)
    set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-msse3 -Wall")
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-msse3 -Wall")
    SET(MANYEARS_LINK_FLAGS "-O3")
ENDIF(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)

IF (MSVC)
    MESSAGE( STATUS "Setting flags for Visual Studio")
    ADD_DEFINITIONS(-DUSE_SIMD -D__SSE__ -D__SSE2__ -D_CRT_SECURE_NO_WARNINGS)
    SET(MANYEARS_LINK_FLAGS " ")
    set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "/arch:SSE2")
ENDIF(MSVC)


IF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
	IF(WIN32)
		MESSAGE( STATUS "Windows version: " ${CMAKE_SYSTEM})
			IF(MSVC)
				SET(CMAKE_INSTALL_PREFIX "$ENV{SYSTEMDRIVE}/ManyEars-win32-VS-${CPACK_PACKAGE_VERSION}")
			ELSE(MSVC)
				SET(CMAKE_INSTALL_PREFIX "$ENV{SYSTEMDRIVE}/ManyEars-win32-mingw-${CPACK_PACKAGE_VERSION}")
			ENDIF(MSVC)
		MESSAGE( STATUS "Install directory is : " ${CMAKE_INSTALL_PREFIX})
	ELSE(WIN32)
			SET(CMAKE_INSTALL_PREFIX "/usr/local/manyears")
	ENDIF(WIN32)
ENDIF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

# use, i.e. don't skip the full RPATH for the build tree
SET(CMAKE_SKIP_BUILD_RPATH  FALSE)

# when building, don't use the install RPATH already
# (but later on when installing)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)

# the RPATH to be used when installing
SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

#Win32
IF (WIN32)
    IF (NOT MSVC)
        SET(MANYEARS_LINK_FLAGS "--enable-auto-import")
    ENDIF(NOT MSVC)

    SET (QT_USE_QTMAIN TRUE)
ENDIF (WIN32)



INCLUDE(InstallRequiredSystemLibraries)

#This will enable to output everything in the bin directory for testing and debugging...
SET (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET (LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# the next line sets up include and link directories and defines some variables that we will use.
# you can modify the behavior by setting some variables, e.g.
#   
# -> this will cause cmake to include and link against the OpenGL module
IF (NOT MANYEARS_GUI_DISABLED)

    #Find Qt4
    find_package(Qt4 4.8.0 QUIET)

    IF(QT4_FOUND)
        MESSAGE(STATUS "Qt4 found. ManyEars GUI will be compiled.")
    ELSE(QT4_FOUND)
        MESSAGE(STATUS "Qt4 Not found. ManyEars GUI disabled.")
        SET(MANYEARS_GUI_DISABLED TRUE)
    ENDIF(QT4_FOUND)

ELSE (NOT MANYEARS_GUI_DISABLED)
    MESSAGE(STATUS "ManyEars GUI disabled.")
ENDIF (NOT MANYEARS_GUI_DISABLED)

#Include package file, contains useful defintions
include(package/CMakeLists.txt)


##########################
# Subdirectories
##########################
#add_subdirectory(package)
ADD_SUBDIRECTORY(dsplib)
ADD_SUBDIRECTORY(RTAudio)
ADD_SUBDIRECTORY(example)
ADD_SUBDIRECTORY(demo)
IF(NOT MANYEARS_GUI_DISABLED)
    ADD_SUBDIRECTORY(Qt4GUI)
ENDIF(NOT MANYEARS_GUI_DISABLED)



