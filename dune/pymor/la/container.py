#! /usr/bin/env python
# This file is part of the dune-pymor project:
#   https://github.com/pymor/dune-pymor
# Copyright Holders: Stephan Rave, Felix Schindler
# License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

import pybindgen
from pybindgen import retval, param
import numpy as np

from pymor.core.defaults import defaults
from pymor.vectorarrays.list import VectorInterface, ListVectorArray

# ReturnValue for converting a raw C pointer to a PyBuffer object
# from pybindgen/examples/buffer/modulegen.py
class BufferReturn(pybindgen.ReturnValue):
    CTYPES = []

    def __init__(self, ctype, length_expression):
        super(BufferReturn, self).__init__(ctype, is_const=False)
        self.length_expression = length_expression

    def convert_c_to_python(self, wrapper):
        pybuf = wrapper.after_call.declare_variable("PyObject*", "pybuf")
        wrapper.after_call.write_code("%s = PyBuffer_FromReadWriteMemory(retval, %s);"
                                      % (pybuf, self.length_expression))
        wrapper.build_params.add_parameter("N", [pybuf], prepend=True)


def inject_VectorImplementation(module, exceptions, interfaces, CONFIG_H, name, Traits, template_parameters=None,
                                provides_data=False):
    assert(isinstance(module, pybindgen.module.Module))
    assert(isinstance(exceptions, list))
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
                                parent=interfaces['Dune::Stuff::LA::Tags::VectorInterface'],
                                template_parameters=template_parameters)
    Class.add_constructor([])
    Class.add_constructor([param(CONFIG_H['DUNE_STUFF_SSIZE_T'], 'size')])
    Class.add_constructor([param(CONFIG_H['DUNE_STUFF_SSIZE_T'], 'size'), param(ScalarType, 'value')])
    Class.add_copy_constructor()
    # what we want from ContainerInterface
    Class.add_method('type_this', retval('std::string'), [], is_const=True, is_static=True,
            throw=exceptions)
    Class.add_method('copy', retval(ThisType), [], is_const=True, throw=exceptions)
    Class.add_method('scal',
                     None,
                     [param('const ' + ScalarType + ' &', 'alpha')],
                     throw=exceptions)
    Class.add_method('axpy',
                     None,
                     [param('const ' + ScalarType + ' &', 'alpha'),
                      param('const ' + ThisType + ' &', 'xx')],
                     throw=exceptions)
    Class.add_method('has_equal_shape',
                     'bool',
                     [param('const ' + ThisType + ' &', 'other')],
                     is_const=True,
                     throw=exceptions)
    Class.add_method('valid',
                     'bool',
                     [], is_const=True, throw=exceptions)
    # what we want from ProvidesData interface
    if provides_data:
        Class.add_method('data', BufferReturn(ScalarType + ' *', 'self->obj->size()  * sizeof(' + ScalarType + ')'), [])
    # what we want from VectorInterface
    Class.add_method('almost_equal',
                     retval('bool'),
                     [param('const ' + ThisType + ' &', 'other'),
                      param('const ' + ScalarType, 'epsilon')],
                     is_const=True, throw=exceptions)
    Class.add_method('almost_equal',
                     retval('bool'),
                     [param('const ' + ThisType + ' &', 'other')],
                     is_const=True, throw=exceptions)
    Class.add_method('dot',
                     retval(ScalarType),
                     [param('const ' + ThisType + ' &', 'other')],
                     is_const=True,
                     throw=exceptions)
    Class.add_method('l1_norm', retval(ScalarType), [], is_const=True, throw=exceptions)
    Class.add_method('l2_norm', retval(ScalarType), [], is_const=True, throw=exceptions)
    Class.add_method('sup_norm', retval(ScalarType), [], is_const=True, throw=exceptions)
    Class.add_method('add',
                     None,
                     [param('const ' + ThisType + ' &', 'other'),
                      param(ThisType + ' &', 'result')],
                     is_const=True,
                     throw=exceptions)
    Class.add_method('add',
                     retval(ThisType),
                     [param('const ' + ThisType + ' &', 'other')],
                     is_const=True,
                     throw=exceptions)
    Class.add_method('iadd',
                     None,
                     [param('const ' + ThisType + ' &', 'other')],
                     throw=exceptions)
    Class.add_method('sub',
                     None,
                     [param('const ' + ThisType + ' &', 'other'),
                      param(ThisType + ' &', 'result')],
                     is_const=True,
                     throw=exceptions)
    Class.add_method('sub',
                     retval(ThisType),
                     [param('const ' + ThisType + ' &', 'other')],
                     is_const=True,
                     throw=exceptions)
    Class.add_method('isub',
                     None,
                     [param('const ' + ThisType + ' &', 'other')],
                     throw=exceptions)
    Class.add_method('pb_dim',
                     retval(CONFIG_H['DUNE_STUFF_SSIZE_T']),
                     [],
                     is_const=True,
                     throw=exceptions,
                     custom_name='dim')
    Class.add_method('pb_add_to_entry',
                     None,
                     [param('const ' + CONFIG_H['DUNE_STUFF_SSIZE_T'], 'ii'),
                      param('const ' + ScalarType + ' &', 'value')],
                     throw=exceptions,
                     custom_name='add_to_entry')
    Class.add_method('pb_set_entry',
                     None,
                     [param('const ' + CONFIG_H['DUNE_STUFF_SSIZE_T'], 'ii'),
                      param('const ' + ScalarType + ' &', 'value')],
                     throw=exceptions,
                     custom_name='set_entry')
    Class.add_method('pb_get_entry',
                     retval(ScalarType),
                     [param('const ' + CONFIG_H['DUNE_STUFF_SSIZE_T'], 'ii')],
                     throw=exceptions,
                     is_const=True,
                     custom_name='get_entry')
    Class.add_method('pb_amax',
                     retval('std::vector< ' + ScalarType + ' >'),
                     [],
                     is_const=True,
                     throw=exceptions,
                     custom_name='amax')
    Class.add_method('components',
                     retval('std::vector< ' + ScalarType + ' >'),
                     [param('const std::vector< ' + CONFIG_H['DUNE_STUFF_SSIZE_T'] + '> &', 'component_indices')],
                     is_const=True,
                     throw=exceptions)


    return module, Class


