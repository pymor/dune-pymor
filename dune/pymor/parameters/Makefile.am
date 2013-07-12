# This file is part of the dune-pymor project:
#   https://github.com/pyMor/dune-pymor
# Copyright Holders: Felix Albrecht, Stephan Rave
# License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

parametersincludedir = $(includedir)/dune/pymor
parametersinclude_HEADERS = base.hh functional.hh

noinst_LTLIBRARIES = libpymorparameters.la

libpymorparameters_la_SOURCES = \
  base.cc \
  functional.cc

libpymorparameters_la_LIBADD =

libpymorparameters_la_CPPFLAGS = ${DUNE_CPPFLAGS}

include $(top_srcdir)/am/global-rules
