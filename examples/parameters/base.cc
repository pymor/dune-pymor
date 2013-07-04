// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#include "base.hh"

Dune::Pymor::ParameterType* createParameterType(const std::string& key, const int& value)
{
  return new Dune::Pymor::ParameterType(key, value);
}

Dune::Pymor::Parameter* createParameter(const std::string& key, const int& value)
{
  return new Dune::Pymor::Parameter(key, value);
}
