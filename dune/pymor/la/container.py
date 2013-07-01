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
    Vector.add_method('type', pybindgen.retval('std::string'), [], is_const=True)
    Vector.add_method('dim', pybindgen.retval('int'), [], is_const=True)
    return module
