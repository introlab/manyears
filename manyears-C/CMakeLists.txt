PROJECT (INTROLAB-ManyEars)

cmake_minimum_required(VERSION 2.6)

#SSE COMPILATION
set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-msse3 -ftree-vectorize")
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-msse3 -ftree-vectorize")
#set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-msse3")
#set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-msse3")



#OSX Architecture, will build for 32 bits systems
set(CMAKE_OSX_ARCHITECTURES "i386")



########################
# Configurations
########################
SET(CPACK_PACKAGE_VERSION_MAJOR "1")
SET(CPACK_PACKAGE_VERSION_MINOR "0")
SET(CPACK_PACKAGE_VERSION_PATCH "0")
SET(CPACK_PACKAGE_VERSION ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH})

#SET THE BUILD TYPE HERE, Release, Debug
SET(CMAKE_BUILD_TYPE Release)

IF(WIN32)
	MESSAGE( STATUS "Windows version: " ${CMAKE_SYSTEM})
	SET(CMAKE_INSTALL_PREFIX "$ENV{SYSTEMDRIVE}/ManyEars-${CMAKE_BUILD_TYPE}")
	MESSAGE( STATUS "Install directory is : " ${CMAKE_INSTALL_PREFIX})
ELSE(WIN32)
        SET(CMAKE_INSTALL_PREFIX "/usr/local/manyears")
ENDIF(WIN32)


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




#finding qt
IF (WIN32)
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

SET(QT_USE_QTNETWORK TRUE)
SET(QT_USE_QTSVG TRUE)
SET(QT_USE_QTXML TRUE)
SET(QT_USE_QTSCRIPT TRUE)
SET(QT_USE_QTOPENGL TRUE)

find_package(Qt4 REQUIRED)


include(${QT_USE_FILE})


#Files excluded from package
set(CPACK_SOURCE_IGNORE_FILES
  "build"
  "bin"
  ${CPACK_SOURCE_IGNORE_FILES}
)



#include files
IF (WIN32)

    SET (QT_DLL
	    "${QT_BINARY_DIR}/Qt3Support4.dll"
	    "${QT_BINARY_DIR}/QtAssistantClient4.dll"
	    "${QT_BINARY_DIR}/QtCore4.dll"
	    "${QT_BINARY_DIR}/QtDesigner4.dll"
	    "${QT_BINARY_DIR}/QtDesignerComponents4.dll"
	    "${QT_BINARY_DIR}/QtGui4.dll"
	    "${QT_BINARY_DIR}/QtHelp4.dll"
	    "${QT_BINARY_DIR}/QtNetwork4.dll"
	    "${QT_BINARY_DIR}/QtOpenGL4.dll"
	    "${QT_BINARY_DIR}/QtScript4.dll"
	    "${QT_BINARY_DIR}/QtSql4.dll"
	    "${QT_BINARY_DIR}/QtSvg4.dll"
	    "${QT_BINARY_DIR}/QtTest4.dll"
	    "${QT_BINARY_DIR}/QtWebKit4.dll"
	    "${QT_BINARY_DIR}/QtXml4.dll"
	    "${QT_BINARY_DIR}/mingwm10.dll"
        "${QT_BINARY_DIR}/libgcc_s_dw2-1.dll"
    )

    #############################
    # Packaging stuff for Windows
    #############################
    SET(CPACK_SOURCE_GENERATOR "ZIP")
    SET(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/COPYING.TXT")
    SET(CPACK_PACKAGE_NAME "ManyEars")
    SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "ManyEars")
    SET(CPACK_PACKAGE_VENDOR "Francois Grondin, Dominic Letourneau")
    SET(CPACK_PACKAGE_DESCRIPTION_FILE "${PROJECT_SOURCE_DIR}/README.TXT")
    SET(CPACK_PACKAGE_CONTACT "francois.grondin@usherbrooke.ca")
    SET(CPACK_PACKAGE_INSTALL_DIRECTORY "ManyEars")
    SET(CPACK_NSIS_URL_INFO_ABOUT "http://introlab.gel.usherbrooke.ca")
    SET(CPACK_NSIS_CONTACT "dominic.letourneau@usherbrooke.ca")
    SET(CPACK_NSIS_MODIFY_PATH OFF)
    SET(CPACK_PACKAGE_EXECUTABLES "ManyEars" "ManyEars")

    INCLUDE(CPack)
ENDIF (WIN32)

