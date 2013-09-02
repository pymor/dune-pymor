#! /usr/bin/env python
# This file is part of the dune-pymor project:
#   https://github.com/pyMor/dune-pymor
# Copyright Holders: Felix Albrecht, Stephan Rave
# License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

from tempfile import mkstemp
import os
import subprocess

import pybindgen
from pybindgen import retval, param

from pymor.discretizations import DiscretizationInterface, StationaryDiscretization
from pymor.la import NumpyVectorArray, induced_norm
from pymor.tools.frozendict import FrozenDict


def inject_StationaryDiscretizationImplementation(module, exceptions, interfaces, CONFIG_H,
                                                  name,
                                                  Traits,
                                                  template_parameters=None):
    assert(isinstance(module, pybindgen.module.Module))
    assert(isinstance(exceptions, dict))
    assert(isinstance(interfaces, dict))
    for element in interfaces:
        assert(isinstance(element, str))
        assert(len(element) > 0)
    assert(isinstance(CONFIG_H, dict))
    assert(isinstance(name, str))
    assert(len(name.strip()) > 0)
    assert(isinstance(Traits, dict))
    for key in Traits.keys():
        assert(isinstance(Traits[key], str))
        assert(len(Traits[key].strip()) > 0)
    assert('OperatorType' in Traits)
    OperatorType = Traits['OperatorType']
    assert('FunctionalType' in Traits)
    FunctionalType = Traits['FunctionalType']
    assert('ProductType' in Traits)
    ProductType = Traits['ProductType']
    assert('VectorType' in Traits)
    VectorType = Traits['VectorType']
    if template_parameters is not None:
        if isinstance(template_parameters, str):
            assert(len(template_parameters.strip()) > 0)
            template_parameters = [ template_parameters ]
        elif isinstance(template_parameters, list):
            for element in template_parameters:
                assert(isinstance(element, str))
                assert(len(element.strip()) > 0)
    # add interface if necessary
    if not 'Dune::Pymor::StationaryDiscretizationInterfaceDynamic' in interfaces:
        (interfaces['Dune::Pymor::StationaryDiscretizationInterfaceDynamic']
         ) = module.add_cpp_namespace('Dune').add_cpp_namespace('Pymor').add_class('StationaryDiscretizationInterfaceDynamic')
    namespace = module
    namespaces = [nspace.strip() for nspace in name.split('::')[:-1]]
    name = name.split('::')[-1].strip()
    if len(namespaces) > 0:
        for nspace in namespaces:
            namespace = namespace.add_cpp_namespace(nspace)
    Class = namespace.add_class(name,
                                parent=[interfaces['Dune::Pymor::StationaryDiscretizationInterfaceDynamic'],
                                        interfaces['Dune::Pymor::Parametric']],
                                template_parameters=template_parameters)
    Class.add_method('get_operator_and_return_ptr',
                     retval(OperatorType + ' *', caller_owns_return=True),
                     [],
                     is_const=True, throw=[exceptions['PymorException']],
                     custom_name='get_operator')
    Class.add_method('get_rhs_and_return_ptr',
                     retval(FunctionalType + ' *', caller_owns_return=True),
                     [],
                     is_const=True, throw=[exceptions['PymorException']],
                     custom_name='get_rhs')
    Class.add_method('available_products',
                     retval('std::vector< std::string >'),
                     [], is_const=True, throw=[exceptions['PymorException']])
    Class.add_method('get_product_and_return_ptr',
                     retval(ProductType + ' *', caller_owns_return=True),
                     [param('const std::string', 'id')],
                     is_const=True, throw=[exceptions['PymorException']],
                     custom_name='get_product')
    Class.add_method('create_vector_and_return_ptr',
                     retval(VectorType + ' *', caller_owns_return=True),
                     [], is_const=True, throw=[exceptions['PymorException']],
                     custom_name='create_vector')
    #Class.add_method('solver_options',
                     #retval('std::vector< std::string >'),
                     #[], is_const=True, throw=[exceptions['PymorException']])
    #Class.add_method('solver_options',
                     #retval('std::string'),
                     #[param('const std::string', 'context')],
                     #is_const=True, throw=[exceptions['PymorException']])
    Class.add_method('solve',
                     None,
                     [param(VectorType + ' &', 'vector')],
                     is_const=True, throw=[exceptions['PymorException']])
    Class.add_method('solve',
                     None,
                     [param(VectorType + ' &', 'vector'),
                      param('Dune::Pymor::Parameter', 'mu')],
                     is_const=True, throw=[exceptions['PymorException']])
    Class.add_method('solve_and_return_ptr',
                     retval(VectorType + ' *', caller_owns_return=True),
                     [], is_const=True, throw=[exceptions['PymorException']])
    Class.add_method('solve_and_return_ptr',
                     retval(VectorType + ' *', caller_owns_return=True),
                     [param('Dune::Pymor::Parameter', 'mu')],
                     is_const=True, throw=[exceptions['PymorException']])
    Class.add_method('visualize',
                     None,
                     [param('const ' + VectorType + ' &', 'vector'),
                      param('const std::string', 'filename'),
                      param('const std::string', 'name')],
                     is_const=True, throw=[exceptions['PymorException']])
    return Class


