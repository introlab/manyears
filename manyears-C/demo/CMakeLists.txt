#ManyEars Executable

#add source here
SET(SRCS
   manyears_demo.c
)

#Include directories
INCLUDE_DIRECTORIES(${QT_INCLUDE_DIR}
                    ${PROJECT_SOURCE_DIR}/
                    ${PROJECT_SOURCE_DIR}/dsplib
                    ${PROJECT_SOURCE_DIR}/RTAudio
                    )


add_executable(manyears_demo manyears_demo.c)
set_target_properties(manyears_demo PROPERTIES LINK_FLAGS ${MANYEARS_LINK_FLAGS})
target_link_libraries(manyears_demo ManyEarsLib -lm)


#INSTALL DEMONSTRATION FILES
INSTALL(FILES ${SRCS} DESTINATION example)


