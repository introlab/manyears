
#finding qt
find_package(Qt4 REQUIRED)

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
   asio/asio.cpp
   asio/asiodrivers.cpp
   asio/asiolist.cpp
   asio/iasiothiscallresolver.cpp 

)	


SET(CMAKE_CXX_FLAGS "-msse -O3")   
#SET(CMAKE_CXX_FLAGS "-O3")


ADD_DEFINITIONS(-D__WINDOWS_ASIO__)
INCLUDE_DIRECTORIES(${FLOWDESIGNER_SOURCE_DIR}/data-flow/include 
     	./
        C:/Dev-Cpp/include/fftw
	    ${LIBXML2_INCLUDE} 
        ${QT_INCLUDE_DIR}
   
)
# create an executable file named "flowdesigner" from the source files 

add_library(libmanyears-dynamic SHARED ${manyears_SRCS})
	
set_target_properties(libmanyears-dynamic PROPERTIES OUTPUT_NAME libmanyears.tlb)

target_link_libraries(libmanyears-dynamic -L${FLOWDESIGNER_SOURCE_DIR}/data-flow/src -llibflow ${LIBXML2_LIBRARIES} ${QT_QTCORE_LIBRARY} -lole32  -lfftw3 -lm )

install(TARGETS libmanyears-dynamic DESTINATION ${FD_TOOLBOX_DIR}/MANYEARS)
  
#INSTALLING .def file  
INSTALL( CODE "EXECUTE_PROCESS(WORKING_DIRECTORY ${FLOWDESIGNER_SOURCE_DIR}/manyears/src OUTPUT_FILE libmanyears.def COMMAND ${PERL} ${FLOWDESIGNER_SOURCE_DIR}/tools/src/info2def.pl ${manyears_SRCS})" )  
INSTALL( FILES libmanyears.def DESTINATION ${FD_TOOLBOX_DIR}/MANYEARS/)
INSTALL( FILES ${MANYEARS_EXAMPLES} DESTINATION examples/MANYEARS)
