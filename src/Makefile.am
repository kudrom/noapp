AUTOMAKE_OPTIONS = subdir-objects
AM_CFLAGS = -I$(includedir) -Wall $(JOURNAL_CFLAGS)
AM_LIBS = $(JOURNAL_LIBS)
SOURCES = utils/utils.c

bin_PROGRAMS = noapp
noapp_SOURCES = noapp_src/noapp.c $(SOURCES)
noapp_LDADD = $(AM_LIBS) $(POCKETSPHINX_LIBS) $(SPHINXBASE_LIBS)
noapp_CFLAGS = $(AM_CFLAGS) $(POCKETSPHINX_CFLAGS) $(SPHINXBASE_CFLAGS) -DMODELDIR=\"$(pkgdatadir)\"
if DEBUG
noapp_CFLAGS += -DDEBUG
endif

bin_PROGRAMS += recorder
recorder_SOURCES = recorder_src/recorder.c recorder_src/rec_handles.c $(SOURCES)
recorder_LDADD = $(AM_LIBS) -lpulse -lm
recorder_CFLAGS = $(AM_CFLAGS)
if DEBUG
recorder_CFLAGS += -DDEBUG
endif

bin_PROGRAMS += recognizer
recognizer_SOURCES = recognizer_src/recognizer.c $(SOURCES)
recognizer_LDADD = $(AM_LIBS) $(POCKETSPHINX_LIBS) $(SPHINXBASE_LIBS)
recognizer_CFLAGS = $(AM_CFLAGS) $(POCKETSPHINX_CFLAGS) $(SPHINXBASE_CFLAGS) -DMODELDIR=\"$(pkgdatadir)\"
