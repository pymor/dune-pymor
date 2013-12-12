#! /usr/bin/env python
# This file is part of the dune-pymor project:
#   https://github.com/pymor/dune-pymor
# Copyright Holders: Felix Albrecht, Stephan Rave
# License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

import pybindgen
from pybindgen import retval, param
import numpy as np

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
                                parent=interfaces['Dune::Stuff::LA::VectorInterfaceDynamic'],
                                template_parameters=template_parameters)
    Class.add_constructor([])
    Class.add_constructor([param(CONFIG_H['DUNE_STUFF_SSIZE_T'], 'size')])
    Class.add_constructor([param(CONFIG_H['DUNE_STUFF_SSIZE_T'], 'size'), param(ScalarType, 'value')])
    Class.add_copy_constructor()
    # what we want from ContainerInterface
    Class.add_method('type_this', retval('std::string'), [], is_const=True, is_static=True,
                     throw=[exceptions['DuneException']])
    Class.add_method('copy', retval(ThisType), [], is_const=True, throw=[exceptions['DuneException']])
    Class.add_method('scal',
                     None,
                     [param('const ' + ScalarType + ' &', 'alpha')],
                     throw=[exceptions['DuneException']])
    Class.add_method('axpy',
                     None,
                     [param('const ' + ScalarType + ' &', 'alpha'),
                      param('const ' + ThisType + ' &', 'xx')],
                     throw=[exceptions['DuneException']])
    Class.add_method('has_equal_shape',
                     'bool',
                     [param('const ' + ThisType + ' &', 'other')],
                     throw=[exceptions['DuneException']])
    # what we want from VectorInterface
    Class.add_method('dot',
                     retval(ScalarType),
                     [param('const ' + ThisType + ' &', 'other')],
                     is_const=True,
                     throw=[exceptions['DuneException']])
    Class.add_method('l1_norm', retval(ScalarType), [], is_const=True, throw=[exceptions['DuneException']])
    Class.add_method('l2_norm', retval(ScalarType), [], is_const=True, throw=[exceptions['DuneException']])
    Class.add_method('sup_norm', retval(ScalarType), [], is_const=True, throw=[exceptions['DuneException']])
    Class.add_method('add',
                     None,
                     [param('const ' + ThisType + ' &', 'other'),
                      param(ThisType + ' &', 'result')],
                     is_const=True,
                     throw=[exceptions['DuneException']])
    Class.add_method('add',
                     retval(ThisType),
                     [param('const ' + ThisType + ' &', 'other')],
                     is_const=True,
                     throw=[exceptions['DuneException']])
    Class.add_method('iadd',
                     None,
                     [param('const ' + ThisType + ' &', 'other')],
                     throw=[exceptions['DuneException']])
    Class.add_method('sub',
                     None,
                     [param('const ' + ThisType + ' &', 'other'),
                      param(ThisType + ' &', 'result')],
                     is_const=True,
                     throw=[exceptions['DuneException']])
    Class.add_method('sub',
                     retval(ThisType),
                     [param('const ' + ThisType + ' &', 'other')],
                     is_const=True,
                     throw=[exceptions['DuneException']])
    Class.add_method('isub',
                     None,
                     [param('const ' + ThisType + ' &', 'other')],
                     throw=[exceptions['DuneException']])
    Class.add_method('pb_dim',
                     retval(CONFIG_H['DUNE_STUFF_SSIZE_T']),
                     [],
                     is_const=True,
                     throw=[exceptions['DuneException']],
                     custom_name='dim')
    Class.add_method('pb_add_to_entry',
                     None,
                     [param('const ' + CONFIG_H['DUNE_STUFF_SSIZE_T'], 'ii'),
                      param('const ' + ScalarType + ' &', 'value')],
                     throw=[exceptions['DuneException']],
                     custom_name='add_to_entry')
    Class.add_method('pb_set_entry',
                     None,
                     [param('const ' + CONFIG_H['DUNE_STUFF_SSIZE_T'], 'ii'),
                      param('const ' + ScalarType + ' &', 'value')],
                     throw=[exceptions['DuneException']],
                     custom_name='set_entry')
    Class.add_method('pb_get_entry',
                     retval(ScalarType),
                     [param('const ' + CONFIG_H['DUNE_STUFF_SSIZE_T'], 'ii')],
                     throw=[exceptions['DuneException']],
                     is_const=True,
                     custom_name='get_entry')
    Class.add_method('pb_amax',
                     retval('std::vector< ' + ScalarType + ' >'),
                     [],
                     is_const=True,
                     throw=[exceptions['DuneException']],
                     custom_name='amax')
    Class.add_method('components',
                     retval('std::vector< ' + ScalarType + ' >'),
                     [param('const std::vector< ' + CONFIG_H['DUNE_STUFF_SSIZE_T'] + '> &', 'component_indices')],
                     is_const=True,
                     throw=[exceptions['DuneException']])


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
                                parent=interfaces['Dune::Stuff::LA::MatrixInterfaceDynamic'],
                                template_parameters=template_parameters)
    Class.add_constructor([])
    # what we want from ContainerInterface
    Class.add_method('type_this', retval('std::string'), [], is_const=True, is_static=True,
                     throw=[exceptions['DuneException'], exceptions['DuneException']])
    Class.add_method('copy',
                     retval(ThisType),
                     [],
                     is_const=True,
                     throw=[exceptions['DuneException']])
    Class.add_method('scal',
                     None,
                     [param('const ' + ScalarType + ' &', 'alpha')],
                     throw=[exceptions['DuneException']])
    Class.add_method('axpy',
                     None,
                     [param('const ' + ScalarType + ' &', 'alpha'),
                      param('const ' + ThisType + ' &', 'xx')],
                     throw=[exceptions['DuneException']])
    Class.add_method('has_equal_shape',
                     retval('bool'),
                     [param('const ' + ThisType + ' &', 'other')],
                     is_const=True,
                     throw=[exceptions['DuneException']])
    # what we want from MatrixInterface
    Class.add_method('pb_rows',
                     retval(CONFIG_H['DUNE_STUFF_SSIZE_T']),
                     [], is_const=True, throw=[exceptions['DuneException']],
                     custom_name='rows')
    Class.add_method('pb_cols',
                     retval(CONFIG_H['DUNE_STUFF_SSIZE_T']),
                     [], is_const=True, throw=[exceptions['DuneException']],
                     custom_name='cols')
    Class.add_method('pb_add_to_entry',
                     None, [param('const ' + CONFIG_H['DUNE_STUFF_SSIZE_T'], 'ii'),
                            param('const ' + CONFIG_H['DUNE_STUFF_SSIZE_T'], 'jj'),
                            param(ScalarType, 'value')],
                     throw=[exceptions['DuneException']],
                     custom_name='add_to_entry')
    Class.add_method('pb_set_entry',
                     None, [param('const ' + CONFIG_H['DUNE_STUFF_SSIZE_T'], 'ii'),
                            param('const ' + CONFIG_H['DUNE_STUFF_SSIZE_T'], 'jj'),
                            param(ScalarType, 'value')],
                     throw=[exceptions['DuneException']],
                     custom_name='set_entry')
    Class.add_method('pb_get_entry',
                     retval(ScalarType),
                     [param('const ' + CONFIG_H['DUNE_STUFF_SSIZE_T'], 'ii'),
                      param('const ' + CONFIG_H['DUNE_STUFF_SSIZE_T'], 'jj')],
                     is_const=True, throw=[exceptions['DuneException']],
                     custom_name='set_entry')
    return module, Class
    Class.add_method('pb_clear_row',
                     None, [param('const ' + CONFIG_H['DUNE_STUFF_SSIZE_T'], 'ii')],
                     throw=[exceptions['DuneException']],
                     custom_name='clear_row')
    Class.add_method('pb_unit_col',
                     None, [param('const ' + CONFIG_H['DUNE_STUFF_SSIZE_T'], 'jj')],
                     throw=[exceptions['DuneException']],
                     custom_name='unit_col')
    Class.add_method('pb_clear_row',
                     None, [param('const ' + CONFIG_H['DUNE_STUFF_SSIZE_T'], 'ii')],
                     throw=[exceptions['DuneException']],
                     custom_name='clear_row')
    Class.add_method('pb_unit_col',
                     None, [param('const ' + CONFIG_H['DUNE_STUFF_SSIZE_T'], 'jj')],
                     throw=[exceptions['DuneException']],
                     custom_name='unit_col')


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
