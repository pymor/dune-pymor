// This file is part of the dune-pymor project:
//   https://github.com/pymor/dune-pymor
// Copyright holders: Stephan Rave, Felix Schindler
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_FUNCTIONS_DEFAULT_HH
#define DUNE_PYMOR_FUNCTIONS_DEFAULT_HH

#include <memory>

#include <dune/stuff/functions/interfaces.hh>
#include <dune/stuff/functions/expression.hh>
#include <dune/stuff/common/configtree.hh>
#include <dune/stuff/common/string.hh>
#include <dune/stuff/functions/expression.hh>
#include <dune/stuff/functions.hh>
#include <dune/stuff/common/memory.hh>
#include <dune/stuff/common/exceptions.hh>

#include <dune/pymor/parameters/base.hh>
#include <dune/pymor/parameters/functional.hh>

#include "interfaces.hh"

namespace Dune {
namespace Pymor {
namespace Function {


template< class EntityImp, class DomainFieldImp, int domainDim, class RangeFieldImp, int rangeDim, int rangeDimCols = 1 >
class NonparametricDefault
  : public AffinelyDecomposableFunctionInterface
        < EntityImp, DomainFieldImp, domainDim, RangeFieldImp, rangeDim, rangeDimCols >
{
  typedef AffinelyDecomposableFunctionInterface
      < EntityImp, DomainFieldImp, domainDim, RangeFieldImp, rangeDim, rangeDimCols > BaseType;
  typedef NonparametricDefault< EntityImp, DomainFieldImp, domainDim, RangeFieldImp, rangeDim, rangeDimCols > ThisType;
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

  typedef typename BaseType::JacobianRangeType JacobianRangeType;

  /**
   * \attention This class takes ownership of nonparametric_ptr (in the sense, that you must not delete it manually)!
   */
  NonparametricDefault(const NonparametricType* nonparametric_ptr)
    : BaseType()
    , nonparametric_(std::shared_ptr< const NonparametricType >(nonparametric_ptr))
  {}

  NonparametricDefault(const std::shared_ptr< const NonparametricType > nonparametric_ptr)
    : BaseType()
    , nonparametric_(nonparametric_ptr)
  {}

  virtual std::string type() const DS_OVERRIDE
  {
    return BaseType::static_id() + ".nonparametricdefault";
  }

  virtual std::string name() const DS_OVERRIDE
  {
    return nonparametric_->name();
  }

  virtual std::shared_ptr< const NonparametricType > affine_part() const DS_OVERRIDE
  {
    return nonparametric_;
  }

  virtual DUNE_STUFF_SSIZE_T num_components() const DS_OVERRIDE
  {
    return 0;
  }

private:
  std::shared_ptr< const NonparametricType > nonparametric_;
}; // class NonparametricDefault


template< class EntityImp, class DomainFieldImp, int domainDim, class RangeFieldImp, int rangeDim, int rangeDimCols = 1 >
class AffinelyDecomposableDefault
  : public AffinelyDecomposableFunctionInterface< EntityImp, DomainFieldImp, domainDim, RangeFieldImp, rangeDim, rangeDimCols >
{
  typedef AffinelyDecomposableDefault
      < EntityImp, DomainFieldImp, domainDim, RangeFieldImp, rangeDim, rangeDimCols > ThisType;
protected:
  typedef AffinelyDecomposableFunctionInterface
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

  typedef typename BaseType::JacobianRangeType JacobianRangeType;

  static std::string static_id()
  {
    return BaseType::static_id() + ".affinelydecomposabledefault";
  }

  static Stuff::Common::ConfigTree default_config(const std::string sub_name = "")
  {
    Stuff::Common::ConfigTree config;
    typedef Stuff::FunctionsProvider< EntityType, DomainFieldType, dimDomain, RangeFieldType, dimRange, dimRangeCols >
        NonparametricFunctions;
    const std::string nonparametricType = NonparametricFunctions::available()[0];
    config.add(NonparametricFunctions::default_config(nonparametricType), "component.0");
    config["component.0.type"] = nonparametricType;
    config["component.0.name"] = "component_0";
    config["coefficient.0.diffusion"] = "1";
    config["coefficient.0.expression"] = "diffusion[0]";
    config.add(NonparametricFunctions::default_config(nonparametricType), "component.1");
    config["component.1.type"] = nonparametricType;
    config["component.1.name"] = "component_1";
    config["coefficient.1.force"] = "2";
    config["coefficient.1.expression"] = "force[0] + sin(force[1])";
    config.add(NonparametricFunctions::default_config(nonparametricType), "affine_part");
    config["affine_part.type"] = nonparametricType;
    config["affine_part.name"] = "affine_part";
    config["name"] = static_id();
    if (sub_name.empty())
      return config;
    else {
      Stuff::Common::ConfigTree tmp;
      tmp.add(config, sub_name);
      return tmp;
    }
  } // ... default_config(...)

  static std::unique_ptr< ThisType > create(const Stuff::Common::ConfigTree config = default_config(),
                                            const std::string sub_name = static_id())

  {
    const Stuff::Common::ConfigTree cfg = config.has_sub(sub_name) ? config.sub(sub_name) : config;
    typedef Stuff::FunctionsProvider< EntityType, DomainFieldType, dimDomain, RangeFieldType, dimRange, dimRangeCols >
        NonparametricFunctions;
    const std::string name = cfg.get< std::string >("name", static_id());
    auto ret = Stuff::Common::make_unique< ThisType >(name);
    if (cfg.has_sub("affine_part")) {
      auto affinePartCfg = cfg.sub("affine_part");
      if (!affinePartCfg.has_key("type"))
        DUNE_THROW_COLORFULLY(Stuff::Exceptions::configuration_error,
                              "no 'type' given in the following 'affine_part' config:\n\n" << affinePartCfg);
      if (!affinePartCfg.has_key("name"))
        affinePartCfg["name"] = name + ", affine part";
      const std::string type = affinePartCfg.get< std::string >("type");
      ret->register_affine_part(NonparametricFunctions::create(type, affinePartCfg));
    }
    size_t pp = 0;
    while (cfg.has_sub("component." + Stuff::Common::toString(pp))
           && cfg.has_sub("coefficient." + Stuff::Common::toString(pp))) {
      auto componentCfg = cfg.sub("component." + Stuff::Common::toString(pp));
      if (!componentCfg.has_key("type"))
        DUNE_THROW_COLORFULLY(Stuff::Exceptions::configuration_error,
                              "no 'type' given in the following 'component." << pp << "' config:\n\n"
                              << componentCfg);
      if (!componentCfg.has_key("name"))
        componentCfg["name"] = name + ", component " + Stuff::Common::toString(pp);
      const std::string componentType = componentCfg.get< std::string >("type");
      const auto coefficientCfg = cfg.sub("coefficient." + Stuff::Common::toString(pp));
      if (!coefficientCfg.has_key("expression"))
        DUNE_THROW_COLORFULLY(Stuff::Exceptions::configuration_error,
                              "no 'expression' given in the following 'coefficient." << pp << "' config:\n\n"
                              << coefficientCfg);
      const std::string coefficientExpression = coefficientCfg.get< std::string >("expression");
      ParameterType coefficientMu;
      for (std::string key : coefficientCfg.getValueKeys()) {
        if (key != "expression")
          coefficientMu.set(key, coefficientCfg.get< int >(key));
      }
      if (coefficientMu.empty())
        DUNE_THROW_COLORFULLY(Stuff::Exceptions::configuration_error,
                              "no 'key = size' pair given in the following 'coefficient." << pp << "' config:\n\n"
                              << coefficientCfg);
      ret->register_component(NonparametricFunctions::create(componentType, componentCfg),
                              new ParameterFunctional(coefficientMu, coefficientExpression));
      ++pp;
    }
    if (cfg.has_sub("component." + Stuff::Common::toString(pp))
        && !cfg.has_sub("coefficient." + Stuff::Common::toString(pp)))
      DUNE_THROW_COLORFULLY(Stuff::Exceptions::configuration_error,
                            "missing 'coefficient." << pp << "' to match 'component." << pp
                            << "' in the following config:\n\n" << cfg);
    if (!cfg.has_sub("component." + Stuff::Common::toString(pp))
        && cfg.has_sub("coefficient." + Stuff::Common::toString(pp)))
      DUNE_THROW_COLORFULLY(Stuff::Exceptions::configuration_error,
                            "missing 'component." << pp << "' to match 'coefficient." << pp
                            << "' in the following config:\n\n" << cfg);
    return ret;
  } // ... create(...)

  AffinelyDecomposableDefault(const std::string nm = static_id())
    : BaseType()
    , name_(nm)
    , num_components_(0)
    , hasAffinePart_(false)
  {}

  /**
   * \attention This class takes ownership of aff_ptr (in the sense, that you must not delete it manually)!
   */
  AffinelyDecomposableDefault(const NonparametricType* aff_ptr, const std::string nm = static_id())
    : BaseType()
    , name_(nm)
    , num_components_(0)
    , hasAffinePart_(true)
    , affinePart_(std::shared_ptr< const NonparametricType >(aff_ptr))
  {}

  AffinelyDecomposableDefault(const std::shared_ptr< const NonparametricType > aff_ptr,
                              const std::string nm = static_id())
    : BaseType()
    , name_(nm)
    , num_components_(0)
    , hasAffinePart_(true)
    , affinePart_(aff_ptr)
  {}

  /**
   * \attention This class takes ownership of comp_ptr and coeff_ptr (in the sense, that you must not delete it
   *            manually)!
   */
  AffinelyDecomposableDefault(const NonparametricType* comp_ptr,
                              const ParameterFunctional* coeff_ptr,
                              const std::string nm = static_id())
    : BaseType()
    , name_(nm)
    , num_components_(1)
    , hasAffinePart_(false)
  {
    components_.emplace_back(comp_ptr);
    coefficients_.emplace_back(coeff_ptr);
    this->inherit_parameter_type(coeff_ptr->parameter_type(), "coefficient_0");
  }

  /**
   * \attention This class takes ownership of comp_ptr (in the sense, that you must not delete it manually)!
   */
  AffinelyDecomposableDefault(const NonparametricType* comp_ptr,
                              const std::shared_ptr< const ParameterFunctional > coeff_ptr,
                              const std::string nm = static_id())
    : BaseType()
    , name_(nm)
    , num_components_(1)
    , hasAffinePart_(false)
  {
    components_.emplace_back(comp_ptr);
    coefficients_.push_back(coeff_ptr);
    this->inherit_parameter_type(coeff_ptr->parameter_type(), "coefficient_0");
  }

  /**
   * \attention This class takes ownership of coeff_ptr (in the sense, that you must not delete it manually)!
   */
  AffinelyDecomposableDefault(const std::shared_ptr< const NonparametricType > comp_ptr,
                              const ParameterFunctional* coeff_ptr,
                              const std::string nm = static_id())
    : BaseType()
    , name_(nm)
    , num_components_(1)
    , hasAffinePart_(false)
  {
    components_.push_back(comp_ptr);
    coefficients_.emplace_back(coeff_ptr);
    this->inherit_parameter_type(coeff_ptr->parameter_type(), "coefficient_0");
  }

  AffinelyDecomposableDefault(const std::shared_ptr< const NonparametricType > comp_ptr,
                              const std::shared_ptr< const ParameterFunctional > coeff_ptr,
                              const std::string nm = static_id())
    : BaseType()
    , name_(nm)
    , num_components_(1)
    , hasAffinePart_(false)
  {
    components_.push_back(comp_ptr);
    coefficients_.push_back(coeff_ptr);
    this->inherit_parameter_type(coeff_ptr->parameter_type(), "coefficient_0");
  }

  virtual std::string type() const DS_OVERRIDE
  {
    return BaseType::static_id() + ".affinelydecomposabledefault";
  }

  /**
   * \attention This class takes ownership of aff_ptr (in the sense, that you must not delete it manually)!
   */
  void register_affine_part(const NonparametricType* aff_ptr)
  {
    register_affine_part(std::shared_ptr< const NonparametricType >(aff_ptr));
  }

  void register_affine_part(const std::shared_ptr< const NonparametricType > aff_ptr)
  {
    if (hasAffinePart_)
      DUNE_THROW_COLORFULLY(Stuff::Exceptions::you_are_using_this_wrongly,
                            "do not call register_affine_part() if has_affine_part() == true!");
    affinePart_ = aff_ptr;
    hasAffinePart_ = true;
  } // ... register_component(...)

  /**
   * \attention This class takes ownership of comp_ptr and coeff_ptr (in the sense, that you must not delete it
   *            manually)!
   */
  void register_component(const NonparametricType* comp_ptr, const ParameterFunctional* coeff_ptr)
  {
    register_component(std::shared_ptr< const NonparametricType >(comp_ptr),
                       std::shared_ptr< const ParameterFunctional >(coeff_ptr));
  }

  /**
   * \attention This class takes ownership of comp_ptr (in the sense, that you must not delete it manually)!
   */
  void register_component(const NonparametricType* comp_ptr,
                          const std::shared_ptr< const ParameterFunctional > coeff_ptr)
  {
    register_component(comp_ptr, std::shared_ptr< const ParameterFunctional >(coeff_ptr));
  }

  /**
   * \attention This class takes ownership of coeff_ptr (in the sense, that you must not delete it manually)!
   */
  void register_component(const std::shared_ptr< const NonparametricType > comp_ptr,
                          const ParameterFunctional* coeff_ptr)
  {
    register_component(comp_ptr, std::shared_ptr< const ParameterFunctional >(coeff_ptr));
  }

  void register_component(const std::shared_ptr< const NonparametricType > comp_ptr,
                          const std::shared_ptr< const ParameterFunctional > coeff_ptr)
  {
    Parametric::inherit_parameter_type(coeff_ptr->parameter_type(), "coefficient_" + Dune::Stuff::Common::toString(num_components_));
    components_.push_back(comp_ptr);
    coefficients_.push_back(coeff_ptr);
    ++num_components_;
  } // ... register_component(...)

  virtual std::string name() const DS_OVERRIDE
  {
    return name_;
  }

  virtual bool has_affine_part() const DS_OVERRIDE
  {
    return hasAffinePart_;
  }

  virtual std::shared_ptr< const NonparametricType > affine_part() const DS_OVERRIDE
  {
    if (!hasAffinePart_)
      DUNE_THROW_COLORFULLY(Stuff::Exceptions::you_are_using_this_wrongly,
                            "do not call affine_part() if has_affine_part() == false!");
    return affinePart_;
  }

  virtual DUNE_STUFF_SSIZE_T num_components() const DS_OVERRIDE
  {
    return num_components_;
  }

  virtual std::shared_ptr< const NonparametricType > component(const DUNE_STUFF_SSIZE_T qq) const DS_OVERRIDE
  {
    if (num_components_ == 0)
      DUNE_THROW_COLORFULLY(Stuff::Exceptions::requirements_not_met,
                            "do not call component(qq) if num_components() == 0!");
    if (qq < 0 || qq >= int(num_components_))
      DUNE_THROW_COLORFULLY(Stuff::Exceptions::index_out_of_range,
                            "the condition 0 < " << qq << " < num_components() = " << num_components_
                            << " is not satisfied!");
    return components_[qq];
  }

  virtual std::shared_ptr< const ParameterFunctional > coefficient(const DUNE_STUFF_SSIZE_T qq) const DS_OVERRIDE
  {
    if (num_components_ == 0)
      DUNE_THROW_COLORFULLY(Stuff::Exceptions::requirements_not_met,
                            "do not call coefficient(qq) if size() == 0!");
    if (qq < 0 || qq >= int(num_components_))
      DUNE_THROW_COLORFULLY(Stuff::Exceptions::index_out_of_range,
                            "the condition 0 < " << qq << " < num_components() = " << num_components_
                            << " is not satisfied!");
    return coefficients_[qq];
  }

public:
  std::string name_;
  size_t num_components_;
  bool hasAffinePart_;
  std::vector< std::shared_ptr< const NonparametricType > > components_;
  std::vector< std::shared_ptr< const ParameterFunctional > > coefficients_;
  std::shared_ptr< const NonparametricType > affinePart_;
}; // class AffinelyDecomposableDefault


} // namespace Function
} // namespace Pymor
} // namespace Dune

