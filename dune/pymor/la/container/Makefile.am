# This file is part of the dune-pymor project:
#   https://github.com/pyMor/dune-pymor
# Copyright Holders: Felix Albrecht, Stephan Rave
# License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

containerincludedir = $(includedir)/dune/pymor/la/container
containerinclude_HEADERS = dunedynamicvector.hh eigenvector.hh interfaces.hh

noinst_LTLIBRARIES = libpymorlacontainer.la

libpymorlacontainer_la_SOURCES = \
  dunedynamicvector.cc \
  eigenvector.cc

libpymorlacontainer_la_LIBADD =

libpymorlacontainer_la_CPPFLAGS = ${DUNE_CPPFLAGS}

include $(top_srcdir)/am/global-rules
