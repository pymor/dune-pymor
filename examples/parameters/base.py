#! /usr/bin/env python

# This file is part of the dune-pymor project:
#   https://github.com/pyMor/dune-pymor
# Copyright Holders: Felix Albrecht, Stephan Rave
# License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

import sys, os
import pybindgen
from dune.pymor.core import create_module as create_dune_module


if __name__ == '__main__':
    # prepare
    assert(len(sys.argv) > 1)
    inputdir, outputdir, includedirs = sys.argv[1], sys.argv[2], sys.argv[3:]
    includedirs = includedirs[0].split(';')
    global_name = 'parametersbaseexample'
    generator_filename = os.path.join(outputdir, global_name + '_bindings_generator.cc')
    # add dune-pymor constructs
    module, exceptions = create_dune_module(global_name, 'base.hh', add_all_of_dune_pymor=True)
    # add user code stuff
    module.add_function('createParameterType',
                        pybindgen.retval('Dune::Pymor::ParameterType *', caller_owns_return=True),
                        [pybindgen.param('std::string', 'key'),
                         pybindgen.param('int', 'value')])
#    module.add_function('createParameter',
#                        pybindgen.retval('Dune::Pymor::Parameter *', caller_owns_return=True),
#                        [pybindgen.param('std::string', 'key'),
#                         pybindgen.param('int', 'value')])
    with open(generator_filename, 'wb') as output:
        module.generate(pybindgen.FileCodeSink(output))
