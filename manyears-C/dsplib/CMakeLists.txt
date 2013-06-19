MESSAGE( STATUS "Buiding ManyEarsLib Library...")

SET(ManyEarsLib_SRCS
    Geometry/microphones.c
    Preprocessing/mcra.c
    Preprocessing/micst.c
    Preprocessing/preprocessor.c
    Localisation/beamformer.c
    Localisation/delays.c
    Localisation/potentialSources.c
    Localisation/rij.c
    Localisation/sphere.c
    Tracking/filter.c
    Tracking/mixture.c
    Tracking/trackedSources.c
    Separation/gss.c
    Separation/postfilter.c
    Separation/postfilteredSources.c
    Separation/separatedSources.c
    Output/output.c
    Output/outputChunk.c
    Output/outputSingleStream.c
    Postprocessing/postprocessor.c
    Utilities/linearCorrelation.c
    Utilities/dynamicMemory.c
    Utilities/fft.c
    Utilities/idList.c
    Utilities/idManager.c
    Utilities/matrix.c
    Utilities/randGen.c
    Utilities/transcendental.c
    Utilities/window.c
    parameters.c
    overallContext.c
)

SET(ManyEarsLib_HEADERS
    Geometry/microphones.h
    Preprocessing/mcra.h
    Preprocessing/micst.h
    Preprocessing/preprocessor.h
    Localisation/beamformer.h
    Localisation/delays.h
    Localisation/potentialSources.h
    Localisation/rij.h
    Localisation/sphere.h
    Tracking/filter.h
    Tracking/mixture.h
    Tracking/trackedSources.h
    Separation/gss.h
    Separation/postfilter.h
    Separation/postfilteredSources.h
    Separation/separatedSources.h
    Output/output.h
    Output/outputChunk.h
    Output/outputSingleStream.h
    Postprocessing/postprocessor.h
    Utilities/linearCorrelation.h
    Utilities/dynamicMemory.h
    Utilities/fft.h
    Utilities/idList.h
    Utilities/idManager.h
    Utilities/mathadvanced.h
    Utilities/matrix.h
    Utilities/randGen.h  
    Utilities/transcendental.h
    Utilities/window.h
    parameters.h
    overallContext.h
    hardware.h
)

#INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/dsplib )
INCLUDE_DIRECTORIES(.)


add_library(ManyEarsLib STATIC ${ManyEarsLib_SRCS} ${ManyEarsLib_HEADERS} )
target_link_libraries(ManyEarsLib )
set_target_properties(ManyEarsLib PROPERTIES LINK_FLAGS ${MANYEARS_LINK_FLAGS} OUTPUT_NAME manyears)

INSTALL(TARGETS ManyEarsLib DESTINATION lib)


#Install headers in the adequate directory
FOREACH(header_file ${ManyEarsLib_HEADERS})
	GET_FILENAME_COMPONENT(HEADER_PATH ${header_file} PATH)
	INSTALL(FILES ${header_file} DESTINATION include/${HEADER_PATH})
ENDFOREACH(header_file)


#INSTALL(FILES ${ManyEarsLib_HEADERS} DESTINATION include)

#add_executable(sse_test sseTest.c sseTest.h)
#set_target_properties(sse_test PROPERTIES LINK_FLAGS ${MANYEARS_LINK_FLAGS})
#target_link_libraries(sse_test ManyEarsLib -lm)



