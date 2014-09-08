// This file is part of the dune-pymor project:
//   https://github.com/pymor/dune-pymor
// Copyright holders: Stephan Rave, Felix Schindler
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_FUNCTIONS_HH
#define DUNE_PYMOR_FUNCTIONS_HH

#include <string>
#include <vector>

#include <dune/stuff/common/configuration.hh>
#include <dune/stuff/common/memory.hh>
#include <dune/stuff/functions.hh>

#include "functions/interfaces.hh"
#include "functions/default.hh"
#include "functions/checkerboard.hh"

namespace Dune {
namespace Pymor {


template< class E, class D, int d, class R, int r, int rC = 1 >
class AffinelyDecomposableFunctions
{
public:
  static std::vector< std::string > available()
  {
    auto availableFunctions = Stuff::FunctionsProvider< E, D, d, R, r, rC >::available();
    for (const auto& id : { Function::AffinelyDecomposableDefault< E, D, d, R, r, rC >::static_id()
                          , Function::Checkerboard< E, D, d, R, r, rC >::static_id()})
      availableFunctions.push_back(id);
    return availableFunctions;
  } // ... available(...)

  static Stuff::Common::Configuration default_config(const std::string type = available()[0])
  {
    if (type == Function::AffinelyDecomposableDefault< E, D, d, R, r, rC >::static_id())
      return Function::AffinelyDecomposableDefault< E, D, d, R, r, rC >::default_config();
    else if (type == Function::Checkerboard< E, D, d, R, r, rC >::static_id())
      return Function::Checkerboard< E, D, d, R, r, rC >::default_config();
    else
      return Stuff::FunctionsProvider< E, D, d, R, r, rC >::default_config(type);
  } // ... default_config(...)

      static std::unique_ptr< AffinelyDecomposableFunctionInterface< E, D, d, R, r, rC > >
  create(const std::string type = available()[0], const Stuff::Common::Configuration config = default_config())
  {
    if (type == Function::AffinelyDecomposableDefault< E, D, d, R, r, rC >::static_id())
      return Function::AffinelyDecomposableDefault< E, D, d, R, r, rC >::create(config);
    else if (type == Function::Checkerboard< E, D, d, R, r, rC >::static_id())
      return Function::Checkerboard< E, D, d, R, r, rC >::create(config);
    else {
      typedef Function::NonparametricDefault< E, D, d, R, r, rC > NonparametricType;
      return Stuff::Common::make_unique< NonparametricType >(Stuff::FunctionsProvider< E, D, d, R, r, rC >::create(type, config));
    }
  } // ... create(...)
}; // class AffinelyDecomposableFunctions


} // namespace Pymor
} // namespace Dune

#endif // DUNE_PYMOR_FUNCTIONS_HH
