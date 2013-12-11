// This file is part of the dune-pymor project:
//   https://github.com/pymor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_LA_CONTAINER_HH
#define DUNE_PYMOR_LA_CONTAINER_HH

#ifndef DUNE_STUFF_SSIZE_T
# define DUNE_STUFF_SSIZE_T long int
#endif

#include "container/interfaces.hh"
#include "container/dunedynamic.hh"
#include "container/eigen.hh"

namespace Dune {
namespace Pymor {
namespace LA {


template< class T >
static T createContainer(const T&, const DUNE_STUFF_SSIZE_T size);


} // namespace LA
} // namespace Pymor
} // namespace Dune

#endif // DUNE_PYMOR_LA_CONTAINER_HH
