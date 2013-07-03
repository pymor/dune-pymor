#! /usr/bin/env python
# This file is part of the dune-pymor project:
#   https://github.com/pyMor/dune-pymor
# Copyright Holders: Felix Albrecht, Stephan Rave
# License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

import pybindgen

from dune.pymor.common import Exceptions


def add_VectorInterface(module, exceptions, name='Dune::Pymor::LA::VectorInterface'):
    assert(isinstance(module, pybindgen.module.Module))
    assert(isinstance(exceptions, Exceptions))
    assert(name is not None)
    name = str(name)
    assert(len(name) > 0)
    namespace = module
    namespaces = [nspace.strip() for nspace in name.split('::')[:-1]]
    name = name.split('::')[-1].strip()
    full_name = name
    if len(namespaces) > 0:
        full_name = '::'.join(namespaces) + '::' + name
    for nspace in namespaces:
        namespace = namespace.add_cpp_namespace(nspace)
    Vector = namespace.add_class(name)
    Vector.add_constructor([pybindgen.param('int', 'ss')])
    Vector.add_method('type',
                      pybindgen.retval('std::string'),
                      [],
                      is_const=True)
    Vector.add_method('dim',
                      pybindgen.retval('int'),
                      [],
                      is_const=True)
    Vector.add_method('almost_equal',
                      pybindgen.retval('bool'),
                      [pybindgen.param(full_name + ' *', 'other', transfer_ownership=False),
                       pybindgen.param('double', 'epsilon')],
                      is_const=True,
                      throw=[exceptions.types_are_not_compatible,
                             exceptions.sizes_do_not_match])
    Vector.add_method('scal',
                      None,
                      [pybindgen.param('double', 'alpha')])
    Vector.add_method('axpy',
                      None,
                      [pybindgen.param('double', 'alpha'),
                       pybindgen.param(full_name + ' *', 'x', transfer_ownership=False)])
    Vector.add_method('dot',
                      pybindgen.retval('double'),
                      [pybindgen.param(full_name + ' *', 'other', transfer_ownership=False)],
                      is_const=True)
    Vector.add_method('l1_norm',
                      pybindgen.retval('double'),
                      [],
                      is_const=True)
    Vector.add_method('l2_norm',
                      pybindgen.retval('double'),
                      [],
                      is_const=True)
    Vector.add_method('sup_norm',
                      pybindgen.retval('double'),
                      [],
                      is_const=True)
    Vector.add_method('components',
                      pybindgen.retval('std::vector< double >'),
                      [pybindgen.param('std::vector< int >', 'component_indices')],
                      is_const=True)
    Vector.add_method('amax',
                      pybindgen.retval('std::vector< double >'),
                      [],
                      is_const=True)
    Vector.add_method('add',
                      pybindgen.retval(full_name + ' *', caller_owns_return=True),
                      [pybindgen.param(full_name + ' *', 'other', transfer_ownership=False)],
                      is_const=True,
                      throw=[exceptions.types_are_not_compatible,
                             exceptions.sizes_do_not_match])
    Vector.add_method('iadd',
                      None,
                      [pybindgen.param(full_name + ' *', 'other', transfer_ownership=False)])
    Vector.add_method('sub',
                      pybindgen.retval(full_name + ' *', caller_owns_return=True),
                      [pybindgen.param(full_name + ' *', 'other', transfer_ownership=False)],
                      is_const=True)
    Vector.add_method('isub',
                      None,
                      [pybindgen.param(full_name + ' *', 'other', transfer_ownership=False)])
    return module, Vector
