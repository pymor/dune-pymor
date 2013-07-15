#! /usr/bin/env python
# This file is part of the dune-pymor project:
#   https://github.com/pyMor/dune-pymor
# Copyright Holders: Felix Albrecht, Stephan Rave
# License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

import pybindgen
from pybindgen import retval, param


def inject_OperatorInterface(module, exceptions, CONFIG_H, Parametric):
    assert(isinstance(module, pybindgen.module.Module))
    assert(isinstance(exceptions, dict))
    assert(isinstance(CONFIG_H, dict))
    assert(isinstance(Parametric, pybindgen.CppClass))
    namespace = module.add_cpp_namespace('Dune').add_cpp_namespace('Pymor')
    OperatorInterface = namespace.add_class('OperatorInterface', parent=Parametric)
    OperatorInterface.add_method('apply2',
                                 retval('double'),
                                 [param('const Dune::Pymor::LA::VectorInterface *', 'range', transfer_ownership=False),
                                  param('const Dune::Pymor::LA::VectorInterface *', 'source', transfer_ownership=False)],
                                 is_const=True,
                                 throw=[exceptions['types_are_not_compatible'],
                                        exceptions['you_have_to_implement_this'],
                                        exceptions['sizes_do_not_match'],
                                        exceptions['wrong_parameter_type'],
                                        exceptions['requirements_not_met'],
                                        exceptions['linear_solver_failed'],
                                        exceptions['this_does_not_make_any_sense']])
    OperatorInterface.add_method('apply2',
                                 retval('double'),
                                 [param('const Dune::Pymor::LA::VectorInterface *', 'range', transfer_ownership=False),
                                  param('const Dune::Pymor::LA::VectorInterface *', 'source', transfer_ownership=False),
                                  param('const Dune::Pymor::Parameter', 'mu')],
                                 is_const=True,
                                 throw=[exceptions['types_are_not_compatible'],
                                        exceptions['you_have_to_implement_this'],
                                        exceptions['sizes_do_not_match'],
                                        exceptions['wrong_parameter_type'],
                                        exceptions['requirements_not_met'],
                                        exceptions['linear_solver_failed'],
                                        exceptions['this_does_not_make_any_sense']])
    return module, OperatorInterface


def inject_LinearOperatorInterface(module, exceptions, CONFIG_H, OperatorInterface):
    assert(isinstance(module, pybindgen.module.Module))
    assert(isinstance(exceptions, dict))
    assert(isinstance(CONFIG_H, dict))
    assert(isinstance(OperatorInterface, pybindgen.CppClass))
    namespace = module.add_cpp_namespace('Dune').add_cpp_namespace('Pymor')
    LinearOperatorInterface = namespace.add_class('LinearOperatorInterface', parent=OperatorInterface)
    LinearOperatorInterface.add_method('linear', retval('bool'), [], is_const=True)
    return module, LinearOperatorInterface


