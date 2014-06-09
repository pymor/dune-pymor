#! /usr/bin/env python
# This file is part of the dune-pymor project:
#   https://github.com/pymor/dune-pymor
# Copyright Holders: Stephan Rave, Felix Schindler
# License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

from tempfile import mkstemp
import os
import subprocess

import pybindgen
from pybindgen import retval, param
import numpy as np

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
    if not 'Dune::Pymor::Tags::StationaryDiscretizationInterface' in interfaces:
        (interfaces['Dune::Pymor::Tags::StationaryDiscretizationInterface']
         ) = module.add_cpp_namespace('Dune').add_cpp_namespace('Pymor').add_cpp_namespace('Tags').add_class('StationaryDiscretizationInterface')
    namespace = module
    namespaces = [nspace.strip() for nspace in name.split('::')[:-1]]
    name = name.split('::')[-1].strip()
    if len(namespaces) > 0:
        for nspace in namespaces:
            namespace = namespace.add_cpp_namespace(nspace)
    Class = namespace.add_class(name,
                                parent=[interfaces['Dune::Pymor::Tags::StationaryDiscretizationInterface'],
                                        interfaces['Dune::Pymor::Parametric']],
                                template_parameters=template_parameters)
    Class.add_method('get_operator_and_return_ptr',
                     retval(OperatorType + ' *', caller_owns_return=True),
                     [],
                     is_const=True, throw=[exceptions['Exception']],
                     custom_name='get_operator')
    Class.add_method('get_rhs_and_return_ptr',
                     retval(FunctionalType + ' *', caller_owns_return=True),
                     [],
                     is_const=True, throw=[exceptions['Exception']],
                     custom_name='get_rhs')
    Class.add_method('available_products',
                     retval('std::vector< std::string >'),
                     [], is_const=True, throw=[exceptions['Exception']])
    Class.add_method('get_product_and_return_ptr',
                     retval(ProductType + ' *', caller_owns_return=True),
                     [param('const std::string', 'id')],
                     is_const=True, throw=[exceptions['Exception']],
                     custom_name='get_product')
    Class.add_method('create_vector_and_return_ptr',
                     retval(VectorType + ' *', caller_owns_return=True),
                     [], is_const=True, throw=[exceptions['Exception']],
                     custom_name='create_vector')
    #Class.add_method('solver_options',
                     #retval('std::vector< std::string >'),
                     #[], is_const=True, throw=[exceptions['Exception']])
    #Class.add_method('solver_options',
                     #retval('std::string'),
                     #[param('const std::string', 'context')],
                     #is_const=True, throw=[exceptions['Exception']])
    Class.add_method('solve',
                     None,
                     [param(VectorType + ' &', 'vector')],
                     is_const=True, throw=[exceptions['Exception']])
    Class.add_method('solve',
                     None,
                     [param(VectorType + ' &', 'vector'),
                      param('Dune::Pymor::Parameter', 'mu')],
                     is_const=True, throw=[exceptions['Exception']])
    Class.add_method('solve_and_return_ptr',
                     retval(VectorType + ' *', caller_owns_return=True),
                     [], is_const=True, throw=[exceptions['Exception']])
    Class.add_method('solve_and_return_ptr',
                     retval(VectorType + ' *', caller_owns_return=True),
                     [param('Dune::Pymor::Parameter', 'mu')],
                     is_const=True, throw=[exceptions['Exception']])
    Class.add_method('visualize',
                     None,
                     [param('const ' + VectorType + ' &', 'vector'),
                      param('const std::string', 'filename'),
                      param('const std::string', 'name')],
                     is_const=True, throw=[exceptions['Exception']])
    return Class


