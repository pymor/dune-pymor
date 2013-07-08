// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#include "base.hh"

namespace Dune {
namespace Pymor {


template< class KeyType, class ValueType >
std::ostream& operator<<(std::ostream& oo, const KeyValueBase< KeyType, ValueType >& pp)
{
  oo << pp.report();
  return oo;
}


} // namespace Pymor
} // namespace Dune
