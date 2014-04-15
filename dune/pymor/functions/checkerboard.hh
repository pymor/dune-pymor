// This file is part of the dune-pymor project:
//   https://github.com/pymor/dune-pymor
// Copyright Holders: Stephan Rave, Felix Schindler
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_FUNCTIONS_CHECKERBOARD_HH
#define DUNE_PYMOR_FUNCTIONS_CHECKERBOARD_HH

#include <vector>

#include <dune/stuff/functions/checkerboard.hh>
#include <dune/stuff/common/parameter/tree.hh>

#include <dune/pymor/common/exceptions.hh>
#include <dune/pymor/parameters/functional.hh>
#include <dune/pymor/functions/default.hh>

#include "interfaces.hh"

namespace Dune {
namespace Pymor {
namespace Function {


template< class EntityImp, class DomainFieldImp, int domainDim, class RangeFieldImp, int rangeDim, int rangeDimCols = 1 >
class Checkerboard
  : public Function::AffinelyDecomposableDefault
        < EntityImp, DomainFieldImp, domainDim, RangeFieldImp, rangeDim, rangeDimCols >
{
  typedef Checkerboard< EntityImp, DomainFieldImp, domainDim, RangeFieldImp, rangeDim, rangeDimCols > ThisType;
  typedef Function::AffinelyDecomposableDefault
      < EntityImp, DomainFieldImp, domainDim, RangeFieldImp, rangeDim, rangeDimCols > BaseType;
public:
  typedef typename BaseType::NonparametricType  NonparametricType;
  typedef typename BaseType::EntityType         EntityType;

  typedef typename BaseType::DomainFieldType  DomainFieldType;
  static const unsigned int                   dimDomain = BaseType::dimDomain;
  typedef typename BaseType::DomainType       DomainType;

  typedef typename BaseType::RangeFieldType RangeFieldType;
  static const unsigned int                 dimRange = BaseType::dimRange;
  static const unsigned int                 dimRangeCols = BaseType::dimRangeCols;
  typedef typename BaseType::RangeType      RangeType;

  static std::string static_id()
  {
    return BaseType::BaseType::static_id() + ".checkerboard";
  }

  Checkerboard(const std::vector< DomainFieldType >& lowerLeft,
               const std::vector< DomainFieldType >& upperRight,
               const std::vector< size_t >& numElements,
               const std::string parameterName = "value",
               const std::string name = static_id())
    : BaseType(name)
  {
    // check input
    if (numElements.size() < dimDomain)
      DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                       "numElements has to be at least of size " << dimDomain << " is(" << numElements.size()
                       << ")!");
    size_t parameterSize = 1;
    for (size_t dd = 0; dd < dimDomain; ++dd) {
      if (lowerLeft[dd] >= upperRight[dd])
        DUNE_PYMOR_THROW(Exception::this_does_not_make_any_sense,
                         "lowerLeft[" << dd << "] = " << lowerLeft[dd] << " has to be smaller than upperRight["
                         << dd << "]!");
      if (numElements[dd] <= 0)
        DUNE_PYMOR_THROW(Exception::index_out_of_range,
                         "numElements[" << dd << "] has to be positive (is " << numElements[dd] << ")!");
      parameterSize *= numElements[dd];
    }
    if (parameterName.empty())
      DUNE_PYMOR_THROW(Exception::wrong_input, "parameterName must not be empty!");
    // build parameter
    const ParameterType parameterType(parameterName, parameterSize);
    // create the coefficients and components
    typedef Dune::Stuff::Function::Checkerboard
        < EntityType, DomainFieldType, dimDomain, RangeFieldType, dimRange, dimRangeCols >
      NonparametricCheckerboardType;
    for (size_t ii = 0; ii < parameterSize; ++ii) {
      std::vector< RangeType > indicator(parameterSize, RangeType(0));
      indicator[ii] = RangeType(1);
      BaseType::register_component(new NonparametricCheckerboardType(std::vector< DomainFieldType >(lowerLeft),
                                                                     std::vector< DomainFieldType >(upperRight),
                                                                     std::vector< size_t >(numElements),
                                                                     std::move(indicator)),
                                   new ParameterFunctional(parameterType,
                                                           parameterName + "[" + DSC::toString(ii) + "]"));
    } // create the coefficients and components
  } // Checkerboard()

  static Dune::ParameterTree defaultSettings(const std::string subName = "")
  {
    Dune::ParameterTree description;
    description["lowerLeft"] = "[0.0; 0.0; 0.0]";
    description["upperRight"] = "[1.0; 1.0; 1.0]";
    description["numElements"] = "[2; 2; 2]";
    description["parameterName"] = "value";
    description["name"] = static_id();
    if (subName.empty())
      return description;
    else {
      DSC::ExtendedParameterTree extendedDescription;
      extendedDescription.add(description, subName);
      return extendedDescription;
    }
  } // ... defaultSettings(...)

  static ThisType* create(const DSC::ExtendedParameterTree settings = defaultSettings())

  {
    // get data
    const std::string nm = settings.get< std::string >("name", static_id());
    const std::string paramName = settings.get< std::string >("parameterName", "value");
    std::vector< DomainFieldType > lowerLeft = settings.getVector("lowerLeft", DomainFieldType(0), dimDomain);
    std::vector< DomainFieldType > upperRight = settings.getVector("upperRight", DomainFieldType(1), dimDomain);
    std::vector< size_t > numElements = settings.getVector("numElements", size_t(1), dimDomain);
    // get paramSize
    size_t paramSize = 1;
    for (size_t dd = 0; dd < dimDomain; ++dd) {
      if (numElements[dd] <= 0)
        DUNE_PYMOR_THROW(Exception::this_does_not_make_any_sense,
                         "numElements[" << dd << "] has to be posititve (is " << numElements[dd] << ")!");
      paramSize *= numElements[dd];
    }
    // create and return
    return new ThisType(lowerLeft, upperRight, numElements, paramName, nm);
  } // ... create(...)
}; // class Checkerboard


} // namespace Function
} // namespace Pymor
} // namespace Dune

