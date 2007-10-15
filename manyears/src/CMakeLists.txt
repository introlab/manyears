

SET(MANYEARS_EXAMPLES
 ../n-files/LocalizeSeparAndSaveWavWin32.n
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
   RtAudio.cpp
   info.cc
   conj_window.cc
   SourceInfo.cc
   LocalizeBeam.cc
   sphere.cc
   particles.cc   
)	


SET(CMAKE_CXX_FLAGS "-msse -O3")
   
INCLUDE_DIRECTORIES(${FLOWDESIGNER_INCLUDE_DIR}
	${FFTW3_INCLUDE_DIR}
	./
)

IF(WIN32)
	ADD_DEFINITIONS(-D__WINDOWS_ASIO__)
	INCLUDE_DIRECTORIES(${FLOWDESIGNER_SOURCE_DIR}/data-flow/include 
    	./
		C:/Dev-Cpp/include/fftw
		${LIBXML2_INCLUDE} 
       	${QT_INCLUDE_DIR}
	)

	SET(manyears_SRCS ${manyears_SRCS}
  			asio/asio.cpp 
  			asio/asiodrivers.cpp
  			asio/asiolist.cpp
  			asio/iasiothiscallresolver.cpp 
  	)

ENDIF(WIN32)

IF(APPLE)
	ADD_DEFINITIONS(-D__MAXOSX_CORE__ -DMAC)
ENDIF(APPLE)


# create an executable file named "flowdesigner" from the source files 

add_library(libmanyears-dynamic SHARED ${manyears_SRCS})
	
set_target_properties(libmanyears-dynamic PROPERTIES OUTPUT_NAME libmanyears.tlb)

IF(WIN32)
	target_link_libraries(libmanyears-dynamic -L${FLOWDESIGNER_LIB_DIR} -llibflow ${LIBXML2_LIBRARIES} ${QT_QTCORE_LIBRARY} -lole32  -lfftw3 -lm )
ENDIF(WIN32)

IF(APPLE)
	set_target_properties(libmanyears-dynamic PROPERTIES LINK_FLAGS "-framework CoreFoundation -framework CoreAudio -framework AudioUnit -framework AudioToolbox")
	target_link_libraries(libmanyears-dynamic -L${FLOWDESIGNER_BIN_DIR} -L${FLOWDESIGNER_LIB_DIR} ${QT_QTCORE_LIBRARY} ${FFTW3_LIBRARY} -llibflow -lm )
ENDIF(APPLE)


install(TARGETS libmanyears-dynamic DESTINATION lib)
  
#INSTALLING .def file  
MESSAGE( STATUS "EXECUTING : ${PERL} ${FLOWDESIGNER_BIN_DIR}/info2def.pl ${manyears_SRCS}")
INSTALL( CODE "EXECUTE_PROCESS(WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/src OUTPUT_FILE libmanyears.def COMMAND ${PERL} ${FLOWDESIGNER_BIN_DIR}/info2def.pl ${manyears_SRCS})" )  
INSTALL( FILES libmanyears.def DESTINATION def)
INSTALL( FILES ${MANYEARS_EXAMPLES} DESTINATION examples)
