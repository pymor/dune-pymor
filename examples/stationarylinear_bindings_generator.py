#! /usr/bin/env python
# This file is part of the dune-pymor project:
#   https://github.com/pymor/dune-pymor
# Copyright Holders: Stephan Rave, Felix Schindler
# License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

import sys
from pybindgen import param, retval

from dune.pymor.core import prepare_python_bindings, inject_lib_dune_pymor, finalize_python_bindings
from dune.pymor.discretizations import inject_StationaryDiscretizationImplementation


def inject_Example(module):
    '''injects the user code into the module'''
    namespace = module.add_cpp_namespace('Example')
    AnalyticalProblem = namespace.add_class('AnalyticalProblem')
    AnalyticalProblem.add_constructor([])
    AnalyticalProblem.add_constructor([param('const int', 'dd')])


if __name__ == '__main__':
    # prepare the module
    module, pybindgen_filename = prepare_python_bindings(sys.argv[1:])
    # add all of libdunepymor
    module, exceptions, interfaces, CONFIG_H = inject_lib_dune_pymor(module)
    # add example user code
    inject_Example(module)
    # add the users discretization
    discretization = inject_StationaryDiscretizationImplementation(
        module, exceptions, interfaces, CONFIG_H,
        'Example::SimpleDiscretization',
        Traits={'VectorType': 'Dune::Stuff::LA::CommonDenseVector< double >',
                'OperatorType': 'Dune::Pymor::Operators::LinearAffinelyDecomposedContainerBased< Dune::Pymor::Operators::CommonDense< double > >',
                'FunctionalType': 'Dune::Pymor::Functionals::LinearAffinelyDecomposedVectorBased< Dune::Stuff::LA::CommonDenseVector< double > >',
                'ProductType': 'Dune::Pymor::Operators::LinearAffinelyDecomposedContainerBased< Dune::Pymor::Operators::CommonDense< double > >'})
    # and add the custom constructor to the discretization
    discretization.add_constructor([param('const Example::AnalyticalProblem *', 'prob', transfer_ownership=True)])
    # and finally write the pybindgen .cc file
    finalize_python_bindings(module, pybindgen_filename)
