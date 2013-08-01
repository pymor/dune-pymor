#! /usr/bin/env python
# This file is part of the dune-pymor project:
#   https://github.com/pyMor/dune-pymor
# Copyright Holders: Felix Albrecht, Stephan Rave
# License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

import pybindgen
from pybindgen import retval, param


def inject_VectorImplementation(module, exceptions, interfaces, CONFIG_H, name, Traits, template_parameters=None):
    assert(isinstance(module, pybindgen.module.Module))
    assert(isinstance(exceptions, dict))
    assert(isinstance(interfaces, dict))
    for element in interfaces:
        assert(isinstance(element, str))
        assert(len(element) > 0)
        assert(isinstance(CONFIG_H, dict))
    assert(len(name.strip()) > 0)
    assert(isinstance(Traits, dict))
    for key in Traits.keys():
        assert(isinstance(Traits[key], str))
        assert(len(Traits[key].strip()) > 0)
    assert('ThisType' in Traits)
    ThisType = Traits['ThisType']
    assert('ScalarType' in Traits)
    ScalarType = Traits['ScalarType']
    if template_parameters is not None:
        if isinstance(template_parameters, str):
            assert(len(template_parameters.strip()) > 0)
            template_parameters = [ template_parameters ]
        elif isinstance(template_parameters, list):
            for element in template_parameters:
                assert(isinstance(element, str))
                assert(len(element.strip()) > 0)
    namespace = module
    namespaces = [nspace.strip() for nspace in name.split('::')[:-1]]
    name = name.split('::')[-1].strip()
    if len(namespaces) > 0:
        for nspace in namespaces:
            namespace = namespace.add_cpp_namespace(nspace)
    Class = namespace.add_class(name,
                                parent=interfaces['Dune::Pymor::LA::VectorInterfaceDynamic'],
                                template_parameters=template_parameters)
    Class.add_constructor([])
    Class.add_constructor([param('int', 'size')])
    Class.add_constructor([param('int', 'size'), param(ScalarType, 'value')])
    # what we want from ContainerInterface
    Class.add_method('type_this', retval('std::string'), [], is_const=True, is_static=True,
                     throw=[exceptions['PymorException'], exceptions['DuneException']])
    Class.add_method('copy_and_return_ptr',
                     retval(ThisType + ' *', caller_owns_return=True),
                     [],
                     is_const=True,
                     throw=[exceptions['PymorException']],
                     custom_name='copy')
    Class.add_method('scal',
                     None,
                     [param('const ' + ScalarType + ' &', 'alpha')],
                     throw=[exceptions['PymorException']])
    Class.add_method('axpy',
                     None,
                     [param('const ' + ScalarType + ' &', 'alpha'),
                      param('const ' + ThisType + ' &', 'xx')],
                     throw=[exceptions['PymorException']])
    # what we want from VectorInterface
    Class.add_method('dim',
                     retval('unsigned int'),
                     [],
                     is_const=True,
                     throw=[exceptions['PymorException']])
    Class.add_method('almost_equal',
                     retval('bool'),
                     [param('const ' + ThisType + ' &', 'other')],
                     is_const=True,
                     throw=[exceptions['PymorException']])
    Class.add_method('almost_equal',
                     retval('bool'),
                     [param('const ' + ThisType + ' &', 'other'),
                      param('const ' + ScalarType, 'epsilon')],
                     is_const=True,
                     throw=[exceptions['PymorException']])
    Class.add_method('dot',
                     retval(ScalarType),
                     [param('const ' + ThisType + ' &', 'other')],
                     is_const=True,
                     throw=[exceptions['PymorException']])
    Class.add_method('l1_norm', retval(ScalarType), [], is_const=True, throw=[exceptions['PymorException']])
    Class.add_method('l2_norm', retval(ScalarType), [], is_const=True, throw=[exceptions['PymorException']])
    Class.add_method('sup_norm', retval(ScalarType), [], is_const=True, throw=[exceptions['PymorException']])
    Class.add_method('components',
                     retval('std::vector< ' + ScalarType + ' >'),
                     [param('const std::vector< int > &', 'component_indices')],
                     is_const=True,
                     throw=[exceptions['PymorException']])
    Class.add_method('amax',
                     retval('std::vector< ' + ScalarType + ' >'),
                     [],
                     is_const=True,
                     throw=[exceptions['PymorException']])
    Class.add_method('add',
                     None,
                     [param('const ' + ThisType + ' &', 'other'),
                      param(ThisType + ' &', 'result')],
                     is_const=True,
                     throw=[exceptions['PymorException']])
    Class.add_method('add_and_return_ptr',
                     retval(ThisType + ' *', caller_owns_return=True),
                     [param('const ' + ThisType + ' &', 'other')],
                     is_const=True,
                     throw=[exceptions['PymorException']],
                     custom_name='add')
    Class.add_method('iadd',
                     None,
                     [param('const ' + ThisType + ' &', 'other')],
                     throw=[exceptions['PymorException']])
    Class.add_method('sub',
                     None,
                     [param('const ' + ThisType + ' &', 'other'),
                      param(ThisType + ' &', 'result')],
                     is_const=True,
                     throw=[exceptions['PymorException']])
    Class.add_method('sub_and_return_ptr',
                     retval(ThisType + ' *', caller_owns_return=True),
                     [param('const ' + ThisType + ' &', 'other')],
                     is_const=True,
                     throw=[exceptions['PymorException']],
                     custom_name='sub')
    Class.add_method('isub',
                     None,
                     [param('const ' + ThisType + ' &', 'other')],
                     throw=[exceptions['PymorException']])

    return module, Class


