
SET(manyears3D_SRCS
	EqualGains3D.cc
	SourceTrack3D.cc
	TrackingDecision3D.cc
	Localize3D.cc
	SteeringVector3D.cc
	particles3D.cc
)	

#Look for dependencies
FIND_PACKAGE(FFTW3 REQUIRED)

SET(CMAKE_CXX_FLAGS "-msse -O3")

   
INCLUDE_DIRECTORIES(${FLOWDESIGNER_INCLUDE_DIR}
	${FFTW3_INCLUDE_DIR}
	../src
)

# create an executable file named "flowdesigner" from the source files 

add_library(libmanyears3D-dynamic SHARED ${manyears3D_SRCS})
	
set_target_properties(libmanyears3D-dynamic PROPERTIES OUTPUT_NAME manyears3D.tlb)

target_link_libraries(libmanyears3D-dynamic -L${FLOWDESIGNER_LIB_DIR} -lflow ${LIBXML2_LIBRARIES} ${FFTW3_LIBRARY} libmanyears-dynamic)


install(TARGETS libmanyears3D-dynamic DESTINATION ${FLOWDESIGNER_TOOLBOX_DIR}/ManyEars/lib)
  
#INSTALLING .def file  
MESSAGE( STATUS "EXECUTING : ${PERL} ${FLOWDESIGNER_BIN_DIR}/info2def.pl ${manyears3D_SRCS}")
INSTALL( CODE "EXECUTE_PROCESS(WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/ManyEars3D OUTPUT_FILE libmanyears3D.def COMMAND ${PERL} ${FLOWDESIGNER_BIN_DIR}/info2def.pl ${manyears3D_SRCS})" )  
INSTALL( FILES libmanyears3D.def DESTINATION ${FLOWDESIGNER_TOOLBOX_DIR}/ManyEars/def)

