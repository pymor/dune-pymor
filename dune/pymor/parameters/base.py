#! /usr/bin/env python
# This file is part of the dune-pymor project:
#   https://github.com/pymor/dune-pymor
# Copyright Holders: Stephan Rave, Felix Schindler
# License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

import pybindgen
from pybindgen import retval, param


def inject_ParameterType(module, exceptions, CONFIG_H):
    assert(isinstance(module, pybindgen.module.Module))
    assert(isinstance(exceptions, dict))
    assert(isinstance(CONFIG_H, dict))
    namespace = module.add_cpp_namespace('Dune').add_cpp_namespace('Pymor')
    ParameterType = namespace.add_class('ParameterType')
    ParameterType.add_constructor([])
    ParameterType.add_constructor([param('std::string', 'kk'),
                                   param(CONFIG_H['DUNE_STUFF_SSIZE_T'], 'vv')],
                                  throw=[exceptions['Exception']])
    ParameterType.add_constructor([param('std::vector< std::string >', 'kk'),
                                   param('std::vector< ' + CONFIG_H['DUNE_STUFF_SSIZE_T'] + ' >', 'vv')],
                                  throw=[exceptions['Exception']])
    ParameterType.add_method('empty', retval('bool'), [], is_const=True)
    ParameterType.add_method('keys',
                             retval('std::vector< std::string >'),
                             [],
                             is_const=True)
    ParameterType.add_method('values',
                             retval('std::vector< ' + CONFIG_H['DUNE_STUFF_SSIZE_T'] + ' >'),
                             [],
                             is_const=True)
    ParameterType.add_method('hasKey',
                             retval('bool'),
                             [param('std::string', 'key')],
                             is_const=True)
    ParameterType.add_method('set',
                             None,
                             [param('std::string', 'key'),
                              param(CONFIG_H['DUNE_STUFF_SSIZE_T'], 'value')],
                             throw=[exceptions['Exception']])
    ParameterType.add_method('get',
                             retval(CONFIG_H['DUNE_STUFF_SSIZE_T']),
                             [param('std::string', 'key')],
                             is_const=True,
                             throw=[exceptions['Exception']])
    ParameterType.add_binary_comparison_operator('==')
    ParameterType.add_binary_comparison_operator('!=')
    ParameterType.add_method('size', retval(CONFIG_H['DUNE_STUFF_SSIZE_T']), [], is_const=True)
    ParameterType.add_method('report', retval('std::string'), [], is_const=True)
    ParameterType.allow_subclassing = True
    return module, ParameterType


def inject_Parameter(module, exceptions, CONFIG_H):
    assert(isinstance(module, pybindgen.module.Module))
    assert(isinstance(exceptions, dict))
    assert(isinstance(CONFIG_H, dict))
    namespace = module.add_cpp_namespace('Dune').add_cpp_namespace('Pymor')
    Parameter = namespace.add_class('Parameter')
    Parameter.add_constructor([])
    Parameter.add_constructor([param('std::string', 'kk'),
                               param('double', 'vv')])
    Parameter.add_constructor([param('Dune::Pymor::ParameterType', 'tt'),
                               param('double', 'vv')],
                              throw=[exceptions['Exception']])
    Parameter.add_constructor([param('std::string', 'kk'),
                               param('std::vector< double >', 'vv')],
                              throw=[exceptions['Exception']])
    Parameter.add_constructor([param('Dune::Pymor::ParameterType', 'tt'),
                               param('std::vector< double >', 'vv')],
                              throw=[exceptions['Exception']])
    Parameter.add_constructor([param('std::vector< std::string >', 'kk'),
                               param('std::vector< std::vector< double > >', 'vv')],
                              throw=[exceptions['Exception']])
    Parameter.add_constructor([param('Dune::Pymor::ParameterType', 'tt'),
                               param('std::vector< std::vector< double > >', 'vv')],
                              throw=[exceptions['Exception']])
    Parameter.add_method('type', retval('Dune::Pymor::ParameterType'), [], is_const=True)
    Parameter.add_method('empty', retval('bool'), [], is_const=True)
    Parameter.add_method('keys',
                         retval('std::vector< std::string >'),
                         [],
                         is_const=True)
    Parameter.add_method('values',
                         retval('std::vector< std::vector< double > >'),
                         [],
                         is_const=True)
    Parameter.add_method('hasKey',
                         retval('bool'),
                         [param('std::string', 'key')],
                         is_const=True)
    Parameter.add_method('set',
                         None,
                         [param('std::string', 'key'),
                          param('std::vector< double >', 'value')],
                         throw=[exceptions['Exception']])
    Parameter.add_method('get',
                         retval('std::vector< double >'),
                         [param('std::string', 'key')],
                         is_const=True,
                         throw=[exceptions['Exception']])
    Parameter.add_binary_comparison_operator('==')
    Parameter.add_binary_comparison_operator('!=')
    Parameter.add_method('size', retval(CONFIG_H['DUNE_STUFF_SSIZE_T']), [], is_const=True)
    Parameter.add_method('report', retval('std::string'), [], is_const=True)
    Parameter.allow_subclassing = True
    return module, Parameter


