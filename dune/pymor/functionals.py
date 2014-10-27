#! /usr/bin/env python
# This file is part of the dune-pymor project:
#   https://github.com/pymor/dune-pymor
# Copyright Holders: Stephan Rave, Felix Schindler
# License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

import pybindgen
from pybindgen import retval, param

import numpy as np

from pymor.la import NumpyVectorArray, NumpyVectorSpace, VectorSpace
from pymor.operators.basic import OperatorBase, LincombOperator

def inject_VectorBasedImplementation(module, exceptions, interfaces, CONFIG_H, Traits, template_parameters=None):
    assert(isinstance(module, pybindgen.module.Module))
    assert(isinstance(exceptions, list))
    assert(isinstance(interfaces, dict))
    for element in interfaces:
        assert(isinstance(element, str))
        assert(len(element) > 0)
    assert(isinstance(CONFIG_H, dict))
    assert(isinstance(Traits, dict))
    for key in Traits.keys():
        assert(isinstance(Traits[key], str))
        assert(len(Traits[key].strip()) > 0)
    assert('SourceType' in Traits)
    SourceType = Traits['SourceType']
    assert('ScalarType' in Traits)
    ScalarType = Traits['ScalarType']
    assert('ContainerType' in Traits)
    ContainerType = Traits['ContainerType']
    if template_parameters is not None:
        if isinstance(template_parameters, str):
            assert(len(template_parameters.strip()) > 0)
            template_parameters = [ template_parameters ]
        elif isinstance(template_parameters, list):
            for element in template_parameters:
                assert(isinstance(element, str))
                assert(len(element.strip()) > 0)
    module = module.add_cpp_namespace('Dune').add_cpp_namespace('Pymor').add_cpp_namespace('Functionals')
    Class = module.add_class('VectorBased',
                             parent=[interfaces['Dune::Pymor::Tags::FunctionalInterface'],
                                     interfaces['Dune::Pymor::Parametric']],
                             template_parameters=template_parameters)
    Class.add_method('type_this', retval('std::string'), [], is_const=True, is_static=True, throw=exceptions)
    Class.add_method('type_source', retval('std::string'), [], is_const=True, is_static=True, throw=exceptions)
    Class.add_method('type_scalar', retval('std::string'), [], is_const=True, is_static=True, throw=exceptions)
    Class.add_method('type_frozen', retval('std::string'), [], is_const=True, is_static=True, throw=exceptions)
    Class.add_method('linear', retval('bool'), [], is_const=True)
    Class.add_method('dim_source', retval(CONFIG_H['DUNE_STUFF_SSIZE_T']), [], is_const=True)
    Class.add_method('apply',
                     retval(ScalarType),
                     [param('const ' + SourceType + ' &', 'source')],
                     is_const=True,
                     throw=exceptions)
    Class.add_method('apply',
                     retval(ScalarType),
                     [param('const ' + SourceType + ' &', 'source'),
                      param('const Dune::Pymor::Parameter', 'mu')],
                     is_const=True,
                     throw=exceptions)
    Class.add_method('as_vector_and_return_ptr',
                     retval(ContainerType + ' *', caller_owns_return=True),
                     [],
                     is_const=True,
                     throw=exceptions,
                     custom_name='as_vector')
    return Class


class WrappedFunctionalBase(OperatorBase):

    wrapped_type = None

    vec_type_source = None

    type_source = None
    range = NumpyVectorSpace(1)

    _wrapper = None

    def __init__(self, op):
        assert isinstance(op, self.wrapped_type)
        self._impl = op
        self.source = VectorSpace(self.type_source, op.dim_source())
        self.linear = op.linear()
        if hasattr(op, 'parametric') and op.parametric():
            pt = self._wrapper[op.parameter_type()]
            self.build_parameter_type(pt, local_global=True)

    def apply(self, U, ind=None, mu=None):
        assert U in self.source
        vectors = U._list if ind is None else [U._list[i] for i in ind]
        if self.parametric:
            mu = self._wrapper.dune_parameter(self.strip_parameter(mu))
            R = np.array([self._impl.apply(v._impl, mu) for v in vectors])[..., np.newaxis]
            return NumpyVectorArray(R, copy=False)
        else:
            R = np.array([self._impl.apply(v._impl) for v in vectors])[..., np.newaxis]
            return NumpyVectorArray(R, copy=False)

    def as_vector(self, mu=None):
        if self.parametric:
            mu = self._wrapper.dune_parameter(self.strip_parameter(mu))
            if hasattr(self._impl, 'as_vector'):
                return self._wrapper.vector_array(self.vec_type_source(self._impl.as_vector(mu)))
            elif hasattr(self._impl, 'freeze_parameter'):
                return self._wrapper.vector_array(self.vec_type_source(self._impl.freeze_parameter(mu).as_vector()))
            else:
                raise NotImplementedError
        else:
            assert self.check_parameter(mu)
            if hasattr(self._impl, 'as_vector'):
                return self._wrapper.vector_array(self.vec_type_source(self._impl.as_vector()))
            else:
                raise NotImplementedError


def wrap_functional(cls, wrapper):

    class WrappedFunctional(WrappedFunctionalBase):
        wrapped_type = cls
        vec_type_source = wrapper[cls.type_source()]
        type_source = wrapper.vector_array(vec_type_source)
        _wrapper = wrapper

        def __init__(self, op):
            WrappedFunctionalBase.__init__(self, op)

    WrappedFunctional.__name__ = cls.__name__
    return WrappedFunctional


