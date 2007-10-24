## Process this file with automake to produce Makefile.in. -*-Makefile-*-

# $Id: Makefile.am,v 1.4 2006-06-13 18:23:51 dominic Exp $

# Disable automatic dependency tracking if using other tools than gcc and gmake
#AUTOMAKE_OPTIONS = no-dependencies

override includedir=${prefix}/lib/flowdesigner/toolbox/ManyEars
override datadir=${prefix}/lib/flowdesigner/toolbox/ManyEars
override libdir=${prefix}/lib/flowdesigner/toolbox/ManyEars

#override bindir=${FLOWDESIGNER_BIN}

lib_LTLIBRARIES = libmanyears.la 

bin_PROGRAMS = audio_server audio_client audio_server2

audio_server_SOURCES = audio_server.cc
audio_server2_SOURCES = audio_server2.cc AudioServer.cc alsadevice.cpp
audio_client_SOURCES = audio_client.cc

noinst_HEADERS = alsadevice.h \
	conj_window.h \
	particles.h \
	SourceInfo.h \
	AudioServer.h \
	Map.h \
	solver.h \
	sphere.h

libmanyears_la_SOURCES = \
	LocalizeBeam.cc \
	sphere.cc \
	conj_window.cc \
	EqualGains.cc \
	SeparGSS.cc \
	SaveWav.cc \
	SaveSpeex.cc \
	SourceTrack.cc \
	SourceInfo.cc \
	particles.cc 

install-data-local:
	mkdir -p $(datadir)
	(cd $(libdir); rm -f manyears.tlb; ln -s libmanyears.so manyears.tlb)
	(cd $(top_srcdir)/src; info2def.pl $(libmanyears_la_SOURCES) > $(datadir)/manyears.def)

libmanyears_la_LDFLAGS = -release $(LT_RELEASE) -L/opt/fftw/lib -lfftw3f -L/opt/fftw/lib $(FLOWDESIGNER_LIB) $(XML2_LIB)

INCLUDES = $(FLOWDESIGNER_INCLUDE) -I$(FLOWDESIGNER_DATA)/RobotFlow/include $(XML2_INCLUDE)

audio_server_LDADD = -lasound

audio_server2_LDADD = -lasound

libmanyears_la_LIBADD = -lfftw -lfftw3f

EXTRA_DIST = CMakeLists.txt

