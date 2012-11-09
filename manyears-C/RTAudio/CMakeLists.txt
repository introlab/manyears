MESSAGE( STATUS "Buiding RTAudio Library...")

IF (WIN32)

    ADD_DEFINITIONS(-D__WINDOWS_ASIO__)


    SET(RTAudioLib_SRCS
        RtAudio.cpp
        iasiothiscallresolver.cpp
        asiolist.cpp
        asiodrivers.cpp
        asio.cpp
    )

    SET(RTAudioLib_HEADERS
        soundcard.h
        RtError.h
        RtAudio.h
        iasiothiscallresolver.h
        iasiodrv.h
        ginclude.h
        dsound.h
        asiosys.h
        asiolist.h
        asiodrvr.h
        asiodrivers.h
        asio.h
    )
    
    INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/RTAudio ${ALSA_INCLUDE_DIR} )
    add_library(RTAudioLib STATIC ${RTAudioLib_SRCS} ${RTAudioLib_HEADERS} )
    target_link_libraries(RTAudioLib ${ALSA_LIBRARY})
    set_target_properties(RTAudioLib PROPERTIES LINK_FLAGS ${MANYEARS_LINK_FLAGS} OUTPUT_NAME rtaudio)
    #INSTALL(TARGETS RTAudioLib DESTINATION bin)
    INSTALL(TARGETS RTAudioLib DESTINATION rtaudio/lib) 
    INSTALL(FILES ${RTAudioLib_HEADERS} DESTINATION rtaudio/include)
ENDIF(WIN32)

IF (UNIX)
        SET(RTAudioLib_SRCS
		RtAudio.cpp
	)
	
	SET(RTAudioLib_HEADERS
		soundcard.h
		RtError.h
		RtAudio.h
		ginclude.h
	)
	
	IF (APPLE)
        ADD_DEFINITIONS(-D__MACOSX_CORE__ -DMAC)
		INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/RTAudio)
                add_library(RTAudioLib SHARED ${RTAudioLib_SRCS} ${RTAudioLib_HEADERS} )
		target_link_libraries(RTAudioLib)
		set_target_properties(RTAudioLib PROPERTIES LINK_FLAGS "-framework CoreFoundation -framework CoreAudio -framework AudioUnit -framework AudioToolbox" OUTPUT_NAME rtaudio) 
	ELSE(APPLE)
        ADD_DEFINITIONS(-D__LINUX_ALSA__)
		#find_package(ALSA REQUIRED)
		SET(ALSA_LIBRARY "-lasound -lpthread")
		INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/RTAudio ${ALSA_INCLUDE_DIR} )		
                add_library(RTAudioLib SHARED ${RTAudioLib_SRCS} ${RTAudioLib_HEADERS} )
		target_link_libraries(RTAudioLib ${ALSA_LIBRARY})
		MESSAGE("ALSA lib : ${ALSA_LIBRARY}")
		set_target_properties(RTAudioLib PROPERTIES LINK_FLAGS ${MANYEARS_LINK_FLAGS} OUTPUT_NAME rtaudio) 
	ENDIF(APPLE)
	
	#Installation directory
	INSTALL(TARGETS RTAudioLib DESTINATION rtaudio/lib)	
        INSTALL(FILES ${RTAudioLib_HEADERS} DESTINATION rtaudio/include)
    
ENDIF(UNIX)