IF (UNIX)

	    #########################################
	    # Packaging stuff for sources (All UNIX)
	    #########################################
	    SET(CPACK_SOURCE_GENERATOR "TGZ")
            SET(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/COPYING.TXT")
	    SET(CPACK_PACKAGE_NAME "ManyEars")
	    SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "ManyEars")
	    SET(CPACK_PACKAGE_VENDOR "Francois Grondin, Dominic Letourneau")
            SET(CPACK_PACKAGE_DESCRIPTION_FILE "${PROJECT_SOURCE_DIR}/README.TXT")
	    SET(CPACK_PACKAGE_CONTACT "francois.grondin@usherbrooke.ca")


	IF(APPLE)
	
            ##########################
	    # Packaging stuff for OSX
	    ##########################
            SET(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/COPYING.TXT")
            #SET(CPACK_GENERATOR Bundle)
            SET(CPACK_GENERATOR DragNDrop)

            #SET(CPACK_PACKAGE_INSTALL_DIRECTORY "/Applications")
            #SET(MACOSX_BUNDLE_INFO_STRING "ManyEars")
            #SET(CPACK_PACKAGE_FILE_NAME "ManyEars-${CPACK_PACKAGE_VERSION}")
            #SET(CPACK_BUNDLE_NAME "ManyEars")
            #SET(CPACK_PACKAGE_DEFAULT_LOCATION "/Applications")
            #SET(CPACK_BUNDLE_PLIST "ManyEars.plist")
            #SET(CPACK_BUNDLE_ICON "ManyEars.icns")
            #SET(CPACK_PACKAGE_ICON "ManyEars.icns")
            #SET(CPACK_BUNDLE_STARTUP_COMMAND "startup.sh")

            #BUNDLE INFO
            SET(MACOSX_BUNDLE_INFO_STRING "ManyEars")
            SET(MACOSX_BUNDLE_ICON_FILE "ManyEars.icns")
            SET(MACOSX_BUNDLE_GUI_IDENTIFIER "ManyEars")
            SET(MACOSX_BUNDLE_LONG_VERSION_STRING "ManyEars-${CPACK_PACKAGE_VERSION}")
            SET(MACOSX_BUNDLE_BUNDLE_NAME "ManyEars.app")
            SET(MACOSX_BUNDLE_SHORT_VERSION_STRING "${CPACK_PACKAGE_VERSION}")
            SET(MACOSX_BUNDLE_BUNDLE_VERSION "${CPACK_PACKAGE_VERSION}")
            SET(MACOSX_BUNDLE_COPYRIGHT "COPYING.TXT")            
            SET(MACOSX_BUNDLE_INFO_PLIST "Info.plist")

        ELSE(APPLE)
            ##########################
	    # Packaging stuff for Linux
	    ##########################
	
	    SET(CPACK_GENERATOR DEB)
	    SET(CPACK_DEBIAN_PACKAGE_NAME "Manyears")
	    SET(CPACK_DEBIAN_PACKAGE_VERSION ${CPACK_PACKAGE_VERSION})
	    SET(CPACK_DEBIAN_ARCHITECTURE i686)
	    SET(CPACK_DEBIAN_PACKAGE_DEPENDS "")
	    SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "dominic.letourneau@usherbrooke.ca")
	    SET(CPACK_DEBIAN_PACKAGE_DESCRIPTION "Manyears Software package.")
	    SET(CPACK_DEBIAN_PACKAGE_SECTION "devel")
	    SET(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
	    SET(CPACK_DEBIAN_PACKAGE_RECOMMENDS "")
	    SET(CPACK_DEBIAN_PACKAGE_SUGGESTS "")
	
	ENDIF(APPLE)


    INCLUDE(CPack)
ENDIF(UNIX)

##########################
# Definitions
##########################

ADD_DEFINITIONS(-DDEF_PACKAGE_NAME="${CPACK_PACKAGE_NAME}")
ADD_DEFINITIONS(-DDEF_PACKAGE_VERSION="${CPACK_PACKAGE_VERSION}")
ADD_DEFINITIONS(-DDEF_PACKAGE_VENDOR="${CPACK_PACKAGE_VENDOR}")
ADD_DEFINITIONS(-DDEF_NSIS_URL_INFO_ABOUT="${CPACK_NSIS_URL_INFO_ABOUT}")
ADD_DEFINITIONS(-DDEF_RESOURCE_FILE_LICENSE="${CPACK_RESOURCE_FILE_LICENSE}")
ADD_DEFINITIONS(-DDEF_PACKAGE_DESCRIPTION_FILE="${CPACK_PACKAGE_DESCRIPTION_FILE}")
ADD_DEFINITIONS(-DDEF_PROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}")

ADD_SUBDIRECTORY(dsplib)
ADD_SUBDIRECTORY(RTAudio)
ADD_SUBDIRECTORY(example)
ADD_SUBDIRECTORY(QtGUI)

add_custom_target(dist COMMAND ${CMAKE_MAKE_PROGRAM} package_source)

