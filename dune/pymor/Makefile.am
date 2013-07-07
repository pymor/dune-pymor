# This file is part of the dune-pymor project:
#   https://github.com/pyMor/dune-pymor
# Copyright Holders: Felix Albrecht, Stephan Rave
# License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

pymorincludedir = $(includedir)/dune/pymor
pymorinclude_HEADERS = exists.hh

SUBDIRDS = . common la operators parameters

noinst_LTLIBRARIES = libpymor.la

libpymor_la_SOURCES = \
  la/container.cc

libpymor_la_LIBADD =

include $(top_srcdir)/am/global-rules