def inject_derived_from_LinearOperatorInterface_with_Inverse(module,
                                                             exceptions,
                                                             CONFIG_H,
                                                             OperatorInterface,
                                                             LinearOperatorInterface,
                                                             LinearOperatorName,
                                                             LinearOperatorInverseName,
                                                             SourceType,
                                                             RangeType):
    assert(isinstance(module, pybindgen.module.Module))
    assert(isinstance(exceptions, dict))
    assert(isinstance(CONFIG_H, dict))
    assert(isinstance(OperatorInterface, pybindgen.CppClass))
    assert(isinstance(LinearOperatorInterface, pybindgen.CppClass))
    assert(isinstance(LinearOperatorName, str))
    assert(isinstance(LinearOperatorInverseName, str))
    assert(len(LinearOperatorInverseName.strip()) > 0)
    assert(len(LinearOperatorName) > 0)
    assert(isinstance(SourceType.strip(), str))
    assert(len(SourceType) > 0)
    assert(isinstance(RangeType, str))
    assert(len(RangeType.strip()) > 0)
    # parse names and create namespaces
    namespace_LinearOperator = module
    namespaces_LinearOperator = [nspace.strip() for nspace in LinearOperatorName.split('::')[:-1]]
    LinearOperatorName = LinearOperatorName.split('::')[-1].strip()
    full_LinearOperatorName = LinearOperatorName
    if len(namespaces_LinearOperator) > 0:
        full_LinearOperatorName = '::'.join(namespaces_LinearOperator) + '::' + LinearOperatorName
    for nspace in namespaces_LinearOperator:
        namespace_LinearOperator = namespace_LinearOperator.add_cpp_namespace(nspace)
    namespace_LinearOperatorInverse = module
    namespaces_LinearOperatorInverse = [nspace.strip() for nspace in LinearOperatorInverseName.split('::')[:-1]]
    LinearOperatorInverseName = LinearOperatorInverseName.split('::')[-1].strip()
    full_LinearOperatorInverseName = LinearOperatorInverseName
    if len(namespaces_LinearOperatorInverse) > 0:
        full_LinearOperatorInverseName = '::'.join(namespaces_LinearOperatorInverse) + '::' + LinearOperatorInverseName
    for nspace in namespaces_LinearOperatorInverse:
        namespace_LinearOperatorInverse = namespace_LinearOperatorInverse.add_cpp_namespace(nspace)
    LinearOperator = namespace_LinearOperator.add_class(LinearOperatorName, parent=LinearOperatorInterface)
    LinearOperatorInverse = namespace_LinearOperatorInverse.add_class(LinearOperatorInverseName, parent=OperatorInterface)
    # create the LinearOperator
    LinearOperator.add_constructor([])
    LinearOperator.add_copy_constructor()
    LinearOperator.add_method('dim_source', retval('unsigned int'), [], is_const=True)
    LinearOperator.add_method('dim_range', retval('unsigned int'), [], is_const=True)
    LinearOperator.add_method('type_source', retval('std::string'), [], is_const=True)
    LinearOperator.add_method('type_range', retval('std::string'), [], is_const=True)
    LinearOperator.add_method('apply',
                              None,
                              [param('const ' + SourceType + ' *', 'source', transfer_ownership=False),
                               param(RangeType + ' *', 'range', transfer_ownership=False)],
                              is_const=True,
                              throw=[exceptions['types_are_not_compatible'],
                                     exceptions['you_have_to_implement_this'],
                                     exceptions['sizes_do_not_match'],
                                     exceptions['wrong_parameter_type'],
                                     exceptions['requirements_not_met'],
                                     exceptions['linear_solver_failed'],
                                     exceptions['this_does_not_make_any_sense']])
    LinearOperator.add_method('apply',
                              None,
                              [param('const ' + SourceType + ' *', 'source', transfer_ownership=False),
                               param(RangeType + ' *', 'range', transfer_ownership=False),
                               param('Dune::Pymor::Parameter', 'mu')],
                              is_const=True,
                              throw=[exceptions['types_are_not_compatible'],
                                     exceptions['you_have_to_implement_this'],
                                     exceptions['sizes_do_not_match'],
                                     exceptions['wrong_parameter_type'],
                                     exceptions['requirements_not_met'],
                                     exceptions['linear_solver_failed'],
                                     exceptions['this_does_not_make_any_sense']])
    LinearOperator.add_method('apply2',
                              retval('double'),
                              [param('const ' + SourceType + ' *', 'source', transfer_ownership=False),
                               param('const ' + RangeType + ' *', 'range', transfer_ownership=False)],
                              is_const=True,
                              throw=[exceptions['types_are_not_compatible'],
                                     exceptions['you_have_to_implement_this'],
                                     exceptions['sizes_do_not_match'],
                                     exceptions['wrong_parameter_type'],
                                     exceptions['requirements_not_met'],
                                     exceptions['linear_solver_failed'],
                                     exceptions['this_does_not_make_any_sense']])
    LinearOperator.add_method('apply2',
                              retval('double'),
                              [param('const ' + SourceType + ' *', 'source', transfer_ownership=False),
                               param('const ' + RangeType + ' *', 'range', transfer_ownership=False),
                               param('Dune::Pymor::Parameter', 'mu')],
                              is_const=True,
                              throw=[exceptions['types_are_not_compatible'],
                                     exceptions['you_have_to_implement_this'],
                                     exceptions['sizes_do_not_match'],
                                     exceptions['wrong_parameter_type'],
                                     exceptions['requirements_not_met'],
                                     exceptions['linear_solver_failed'],
                                     exceptions['this_does_not_make_any_sense']])
    LinearOperator.add_method('invert_options',
                              retval('std::vector< std::string >'),
                              [],
                              is_const=True,
                              throw=[exceptions['not_invertible']])
    LinearOperator.add_method('invert',
                              retval('const ' + full_LinearOperatorInverseName + ' *', caller_owns_return=True),
                              [],
                              is_const=True,
                              throw=[exceptions['not_invertible'],
                                     exceptions['key_is_not_valid']])
    LinearOperator.add_method('invert',
                              retval('const ' + full_LinearOperatorInverseName + ' *', caller_owns_return=True),
                              [param('const std::string', 'type')],
                              is_const=True,
                              throw=[exceptions['not_invertible'],
                                     exceptions['key_is_not_valid']])
    LinearOperator.add_method('invert',
                              retval('const ' + full_LinearOperatorInverseName + ' *', caller_owns_return=True),
                              [param('const std::string', 'type'),
                               param('const Dune::Pymor::Parameter', 'mu')],
                              is_const=True,
                              throw=[exceptions['not_invertible'],
                                     exceptions['key_is_not_valid']])
    LinearOperator.add_method('apply_inverse',
                              None,
                              [param('const ' + RangeType + ' *', 'range', transfer_ownership=False),
                               param(SourceType + ' *', 'source', transfer_ownership=False)],
                              is_const=True,
                              throw=[exceptions['types_are_not_compatible'],
                                     exceptions['you_have_to_implement_this'],
                                     exceptions['sizes_do_not_match'],
                                     exceptions['wrong_parameter_type'],
                                     exceptions['requirements_not_met'],
                                     exceptions['linear_solver_failed'],
                                     exceptions['this_does_not_make_any_sense']])
    LinearOperator.add_method('apply_inverse',
                              None,
                              [param('const ' + RangeType + ' *', 'range', transfer_ownership=False),
                               param(SourceType + ' *', 'source', transfer_ownership=False),
                               param('const std::string', 'type')],
                              is_const=True,
                              throw=[exceptions['types_are_not_compatible'],
                                     exceptions['you_have_to_implement_this'],
                                     exceptions['sizes_do_not_match'],
                                     exceptions['wrong_parameter_type'],
                                     exceptions['requirements_not_met'],
                                     exceptions['linear_solver_failed'],
                                     exceptions['this_does_not_make_any_sense']])
    LinearOperator.add_method('apply_inverse',
                              None,
                              [param('const ' + RangeType + ' *', 'range', transfer_ownership=False),
                               param(SourceType + ' *', 'source', transfer_ownership=False),
                               param('const std::string', 'type'),
                               param('const Dune::Pymor::Parameter', 'mu')],
                              is_const=True,
                              throw=[exceptions['types_are_not_compatible'],
                                     exceptions['you_have_to_implement_this'],
                                     exceptions['sizes_do_not_match'],
                                     exceptions['wrong_parameter_type'],
                                     exceptions['requirements_not_met'],
                                     exceptions['linear_solver_failed'],
                                     exceptions['this_does_not_make_any_sense']])
    LinearOperator.add_method('freeze_parameter',
                              retval(full_LinearOperatorName + ' *', caller_owns_return=True),
                              [],
                              is_const=True,
                              throw=[exceptions['this_is_not_parametric'],
                                     exceptions['you_have_to_implement_this'],
                                     exceptions['this_does_not_make_any_sense']])
    LinearOperator.add_method('copy',
                              retval(full_LinearOperatorName + ' * ', caller_owns_return=True),
                              [],
                              is_const=True)
    LinearOperator.add_method('scal', None, [param('const double', 'alpha')])
    LinearOperator.add_method('axpy',
                              None,
                              [param('const double', 'alpha'),
                               param('const ' + full_LinearOperatorName + ' *', 'x', transfer_ownership=False)],
                              throw=[exceptions['sizes_do_not_match'],
                                     exceptions['types_are_not_compatible']])
    # create the LinearOperatorInverse
    LinearOperatorInverse.add_method('linear', retval('bool'), [], is_const=True)
    LinearOperatorInverse.add_method('dim_source', retval('unsigned int'), [], is_const=True)
    LinearOperatorInverse.add_method('dim_range', retval('unsigned int'), [], is_const=True)
    LinearOperatorInverse.add_method('type_source', retval('std::string'), [], is_const=True)
    LinearOperatorInverse.add_method('type_range', retval('std::string'), [], is_const=True)
    LinearOperatorInverse.add_method('apply',
                                     None,
                                     [param('const ' + RangeType + ' *', 'source', transfer_ownership=False),
                                      param(SourceType + ' *', 'range', transfer_ownership=False)],
                                     is_const=True,
                                     throw=[exceptions['types_are_not_compatible'],
                                            exceptions['you_have_to_implement_this'],
                                            exceptions['sizes_do_not_match'],
                                            exceptions['wrong_parameter_type'],
                                            exceptions['requirements_not_met'],
                                            exceptions['linear_solver_failed'],
                                            exceptions['this_does_not_make_any_sense']])
    LinearOperatorInverse.add_method('apply',
                                     None,
                                     [param('const ' + RangeType + ' *', 'source', transfer_ownership=False),
                                      param(SourceType + ' *', 'range', transfer_ownership=False),
                                      param('Dune::Pymor::Parameter', 'mu')],
                                     is_const=True,
                                     throw=[exceptions['types_are_not_compatible'],
                                            exceptions['you_have_to_implement_this'],
                                            exceptions['sizes_do_not_match'],
                                            exceptions['wrong_parameter_type'],
                                            exceptions['requirements_not_met'],
                                            exceptions['linear_solver_failed'],
                                            exceptions['this_does_not_make_any_sense']])
    LinearOperatorInverse.add_method('invert_options',
                                     retval('std::vector< std::string >'),
                                     [],
                                     is_const=True,
                                     throw=[exceptions['not_invertible']])
    LinearOperatorInverse.add_method('invert',
                                     retval('const Dune::Pymor::OperatorInterface *', caller_owns_return=True),
                                     [],
                                     is_const=True,
                                     throw=[exceptions['not_invertible'],
                                            exceptions['key_is_not_valid']])
    LinearOperatorInverse.add_method('invert',
                                     retval('const Dune::Pymor::OperatorInterface *', caller_owns_return=True),
                                     [param('const std::string', 'type')],
                                     is_const=True,
                                     throw=[exceptions['not_invertible'],
                                            exceptions['key_is_not_valid']])
    LinearOperatorInverse.add_method('invert',
                                     retval('const Dune::Pymor::OperatorInterface *', caller_owns_return=True),
                                     [param('const std::string', 'type'),
                                      param('const Dune::Pymor::Parameter', 'mu')],
                                     is_const=True,
                                     throw=[exceptions['not_invertible'],
                                            exceptions['key_is_not_valid']])
    LinearOperatorInverse.add_method('apply_inverse',
                                     None,
                                     [param('const ' + RangeType + ' *', 'range', transfer_ownership=False),
                                      param(SourceType + ' *', 'source', transfer_ownership=False)],
                                     is_const=True,
                                     throw=[exceptions['types_are_not_compatible'],
                                            exceptions['you_have_to_implement_this'],
                                            exceptions['sizes_do_not_match'],
                                            exceptions['wrong_parameter_type'],
                                            exceptions['requirements_not_met'],
                                            exceptions['linear_solver_failed'],
                                            exceptions['this_does_not_make_any_sense']])
    LinearOperatorInverse.add_method('apply_inverse',
                                     None,
                                     [param('const ' + RangeType + ' *', 'range', transfer_ownership=False),
                                      param(SourceType + ' *', 'source', transfer_ownership=False),
                                      param('const std::string', 'type')],
                                     is_const=True,
                                     throw=[exceptions['types_are_not_compatible'],
                                            exceptions['you_have_to_implement_this'],
                                            exceptions['sizes_do_not_match'],
                                            exceptions['wrong_parameter_type'],
                                            exceptions['requirements_not_met'],
                                            exceptions['linear_solver_failed'],
                                            exceptions['this_does_not_make_any_sense']])
    LinearOperatorInverse.add_method('apply_inverse',
                              None,
                              [param('const ' + RangeType + ' *', 'range', transfer_ownership=False),
                               param(SourceType + ' *', 'source', transfer_ownership=False),
                               param('const std::string', 'type'),
                               param('const Dune::Pymor::Parameter', 'mu')],
                              is_const=True,
                              throw=[exceptions['types_are_not_compatible'],
                                     exceptions['you_have_to_implement_this'],
                                     exceptions['sizes_do_not_match'],
                                     exceptions['wrong_parameter_type'],
                                     exceptions['requirements_not_met'],
                                     exceptions['linear_solver_failed'],
                                     exceptions['this_does_not_make_any_sense']])
    LinearOperatorInverse.add_method('freeze_parameter',
                                     retval(full_LinearOperatorInverseName + ' *', caller_owns_return=True),
                                     [],
                                     is_const=True,
                                     throw=[exceptions['this_is_not_parametric'],
                                            exceptions['you_have_to_implement_this'],
                                            exceptions['this_does_not_make_any_sense']])
    return module, LinearOperator, LinearOperatorInverse


