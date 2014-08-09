hmmdir = $(pkgdatadir)/hmm/en_US/hub4wsj_sc_8k
hmmd = $(srcdir)/data/hmm/en_US/hub4wsj_sc_8k
hmm_DATA = $(hmmd)/mdef $(hmmd)/means $(hmmd)/noisedict $(hmmd)/sendump $(hmmd)/transition_matrices $(hmmd)/variances $(hmmd)/feat.params

lmdir = $(pkgdatadir)/lm/turtle
lmd = $(srcdir)/data/lm
lm_DATA = $(lmd)/turtle/turtle.dic $(lmd)/turtle/turtle.DMP

if !DEBUG
include_HEADERS = include/rec_handles.h include/recorder.h
endif
SUBDIRS = . src
