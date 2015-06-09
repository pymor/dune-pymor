#! /usr/bin/env python
# This file is part of the dune-pymor project:
#   https://github.com/pymor/dune-pymor
# Copyright Holders: Stephan Rave, Felix Schindler
# License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

from collections import OrderedDict

import pybindgen
from pybindgen import retval, param

from pymor.vectorarrays.interfaces import VectorSpace
from pymor.vectorarrays.list import ListVectorArray
from pymor.operators.basic import OperatorBase
from pymor.operators.constructions import LincombOperator


def inject_OperatorAndInverseImplementation(module, exceptions, interfaces, CONFIG_H,
                                            operator_name,
                                            operator_Traits,
                                            inverse_name,
                                            inverse_Traits,
                                            operator_template_parameters=None,
                                            inverse_template_parameters=None,
                                            container_based=False):
    assert(isinstance(module, pybindgen.module.Module))
    assert(isinstance(exceptions, list))
    assert(isinstance(interfaces, dict))
    for element in interfaces:
        assert(isinstance(element, str))
        assert(len(element) > 0)
    assert(isinstance(CONFIG_H, dict))
    # checks for the operator
    assert(isinstance(operator_Traits, dict))
    for key in operator_Traits.keys():
        assert(isinstance(operator_Traits[key], str))
        assert(len(operator_Traits[key].strip()) > 0)
    assert('SourceType' in operator_Traits)
    operator_SourceType = operator_Traits['SourceType']
    assert('RangeType' in operator_Traits)
    operator_RangeType = operator_Traits['RangeType']
    assert('ScalarType' in operator_Traits)
    operator_ScalarType = operator_Traits['ScalarType']
    assert('InverseType' in operator_Traits)
    operator_InverseType = operator_Traits['InverseType']
    assert('FrozenType' in operator_Traits)
    operator_FrozenType = operator_Traits['FrozenType']
    operator_SpaceType = operator_Traits['SpaceType']
    if container_based:
        operator_ContainerType = operator_Traits['ContainerType']
    if operator_template_parameters is not None:
        if isinstance(operator_template_parameters, str):
            assert(len(operator_template_parameters.strip()) > 0)
            operator_template_parameters = [ operator_template_parameters ]
        elif isinstance(operator_template_parameters, list):
            for element in operator_template_parameters:
                assert(isinstance(element, str))
                assert(len(element.strip()) > 0)
    # checks for the inverse
    assert(isinstance(inverse_Traits, dict))
    for key in inverse_Traits.keys():
        assert(isinstance(inverse_Traits[key], str))
        assert(len(inverse_Traits[key].strip()) > 0)
    assert('SourceType' in inverse_Traits)
    inverse_SourceType = inverse_Traits['SourceType']
    assert('RangeType' in inverse_Traits)
    inverse_RangeType = inverse_Traits['RangeType']
    assert('ScalarType' in inverse_Traits)
    inverse_ScalarType = inverse_Traits['ScalarType']
    assert('InverseType' in inverse_Traits)
    inverse_InverseType = inverse_Traits['InverseType']
    assert('FrozenType' in inverse_Traits)
    inverse_FrozenType = inverse_Traits['FrozenType']
    if inverse_template_parameters is not None:
        if isinstance(inverse_template_parameters, str):
            assert(len(inverse_template_parameters.strip()) > 0)
            inverse_template_parameters = [ inverse_template_parameters ]
        elif isinstance(inverse_template_parameters, list):
            for element in inverse_template_parameters:
                assert(isinstance(element, str))
                assert(len(element.strip()) > 0)
    # add the operator
    operator_namespace = module
    operator_namespaces = [nspace.strip() for nspace in operator_name.split('::')[:-1]]
    operator_name = operator_name.split('::')[-1].strip()
    if len(operator_namespaces) > 0:
        for nspace in operator_namespaces:
            operator_namespace = operator_namespace.add_cpp_namespace(nspace)
    Operator = operator_namespace.add_class(operator_name,
                                            parent=interfaces['Dune::Pymor::Tags::OperatorInterface'],
                                            template_parameters=operator_template_parameters)
    # add the inverse
    inverse_namespace = module
    inverse_namespaces = [nspace.strip() for nspace in inverse_name.split('::')[:-1]]
    inverse_name = inverse_name.split('::')[-1].strip()
    if len(inverse_namespaces) > 0:
        for nspace in inverse_namespaces:
            inverse_namespace = inverse_namespace.add_cpp_namespace(nspace)
    Inverse = inverse_namespace.add_class(inverse_name,
                                          parent=interfaces['Dune::Pymor::Tags::OperatorInterface'],
                                          template_parameters=inverse_template_parameters)
    # fill the operator
    if container_based:
        Operator.add_constructor([param('const ' + operator_ContainerType + '&', 'matrix'),
                                  param('const {}&'.format(operator_SpaceType), 'space')], throw=exceptions)
    Operator.add_method('type_this', retval('std::string'), [], is_const=True, is_static=True, throw=exceptions)
    Operator.add_method('type_source', retval('std::string'), [], is_const=True, is_static=True, throw=exceptions)
    Operator.add_method('type_range', retval('std::string'), [], is_const=True, is_static=True, throw=exceptions)
    Operator.add_method('type_scalar', retval('std::string'), [], is_const=True, is_static=True, throw=exceptions)
    Operator.add_method('type_inverse', retval('std::string'), [], is_const=True, is_static=True, throw=exceptions)
    Operator.add_method('type_frozen', retval('std::string'), [], is_const=True, is_static=True, throw=exceptions)
    Operator.add_method('linear', retval('bool'), [], is_const=True, throw=exceptions)
    Operator.add_method('dim_source',
                        retval(CONFIG_H['DUNE_STUFF_SSIZE_T']),
                        [], is_const=True, throw=exceptions)
    Operator.add_method('dim_range',
                        retval(CONFIG_H['DUNE_STUFF_SSIZE_T']),
                        [], is_const=True, throw=exceptions)
    Operator.add_method('apply', None,
                        [param('const ' + operator_SourceType + ' &', 'source'),
                         param(operator_RangeType + ' &', 'range')],
                        is_const=True, throw=exceptions)
    Operator.add_method('apply', None,
                        [param('const ' + operator_SourceType + ' &', 'source'),
                         param(operator_RangeType + ' &', 'range'),
                         param('Dune::Pymor::Parameter', 'mu')],
                        is_const=True, throw=exceptions)
    Operator.add_method('apply_and_return_ptr',
                        retval(operator_RangeType + ' *', caller_owns_return=True),
                        [param('const ' + operator_SourceType + ' &', 'source')],
                        is_const=True, throw=exceptions, custom_name='apply')
    Operator.add_method('apply_and_return_ptr',
                        retval(operator_RangeType + ' *', caller_owns_return=True),
                        [param('const ' + operator_SourceType + ' &', 'source'),
                         param('Dune::Pymor::Parameter', 'mu')],
                        is_const=True, throw=exceptions, custom_name='apply')
    Operator.add_method('apply2', operator_ScalarType,
                        [param('const ' + operator_RangeType + ' &', 'range'),
                         param('const ' + operator_SourceType + ' &', 'source')],
                        is_const=True, throw=exceptions)
    Operator.add_method('apply2', operator_ScalarType,
                        [param('const ' + operator_RangeType + ' &', 'range'),
                         param('const ' + operator_SourceType + ' &', 'source'),
                         param('Dune::Pymor::Parameter', 'mu')],
                        is_const=True, throw=exceptions)
    Operator.add_method('invert_options',
                        retval('std::vector< std::string >'),
                        [], is_const=True, is_static=True, throw=exceptions)
    Operator.add_method('invert_and_return_ptr',
                        retval(operator_InverseType + ' *', caller_owns_return=True),
                        [], is_const=True, throw=exceptions, custom_name='invert')
    Operator.add_method('invert_and_return_ptr',
                        retval(operator_InverseType + ' *', caller_owns_return=True),
                        [param('const std::string', 'option')],
                        is_const=True, throw=exceptions, custom_name='invert')
    Operator.add_method('invert_and_return_ptr',
                        retval(operator_InverseType + ' *', caller_owns_return=True),
                        [param('const std::string', 'option'),
                         param('Dune::Pymor::Parameter', 'mu')],
                        is_const=True, throw=exceptions, custom_name='invert')
    Operator.add_method('apply_inverse', None,
                        [param('const ' + operator_RangeType + ' &', 'range'),
                         param(operator_SourceType + ' &', 'source')],
                        is_const=True, throw=exceptions)
    Operator.add_method('apply_inverse', None,
                        [param('const ' + operator_RangeType + ' &', 'range'),
                         param(operator_SourceType + ' &', 'source'),
                         param('const std::string', 'option')],
                        is_const=True, throw=exceptions)
    Operator.add_method('apply_inverse', None,
                        [param('const ' + operator_RangeType + ' &', 'range'),
                         param(operator_SourceType + ' &', 'source'),
                         param('const std::string', 'option'),
                         param('const Dune::Pymor::Parameter', 'mu')],
                        is_const=True, throw=exceptions)
    Operator.add_method('apply_inverse_and_return_ptr',
                        retval(operator_SourceType + ' *', caller_owns_return=True),
                        [param('const ' + operator_RangeType + ' &', 'range')],
                        is_const=True, throw=exceptions, custom_name='apply_inverse')
    Operator.add_method('apply_inverse_and_return_ptr',
                        retval(operator_SourceType + ' *', caller_owns_return=True),
                        [param('const ' + operator_RangeType + ' &', 'range'),
                         param('const std::string', 'option')],
                        is_const=True, throw=exceptions, custom_name='apply_inverse')
    Operator.add_method('apply_inverse_and_return_ptr',
                        retval(operator_SourceType + ' *', caller_owns_return=True),
                        [param('const ' + operator_RangeType + ' &', 'range'),
                         param('const Dune::Stuff::Common::Configuration &', 'option')],
                        is_const=True, throw=exceptions, custom_name='apply_inverse')
    Operator.add_method('apply_inverse_and_return_ptr',
                        retval(operator_SourceType + ' *', caller_owns_return=True),
                        [param('const ' + operator_RangeType + ' &', 'range'),
                         param('const std::string', 'option'),
                         param('const Dune::Pymor::Parameter', 'mu')],
                        is_const=True, throw=exceptions, custom_name='apply_inverse')
    Operator.add_method('apply_inverse_and_return_ptr',
                        retval(operator_SourceType + ' *', caller_owns_return=True),
                        [param('const ' + operator_RangeType + ' &', 'range'),
                         param('const Dune::Stuff::Common::Configuration &', 'option'),
                         param('const Dune::Pymor::Parameter', 'mu')],
                        is_const=True, throw=exceptions, custom_name='apply_inverse')
    Operator.add_method('freeze_parameter_and_return_ptr',
                        retval(operator_FrozenType + ' *', caller_owns_return=True),
                        [param('Dune::Pymor::Parameter', 'mu')],
                        is_const=True, throw=exceptions, custom_name='freeze_parameter')
    if container_based:
        Operator.add_method('pb_container',
                            retval(operator_ContainerType + '*', caller_owns_return=True),
                            [], is_const=True, throw=exceptions,
                            custom_name='container')
    # fill the inverse
    Inverse.add_method('type_this', retval('std::string'), [], is_const=True, is_static=True, throw=exceptions)
    Inverse.add_method('type_source', retval('std::string'), [], is_const=True, is_static=True, throw=exceptions)
    Inverse.add_method('type_range', retval('std::string'), [], is_const=True, is_static=True, throw=exceptions)
    Inverse.add_method('type_scalar', retval('std::string'), [], is_const=True, is_static=True, throw=exceptions)
    Inverse.add_method('type_inverse', retval('std::string'), [], is_const=True, is_static=True, throw=exceptions)
    Inverse.add_method('type_frozen', retval('std::string'), [], is_const=True, is_static=True, throw=exceptions)
    Inverse.add_method('linear', retval('bool'), [], is_const=True, throw=exceptions)
    Inverse.add_method('dim_source',
                       retval(CONFIG_H['DUNE_STUFF_SSIZE_T']),
                       [], is_const=True, throw=exceptions)
    Inverse.add_method('dim_range',
                       retval(CONFIG_H['DUNE_STUFF_SSIZE_T']),
                       [], is_const=True, throw=exceptions)
    Inverse.add_method('apply', None,
                       [param('const ' + inverse_SourceType + ' &', 'source'),
                        param(inverse_RangeType + ' &', 'range')],
                       is_const=True, throw=exceptions)
    Inverse.add_method('apply', None,
                       [param('const ' + inverse_SourceType + ' &', 'source'),
                        param(inverse_RangeType + ' &', 'range'),
                        param('Dune::Pymor::Parameter', 'mu')],
                       is_const=True, throw=exceptions)
    Inverse.add_method('apply_and_return_ptr',
                       retval(inverse_RangeType + ' *', caller_owns_return=True),
                       [param('const ' + inverse_SourceType + ' &', 'source')],
                       is_const=True, throw=exceptions, custom_name='apply')
    Inverse.add_method('apply_and_return_ptr',
                       retval(inverse_RangeType + ' *', caller_owns_return=True),
                       [param('const ' + inverse_SourceType + ' &', 'source'),
                        param('Dune::Pymor::Parameter', 'mu')],
                       is_const=True, throw=exceptions, custom_name='apply')
    Inverse.add_method('apply2', inverse_ScalarType,
                       [param('const ' + inverse_RangeType + ' &', 'range'),
                        param('const ' + inverse_SourceType + ' &', 'source')],
                       is_const=True, throw=exceptions)
    Inverse.add_method('apply2', inverse_ScalarType,
                       [param('const ' + inverse_RangeType + ' &', 'range'),
                        param('const ' + inverse_SourceType + ' &', 'source'),
                        param('Dune::Pymor::Parameter', 'mu')],
                       is_const=True, throw=exceptions)
    Inverse.add_method('invert_options',
                       retval('std::vector< std::string >'),
                       [], is_const=True, throw=exceptions)
    Inverse.add_method('invert_and_return_ptr',
                       retval(inverse_InverseType + ' *', caller_owns_return=True),
                       [], is_const=True, throw=exceptions, custom_name='invert')
    Inverse.add_method('invert_and_return_ptr',
                       retval(inverse_InverseType + ' *', caller_owns_return=True),
                       [param('const std::string', 'option')],
                       is_const=True, throw=exceptions, custom_name='invert')
    Inverse.add_method('invert_and_return_ptr',
                       retval(inverse_InverseType + ' *', caller_owns_return=True),
                       [param('const std::string', 'option'),
                        param('Dune::Pymor::Parameter', 'mu')],
                       is_const=True, throw=exceptions, custom_name='invert')
    Inverse.add_method('apply_inverse', None,
                       [param('const ' + inverse_RangeType + ' &', 'range'),
                        param(inverse_SourceType + ' &', 'source')],
                       is_const=True, throw=exceptions)
    Inverse.add_method('apply_inverse', None,
                       [param('const ' + inverse_RangeType + ' &', 'range'),
                        param(inverse_SourceType + ' &', 'source'),
                        param('const std::string', 'option')],
                       is_const=True, throw=exceptions)
    Inverse.add_method('apply_inverse', None,
                       [param('const ' + inverse_RangeType + ' &', 'range'),
                        param(inverse_SourceType + ' &', 'source'),
                        param('const std::string', 'option'),
                        param('const Dune::Pymor::Parameter', 'mu')],
                       is_const=True, throw=exceptions)
    Inverse.add_method('apply_inverse_and_return_ptr',
                       retval(inverse_SourceType + ' *', caller_owns_return=True),
                       [param('const ' + inverse_RangeType + ' &', 'range')],
                       is_const=True, throw=exceptions, custom_name='apply_inverse')
    Inverse.add_method('apply_inverse_and_return_ptr',
                       retval(inverse_SourceType + ' *', caller_owns_return=True),
                       [param('const ' + inverse_RangeType + ' &', 'range'),
                        param('const std::string', 'option')],
                       is_const=True, throw=exceptions, custom_name='apply_inverse')
    Inverse.add_method('apply_inverse_and_return_ptr',
                       retval(inverse_SourceType + ' *', caller_owns_return=True),
                       [param('const ' + inverse_RangeType + ' &', 'range'),
                        param('const std::string', 'option'),
                        param('const Dune::Pymor::Parameter', 'mu')],
                       is_const=True, throw=exceptions, custom_name='apply_inverse')
    Inverse.add_method('freeze_parameter_and_return_ptr',
                       retval(inverse_FrozenType + ' *', caller_owns_return=True),
                       [param('Dune::Pymor::Parameter', 'mu')],
                       is_const=True, throw=exceptions, custom_name='freeze_parameter')
    return Operator, Inverse


