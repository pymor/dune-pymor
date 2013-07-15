#! /usr/bin/env python
# This file is part of the dune-pymor project:
#   https://github.com/pyMor/dune-pymor
# Copyright Holders: Felix Albrecht, Stephan Rave
# License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

import pybindgen
from pybindgen import retval, param


def inject_VectorInterface(module, exceptions, CONFIG_H, FunctionalInterface):
    assert(isinstance(module, pybindgen.module.Module))
    assert(isinstance(exceptions, dict))
    assert(isinstance(CONFIG_H, dict))
    assert(isinstance(FunctionalInterface, pybindgen.CppClass))
    namespace = module.add_cpp_namespace('Dune').add_cpp_namespace('Pymor').add_cpp_namespace('LA')
    VectorInterface = namespace.add_class('VectorInterface', parent=FunctionalInterface)
    VectorInterface.add_method('almost_equal',
                               retval('bool'),
                               [param('const Dune::Pymor::LA::VectorInterface *', 'other', transfer_ownership=False),
                                param('const double', 'epsilon')],
                               is_const=True,
                               throw=[exceptions['types_are_not_compatible'],
                                      exceptions['sizes_do_not_match'],
                                      exceptions['you_have_to_implement_this']])
    VectorInterface.add_method('almost_equal',
                               retval('bool'),
                               [param('const Dune::Pymor::LA::VectorInterface *', 'other', transfer_ownership=False)],
                               is_const=True,
                               throw=[exceptions['types_are_not_compatible'],
                                      exceptions['sizes_do_not_match'],
                                      exceptions['you_have_to_implement_this']])
    VectorInterface.add_method('axpy',
                               None,
                               [param('const double', 'alpha'),
                                param('const Dune::Pymor::LA::VectorInterface *', 'x', transfer_ownership=False)],
                               throw=[exceptions['types_are_not_compatible'],
                                      exceptions['sizes_do_not_match']])
    VectorInterface.add_method('dot',
                               retval('double'),
                               [param('const Dune::Pymor::LA::VectorInterface *', 'other', transfer_ownership=False)],
                               is_const=True,
                               throw=[exceptions['types_are_not_compatible'],
                                      exceptions['sizes_do_not_match'],
                                      exceptions['you_have_to_implement_this']])
    VectorInterface.add_method('add',
                               retval('Dune::Pymor::LA::VectorInterface *', caller_owns_return=True),
                               [param('const Dune::Pymor::LA::VectorInterface *', 'other', transfer_ownership=False)],
                               is_const=True,
                               throw=[exceptions['types_are_not_compatible'],
                                      exceptions['sizes_do_not_match'],
                                      exceptions['you_have_to_implement_this']])
    VectorInterface.add_method('iadd',
                               None,
                               [param('const Dune::Pymor::LA::VectorInterface *', 'other', transfer_ownership=False)],
                               throw=[exceptions['types_are_not_compatible'],
                                      exceptions['sizes_do_not_match'],
                                      exceptions['you_have_to_implement_this']])
    VectorInterface.add_method('sub',
                               retval('Dune::Pymor::LA::VectorInterface *', caller_owns_return=True),
                               [param('const Dune::Pymor::LA::VectorInterface *', 'other', transfer_ownership=False)],
                               throw=[exceptions['types_are_not_compatible'],
                                      exceptions['sizes_do_not_match'],
                                      exceptions['you_have_to_implement_this']],
                               is_const=True)
    VectorInterface.add_method('isub',
                               None,
                               [param('const Dune::Pymor::LA::VectorInterface *', 'other', transfer_ownership=False)],
                               throw=[exceptions['types_are_not_compatible'],
                                      exceptions['sizes_do_not_match'],
                                      exceptions['you_have_to_implement_this']])
    VectorInterface.add_method('linear', retval('bool'), [], is_const=True)
    VectorInterface.add_method('dim_source', retval('unsigned int'), [], is_const=True)
    VectorInterface.add_method('type_source', retval('std::string'), [], is_const=True)
    VectorInterface.add_method('apply',
                               retval('double'),
                               [param('const Dune::Pymor::LA::VectorInterface *', 'source')],
                               throw=[exceptions['types_are_not_compatible'],
                                      exceptions['you_have_to_implement_this'],
                                      exceptions['sizes_do_not_match'],
                                      exceptions['wrong_parameter_type'],
                                      exceptions['requirements_not_met'],
                                      exceptions['linear_solver_failed'],
                                      exceptions['this_does_not_make_any_sense']],
                               is_const=True)
    VectorInterface.add_method('apply',
                               retval('double'),
                               [param('const Dune::Pymor::LA::VectorInterface *', 'source'),
                                param('const Dune::Pymor::Parameter', 'mu')],
                               throw=[exceptions['types_are_not_compatible'],
                                      exceptions['you_have_to_implement_this'],
                                      exceptions['sizes_do_not_match'],
                                      exceptions['wrong_parameter_type'],
                                      exceptions['requirements_not_met'],
                                      exceptions['linear_solver_failed'],
                                      exceptions['this_does_not_make_any_sense']],
                               is_const=True)
    return module, VectorInterface


