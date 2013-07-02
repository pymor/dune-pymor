#! /usr/bin/env python

import sys, os
import pybindgen
from dune.pymor.core import create_module as create_dune_module
from dune.pymor.la.container import add_VectorInterface as add_dune_Vector


if __name__ == '__main__':
    # prepare
    assert(len(sys.argv) > 1)
    inputdir, outputdir, includedirs = sys.argv[1], sys.argv[2], sys.argv[3:]
    includedirs = includedirs[0].split(';')
    generator_filename = os.path.join(outputdir, 'dunepymor_container_example.cc')
    # add dune-pymor constructs
    module = create_dune_module('lacontainerexample', 'container.hh')
    module, _ = add_dune_Vector(module, 'Dune::Pymor::LA::DuneDynamicVector')
    # add user code stuff
    module.add_function('createVector',
                        pybindgen.retval('Dune::Pymor::LA::DuneDynamicVector *', caller_owns_return=True),
                        [pybindgen.param('int', 'ss')])
    with open(generator_filename, 'wb') as output:
        module.generate(pybindgen.FileCodeSink(output))
