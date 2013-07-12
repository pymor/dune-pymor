# This file is part of the dune-pymor project:
#   https://github.com/pyMor/dune-pymor
# Copyright Holders: Felix Albrecht, Stephan Rave
# License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

SUBDIRS = container

noinst_LTLIBRARIES = libpymorla.la

libpymorla_la_SOURCES = \
  container.cc

libpymorla_la_LIBADD = \
  container/libpymorlacontainer.la

libpymorla_la_CPPFLAGS = ${DUNE_CPPFLAGS}

include $(top_srcdir)/am/global-rules
