# This file is part of the dune-pymor project:
#   https://github.com/pymor/dune-pymor
# Copyright Holders: Felix Albrecht, Stephan Rave
# License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

pymorincludedir = $(includedir)/dune/pymor/
pymorinclude_HEADERS = 

SUBDIRDS = . common discretizations functionals functions la operators parameters

noinst_LTLIBRARIES = libpymor.la

libpymor_la_SOURCES = \
  functions.cc \
  functions/checkerboard.cc \
  functions/default.cc \
  functions/interfaces.cc \
  la/container/dunedynamic.cc \
  la/container/eigen.cc \
  operators/dunedynamic.cc \
  operators/eigen.cc \
  parameters/base.cc \
  parameters/functional.cc

libpymor_la_LIBADD = $(DUNE_LIBS) $(ALUGRID_LIBS)

libpymor_la_CPPFLAGS = $(DUNE_CPPFLAGS) $(ALUGRID_CPPFLAGS)

include $(top_srcdir)/am/global-rules