#ifdef DUNE_PYMOR_FUNCTIONS_TO_LIB
#define DUNE_PYMOR_FUNCTION_DEFAULT_LIST_DIMRANGE(etype, ddim) \
  DUNE_PYMOR_FUNCTION_DEFAULT_LIST_DIMRANGECOLS(etype, ddim, 1) \
  DUNE_PYMOR_FUNCTION_DEFAULT_LIST_DIMRANGECOLS(etype, ddim, 2) \
  DUNE_PYMOR_FUNCTION_DEFAULT_LIST_DIMRANGECOLS(etype, ddim, 3)

#define DUNE_PYMOR_FUNCTION_DEFAULT_LIST_DIMRANGECOLS(etype, ddim, rdim) \
  DUNE_PYMOR_FUNCTION_DEFAULT_LIST_DOMAINFIELDTYPES(etype, ddim, rdim, 1) \
  DUNE_PYMOR_FUNCTION_DEFAULT_LIST_DOMAINFIELDTYPES(etype, ddim, rdim, 2) \
  DUNE_PYMOR_FUNCTION_DEFAULT_LIST_DOMAINFIELDTYPES(etype, ddim, rdim, 3)

#define DUNE_PYMOR_FUNCTION_DEFAULT_LIST_DOMAINFIELDTYPES(etype, ddim, rdim, rcdim) \
  DUNE_PYMOR_FUNCTION_DEFAULT_LIST_RANGEFIELDTYPES(etype, double, ddim, rdim, rcdim)

