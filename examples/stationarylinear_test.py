#! /usr/bin/env python
# This file is part of the dune-pymor project:
#   https://github.com/pymor/dune-pymor
# Copyright Holders: Stephan Rave, Felix Schindler
# License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

from __future__ import print_function

try:
    import stationarylinearexample as example_module

    print('creating analytical problem... ', end='')
    analytical_problem = example_module.Example.AnalyticalProblem(4)
    print('done')

    print('discretizing... ', end='')
    discretization = example_module.Example.SimpleDiscretization(analytical_problem)
    print('done')

    operator = discretization.get_operator()
    print('parameter type of operator is: {}'.format(operator.parameter_type().report()))

    functional = discretization.get_rhs()
    print('parameter type of rhs is: {}'.format(functional.parameter_type().report()))

    mu = example_module.Dune.Pymor.Parameter(
            ['diffusion', 'force', 'neumann', 'dirichlet'],
            [[1.0, 1.0, 1.0, 1.0], [1.0, 1.0, 1.0, 1.0], [1.0, 1.0, 1.0, 1.0], [1.0, 1.0, 1.0, 1.0]])

    print('solving for mu = {}... '.format(mu.report()), end='')
    solution = discretization.create_vector()
    discretization.solve(solution, mu)
    print('done')

    name = 'solution'
    filename = 'solution.txt'
    print('writing {} to \'{}\'... '.format(name, filename), end='')
    discretization.visualize(solution, filename, name)
    print('done')

except ImportError:
    print('There was an error importing \'stationarylinearexample\'!')
    print('Did you build the example successfully,')
    print('i.e. did \'make stationarylinearexample\' succeed?')
    raise
