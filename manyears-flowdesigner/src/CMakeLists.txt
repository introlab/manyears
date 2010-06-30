SET(MANYEARS_EXAMPLES
 ../n-files/LocalizeSeparAndSaveWavWin32.n
 ../n-files/LocalizeSeparAndSaveTCP.n
 ../n-files/start_manyears.bat
 ../config/cube_mic_pos.mat
 ../config/robot_mic_pos.mat
 )

SET(manyears_SRCS
   openStream.cc
   EqualGains.cc
   SourceTrack.cc
   SeparGSS.cc
   SaveWavWin32.cc
   SaveRawAudio.cc
   RtAudio.cpp
   info.cc
   conj_window.cc
   SourceInfo.cc
   LocalizeBeam.cc
   sphere.cc
   particles.cc   
   UDPOSCOut.cc
   QtTCPBinarySend.cc
   SaveAudioStreamTCP.cc
   ../ManyEars3D/EqualGains3D.cc
   ../ManyEars3D/SourceTrack3D.cc
   ../ManyEars3D/TrackingDecision3D.cc
   ../ManyEars3D/Localize3D.cc
   ../ManyEars3D/SteeringVector3D.cc
   ../ManyEars3D/particles3D.cc
)	

#Look for dependencies
FIND_PACKAGE(FFTW3 REQUIRED)
IF(UNIX AND NOT APPLE)
    find_package(Alsa REQUIRED)
ENDIF(UNIX AND NOT APPLE)

SET(CMAKE_CXX_FLAGS "-msse -O3")

#finding qt
SET(QT_USE_QTNETWORK TRUE)

find_package(Qt4 REQUIRED)

#REQUIRED
include(${QT_USE_FILE})

   
INCLUDE_DIRECTORIES(${FLOWDESIGNER_INCLUDE_DIR}
    ${QT_INCLUDE_DIR}
	${FFTW3_INCLUDE_DIR}
	./
	../ManyEars3D/
)

IF(WIN32)
	ADD_DEFINITIONS(-D__WINDOWS_ASIO__)
	INCLUDE_DIRECTORIES(${INCLUDE_DIRECTORIES}     	
		asio/
	)

	SET(manyears_SRCS ${manyears_SRCS}
  			asio/asio.cpp 
  			asio/asiodrivers.cpp
  			asio/asiolist.cpp
  			asio/iasiothiscallresolver.cpp 
  	)

ENDIF(WIN32)

IF(NOT WIN32)
	SET(manyears_SRCS ${manyears_SRCS}
  			SaveWav.cc
  	)
ENDIF(NOT WIN32)

IF(APPLE)
	ADD_DEFINITIONS(-D__MACOSX_CORE__ -DMAC)
ENDIF(APPLE)
IF(UNIX AND NOT APPLE)
    # With RtAudio, we use Alsa by default
    # __LINUX_ALSA__ , __UNIX_JACK__ , __LINUX_OSS__ 
    #
	ADD_DEFINITIONS(-D__LINUX_ALSA__ -DLINUX)
    INCLUDE_DIRECTORIES(${INCLUDE_DIRECTORIES}     	
		${LIBALSA_INCLUDE_DIR} 
	)
    SET(manyears_SRCS ${manyears_SRCS}
  			alsadevice.cpp 
  	)
ENDIF(UNIX AND NOT APPLE)

# create an executable file named "flowdesigner" from the source files 

add_library(libmanyears-dynamic SHARED ${manyears_SRCS})
	
set_target_properties(libmanyears-dynamic PROPERTIES OUTPUT_NAME manyears.tlb)

IF(WIN32)
	target_link_libraries(libmanyears-dynamic -L${FLOWDESIGNER_LIB_DIR} -llibflow ${QT_QTCORE_LIBRARY} ${QT_QTNETWORK_LIBRARY} -lole32  ${FFTW3_LIBRARY} -lm )
ENDIF(WIN32)

IF(APPLE)
	set_target_properties(libmanyears-dynamic PROPERTIES LINK_FLAGS "-framework CoreFoundation -framework CoreAudio -framework AudioUnit -framework AudioToolbox"
	BUILD_WITH_INSTALL_RPATH ON
	INSTALL_NAME_DIR ${FLOWDESIGNER_TOOLBOX_DIR}/ManyEars/lib)
	target_link_libraries(libmanyears-dynamic -L${FLOWDESIGNER_BIN_DIR} -L${FLOWDESIGNER_LIB_DIR} ${QT_QTCORE_LIBRARY} ${QT_QTNETWORK_LIBRARY} ${FFTW3_LIBRARY} -lflow -lm )
ENDIF(APPLE)
    
IF(UNIX)
    target_link_libraries(libmanyears-dynamic -L${FLOWDESIGNER_BIN_DIR} -L${FLOWDESIGNER_LIB_DIR} ${QT_QTCORE_LIBRARY} ${QT_QTNETWORK_LIBRARY} ${FFTW3_LIBRARY} -lflow -lm ${LIBALSA_LIBRARIES})
ENDIF(UNIX)


install(TARGETS libmanyears-dynamic DESTINATION ${FLOWDESIGNER_TOOLBOX_DIR}/ManyEars/lib)
  
#INSTALLING .def file  
MESSAGE( STATUS "EXECUTING : ${PERL} ${FLOWDESIGNER_BIN_DIR}/info2def.pl ${manyears_SRCS}")
INSTALL( CODE "EXECUTE_PROCESS(WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/src OUTPUT_FILE libmanyears.def COMMAND ${PERL} ${FLOWDESIGNER_BIN_DIR}info2def.pl ${manyears_SRCS})" )  
INSTALL( FILES libmanyears.def DESTINATION ${FLOWDESIGNER_TOOLBOX_DIR}/ManyEars/def)
INSTALL( FILES ${MANYEARS_EXAMPLES} DESTINATION ${FLOWDESIGNER_TOOLBOX_DIR}/ManyEars/examples)
