#! /usr/bin/env python

import pybindgen


def create_module(module_name, include_header):
    module = pybindgen.Module(module_name)
    module.add_include('"' + include_header + '"')
    module.add_container('std::vector< double >', 'double', 'list')
    module.add_container('std::vector< int >', 'int', 'list')
    return module
