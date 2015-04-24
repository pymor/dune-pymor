# This file is part of the dune-pymor project:
#   https://github.com/pymor/dune-pymor
# Copyright Holders: Stephan Rave, Felix Schindler
# License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

from __future__ import absolute_import, division, print_function

from inspect import isclass
from itertools import izip
from types import ModuleType

import numpy as np

from pymor.core.logger import getLogger
from pymor.parameters.base import Parameter, ParameterType
from pymor.parameters.functionals import ExpressionParameterFunctional


class Wrapper(object):

    def __init__(self, DuneParameterType, DuneParameter, DuneParameterFunctional):
        self.wrapped_classes = {}
        self.wrapped_classes_by_type_this = {}
        self.DuneParameterType = DuneParameterType
        self.DuneParameter = DuneParameter
        self.instance_wrappers = {DuneParameterType: self._parameter_type,
                                  DuneParameter: self._parameter,
                                  DuneParameterFunctional: self._parameter_functional}

    def add_class(self, cls, wrapped_cls):
        self.wrapped_classes[cls] = wrapped_cls
        if hasattr(cls, 'type_this'):
            try:
                self.wrapped_classes_by_type_this[cls.type_this()] = wrapped_cls
            except TypeError:
                logger = getLogger('dune.pymor.core')
                logger.warn('Could not call type_this on {}. (Not a static method?)'.format(cls.__name__))

    def add_vector_class(self, cls, wrapped_cls):
        self.add_class(cls, wrapped_cls)

    def _parameter_type(self, dune_parameter_type):
        return ParameterType({k: v for k, v in izip(list(dune_parameter_type.keys()),
                                                    list(dune_parameter_type.values()))})

    def _parameter(self, dune_parameter):
        assert isinstance(dune_parameter, self.DuneParameter)
        return Parameter({k: np.array(v) for k, v in izip(list(dune_parameter.keys()),
                                                          [list(p) for p in list(dune_parameter.values())])})

    def _parameter_functional(self, dune_functional):
        pt = self[dune_functional.parameter_type()]
        expression = dune_functional.expression() + '.reshape(tuple())'
        return ExpressionParameterFunctional(expression, pt)

    def dune_parameter(self, parameter):
        assert isinstance(parameter, Parameter)
        dune_parameter = self.DuneParameter()
        for k, v in parameter.iteritems():
            assert v.ndim == 1
            dune_parameter.set(k, list(v))
        return dune_parameter

    def __getitem__(self, obj):
        if isclass(obj):
            return self.wrapped_classes[obj]
        elif type(obj) in self.instance_wrappers:
            return self.instance_wrappers[type(obj)](obj)
        elif isinstance(obj, str):
            return self.wrapped_classes_by_type_this[obj]
        else:
            return self.wrapped_classes[type(obj)](obj)
