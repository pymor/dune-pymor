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
    # then what we need of la.container
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
        'Dune::Pymor::LA::DuneDynamicVector',
        {'ThisType' : 'Dune::Pymor::LA::DuneDynamicVector< double >',
         'ScalarType' : 'double'},
         'double')
    if CONFIG_H['HAVE_EIGEN']:
        module, _ = dune.pymor.la.container.inject_VectorImplementation(
            module,
            exceptions,
            interfaces,
            CONFIG_H,
            'Dune::Pymor::LA::EigenDenseVector',
            {'ThisType' : 'Dune::Pymor::LA::EigenDenseVector< double >',
             'ScalarType' : 'double'},
            'double')

    return module, exceptions, interfaces


def finalize_python_bindings(module, pybindgen_filename):
    assert(isinstance(module, pybindgen.module.Module))
    assert(len(pybindgen_filename) > 0)
    with open(pybindgen_filename, 'wb') as pybindgen_filen:
        module.generate(pybindgen.FileCodeSink(pybindgen_filen))
