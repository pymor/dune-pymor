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
#include <dune/stuff/common/fvector.hh>

#include <dune/pymor/common/exceptions.hh>
#include <dune/pymor/parameters/functional.hh>
#include <dune/pymor/functions/default.hh>

#include "interfaces.hh"

namespace Dune {
namespace Pymor {
namespace Functions {


template< class EntityImp, class DomainFieldImp, int domainDim, class RangeFieldImp, int rangeDim, int rangeDimCols = 1 >
class Checkerboard
  : public AffinelyDecomposableDefault < EntityImp, DomainFieldImp, domainDim, RangeFieldImp, rangeDim, rangeDimCols >
{
  typedef Checkerboard< EntityImp, DomainFieldImp, domainDim, RangeFieldImp, rangeDim, rangeDimCols > ThisType;
  typedef AffinelyDecomposableDefault
      < EntityImp, DomainFieldImp, domainDim, RangeFieldImp, rangeDim, rangeDimCols >                 BaseType;
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

  static const bool available = true;

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
          cfg.get("lower_left",     default_cfg.get< DomainType >("lower_left")),
          cfg.get("upper_right",    default_cfg.get< DomainType >("upper_right")),
          cfg.get("num_elements",   default_cfg.get< Dune::FieldVector< size_t, dimDomain > >("num_elements")),
          cfg.get("parameter_name", default_cfg.get< std::string >("parameter_name")),
          cfg.get("name",           default_cfg.get< std::string >("name"))
    );
  } // ... create(...)

  Checkerboard(const Stuff::Common::FieldVector< DomainFieldType, dimDomain >& lowerLeft,
               const Stuff::Common::FieldVector< DomainFieldType, dimDomain >& upperRight,
               const Stuff::Common::FieldVector< size_t, dimDomain  >& numElements,
               const std::string parameterName = "value",
               const std::string name = static_id())
    : BaseType(name)
  {
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
      BaseType::register_component(new NonparametricCheckerboardType(lowerLeft,
                                                                     upperRight,
                                                                     numElements,
                                                                     std::move(indicator),
                                                                     name + "_component_" + DSC::toString(ii)),
                                   new ParameterFunctional(parameterType,
                                                           parameterName + "[" + DSC::toString(ii) + "]"));
    } // create the coefficients and components
  } // Checkerboard()

  virtual std::string type() const override
  {
    return BaseType::BaseType::static_id() + ".checkerboard";
  }
}; // class Checkerboard


} // namespace Functions
} // namespace Pymor
} // namespace Dune

#endif // DUNE_PYMOR_FUNCTIONS_CHECKERBOARD_HH
