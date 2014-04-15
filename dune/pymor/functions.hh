// This file is part of the dune-pymor project:
//   https://github.com/pymor/dune-pymor
// Copyright Holders: Stephan Rave, Felix Schindler
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_FUNCTIONS_HH
#define DUNE_PYMOR_FUNCTIONS_HH

#include <string>
#include <vector>

#include <dune/common/parametertree.hh>

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
    auto availableFunctions = Stuff::Functions< E, D, d, R, r, rC >::available();
    for (const auto& id : { Function::AffinelyDecomposableDefault< E, D, d, R, r, rC >::static_id()
                          , Function::Checkerboard< E, D, d, R, r, rC >::static_id()})
      availableFunctions.push_back(id);
    return availableFunctions;
  } // ... available(...)

  static Dune::ParameterTree defaultSettings(const std::string type = available()[0])
  {
    auto dune_stuff_functions = Stuff::Functions< E, D, d, R, r, rC >::available();
    for (auto dune_stuff_function : dune_stuff_functions)
      if (type.compare(dune_stuff_function) == 0)
        return Stuff::Functions< E, D, d, R, r, rC >::defaultSettings(type);
    if (type.compare(Function::AffinelyDecomposableDefault< E, D, d, R, r, rC >::static_id()) == 0)
      return Function::AffinelyDecomposableDefault< E, D, d, R, r, rC >::defaultSettings();
    else if (type.compare(Function::Checkerboard< E, D, d, R, r, rC >::static_id()) == 0)
      return Function::Checkerboard< E, D, d, R, r, rC >::defaultSettings();
    else
      DUNE_PYMOR_THROW(Exception::wrong_input, "unknown function '" << type << "' requested!");
  } // ... defaultSettings(...)

  static AffinelyDecomposableFunctionInterface< E, D, d, R, r, rC >*
  create(const std::string type = available()[0], const ParameterTree settings = defaultSettings())
  {
    typedef Function::NonparametricDefault< E, D, d, R, r, rC > NonparametricType;
    auto dune_stuff_functions = Stuff::Functions< E, D, d, R, r, rC >::available();
    for (auto dune_stuff_function : dune_stuff_functions)
      if (type.compare(dune_stuff_function))
        return new NonparametricType(Stuff::Functions< E, D, d, R, r, rC >::create(type, settings));
    if (type.compare(Function::AffinelyDecomposableDefault< E, D, d, R, r, rC >::static_id()))
      return Function::AffinelyDecomposableDefault< E, D, d, R, r, rC >::create(settings);
    else if (type.compare(Function::Checkerboard< E, D, d, R, r, rC >::static_id()))
      return Function::Checkerboard< E, D, d, R, r, rC >::create(settings);
    else
      DUNE_PYMOR_THROW(Exception::wrong_input, "unknown function '" << type << "' requested!");
  } // ... create(...)
}; // class AffinelyDecomposableFunctions


} // namespace Pymor
} // namespace Dune

#ifdef DUNE_PYMOR_FUNCTIONS_TO_LIB
# define DUNE_PYMOR_FUNCTIONS_LIST_DIMRANGE(etype, ddim) \
  DUNE_PYMOR_FUNCTIONS_LIST_DIMRANGECOLS(etype, ddim, 1) \
  DUNE_PYMOR_FUNCTIONS_LIST_DIMRANGECOLS(etype, ddim, 2) \
  DUNE_PYMOR_FUNCTIONS_LIST_DIMRANGECOLS(etype, ddim, 3)

# define DUNE_PYMOR_FUNCTIONS_LIST_DIMRANGECOLS(etype, ddim, rdim) \
  DUNE_PYMOR_FUNCTIONS_LIST_DOMAINFIELDTYPES(etype, ddim, rdim, 1) \
  DUNE_PYMOR_FUNCTIONS_LIST_DOMAINFIELDTYPES(etype, ddim, rdim, 2) \
  DUNE_PYMOR_FUNCTIONS_LIST_DOMAINFIELDTYPES(etype, ddim, rdim, 3)

# define DUNE_PYMOR_FUNCTIONS_LIST_DOMAINFIELDTYPES(etype, ddim, rdim, rcdim) \
  DUNE_PYMOR_FUNCTIONS_LIST_RANGEFIELDTYPES(etype, double, ddim, rdim, rcdim)

# define DUNE_PYMOR_FUNCTIONS_LIST_RANGEFIELDTYPES(etype, dftype, ddim, rdim, rcdim) \
  DUNE_PYMOR_FUNCTIONS_LAST_EXPANSION(etype, dftype, ddim, double, rdim, rcdim) \
  DUNE_PYMOR_FUNCTIONS_LAST_EXPANSION(etype, dftype, ddim, long double, rdim, rcdim)

# define DUNE_PYMOR_FUNCTIONS_LAST_EXPANSION(etype, dftype, ddim, rftype, rdim, rcdim) \
  extern template class Dune::Pymor::AffinelyDecomposableFunctions< etype, dftype, ddim, rftype, rdim, rcdim >;

# include <dune/stuff/grid/fakeentity.hh>