def inject_MatrixImplementation(module, exceptions, interfaces, CONFIG_H, name, Traits, template_parameters=None):
    assert(isinstance(module, pybindgen.module.Module))
    assert(isinstance(exceptions, dict))
    assert(isinstance(interfaces, dict))
    for element in interfaces:
        assert(isinstance(element, str))
        assert(len(element) > 0)
    assert(isinstance(CONFIG_H, dict))
    assert(len(name.strip()) > 0)
    assert(isinstance(Traits, dict))
    for key in Traits.keys():
        assert(isinstance(Traits[key], str))
        assert(len(Traits[key].strip()) > 0)
    assert('ThisType' in Traits)
    ThisType = Traits['ThisType']
    assert('ScalarType' in Traits)
    ScalarType = Traits['ScalarType']
    if template_parameters is not None:
        if isinstance(template_parameters, str):
            assert(len(template_parameters.strip()) > 0)
            template_parameters = [ template_parameters ]
        elif isinstance(template_parameters, list):
            for element in template_parameters:
                assert(isinstance(element, str))
                assert(len(element.strip()) > 0)
    namespace = module
    namespaces = [nspace.strip() for nspace in name.split('::')[:-1]]
    name = name.split('::')[-1].strip()
    if len(namespaces) > 0:
        for nspace in namespaces:
            namespace = namespace.add_cpp_namespace(nspace)
    Class = namespace.add_class(name,
                                parent=interfaces['Dune::Pymor::LA::MatrixInterfaceDynamic'],
                                template_parameters=template_parameters)
    Class.add_constructor([])
    # what we want from ContainerInterface
    Class.add_method('type_this', retval('std::string'), [], is_const=True, is_static=True,
                     throw=[exceptions['PymorException'], exceptions['DuneException']])
    Class.add_method('copy_and_return_ptr',
                     retval(ThisType + ' *', caller_owns_return=True),
                     [],
                     is_const=True,
                     throw=[exceptions['PymorException']],
                     custom_name='copy')
    Class.add_method('scal',
                     None,
                     [param('const ' + ScalarType + ' &', 'alpha')],
                     throw=[exceptions['PymorException']])
    Class.add_method('axpy',
                     None,
                     [param('const ' + ScalarType + ' &', 'alpha'),
                      param('const ' + ThisType + ' &', 'xx')],
                     throw=[exceptions['PymorException']])
    # what we want from MatrixInterface
    Class.add_method('dim_source',
                     retval('unsigned int'),
                     [],
                     is_const=True,
                     throw=[exceptions['PymorException']])
    Class.add_method('dim_range',
                     retval('unsigned int'),
                     [],
                     is_const=True,
                     throw=[exceptions['PymorException']])
    return module, Class
