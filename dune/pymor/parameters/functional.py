#! /usr/bin/env python
# This file is part of the dune-pymor project:
#   https://github.com/pymor/dune-pymor
# Copyright Holders: Stephan Rave, Felix Schindler
# License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

import pybindgen
from pybindgen import retval, param


def inject_ParameterFunctional(module, exceptions, interfaces, CONFIG_H):
    assert(isinstance(module, pybindgen.module.Module))
    assert(isinstance(exceptions, dict))
    assert(isinstance(interfaces, dict))
    for element in interfaces:
        assert(isinstance(element, str))
        assert(len(element) > 0)
        assert(isinstance(CONFIG_H, dict))
    namespace = module.add_cpp_namespace('Dune').add_cpp_namespace('Pymor')
    ParameterFunctional = namespace.add_class('ParameterFunctional', parent=[interfaces['Dune::Pymor::Parametric']])
    ParameterFunctional.add_copy_constructor()
    ParameterFunctional.add_constructor([param('const Dune::Pymor::ParameterType&', 'tt'),
                                         param('const std::string', 'exp')],
                                        throw=[exceptions['Exception']])
    ParameterFunctional.add_method('expression', retval('const std::string'), [], is_const=True)
    ParameterFunctional.add_method('report', retval('std::string'), [], is_const=True)
    ParameterFunctional.add_method('report',
                                   retval('std::string'),
                                   [param('const std::string', 'name')],
                                   is_const=True)
    ParameterFunctional.add_method('evaluate',
                                   'double',
                                   [param('const Parameter&', 'mu')],
                                   throw=[exceptions['Exception']],
                                   is_const=True)
    ParameterFunctional.allow_subclassing = True
    return module, ParameterFunctional
