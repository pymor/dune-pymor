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
    #input_dir = argv[2]
    output_dir = argv[3]
    #include_dirs = argv[4:][0].split(';')
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
    CommonDenseVector = 'Dune::Stuff::LA::CommonDenseVector< double >'
    module, _ = dune.pymor.la.container.inject_VectorImplementation(
        module,
        exceptions,
        interfaces,
        CONFIG_H,
        name='Dune::Stuff::LA::CommonDenseVector',
        Traits={'ThisType': CommonDenseVector,
                'ScalarType': 'double'},
        template_parameters='double',
        provides_data=False)
    if CONFIG_H['HAVE_EIGEN']:
        EigenDenseVector = 'Dune::Stuff::LA::EigenDenseVector< double >'
        EigenMappedDenseVector = 'Dune::Stuff::LA::EigenMappedDenseVector< double >'
        module, _ = dune.pymor.la.container.inject_VectorImplementation(
            module,
            exceptions,
            interfaces,
            CONFIG_H,
            name='Dune::Stuff::LA::EigenDenseVector',
            Traits={'ThisType': EigenDenseVector,
                    'ScalarType': 'double'},
            template_parameters='double',
            provides_data=True)
        module, _ = dune.pymor.la.container.inject_VectorImplementation(
            module,
            exceptions,
            interfaces,
            CONFIG_H,
            name='Dune::Stuff::LA::EigenMappedDenseVector',
            Traits={'ThisType': EigenMappedDenseVector,
                    'ScalarType': 'double'},
            template_parameters='double',
            provides_data=False)
    if CONFIG_H['HAVE_DUNE_ISTL']:
        IstlDenseVector = 'Dune::Stuff::LA::IstlDenseVector< double >'
        module, _ = dune.pymor.la.container.inject_VectorImplementation(
            module,
            exceptions,
            interfaces,
            CONFIG_H,
            name='Dune::Stuff::LA::IstlDenseVector',
            Traits={'ThisType': IstlDenseVector,
                    'ScalarType': 'double'},
            template_parameters='double',
            provides_data=False)
    #   and the matrices
    CommonDenseMatrix = 'Dune::Stuff::LA::CommonDenseMatrix< double >'
    module, _ = dune.pymor.la.container.inject_MatrixImplementation(
        module, exceptions, interfaces, CONFIG_H,
        name='Dune::Stuff::LA::CommonDenseMatrix',
        Traits={'ThisType': CommonDenseMatrix,
                'ScalarType': 'double'},
        template_parameters='double')
    if CONFIG_H['HAVE_EIGEN']:
        EigenDenseMatrix = 'Dune::Stuff::LA::EigenDenseMatrix< double >'
        EigenRowMajorSparseMatrix = 'Dune::Stuff::LA::EigenRowMajorSparseMatrix< double >'
        module, _ = dune.pymor.la.container.inject_MatrixImplementation(
            module, exceptions, interfaces, CONFIG_H,
            name='Dune::Stuff::LA::EigenDenseMatrix',
            Traits={'ThisType' : EigenDenseMatrix,
                    'ScalarType' : 'double'},
            template_parameters='double')
        module, _ = dune.pymor.la.container.inject_MatrixImplementation(
            module, exceptions, interfaces, CONFIG_H,
            name='Dune::Stuff::LA::EigenRowMajorSparseMatrix',
            Traits={'ThisType' : EigenRowMajorSparseMatrix,
                    'ScalarType' : 'double'},
            template_parameters='double')
    if CONFIG_H['HAVE_DUNE_ISTL']:
        IstlRowMajorSparseMatrix = 'Dune::Stuff::LA::IstlRowMajorSparseMatrix< double >'
        module, _ = dune.pymor.la.container.inject_MatrixImplementation(
            module, exceptions, interfaces, CONFIG_H,
            name='Dune::Stuff::LA::IstlRowMajorSparseMatrix',
            Traits={'ThisType' : IstlRowMajorSparseMatrix,
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
        Traits={'SourceType' : CommonDenseVector,
                'ScalarType' : 'double',
                'ContainerType' : CommonDenseVector},
        template_parameters=CommonDenseVector)
    _ = dune.pymor.functionals.inject_LinearAffinelyDecomposedVectorBasedImplementation(
        module, exceptions, interfaces, CONFIG_H,
        Traits={'SourceType' : CommonDenseVector,
                'ComponentType': 'Dune::Pymor::Functionals::VectorBased< ' + CommonDenseVector + ' >',
                'FrozenType': 'Dune::Pymor::Functionals::VectorBased< ' + CommonDenseVector + ' >',
                'ScalarType' : 'double'},
        template_parameters=CommonDenseVector)
    #   and the Eigen backend
    if CONFIG_H['HAVE_EIGEN']:
        _ = dune.pymor.functionals.inject_VectorBasedImplementation(
            module, exceptions, interfaces, CONFIG_H,
            Traits={'SourceType' : EigenDenseVector,
                    'ScalarType' : 'double',
                    'ContainerType' : EigenDenseVector},
            template_parameters=EigenDenseVector)
        _ = dune.pymor.functionals.inject_LinearAffinelyDecomposedVectorBasedImplementation(
            module, exceptions, interfaces, CONFIG_H,
            Traits={'SourceType' : EigenDenseVector,
                    'ComponentType': 'Dune::Pymor::Functionals::VectorBased< ' + EigenDenseVector + ' >',
                    'FrozenType': 'Dune::Pymor::Functionals::VectorBased< ' + EigenDenseVector + ' >',
                    'ScalarType' : 'double'},
            template_parameters=EigenDenseVector)
        _ = dune.pymor.functionals.inject_VectorBasedImplementation(
            module, exceptions, interfaces, CONFIG_H,
            Traits={'SourceType' : EigenMappedDenseVector,
                    'ScalarType' : 'double',
                    'ContainerType' : EigenMappedDenseVector},
            template_parameters=EigenMappedDenseVector)
        _ = dune.pymor.functionals.inject_LinearAffinelyDecomposedVectorBasedImplementation(
            module, exceptions, interfaces, CONFIG_H,
            Traits={'SourceType' : EigenMappedDenseVector,
                    'ComponentType': 'Dune::Pymor::Functionals::VectorBased< ' + EigenMappedDenseVector + ' >',
                    'FrozenType': 'Dune::Pymor::Functionals::VectorBased< ' + EigenMappedDenseVector + ' >',
                    'ScalarType' : 'double'},
            template_parameters=EigenMappedDenseVector)
    if CONFIG_H['HAVE_DUNE_ISTL']:
        _ = dune.pymor.functionals.inject_VectorBasedImplementation(
            module, exceptions, interfaces, CONFIG_H,
            Traits={'SourceType' : IstlDenseVector,
                    'ScalarType' : 'double',
                    'ContainerType' : IstlDenseVector},
            template_parameters=IstlDenseVector)
        _ = dune.pymor.functionals.inject_LinearAffinelyDecomposedVectorBasedImplementation(
            module, exceptions, interfaces, CONFIG_H,
            Traits={'SourceType' : IstlDenseVector,
                    'ComponentType': 'Dune::Pymor::Functionals::VectorBased< ' + IstlDenseVector + ' >',
                    'FrozenType': 'Dune::Pymor::Functionals::VectorBased< ' + IstlDenseVector + ' >',
                    'ScalarType' : 'double'},
            template_parameters=IstlDenseVector)
    # next we add what we need of the operators
    (_, interfaces['Dune::Pymor::Tags::OperatorInterface']
            ) = inject_Class(module, 'Dune::Pymor::Tags::OperatorInterface')
    (_, interfaces['Dune::Pymor::Tags::AffinelyDecomposedOperatorInterface']
            ) = inject_Class(module, 'Dune::Pymor::Tags::AffinelyDecomposedOperatorInterface',
                             parent=interfaces['Dune::Pymor::Tags::OperatorInterface'])
    BaseOperatorName='Dune::Pymor::Operators::MatrixBasedDefault'
    BaseOperatorInverseName='Dune::Pymor::Operators::MatrixBasedInverseDefault'
    def inject_operator_inverse_combo(matrix, vector):
        _, _ = dune.pymor.operators.inject_OperatorAndInverseImplementation(
            module, exceptions, interfaces, CONFIG_H,
            operator_name=BaseOperatorName,
            operator_Traits={'SourceType': vector,
                             'RangeType': vector,
                             'ScalarType': 'double',
                             'FrozenType': BaseOperatorName + '< ' + matrix + ', ' + vector + ' >',
                             'InverseType': BaseOperatorInverseName + '< ' + matrix + ', ' + vector + ' >'},
            inverse_name=BaseOperatorInverseName,
            inverse_Traits={'SourceType': vector,
                            'RangeType': vector,
                            'ScalarType': 'double',
                            'FrozenType': BaseOperatorInverseName + '< ' + matrix + ', ' + vector + ' >',
                            'InverseType': BaseOperatorName + '< ' + matrix + ', ' + vector + ' >'},
            operator_template_parameters=(matrix, vector),
            inverse_template_parameters=(matrix, vector))
    def inject_affinelydecomposed_operator(matrix, vector):
        _ = dune.pymor.operators.inject_LinearAffinelyDecomposedContainerBasedImplementation(
            module, exceptions, interfaces, CONFIG_H,
            Traits={'SourceType': vector,
                    'RangeType': vector,
                    'ScalarType': 'double',
                    'FrozenType': BaseOperatorName + '< ' + matrix + ', ' + vector + ' >',
                    'ComponentType': BaseOperatorName + '< ' + matrix + ', ' + vector + ' >',
                    'InverseType': BaseOperatorInverseName + '< ' + matrix + ', ' + vector + ' >'},
            template_parameters=(matrix, vector))
    #   the Dune::CommonDense backend
    inject_operator_inverse_combo(CommonDenseMatrix, CommonDenseVector)
    inject_affinelydecomposed_operator(CommonDenseMatrix, CommonDenseVector)
    #   and the Eigen backend
    if CONFIG_H['HAVE_EIGEN']:
        inject_operator_inverse_combo(EigenDenseMatrix, EigenDenseVector)
        inject_operator_inverse_combo(EigenDenseMatrix, EigenMappedDenseVector)
        inject_operator_inverse_combo(EigenRowMajorSparseMatrix, EigenDenseVector)
        inject_operator_inverse_combo(EigenRowMajorSparseMatrix, EigenMappedDenseVector)
        inject_affinelydecomposed_operator(EigenDenseMatrix, EigenDenseVector)
        inject_affinelydecomposed_operator(EigenDenseMatrix, EigenMappedDenseVector)
        inject_affinelydecomposed_operator(EigenRowMajorSparseMatrix, EigenDenseVector)
        inject_affinelydecomposed_operator(EigenRowMajorSparseMatrix, EigenMappedDenseVector)
    if CONFIG_H['HAVE_DUNE_ISTL']:
        inject_operator_inverse_combo(IstlRowMajorSparseMatrix, IstlDenseVector)
        inject_affinelydecomposed_operator(IstlRowMajorSparseMatrix, IstlDenseVector)

    return module, exceptions, interfaces, CONFIG_H


def finalize_python_bindings(module, pybindgen_filename):
    assert(isinstance(module, pybindgen.module.Module))
    assert(len(pybindgen_filename) > 0)
    with open(pybindgen_filename, 'wb') as pybindgen_filen:
        module.generate(pybindgen.FileCodeSink(pybindgen_filen))
