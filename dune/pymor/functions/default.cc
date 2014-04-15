// This file is part of the dune-pymor project:
//   https://github.com/pymor/dune-pymor
// Copyright Holders: Stephan Rave, Felix Schindler
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#include "config.h"

#ifdef DUNE_PYMOR_FUNCTIONS_TO_LIB
#include "default.hh"

#define DUNE_PYMOR_FUNCTION_DEFAULT_CC_LIST_DIMRANGE(etype, ddim) \
  DUNE_PYMOR_FUNCTION_DEFAULT_CC_LIST_DIMRANGECOLS(etype, ddim, 1) \
  DUNE_PYMOR_FUNCTION_DEFAULT_CC_LIST_DIMRANGECOLS(etype, ddim, 2) \
  DUNE_PYMOR_FUNCTION_DEFAULT_CC_LIST_DIMRANGECOLS(etype, ddim, 3)

#define DUNE_PYMOR_FUNCTION_DEFAULT_CC_LIST_DIMRANGECOLS(etype, ddim, rdim) \
  DUNE_PYMOR_FUNCTION_DEFAULT_CC_LIST_DOMAINFIELDTYPES(etype, ddim, rdim, 1) \
  DUNE_PYMOR_FUNCTION_DEFAULT_CC_LIST_DOMAINFIELDTYPES(etype, ddim, rdim, 2) \
  DUNE_PYMOR_FUNCTION_DEFAULT_CC_LIST_DOMAINFIELDTYPES(etype, ddim, rdim, 3)

#define DUNE_PYMOR_FUNCTION_DEFAULT_CC_LIST_DOMAINFIELDTYPES(etype, ddim, rdim, rcdim) \
  DUNE_PYMOR_FUNCTION_DEFAULT_CC_LIST_RANGEFIELDTYPES(etype, double, ddim, rdim, rcdim)

#define DUNE_PYMOR_FUNCTION_DEFAULT_CC_LIST_RANGEFIELDTYPES(etype, dftype, ddim, rdim, rcdim) \
  DUNE_PYMOR_FUNCTION_DEFAULT_CC_LAST_EXPANSION(etype, dftype, ddim, double, rdim, rcdim) \
  DUNE_PYMOR_FUNCTION_DEFAULT_CC_LAST_EXPANSION(etype, dftype, ddim, long double, rdim, rcdim)

#define DUNE_PYMOR_FUNCTION_DEFAULT_CC_LAST_EXPANSION(etype, dftype, ddim, rftype, rdim, rcdim) \
  template class Dune::Pymor::Function::NonparametricDefault< etype, dftype, ddim, rftype, rdim, rcdim >; \
  template class Dune::Pymor::Function::AffinelyDecomposableDefault< etype, dftype, ddim, rftype, rdim, rcdim >;

#ifdef HAVE_DUNE_GRID

DUNE_PYMOR_FUNCTION_DEFAULT_CC_LIST_DIMRANGE(DunePymorFunctionsInterfacesSGrid1dEntityType, 1)
DUNE_PYMOR_FUNCTION_DEFAULT_CC_LIST_DIMRANGE(DunePymorFunctionsInterfacesSGrid2dEntityType, 2)
DUNE_PYMOR_FUNCTION_DEFAULT_CC_LIST_DIMRANGE(DunePymorFunctionsInterfacesSGrid3dEntityType, 3)

DUNE_PYMOR_FUNCTION_DEFAULT_CC_LIST_DIMRANGE(DunePymorFunctionsInterfacesYaspGrid1dEntityType, 1)
DUNE_PYMOR_FUNCTION_DEFAULT_CC_LIST_DIMRANGE(DunePymorFunctionsInterfacesYaspGrid2dEntityType, 2)
DUNE_PYMOR_FUNCTION_DEFAULT_CC_LIST_DIMRANGE(DunePymorFunctionsInterfacesYaspGrid3dEntityType, 3)

# if HAVE_ALUGRID_SERIAL_H || HAVE_ALUGRID_PARALLEL_H

DUNE_PYMOR_FUNCTION_DEFAULT_CC_LIST_DIMRANGE(DunePymorFunctionsInterfacesAluSimplexGrid2dEntityType, 2)
DUNE_PYMOR_FUNCTION_DEFAULT_CC_LIST_DIMRANGE(DunePymorFunctionsInterfacesAluSimplexGrid3dEntityType, 3)
DUNE_PYMOR_FUNCTION_DEFAULT_CC_LIST_DIMRANGE(DunePymorFunctionsInterfacesAluCubeGrid3dEntityType, 3)

# endif // HAVE_ALUGRID_SERIAL_H || HAVE_ALUGRID_PARALLEL_H
#endif // HAVE_DUNE_GRID

#undef DUNE_PYMOR_FUNCTION_DEFAULT_CC_LAST_EXPANSION
#undef DUNE_PYMOR_FUNCTION_DEFAULT_CC_LIST_RANGEFIELDTYPES
#undef DUNE_PYMOR_FUNCTION_DEFAULT_CC_LIST_DOMAINFIELDTYPES
#undef DUNE_PYMOR_FUNCTION_DEFAULT_CC_LIST_DIMRANGECOLS
#undef DUNE_PYMOR_FUNCTION_DEFAULT_CC_LIST_DIMRANGE
#undef DUNE_PYMOR_FUNCTION_DEFAULT_CC_LIST_CLASSES
#endif // DUNE_PYMOR_FUNCTIONS_TO_LIB