def inject_Parametric(module, exceptions, CONFIG_H):
    assert(isinstance(module, pybindgen.module.Module))
    assert(isinstance(exceptions, dict))
    assert(isinstance(CONFIG_H, dict))
    namespace = module.add_cpp_namespace('Dune').add_cpp_namespace('Pymor')
    Parametric = namespace.add_class('Parametric')

    ###################################################################

    # The following code leads to segfaults of the python interpreter.
    # Not excactly sure why, but probably not needed on the python side
    # anyway.

    # Parametric.add_constructor([])
    # Parametric.add_constructor([param('Dune::Pymor::ParameterType', 'tt')])
    # Parametric.add_constructor([param('std::string', 'kk'),
    #                             param(CONFIG_H['DUNE_STUFF_SSIZE_T'], 'vv')])
    # Parametric.add_constructor([param('std::vector< std::string >', 'kk'),
    #                             param('std::vector< ' + CONFIG_H['DUNE_STUFF_SSIZE_T'] + ' >', 'vv')])
    # Parametric.add_copy_constructor()

    ###################################################################

    Parametric.add_method('parameter_type',
                          retval('const Dune::Pymor::ParameterType&'),
                          [],
                          is_const=True)
    Parametric.add_method('parametric', retval('bool'), [], is_const=True)
    # Parametric.allow_subclassing = True
    return module, Parametric


#def create_ParameterType(base):
#
#    from pymor.parameters.base import ParameterType as PymorBase
#
#    class ParameterType(base, PymorBase):
#
#        def __init__(self, *args, **kwargs):
#            super(ParameterType, self).__init__(*args, **kwargs)
#
#        def __setitem__(self, key, value):
#            super(ParameterType, self).set(key, value)
#
#        def __getitem__(self, key):
#            return super(ParameterType, self).get(key)
#
#        def __eq__(self, other):
#            raise Exception('Not yet implemented!')
#
#        def __ne__(self, other):
#            raise Exception('Not yet implemented!')
#
#        def __nonzero__(self):
#            return not bool(super(ParameterType, self).size())
#
#        def __len__(self):
#            return super(ParameterType, self).size()
#
#        def __iter__(self):
#            return iter(super(ParameterType, self).keys())
#
#        def update(self, *args, **kwargs):
#            raise Exception('Not yet implemented!')
#
#        def copy(self):
#            raise Exception('Not yet implemented!')
#
#        def keys(self):
#            return iter(super(ParameterType, self).keys())
#
#        def values(self):
#            return iter(super(ParameterType, self).values())
#
#        def iteritems(self):
#            for kk, vv in super(ParameterType, self).keys(), super(ParameterType, self).values():
#                yield (kk, vv)
#
#        def __str__(self):
#            return super(ParameterType, self).report()
#
#        def __repr__(self):
#            return super(ParameterType, self).report()
#
#    return ParameterType
#
