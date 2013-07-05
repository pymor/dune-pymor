#! /usr/bin/env python

# This file is part of the dune-pymor project:
#   https://github.com/pyMor/dune-pymor
# Copyright Holders: Felix Albrecht, Stephan Rave
# License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

import pybindgen


class Exceptions(object):

    def __init__(self,
                 PymorException,
                 types_are_not_compatible,
                 sizes_do_not_match,
                 index_out_of_range,
                 key_is_not_valid,
                 you_have_to_implement_this,
                 wrong_parameter_type,
                 this_is_not_parametric):
        self.PymorException = PymorException
        self.types_are_not_compatible = types_are_not_compatible
        self.sizes_do_not_match = sizes_do_not_match
        self.index_out_of_range = index_out_of_range
        self.key_is_not_valid = key_is_not_valid
        self.you_have_to_implement_this = you_have_to_implement_this
        self.wrong_parameter_type = wrong_parameter_type
        self.this_is_not_parametric = this_is_not_parametric


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
    you_have_to_implement_this = namespace.add_exception('you_have_to_implement_this',
                                                         parent=PymorException,
                                                         message_rvalue='%(EXC)s.pymorWhat()')
    wrong_parameter_type = namespace.add_exception('wrong_parameter_type',
                                                   parent=PymorException,
                                                   message_rvalue='%(EXC)s.pymorWhat()')
    this_is_not_parametric = namespace.add_exception('this_is_not_parametric',
                                                     parent=PymorException,
                                                     message_rvalue='%(EXC)s.pymorWhat()')
    return module, Exceptions(PymorException,
                              types_are_not_compatible,
                              sizes_do_not_match,
                              index_out_of_range,
                              key_is_not_valid,
                              you_have_to_implement_this,
                              wrong_parameter_type,
                              this_is_not_parametric)
