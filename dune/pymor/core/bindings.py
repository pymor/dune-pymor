#! /usr/bin/env python
# This file is part of the dune-pymor project:
#   https://github.com/pymor/dune-pymor
# Copyright Holders: Stephan Rave, Felix Schindler
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
    module.add_container('std::vector< ' + CONFIG_H['DUNE_STUFF_SSIZE_T'] + ' >',
                         CONFIG_H['DUNE_STUFF_SSIZE_T'],
                         'list')
    module.add_container('std::vector< std::vector< double > >', 'std::vector< double >', 'list')
    # then of dune-pymor we need the exceptions first
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
    (module, interfaces['Dune::Stuff::LA::Tags::ContainerInterface']
            ) = inject_Class(module, 'Dune::Stuff::LA::Tags::ContainerInterface')
    (module, interfaces['Dune::Stuff::LA::Tags::VectorInterface']
     ) = inject_Class(module,
                     'Dune::Stuff::LA::Tags::VectorInterface',
                     interfaces['Dune::Stuff::LA::Tags::ContainerInterface'])
    (module, interfaces['Dune::Stuff::LA::Tags::MatrixInterface']
     ) = inject_Class(module,
                     'Dune::Stuff::LA::Tags::MatrixInterface',
                     interfaces['Dune::Stuff::LA::Tags::ContainerInterface'])
    module, _ = dune.pymor.la.container.inject_VectorImplementation(
        module,
        exceptions,
        interfaces,
        CONFIG_H,
        name='Dune::Stuff::LA::CommonDenseVector',
        Traits={'ThisType': 'Dune::Stuff::LA::CommonDenseVector< double >',
                'ScalarType': 'double'},
        template_parameters='double',
        provides_data=False)
    if CONFIG_H['HAVE_EIGEN']:
        module, _ = dune.pymor.la.container.inject_VectorImplementation(
            module,
            exceptions,
            interfaces,
            CONFIG_H,
            name='Dune::Stuff::LA::EigenDenseVector',
            Traits={'ThisType': 'Dune::Stuff::LA::EigenDenseVector< double >',
                    'ScalarType': 'double'},
            template_parameters='double',
            provides_data=True)
    #   and the matrices
    module, _ = dune.pymor.la.container.inject_MatrixImplementation(
        module, exceptions, interfaces, CONFIG_H,
        name='Dune::Stuff::LA::CommonDenseMatrix',
        Traits={'ThisType': 'Dune::Stuff::LA::CommonDenseMatrix< double >',
                'ScalarType': 'double'},
        template_parameters='double')
    if CONFIG_H['HAVE_EIGEN']:
        module, _ = dune.pymor.la.container.inject_MatrixImplementation(
            module, exceptions, interfaces, CONFIG_H,
            name='Dune::Stuff::LA::EigenRowMajorSparseMatrix',
            Traits={'ThisType' : 'Dune::Stuff::LA::EigenRowMajorSparseMatrix< double >',
                    'ScalarType' : 'double'},
            template_parameters='double')
    # next we add what we need of the functionals
    (module, interfaces['Dune::Pymor::Tags::FunctionalInterface']
            ) = inject_Class(module, 'Dune::Pymor::Tags::FunctionalInterface')
    (module, interfaces['Dune::Pymor::Tags::AffinelyDecomposedFunctionalInterface']
            ) = inject_Class(module, 'Dune::Pymor::Tags::AffinelyDecomposedFunctionalInterface',
                             parent=interfaces['Dune::Pymor::Tags::FunctionalInterface'])
    #   for the Dune::CommonDenseVector
    _ = dune.pymor.functionals.inject_VectorBasedImplementation(
        module, exceptions, interfaces, CONFIG_H,
        Traits={'SourceType' : 'Dune::Stuff::LA::CommonDenseVector< double >',
                'ScalarType' : 'double',
                'ContainerType' : 'Dune::Stuff::LA::CommonDenseVector< double >'},
        template_parameters='Dune::Stuff::LA::CommonDenseVector< double >')
    _ = dune.pymor.functionals.inject_LinearAffinelyDecomposedVectorBasedImplementation(
        module, exceptions, interfaces, CONFIG_H,
        Traits={'SourceType' : 'Dune::Stuff::LA::CommonDenseVector< double >',
                'ComponentType': 'Dune::Pymor::Functionals::VectorBased< Dune::Stuff::LA::CommonDenseVector< double > >',
                'FrozenType': 'Dune::Pymor::Functionals::VectorBased< Dune::Stuff::LA::CommonDenseVector< double > >',
                'ScalarType' : 'double'},
        template_parameters='Dune::Stuff::LA::CommonDenseVector< double >')
    #   and the Eigen backend
    if CONFIG_H['HAVE_EIGEN']:
        _ = dune.pymor.functionals.inject_VectorBasedImplementation(
            module, exceptions, interfaces, CONFIG_H,
            Traits={'SourceType' : 'Dune::Stuff::LA::EigenDenseVector< double >',
                    'ScalarType' : 'double',
                    'ContainerType' : 'Dune::Stuff::LA::EigenDenseVector< double >'},
            template_parameters='Dune::Stuff::LA::EigenDenseVector< double >')
        _ = dune.pymor.functionals.inject_LinearAffinelyDecomposedVectorBasedImplementation(
            module, exceptions, interfaces, CONFIG_H,
            Traits={'SourceType' : 'Dune::Stuff::LA::EigenDenseVector< double >',
                    'ComponentType': 'Dune::Pymor::Functionals::VectorBased< Dune::Stuff::LA::EigenDenseVector< double > >',
                    'FrozenType': 'Dune::Pymor::Functionals::VectorBased< Dune::Stuff::LA::EigenDenseVector< double > >',
                    'ScalarType' : 'double'},
            template_parameters='Dune::Stuff::LA::EigenDenseVector< double >')
    # next we add what we need of the operators
    (_, interfaces['Dune::Pymor::Tags::OperatorInterface']
            ) = inject_Class(module, 'Dune::Pymor::Tags::OperatorInterface')
    (_, interfaces['Dune::Pymor::Tags::AffinelyDecomposedOperatorInterface']
            ) = inject_Class(module, 'Dune::Pymor::Tags::AffinelyDecomposedOperatorInterface',
                             parent=interfaces['Dune::Pymor::Tags::OperatorInterface'])
    #   the Dune::CommonDense backend
    _, _ = dune.pymor.operators.inject_OperatorAndInverseImplementation(
        module, exceptions, interfaces, CONFIG_H,
        operator_name='Dune::Pymor::Operators::CommonDense',
        operator_Traits={'SourceType': 'Dune::Stuff::LA::CommonDenseVector< double >',
                         'RangeType': 'Dune::Stuff::LA::CommonDenseVector< double >',
                         'ScalarType': 'double',
                         'FrozenType': 'Dune::Pymor::Operators::CommonDense< double >',
                         'InverseType': 'Dune::Pymor::Operators::CommonDenseInverse< double >'},
        inverse_name='Dune::Pymor::Operators::CommonDenseInverse',
        inverse_Traits={'SourceType': 'Dune::Stuff::LA::CommonDenseVector< double >',
                        'RangeType': 'Dune::Stuff::LA::CommonDenseVector< double >',
                        'ScalarType': 'double',
                        'FrozenType': 'Dune::Pymor::Operators::CommonDenseInverse< double >',
                        'InverseType': 'Dune::Pymor::Operators::CommonDense< double >'},
        operator_template_parameters='double',
        inverse_template_parameters='double')
    _ = dune.pymor.operators.inject_LinearAffinelyDecomposedContainerBasedImplementation(
        module, exceptions, interfaces, CONFIG_H,
        Traits={'SourceType': 'Dune::Stuff::LA::CommonDenseVector< double >',
                'RangeType': 'Dune::Stuff::LA::CommonDenseVector< double >',
                'ScalarType': 'double',
                'FrozenType': 'Dune::Pymor::Operators::CommonDense< double >',
                'ComponentType': 'Dune::Pymor::Operators::CommonDense< double >',
                'InverseType': 'Dune::Pymor::Operators::CommonDenseInverse< double >'},
        template_parameters='Dune::Pymor::Operators::CommonDense< double >')
    #   and the Eigen backend
    if CONFIG_H['HAVE_EIGEN']:
        _, _ = dune.pymor.operators.inject_OperatorAndInverseImplementation(
            module, exceptions, interfaces, CONFIG_H,
            operator_name='Dune::Pymor::Operators::EigenRowMajorSparse',
            operator_Traits={'SourceType': 'Dune::Stuff::LA::EigenDenseVector< double >',
                             'RangeType': 'Dune::Stuff::LA::EigenDenseVector< double >',
                             'ScalarType': 'double',
                             'FrozenType': 'Dune::Pymor::Operators::EigenRowMajorSparse< double >',
                             'InverseType': 'Dune::Pymor::Operators::EigenRowMajorSparseInverse< double >'},
            inverse_name='Dune::Pymor::Operators::EigenRowMajorSparseInverse',
            inverse_Traits={'SourceType': 'Dune::Stuff::LA::EigenDenseVector< double >',
                            'RangeType': 'Dune::Stuff::LA::EigenDenseVector< double >',
                            'ScalarType': 'double',
                            'FrozenType': 'Dune::Pymor::Operators::EigenRowMajorSparseInverse< double >',
                            'InverseType': 'Dune::Pymor::Operators::EigenRowMajorSparse< double >'},
            operator_template_parameters='double',
            inverse_template_parameters='double')
        _ = dune.pymor.operators.inject_LinearAffinelyDecomposedContainerBasedImplementation(
            module, exceptions, interfaces, CONFIG_H,
            Traits={'SourceType': 'Dune::Stuff::LA::EigenDenseVector< double >',
                    'RangeType': 'Dune::Stuff::LA::EigenDenseVector< double >',
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
