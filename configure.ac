# -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([NoApp], [0.1], [noapp@riseup.net])
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_AWK
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_PROG_CPP
AC_PROG_MKDIR_P

# Checks for libraries.
AC_CHECK_LIB([pulse], [pa_stream_new_with_proplist])
AC_CHECK_LIB([m], [sqrt])

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_INT16_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([sqrt strerror])

AC_CONFIG_FILES([Makefile
                 src/Makefile
                 src/recorder/Makefile])

AC_ARG_ENABLE([debug],
              [  --enable-debug    Turn on debugging],
              [case "${enableval}" in
               yes) debug=true ;;
               no)  debug=false ;;
               *) AC_MSG_ERROR([bad value ${enableval} for --enable-debug]) ;;
               esac],
               [debug=false])
AM_CONDITIONAL([DEBUG], [test x$debug = xtrue])

PKG_CHECK_MODULES(POCKETSPHINX, pocketsphinx)
PKG_CHECK_MODULES(SPHINXBASE, sphinxbase)
MODELDIR=`pkg-config --variable=modeldir pocketsphinx`
AC_SUBST(MODELDIR)
AC_OUTPUT
