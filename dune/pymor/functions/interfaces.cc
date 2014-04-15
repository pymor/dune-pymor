// This file is part of the dune-pymor project:
//   https://github.com/pymor/dune-pymor
// Copyright Holders: Stephan Rave, Felix Schindler
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#include "config.h"

#include "interfaces.hh"

#ifdef DUNE_PYMOR_FUNCTIONS_TO_LIB
// we use this macro so we can add ParametricFunctionInterface at some point
#define DUNE_PYMOR_FUNCTION_INTERFACES_CC_LIST_CLASSES(etype, ddim) \
  DUNE_PYMOR_FUNCTION_INTERFACES_CC_LIST_DIMRANGE(Dune::Pymor::AffinelyDecomposableFunctionInterface, etype, ddim)

#define DUNE_PYMOR_FUNCTION_INTERFACES_CC_LIST_DIMRANGE(cname, etype, ddim) \
  DUNE_PYMOR_FUNCTION_INTERFACES_CC_LIST_DIMRANGECOLS(cname, etype, ddim, 1) \
  DUNE_PYMOR_FUNCTION_INTERFACES_CC_LIST_DIMRANGECOLS(cname, etype, ddim, 2) \
  DUNE_PYMOR_FUNCTION_INTERFACES_CC_LIST_DIMRANGECOLS(cname, etype, ddim, 3)

#define DUNE_PYMOR_FUNCTION_INTERFACES_CC_LIST_DIMRANGECOLS(cname, etype, ddim, rdim) \
  DUNE_PYMOR_FUNCTION_INTERFACES_CC_LIST_DOMAINFIELDTYPES(cname, etype, ddim, rdim, 1) \
  DUNE_PYMOR_FUNCTION_INTERFACES_CC_LIST_DOMAINFIELDTYPES(cname, etype, ddim, rdim, 2) \
  DUNE_PYMOR_FUNCTION_INTERFACES_CC_LIST_DOMAINFIELDTYPES(cname, etype, ddim, rdim, 3)

#define DUNE_PYMOR_FUNCTION_INTERFACES_CC_LIST_DOMAINFIELDTYPES(cname, etype, ddim, rdim, rcdim) \
  DUNE_PYMOR_FUNCTION_INTERFACES_CC_LIST_RANGEFIELDTYPES(cname, etype, double, ddim, rdim, rcdim)

#define DUNE_PYMOR_FUNCTION_INTERFACES_CC_LIST_RANGEFIELDTYPES(cname, etype, dftype, ddim, rdim, rcdim) \
  DUNE_PYMOR_FUNCTION_INTERFACES_CC_LAST_EXPANSION(cname, etype, dftype, ddim, double, rdim, rcdim) \
  DUNE_PYMOR_FUNCTION_INTERFACES_CC_LAST_EXPANSION(cname, etype, dftype, ddim, long double, rdim, rcdim)

#define DUNE_PYMOR_FUNCTION_INTERFACES_CC_LAST_EXPANSION(cname, etype, dftype, ddim, rftype, rdim, rcdim) \
  template class cname< etype, dftype, ddim, rftype, rdim, rcdim >;

DUNE_PYMOR_FUNCTION_INTERFACES_CC_LIST_CLASSES(DunePymorFunctionsInterfacesStuffFakeGrid1dEntityType, 1)
DUNE_PYMOR_FUNCTION_INTERFACES_CC_LIST_CLASSES(DunePymorFunctionsInterfacesStuffFakeGrid2dEntityType, 2)
DUNE_PYMOR_FUNCTION_INTERFACES_CC_LIST_CLASSES(DunePymorFunctionsInterfacesStuffFakeGrid3dEntityType, 3)

#ifdef HAVE_DUNE_GRID

DUNE_PYMOR_FUNCTION_INTERFACES_CC_LIST_CLASSES(DunePymorFunctionsInterfacesSGrid1dEntityType, 1)
DUNE_PYMOR_FUNCTION_INTERFACES_CC_LIST_CLASSES(DunePymorFunctionsInterfacesSGrid2dEntityType, 2)
DUNE_PYMOR_FUNCTION_INTERFACES_CC_LIST_CLASSES(DunePymorFunctionsInterfacesSGrid3dEntityType, 3)

DUNE_PYMOR_FUNCTION_INTERFACES_CC_LIST_CLASSES(DunePymorFunctionsInterfacesYaspGrid1dEntityType, 1)
DUNE_PYMOR_FUNCTION_INTERFACES_CC_LIST_CLASSES(DunePymorFunctionsInterfacesYaspGrid2dEntityType, 2)
DUNE_PYMOR_FUNCTION_INTERFACES_CC_LIST_CLASSES(DunePymorFunctionsInterfacesYaspGrid3dEntityType, 3)

# if HAVE_ALUGRID_SERIAL_H || HAVE_ALUGRID_PARALLEL_H

DUNE_PYMOR_FUNCTION_INTERFACES_CC_LIST_CLASSES(DunePymorFunctionsInterfacesAluSimplexGrid2dEntityType, 2)
DUNE_PYMOR_FUNCTION_INTERFACES_CC_LIST_CLASSES(DunePymorFunctionsInterfacesAluSimplexGrid3dEntityType, 3)
DUNE_PYMOR_FUNCTION_INTERFACES_CC_LIST_CLASSES(DunePymorFunctionsInterfacesAluCubeGrid3dEntityType, 3)

# endif // HAVE_ALUGRID_SERIAL_H || HAVE_ALUGRID_PARALLEL_H
#endif // HAVE_DUNE_GRID

#undef DUNE_PYMOR_FUNCTION_INTERFACES_CC_LAST_EXPANSION
#undef DUNE_PYMOR_FUNCTION_INTERFACES_CC_LIST_RANGEFIELDTYPES
#undef DUNE_PYMOR_FUNCTION_INTERFACES_CC_LIST_DOMAINFIELDTYPES
#undef DUNE_PYMOR_FUNCTION_INTERFACES_CC_LIST_DIMRANGECOLS
#undef DUNE_PYMOR_FUNCTION_INTERFACES_CC_LIST_DIMRANGE
#undef DUNE_PYMOR_FUNCTION_INTERFACES_CC_LIST_CLASSES
#endif // DUNE_PYMOR_FUNCTIONS_TO_LIB
