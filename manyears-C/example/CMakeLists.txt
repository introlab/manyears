#ManyEars Executable

#add source here
SET(SRCS
   manyears_console.c
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


add_executable(manyears_console manyears_console.c)
set_target_properties(manyears_console PROPERTIES LINK_FLAGS ${MANYEARS_LINK_FLAGS})
target_link_libraries(manyears_console ManyEarsLib -lm)


#INSTALL DEMONSTRATION FILES
INSTALL(FILES ${SRCS} DESTINATION example)

#INSTALL SAMPLE FILES
INSTALL(FILES ${SAMPLES} DESTINATION samples)
