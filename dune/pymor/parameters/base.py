#! /usr/bin/env python
# This file is part of the dune-pymor project:
#   https://github.com/pyMor/dune-pymor
# Copyright Holders: Felix Albrecht, Stephan Rave
# License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

import pybindgen
from pybindgen import retval, param

from dune.pymor.common import Exceptions


def add_ParameterType(module, exceptions):
    assert(isinstance(module, pybindgen.module.Module))
    assert(isinstance(exceptions, Exceptions))
    namespace = module.add_cpp_namespace('Dune').add_cpp_namespace('Pymor')
    ParameterType = namespace.add_class('ParameterType')
    ParameterType.add_constructor([])
    ParameterType.add_constructor([param('std::string', 'kk'),
                                   param('int', 'vv')],
                                  throw=[exceptions.key_is_not_valid,
                                         exceptions.index_out_of_range])
    ParameterType.add_constructor([param('std::vector< std::string >', 'kk'),
                                   param('std::vector< int >', 'vv')],
                                  throw=[exceptions.key_is_not_valid,
                                         exceptions.index_out_of_range,
                                         exceptions.sizes_do_not_match])
    ParameterType.add_method('keys',
                             retval('std::vector< std::string >'),
                             [],
                             is_const=True)
    ParameterType.add_method('values',
                             retval('std::vector< int >'),
                             [],
                             is_const=True)
    ParameterType.add_method('hasKey',
                             retval('bool'),
                             [param('std::string', 'key')],
                             is_const=True)
    ParameterType.add_method('set',
                             None,
                             [param('std::string', 'key'),
                              param('int', 'value')],
                             throw=[exceptions.key_is_not_valid,
                                    exceptions.index_out_of_range])
    ParameterType.add_method('get',
                             retval('int'),
                             [param('std::string', 'key')],
                             is_const=True,
                             throw=[exceptions.key_is_not_valid])
    ParameterType.add_binary_comparison_operator('==')
    ParameterType.add_binary_comparison_operator('!=')
    ParameterType.add_method('size',
                             retval('unsigned int'),
                             [],
                             is_const=True)
    ParameterType.add_method('report',
                             retval('std::string'),
                             [],
                             is_const=True)
    ParameterType.allow_subclassing = True
    return module, ParameterType


def create_ParameterType(base):

    from pymor.parameters.base import ParameterType as PymorBase

    class ParameterType(base, PymorBase):

        def __init__(self, *args, **kwargs):
            super(ParameterType, self).__init__(*args, **kwargs)

        def __setitem__(self, key, value):
            super(ParameterType, self).set(key, value)

        def __getitem__(self, key):
            return super(ParameterType, self).get(key)

        def __eq__(self, other):
            raise Exception('Not yet implemented!')

        def __ne__(self, other):
            raise Exception('Not yet implemented!')

        def __nonzero__(self):
            return not bool(super(ParameterType, self).size())

        def __len__(self):
            return super(ParameterType, self).size()

        def __iter__(self):
            return iter(super(ParameterType, self).keys())

        def update(self, *args, **kwargs):
            raise Exception('Not yet implemented!')

        def copy(self):
            raise Exception('Not yet implemented!')

        def keys(self):
            return iter(super(ParameterType, self).keys())

        def values(self):
            return iter(super(ParameterType, self).values())

        def iteritems(self):
            for kk, vv in super(ParameterType, self).keys(), super(ParameterType, self).values():
                yield (kk, vv)

        def __str__(self):
            return super(ParameterType, self).report()

        def __repr__(self):
            return super(ParameterType, self).report()

    return ParameterType


