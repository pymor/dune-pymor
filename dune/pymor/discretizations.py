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

from pymor.discretizations.interfaces import DiscretizationInterface
from pymor.discretizations.basic import StationaryDiscretization
from pymor.vectorarrays.list import ListVectorArray
from pymor.vectorarrays.numpy import NumpyVectorArray
from pymor.operators.constructions import induced_norm
from pymor.tools.frozendict import FrozenDict
from pymor.vectorarrays.block import BlockVectorArray
try:
    from pymor.playground.operators import BlockOperator
    BLOCK_OPERATOR_PRESENT=True
except:
    BLOCK_OPERATOR_PRESENT=False


def inject_StationaryDiscretizationImplementation(module, exceptions, interfaces, CONFIG_H,
                                                  name,
                                                  Traits,
                                                  template_parameters=None,
                                                  derives_from_multiscale=False):
    assert(isinstance(module, pybindgen.module.Module))
    assert(isinstance(exceptions, list))
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
    if derives_from_multiscale and not 'Dune::Pymor::Tags::StationaryMultiscaleDiscretiztionInterface' in interfaces:
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
                                        interfaces['Dune::Pymor::Parametric']]
                                       if not derives_from_multiscale else
                                       [interfaces['Dune::Pymor::Tags::StationaryMultiscaleDiscretiztionInterface'],
                                        interfaces['Dune::Pymor::Tags::StationaryDiscretizationInterface'],
                                        interfaces['Dune::Pymor::Parametric']],
                                template_parameters=template_parameters)
    Class.add_method('get_operator_and_return_ptr',
                     retval(OperatorType + ' *', caller_owns_return=True),
                     [],
                     is_const=True, throw=exceptions,
                     custom_name='get_operator')
    Class.add_method('get_rhs_and_return_ptr',
                     retval(FunctionalType + ' *', caller_owns_return=True),
                     [],
                     is_const=True, throw=exceptions,
                     custom_name='get_rhs')
    Class.add_method('available_products',
                     retval('std::vector< std::string >'),
                     [], is_const=True, throw=exceptions)
    Class.add_method('get_product_and_return_ptr',
                     retval(ProductType + ' *', caller_owns_return=True),
                     [param('const std::string', 'id')],
                     is_const=True, throw=exceptions,
                     custom_name='get_product')
    Class.add_method('create_vector_and_return_ptr',
                     retval(VectorType + ' *', caller_owns_return=True),
                     [], is_const=True, throw=exceptions,
                     custom_name='create_vector')
    Class.add_method('solver_types',
                     retval('std::vector< std::string >'),
                     [], is_const=True, throw=exceptions)
    Class.add_method('solver_options',
                     retval('Dune::Stuff::Common::Configuration'),
                     [param('const std::string', 'type')],
                     is_const=True, throw=exceptions)
    Class.add_method('solver_options',
                     retval('Dune::Stuff::Common::Configuration'),
                     [], is_const=True, throw=exceptions)
    Class.add_method('solve',
                     None,
                     [param('const std::string&', 'type'),
                      param(VectorType + ' &', 'vector'),
                      param('Dune::Pymor::Parameter', 'mu')],
                     is_const=True, throw=exceptions)
    Class.add_method('solve',
                     None,
                     [param('const Dune::Stuff::Common::Configuration', 'options'),
                      param(VectorType + ' &', 'vector'),
                      param('Dune::Pymor::Parameter', 'mu')],
                     is_const=True, throw=exceptions)
    Class.add_method('solve',
                     None,
                     [param(VectorType + ' &', 'vector'),
                      param('Dune::Pymor::Parameter', 'mu')],
                     is_const=True, throw=exceptions)
    Class.add_method('solve',
                     None,
                     [param(VectorType + ' &', 'vector')],
                     is_const=True, throw=exceptions)
    Class.add_method('solve',
                     None,
                     [param('const Dune::Stuff::Common::Configuration', 'options'),
                      param(VectorType + ' &', 'vector')],
                     is_const=True, throw=exceptions)

    Class.add_method('solve_and_return_ptr',
                     retval(VectorType + ' *', caller_owns_return=True),
                     [param('const std::string&', 'type'),
                      param('Dune::Pymor::Parameter', 'mu')],
                     is_const=True, throw=exceptions)
    Class.add_method('solve_and_return_ptr',
                     retval(VectorType + ' *', caller_owns_return=True),
                     [param('const Dune::Stuff::Common::Configuration&', 'options'),
                      param('Dune::Pymor::Parameter', 'mu')],
                     is_const=True, throw=exceptions)
    Class.add_method('solve_and_return_ptr',
                     retval(VectorType + ' *', caller_owns_return=True),
                     [param('Dune::Pymor::Parameter', 'mu')],
                     is_const=True, throw=exceptions)
    Class.add_method('solve_and_return_ptr',
                     retval(VectorType + ' *', caller_owns_return=True),
                     [], is_const=True, throw=exceptions)
    Class.add_method('visualize',
                     None,
                     [param('const ' + VectorType + ' &', 'vector'),
                      param('const std::string', 'filename'),
                      param('const std::string', 'name')],
                     is_const=True, throw=exceptions)
    return Class


