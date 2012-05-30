

# the next line sets up include and link directories and defines some variables that we will use.
# you can modify the behavior by setting some variables, e.g.
#   
# -> this will cause cmake to include and link against the OpenGL module

SET(QT_USE_QTNETWORK TRUE)
SET(QT_USE_QTSVG TRUE)
SET(QT_USE_QTXML TRUE)
SET(QT_USE_QTSCRIPT TRUE)
SET(QT_USE_QTOPENGL TRUE)
SET(QT_USE_QTMULTIMEDIA TRUE)

find_package(Qt4 REQUIRED)
find_package(OpenGL REQUIRED)


include(${QT_USE_FILE})


SET(ManyEars_SRCS
    property.cpp
    configurationVector.cpp
    colorPalette.cpp
    colorID.cpp
    sphereObj.cpp
    parametersManager.cpp
    graph2D.cpp
    graph3D.cpp
    mdiWindow.cpp
    mdiSubWindow.cpp
    propertyEditor.cpp
    sourceBuffer.cpp
    aboutDialog.cpp
    inputOutputManager.cpp
    coreThread.cpp
    sourceEvents.cpp
    StreamOutputWidget.cpp
)

SET(ManyEars_HEADERS 
    property.h
    configurationVector.h
    colorPalette.h
    colorID.h
    sphereObj.h
    parametersManager.h
    graph2D.h
    graph3D.h
    mdiWindow.h
    mdiSubWindow.h
    propertyEditor.h
    sourceBuffer.h
    aboutDialog.h
    inputOutputManager.h
    coreThread.h
    sourceEvents.h
    StreamOutputWidget.h
)

SET(ManyEars_UIS
    StreamOutputWidget.ui
)

SET(qrc 
    resources.qrc
)



# generate rules for building source files from the resources
QT4_ADD_RESOURCES(SRCS_QRC ${qrc})

SET (QTVERSION "${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}.${QT_VERSION_PATCH}")


#This will generate ui_* for Qt
QT4_WRAP_UI(ManyEars_UI_SRCS ${ManyEars_UIS})

#This will generate moc_* for Qt
QT4_WRAP_CPP(ManyEars_MOC_SRCS ${ManyEars_HEADERS})

#Include directories
INCLUDE_DIRECTORIES(${QT_INCLUDE_DIR}
                    ${OPENGL_INCLUDE_DIR}
                    ${PROJECT_SOURCE_DIR}/
                    ${PROJECT_SOURCE_DIR}/dsplib
                    ${PROJECT_SOURCE_DIR}/RTAudio
                    ${CMAKE_CURRENT_BINARY_DIR}
                    )

#include files
IF (WIN32)
    #ManyEars Executable
    add_executable(ManyEars WIN32 main.cpp ${ManyEars_SRCS} ${ManyEars_HEADERS} ${ManyEars_MOC_SRCS} ${ManyEars_UI_SRCS} ${SRCS_QRC})

	IF (MINGW)
		set_target_properties(ManyEars PROPERTIES LINK_FLAGS ${MANYEARS_LINK_FLAGS})
		target_link_libraries(ManyEars ${QT_LIBRARIES} ManyEarsLib RTAudioLib ${OPENGL_LIBRARIES})
	ELSE (MINGW)
		target_link_libraries(ManyEars ${QT_LIBRARIES} ManyEarsLib RTAudioLib ${OPENGL_LIBRARIES})
	ENDIF(MINGW)
    INSTALL(TARGETS ManyEars DESTINATION bin)
ENDIF (WIN32)

IF (UNIX)
	IF(APPLE)
            #ManyEars Executable
            add_executable(ManyEars MACOSX_BUNDLE main.cpp ${ManyEars_SRCS} ${ManyEars_HEADERS} ${ManyEars_MOC_SRCS} ${ManyEars_UI_SRCS} ${SRCS_QRC} )
            set_target_properties(ManyEars PROPERTIES LINK_FLAGS "-framework OpenGL -framework CoreFoundation -framework CoreAudio -framework AudioUnit -framework AudioToolbox")
            target_link_libraries(ManyEars ${QT_LIBRARIES} ManyEarsLib RTAudioLib )

            #Create a custom app package
            #INSTALL(TARGETS ManyEars DESTINATION ${MACOSX_BUNDLE_BUNDLE_NAME}/Contents/Resources/bin)
            INSTALL(TARGETS ManyEars DESTINATION .)
            INSTALL(FILES ${PROJECT_SOURCE_DIR}/${MACOSX_BUNDLE_COPYRIGHT} DESTINATION ${MACOSX_BUNDLE_BUNDLE_NAME}/Contents)
            INSTALL(FILES ${PROJECT_SOURCE_DIR}/${MACOSX_BUNDLE_ICON_FILE} DESTINATION ${MACOSX_BUNDLE_BUNDLE_NAME}/Contents/Resources)


        ELSE(APPLE)	
	    #ManyEars Executable
            add_executable(ManyEars main.cpp ${ManyEars_SRCS} ${ManyEars_HEADERS} ${ManyEars_MOC_SRCS} ${ManyEars_UI_SRCS} ${SRCS_QRC})
	    target_link_libraries(ManyEars ${QT_LIBRARIES} ManyEarsLib RTAudioLib)
	    INSTALL(TARGETS ManyEars DESTINATION bin)
	ENDIF(APPLE)
   
ENDIF(UNIX)



