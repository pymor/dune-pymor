#! /usr/bin/env python
# This file is part of the dune-pymor project:
#   https://github.com/pymor/dune-pymor
# Copyright Holders: Stephan Rave, Felix Schindler
# License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

import pybindgen


def inject_exceptions(module, CONFIG_H):
    assert(isinstance(module, pybindgen.module.Module))
    exceptions = dict()
    namespace = module.add_cpp_namespace('Dune')
    exception = namespace.add_exception('Exception', message_rvalue='%(EXC)s.what().c_str()')
    return module, [exception]
