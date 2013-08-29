# This file is part of the dune-pymor project:
#   https://github.com/pyMor/dune-pymor
# Copyright Holders: Felix Albrecht, Stephan Rave
# License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

from __future__ import absolute_import, division, print_function

from inspect import isclass
from itertools import izip
from types import ModuleType

import numpy as np

from pymor.core import getLogger
from pymor.parameters.base import Parameter, ParameterType
from pymor.parameters.functionals import GenericParameterFunctional


class Wrapper(object):

    def __init__(self, DuneParameterType, DuneParameter):
        self.wrapped_classes = {}
        self.wrapped_classes_by_type_this = {}
        self.vector_arrays = {}
        self.DuneParameterType = DuneParameterType
        self.DuneParameter = DuneParameter

    def add_class(self, cls, wrapped_cls):
        self.wrapped_classes[cls] = wrapped_cls
        if hasattr(cls, 'type_this'):
            try:
                self.wrapped_classes_by_type_this[cls.type_this()] = wrapped_cls
            except TypeError:
                logger = getLogger('dune.pymor.core')
                logger.warn('Could not call type_this on {}. (Not a static method?)'.format(cls.__name__))

    def add_vector_class(self, cls, wrapped_cls, vector_array):
        self.add_class(cls, wrapped_cls)
        self.vector_arrays[wrapped_cls] = vector_array

    def vector_array(self, obj):
        if isclass(obj):
            return self.vector_arrays[obj]
        else:
            return self.vector_arrays[type(obj)]([obj])

    def parameter_type(self, dune_parameter_type):
        assert isinstance(dune_parameter_type, self.DuneParameterType)
        return ParameterType({k: v for k, v in izip(list(dune_parameter_type.keys()),
                                                    list(dune_parameter_type.values()))})

    def parameter(self, dune_parameter):
        assert isinstance(dune_parameter, self.DuneParameter)
        return Parameter({k: np.array(v) for k, v in izip(list(dune_parameter.keys()),
                                                          [list(p) for p in list(dune_parameter.values())])})

    def dune_parameter(self, parameter):
        assert isinstance(parameter, Parameter)
        dune_parameter = self.DuneParameter()
        for k, v in parameter.iteritems():
            assert v.ndim == 1
            dune_parameter.set(k, list(v))
        return dune_parameter

    def parameter_functional(self, dune_functional):
        pt = self.parameter_type(dune_functional.parameter_type())
        expression = dune_functional.expression()
        code = compile(expression, '<dune expression>', 'eval')
        mapping = lambda mu: eval(code, {}, mu)
        return GenericParameterFunctional(mapping, pt)

    def __getitem__(self, obj):
        if isclass(obj):
            return self.wrapped_classes[obj]
        elif isinstance(obj, str):
            return self.wrapped_classes_by_type_this[obj]
        else:
            return self.wrapped_classes[type(obj)](obj)
