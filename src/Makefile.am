bin_PROGRAMS = 
AUTOMAKE_OPTIONS = subdir-objects
AM_CFLAGS = -I$(top_srcdir)/includes -Wall

bin_PROGRAMS += recorder_d
recorder_d_SOURCES = recorder/recorder.c recorder/rec_handlers.c
recorder_d_LDADD = -lpulse -lm
recorder_d_CFLAGS = $(AM_CFLAGS)
if DEBUG
recorder_d_CFLAGS += -DDEBUG
endif

bin_PROGRAMS += translator_d
translator_d_SOURCES = translator/translator.c
translator_d_LDADD = $(POCKETSPHINX_LIBS) $(SPHINXBASE_LIBS)
translator_d_CFLAGS = $(AM_CFLAGS) $(POCKETSPHINX_CFLAGS) $(SPHINXBASE_CFLAGS) -DMODELDIR=\"@MODELDIR@\"