def wrap_stationary_discretization(cls, wrapper):

    class WrappedDiscretization(DiscretizationInterface):

        wrapped_type = cls

        _wrapper = wrapper

        def __init__(self, d):
            def wrap_op(op):
                if not op.parametric() and op.num_components() == 0 and op.has_affine_part():
                    return self._wrapper[op.affine_part()]
                else:
                    return self._wrapper[op]
            self._impl = d
            operators = {'operator': wrap_op(d.get_operator())}
            functionals = {'rhs': self._wrapper[d.get_rhs()]}
            vector_operators = {}
            self.operators = FrozenDict(operators)
            self.functionals = FrozenDict(functionals)
            self.vector_operators = FrozenDict(vector_operators)
            self.operator = operators['operator']
            self.rhs = functionals['rhs']
            self.products = FrozenDict({k: wrap_op(d.get_product(k)) for k in list(d.available_products())})
            if self.products:
                for k, v in self.products.iteritems():
                    setattr(self, '{}_product'.format(k), v)
                    setattr(self, '{}_norm'.format(k), induced_norm(v))
            self.linear = all(op.linear for op in operators.itervalues())
            self.solution_space = self.operator.source
            self.build_parameter_type(inherits=operators.values())
            assert self.parameter_type == self._wrapper[d.parameter_type()]
            self.solver_options = self._impl.solver_options()

        with_arguments = frozenset({'operators', 'functionals', 'vector_operators', 'solver_options'})

        def with_(self, **kwargs):
            assert 'vector_operators' not in kwargs or not kwargs['vector_operators']
            if 'operators' in kwargs and 'functionals' in kwargs:
                assert 'operators' and 'functionals' in kwargs
                operators = kwargs.pop('operators')
                functionals = kwargs.pop('functionals')
                assert set(operators.keys()) == {'operator'}
                assert set(functionals.keys()) == {'rhs'}
                operator = operators['operator']
                rhs = functionals['rhs']
                # assert all(op.source == NumpyVectorArray for op in {operator, rhs})
                # assert all(op.type_range == NumpyVectorArray for op in {operator, rhs})
                d = StationaryDiscretization(operator=operator, rhs=rhs)
                return d.with_(**kwargs)
            else:
                d = type(self)(self._impl)
                d.unlock()
                for attr in ('solver_options', 'parameter_space'):
                    if attr in dir(self):
                        setattr(d, attr, getattr(self, attr))
                if 'parameter_space' in kwargs:
                    d.parameter_space = kwargs.pop('parameter_space')
                if 'solver_options' in kwargs:
                    d.solver_options = kwargs.pop('solver_options')
                assert len(kwargs) == 0
                d.lock()
                return d

        def solve(self, mu=None):
            mu = self.parse_parameter(mu)
            if not self.logging_disabled:
                self.logger.info('Solving {} for {} ...'.format(self.name, mu))
            mu = self._wrapper.dune_parameter(mu)
            solution = self._impl.solve_and_return_ptr(self.solver_options, mu)
            assert solution.valid()
            solution = self._wrapper[solution]
            return ListVectorArray([solution])

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