#ifdef DUNE_PYMOR_FUNCTIONS_TO_LIB
#define DUNE_PYMOR_FUNCTION_CHECKERBOARD_LIST_DIMRANGE(etype, ddim) \
  DUNE_PYMOR_FUNCTION_CHECKERBOARD_LIST_DIMRANGECOLS(etype, ddim, 1) \
  DUNE_PYMOR_FUNCTION_CHECKERBOARD_LIST_DIMRANGECOLS(etype, ddim, 2) \
  DUNE_PYMOR_FUNCTION_CHECKERBOARD_LIST_DIMRANGECOLS(etype, ddim, 3)

#define DUNE_PYMOR_FUNCTION_CHECKERBOARD_LIST_DIMRANGECOLS(etype, ddim, rdim) \
  DUNE_PYMOR_FUNCTION_CHECKERBOARD_LIST_DOMAINFIELDTYPES(etype, ddim, rdim, 1) \
  DUNE_PYMOR_FUNCTION_CHECKERBOARD_LIST_DOMAINFIELDTYPES(etype, ddim, rdim, 2) \
  DUNE_PYMOR_FUNCTION_CHECKERBOARD_LIST_DOMAINFIELDTYPES(etype, ddim, rdim, 3)

#define DUNE_PYMOR_FUNCTION_CHECKERBOARD_LIST_DOMAINFIELDTYPES(etype, ddim, rdim, rcdim) \
  DUNE_PYMOR_FUNCTION_CHECKERBOARD_LIST_RANGEFIELDTYPES(etype, double, ddim, rdim, rcdim)

#define DUNE_PYMOR_FUNCTION_CHECKERBOARD_LIST_RANGEFIELDTYPES(etype, dftype, ddim, rdim, rcdim) \
  DUNE_PYMOR_FUNCTION_CHECKERBOARD_LAST_EXPANSION(etype, dftype, ddim, double, rdim, rcdim) \
  DUNE_PYMOR_FUNCTION_CHECKERBOARD_LAST_EXPANSION(etype, dftype, ddim, long double, rdim, rcdim)

#define DUNE_PYMOR_FUNCTION_CHECKERBOARD_LAST_EXPANSION(etype, dftype, ddim, rftype, rdim, rcdim) \
  extern template class Dune::Pymor::Function::Checkerboard< etype, dftype, ddim, rftype, rdim, rcdim >;

#ifdef HAVE_DUNE_GRID

DUNE_PYMOR_FUNCTION_CHECKERBOARD_LIST_DIMRANGE(DunePymorFunctionsInterfacesSGrid1dEntityType, 1)
DUNE_PYMOR_FUNCTION_CHECKERBOARD_LIST_DIMRANGE(DunePymorFunctionsInterfacesSGrid2dEntityType, 2)
DUNE_PYMOR_FUNCTION_CHECKERBOARD_LIST_DIMRANGE(DunePymorFunctionsInterfacesSGrid3dEntityType, 3)

DUNE_PYMOR_FUNCTION_CHECKERBOARD_LIST_DIMRANGE(DunePymorFunctionsInterfacesYaspGrid1dEntityType, 1)
DUNE_PYMOR_FUNCTION_CHECKERBOARD_LIST_DIMRANGE(DunePymorFunctionsInterfacesYaspGrid2dEntityType, 2)
DUNE_PYMOR_FUNCTION_CHECKERBOARD_LIST_DIMRANGE(DunePymorFunctionsInterfacesYaspGrid3dEntityType, 3)

# if HAVE_ALUGRID_SERIAL_H || HAVE_ALUGRID_PARALLEL_H

DUNE_PYMOR_FUNCTION_CHECKERBOARD_LIST_DIMRANGE(DunePymorFunctionsInterfacesAluSimplexGrid2dEntityType, 2)
DUNE_PYMOR_FUNCTION_CHECKERBOARD_LIST_DIMRANGE(DunePymorFunctionsInterfacesAluSimplexGrid3dEntityType, 3)
DUNE_PYMOR_FUNCTION_CHECKERBOARD_LIST_DIMRANGE(DunePymorFunctionsInterfacesAluCubeGrid3dEntityType, 3)

# endif // HAVE_ALUGRID_SERIAL_H || HAVE_ALUGRID_PARALLEL_H
#endif // HAVE_DUNE_GRID

#undef DUNE_PYMOR_FUNCTION_CHECKERBOARD_LAST_EXPANSION
#undef DUNE_PYMOR_FUNCTION_CHECKERBOARD_LIST_RANGEFIELDTYPES
#undef DUNE_PYMOR_FUNCTION_CHECKERBOARD_LIST_DOMAINFIELDTYPES
#undef DUNE_PYMOR_FUNCTION_CHECKERBOARD_LIST_DIMRANGECOLS
#undef DUNE_PYMOR_FUNCTION_CHECKERBOARD_LIST_DIMRANGE
#undef DUNE_PYMOR_FUNCTION_CHECKERBOARD_LIST_CLASSES
#endif // DUNE_PYMOR_FUNCTIONS_TO_LIB

#endif // DUNE_PYMOR_FUNCTIONS_CHECKERBOARD_HH
