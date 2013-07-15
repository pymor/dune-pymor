# This file is part of the dune-pymor project:
#   https://github.com/pyMor/dune-pymor
# Copyright Holders: Felix Albrecht, Stephan Rave
# License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

pymorincludedir = $(includedir)/dune/pymor/
pymorinclude_HEADERS = operators.hh

SUBDIRDS = . common discretizations functionals functions la operators parameters

noinst_LTLIBRARIES = libpymor.la

libpymor_la_SOURCES = \
  discretizations/interfaces.cc \
  functionals/default.cc \
  functionals/interfaces.cc \
  la/container/dunedynamicvector.cc \
  la/container/eigenvector.cc \
  la/container/interfaces.cc \
  la/container.cc \
  operators/default.cc \
  operators/interfaces.cc \
  operators/dunedynamicmatrix.cc \
  operators/eigen.cc \
  parameters/base.cc \
  parameters/functional.cc \
  operators.cc

libpymor_la_LIBADD = $(DUNE_LIBS)

libpymor_la_CPPFLAGS = ${DUNE_CPPFLAGS}

include $(top_srcdir)/am/global-rules
