#ManyEars Executable

#add source here
SET(SRCS
   manyears_console.c
   manyears_iterative.c
)


SET(SAMPLES
    "${PROJECT_SOURCE_DIR}/Samples/rawdata_2srcs_90_90.raw"
)

#Include directories
INCLUDE_DIRECTORIES(${QT_INCLUDE_DIR}
                    ${PROJECT_SOURCE_DIR}/
                    ${PROJECT_SOURCE_DIR}/dsplib
                    ${PROJECT_SOURCE_DIR}/RTAudio
                    )

#Batch application
add_executable(manyears_console manyears_console.c)
set_target_properties(manyears_console PROPERTIES LINK_FLAGS ${MANYEARS_LINK_FLAGS})
target_link_libraries(manyears_console ManyEarsLib -lm)

#Live application
add_executable(manyears_live manyears_live.cpp)
set_target_properties(manyears_live PROPERTIES LINK_FLAGS ${MANYEARS_LINK_FLAGS})
target_link_libraries(manyears_live ManyEarsLib RTAudioLib -lm)

#Iterative separation application
add_executable(manyears_iterative manyears_iterative.c)
set_target_properties(manyears_iterative PROPERTIES LINK_FLAGS ${MANYEARS_LINK_FLAGS})
target_link_libraries(manyears_iterative ManyEarsLib -lm)

#INSTALL DEMONSTRATION FILES
INSTALL(FILES ${SRCS} DESTINATION example)

#INSTALL SAMPLE FILES
INSTALL(FILES ${SAMPLES} DESTINATION samples)
