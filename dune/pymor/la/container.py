#! /usr/bin/env python

import pybindgen


def add_VectorInterface(module, name='Dune::Pymor::LA::VectorInterface'):
    assert(isinstance(module, pybindgen.module.Module))
    assert(name is not None)
    name = str(name)
    assert(len(name) > 0)
    namespace = module
    namespaces = [nspace.strip() for nspace in name.split('::')[:-1]]
    name = name.split('::')[-1].strip()
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
                      [pybindgen.param(name + ' *', 'other', transfer_ownership=False),
                       pybindgen.param('double', 'epsilon')],
                      is_const=True)
    Vector.add_method('scal',
                      None,
                      [pybindgen.param('double', 'alpha')])
    Vector.add_method('axpy',
                      None,
                      [pybindgen.param('double', 'alpha'),
                       pybindgen.param(name + ' *', 'x', transfer_ownership=False)])
    Vector.add_method('dot',
                      pybindgen.retval('double'),
                      [pybindgen.param(name + ' *', 'other', transfer_ownership=False)],
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
                      pybindgen.retval(name + ' *', caller_owns_return=True),
                      [pybindgen.param(name + ' *', 'other', transfer_ownership=False)],
                      is_const=True)
    Vector.add_method('iadd',
                      None,
                      [pybindgen.param(name + ' *', 'other', transfer_ownership=False)])
    Vector.add_method('sub',
                      pybindgen.retval(name + ' *', caller_owns_return=True),
                      [pybindgen.param(name + ' *', 'other', transfer_ownership=False)],
                      is_const=True)
    Vector.add_method('isub',
                      None,
                      [pybindgen.param(name + ' *', 'other', transfer_ownership=False)])
    return module, Vector
