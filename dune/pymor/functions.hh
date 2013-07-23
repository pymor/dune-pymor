// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_FUNCTIONS_HH
#define DUNE_PYMOR_FUNCTIONS_HH

#include <string>
#include <vector>

#include <dune/common/parametertree.hh>

#include <dune/stuff/functions.hh>

#include "functions/interfaces.hh"
#include "functions/checkerboard.hh"

namespace Dune {
namespace Pymor {

template< class D, int d, class R, int rR, int rC >
class ParametricFunctions
{
public:
  static std::vector< std::string > available();

  static Dune::ParameterTree defaultSettings(const std::string type = available()[0]);

  static ParametricFunctionInterface< D, d, R, rR, rC >* create(const std::string type = available()[0],
                                                                const Dune::ParameterTree settings = defaultSettings());
}; // class ParametricFunctions


template< class D, int d, class R >
class ParametricFunctions< D, d, R, 1, 1 >
{
public:
  static std::vector< std::string > available();

  static Dune::ParameterTree defaultSettings(const std::string type = available()[0]);

  static ParametricFunctionInterface< D, d, R, 1, 1 >* create(const std::string type = available()[0],
                                                              const Dune::ParameterTree settings = defaultSettings());
}; // class ParametricFunctions

} // namespace Pymor
} // namespace Dune

#endif // DUNE_PYMOR_FUNCTIONS_HH
