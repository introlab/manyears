## Process this file with automake to produce Makefile.in. -*-Makefile-*-
# Disable automatic dependency tracking if using other tools than gcc and gmake
#AUTOMAKE_OPTIONS = no-dependencies

override bindir=${FLOWDESIGNER_BIN}


EXTRA_DIST = DefaultImage.xpm CMakeLists.txt
if WITH_QT

INCLUDES = $(XML2_INCLUDE) $(QT_INCLUDES) $(FLOWDESIGNER_INCLUDE)  -Wall -DQT_THREAD_SUPPORT

bin_PROGRAMS = audioviewer

audioviewer_SOURCES = .moc/moc_audioview.cpp \
		.moc/moc_TrackAudioWidget.cpp \
		.moc/moc_ImageView.cpp \
		.moc/moc_FDReader.cpp \
		audioview.cpp \
		TrackAudioWidget.cpp \
		ImageView.cpp \
		FDReader.cpp \
		SourceInfo.cpp \
		main.cpp


audioviewer_LDADD = $(XML2_LIB) $(QT_LIBS) $(FLOWDESIGNER_LIB) -lasound

 
# This rule lets GNU make create any moc_*.cpp from the equivalent *.h
.moc/moc_%.cpp: %.h
	mkdir -p .moc
	moc-qt4 $< -o $@

%.ui.cpp: %.ui
	uic-qt4 $< -o $<.h
	uic-qt4 -impl $<.h $< -o $@

# Adjust this line according to your resources.qrc
# 	resources.cpp : resources.qrc octave-gui.png
# 		rcc resources.qrc -o resources.cpp
#
# This line ensures that generated moc_*.cpp and resources.cpp files are
# deleted when we make clean.
CLEANFILES = $(filter .moc/moc_%.cpp,$(audioviewer_SOURCES) )

endif

noinst_HEADERS = AudioSource.h \
		audioview.h  \
		FDReader.h \
		ImageView.h \
		TrackAudioWidget.h \
		SourceInfo.h

