MESSAGE( STATUS "Buiding ManyEarsLib Library...")

#ADD DEFINITIONS HERE
ADD_DEFINITIONS(-DUSE_SIMD)

SET(ManyEarsLib_SRCS
    beamformer.c
    fft.c
    filter.c
    micSoundTrack.c
    mixture.c
    parameters.c
    randGen.c
    sphere.c
    dynamicMemory.c
    potentialSources.c
    trackedSources.c
    overallContext.c
)

SET(ManyEarsLib_HEADERS
    beamformer.h
    fft.h
    filter.h
    hardware.h
    micSoundTrack.h
    mixture.h
    parameters.h
    randGen.h
    sphere.h
    mathadvanced.h
    dynamicMemory.h
    potentialSources.h
    trackedSources.h
    overallContext.h
)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/ManyEars )

add_library(ManyEarsLib STATIC ${ManyEarsLib_SRCS} ${ManyEarsLib_HEADERS} )
target_link_libraries(ManyEarsLib )
set_target_properties(ManyEarsLib PROPERTIES LINK_FLAGS "--enable-auto-import" OUTPUT_NAME manyears)

INSTALL(TARGETS ManyEarsLib DESTINATION lib)
INSTALL(FILES ${ManyEarsLib_HEADERS} DESTINATION include)
