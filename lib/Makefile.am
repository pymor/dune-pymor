# This file is part of the dune-pymor project:
#   https://github.com/pymor/dune-pymor
# Copyright Holders: Felix Albrecht, Stephan Rave
# License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

dunemodulelibdir=$(libdir)

#the dune-pymor library
lib_LTLIBRARIES = libdunepymor.la

# this is just a renaming of libpymor to prevent name clashes
libdunepymor_la_SOURCES =
# This forces automake to use the C++ linker
# (see the automake manual, section "Libtool Convenience Libraries")
nodist_EXTRA_libdunepymor_la_SOURCES = dummy.cc
sourcescheck_DUMMY = dummy.cc
libdunepymor_la_LIBADD = ../dune/pymor/libpymor.la

include $(top_srcdir)/am/global-rules
