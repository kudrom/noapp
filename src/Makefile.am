AUTOMAKE_OPTIONS = subdir-objects
AM_CFLAGS = -I$(includedir) -Wall

bin_PROGRAMS = noapp_cmd
noapp_cmd_SOURCES = noapp/noapp_cmd.c
noapp_cmd_LDADD = $(POCKETSPHINX_LIBS) $(SPHINXBASE_LIBS)
noapp_cmd_CFLAGS = $(AM_CFLAGS) $(POCKETSPHINX_CFLAGS) $(SPHINXBASE_CFLAGS) -DMODELDIR=\"$(pkgdatadir)\"

bin_PROGRAMS += recorder_d
recorder_d_SOURCES = recorder/recorder.c recorder/rec_handles.c
recorder_d_LDADD = -lpulse -lm
recorder_d_CFLAGS = $(AM_CFLAGS)
if DEBUG
recorder_d_CFLAGS += -DDEBUG
endif

bin_PROGRAMS += recognizer_d
recognizer_d_SOURCES = recognizer/recognizer.c
recognizer_d_LDADD = $(POCKETSPHINX_LIBS) $(SPHINXBASE_LIBS)
recognizer_d_CFLAGS = $(AM_CFLAGS) $(POCKETSPHINX_CFLAGS) $(SPHINXBASE_CFLAGS) -DMODELDIR=\"$(pkgdatadir)\"