def inject_AffinelyDecomposedOperatorInterface(module, exceptions, CONFIG_H, OperatorInterface):
    assert(isinstance(module, pybindgen.module.Module))
    assert(isinstance(exceptions, dict))
    assert(isinstance(CONFIG_H, dict))
    assert(isinstance(OperatorInterface, pybindgen.CppClass))
    namespace = module.add_cpp_namespace('Dune').add_cpp_namespace('Pymor')
    AffinelyDecomposedOperatorInterface = namespace.add_class('AffinelyDecomposedOperatorInterface',
                                                              parent=OperatorInterface)
    return module, AffinelyDecomposedOperatorInterface



def inject_specialization_of_LinearAffinelyDecomposedDefault(module,
                                                             exceptions,
                                                             CONFIG_H,
                                                             AffinelyDecomposedOperatorInterface,
                                                             LinearOperatorType,
                                                             LinearOperatorInverseType,
                                                             SourceType,
                                                             RangeType):
    assert(isinstance(module, pybindgen.module.Module))
    assert(isinstance(exceptions, dict))
    assert(isinstance(CONFIG_H, dict))
    assert(isinstance(AffinelyDecomposedOperatorInterface, pybindgen.CppClass))
    assert(isinstance(LinearOperatorType, str))
    assert(len(LinearOperatorType) > 0)
    assert(isinstance(SourceType.strip(), str))
    assert(len(SourceType) > 0)
    assert(isinstance(RangeType, str))
    assert(len(RangeType.strip()) > 0)
    namespace = module.add_cpp_namespace('Dune').add_cpp_namespace('Pymor').add_cpp_namespace('Operators')
    Class = namespace.add_class('LinearAffinelyDecomposedDefault',
                                parent=AffinelyDecomposedOperatorInterface,
                                template_parameters=[LinearOperatorType])
    Class.add_constructor([])
    Class.add_constructor([param(LinearOperatorType + ' *', 'aff', transfer_ownership=True)],
                                                    throw=[exceptions['requirements_not_met']])
    Class.add_method('register_component',
                     None,
                     [param(LinearOperatorType + ' *', 'aff', transfer_ownership=True)],
                     throw=[exceptions['this_does_not_make_any_sense'],
                            exceptions['sizes_do_not_match'],
                            exceptions['types_are_not_compatible']])
    Class.add_method('register_component',
                     None,
                     [param(LinearOperatorType + ' *', 'comp', transfer_ownership=True),
                      param('const Dune::Pymor::ParameterFunctional *',
                            'coeff',
                            transfer_ownership=True)],
                     throw=[exceptions['this_does_not_make_any_sense'],
                            exceptions['sizes_do_not_match'],
                            exceptions['types_are_not_compatible'],
                            exceptions['wrong_parameter_type']])
    Class.add_method('num_components', retval('unsigned int'), [], is_const=True)
    Class.add_method('component',
                     retval(LinearOperatorType + ' *', caller_owns_return=False),
                     [param('const int', 'ii')],
                     throw=[exceptions['requirements_not_met'],
                            exceptions['index_out_of_range']])
    Class.add_method('component',
                     retval('const ' + LinearOperatorType + ' *',
                            caller_owns_return=False),
                     [param('const int', 'ii')],
                     is_const=True,
                     throw=[exceptions['requirements_not_met'],
                            exceptions['index_out_of_range']])
    Class.add_method('coefficient',
                     retval('const Dune::Pymor::ParameterFunctional *',
                            caller_owns_return=False),
                     [param('const int', 'ii')],
                     is_const=True,
                     throw=[exceptions['requirements_not_met'],
                            exceptions['index_out_of_range']])
    Class.add_method('hasAffinePart', retval('bool'), [], is_const=True)
    Class.add_method('affinePart',
                     retval(LinearOperatorType + ' *', caller_owns_return=False),
                     [],
                     throw=[exceptions['requirements_not_met']])
    Class.add_method('affinePart',
                     retval('const ' + LinearOperatorType + ' *', caller_owns_return=False),
                     [],
                     is_const=True,
                     throw=[exceptions['requirements_not_met']])
    Class.add_method('linear', retval('bool'), [], is_const=True)
    Class.add_method('dim_source', retval('unsigned int'), [], is_const=True)
    Class.add_method('dim_range', retval('unsigned int'), [], is_const=True)
    Class.add_method('type_source', retval('std::string'), [], is_const=True)
    Class.add_method('type_range', retval('std::string'), [], is_const=True)
    Class.add_method('apply',
                     None,
                     [param('const ' + SourceType + ' *', 'source', transfer_ownership=False),
                      param(RangeType + ' *', 'range', transfer_ownership=False)],
                     is_const=True,
                     throw=[exceptions['types_are_not_compatible'],
                            exceptions['you_have_to_implement_this'],
                            exceptions['sizes_do_not_match'],
                            exceptions['wrong_parameter_type'],
                            exceptions['requirements_not_met'],
                            exceptions['linear_solver_failed'],
                            exceptions['this_does_not_make_any_sense']])
    Class.add_method('apply',
                     None,
                     [param('const ' + SourceType + ' *', 'source', transfer_ownership=False),
                      param(RangeType + ' *', 'range', transfer_ownership=False),
                      param('const Dune::Pymor::Parameter', 'mu')],
                     is_const=True,
                     throw=[exceptions['types_are_not_compatible'],
                            exceptions['you_have_to_implement_this'],
                            exceptions['sizes_do_not_match'],
                            exceptions['wrong_parameter_type'],
                            exceptions['requirements_not_met'],
                            exceptions['linear_solver_failed'],
                            exceptions['this_does_not_make_any_sense']])
    Class.add_method('apply2',
                     retval('double'),
                     [param('const ' + SourceType + ' *', 'source', transfer_ownership=False),
                      param('const ' + RangeType + ' *', 'range', transfer_ownership=False)],
                     is_const=True,
                     throw=[exceptions['types_are_not_compatible'],
                            exceptions['you_have_to_implement_this'],
                            exceptions['sizes_do_not_match'],
                            exceptions['wrong_parameter_type'],
                            exceptions['requirements_not_met'],
                            exceptions['linear_solver_failed'],
                            exceptions['this_does_not_make_any_sense']])
    Class.add_method('apply2',
                     retval('double'),
                     [param('const ' + SourceType + ' *', 'source', transfer_ownership=False),
                      param('const ' + RangeType + ' *', 'range', transfer_ownership=False),
                      param('const Dune::Pymor::Parameter', 'mu')],
                     is_const=True,
                     throw=[exceptions['types_are_not_compatible'],
                            exceptions['you_have_to_implement_this'],
                            exceptions['sizes_do_not_match'],
                            exceptions['wrong_parameter_type'],
                            exceptions['requirements_not_met'],
                            exceptions['linear_solver_failed'],
                            exceptions['this_does_not_make_any_sense']])
    Class.add_method('invert_options',
                     retval('std::vector< std::string >'),
                     [],
                     is_const=True,
                     throw=[exceptions['not_invertible']])
    Class.add_method('invert',
                     retval('const ' + LinearOperatorInverseType + ' *', caller_owns_return=True),
                     [],
                     is_const=True,
                     throw=[exceptions['not_invertible'],
                            exceptions['key_is_not_valid']])
    Class.add_method('invert',
                     retval('const ' + LinearOperatorInverseType + ' *', caller_owns_return=True),
                     [param('const std::string', 'type')],
                     is_const=True,
                     throw=[exceptions['not_invertible'],
                            exceptions['key_is_not_valid']])
    Class.add_method('invert',
                     retval('const ' + LinearOperatorInverseType + ' *', caller_owns_return=True),
                     [param('const std::string', 'type'),
                      param('const Dune::Pymor::Parameter', 'mu')],
                     is_const=True,
                     throw=[exceptions['not_invertible'],
                            exceptions['key_is_not_valid']])
    Class.add_method('apply_inverse',
                     None,
                     [param('const ' + RangeType + ' *', 'range', transfer_ownership=False),
                      param(SourceType + ' *', 'source', transfer_ownership=False)],
                     is_const=True,
                     throw=[exceptions['types_are_not_compatible'],
                            exceptions['you_have_to_implement_this'],
                            exceptions['sizes_do_not_match'],
                            exceptions['wrong_parameter_type'],
                            exceptions['requirements_not_met'],
                            exceptions['linear_solver_failed'],
                            exceptions['this_does_not_make_any_sense']])
    Class.add_method('apply_inverse',
                     None,
                     [param('const ' + RangeType + ' *', 'range', transfer_ownership=False),
                      param(SourceType + ' *', 'source', transfer_ownership=False),
                      param('const std::string', 'type')],
                     is_const=True,
                     throw=[exceptions['types_are_not_compatible'],
                            exceptions['you_have_to_implement_this'],
                            exceptions['sizes_do_not_match'],
                            exceptions['wrong_parameter_type'],
                            exceptions['requirements_not_met'],
                            exceptions['linear_solver_failed'],
                            exceptions['this_does_not_make_any_sense']])
    Class.add_method('apply_inverse',
                     None,
                     [param('const ' + RangeType + ' *', 'range', transfer_ownership=False),
                      param(SourceType + ' *', 'source', transfer_ownership=False),
                      param('const std::string', 'type'),
                      param('const Dune::Pymor::Parameter', 'mu')],
                     is_const=True,
                     throw=[exceptions['types_are_not_compatible'],
                            exceptions['you_have_to_implement_this'],
                            exceptions['sizes_do_not_match'],
                            exceptions['wrong_parameter_type'],
                            exceptions['requirements_not_met'],
                            exceptions['linear_solver_failed'],
                            exceptions['this_does_not_make_any_sense']])
    Class.add_method('freeze_parameter',
                     retval(LinearOperatorType + ' *', caller_owns_return=True),
                     [],
                     is_const=True,
                     throw=[exceptions['this_is_not_parametric'],
                            exceptions['you_have_to_implement_this'],
                            exceptions['this_does_not_make_any_sense']])
    Class.add_method('freeze_parameter',
                     retval(LinearOperatorType + ' *', caller_owns_return=True),
                     [param('const Dune::Pymor::Parameter', 'mu')],
                     is_const=True,
                     throw=[exceptions['this_is_not_parametric'],
                            exceptions['you_have_to_implement_this'],
                            exceptions['this_does_not_make_any_sense']])
    return module, Class
