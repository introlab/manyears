
PROJECT (ManyEars)

#Look for minimum cmake version
cmake_minimum_required(VERSION 2.6.0)

SET(CMAKE_BUILD_TYPE Release )
SET(CMAKE_VERBOSE_MAKEFILE ON)

#Let's find FlowDesigner
#Hard coded paths right now...
IF (WIN32)
	SET(FD_PREFIX "C:/Progra~1/FlowDesigner")
ELSE(WIN32)
	SET(FD_PREFIX "/Volumes/FlowDesigner-0.10.0/FlowDesigner.app/Contents/Resources/")
ENDIF(WIN32)
	
SET (FLOWDESIGNER_BIN_DIR "${FD_PREFIX}/bin/")
SET (FLOWDESIGNER_INCLUDE_DIR "${FD_PREFIX}/include/flowdesigner/")
SET (FLOWDESIGNER_LIB_DIR "${FD_PREFIX}/lib/")
SET (FLOWDESIGNER_TOOLBOX_DIR "${FD_PREFIX}/lib/flowdesigner/toolbox/")

ADD_DEFINITIONS(-DSPHINX_INSTALL_DIR=\"${FLOWDESIGNER_TOOLBOX_DIR}/ManyEars/sphinx4\")


#SET(PEDANTIC TRUE)


# skip the full RPATH for the build tree
#SET(CMAKE_SKIP_BUILD_RPATH  FALSE)

# when building, use the install RPATH already
# (so it doesn't need to relink when installing)
#SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE) 

# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
#SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)



#Add the macros directory for cmake scripts
SET(CMAKE_MODULE_PATH "${ManyEars_SOURCE_DIR}/macros")

#Look for dependencies
find_package(Qt4)
find_package(Perl REQUIRED)
find_package(FFTW3 REQUIRED)
find_package(LibXml2 REQUIRED)
find_package(Java)


#set the prefix
SET(CMAKE_INSTALL_PREFIX ${FD_PREFIX})

# use, i.e. don't skip the full RPATH for the build tree
SET(CMAKE_SKIP_BUILD_RPATH  FALSE)

# when building, don't use the install RPATH already
# (but later on when installing)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE) 

# the RPATH to be used when installing
SET(CMAKE_INSTALL_RPATH "${FLOWDESIGNER_TOOLBOX_DIR}/ManyEars/lib")

# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)


#Adding subdirectories
ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(tools)
ADD_SUBDIRECTORY(cameraBridge)

IF (JAVA_COMPILE AND JAVA_RUNTIME)
    ADD_SUBDIRECTORY(sphinx4)
ENDIF(JAVA_COMPILE AND JAVA_RUNTIME)

#ADD_SUBDIRECTORY(ManyEars3D)

IF (QT4_FOUND)
	ADD_SUBDIRECTORY(GUI)
ENDIF(QT4_FOUND)

#WIN32 DLL INSTALLATION
IF(WIN32)

	SET(FFTW3_DLL
 		"C:/FlowDesigner/fftw-3.2.1-dll/libfftw3f-3.dll"
		"C:/FlowDesigner/fftw-3.2.1-dll/libfftw3-3.dll"
		
	)
	INSTALL(FILES ${FFTW3_DLL} DESTINATION bin)

ENDIF (WIN32)


#SET(CPACK_PACKAGE_NAME "ManyEars")
#SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "ManyEars FlowDesigner Toolbox")
#SET(CPACK_PACKAGE_VENDOR "Dominic Letourneau, Jean-Marc Valin")
#SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README")

#IF(APPLE)
#	SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/license.txt")
#ELSE(APPLE)
#	SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING")
#ENDIF(APPLE)

#SET(CPACK_PACKAGE_VERSION_MAJOR "0")
#SET(CPACK_PACKAGE_VERSION_MINOR "2")
#SET(CPACK_PACKAGE_VERSION_PATCH "0")
#SET(CPACK_PACKAGE_INSTALL_DIRECTORY "FlowDesigner")
#IF(WIN32 AND NOT UNIX)
  # There is a bug in NSI that does not handle full unix paths properly. Make
  # sure there is at least one set of four (4) backlasshes.
  # SET(CPACK_PACKAGE_ICON "${CMake_SOURCE_DIR}/Utilities/Release\\\\InstallIcon.bmp")
  # SET(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\FDSetup.exe")
#  SET(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY} ManyEars Toolkit")
  # SET(CPACK_NSIS_HELP_LINK "http:\\\\\\\\manyears.sourceforge.net")
#  SET(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\manyears.sourceforge.net")
#  SET(CPACK_NSIS_CONTACT "manyears-devel@lists.sourceforge.net")
#  SET(CPACK_NSIS_MODIFY_PATH ON)
#  SET(CPACK_PACKAGE_EXECUTABLES "audioviewer;AudioViewer")
#  SET(CPACK_TOPLEVEL_TAG "ManyEars")
#ENDIF(WIN32 AND NOT UNIX)

INCLUDE(CPack)





