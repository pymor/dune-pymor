// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_LA_CONTAINER_HH
#define DUNE_PYMOR_LA_CONTAINER_HH

#include <dune/pymor/common/exceptions.hh>
#include "container/interfaces.hh"
#include "container/dunedynamic.hh"
#include "container/eigen.hh"

namespace Dune {
namespace Pymor {
namespace LA {


std::vector< std::string > availableVectors()
{
  return {
        DuneDynamicVector::static_type()
#if HAVE_EIGEN
      , EigenDenseVector::static_type()
#endif
  };
} // ... availableVectors(...)


VectorInterface* createVector(const std::string type,
                              const size_t size,
                              const value = 0.0) throw (Exception::types_are_not_compatible)
{
  if (type == DuneDynamicVector::static_type())
    return new DuneDynamicVector(size, value);
#if HAVE_EIGEN
  else if (type == EigenDenseVector::static_type())
    return new EigenDenseVector(size, value);
#endif
  else {
    std::stringstream msg;
    msg << "type (" << type << ") is not any of {";
    const auto types = availableVectors();
    for (size_t ii = 0; ii < (types.size() - 1); ++ii)
      msg << types[ii] << ", ";
    msg << types[types.size() - 1] << "}!";
    DUNE_PYMOR_THROW(Exception::types_are_not_compatible, msg.str());
  }
} // ... createVector(...)


} // namespace LA
} // namespace Pymor
} // namespace Dune

#endif // DUNE_PYMOR_LA_CONTAINER_HH