def wrap_stationary_discretization(cls, wrapper):

    class WrappedDiscretization(DiscretizationInterface):

        wrapped_type = cls

        _wrapper = wrapper

        def __init__(self, d):
            self._impl = d
            operators = {'operator': self._wrapper[d.get_operator()]}
            functionals = {'rhs': self._wrapper[d.get_rhs()]}
            vector_operators = {}
            self.operators = FrozenDict(operators)
            self.functionals = FrozenDict(functionals)
            self.vector_operators = FrozenDict(vector_operators)
            self.operator = operators['operator']
            self.rhs = functionals['rhs']
            self.products = FrozenDict({k: self._wrapper[d.get_product(k)] for k in list(d.available_products())})
            if self.products:
                for k, v in self.products.iteritems():
                    setattr(self, '{}_product'.format(k), v)
                    setattr(self, '{}_norm'.format(k), induced_norm(v))
            self.linear = all(op.linear for op in operators.itervalues())
            self.dim_solution = self.operator.dim_source
            self.type_solution = self.operator.type_source
            self.build_parameter_type(inherits=operators.values())
            assert self.parameter_type == self._wrapper[d.parameter_type()]

        with_arguments = StationaryDiscretization.with_arguments

        def with_(self, **kwargs):
            assert 'operators' in kwargs and 'functionals' in kwargs or kwargs.keys() == ['parameter_space']
            assert 'vector_operators' not in kwargs or not kwargs['vector_operators']
            if 'operators' in kwargs:
                operators = kwargs.pop('operators')
                functionals = kwargs.pop('functionals')
                assert set(operators.keys()) == {'operator'}
                assert set(functionals.keys()) == {'rhs'}
                operator = operators['operator']
                rhs = functionals['rhs']
                assert all(op.type_source == NumpyVectorArray for op in {operator, rhs})
                assert all(op.type_range == NumpyVectorArray for op in {operator, rhs})
                d = StationaryDiscretization(operator=operator, rhs=rhs)
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