def inject_derived_from_VectorInterface(module, exceptions, CONFIG_H, VectorInterface, name):
    assert(isinstance(module, pybindgen.module.Module))
    assert(isinstance(exceptions, dict))
    assert(isinstance(CONFIG_H, dict))
    assert(isinstance(VectorInterface, pybindgen.CppClass))
    assert(len(name.strip()) > 0)
    namespace = module
    namespaces = [nspace.strip() for nspace in name.split('::')[:-1]]
    name = name.split('::')[-1].strip()
    full_name = name
    if len(namespaces) > 0:
        full_name = '::'.join(namespaces) + '::' + name
    for nspace in namespaces:
        namespace = namespace.add_cpp_namespace(nspace)
    Vector = namespace.add_class(name, parent=VectorInterface)
    Vector.add_constructor([param('const int', 'ss')])
    Vector.add_constructor([param('const int', 'ss'), param('const double', 'value')])
    Vector.add_method('type', retval('std::string'), [], is_const=True)
    Vector.add_method('dim', retval('unsigned int'), [], is_const=True)
    Vector.add_method('almost_equal',
                      retval('bool'),
                      [param('const ' + full_name + ' *', 'other', transfer_ownership=False),
                       param('const double', 'epsilon')],
                      is_const=True,
                      throw=[exceptions['types_are_not_compatible'],
                             exceptions['sizes_do_not_match']])
    Vector.add_method('almost_equal',
                      retval('bool'),
                      [param('const ' + full_name + ' *', 'other', transfer_ownership=False)],
                      is_const=True,
                      throw=[exceptions['types_are_not_compatible'],
                             exceptions['sizes_do_not_match']])
    Vector.add_method('scal', None, [param('const double', 'alpha')])
    Vector.add_method('axpy',
                      None,
                      [param('const double', 'alpha'),
                       param('const ' + full_name + ' *', 'x', transfer_ownership=False)],
                      throw=[exceptions['types_are_not_compatible'],
                             exceptions['sizes_do_not_match']])
    Vector.add_method('dot',
                      retval('double'),
                      [param('const ' + full_name + ' *', 'other', transfer_ownership=False)],
                      is_const=True,
                      throw=[exceptions['types_are_not_compatible'],
                             exceptions['sizes_do_not_match']])
    Vector.add_method('l1_norm', retval('double'), [], is_const=True)
    Vector.add_method('l2_norm', retval('double'), [], is_const=True)
    Vector.add_method('sup_norm', retval('double'), [], is_const=True)
    Vector.add_method('components',
                      retval('std::vector< double >'),
                      [param('const std::vector< int >', 'component_indices')],
                      throw=[exceptions['sizes_do_not_match'],
                             exceptions['index_out_of_range']],
                      is_const=True)
    Vector.add_method('amax', retval('std::vector< double >'), [], is_const=True)
    Vector.add_method('add',
                      retval(full_name + ' *', caller_owns_return=True),
                      [param('const ' + full_name + ' *', 'other', transfer_ownership=False)],
                      is_const=True,
                      throw=[exceptions['types_are_not_compatible'],
                             exceptions['sizes_do_not_match']])
    Vector.add_method('iadd',
                      None,
                      [param('const ' + full_name + ' *', 'other', transfer_ownership=False)],
                      throw=[exceptions['types_are_not_compatible'],
                             exceptions['sizes_do_not_match']])
    Vector.add_method('sub',
                      retval(full_name + ' *', caller_owns_return=True),
                      [param('const ' + full_name + ' *', 'other', transfer_ownership=False)],
                      throw=[exceptions['types_are_not_compatible'],
                             exceptions['sizes_do_not_match']],
                      is_const=True)
    Vector.add_method('isub',
                      None,
                      [param('const ' + full_name + ' *', 'other', transfer_ownership=False)],
                      throw=[exceptions['types_are_not_compatible'],
                             exceptions['sizes_do_not_match']])
    Vector.add_method('apply',
                      retval('double'),
                      [param('const ' + full_name + ' *', 'source')],
                      throw=[exceptions['types_are_not_compatible'],
                             exceptions['you_have_to_implement_this'],
                             exceptions['sizes_do_not_match'],
                             exceptions['wrong_parameter_type'],
                             exceptions['requirements_not_met'],
                             exceptions['linear_solver_failed'],
                             exceptions['this_does_not_make_any_sense']],
                      is_const=True)
    Vector.add_method('apply',
                      retval('double'),
                      [param('const ' + full_name + ' *', 'source'),
                       param('const Dune::Pymor::Parameter', 'mu')],
                      throw=[exceptions['types_are_not_compatible'],
                             exceptions['you_have_to_implement_this'],
                             exceptions['sizes_do_not_match'],
                             exceptions['wrong_parameter_type'],
                             exceptions['requirements_not_met'],
                             exceptions['linear_solver_failed'],
                             exceptions['this_does_not_make_any_sense']],
                      is_const=True)
    return module, Vector
