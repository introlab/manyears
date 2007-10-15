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



SET(CPACK_PACKAGE_NAME "ManyEars")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "ManyEars FlowDesigner Toolbox")
SET(CPACK_PACKAGE_VENDOR "Dominic Letourneau, Jean-Marc Valin")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README")

IF(APPLE)
	SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/license.txt")
ELSE(APPLE)
	SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING")
ENDIF(APPLE)

SET(CPACK_PACKAGE_VERSION_MAJOR "0")
SET(CPACK_PACKAGE_VERSION_MINOR "2")
SET(CPACK_PACKAGE_VERSION_PATCH "0")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "ManyEars")
IF(WIN32 AND NOT UNIX)
  # There is a bug in NSI that does not handle full unix paths properly. Make
  # sure there is at least one set of four (4) backlasshes.
  # SET(CPACK_PACKAGE_ICON "${CMake_SOURCE_DIR}/Utilities/Release\\\\InstallIcon.bmp")
  # SET(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\FDSetup.exe")
  # SET(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY} FlowDesigner")
  # SET(CPACK_NSIS_HELP_LINK "http:\\\\\\\\manyears.sourceforge.net")
  SET(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\manyears.sourceforge.net")
  SET(CPACK_NSIS_CONTACT "manyears-devel@lists.sourceforge.net")
  SET(CPACK_NSIS_MODIFY_PATH ON)
ENDIF(WIN32 AND NOT UNIX)

INCLUDE(CPack)





