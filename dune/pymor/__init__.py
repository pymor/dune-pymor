#! /usr/bin/env python
# This file is part of the dune-pymor project:
#   https://github.com/pymor/dune-pymor
# Copyright Holders: Stephan Rave, Felix Schindler
# License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

from __future__ import print_function

import os
from os.path import abspath, join
from collections import defaultdict


def config_h(config_h_filename = None):
    if config_h_filename is None:
        this_path = abspath(__file__)
        if '__init__.pyc' in this_path:
            this_path = this_path.rstrip('__init__.pyc')
        elif '__init__.py' in this_path:
            this_path = this_path.rstrip('__init__.py')
        config_h_filename = join(this_path, '..', '..', 'config.h')
    CONFIG_H = defaultdict(bool)
    try:
        with open(config_h_filename) as config_h_file:
            for line in config_h_file:
                if len(line.strip().split(' ')) > 0:
                    if line.strip().split(' ')[0] == '#define':
                        rest_of_line = line.strip().lstrip('#define ')
                        words = rest_of_line.split(' ')
                        if len(words) > 1:
                            key = words[0]
                            value = ' '.join(words[1:])
                            if value == '/**/':
                                value = 1
                            if not ('(' in key or ')' in key):
                                CONFIG_H[key] = value
        return CONFIG_H
    except:
        print('Did you configure dune-pymor? Could not find this config.h:\n  \'{}\''.format(config_h_filename))
        return None
