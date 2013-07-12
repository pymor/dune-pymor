// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_LA_CONTAINER_HH
#define DUNE_PYMOR_LA_CONTAINER_HH

#include <dune/pymor/common/exceptions.hh>
#include "container/interfaces.hh"
#include "container/dunedynamicvector.hh"
#include "container/eigen.hh"

namespace Dune {
namespace Pymor {
namespace LA {

std::vector< std::string > availableVectors();

VectorInterface* createVector(const std::string type,
                              const size_t size,
                              const double value = 0.0) throw (Exception::types_are_not_compatible);

} // namespace LA
} // namespace Pymor
} // namespace Dune

#endif // DUNE_PYMOR_LA_CONTAINER_HH