def inject_MatrixImplementation(module, exceptions, interfaces, CONFIG_H, name, Traits, template_parameters=None):
    assert(isinstance(module, pybindgen.module.Module))
    assert(isinstance(exceptions, list))
    assert(isinstance(interfaces, dict))
    for element in interfaces:
        assert(isinstance(element, str))
        assert(len(element) > 0)
    assert(isinstance(CONFIG_H, dict))
    assert(len(name.strip()) > 0)
    assert(isinstance(Traits, dict))
    # for key in Traits.keys():
    #     assert(isinstance(Traits[key], str))
    #     assert(len(Traits[key].strip()) > 0)
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
                                parent=interfaces['Dune::Stuff::LA::Tags::MatrixInterface'],
                                template_parameters=template_parameters)
    Class.add_constructor([])
    # what we want from ContainerInterface
    Class.add_method('type_this', retval('std::string'), [], is_const=True, is_static=True,
            throw=exceptions)
    Class.add_method('copy',
                     retval(ThisType),
                     [],
                     is_const=True,
                     throw=exceptions)
    Class.add_method('scal',
                     None,
                     [param('const ' + ScalarType + ' &', 'alpha')],
                     throw=exceptions)
    Class.add_method('axpy',
                     None,
                     [param('const ' + ScalarType + ' &', 'alpha'),
                      param('const ' + ThisType + ' &', 'xx')],
                     throw=exceptions)
    Class.add_method('has_equal_shape',
                     retval('bool'),
                     [param('const ' + ThisType + ' &', 'other')],
                     is_const=True,
                     throw=exceptions)
    # what we want from MatrixInterface
    Class.add_method('pb_rows',
                     retval(CONFIG_H['DUNE_STUFF_SSIZE_T']),
                     [], is_const=True, throw=exceptions,
                     custom_name='rows')
    Class.add_method('pb_cols',
                     retval(CONFIG_H['DUNE_STUFF_SSIZE_T']),
                     [], is_const=True, throw=exceptions,
                     custom_name='cols')
    Class.add_method('pb_add_to_entry',
                     None, [param('const ' + CONFIG_H['DUNE_STUFF_SSIZE_T'], 'ii'),
                            param('const ' + CONFIG_H['DUNE_STUFF_SSIZE_T'], 'jj'),
                            param(ScalarType, 'value')],
                     throw=exceptions,
                     custom_name='add_to_entry')
    Class.add_method('pb_set_entry',
                     None, [param('const ' + CONFIG_H['DUNE_STUFF_SSIZE_T'], 'ii'),
                            param('const ' + CONFIG_H['DUNE_STUFF_SSIZE_T'], 'jj'),
                            param(ScalarType, 'value')],
                     throw=exceptions,
                     custom_name='set_entry')
    Class.add_method('pb_get_entry',
                     retval(ScalarType),
                     [param('const ' + CONFIG_H['DUNE_STUFF_SSIZE_T'], 'ii'),
                      param('const ' + CONFIG_H['DUNE_STUFF_SSIZE_T'], 'jj')],
                     is_const=True, throw=exceptions,
                     custom_name='get_entry')
    Class.add_method('valid',
                     'bool',
                     [], is_const=True, throw=exceptions)
    Class.add_method('pb_clear_row',
                     None, [param('const ' + CONFIG_H['DUNE_STUFF_SSIZE_T'], 'ii')],
                     throw=exceptions,
                     custom_name='clear_row')
    Class.add_method('pb_unit_col',
                     None, [param('const ' + CONFIG_H['DUNE_STUFF_SSIZE_T'], 'jj')],
                     throw=exceptions,
                     custom_name='unit_col')
    Class.add_method('pb_clear_row',
                     None, [param('const ' + CONFIG_H['DUNE_STUFF_SSIZE_T'], 'ii')],
                     throw=exceptions,
                     custom_name='clear_row')
    Class.add_method('pb_unit_col',
                     None, [param('const ' + CONFIG_H['DUNE_STUFF_SSIZE_T'], 'jj')],
                     throw=exceptions,
                     custom_name='unit_col')
    if 'VectorType' in Traits:
        VectorTypes = Traits['VectorType']
        if not isinstance(VectorTypes, list):
            VectorTypes = [VectorTypes]
        for VectorType in VectorTypes:
            Class.add_method('mv',
                             None, [param('const ' + VectorType + '&', 'xx'),
                                    param(VectorType + '&', 'yy')],
                             is_const=True,
                             throw=exceptions)
    return module, Class


