#! /usr/bin/env python
# This file is part of the dune-pymor project:
#   https://github.com/pyMor/dune-pymor
# Copyright Holders: Felix Albrecht, Stephan Rave
# License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

import pybindgen
from pybindgen import retval, param


def inject_FunctionalInterface(module, exceptions, CONFIG_H, Parametric):
    assert(isinstance(module, pybindgen.module.Module))
    assert(isinstance(exceptions, dict))
    assert(isinstance(CONFIG_H, dict))
    assert(isinstance(Parametric, pybindgen.CppClass))
    namespace = module.add_cpp_namespace('Dune').add_cpp_namespace('Pymor')
    FunctionalInterface = namespace.add_class('FunctionalInterface', parent=Parametric)
    return module, FunctionalInterface


def inject_AffinelyDecomposedFunctionalInterface(module, exceptions, CONFIG_H, FunctionalInterface):
    assert(isinstance(module, pybindgen.module.Module))
    assert(isinstance(exceptions, dict))
    assert(isinstance(CONFIG_H, dict))
    assert(isinstance(FunctionalInterface, pybindgen.CppClass))
    namespace = module.add_cpp_namespace('Dune').add_cpp_namespace('Pymor')
    AffinelyDecomposedFunctionalInterface = namespace.add_class('AffinelyDecomposedFunctionalInterface',
                                                                parent=FunctionalInterface)
    return module, AffinelyDecomposedFunctionalInterface


def inject_specialization_of_LinearAffinelyDecomposedDefault(module,
                                                             exceptions,
                                                             CONFIG_H,
                                                             AffinelyDecomposedFunctionalInterface,
                                                             LinearFunctionalType,
                                                             SourceType):
    assert(isinstance(module, pybindgen.module.Module))
    assert(isinstance(exceptions, dict))
    assert(isinstance(CONFIG_H, dict))
    assert(isinstance(AffinelyDecomposedFunctionalInterface, pybindgen.CppClass))
    assert(isinstance(LinearFunctionalType, str))
    assert(len(LinearFunctionalType) > 0)
    assert(isinstance(SourceType, str))
    assert(len(SourceType) > 0)
    namespace = module.add_cpp_namespace('Dune').add_cpp_namespace('Pymor').add_cpp_namespace('Functionals')
    LinearAffinelyDecomposedDefault = namespace.add_class('LinearAffinelyDecomposedDefault',
                                                          parent=AffinelyDecomposedFunctionalInterface,
                                                          template_parameters=[LinearFunctionalType])
    LinearAffinelyDecomposedDefault.add_constructor([])
    LinearAffinelyDecomposedDefault.add_constructor([param(LinearFunctionalType + ' *', 'aff', transfer_ownership=True)],
                                                    throw=[exceptions['requirements_not_met']])
    LinearAffinelyDecomposedDefault.add_method('register_component',
                                               None,
                                               [param(LinearFunctionalType + ' *', 'aff', transfer_ownership=True)],
                                               throw=[exceptions['this_does_not_make_any_sense'],
                                                      exceptions['sizes_do_not_match'],
                                                      exceptions['types_are_not_compatible']])
    LinearAffinelyDecomposedDefault.add_method('register_component',
                                               None,
                                               [param(LinearFunctionalType + ' *', 'comp', transfer_ownership=True),
                                                param('const Dune::Pymor::ParameterFunctional *',
                                                      'coeff',
                                                      transfer_ownership=True)],
                                               throw=[exceptions['this_does_not_make_any_sense'],
                                                      exceptions['sizes_do_not_match'],
                                                      exceptions['types_are_not_compatible'],
                                                      exceptions['wrong_parameter_type']])
    LinearAffinelyDecomposedDefault.add_method('num_components', retval('unsigned int'), [], is_const=True)
    LinearAffinelyDecomposedDefault.add_method('component',
                                               retval(LinearFunctionalType + ' *', caller_owns_return=False),
                                               [param('const int', 'ii')],
                                               throw=[exceptions['requirements_not_met'],
                                                      exceptions['index_out_of_range']])
    LinearAffinelyDecomposedDefault.add_method('component',
                                               retval('const ' + LinearFunctionalType + ' *',
                                                      caller_owns_return=False),
                                               [param('const int', 'ii')],
                                               is_const=True,
                                               throw=[exceptions['requirements_not_met'],
                                                      exceptions['index_out_of_range']])
    LinearAffinelyDecomposedDefault.add_method('coefficient',
                                               retval('const Dune::Pymor::ParameterFunctional *',
                                                      caller_owns_return=False),
                                               [param('const int', 'ii')],
                                               is_const=True,
                                               throw=[exceptions['requirements_not_met'],
                                                      exceptions['index_out_of_range']])
    LinearAffinelyDecomposedDefault.add_method('hasAffinePart', retval('bool'), [], is_const=True)
    LinearAffinelyDecomposedDefault.add_method('affinePart',
                                               retval(LinearFunctionalType + ' *', caller_owns_return=False),
                                               [],
                                               throw=[exceptions['requirements_not_met']])
    LinearAffinelyDecomposedDefault.add_method('affinePart',
                                               retval('const ' + LinearFunctionalType + ' *', caller_owns_return=False),
                                               [],
                                               is_const=True,
                                               throw=[exceptions['requirements_not_met']])
    LinearAffinelyDecomposedDefault.add_method('linear', retval('bool'), [], is_const=True)
    LinearAffinelyDecomposedDefault.add_method('dim_source', retval('unsigned int'), [], is_const=True)
    LinearAffinelyDecomposedDefault.add_method('type_source', retval('std::string'), [], is_const=True)
    LinearAffinelyDecomposedDefault.add_method('apply',
                                               retval('double'),
                                               [param('const ' + SourceType + ' *', 'source', transfer_ownership=False)],
                                               is_const=True,
                                               throw=[exceptions['types_are_not_compatible'],
                                                      exceptions['you_have_to_implement_this'],
                                                      exceptions['sizes_do_not_match'],
                                                      exceptions['wrong_parameter_type'],
                                                      exceptions['requirements_not_met'],
                                                      exceptions['linear_solver_failed'],
                                                      exceptions['this_does_not_make_any_sense']])
    LinearAffinelyDecomposedDefault.add_method('apply',
                                               retval('double'),
                                               [param('const ' + SourceType + ' *', 'source', transfer_ownership=False),
                                                param('const Dune::Pymor::Parameter', 'mu')],
                                               is_const=True,
                                               throw=[exceptions['types_are_not_compatible'],
                                                      exceptions['you_have_to_implement_this'],
                                                      exceptions['sizes_do_not_match'],
                                                      exceptions['wrong_parameter_type'],
                                                      exceptions['requirements_not_met'],
                                                      exceptions['linear_solver_failed'],
                                                      exceptions['this_does_not_make_any_sense']])
    LinearAffinelyDecomposedDefault.add_method('freeze_parameter',
                                               retval(LinearFunctionalType + ' *', caller_owns_return=True),
                                               [],
                                               is_const=True,
                                               throw=[exceptions['this_is_not_parametric'],
                                                      exceptions['you_have_to_implement_this'],
                                                      exceptions['this_does_not_make_any_sense']])
    LinearAffinelyDecomposedDefault.add_method('freeze_parameter',
                                               retval(LinearFunctionalType + ' *', caller_owns_return=True),
                                               [param('const Dune::Pymor::Parameter', 'mu')],
                                               is_const=True,
                                               throw=[exceptions['this_is_not_parametric'],
                                                      exceptions['you_have_to_implement_this'],
                                                      exceptions['this_does_not_make_any_sense']])
    return module, LinearAffinelyDecomposedDefault
