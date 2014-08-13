AUTOMAKE_OPTIONS = subdir-objects
AM_CFLAGS = -I$(includedir) -Wall

bin_PROGRAMS = noapp
noapp_SOURCES = noapp_src/noapp.c
noapp_LDADD = $(POCKETSPHINX_LIBS) $(SPHINXBASE_LIBS)
noapp_CFLAGS = $(AM_CFLAGS) $(POCKETSPHINX_CFLAGS) $(SPHINXBASE_CFLAGS) -DMODELDIR=\"$(pkgdatadir)\"
if DEBUG
noapp_CFLAGS += -DDEBUG
endif

bin_PROGRAMS += recorder
recorder_SOURCES = recorder_src/recorder.c recorder_src/rec_handles.c
recorder_LDADD = -lpulse -lm
recorder_CFLAGS = $(AM_CFLAGS)
if DEBUG
recorder_CFLAGS += -DDEBUG
endif

bin_PROGRAMS += recognizer
recognizer_SOURCES = recognizer_src/recognizer.c
recognizer_LDADD = $(POCKETSPHINX_LIBS) $(SPHINXBASE_LIBS)
recognizer_CFLAGS = $(AM_CFLAGS) $(POCKETSPHINX_CFLAGS) $(SPHINXBASE_CFLAGS) -DMODELDIR=\"$(pkgdatadir)\"