class WrappedOperatorBase(OperatorBase):

    wrapped_type = None

    vec_type_source = None
    vec_type_range = None

    _wrapper = None

    def __init__(self, op):
        assert isinstance(op, self.wrapped_type)
        self._impl = op
        self.source = VectorSpace(ListVectorArray, (self.vec_type_source, int(op.dim_source())))
        self.range = VectorSpace(ListVectorArray, (self.vec_type_range, int(op.dim_range())))
        self.linear = op.linear()
        if hasattr(op, 'parametric') and op.parametric():
            pt = self._wrapper[op.parameter_type()]
            self.build_parameter_type(pt, local_global=True)

    @property
    def invert_options(self):
        return OrderedDict([(k, {'type': k}) for k in list(self._impl.invert_options())])

    def apply(self, U, ind=None, mu=None):
        assert U in self.source
        if ind is not None and not isinstance(ind, list):
            ind = [ind]
        vectors = U._list if ind is None else [U._list[i] for i in ind]
        if self.parametric:
            mu = self._wrapper.dune_parameter(self.strip_parameter(mu))
            return ListVectorArray([self.vec_type_range(self._impl.apply(v._impl, mu)) for v in vectors],
                                   subtype=self.range.subtype)
        else:
            return ListVectorArray([self.vec_type_range(self._impl.apply(v._impl)) for v in vectors],
                                   subtype=self.range.subtype)

    def apply_inverse(self, U, ind=None, mu=None, options=None):
        assert U in self.range
        # assert options is None or isinstance(options, str) \
        #     or (isinstance(options, dict) and options.keys() == ['type'] and
        #         options['type'] in self.invert_options.keys())
        if isinstance(options, dict):
            options = options['type']
        elif options is None:
            options = next(self.invert_options.iterkeys())
        vectors = U._list if ind is None else [U._list[ind]] if isinstance(ind, Number) else [U._list[i] for i in ind]
        if self.parametric:
            mu = self._wrapper.dune_parameter(self.strip_parameter(mu))
            return ListVectorArray([self.vec_type_source(self._impl.apply_inverse(v._impl, options, mu))
                                    for v in vectors],
                                   subtype=self.source.subtype)
        else:
            return ListVectorArray([self.vec_type_source(self._impl.apply_inverse(v._impl, options))
                                    for v in vectors],
                                   subtype=self.source.subtype)


