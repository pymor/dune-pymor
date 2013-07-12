# This file is part of the dune-pymor project:
#   https://github.com/pyMor/dune-pymor
# Copyright Holders: Felix Albrecht, Stephan Rave
# License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

SUBDIRDS = . common functions la operators parameters

noinst_LTLIBRARIES = libpymor.la

libpymor_la_SOURCES = \
  la/container.cc \
  parameters/base.cc \
  operators.cc \
  operators/dunedynamic.cc \
  operators/eigen.cc \
  parameters/base.cc

libpymor_la_LIBADD =

libpymor_la_CPPFLAGS = ${DUNE_CPPFLAGS}

include $(top_srcdir)/am/global-rules
