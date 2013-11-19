#! /usr/bin/env python
# This file is part of the dune-pymor project:
#   https://github.com/pymor/dune-pymor
# Copyright Holders: Felix Albrecht, Stephan Rave
# License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

import pybindgen
from pybindgen import retval, param

from pymor import defaults
from pymor.la.listvectorarray import VectorInterface, ListVectorArray

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
    Class.add_constructor([param(CONFIG_H['DUNE_PYMOR_SSIZE_T'], 'size')])
    Class.add_constructor([param(CONFIG_H['DUNE_PYMOR_SSIZE_T'], 'size'), param(ScalarType, 'value')])
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
                     retval(CONFIG_H['DUNE_PYMOR_SSIZE_T']),
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
                     [param('const std::vector< ' + CONFIG_H['DUNE_PYMOR_SSIZE_T'] + ' > &', 'component_indices')],
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
                     retval(CONFIG_H['DUNE_PYMOR_SSIZE_T']),
                     [], is_const=True, throw=[exceptions['PymorException']])
    Class.add_method('dim_range',
                     retval(CONFIG_H['DUNE_PYMOR_SSIZE_T']),
                     [], is_const=True, throw=[exceptions['PymorException']])
    return module, Class


def wrap_vector(cls):

    class WrappedVector(VectorInterface):

        wrapped_type = cls

        def __init__(self, v):
            self._impl = v

        def zeros(cls, dim):
            return cls(cls.wrapped_type(dim))

        @property
        def dim(self):
            return self._impl.dim()

        def copy(self):
            return type(self)(self._impl.copy())

        def almost_equal(self, other, rtol=None, atol=0):
            assert atol is None or atol == 0, 'Not supported'
            assert type(other) == type(self)
            rtol = defaults.float_cmp_tol if rtol is None else rtol
            return self._impl.almost_equal(other._impl, rtol)

        def scal(self, alpha):
            self._impl.scal(alpha)

        def axpy(self, alpha, x):
            assert type(x) == type(self)
            self._impl.axpy(alpha, x._impl)

        def dot(self, other):
            assert type(other) == type(self)
            return self._impl.dot(other._impl)

        def l1_norm(self):
            return self._impl.l1_norm()

        def l2_norm(self):
            return self._impl.l2_norm()

        def sup_norm(self):
            return self._impl.sup_norm()

        def components(self, component_indices):
            if isinstance(component_indices, np.ndarray):
                component_indices = list(component_indices)
            return np.array(list(self._impl.components(component_indices)), ndmin=1)

        def amax(self):
            return tuple(self._impl.amax())

        def __add__(self, other):
            assert type(other) == type(self)
            return type(self)(self._impl.add(other._impl))

        def __iadd__(self, other):
            assert type(other) == type(self)
            self._impl.iadd(other._impl)
            return self

        __radd__ = __add__

        def __sub__(self, other):
            assert type(other) == type(self)
            return type(self)(self._impl.sub(other._impl))

        def __isub__(self, other):
            assert type(other) == type(self)
            self._impl.isub(other._impl)
            return self

    WrappedVector.__name__ = cls.__name__

    class VectorArray(ListVectorArray):
        vector_type = WrappedVector

    VectorArray.__name__ = '{}_ListVectorArray'.format(cls.__name__)

    return WrappedVector, VectorArray