#define DUNE_PYMOR_FUNCTION_DEFAULT_LIST_RANGEFIELDTYPES(etype, dftype, ddim, rdim, rcdim) \
  DUNE_PYMOR_FUNCTION_DEFAULT_LAST_EXPANSION(etype, dftype, ddim, double, rdim, rcdim) \
  DUNE_PYMOR_FUNCTION_DEFAULT_LAST_EXPANSION(etype, dftype, ddim, long double, rdim, rcdim)

#define DUNE_PYMOR_FUNCTION_DEFAULT_LAST_EXPANSION(etype, dftype, ddim, rftype, rdim, rcdim) \
  extern template class Dune::Pymor::Function::NonparametricDefault< etype, dftype, ddim, rftype, rdim, rcdim >; \
  extern template class Dune::Pymor::Function::AffinelyDecomposableDefault< etype, dftype, ddim, rftype, rdim, rcdim >;

#ifdef HAVE_DUNE_GRID

DUNE_PYMOR_FUNCTION_DEFAULT_LIST_DIMRANGE(DunePymorFunctionsInterfacesSGrid1dEntityType, 1)
DUNE_PYMOR_FUNCTION_DEFAULT_LIST_DIMRANGE(DunePymorFunctionsInterfacesSGrid2dEntityType, 2)
DUNE_PYMOR_FUNCTION_DEFAULT_LIST_DIMRANGE(DunePymorFunctionsInterfacesSGrid3dEntityType, 3)

