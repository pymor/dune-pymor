#! /usr/bin/env python

# This file is part of the dune-pymor project:
#   https://github.com/pyMor/dune-pymor
# Copyright Holders: Felix Albrecht, Stephan Rave
# License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

import pybindgen

from dune.pymor.common import add_PymorExceptions


def create_module(module_name, include_header):
    module = pybindgen.Module(module_name)
    module.add_include('"' + include_header + '"')
    module.add_container('std::vector< double >', 'double', 'list')
    module.add_container('std::vector< int >', 'int', 'list')
    module, exceptions = add_PymorExceptions(module)
    return module, exceptions
