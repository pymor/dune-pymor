// This file is part of the dune-pymor project:
//   https://github.com/pymor/dune-pymor
// Copyright holders: Stephan Rave, Felix Schindler
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_FUNCTIONS_CHECKERBOARD_HH
#define DUNE_PYMOR_FUNCTIONS_CHECKERBOARD_HH

#include <vector>
#include <memory>

#include <dune/stuff/functions/checkerboard.hh>
#include <dune/stuff/common/configuration.hh>
#include <dune/stuff/common/memory.hh>

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

  static Stuff::Common::Configuration default_config(const std::string sub_name = "")
  {
    Stuff::Common::Configuration config;
    config["lower_left"] = "[0.0 0.0 0.0]";
    config["upper_right"] = "[1.0 1.0 1.0]";
    config["num_elements"] = "[2 2 2]";
    config["parameter_name"] = "value";
    config["name"] = static_id();
    if (sub_name.empty())
      return config;
    else {
      Stuff::Common::Configuration tmp;
      tmp.add(config, sub_name);
      return tmp;
    }
  } // ... default_config(...)

  static std::unique_ptr< ThisType > create(const Stuff::Common::Configuration config = default_config(),
                                            const std::string sub_name = static_id())

  {
    // get correct config
    const Stuff::Common::Configuration cfg = config.has_sub(sub_name) ? config.sub(sub_name) : config;
    const Stuff::Common::Configuration default_cfg = default_config();
    // create
    return Stuff::Common::make_unique< ThisType >(
          cfg.get("lower_left",     default_cfg.get< std::vector< DomainFieldType > >("lower_left")),
          cfg.get("upper_right",    default_cfg.get< std::vector< DomainFieldType > >("upper_right")),
          cfg.get("num_elements",   default_cfg.get< std::vector< size_t > >("num_elements")),
          cfg.get("parameter_name", default_cfg.get< std::string >("parameter_name")),
          cfg.get("name",           default_cfg.get< std::string >("name"))
    );
  } // ... create(...)

  Checkerboard(const std::vector< DomainFieldType >& lowerLeft,
               const std::vector< DomainFieldType >& upperRight,
               const std::vector< size_t >& numElements,
               const std::string parameterName = "value",
               const std::string name = static_id())
    : BaseType(name)
  {
    // check input
    if (numElements.size() < dimDomain)
      DUNE_THROW(Stuff::Exceptions::shapes_do_not_match,
                 "numElements has to be at least of size " << dimDomain << " is(" << numElements.size() << ")!");
    size_t parameterSize = 1;
    for (size_t dd = 0; dd < dimDomain; ++dd) {
      if (lowerLeft[dd] >= upperRight[dd])
        DUNE_THROW(Stuff::Exceptions::wrong_input_given,
                   "lowerLeft[" << dd << "] = " << lowerLeft[dd] << " has to be smaller than upperRight[" << dd << "]!");
      if (numElements[dd] <= 0)
        DUNE_THROW(Stuff::Exceptions::wrong_input_given,
                   "numElements[" << dd << "] has to be positive (is " << numElements[dd] << ")!");
      parameterSize *= numElements[dd];
    }
    if (parameterName.empty())
      DUNE_THROW(Stuff::Exceptions::wrong_input_given, "parameterName must not be empty!");
    // build parameter
    const ParameterType parameterType(parameterName, parameterSize);
    // create the coefficients and components
    typedef Dune::Stuff::Functions::Checkerboard
        < EntityType, DomainFieldType, dimDomain, RangeFieldType, dimRange, dimRangeCols >
      NonparametricCheckerboardType;
    for (size_t ii = 0; ii < parameterSize; ++ii) {
      std::vector< RangeType > indicator(parameterSize, RangeType(0));
      indicator[ii] = RangeType(1);
      BaseType::register_component(new NonparametricCheckerboardType(std::vector< DomainFieldType >(lowerLeft),
                                                                     std::vector< DomainFieldType >(upperRight),
                                                                     std::vector< size_t >(numElements),
                                                                     std::move(indicator),
                                                                     name + "_component_" + DSC::toString(ii)),
                                   new ParameterFunctional(parameterType,
                                                           parameterName + "[" + DSC::toString(ii) + "]"));
    } // create the coefficients and components
  } // Checkerboard()

  virtual std::string type() const DS_OVERRIDE
  {
    return BaseType::BaseType::static_id() + ".checkerboard";
  }
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
