#! /usr/bin/env python

# This file is part of the dune-pymor project:
#   https://github.com/pyMor/dune-pymor
# Copyright Holders: Felix Albrecht, Stephan Rave
# License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

import pybindgen


class Exceptions():

    def __init__(self,
                 PymorException,
                 types_are_not_compatible,
                 sizes_do_not_match,
                 index_out_of_range,
                 key_is_not_valid):
        self.PymorException = PymorException
        self.types_are_not_compatible = types_are_not_compatible
        self.sizes_do_not_match = sizes_do_not_match
        self.index_out_of_range = index_out_of_range
        self.key_is_not_valid = key_is_not_valid


def add_Exceptions(module):
    assert(isinstance(module, pybindgen.module.Module))
    namespace = module.add_cpp_namespace('Dune')
    PymorException = namespace.add_exception('Dune::PymorException')
    namespace = namespace.add_cpp_namespace('Pymor').add_cpp_namespace('Exception')
    types_are_not_compatible = namespace.add_exception('types_are_not_compatible',
                                                       parent=PymorException,
                                                       message_rvalue='%(EXC)s.pymorWhat()')
    sizes_do_not_match = namespace.add_exception('sizes_do_not_match',
                                                 parent=PymorException,
                                                 message_rvalue='%(EXC)s.pymorWhat()')
    index_out_of_range = namespace.add_exception('index_out_of_range',
                                                 parent=PymorException,
                                                 message_rvalue='%(EXC)s.pymorWhat()')
    key_is_not_valid = namespace.add_exception('key_is_not_valid',
                                               parent=PymorException,
                                               message_rvalue='%(EXC)s.pymorWhat()')
    return module, Exceptions(PymorException,
                              types_are_not_compatible,
                              sizes_do_not_match,
                              index_out_of_range,
                              key_is_not_valid)
