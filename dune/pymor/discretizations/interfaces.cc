// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifdef HAVE_CMAKE_CONFIG
  #include "cmake_config.h"
#elif defined (HAVE_CONFIG_H)
  #include "config.h"
#endif // HAVE_CMAKE_CONFIG

#include "interfaces.hh"

namespace Dune {
namespace Pymor {


StationaryDiscretizationInterface::StationaryDiscretizationInterface()
  : Parametric()
{}

StationaryDiscretizationInterface::StationaryDiscretizationInterface(const ParameterType& tt)
  : Parametric(tt)
{}

StationaryDiscretizationInterface::StationaryDiscretizationInterface(const std::string& kk,
                                                                     const int& vv)
  throw (Exception::key_is_not_valid, Exception::index_out_of_range)
  : Parametric(kk, vv)
{}

StationaryDiscretizationInterface::StationaryDiscretizationInterface(const std::vector< std::string >& kk,
                                                                     const std::vector< int >& vv)
  throw (Exception::key_is_not_valid, Exception::index_out_of_range, Exception::sizes_do_not_match)
  : Parametric(kk, vv)
{}

StationaryDiscretizationInterface::StationaryDiscretizationInterface(const Parametric& pp)
  : Parametric(pp)
{}

StationaryDiscretizationInterface::~StationaryDiscretizationInterface()
{}


} // namespace Pymor
} // namespace Dune
