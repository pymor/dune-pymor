#! /usr/bin/env python
# This file is part of the dune-pymor project:
#   https://github.com/pymor/dune-pymor
# Copyright Holders: Felix Albrecht, Stephan Rave
# License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

from os.path import join
import pybindgen

import dune.pymor
import dune.pymor.common
import dune.pymor.parameters
import dune.pymor.la.container
import dune.pymor.functionals
import dune.pymor.operators


def prepare_python_bindings(argv):
    if (len(argv) < 5):
        raise IndexError('argv is expected to be at least of length 5!')
    # I do not remember, why we need 4 arguments if we do not use the 3rd and 4th one
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

    def inject_Class(module, name, parent=None):
        namespace = module
        namespaces = [nspace.strip() for nspace in name.split('::')[:-1]]
        name = name.split('::')[-1].strip()
        if len(namespaces) > 0:
            for nspace in namespaces:
                namespace = namespace.add_cpp_namespace(nspace)
        if parent is not None:
            return module, namespace.add_class(name, parent=parent)
        else:
            return module, namespace.add_class(name)

    # first of all, add all the stl containers
    module.add_container('std::vector< std::string >', 'std::string', 'list')
    module.add_container('std::vector< double >', 'double', 'list')
    module.add_container('std::vector< ' + CONFIG_H['DUNE_PYMOR_SSIZE_T'] + ' >',
                         CONFIG_H['DUNE_PYMOR_SSIZE_T'],
                         'list')
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
     ) = dune.pymor.parameters.inject_ParameterFunctional(module, exceptions, interfaces, CONFIG_H)
    # then what we need of la.container
    #   the vectors
    (module, interfaces['Dune::Pymor::LA::ContainerInterfaceDynamic']
     ) = inject_Class(module, 'Dune::Pymor::LA::ContainerInterfaceDynamic')
    (module, interfaces['Dune::Pymor::LA::VectorInterfaceDynamic']
     ) = inject_Class(module,
                      'Dune::Pymor::LA::VectorInterfaceDynamic',
                      interfaces['Dune::Pymor::LA::ContainerInterfaceDynamic'])
    (module, interfaces['Dune::Pymor::LA::MatrixInterfaceDynamic']
     ) = inject_Class(module,
                      'Dune::Pymor::LA::MatrixInterfaceDynamic',
                      interfaces['Dune::Pymor::LA::ContainerInterfaceDynamic'])
    module, _ = dune.pymor.la.container.inject_VectorImplementation(
        module,
        exceptions,
        interfaces,
        CONFIG_H,
        name='Dune::Pymor::LA::DuneDynamicVector',
        Traits={'ThisType': 'Dune::Pymor::LA::DuneDynamicVector< double >',
                'ScalarType': 'double'},
        template_parameters='double')
    if CONFIG_H['HAVE_EIGEN']:
        module, _ = dune.pymor.la.container.inject_VectorImplementation(
            module,
            exceptions,
            interfaces,
            CONFIG_H,
            name='Dune::Pymor::LA::EigenDenseVector',
            Traits={'ThisType': 'Dune::Pymor::LA::EigenDenseVector< double >',
                    'ScalarType': 'double'},
            template_parameters='double')
    #   and the matrices
    module, _ = dune.pymor.la.container.inject_MatrixImplementation(
        module, exceptions, interfaces, CONFIG_H,
        name='Dune::Pymor::LA::DuneDynamicMatrix',
        Traits={'ThisType': 'Dune::Pymor::LA::DuneDynamicMatrix< double >',
                'ScalarType': 'double'},
        template_parameters='double')
    if CONFIG_H['HAVE_EIGEN']:
        module, _ = dune.pymor.la.container.inject_MatrixImplementation(
            module, exceptions, interfaces, CONFIG_H,
            name='Dune::Pymor::LA::EigenRowMajorSparseMatrix',
            Traits={'ThisType' : 'Dune::Pymor::LA::EigenRowMajorSparseMatrix< double >',
                    'ScalarType' : 'double'},
            template_parameters='double')
    # next we add what we need of the functionals
    (module, interfaces['Dune::Pymor::FunctionalInterfaceDynamic']
     ) = inject_Class(module, 'Dune::Pymor::FunctionalInterfaceDynamic')
    (module, interfaces['Dune::Pymor::AffinelyDecomposedFunctionalInterfaceDynamic']
     ) = inject_Class(module, 'Dune::Pymor::AffinelyDecomposedFunctionalInterfaceDynamic',
                      parent=interfaces['Dune::Pymor::FunctionalInterfaceDynamic'])
    #   for the Dune::DynamicVector backend
    _ = dune.pymor.functionals.inject_VectorBasedImplementation(
        module, exceptions, interfaces, CONFIG_H,
        Traits={'SourceType' : 'Dune::Pymor::LA::DuneDynamicVector< double >',
                'ScalarType' : 'double',
                'ContainerType' : 'Dune::Pymor::LA::DuneDynamicVector< double >'},
        template_parameters='Dune::Pymor::LA::DuneDynamicVector< double >')
    _ = dune.pymor.functionals.inject_LinearAffinelyDecomposedVectorBasedImplementation(
        module, exceptions, interfaces, CONFIG_H,
        Traits={'SourceType' : 'Dune::Pymor::LA::DuneDynamicVector< double >',
                'ComponentType': 'Dune::Pymor::Functionals::VectorBased< Dune::Pymor::LA::DuneDynamicVector< double > >',
                'FrozenType': 'Dune::Pymor::Functionals::VectorBased< Dune::Pymor::LA::DuneDynamicVector< double > >',
                'ScalarType' : 'double'},
        template_parameters='Dune::Pymor::LA::DuneDynamicVector< double >')
    #   and the Eigen backend
    if CONFIG_H['HAVE_EIGEN']:
        _ = dune.pymor.functionals.inject_VectorBasedImplementation(
            module, exceptions, interfaces, CONFIG_H,
            Traits={'SourceType' : 'Dune::Pymor::LA::EigenDenseVector< double >',
                    'ScalarType' : 'double',
                    'ContainerType' : 'Dune::Pymor::LA::EigenDenseVector< double >'},
            template_parameters='Dune::Pymor::LA::EigenDenseVector< double >')
        _ = dune.pymor.functionals.inject_LinearAffinelyDecomposedVectorBasedImplementation(
            module, exceptions, interfaces, CONFIG_H,
            Traits={'SourceType' : 'Dune::Pymor::LA::EigenDenseVector< double >',
                    'ComponentType': 'Dune::Pymor::Functionals::VectorBased< Dune::Pymor::LA::EigenDenseVector< double > >',
                    'FrozenType': 'Dune::Pymor::Functionals::VectorBased< Dune::Pymor::LA::EigenDenseVector< double > >',
                    'ScalarType' : 'double'},
            template_parameters='Dune::Pymor::LA::EigenDenseVector< double >')
    # next we add what we need of the operators
    (_, interfaces['Dune::Pymor::OperatorInterfaceDynamic']
     ) = inject_Class(module, 'Dune::Pymor::OperatorInterfaceDynamic')
    (_, interfaces['Dune::Pymor::AffinelyDecomposedOperatorInterfaceDynamic']
     ) = inject_Class(module, 'Dune::Pymor::AffinelyDecomposedOperatorInterfaceDynamic',
                      parent=interfaces['Dune::Pymor::OperatorInterfaceDynamic'])
    #   the Dune::DynamicMatrix backend
    _, _ = dune.pymor.operators.inject_OperatorAndInverseImplementation(
        module, exceptions, interfaces, CONFIG_H,
        operator_name='Dune::Pymor::Operators::DuneDynamic',
        operator_Traits={'SourceType': 'Dune::Pymor::LA::DuneDynamicVector< double >',
                         'RangeType': 'Dune::Pymor::LA::DuneDynamicVector< double >',
                         'ScalarType': 'double',
                         'FrozenType': 'Dune::Pymor::Operators::DuneDynamic< double >',
                         'InverseType': 'Dune::Pymor::Operators::DuneDynamicInverse< double >'},
        inverse_name='Dune::Pymor::Operators::DuneDynamicInverse',
        inverse_Traits={'SourceType': 'Dune::Pymor::LA::DuneDynamicVector< double >',
                        'RangeType': 'Dune::Pymor::LA::DuneDynamicVector< double >',
                        'ScalarType': 'double',
                        'FrozenType': 'Dune::Pymor::Operators::DuneDynamicInverse< double >',
                        'InverseType': 'Dune::Pymor::Operators::DuneDynamic< double >'},
        operator_template_parameters='double',
        inverse_template_parameters='double')
    _ = dune.pymor.operators.inject_LinearAffinelyDecomposedContainerBasedImplementation(
        module, exceptions, interfaces, CONFIG_H,
        Traits={'SourceType': 'Dune::Pymor::LA::DuneDynamicVector< double >',
                'RangeType': 'Dune::Pymor::LA::DuneDynamicVector< double >',
                'ScalarType': 'double',
                'FrozenType': 'Dune::Pymor::Operators::DuneDynamic< double >',
                'ComponentType': 'Dune::Pymor::Operators::DuneDynamic< double >',
                'InverseType': 'Dune::Pymor::Operators::DuneDynamicInverse< double >'},
        template_parameters='Dune::Pymor::Operators::DuneDynamic< double >')
    #   and the Eigen backend
    if CONFIG_H['HAVE_EIGEN']:
        _, _ = dune.pymor.operators.inject_OperatorAndInverseImplementation(
            module, exceptions, interfaces, CONFIG_H,
            operator_name='Dune::Pymor::Operators::EigenRowMajorSparse',
            operator_Traits={'SourceType': 'Dune::Pymor::LA::EigenDenseVector< double >',
                             'RangeType': 'Dune::Pymor::LA::EigenDenseVector< double >',
                             'ScalarType': 'double',
                             'FrozenType': 'Dune::Pymor::Operators::EigenRowMajorSparse< double >',
                             'InverseType': 'Dune::Pymor::Operators::EigenRowMajorSparseInverse< double >'},
            inverse_name='Dune::Pymor::Operators::EigenRowMajorSparseInverse',
            inverse_Traits={'SourceType': 'Dune::Pymor::LA::EigenDenseVector< double >',
                            'RangeType': 'Dune::Pymor::LA::EigenDenseVector< double >',
                            'ScalarType': 'double',
                            'FrozenType': 'Dune::Pymor::Operators::EigenRowMajorSparseInverse< double >',
                            'InverseType': 'Dune::Pymor::Operators::EigenRowMajorSparse< double >'},
            operator_template_parameters='double',
            inverse_template_parameters='double')
        _ = dune.pymor.operators.inject_LinearAffinelyDecomposedContainerBasedImplementation(
            module, exceptions, interfaces, CONFIG_H,
            Traits={'SourceType': 'Dune::Pymor::LA::EigenDenseVector< double >',
                    'RangeType': 'Dune::Pymor::LA::EigenDenseVector< double >',
                    'ScalarType': 'double',
                    'FrozenType': 'Dune::Pymor::Operators::EigenRowMajorSparse< double >',
                    'ComponentType': 'Dune::Pymor::Operators::EigenRowMajorSparse< double >',
                    'InverseType': 'Dune::Pymor::Operators::EigenRowMajorSparseInverse< double >'},
            template_parameters='Dune::Pymor::Operators::EigenRowMajorSparse< double >')

    return module, exceptions, interfaces, CONFIG_H


def finalize_python_bindings(module, pybindgen_filename):
    assert(isinstance(module, pybindgen.module.Module))
    assert(len(pybindgen_filename) > 0)
    with open(pybindgen_filename, 'wb') as pybindgen_filen:
        module.generate(pybindgen.FileCodeSink(pybindgen_filen))
