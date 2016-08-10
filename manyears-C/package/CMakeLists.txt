
INCLUDE(CPack)

#Files excluded from package
set(CPACK_SOURCE_IGNORE_FILES
  "build"
  "bin"
  ${CPACK_SOURCE_IGNORE_FILES}
  ".svn"
)



#include files
IF (WIN32)

    #EXECUTE QT_QMAKE TO FIND ALL NECESSARY LIBRARY WE NEED TO INSTALL
    EXEC_PROGRAM(${QT_QMAKE_EXECUTABLE} ARGS "-query QT_INSTALL_BINS" OUTPUT_VARIABLE QTBINS)
    SET(QT_BINARY_DIR ${QTBINS} CACHE INTERNAL "" FORCE)
    FILE(GLOB QT_DLL "${QT_BINARY_DIR}/*.dll")
    FILE(GLOB QT_DEBUG_DLL "${QT_BINARY_DIR}/*d4.dll")
    list(REMOVE_ITEM QT_DLL ${QT_DEBUG_DLL})


    #WE NEED TO CORRECT THE PATH OF EACH REMAINING LIBRARY SINCE "\" IS CONSIDER AN ESCAPE SEQUENCE FOR CMAKE
    FOREACH(LIB ${QT_DLL})
        STRING(REGEX REPLACE "\\\\" "/" NEWLIB ${LIB})
        list(REMOVE_ITEM QT_DLL ${LIB})
        list(APPEND QT_DLL ${NEWLIB})
    ENDFOREACH(LIB)

    INSTALL(FILES ${QT_DLL} DESTINATION bin)

    #############################
    # Packaging stuff for Windows
    #############################
    SET(CPACK_SOURCE_GENERATOR "ZIP")
    SET(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/COPYING.TXT")
    SET(CPACK_PACKAGE_NAME "ManyEars")
    SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "ManyEars")
    SET(CPACK_PACKAGE_VENDOR "IntRoLab")
    SET(CPACK_PACKAGE_DESCRIPTION_FILE "${PROJECT_SOURCE_DIR}/README.TXT")
    SET(CPACK_PACKAGE_CONTACT "francois.grondin@usherbrooke.ca")
    SET(CPACK_PACKAGE_INSTALL_DIRECTORY "ManyEars")
    SET(CPACK_NSIS_URL_INFO_ABOUT "http://introlab.gel.usherbrooke.ca")
    SET(CPACK_NSIS_CONTACT "dominic.letourneau@usherbrooke.ca")
    SET(CPACK_NSIS_MODIFY_PATH OFF)
    SET(CPACK_PACKAGE_EXECUTABLES "ManyEars" "ManyEars")

    INSTALL(FILES ${PROJECT_SOURCE_DIR}/README.TXT DESTINATION .)


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
            SET(MACOSX_BUNDLE_COPYRIGHT "README.TXT")
            SET(MACOSX_BUNDLE_INFO_PLIST "Info.plist")

            INSTALL(FILES ${PROJECT_SOURCE_DIR}/README.TXT DESTINATION .)

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



add_custom_target(dist COMMAND ${CMAKE_MAKE_PROGRAM} package_source)