typedef Dune::Stuff::Grid::FakeEntity< 1 > DunePymorFunctionsInterfacesFake1dEntityType;
typedef Dune::Stuff::Grid::FakeEntity< 2 > DunePymorFunctionsInterfacesFake2dEntityType;
typedef Dune::Stuff::Grid::FakeEntity< 3 > DunePymorFunctionsInterfacesFake3dEntityType;

# ifdef HAVE_DUNE_GRID

#   include <dune/grid/sgrid.hh>

typedef Dune::SGrid< 1, 1 >::Codim< 0 >::Entity DunePymorFunctionsInterfacesSGrid1dEntityType;
typedef Dune::SGrid< 2, 2 >::Codim< 0 >::Entity DunePymorFunctionsInterfacesSGrid2dEntityType;
typedef Dune::SGrid< 3, 3 >::Codim< 0 >::Entity DunePymorFunctionsInterfacesSGrid3dEntityType;

DUNE_PYMOR_FUNCTIONS_LIST_DIMRANGE(DunePymorFunctionsInterfacesSGrid1dEntityType, 1)
DUNE_PYMOR_FUNCTIONS_LIST_DIMRANGE(DunePymorFunctionsInterfacesSGrid2dEntityType, 2)
DUNE_PYMOR_FUNCTIONS_LIST_DIMRANGE(DunePymorFunctionsInterfacesSGrid3dEntityType, 3)

#   include <dune/grid/yaspgrid.hh>

typedef Dune::YaspGrid< 1 >::Codim< 0 >::Entity DunePymorFunctionsInterfacesYaspGrid1dEntityType;
typedef Dune::YaspGrid< 2 >::Codim< 0 >::Entity DunePymorFunctionsInterfacesYaspGrid2dEntityType;
typedef Dune::YaspGrid< 3 >::Codim< 0 >::Entity DunePymorFunctionsInterfacesYaspGrid3dEntityType;

DUNE_PYMOR_FUNCTIONS_LIST_DIMRANGE(DunePymorFunctionsInterfacesYaspGrid1dEntityType, 1)
DUNE_PYMOR_FUNCTIONS_LIST_DIMRANGE(DunePymorFunctionsInterfacesYaspGrid2dEntityType, 2)
DUNE_PYMOR_FUNCTIONS_LIST_DIMRANGE(DunePymorFunctionsInterfacesYaspGrid3dEntityType, 3)

#   if HAVE_ALUGRID_SERIAL_H || HAVE_ALUGRID_PARALLEL_H
#     ifdef ALUGRID_CONFORM
#       define DUNE_PYMOR_FUNCTIONS_ALUGRID_CONFORM_WAS_DEFINED_BEFORE
#     else
#       define ALUGRID_CONFORM 1
#     endif
#     ifdef ENABLE_ALUGRID
#       define DUNE_PYMOR_FUNCTIONS_ENABLE_ALUGRID_WAS_DEFINED_BEFORE
#     else
#       define ENABLE_ALUGRID 1
#     endif

#     include <dune/grid/alugrid.hh>

typedef Dune::ALUSimplexGrid< 2, 2 >::Codim< 0 >::Entity DunePymorFunctionsInterfacesAluSimplexGrid2dEntityType;
typedef Dune::ALUSimplexGrid< 3, 3 >::Codim< 0 >::Entity DunePymorFunctionsInterfacesAluSimplexGrid3dEntityType;
typedef Dune::ALUCubeGrid< 3, 3 >::Codim< 0 >::Entity DunePymorFunctionsInterfacesAluCubeGrid3dEntityType;

DUNE_PYMOR_FUNCTIONS_LIST_DIMRANGE(DunePymorFunctionsInterfacesAluSimplexGrid2dEntityType, 2)
DUNE_PYMOR_FUNCTIONS_LIST_DIMRANGE(DunePymorFunctionsInterfacesAluSimplexGrid3dEntityType, 3)
DUNE_PYMOR_FUNCTIONS_LIST_DIMRANGE(DunePymorFunctionsInterfacesAluCubeGrid3dEntityType, 3)

#     ifdef DUNE_PYMOR_FUNCTIONS_ALUGRID_CONFORM_WAS_DEFINED_BEFORE
#       undef DUNE_PYMOR_FUNCTIONS_ALUGRID_CONFORM_WAS_DEFINED_BEFORE
#     else
#       undef ALUGRID_CONFORM
#     endif
#     ifdef DUNE_PYMOR_FUNCTIONS_ENABLE_ALUGRID_WAS_DEFINED_BEFORE
#       undef DUNE_PYMOR_FUNCTIONS_ENABLE_ALUGRID_WAS_DEFINED_BEFORE
#     else
#       undef ENABLE_ALUGRID
#     endif

#   endif // HAVE_ALUGRID_SERIAL_H || HAVE_ALUGRID_PARALLEL_H
# endif // HAVE_DUNE_GRID

# undef DUNE_PYMOR_FUNCTIONS_LAST_EXPANSION
# undef DUNE_PYMOR_FUNCTIONS_LIST_RANGEFIELDTYPES
# undef DUNE_PYMOR_FUNCTIONS_LIST_DOMAINFIELDTYPES
# undef DUNE_PYMOR_FUNCTIONS_LIST_DIMRANGE

#endif // DUNE_PYMOR_FUNCTIONS_TO_LIB
#endif // DUNE_PYMOR_FUNCTIONS_HH