def wrap_operator(cls, wrapper):

    class WrappedOperator(WrappedOperatorBase):
        wrapped_type = cls
        vec_type_source = wrapper[cls.type_source()]
        vec_type_range = wrapper[cls.type_range()]
        _wrapper = wrapper

        def __init__(self, op):
            WrappedOperatorBase.__init__(self, op)

    WrappedOperator.__name__ = cls.__name__
    return WrappedOperator


def inject_LinearAffinelyDecomposedContainerBasedImplementation(module,
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
    assert('RangeType' in Traits)
    RangeType = Traits['RangeType']
    assert('ComponentType' in Traits)
    ComponentType = Traits['ComponentType']
    assert('ScalarType' in Traits)
    ScalarType = Traits['ScalarType']
    assert('FrozenType' in Traits)
    FrozenType = Traits['ComponentType']
    assert('InverseType' in Traits)
    InverseType = Traits['InverseType']
    if template_parameters is not None:
        if isinstance(template_parameters, str):
            assert(len(template_parameters.strip()) > 0)
            template_parameters = [ template_parameters ]
        elif isinstance(template_parameters, list):
            for element in template_parameters:
                assert(isinstance(element, str))
                assert(len(element.strip()) > 0)
    module = module.add_cpp_namespace('Dune').add_cpp_namespace('Pymor').add_cpp_namespace('Operators')
    Class = module.add_class('LinearAffinelyDecomposedContainerBased',
                             parent=[interfaces['Dune::Pymor::Tags::AffinelyDecomposedOperatorInterface'],
                                     interfaces['Dune::Pymor::Parametric']],
                             template_parameters=template_parameters)
    Class.add_method('type_this', retval('std::string'), [], is_const=True, is_static=True, throw=exceptions)
    Class.add_method('type_source', retval('std::string'), [], is_const=True, is_static=True, throw=exceptions)
    Class.add_method('type_range', retval('std::string'), [], is_const=True, is_static=True, throw=exceptions)
    Class.add_method('type_scalar', retval('std::string'), [], is_const=True, is_static=True, throw=exceptions)
    Class.add_method('type_inverse', retval('std::string'), [], is_const=True, is_static=True, throw=exceptions)
    Class.add_method('type_frozen', retval('std::string'), [], is_const=True, is_static=True, throw=exceptions)
    Class.add_method('num_components',
                     retval(CONFIG_H['DUNE_STUFF_SSIZE_T']),
                     [], is_const=True, throw=exceptions)
    Class.add_method('component_and_return_ptr',
                     retval(ComponentType + ' *', caller_owns_return=True),
                     [param('const int', 'qq')],
                     is_const=True,
                     throw=exceptions,
                     custom_name='component')
    Class.add_method('coefficient_and_return_ptr',
                     retval('Dune::Pymor::ParameterFunctional *', caller_owns_return=True),
                     [param('const int', 'qq')],
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
    Class.add_method('dim_source',
                     retval(CONFIG_H['DUNE_STUFF_SSIZE_T']),
                     [], is_const=True, throw=exceptions)
    Class.add_method('dim_range',
                     retval(CONFIG_H['DUNE_STUFF_SSIZE_T']),
                     [], is_const=True, throw=exceptions)
    Class.add_method('apply', None,
                     [param('const ' + SourceType + ' &', 'source'),
                      param(RangeType + ' &', 'range')],
                     is_const=True, throw=exceptions)
    Class.add_method('apply', None,
                     [param('const ' + SourceType + ' &', 'source'),
                      param(RangeType + ' &', 'range'),
                      param('Dune::Pymor::Parameter', 'mu')],
                     is_const=True, throw=exceptions)
    Class.add_method('apply_and_return_ptr',
                     retval(RangeType + ' *', caller_owns_return=True),
                     [param('const ' + SourceType + ' &', 'source')],
                     is_const=True, throw=exceptions, custom_name='apply')
    Class.add_method('apply_and_return_ptr',
                     retval(RangeType + ' *', caller_owns_return=True),
                     [param('const ' + SourceType + ' &', 'source'),
                      param('Dune::Pymor::Parameter', 'mu')],
                     is_const=True, throw=exceptions, custom_name='apply')
    Class.add_method('apply2', ScalarType,
                     [param('const ' + RangeType + ' &', 'range'),
                      param('const ' + SourceType + ' &', 'source')],
                     is_const=True, throw=exceptions)
    Class.add_method('apply2', ScalarType,
                     [param('const ' + RangeType + ' &', 'range'),
                      param('const ' + SourceType + ' &', 'source'),
                      param('Dune::Pymor::Parameter', 'mu')],
                     is_const=True, throw=exceptions)
    Class.add_method('invert_options',
                     retval('std::vector< std::string >'),
                     [], is_const=True, throw=exceptions)
    Class.add_method('invert_and_return_ptr',
                     retval(InverseType + ' *', caller_owns_return=True),
                     [], is_const=True, throw=exceptions, custom_name='invert')
    Class.add_method('invert_and_return_ptr',
                     retval(InverseType + ' *', caller_owns_return=True),
                     [param('const std::string', 'option')],
                     is_const=True, throw=exceptions, custom_name='invert')
    Class.add_method('invert_and_return_ptr',
                     retval(InverseType + ' *', caller_owns_return=True),
                     [param('const std::string', 'option'),
                      param('Dune::Pymor::Parameter', 'mu')],
                     is_const=True, throw=exceptions, custom_name='invert')
    Class.add_method('apply_inverse', None,
                     [param('const ' + RangeType + ' &', 'range'),
                      param(SourceType + ' &', 'source')],
                     is_const=True, throw=exceptions)
    Class.add_method('apply_inverse', None,
                     [param('const ' + RangeType + ' &', 'range'),
                      param(SourceType + ' &', 'source'),
                      param('const std::string', 'option')],
                     is_const=True, throw=exceptions)
    Class.add_method('apply_inverse', None,
                     [param('const ' + RangeType + ' &', 'range'),
                      param(SourceType + ' &', 'source'),
                      param('const std::string', 'option'),
                      param('const Dune::Pymor::Parameter', 'mu')],
                     is_const=True, throw=exceptions)
    Class.add_method('apply_inverse_and_return_ptr',
                     retval(SourceType + ' *', caller_owns_return=True),
                     [param('const ' + RangeType + ' &', 'range')],
                     is_const=True, throw=exceptions, custom_name='apply_inverse')
    Class.add_method('apply_inverse_and_return_ptr',
                     retval(SourceType + ' *', caller_owns_return=True),
                     [param('const ' + RangeType + ' &', 'range'),
                      param('const std::string', 'option')],
                     is_const=True, throw=exceptions, custom_name='apply_inverse')
    Class.add_method('apply_inverse_and_return_ptr',
                     retval(SourceType + ' *', caller_owns_return=True),
                     [param('const ' + RangeType + ' &', 'range'),
                      param('const std::string', 'option'),
                      param('const Dune::Pymor::Parameter', 'mu')],
                     is_const=True, throw=exceptions, custom_name='apply_inverse')
    Class.add_method('freeze_parameter_and_return_ptr',
                     retval(FrozenType + ' *', caller_owns_return=True),
                     [param('Dune::Pymor::Parameter', 'mu')],
                     is_const=True, throw=exceptions, custom_name='freeze_parameter')
    return Class


def wrap_affinely_decomposed_operator(cls, wrapper):

    class WrappedOperator(LincombOperator):
        wrapped_type = cls
        _wrapper = wrapper

        def __init__(self, op):
            self._impl = op
            operators    = [self._wrapper[op.component(i)]   for i in xrange(op.num_components())]
            coefficients = [self._wrapper[op.coefficient(i)] for i in xrange(op.num_components())]
            if op.has_affine_part():
                operators.append(self._wrapper[op.affine_part()])
                coefficients.append(1.)
            LincombOperator.__init__(self, operators, coefficients)

        def with_(self, **kwargs):
            assert 'operators' in kwargs
            ops = kwargs['operators']
            assert len(ops) == len(self.operators)
            return LincombOperator(operators=ops,
                                   coefficients=kwargs['coefficients'] if 'coefficients' in kwargs.keys() else self.coefficients,
                                   name=kwargs['name'] if 'name' in kwargs.keys() else self.name)

        def assemble(self, mu=None):
            mu = self._wrapper.dune_parameter(self.strip_parameter(mu))
            op = self._impl.freeze_parameter(mu)
            return self._wrapper[op]

    WrappedOperator.__name__ = cls.__name__
    return WrappedOperator
