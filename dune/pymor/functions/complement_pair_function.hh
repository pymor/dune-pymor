// This file is part of the dune-pymor project:
//   https://github.com/pymor/dune-pymor
// Copyright holders: Rene Milk, Stephan Rave, Felix Schindler
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_FUNCTIONS_COMPLEMENT_PAIR_FUNCTION_HH
#define DUNE_PYMOR_FUNCTIONS_COMPLEMENT_PAIR_FUNCTION_HH

#include <vector>
#include <memory>

#include <dune/stuff/functions/random_ellipsoids_function.hh>
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
class ComplementPairPymorFunction
  : public AffinelyDecomposableDefault < EntityImp, DomainFieldImp, domainDim, RangeFieldImp, rangeDim, rangeDimCols >
{
  typedef ComplementPairPymorFunction< EntityImp, DomainFieldImp, domainDim, RangeFieldImp, rangeDim, rangeDimCols > ThisType;
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
    return BaseType::BaseType::static_id() + ".ComplementPairPymorFunction";
  }

  static Stuff::Common::Configuration default_config(const std::string sub_name = "")
  {
    Stuff::Common::Configuration config;
    config["lower_left"] = "[0.0 0.0 0.0]";
    config["upper_right"] = "[1.0 1.0 1.0]";
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
          cfg,
          cfg.get("parameter_name", default_cfg.get< std::string >("parameter_name")),
          cfg.get("name",           default_cfg.get< std::string >("name"))
    );
  } // ... create(...)

  ComplementPairPymorFunction(const Stuff::Common::FieldVector< DomainFieldType, dimDomain >& lowerLeft,
               const Stuff::Common::FieldVector< DomainFieldType, dimDomain >& upperRight,
               const Stuff::Common::Configuration& ellipsoid_cfg,
               const std::string parameterName = "value",
               const std::string name = static_id())
    : BaseType(name)
  {
    constexpr size_t parameterSize = 1u;
    for (size_t dd = 0; dd < dimDomain; ++dd) {
      if (lowerLeft[dd] >= upperRight[dd])
        DUNE_THROW(Stuff::Exceptions::wrong_input_given,
                   "lowerLeft[" << dd << "] = " << lowerLeft[dd] << " has to be smaller than upperRight[" << dd << "]!");
    }
    if (parameterName.empty())
      DUNE_THROW(Stuff::Exceptions::wrong_input_given, "parameterName must not be empty!");
    // build parameter
    const ParameterType parameterType(parameterName, parameterSize);
    // create the coefficients and components
    typedef Dune::Stuff::Functions::RandomEllipsoidsFunction
        < EntityType, DomainFieldType, dimDomain, RangeFieldType, dimRange, dimRangeCols >
      NonparametricFunctionType;

    typedef Dune::Stuff::Functions::Constant
        < EntityType, DomainFieldType, dimDomain, RangeFieldType, dimRange, dimRangeCols >
      ConstantFunctionType;

    BaseType::register_component(new NonparametricFunctionType(lowerLeft,
                                                                   upperRight,
                                                                   ellipsoid_cfg,
                                                                   name + "_component_0"),
                                 new ParameterFunctional(parameterType,
                                                         parameterName + "[0]"));
    BaseType::register_affine_part(new ConstantFunctionType(1));
  } // ComplementPairPymorFunction()

  virtual std::string type() const override
  {
    return BaseType::BaseType::static_id() + ".checkerboard";
  }
}; // class ComplementPairPymorFunction


} // namespace Functions
} // namespace Pymor
} // namespace Dune

#endif // DUNE_PYMOR_FUNCTIONS_COMPLEMENT_PAIR_FUNCTION_HH