try:
    from pymor.la.blockvectorarray import BlockVectorArray
    from pymor.operators.block import BlockOperator


    def inject_StationaryMultiscaleDiscretizationImplementation(module, exceptions, interfaces, CONFIG_H,
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
        if not 'Dune::Pymor::Tags::StationaryDiscretizationInterface' in interfaces:
            (interfaces['Dune::Pymor::Tags::StationaryDiscretizationInterface']
             ) = module.add_cpp_namespace('Dune').add_cpp_namespace('Pymor').add_cpp_namespace('Tags').add_class('StationaryDiscretizationInterface')
        if not 'Dune::Pymor::Tags::StationaryMultiscaleDiscretiztionInterface' in interfaces:
            (interfaces['Dune::Pymor::Tags::StationaryMultiscaleDiscretiztionInterface']
             ) = module.add_cpp_namespace('Dune').add_cpp_namespace('Pymor').add_cpp_namespace('Tags').add_class('StationaryMultiscaleDiscretiztionInterface')
        namespace = module
        namespaces = [nspace.strip() for nspace in name.split('::')[:-1]]
        name = name.split('::')[-1].strip()
        if len(namespaces) > 0:
            for nspace in namespaces:
                namespace = namespace.add_cpp_namespace(nspace)
        Class = namespace.add_class(name,
                                    parent=[interfaces['Dune::Pymor::Tags::StationaryDiscretizationInterface'],
                                            interfaces['Dune::Pymor::Parametric'],
                                            interfaces['Dune::Pymor::Tags::StationaryMultiscaleDiscretiztionInterface']],
                                    template_parameters=template_parameters)
        Class.add_method('get_operator_and_return_ptr',
                         retval(OperatorType + ' *', caller_owns_return=True),
                         [],
                         is_const=True, throw=[exceptions['Exception']],
                         custom_name='get_operator')
        Class.add_method('get_rhs_and_return_ptr',
                         retval(FunctionalType + ' *', caller_owns_return=True),
                         [],
                         is_const=True, throw=[exceptions['Exception']],
                         custom_name='get_rhs')
        Class.add_method('available_products',
                         retval('std::vector< std::string >'),
                         [], is_const=True, throw=[exceptions['Exception']])
        Class.add_method('get_product_and_return_ptr',
                         retval(ProductType + ' *', caller_owns_return=True),
                         [param('const std::string', 'id')],
                         is_const=True, throw=[exceptions['Exception']],
                         custom_name='get_product')
        Class.add_method('create_vector_and_return_ptr',
                         retval(VectorType + ' *', caller_owns_return=True),
                         [], is_const=True, throw=[exceptions['Exception']],
                         custom_name='create_vector')
        #Class.add_method('solver_options',
                         #retval('std::vector< std::string >'),
                         #[], is_const=True, throw=[exceptions['Exception']])
        #Class.add_method('solver_options',
                         #retval('std::string'),
                         #[param('const std::string', 'context')],
                         #is_const=True, throw=[exceptions['Exception']])
        Class.add_method('solve',
                         None,
                         [param(VectorType + ' &', 'vector')],
                         is_const=True, throw=[exceptions['Exception']])
        Class.add_method('solve',
                         None,
                         [param(VectorType + ' &', 'vector'),
                          param('Dune::Pymor::Parameter', 'mu')],
                         is_const=True, throw=[exceptions['Exception']])
        Class.add_method('solve_and_return_ptr',
                         retval(VectorType + ' *', caller_owns_return=True),
                         [], is_const=True, throw=[exceptions['Exception']])
        Class.add_method('solve_and_return_ptr',
                         retval(VectorType + ' *', caller_owns_return=True),
                         [param('Dune::Pymor::Parameter', 'mu')],
                         is_const=True, throw=[exceptions['Exception']])
        Class.add_method('visualize',
                         None,
                         [param('const ' + VectorType + ' &', 'vector'),
                          param('const std::string', 'filename'),
                          param('const std::string', 'name')],
                         is_const=True, throw=[exceptions['Exception']])
        Class.add_method('num_subdomains',
                                     retval(CONFIG_H['DUNE_STUFF_SSIZE_T']),
                                     [], is_const=True, throw=[exceptions['Exception']])
        Class.add_method('neighbouring_subdomains',
                                     retval('std::vector< ' + CONFIG_H['DUNE_STUFF_SSIZE_T'] + ' >'),
                                     [param('const ' + CONFIG_H['DUNE_STUFF_SSIZE_T'], 'ss')],
                                     is_const=True, throw=[exceptions['Exception']])
        Class.add_method('get_local_operator_and_return_ptr',
                                     retval(OperatorType + ' *', caller_owns_return=True),
                                     [param('const '+ CONFIG_H['DUNE_STUFF_SSIZE_T'], 'ss')],
                                     is_const=True, throw=[exceptions['Exception']],
                                     custom_name='get_local_operator')
        Class.add_method('get_local_product_and_return_ptr',
                                     retval(ProductType + ' *', caller_owns_return=True),
                                     [param('const ' + CONFIG_H['DUNE_STUFF_SSIZE_T'], 'ss'),
                                      param('const std::string', 'id')],
                                     is_const=True, throw=[exceptions['Exception']],
                                     custom_name='get_local_product')
        Class.add_method('get_coupling_operator_and_return_ptr',
                                     retval(OperatorType + ' *', caller_owns_return=True),
                                     [param('const ' + CONFIG_H['DUNE_STUFF_SSIZE_T'], 'ss'),
                                      param('const ' + CONFIG_H['DUNE_STUFF_SSIZE_T'], 'nn')],
                                     is_const=True, throw=[exceptions['Exception']],
                                     custom_name='get_coupling_operator')
        Class.add_method('get_local_functional_and_return_ptr',
                                     retval(FunctionalType + ' *', caller_owns_return=True),
                                     [param('const ' + CONFIG_H['DUNE_STUFF_SSIZE_T'], 'ss')],
                                     is_const=True, throw=[exceptions['Exception']],
                                     custom_name='get_local_functional')
        Class.add_method('localize_vector_and_return_ptr',
                                     retval(VectorType + ' *', caller_owns_return=True),
                                     [param('const ' + VectorType + ' &',  'global_vector'),
                                      param('const ' + CONFIG_H['DUNE_STUFF_SSIZE_T'], 'ss')],
                                     is_const=True, throw=[exceptions['Exception']],
                                     custom_name='localize_vector')
        return Class

    def wrap_multiscale_discretization(cls, wrapper):

        class WrappedDiscretization(DiscretizationInterface):

            wrapped_type = cls

            _wrapper = wrapper

            def __init__(self, d):
                self._impl = d
                lhs_op = BlockOperator([[wrapper[d.get_local_operator(ss)] if ss == nn
                                         else wrapper[d.get_coupling_operator(ss, nn)] if nn in list(d.neighbouring_subdomains(ss))
                                         else None
                                         for nn in np.arange(d.num_subdomains())] for ss in np.arange(d.num_subdomains())],
                                       [d.get_local_operator(ss).dim_source() for ss in np.arange(d.num_subdomains())],
                                       [d.get_local_operator(ss).dim_range() for ss in np.arange(d.num_subdomains())])
                rhs_op = BlockOperator([wrapper[d.get_local_functional(ss)] for ss in np.arange(d.num_subdomains())])
                operators = {'operator': lhs_op}
                functionals = {'rhs': rhs_op}
                vector_operators = {}
                self.operators = FrozenDict(operators)
                self.functionals = FrozenDict(functionals)
                self.vector_operators = FrozenDict(vector_operators)
                self.operator = operators['operator']
                self.rhs = functionals['rhs']
                self.products = {k: BlockOperator([[wrapper[d.get_local_product(ss, k)] if ss == nn else None
                                                    for nn in np.arange(d.num_subdomains())]
                                                   for ss in np.arange(d.num_subdomains())],
                                                  [d.get_local_operator(ss).dim_source() for ss in np.arange(d.num_subdomains())],
                                                  [d.get_local_operator(ss).dim_range() for ss in np.arange(d.num_subdomains())])
                                    for k in list(d.available_products())}
                if self.products:
                    for k, v in self.products.iteritems():
                        setattr(self, '{}_product'.format(k), v)
                        setattr(self, '{}_norm'.format(k), induced_norm(v))
                self.linear = all(op.linear for op in operators.itervalues())
                self.build_parameter_type(inherits=operators.values())
                assert self.parameter_type == self._wrapper[d.parameter_type()]

            with_arguments = StationaryDiscretization.with_arguments

            def with_(self, **kwargs):
                assert 'operators' and 'functionals' in kwargs or kwargs.keys() == ['parameter_space']
                assert 'vector_operators' not in kwargs or not kwargs['vector_operators']
                if 'operators' in kwargs:
                    operators = kwargs.pop('operators')
                    functionals = kwargs.pop('functionals')
                    assert set(operators.keys()) == {'operator'}
                    assert set(functionals.keys()) == {'rhs'}
                    operator = operators['operator']
                    rhs = functionals['rhs']
                    assert all(op.type_source == NumpyVectorArray or op.type_source == BlockVectorArray for op in (operator, rhs))
                    assert all(op.type_range == NumpyVectorArray or op.type_range == BlockVectorArray for op in (operator, rhs))
                    d = StationaryDiscretization(operator=operator, rhs=rhs)
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
                global_solution = self._wrapper.vector_array(self._wrapper[self._impl.solve_and_return_ptr(mu)])
                return BlockVectorArray([self.localize_vector(global_solution, ss) for ss in np.arange(self._impl.num_subdomains())])

            _solve = solve

            def visualize(self, U, file_name=None, name='solution', delete=True):
                raise Exception('Not implemented yet!')
                assert len(U) == 1
                if file_name is None:
                    _, file_name = mkstemp(suffix='.vtu')
                if not file_name.endswith('.vtu'):
                    file_name = file_name + '.vtu'
                self._impl.visualize(U._list[0]._impl, file_name[:-4], name)
                subprocess.call(['paraview', file_name])
                if delete:
                    os.remove(file_name)

            def localize_vector(self, global_vector, subdomain):
                assert len(global_vector) == 1
                return self._wrapper.vector_array(self._wrapper[self._impl.localize_vector(global_vector._list[0]._impl,
                                                                                           subdomain)])

            def local_product(self, subdomain, id):
                assert subdomain < self._impl.num_subdomains()
                assert id in { 'l2', 'h1_semi' }
                return self._wrapper[self._impl.get_local_product(subdomain, id)]

            def local_operator(self, subdomain):
                assert subdomain < self._impl.num_subdomains()
                return self._wrapper[self._impl.get_local_operator(subdomain)]

            def local_rhs(self, subdomain):
                assert subdomain < self._impl.num_subdomains()
                return self._wrapper[self._impl.get_local_functional(subdomain)]

            def coupling_operator(self, subdomain, neighbour):
                assert subdomain < self._impl.num_subdomains()
                assert neighbour in self._impl.neighbouring_subdomains(subdomain)
                return self._wrapper[self._impl.get_coupling_operator(subdomain, neighbour)]


        WrappedDiscretization.__name__ = cls.__name__
        return WrappedDiscretization

except ImportError:
    pass
