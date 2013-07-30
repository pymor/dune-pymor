#! /usr/bin/env python
# This file is part of the dune-pymor project:
#   https://github.com/pyMor/dune-pymor
# Copyright Holders: Felix Albrecht, Stephan Rave
# License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

import pybindgen
from pybindgen import retval, param


def inject_StationaryDiscretizationImplementation(module, exceptions, interfaces, CONFIG_H,
                                                  name,
                                                  Traits,
                                                  template_parameters=None):
    assert(isinstance(module, pybindgen.module.Module))
    assert(isinstance(exceptions, dict))
    assert(isinstance(interfaces, dict))
    for element in interfaces:
        assert(isinstance(element, str))
        assert(len(element) > 0)
    assert(isinstance(CONFIG_H, dict))
    assert(isinstance(name, str))
    assert(len(name.strip()) > 0)
    assert(isinstance(Traits, dict))
    for key in Traits.keys():
        assert(isinstance(Traits[key], str))
        assert(len(Traits[key].strip()) > 0)
    assert('OperatorType' in Traits)
    OperatorType = Traits['OperatorType']
    assert('FunctionalType' in Traits)
    FunctionalType = Traits['FunctionalType']
    assert('VectorType' in Traits)
    VectorType = Traits['VectorType']
    if template_parameters is not None:
        if isinstance(template_parameters, str):
            assert(len(template_parameters.strip()) > 0)
            template_parameters = [ template_parameters ]
        elif isinstance(template_parameters, list):
            for element in template_parameters:
                assert(isinstance(element, str))
                assert(len(element.strip()) > 0)
    # add interface if necessary
    if not 'Dune::Pymor::StationaryDiscretizationInterfaceDynamic' in interfaces:
        (interfaces['Dune::Pymor::StationaryDiscretizationInterfaceDynamic']
         ) = module.add_cpp_namespace('Dune').add_cpp_namespace('Pymor').add_class('StationaryDiscretizationInterfaceDynamic')
    namespace = module
    namespaces = [nspace.strip() for nspace in name.split('::')[:-1]]
    name = name.split('::')[-1].strip()
    if len(namespaces) > 0:
        for nspace in namespaces:
            namespace = namespace.add_cpp_namespace(nspace)
    Class = namespace.add_class(name,
                                parent=[interfaces['Dune::Pymor::StationaryDiscretizationInterfaceDynamic'],
                                        interfaces['Dune::Pymor::Parametric']],
                                template_parameters=template_parameters)
    Class.add_method('available_operators',
                     retval('std::vector< std::string >'),
                     [], is_const=True, throw=[exceptions['PymorException']])
    Class.add_method('get_operator_and_return_ptr',
                     retval(OperatorType + ' *', caller_owns_return=True),
                     [param('const std::string', 'id')],
                     is_const=True, throw=[exceptions['PymorException']],
                     custom_name='get_operator')
    Class.add_method('available_functionals',
                     retval('std::vector< std::string >'),
                     [], is_const=True, throw=[exceptions['PymorException']])
    Class.add_method('get_functional_and_return_ptr',
                     retval(FunctionalType + ' *', caller_owns_return=True),
                     [param('const std::string', 'id')],
                     is_const=True, throw=[exceptions['PymorException']],
                     custom_name='get_functional')
    Class.add_method('create_vector_and_return_ptr',
                     retval(VectorType + ' *', caller_owns_return=True),
                     [], is_const=True, throw=[exceptions['PymorException']],
                     custom_name='create_vector')
    Class.add_method('solver_options',
                     retval('std::vector< std::string >'),
                     [], is_const=True, throw=[exceptions['PymorException']])
    Class.add_method('solver_options',
                     retval('std::string'),
                     [param('const std::string', 'context')],
                     is_const=True, throw=[exceptions['PymorException']])
    Class.add_method('solve',
                     None,
                     [param(VectorType + ' &', 'vector')],
                     is_const=True, throw=[exceptions['PymorException']])
    Class.add_method('solve',
                     None,
                     [param(VectorType + ' &', 'vector'),
                      param('Dune::Pymor::Parameter', 'mu')],
                     is_const=True, throw=[exceptions['PymorException']])
    Class.add_method('solve_and_return_ptr',
                     retval(VectorType + ' *', caller_owns_return=True),
                     [], is_const=True, throw=[exceptions['PymorException']])
    Class.add_method('solve_and_return_ptr',
                     retval(VectorType + ' *', caller_owns_return=True),
                     [param('Dune::Pymor::Parameter', 'mu')],
                     is_const=True, throw=[exceptions['PymorException']])
    Class.add_method('visualize',
                     None,
                     [param('const ' + VectorType + ' &', 'vector'),
                      param('const std::string', 'filename'),
                      param('const std::string', 'name')],
                     is_const=True, throw=[exceptions['PymorException']])
    return Class