def wrap_vector(cls):

    class WrappedVector(VectorInterface):

        wrapped_type = cls

        @property
        def data(self):
            return np.frombuffer(self._impl.data(), dtype=np.float)

        def __init__(self, v):
            self._impl = v

        @classmethod
        def make_zeros(cls, subtype):
            return cls(cls.wrapped_type(subtype))

        @property
        def subtype(self):
            return self._impl.dim()

        @property
        def dim(self):
            return self._impl.dim()

        def copy(self):
            return type(self)(self._impl.copy())

        # @defaults('atol', 'rtol', qualname='dune.pymor.WrappedVector.almost_equal')
        def almost_equal(self, other,
                         rtol=2**4 * np.finfo(np.zeros(1.).dtype).eps,
                         atol=0.):
            # assert atol == 0., 'Not supported'
            assert type(other) == type(self)
            return self._impl.almost_equal(other._impl,
                                           rtol if rtol is not None else 2**4 *np.finfo(np.zeros(1.).dtype).eps)

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

        def valid(self):
            return self._impl.valid()

    WrappedVector.__name__ = cls.__name__

    class VectorArray(ListVectorArray):
        vector_type = WrappedVector

    VectorArray.__name__ = '{}_ListVectorArray'.format(cls.__name__)

    return WrappedVector, VectorArray
