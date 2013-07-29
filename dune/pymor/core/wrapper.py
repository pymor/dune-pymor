# This file is part of the dune-pymor project:
#   https://github.com/pyMor/dune-pymor
# Copyright Holders: Felix Albrecht, Stephan Rave
# License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

from __future__ import absolute_import, division, print_function

from inspect import isclass
from types import ModuleType

import numpy as np

from pymor.la.listvectorarray import VectorInterface, ListVectorArray

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
            assert atol == 0, 'Not supported'
            assert type(other) == type(self)
            return self._impl.almost_equal(other._impl, rtol)

        def scal(self, alpha):
            self._impl.scal(alpha)

        def axpy(self, alpha, x):
            assert type(other) == type(self)
            self._impl.axpy(alpha, x)

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
    return WrappedVector


def create_listvectorarray(vec_type):

    class VectorArray(ListVectorArray):
        vector_type = vec_type

    VectorArray.__name__ = '{}_ListVectorArray'.format(vec_type.__name__)
    return VectorArray


def wrap(mod):

    VectorInterfaceDynamic = mod.Dune.Pymor.LA.VectorInterfaceDynamic

    def add_to_module(name, obj, mod):
        try:
            if hasattr(obj, '__module__'):
                obj.__module__ = mod.__name__
        except (AttributeError, TypeError):
            pass
        mod.__dict__[name] = obj

    def wrap_module(mod):

        wrapped_mod = ModuleType(mod.__name__.lower())

        for k, v in mod.__dict__.iteritems():
            if k.startswith('_'):
                pass
            elif isinstance(v, ModuleType):
                add_to_module(k.lower(), wrap_module(v), wrapped_mod)
            elif v in (VectorInterfaceDynamic,):
                add_to_module(k, v, wrapped_mod)
            elif isclass(v) and issubclass(v, VectorInterfaceDynamic):
                wrapped_vector = wrap_vector(v)
                vector_array = create_listvectorarray(wrapped_vector)
                add_to_module(k, wrapped_vector, wrapped_mod)
                add_to_module(vector_array.__name__, vector_array, wrapped_mod)
            else:
                add_to_module(k, v, wrapped_mod)

        return wrapped_mod

    return wrap_module(mod)
