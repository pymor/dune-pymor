#! /usr/bin/env python

# This file is part of the dune-pymor project:
#   https://github.com/pyMor/dune-pymor
# Copyright Holders: Felix Albrecht, Stephan Rave
# License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

import pybindgen

from dune.pymor.common import add_Exceptions
from dune.pymor.parameters.base import add_ParameterType


def create_module(module_name, include_header, add_all_of_dune_pymor=True):
    module = pybindgen.Module(module_name)
    module.add_include('"' + include_header + '"')
    # add stl container
    module.add_container('std::vector< std::string >', 'std::string', 'list')
    module.add_container('std::vector< double >', 'double', 'list')
    module.add_container('std::vector< int >', 'int', 'list')
    # add dnue-pymor
    module, exceptions = add_Exceptions(module)
    if add_all_of_dune_pymor:
        module, _ = add_ParameterType(module, exceptions)
#        module, _ = add_Parameter(module, exceptions)
    return module, exceptions
