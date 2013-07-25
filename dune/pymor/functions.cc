// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifdef HAVE_CMAKE_CONFIG
  #include "cmake_config.h"
#elif defined (HAVE_CONFIG_H)
  #include "config.h"
#endif // HAVE_CMAKE_CONFIG

#include "functions/default.hh"
#include "functions.hh"

namespace Dune {
namespace Pymor {


template< class D, int d, class R, int rR, int rC >
std::vector< std::string > ParametricFunctions< D, d, R, rR, rC >::available()
{
  auto availableFunctions = Stuff::Functions< D, d, R, rR, rC >::available();
  availableFunctions.push_back(Function::AffinelyDecomposableDefault< D, d, R, rR, rC >::static_id());
  return availableFunctions;
}

template< class D, int d, class R, int rR, int rC >
Dune::ParameterTree ParametricFunctions< D, d, R, rR, rC >::defaultSettings(const std::string type)
{
  auto dune_stuff_functions = Stuff::Functions< D, d, R, rR, rC >::available();
  for (auto dune_stuff_function : dune_stuff_functions)
    if (type == dune_stuff_function)
      return Stuff::Functions< D, d, R, rR, rC >::defaultSettings(type);
  if (type == Function::AffinelyDecomposableDefault< D, d, R, rR, rC >::static_id())
    return Function::AffinelyDecomposableDefault< D, d, R, rR, rC >::defaultSettings();
  else
    DUNE_PYMOR_THROW(Exception::wrong_input,
                     "unknown function '" << type << "' requested!");
}

template< class D, int d, class R, int rR, int rC >
ParametricFunctionInterface< D, d, R, rR, rC >*
ParametricFunctions< D, d, R, rR, rC >::create(const std::string type, const Dune::ParameterTree settings)
{
  typedef Function::NonparametricDefault< D, d, R, rR, rC > NonparametricType;
  auto dune_stuff_functions = Stuff::Functions< D, d, R, rR, rC >::available();
  for (auto dune_stuff_function : dune_stuff_functions)
    if (type == dune_stuff_function)
      return new NonparametricType(Stuff::Functions< D, d, R, rR, rC >::create(type, settings));
  if (type == Function::AffinelyDecomposableDefault< D, d, R, rR, rC >::static_id())
    return Function::AffinelyDecomposableDefault< D, d, R, rR, rC >::create(settings);
  else
    DUNE_PYMOR_THROW(Exception::wrong_input,
                     "unknown function '" << type << "' requested!");
}

template class ParametricFunctions< double, 1, double, 2, 1 >;
template class ParametricFunctions< double, 1, double, 3, 1 >;
template class ParametricFunctions< double, 2, double, 2, 1 >;
template class ParametricFunctions< double, 2, double, 3, 1 >;
template class ParametricFunctions< double, 3, double, 2, 1 >;
template class ParametricFunctions< double, 3, double, 3, 1 >;


template< class D, int d, class R >
std::vector< std::string > ParametricFunctions< D, d, R, 1, 1 >::available()
{
  auto availableFunctions = Stuff::Functions< D, d, R, 1, 1 >::available();
  availableFunctions.push_back(Function::Checkerboard< D, d, R >::static_id());
  availableFunctions.push_back(Function::AffinelyDecomposableDefault< D, d, R, 1, 1 >::static_id());
  return availableFunctions;
}

template< class D, int d, class R >
Dune::ParameterTree ParametricFunctions< D, d, R, 1, 1 >::defaultSettings(const std::string type)
{
  auto dune_stuff_functions = Stuff::Functions< D, d, R, 1, 1 >::available();
  for (auto dune_stuff_function : dune_stuff_functions)
    if (type == dune_stuff_function)
      return Stuff::Functions< D, d, R, 1, 1 >::defaultSettings(type);
  if (type == Function::Checkerboard< D, d, R >::static_id())
    return Function::Checkerboard< D, d, R >::defaultSettings();
  else if (type == Function::AffinelyDecomposableDefault< D, d, R, 1, 1 >::static_id())
    return Function::AffinelyDecomposableDefault< D, d, R, 1, 1 >::defaultSettings();
  else
    DUNE_PYMOR_THROW(Exception::wrong_input,
                     "unknown function '" << type << "' requested!");
}

template< class D, int d, class R >
ParametricFunctionInterface< D, d, R, 1, 1 >*
ParametricFunctions< D, d, R, 1, 1 >::create(const std::string type, const Dune::ParameterTree settings)
{
  typedef Function::NonparametricDefault< D, d, R, 1, 1 > NonparametricType;
  auto dune_stuff_functions = Stuff::Functions< D, d, R, 1, 1 >::available();
  for (auto dune_stuff_function : dune_stuff_functions)
    if (type == dune_stuff_function)
      return new NonparametricType(Stuff::Functions< D, d, R, 1, 1 >::create(type, settings));
  if (type == Function::Checkerboard< D, d, R >::static_id())
    return Function::Checkerboard< D, d, R >::create(settings);
  else if (type == Function::AffinelyDecomposableDefault< D, d, R, 1, 1 >::static_id())
    return Function::AffinelyDecomposableDefault< D, d, R, 1, 1 >::create(settings);
  else
    DUNE_PYMOR_THROW(Exception::wrong_input,
                     "unknown function '" << type << "' requested!");
}

template class ParametricFunctions< double, 1, double, 1, 1 >;
template class ParametricFunctions< double, 2, double, 1, 1 >;
template class ParametricFunctions< double, 3, double, 1, 1 >;


} // namespace Pymor
} // namespace Dune
