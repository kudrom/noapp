bin_PROGRAMS = 
AUTOMAKE_OPTIONS = subdir-objects
AM_CFLAGS = -I$(top_srcdir)/includes -Wall

if DEBUG
bin_PROGRAMS += recorder_d
recorder_d_SOURCES = recorder/recorder.c recorder/rec_handlers.c
recorder_d_LDADD = -lpulse -lm
recorder_d_CFLAGS = $(AM_CFLAGS) -DDEBUG
endif