DUNE_PYMOR_FUNCTION_DEFAULT_LIST_DIMRANGE(DunePymorFunctionsInterfacesYaspGrid1dEntityType, 1)
DUNE_PYMOR_FUNCTION_DEFAULT_LIST_DIMRANGE(DunePymorFunctionsInterfacesYaspGrid2dEntityType, 2)
DUNE_PYMOR_FUNCTION_DEFAULT_LIST_DIMRANGE(DunePymorFunctionsInterfacesYaspGrid3dEntityType, 3)

# if HAVE_ALUGRID_SERIAL_H || HAVE_ALUGRID_PARALLEL_H

DUNE_PYMOR_FUNCTION_DEFAULT_LIST_DIMRANGE(DunePymorFunctionsInterfacesAluSimplexGrid2dEntityType, 2)
DUNE_PYMOR_FUNCTION_DEFAULT_LIST_DIMRANGE(DunePymorFunctionsInterfacesAluSimplexGrid3dEntityType, 3)
DUNE_PYMOR_FUNCTION_DEFAULT_LIST_DIMRANGE(DunePymorFunctionsInterfacesAluCubeGrid3dEntityType, 3)

# endif // HAVE_ALUGRID_SERIAL_H || HAVE_ALUGRID_PARALLEL_H
#endif // HAVE_DUNE_GRID

#undef DUNE_PYMOR_FUNCTION_DEFAULT_LAST_EXPANSION
#undef DUNE_PYMOR_FUNCTION_DEFAULT_LIST_RANGEFIELDTYPES
#undef DUNE_PYMOR_FUNCTION_DEFAULT_LIST_DOMAINFIELDTYPES
#undef DUNE_PYMOR_FUNCTION_DEFAULT_LIST_DIMRANGECOLS
#undef DUNE_PYMOR_FUNCTION_DEFAULT_LIST_DIMRANGE
#undef DUNE_PYMOR_FUNCTION_DEFAULT_LIST_CLASSES
#endif // DUNE_PYMOR_FUNCTIONS_TO_LIB

#endif // DUNE_PYMOR_FUNCTIONS_DEFAULT_HH