def wrap_stationary_discretization(cls, wrapper):

    class WrappedDiscretization(DiscretizationInterface):

        wrapped_type = cls

        _wrapper = wrapper

        def __init__(self, d):
            self._impl = d
            operators = {'operator': self._wrapper[d.get_operator()],
                         'rhs': self._wrapper[d.get_rhs()]}
            self.operators = FrozenDict(operators)
            self.operator = operators['operator']
            self.rhs = operators['rhs']
            self.products = {k: self._wrapper[d.get_product(k)] for k in list(d.available_products())}
            if self.products:
                for k, v in self.products.iteritems():
                    setattr(self, '{}_product'.format(k), v)
                    setattr(self, '{}_norm'.format(k), induced_norm(v))
            self.linear = all(op.linear for op in operators.itervalues())
            self.build_parameter_type(inherits=operators.values())
            assert self.parameter_type == self._wrapper[d.parameter_type()]
            self.lock()

        with_arguments = StationaryDiscretization.with_arguments

        def with_(self, **kwargs):
            assert 'operators' in kwargs or kwargs.keys() == ['parameter_space']
            if 'operators' in kwargs:
                operators = kwargs.pop('operators')
                assert set(operators.keys()) == {'operator', 'rhs'}
                assert all(op.type_source == NumpyVectorArray for op in operators.itervalues())
                assert all(op.type_range == NumpyVectorArray for op in operators.itervalues())
                d = StationaryDiscretization(operator=operators['operator'], rhs=operators['rhs'])
                return d.with_(**kwargs)
            else:
                d = type(self)(self._impl)
                d.unlock()
                d.parameter_space = kwargs['parameter_space']
                d.lock()
                return d

        def solve(self, mu=None):
            mu = self.parse_parameter(mu)
            if not self.logging_disabled:
                self.logger.info('Solving {} for {} ...'.format(self.name, mu))
            mu = self._wrapper.dune_parameter(mu)
            return self._wrapper.vector_array(self._wrapper[self._impl.solve_and_return_ptr(mu)])

        _solve = solve

        def visualize(self, U, file_name=None, name='solution', delete=True):
            assert len(U) == 1
            if file_name is None:
                _, file_name = mkstemp(suffix='.vtu')
            if not file_name.endswith('.vtu'):
                file_name = file_name + '.vtu'
            self._impl.visualize(U._list[0]._impl, file_name[:-4], name)
            subprocess.call(['paraview', file_name])
            if delete:
                os.remove(file_name)


    WrappedDiscretization.__name__ = cls.__name__
    return WrappedDiscretization
