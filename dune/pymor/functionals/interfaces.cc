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


// ===============================
// ===== FunctionalInterface =====
// ===============================
FunctionalInterface::FunctionalInterface()
  : Parametric()
{}

FunctionalInterface::FunctionalInterface(const Parametric& other)
  : Parametric(other)
{}

FunctionalInterface::FunctionalInterface(const ParameterType& tt)
  : Parametric(tt)
{}

FunctionalInterface::FunctionalInterface(const std::string& kk, const int& vv) throw (Exception::key_is_not_valid,
                                                                                      Exception::index_out_of_range)
  : Parametric(kk, vv)
{}

FunctionalInterface::FunctionalInterface(const std::vector< std::string >& kk,
                                         const std::vector< int >& vv) throw (Exception::key_is_not_valid,
                                                                              Exception::index_out_of_range,
                                                                              Exception::sizes_do_not_match)
  : Parametric(kk, vv)
{}

FunctionalInterface::~FunctionalInterface()
{}

FunctionalInterface* FunctionalInterface::freeze_parameter(const Parameter /*mu*/) const
  throw (Exception::this_is_not_parametric,
         Exception::you_have_to_implement_this,
         Exception::this_does_not_make_any_sense)
{
  DUNE_PYMOR_THROW(Exception::you_have_to_implement_this, "you really do!");
}


// =================================================
// ===== AffinelyDecomposedFunctionalInterface =====
// =================================================
AffinelyDecomposedFunctionalInterface::AffinelyDecomposedFunctionalInterface()
  : FunctionalInterface()
{}

AffinelyDecomposedFunctionalInterface::AffinelyDecomposedFunctionalInterface(const Parametric& other)
  : FunctionalInterface(other)
{}

AffinelyDecomposedFunctionalInterface::AffinelyDecomposedFunctionalInterface(const ParameterType& tt)
  : FunctionalInterface(tt)
{}

AffinelyDecomposedFunctionalInterface::AffinelyDecomposedFunctionalInterface(const std::string& kk, const int& vv)
  throw (Exception::key_is_not_valid, Exception::index_out_of_range)
  : FunctionalInterface(kk, vv)
{}

AffinelyDecomposedFunctionalInterface::AffinelyDecomposedFunctionalInterface(const std::vector< std::string >& kk,
                                                                             const std::vector< int >& vv)
  throw (Exception::key_is_not_valid, Exception::index_out_of_range, Exception::sizes_do_not_match)
  : FunctionalInterface(kk, vv)
{}

AffinelyDecomposedFunctionalInterface::~AffinelyDecomposedFunctionalInterface()
{}


} // namespace Pymor
} // namespace Dune
