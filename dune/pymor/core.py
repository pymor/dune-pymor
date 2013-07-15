#! /usr/bin/env python
# This file is part of the dune-pymor project:
#   https://github.com/pyMor/dune-pymor
# Copyright Holders: Felix Albrecht, Stephan Rave
# License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

from os.path import join
import pybindgen

import dune.pymor
import dune.pymor.common
import dune.pymor.parameters
import dune.pymor.functionals
import dune.pymor.la.container
import dune.pymor.operators


def prepare_python_bindings(argv):
    if (len(argv) < 5):
        raise IndexError('argv is expected to be at least of length 5!')
    module_name = argv[0]
    cpp_header = argv[1]
#    input_dir = argv[2]
    output_dir = argv[3]
#    include_dirs = argv[4:][0].split(';')
    pybindgen_filename = join(output_dir, module_name + '_bindings_generator.cc')
    module = pybindgen.Module(module_name)
    module.add_include('"' + cpp_header + '"')
    return module, pybindgen_filename


def inject_lib_dune_pymor(module):
    assert(isinstance(module, pybindgen.module.Module))
    interfaces = dict()
    # get config.h
    CONFIG_H = dune.pymor.CONFIG_H
    # first of all, add all the stl containers
    module.add_container('std::vector< std::string >', 'std::string', 'list')
    module.add_container('std::vector< double >', 'double', 'list')
    module.add_container('std::vector< int >', 'int', 'list')
    module.add_container('std::vector< std::vector< double > >', 'std::vector< double >', 'list')
    # then of dune-pymor we need the eceptions first
    module, exceptions = dune.pymor.common.inject_exceptions(module, CONFIG_H)
    # then all of parameters
    (module, interfaces['Dune::Pymor::ParameterType']
     ) = dune.pymor.parameters.inject_ParameterType(module, exceptions, CONFIG_H)
    (module, interfaces['Dune::Pymor::Parameter']
     ) = dune.pymor.parameters.inject_Parameter(module, exceptions, CONFIG_H)
    (module, interfaces['Dune::Pymor::Parametric']
     ) = dune.pymor.parameters.inject_Parametric(module, exceptions, CONFIG_H)
    (module, interfaces['Dune::Pymor::ParameterFunctional']
     ) = dune.pymor.parameters.inject_ParameterFunctional(module, exceptions, CONFIG_H)
    # then the interfaces from functionals
    (module, interfaces['Dune::Pymor::FunctionalInterface']
     ) = dune.pymor.functionals.inject_FunctionalInterface(
            module,
            exceptions,
            CONFIG_H,
            interfaces['Dune::Pymor::Parametric'])
    (module, interfaces['Dune::Pymor::AffinelyDecomposedFunctionalInterface']
     ) = dune.pymor.functionals.inject_AffinelyDecomposedFunctionalInterface(
            module,
            exceptions,
            CONFIG_H,
            interfaces['Dune::Pymor::FunctionalInterface'])
    # then all of la.container
    (module, interfaces['Dune::LA::VectorInterface']
     ) = dune.pymor.la.container.inject_VectorInterface(
            module,
            exceptions,
            CONFIG_H,
            interfaces['Dune::Pymor::FunctionalInterface'])
    module, _ = dune.pymor.la.container.inject_derived_from_VectorInterface(
            module,
            exceptions,
            CONFIG_H,
            interfaces['Dune::LA::VectorInterface'],
            'Dune::Pymor::LA::DuneDynamicVector')
    if CONFIG_H['HAVE_EIGEN']:
        module, _ = dune.pymor.la.container.inject_derived_from_VectorInterface(
                module,
                exceptions,
                CONFIG_H,
                interfaces['Dune::LA::VectorInterface'],
                'Dune::Pymor::LA::EigenDenseVector')
    # and build the Functionals::LinearAffinelyDecomposed for those types
    module, _ = dune.pymor.functionals.inject_specialization_of_LinearAffinelyDecomposedDefault(
            module,
            exceptions,
            CONFIG_H,
            interfaces['Dune::Pymor::AffinelyDecomposedFunctionalInterface'],
            'Dune::Pymor::LA::DuneDynamicVector',
            'Dune::Pymor::LA::DuneDynamicVector')
    if CONFIG_H['HAVE_EIGEN']:
        module, _ = dune.pymor.functionals.inject_specialization_of_LinearAffinelyDecomposedDefault(
                module,
                exceptions,
                CONFIG_H,
                interfaces['Dune::Pymor::AffinelyDecomposedFunctionalInterface'],
                'Dune::Pymor::LA::EigenDenseVector',
                'Dune::Pymor::LA::EigenDenseVector')
    # then all of operators
    (module, interfaces['Dune::Pymor::OperatorInterface']
     ) = dune.pymor.operators.inject_OperatorInterface(
            module,
            exceptions,
            CONFIG_H,
            interfaces['Dune::Pymor::Parametric'])
    (module, interfaces['Dune::Pymor::LinearOperatorInterface']
     ) = dune.pymor.operators.inject_LinearOperatorInterface(
            module,
            exceptions,
            CONFIG_H,
            interfaces['Dune::Pymor::OperatorInterface'])
    (module, interfaces['Dune::Pymor::AffinelyDecomposedOperatorInterface']
     ) = dune.pymor.operators.inject_AffinelyDecomposedOperatorInterface(
            module,
            exceptions,
            CONFIG_H,
            interfaces['Dune::Pymor::OperatorInterface'])
    module, _, _ = dune.pymor.operators.inject_derived_from_LinearOperatorInterface_with_Inverse(
            module,
            exceptions,
            CONFIG_H,
            interfaces['Dune::Pymor::LinearOperatorInterface'],
            interfaces['Dune::Pymor::OperatorInterface'],
            'Dune::Pymor::Operators::DuneDynamicMatrix',
            'Dune::Pymor::Operators::DuneDynamicMatrixInverse',
            'Dune::Pymor::LA::DuneDynamicVector',
            'Dune::Pymor::LA::DuneDynamicVector')
    if CONFIG_H['HAVE_EIGEN']:
        module, _, _ = dune.pymor.operators.inject_derived_from_LinearOperatorInterface_with_Inverse(
                module,
                exceptions,
                CONFIG_H,
                interfaces['Dune::Pymor::LinearOperatorInterface'],
                interfaces['Dune::Pymor::OperatorInterface'],
                'Dune::Pymor::Operators::EigenDenseMatrix',
                'Dune::Pymor::Operators::EigenDenseMatrixInverse',
                'Dune::Pymor::LA::EigenDenseVector',
                'Dune::Pymor::LA::EigenDenseVector')
        module, _, _ = dune.pymor.operators.inject_derived_from_LinearOperatorInterface_with_Inverse(
                module,
                exceptions,
                CONFIG_H,
                interfaces['Dune::Pymor::LinearOperatorInterface'],
                interfaces['Dune::Pymor::OperatorInterface'],
                'Dune::Pymor::Operators::EigenRowMajorSparseMatrix',
                'Dune::Pymor::Operators::EigenRowMajorSparseMatrixInverse',
                'Dune::Pymor::LA::EigenDenseVector',
                'Dune::Pymor::LA::EigenDenseVector')
    module, _ = dune.pymor.operators.inject_specialization_of_LinearAffinelyDecomposedDefault(
            module,
            exceptions,
            CONFIG_H,
            interfaces['Dune::Pymor::AffinelyDecomposedOperatorInterface'],
            'Dune::Pymor::Operators::DuneDynamicMatrix',
            'Dune::Pymor::Operators::DuneDynamicMatrixInverse',
            'Dune::Pymor::LA::DuneDynamicVector',
            'Dune::Pymor::LA::DuneDynamicVector')
    if CONFIG_H['HAVE_EIGEN']:
        module, _ = dune.pymor.operators.inject_specialization_of_LinearAffinelyDecomposedDefault(
                module,
                exceptions,
                CONFIG_H,
                interfaces['Dune::Pymor::AffinelyDecomposedOperatorInterface'],
                'Dune::Pymor::Operators::EigenDenseMatrix',
                'Dune::Pymor::Operators::EigenDenseMatrixInverse',
                'Dune::Pymor::LA::EigenDenseVector',
                'Dune::Pymor::LA::EigenDenseVector')
        module, _ = dune.pymor.operators.inject_specialization_of_LinearAffinelyDecomposedDefault(
                module,
                exceptions,
                CONFIG_H,
                interfaces['Dune::Pymor::AffinelyDecomposedOperatorInterface'],
                'Dune::Pymor::Operators::EigenRowMajorSparseMatrix',
                'Dune::Pymor::Operators::EigenRowMajorSparseMatrixInverse',
                'Dune::Pymor::LA::EigenDenseVector',
                'Dune::Pymor::LA::EigenDenseVector')
    return module, exceptions, interfaces


def finalize_python_bindings(module, pybindgen_filename):
    assert(isinstance(module, pybindgen.module.Module))
    assert(len(pybindgen_filename) > 0)
    with open(pybindgen_filename, 'wb') as pybindgen_filen:
        module.generate(pybindgen.FileCodeSink(pybindgen_filen))