def inject_LinearAffinelyDecomposedVectorBasedImplementation(module,
                                                             exceptions,
                                                             interfaces,
                                                             CONFIG_H,
                                                             Traits,
                                                             template_parameters=None):
    assert(isinstance(module, pybindgen.module.Module))
    assert(isinstance(exceptions, list))
    assert(isinstance(interfaces, dict))
    for element in interfaces:
        assert(isinstance(element, str))
        assert(len(element) > 0)
    assert(isinstance(CONFIG_H, dict))
    assert(isinstance(Traits, dict))
    for key in Traits.keys():
        assert(isinstance(Traits[key], str))
        assert(len(Traits[key].strip()) > 0)
    assert('SourceType' in Traits)
    SourceType = Traits['SourceType']
    assert('ComponentType' in Traits)
    ComponentType = Traits['ComponentType']
    assert('ScalarType' in Traits)
    ScalarType = Traits['ScalarType']
    assert('FrozenType' in Traits)
    FrozenType = Traits['ComponentType']
    if template_parameters is not None:
        if isinstance(template_parameters, str):
            assert(len(template_parameters.strip()) > 0)
            template_parameters = [ template_parameters ]
        elif isinstance(template_parameters, list):
            for element in template_parameters:
                assert(isinstance(element, str))
                assert(len(element.strip()) > 0)
    module = module.add_cpp_namespace('Dune').add_cpp_namespace('Pymor').add_cpp_namespace('Functionals')
    Class = module.add_class('LinearAffinelyDecomposedVectorBased',
                             parent=[interfaces['Dune::Pymor::Tags::AffinelyDecomposedFunctionalInterface'],
                                     interfaces['Dune::Pymor::Parametric']],
                             template_parameters=template_parameters)
    Class.add_method('type_this', retval('std::string'), [], is_const=True, is_static=True, throw=exceptions)
    Class.add_method('type_source', retval('std::string'), [], is_const=True, is_static=True, throw=exceptions)
    Class.add_method('type_scalar', retval('std::string'), [], is_const=True, is_static=True, throw=exceptions)
    Class.add_method('type_frozen', retval('std::string'), [], is_const=True, is_static=True, throw=exceptions)
    Class.add_method('num_components',
                     retval(CONFIG_H['DUNE_STUFF_SSIZE_T']),
                     [], is_const=True, throw=exceptions)
    Class.add_method('component_and_return_ptr',
                     retval(ComponentType + ' *', caller_owns_return=True),
                     [param('const ' + CONFIG_H['DUNE_STUFF_SSIZE_T'], 'qq')],
                     is_const=True,
                     throw=exceptions,
                     custom_name='component')
    Class.add_method('coefficient_and_return_ptr',
                     retval('Dune::Pymor::ParameterFunctional *', caller_owns_return=True),
                     [param('const ' + CONFIG_H['DUNE_STUFF_SSIZE_T'], 'qq')],
                     is_const=True,
                     throw=exceptions,
                     custom_name='coefficient')
    Class.add_method('has_affine_part', retval('bool'), [], is_const=True, throw=exceptions)
    Class.add_method('affine_part_and_return_ptr',
                     retval(ComponentType + ' *', caller_owns_return=True),
                     [],
                     is_const=True,
                     throw=exceptions,
                     custom_name='affine_part')
    Class.add_method('linear', retval('bool'), [], is_const=True, throw=exceptions)
    Class.add_method('dim_source', retval(CONFIG_H['DUNE_STUFF_SSIZE_T']), [], is_const=True, throw=exceptions)
    Class.add_method('apply',
                     retval(ScalarType),
                     [param('const ' + SourceType + ' &', 'source')],
                     is_const=True,
                     throw=exceptions)
    Class.add_method('apply',
                     retval(ScalarType),
                     [param('const ' + SourceType + ' &', 'source'),
                      param('const Dune::Pymor::Parameter', 'mu')],
                     is_const=True,
                     throw=exceptions)
    Class.add_method('freeze_parameter_and_return_ptr',
                     retval(FrozenType + ' *', caller_owns_return=True),
                     [param('const Dune::Pymor::Parameter', 'mu')],
                     is_const=True,
                     throw=exceptions,
                     custom_name='freeze_parameter')
    return Class


def wrap_affinely_decomposed_functional(cls, wrapper):

    class WrappedFunctional(WrappedFunctionalBase):
        wrapped_type = cls
        vec_type_source = wrapper[cls.type_source()]
        type_source = wrapper.vector_array(vec_type_source)
        _wrapper = wrapper

        def __init__(self, op):
            WrappedFunctionalBase.__init__(self, op)
            self.operators = [self._wrapper[op.component(i)] for i in xrange(op.num_components())]
            self.coefficients = [self._wrapper[op.coefficient(i)] for i in xrange(op.num_components())]
            if op.has_affine_part():
                self.operators.append(self._wrapper[op.affine_part()])
                self.coefficients.append(1.)
                self.affine_part = True
            else:
                self.affine_part = False

        def projected(self, source_basis, range_basis, product=None, name=None):
            return (LincombOperator(self.operators, self.coefficients)
                    .projected(source_basis, range_basis, product=product, name=name))

    WrappedFunctional.__name__ = cls.__name__
    return WrappedFunctional
