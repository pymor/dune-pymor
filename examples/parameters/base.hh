// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_EXAMPLES_PARAMETERS_BASE_HH
#define DUNE_PYMOR_EXAMPLES_PARAMETERS_BASE_HH

#ifdef HAVE_CMAKE_CONFIG
  #include "cmake_config.h"
#elif defined (HAVE_CONFIG_H)
  #include "config.h"
#endif

#include <dune/pymor/parameters/base.hh>

Dune::Pymor::ParameterType* createParameterType(const std::string& key, const int& value);

Dune::Pymor::Parameter* createParameter(const std::string& key, const int& value);

#endif // DUNE_PYMOR_EXAMPLES_PARAMETERS_BASE_HH
