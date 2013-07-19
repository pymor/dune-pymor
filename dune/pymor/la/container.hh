// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_LA_CONTAINER_HH
#define DUNE_PYMOR_LA_CONTAINER_HH

#include "container/interfaces.hh"
#include "container/dunedynamic.hh"

namespace Dune {
namespace Pymor {
namespace LA {


template< class T >
T createContainer(const T&, const size_t size);


} // namespace LA
} // namespace Pymor
} // namespace Dune

#endif // DUNE_PYMOR_LA_CONTAINER_HH
