# This file is part of the dune-pymor project:
#   https://github.com/pyMor/dune-pymor
# Copyright Holders: Felix Albrecht, Stephan Rave
# License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

add_library( dunepymor-example-stationary-linear STATIC "stationarylinear.cc" )

ADD_EXECUTABLE( example_stationary_linear "stationarylinear_main.cc" ${COMMON_HEADER} )
TARGET_LINK_LIBRARIES( example_stationary_linear dunepymor-example-stationary-linear ${COMMON_LIBS} )

add_python_bindings(stationarylinearexample
                    stationarylinear_bindings_generator.py
                    stationarylinear.hh
                    dunepymor-example-stationary-linear)
