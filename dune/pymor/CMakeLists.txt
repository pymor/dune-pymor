# This file is part of the dune-pymor project:
#   https://github.com/pymor/dune-pymor
# Copyright Holders: Stephan Rave, Felix Schindler
# License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

set(lib_dune_pymor_sources
    parameters/base.cc
    parameters/functional.cc
)

dune_add_library("dunepymor" ${lib_dune_pymor_sources} ADD_LIBS ${DUNE_LIBS})
target_link_dune_default_libraries(dunepymor)
