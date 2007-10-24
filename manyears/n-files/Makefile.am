#AUTOMAKE_OPTIONS = no-dependencies

override includedir=${prefix}/lib/flowdesigner/toolbox/ManyEars
override datadir=${prefix}/lib/flowdesigner/toolbox/ManyEars
override libdir=${prefix}/lib/flowdesigner/toolbox/ManyEars

toolboxdir = $(datadir)

toolbox_DATA = localization.n \
	localize.n \
	separation_spl_coque_raw.n \
	track_sources.n \
	LocalizeSeparAndSaveWav.n

EXTRA_DIST = $(toolbox_DATA)