if BLOCK_OPERATOR_PRESENT:
    def inject_StationaryMultiscaleDiscretizationImplementation(module, exceptions, interfaces, CONFIG_H,
                                                                name,
                                                                Traits,
                                                                template_parameters=None):
        Class = inject_StationaryDiscretizationImplementation(module, exceptions, interfaces, CONFIG_H, name, Traits,
                                                              template_parameters=template_parameters,
                                                              derives_from_multiscale=True)
        OperatorType = Traits['OperatorType']
        FunctionalType = Traits['FunctionalType']
        ProductType = Traits['ProductType']
        VectorType = Traits['VectorType']
        LocalDiscretizationType = Traits['LocalDiscretizationType']
        OversampledDiscretizationType = Traits['OversampledDiscretizationType']
        ssize_t = CONFIG_H['DUNE_STUFF_SSIZE_T']
        Class.add_method('num_subdomains',
                                     retval(CONFIG_H['DUNE_STUFF_SSIZE_T']),
                                     [], is_const=True, throw=exceptions)
        Class.add_method('neighbouring_subdomains',
                                     retval('std::vector< ' + CONFIG_H['DUNE_STUFF_SSIZE_T'] + ' >'),
                                     [param('const ' + CONFIG_H['DUNE_STUFF_SSIZE_T'], 'ss')],
                                     is_const=True, throw=exceptions)
        Class.add_method('pb_get_local_discretization',
                         retval(LocalDiscretizationType + '*', caller_owns_return=True),
                         [param('const ' + ssize_t, 'subdomain')],
                         is_const=True, throw=exceptions,
                         custom_name='get_local_discretization')
        Class.add_method('pb_get_oversampled_discretization',
                         retval(OversampledDiscretizationType + '*', caller_owns_return=True),
                         [param('const ' + ssize_t, 'subdomain'),
                          param('const std::string', 'boundary_value_type')],
                         is_const=True, throw=exceptions,
                         custom_name='get_oversampled_discretization')
        Class.add_method('get_local_operator_and_return_ptr',
                                     retval(OperatorType + ' *', caller_owns_return=True),
                                     [param('const '+ CONFIG_H['DUNE_STUFF_SSIZE_T'], 'ss')],
                                     is_const=True, throw=exceptions,
                                     custom_name='get_local_operator')
        Class.add_method('get_local_product_and_return_ptr',
                                     retval(ProductType + ' *', caller_owns_return=True),
                                     [param('const ' + CONFIG_H['DUNE_STUFF_SSIZE_T'], 'ss'),
                                      param('const std::string', 'id')],
                                     is_const=True, throw=exceptions,
                                     custom_name='get_local_product')
        Class.add_method('get_coupling_operator_and_return_ptr',
                                     retval(OperatorType + ' *', caller_owns_return=True),
                                     [param('const ' + CONFIG_H['DUNE_STUFF_SSIZE_T'], 'ss'),
                                      param('const ' + CONFIG_H['DUNE_STUFF_SSIZE_T'], 'nn')],
                                     is_const=True, throw=exceptions,
                                     custom_name='get_coupling_operator')
        Class.add_method('get_local_functional_and_return_ptr',
                                     retval(FunctionalType + ' *', caller_owns_return=True),
                                     [param('const ' + CONFIG_H['DUNE_STUFF_SSIZE_T'], 'ss')],
                                     is_const=True, throw=exceptions,
                                     custom_name='get_local_functional')
        Class.add_method('localize_vector_and_return_ptr',
                                     retval(VectorType + ' *', caller_owns_return=True),
                                     [param('const ' + VectorType + ' &',  'global_vector'),
                                      param('const ' + CONFIG_H['DUNE_STUFF_SSIZE_T'], 'ss')],
                                     is_const=True, throw=exceptions,
                                     custom_name='localize_vector')
        Class.add_method('globalize_vectors_and_return_ptr',
                         retval(VectorType + '*', caller_owns_return=True),
                         [param('const std::vector< ' + VectorType + ' >&', 'local_vectors')],
                         is_const=True, throw=exceptions,
                         custom_name='globalize_vectors')
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
                                         for nn in np.arange(d.num_subdomains())] for ss in np.arange(d.num_subdomains())])
                rhs_op = BlockOperator([wrapper[d.get_local_functional(ss)] for ss in np.arange(d.num_subdomains())])
                operators = {'operator': lhs_op}
                functionals = {'rhs': rhs_op}
                vector_operators = {}
                self.operators = FrozenDict(operators)
                self.functionals = FrozenDict(functionals)
                self.vector_operators = FrozenDict(vector_operators)
                self.operator = operators['operator']
                self.solution_space = self.operator.source
                self.rhs = functionals['rhs']
                self.products = {k: BlockOperator([[wrapper[d.get_local_product(ss, k)] if ss == nn else None
                                                    for nn in np.arange(d.num_subdomains())]
                                                   for ss in np.arange(d.num_subdomains())])
                                    for k in list(d.available_products())}
                if self.products:
                    for k, v in self.products.iteritems():
                        setattr(self, '{}_product'.format(k), v)
                        setattr(self, '{}_norm'.format(k), induced_norm(v))
                self.linear = all(op.linear for op in operators.itervalues())
                self.num_subdomains = self._impl.num_subdomains()
                self.neighboring_subdomains = [self._impl.neighbouring_subdomains(ss)
                                               for ss in np.arange(self.num_subdomains)]
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
                    assert all(op.source.type == NumpyVectorArray or op.source.type == BlockVectorArray for op in (operator, rhs))
                    assert all(op.range.type  == NumpyVectorArray or op.range.type  == BlockVectorArray for op in (operator, rhs))
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
                global_solution = self._impl.solve_and_return_ptr(mu)
                assert global_solution.valid()
                global_solution = self._wrapper[global_solution]
                global_solution = ListVectorArray(global_solution)
                return BlockVectorArray([self.localize_vector(global_solution, ss)
                                         for ss in np.arange(self._impl.num_subdomains())])

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
                if len(global_vector) != 1:
                    raise NotImplementedError
                assert subdomain < self.num_subdomains
                return ListVectorArray(self._wrapper[self._impl.localize_vector(global_vector._list[0]._impl,
                                                                                subdomain)])
            def globalize_vectors(self, local_vectors):
                assert isinstance(local_vectors, BlockVectorArray)
                if len(local_vectors) != 1:
                    raise NotImplementedError
                global_vector = self._impl.globalize_vectors([block._list[0]._impl for block in local_vectors._blocks])
                return ListVectorArray([self._wrapper[global_vector]])

            def local_product(self, subdomain, id):
                assert subdomain < self.num_subdomains
                assert id in list(self._impl.available_products())
                return self._wrapper[self._impl.get_local_product(subdomain, id)]

            def local_operator(self, subdomain):
                assert subdomain < self.num_subdomains
                return self._wrapper[self._impl.get_local_operator(subdomain)]

            def local_rhs(self, subdomain):
                assert subdomain < self.num_subdomains
                return self._wrapper[self._impl.get_local_functional(subdomain)]

            def coupling_operator(self, subdomain, neighbor):
                assert subdomain < self.num_subdomains
                assert neighbor in self.neighboring_subdomains[subdomain]
                return self._wrapper[self._impl.get_coupling_operator(subdomain, neighbour)]

            def as_nonblocked(self):
                return wrap_stationary_discretization(self.wrapped_type, self._wrapper)(self._impl)

            def get_local_discretization(self, subdomain):
                return self._wrapper[self._impl.get_local_discretization(subdomain)]

            def get_oversampled_discretization(self, subdomain, boundary_value_type):
                return self._wrapper[self._impl.get_oversampled_discretization(subdomain, boundary_value_type)]


        WrappedDiscretization.__name__ = cls.__name__
        return WrappedDiscretization

