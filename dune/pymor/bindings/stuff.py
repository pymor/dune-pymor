#! /usr/bin/env python
# This file is part of the dune-pymor project:
#   https://github.com/pymor/dune-pymor
# Copyright holders: Stephan Rave, Felix Schindler
# License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

import sys

from dune.pymor.core import prepare_python_bindings, inject_lib_dune_stuff, finalize_python_bindings


if __name__ == '__main__':
    # prepare the module
    module, pybindgen_filename, config_h_filename = prepare_python_bindings(sys.argv[1:])
    # add all of libdunepymor
    module, _, _, _ = inject_lib_dune_stuff(module, config_h_filename)
    # and finally write the pybindgen .cc file
    finalize_python_bindings(module, pybindgen_filename)
