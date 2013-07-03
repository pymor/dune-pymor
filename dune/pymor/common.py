#! /usr/bin/env python

# This file is part of the dune-pymor project:
#   https://github.com/pyMor/dune-pymor
# Copyright Holders: Felix Albrecht, Stephan Rave
# License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

import pybindgen


class DunePymorExceptions():

    def __init__(self,
                 PymorException,
                 not_implemented_for_this_combination,
                 sizes_do_not_match,
                 index_out_of_range):
        self.PymorException = PymorException
        self.not_implemented_for_this_combination = not_implemented_for_this_combination
        self.sizes_do_not_match = sizes_do_not_match
        self.index_out_of_range = index_out_of_range


def add_PymorExceptions(module):
    assert(isinstance(module, pybindgen.module.Module))
    namespace = module.add_cpp_namespace('Dune')
    PymorException = namespace.add_exception('Dune::PymorException')
    namespace = namespace.add_cpp_namespace('Pymor').add_cpp_namespace('Exception')
    not_implemented_for_this_combination = namespace.add_exception('not_implemented_for_this_combination', parent=PymorException)
    sizes_do_not_match = namespace.add_exception('sizes_do_not_match', parent=PymorException, message_rvalue='%(EXC)s.pymorWhat()')
    index_out_of_range = namespace.add_exception('index_out_of_range', parent=PymorException)
    return module, DunePymorExceptions(PymorException,
                                       not_implemented_for_this_combination,
                                       sizes_do_not_match,
                                       index_out_of_range)
