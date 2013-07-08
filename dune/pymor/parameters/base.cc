// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#include "base.hh"

namespace Dune {
namespace Pymor {


std::ostream& operator<<(std::ostream& oo, const ParameterType& pp)
{
  oo << pp.report();
  return oo;
}


std::ostream& operator<<(std::ostream& oo, const Parameter& pp)
{
  oo << pp.report();
  return oo;
}


} // namespace Pymor
} // namespace Dune
