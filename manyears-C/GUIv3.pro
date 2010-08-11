HEADERS += algorithmCore.h \
    audioManager.h \
    colorID.h \
    colorPalette.h \
    graph2D.h \
    mdiWindow.h \
    soundInputConfig.h \
    potentialSourceManager.h \
	source.h \
	property.h \
	configurationVector.h \
	parametersManager.h \
	propertyEditor.h \
	sourceBuffer.h \
    ManyEars/beamformer.h \
    ManyEars/fft.h \
    ManyEars/filter.h \
    ManyEars/hardware.h \
    ManyEars/iodisk.h \
    ManyEars/micSoundTrack.h \
    ManyEars/mixture.h \
    ManyEars/parameters.h \
    ManyEars/randGen.h \
    ManyEars/sphere.h \
    RTAudio/asio.h \
    RTAudio/asiodrivers.h \
    RTAudio/asiodrvr.h \
    RTAudio/asiolist.h \
    RTAudio/asiosys.h \
    RTAudio/dsound.h \
    RTAudio/ginclude.h \
    RTAudio/iasiodrv.h \
    RTAudio/iasiothiscallresolver.h \
    RTAudio/RtAudio.h \
    RTAudio/RtError.h \
    RTAudio/soundcard.h
SOURCES += main.cpp \
    algorithmCore.cpp \
    audioManager.cpp \
    colorID.cpp \
    colorPalette.cpp \
    graph2D.cpp \
    mdiWindow.cpp \
    soundInputConfig.cpp \
	source.cpp \
	property.cpp \
	configurationVector.cpp \
	parametersManager.cpp \
	propertyEditor.cpp \
	sourceBuffer.cpp \
    potentialSourceManager.cpp \
    ManyEars/beamformer.c \
    ManyEars/fft.c \
    ManyEars/filter.c \
    ManyEars/iodisk.c \
    ManyEars/micSoundTrack.c \
    ManyEars/mixture.c \
    ManyEars/parameters.c \
    ManyEars/randGen.c \
    ManyEars/sphere.c \
    RTAudio/asio.cpp \
    RTAudio/asiodrivers.cpp \
    RTAudio/asiolist.cpp \
    RTAudio/iasiothiscallresolver.cpp \
    RTAudio/RtAudio.cpp
QT += opengl
DEFINES += __WINDOWS_ASIO__
DEFINES -= UNICODE
LIBS += -lole32
