# This file is part of the dune-pymor project:
#   https://github.com/pyMor/dune-pymor
# Copyright Holders: Felix Albrecht, Stephan Rave
# License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

discretizationsincludedir = $(includedir)/dune/pymor/discretizations
discretizationsinclude_HEADERS = interfaces.hh

noinst_LTLIBRARIES = libpymordiscretizations.la

libpymordiscretizations_la_SOURCES = \
  interfaces.cc

libpymordiscretizations_la_LIBADD =

libpymordiscretizations_la_CPPFLAGS = ${DUNE_CPPFLAGS}

include $(top_srcdir)/am/global-rules
