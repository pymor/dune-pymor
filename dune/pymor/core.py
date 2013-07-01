#! /usr/bin/env python

import pybindgen


def create_module(module_name, include_header):
    module = pybindgen.Module(module_name)
    module.add_include('"' + include_header + '"')
    return module
