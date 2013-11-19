# This file is part of the dune-pymor project:
#   https://github.com/pymor/dune-pymor
# Copyright Holders: Felix Albrecht, Stephan Rave
# License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

from inspect import isclass
from types import ModuleType

from dune.pymor.core.wrapper import Wrapper
from dune.pymor.la.container import wrap_vector
from dune.pymor.discretizations import wrap_stationary_discretization
from dune.pymor.functionals import wrap_affinely_decomposed_functional, wrap_functional
from dune.pymor.operators import wrap_affinely_decomposed_operator, wrap_operator

try:
    from dune.pymor.discretizations import wrap_multiscale_discretization
    HAVE_MULTISCALE = True
except ImportError:
    HAVE_MULTISCALE = False


def wrap_module(mod):

    AffinelyDecomposedFunctionalInterface = mod.Dune.Pymor.AffinelyDecomposedFunctionalInterfaceDynamic
    AffinelyDecomposedOperatorInterface = mod.Dune.Pymor.AffinelyDecomposedOperatorInterfaceDynamic
    FunctionalInterface = mod.Dune.Pymor.FunctionalInterfaceDynamic
    VectorInterface = mod.Dune.Pymor.LA.VectorInterfaceDynamic
    OperatorInterface = mod.Dune.Pymor.OperatorInterfaceDynamic
    Parameter = mod.Dune.Pymor.Parameter
    ParameterFunctional = mod.Dune.Pymor.ParameterFunctional
    ParameterType = mod.Dune.Pymor.ParameterType
    StationaryDiscretizationInterface = mod.Dune.Pymor.StationaryDiscretizationInterfaceDynamic
    if HAVE_MULTISCALE:
        StationaryMultiscaleDiscretiztionInterface = mod.Dune.Pymor.StationaryMultiscaleDiscretiztionInterfaceDynamic

    wrapped_modules = {}

    wrapper = Wrapper(DuneParameterType = ParameterType,
                      DuneParameter = Parameter,
                      DuneParameterFunctional = ParameterFunctional)

    def create_modules(mod):
        wrapped_mod = ModuleType(mod.__name__.lower())
        wrapped_modules[mod] = {'wrapped': wrapped_mod, 'empty': True}
        for k, v in mod.__dict__.iteritems():
            if isinstance(v, ModuleType):
                create_modules(v)

    def add_to_module(k, v, mod):
        wrapped_mod = wrapped_modules[mod]['wrapped']
        try:
            v.__module__ = wrapped_mod.__name__
        except AttributeError:
            pass
        wrapped_mod.__dict__[k] = v
        wrapped_modules[mod]['empty'] = False

    def add_modules(mod):
        wrapped_mod = wrapped_modules[mod]['wrapped']
        for k, v in mod.__dict__.iteritems():
            if isinstance(v, ModuleType):
                wv = add_modules(v)
                if not wrapped_modules[v]['empty']:
                    wrapped_mod.__dict__[k.lower()] = wv
                    wrapped_modules[mod]['empty'] = False

        return wrapped_mod

    def wrap_vectors(mod):
        for k, v in mod.__dict__.iteritems():
            if isinstance(v, ModuleType):
                wrap_vectors(v)
            elif v == VectorInterface:
                continue
            elif isclass(v) and issubclass(v, VectorInterface):
                wrapped_vector, vector_array = wrap_vector(v)
                add_to_module(k, wrapped_vector, mod)
                add_to_module(vector_array.__name__, vector_array, mod)
                wrapper.add_vector_class(v, wrapped_vector, vector_array)

    def wrap_classes(mod):
        for k, v in mod.__dict__.iteritems():
            if isinstance(v, ModuleType):
                wrap_classes(v)
            elif hasattr(v, '__name__') and 'Interface' in v.__name__:
                continue
            elif isclass(v):
                if issubclass(v, AffinelyDecomposedFunctionalInterface):
                    wrapped_class = wrap_affinely_decomposed_functional(v, wrapper)
                elif issubclass(v, AffinelyDecomposedOperatorInterface):
                    wrapped_class = wrap_affinely_decomposed_operator(v, wrapper)
                elif issubclass(v, FunctionalInterface):
                    wrapped_class = wrap_functional(v, wrapper)
                elif issubclass(v, OperatorInterface):
                    wrapped_class = wrap_operator(v, wrapper)
                elif HAVE_MULTISCALE and issubclass(v, StationaryMultiscaleDiscretiztionInterface):
                    wrapped_class = wrap_multiscale_discretization(v, wrapper)
                elif issubclass(v, StationaryDiscretizationInterface):
                    wrapped_class = wrap_stationary_discretization(v, wrapper)
                else:
                    continue
                add_to_module(k, wrapped_class, mod)
                wrapper.add_class(v, wrapped_class)

    create_modules(mod)
    wrap_vectors(mod)
    wrap_classes(mod)
    wrapped_module = add_modules(mod)

    return wrapped_module, wrapper
