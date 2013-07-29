#! /usr/bin/env python
# This file is part of the dune-pymor project:
#   https://github.com/pyMor/dune-pymor
# Copyright Holders: Felix Albrecht, Stephan Rave
# License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

import sys
from pybindgen import param, retval

from dune.pymor.core import prepare_python_bindings, inject_lib_dune_pymor, finalize_python_bindings


if __name__ == '__main__':
    # prepare the module
    module, pybindgen_filename = prepare_python_bindings(sys.argv[1:])
    # add all of libdunepymor
    module, exceptions, interfaces = inject_lib_dune_pymor(module)
    # add example user code
    #namespace = module.add_cpp_namespace('Example')
    #AnalyticalProblem = namespace.add_class('AnalyticalProblem')
    #AnalyticalProblem.add_constructor([])
    #SimpleDiscretization = namespace.add_class('SimpleDiscretization',
                                               #parent=interfaces['Dune::Pymor::Parametric'])
    #SimpleDiscretization.add_constructor([param('const AnalyticalProblem *', 'prob')],
                                         #throw=[exceptions['this_does_not_make_any_sense'],
                                                #exceptions['sizes_do_not_match'],
                                                #exceptions['types_are_not_compatible'],
                                                #exceptions['wrong_parameter_type']])
    #SimpleDiscretization.add_method('available_operators',
                                    #retval('std::vector< std::string >'),
                                    #[],
                                    #is_const=True)
    #SimpleDiscretization.add_method('get_operator',
                                    #retval('const Dune::Pymor::Operators::LinearAffinelyDecomposedDefault< Dune::Pymor::Operators::DuneDynamicMatrix > *'),
                                    #[param('const std::string', 'id')],
                                    #is_const=True,
                                    #throw=[exceptions['key_is_not_valid']])
    #SimpleDiscretization.add_method('available_functionals',
                                    #retval('std::vector< std::string >'),
                                    #[],
                                    #is_const=True)
    #SimpleDiscretization.add_method('get_functional',
                                    #retval('const Dune::Pymor::Functionals::LinearAffinelyDecomposedDefault< Dune::Pymor::LA::DuneDynamicVector > *'),
                                    #[param('const std::string', 'id')],
                                    #is_const=True,
                                    #throw=[exceptions['key_is_not_valid']])
    #SimpleDiscretization.add_method('create_vector',
                                    #retval('Dune::Pymor::LA::DuneDynamicVector *', caller_owns_return=True),
                                    #[],
                                    #is_const=True)
    #SimpleDiscretization.add_method('solver_options',
                                    #retval('std::vector< std::string >'),
                                    #[],
                                    #is_const=True)
    #SimpleDiscretization.add_method('solver_options',
                                    #retval('std::string'),
                                    #[param('const std::string', 'context')],
                                    #is_const=True,
                                    #throw=[exceptions['key_is_not_valid']])
    #SimpleDiscretization.add_method('solve',
                                    #None,
                                    #[param('Dune::Pymor::LA::DuneDynamicVector *', 'vector', transfer_ownership=False)],
                                    #is_const=True,
                                    #throw=[exceptions['wrong_parameter_type'],
                                           #exceptions['types_are_not_compatible'],
                                           #exceptions['you_have_to_implement_this'],
                                           #exceptions['sizes_do_not_match'],
                                           #exceptions['wrong_parameter_type'],
                                           #exceptions['requirements_not_met'],
                                           #exceptions['linear_solver_failed'],
                                           #exceptions['this_does_not_make_any_sense']])
    #SimpleDiscretization.add_method('solve',
                                    #None,
                                    #[param('Dune::Pymor::LA::DuneDynamicVector *', 'vector', transfer_ownership=False),
                                     #param('const Dune::Pymor::Parameter', 'mu')],
                                    #is_const=True,
                                    #throw=[exceptions['wrong_parameter_type'],
                                           #exceptions['types_are_not_compatible'],
                                           #exceptions['you_have_to_implement_this'],
                                           #exceptions['sizes_do_not_match'],
                                           #exceptions['wrong_parameter_type'],
                                           #exceptions['requirements_not_met'],
                                           #exceptions['linear_solver_failed'],
                                           #exceptions['this_does_not_make_any_sense']])
    #SimpleDiscretization.add_method('visualize',
                                    #None,
                                    #[param('const Dune::Pymor::LA::DuneDynamicVector *', 'vector', transfer_ownership=False),
                                     #param('const std::string', 'filename')],
                                    #is_const=True,
                                    #throw=[exceptions['sizes_do_not_match']])
    #SimpleDiscretization.add_method('visualize',
                                    #None,
                                    #[param('const Dune::Pymor::LA::DuneDynamicVector *', 'vector', transfer_ownership=False),
                                     #param('const std::string', 'filename'),
                                     #param('const std::string', 'name')],
                                    #is_const=True,
                                    #throw=[exceptions['sizes_do_not_match']])
    # write the pybindgen .cc file
    finalize_python_bindings(module, pybindgen_filename)
