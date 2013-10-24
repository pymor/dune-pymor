// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_FUNCTIONS_DEFAULT_HH
#define DUNE_PYMOR_FUNCTIONS_DEFAULT_HH

#include <memory>

#include <dune/stuff/functions/interfaces.hh>
#include <dune/stuff/functions/expression.hh>
#include <dune/stuff/common/parameter/tree.hh>
#include <dune/stuff/common/string.hh>
#include <dune/stuff/functions/expression.hh>
#include <dune/stuff/functions.hh>

#include <dune/pymor/parameters/base.hh>
#include <dune/pymor/common/exceptions.hh>
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

  virtual std::string name() const DS_OVERRIDE
  {
    return nonparametric_->name();
  }

  virtual std::shared_ptr< const NonparametricType > affine_part() const DS_OVERRIDE
  {
    return nonparametric_;
  }

  virtual DUNE_PYMOR_SSIZE_T num_components() const DS_OVERRIDE
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

  AffinelyDecomposableDefault(const std::string nm = static_id())
    : BaseType()
    , name_(nm)
    , num_components_(0)
    , hasAffinePart_(false)
  {}

  /**
   * \attention This class takes ownership of aff_ptr (in the sense, that you must not delete it manually)!
   */
  AffinelyDecomposableDefault(const NonparametricType* aff_ptr,
                              const std::string nm = static_id())
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

  /**
   * \attention This class takes ownership of aff_ptr (in the sense, that you must not delete it manually)!
   */
  void register_affine_part(const NonparametricType* aff_ptr)
  {
    if (hasAffinePart_)
      DUNE_PYMOR_THROW(Exception::this_does_not_make_any_sense,
                       "do not call register_affine_part() if has_affine_part() == true!");
    affinePart_ = std::shared_ptr< const NonparametricType >(aff_ptr);
    hasAffinePart_ = true;
  } // ... register_component(...)

  void register_affine_part(const std::shared_ptr< const NonparametricType > aff_ptr)
  {
    register_affine_part(std::shared_ptr< const NonparametricType >(aff_ptr));
  }

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
    register_component(comp_ptr,
                       std::shared_ptr< const ParameterFunctional >(coeff_ptr));
  }

  /**
   * \attention This class takes ownership of coeff_ptr (in the sense, that you must not delete it manually)!
   */
  void register_component(const std::shared_ptr< const NonparametricType > comp_ptr,
                          const ParameterFunctional* coeff_ptr)
  {
    register_component(std::shared_ptr< const NonparametricType >(comp_ptr),
                       coeff_ptr);
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
      DUNE_PYMOR_THROW(Exception::requirements_not_met,
                       "do not call affine_part() if has_affine_part() == false!");
    return affinePart_;
  }

  virtual DUNE_PYMOR_SSIZE_T num_components() const DS_OVERRIDE
  {
    return num_components_;
  }

  virtual std::shared_ptr< const NonparametricType > component(const DUNE_PYMOR_SSIZE_T qq) const DS_OVERRIDE
  {
    if (num_components_ == 0)
      DUNE_PYMOR_THROW(Exception::requirements_not_met,
                       "do not call component(qq) if num_components() == 0!");
    if (qq < 0 || qq >= int(num_components_))
      DUNE_PYMOR_THROW(Exception::index_out_of_range,
                       "the condition 0 < " << qq << " < num_components() = " << num_components_
                       << " is not satisfied!");
    return components_[qq];
  }

  virtual std::shared_ptr< const ParameterFunctional > coefficient(const DUNE_PYMOR_SSIZE_T qq) const DS_OVERRIDE
  {
    if (num_components_ == 0)
      DUNE_PYMOR_THROW(Exception::requirements_not_met,
                       "do not call coefficient(qq) if size() == 0!");
    if (qq < 0 || qq >= int(num_components_))
      DUNE_PYMOR_THROW(Exception::index_out_of_range,
                       "the condition 0 < " << qq << " < num_components() = " << num_components_
                       << " is not satisfied!");
    return coefficients_[qq];
  }

  static Dune::ParameterTree defaultSettings(const std::string subName = "")

  {
    DSC::ExtendedParameterTree settings;
    typedef Stuff::Functions< EntityType, DomainFieldType, dimDomain, RangeFieldType, dimRange, dimRangeCols >
        NonparametricFunctions;
    const std::string nonparametricType = NonparametricFunctions::available()[0];
    settings.add(NonparametricFunctions::defaultSettings(nonparametricType), "component.0");
    settings["component.0.type"] = nonparametricType;
    settings["coefficient.0.diffusion"] = "1";
    settings["coefficient.0.expression"] = "diffusion[0]";
    settings.add(NonparametricFunctions::defaultSettings(nonparametricType), "component.1");
    settings["component.1.type"] = nonparametricType;
    settings["coefficient.1.force"] = "2";
    settings["coefficient.1.expression"] = "force[0] + sin(force[1])";
    settings.add(NonparametricFunctions::defaultSettings(nonparametricType), "affine_part");
    settings["affine_part.type"] = nonparametricType;
    settings["name"] = static_id();
    settings["order"] = "4";
    if (subName.empty())
      return settings;
    else {
      DSC::ExtendedParameterTree extendedSettings;
      extendedSettings.add(settings, subName);
      return extendedSettings;
    }
  } // ... defaultSettings(...)

  static ThisType* create(const DSC::ExtendedParameterTree settings = defaultSettings())

  {
    typedef Stuff::Functions< EntityType, DomainFieldType, dimDomain, RangeFieldType, dimRange, dimRangeCols >
        NonparametricFunctions;
    const std::string name = settings.get< std::string >("name", static_id());
    ThisType* ret = new ThisType(name);
    if (settings.hasSub("affine_part")) {
      auto affinePartSettings = settings.sub("affine_part");
      if (!affinePartSettings.hasKey("type"))
        DUNE_PYMOR_THROW(Exception::wrong_option_given,
                         "no 'type' given in the following 'affine_part' settings:\n\n"
                         << affinePartSettings.reportString());
      if (!affinePartSettings.hasKey("name"))
        affinePartSettings["name"] = name + ", affine part";
      const std::string type = affinePartSettings.get< std::string >("type");
      ret->register_affine_part(NonparametricFunctions::create(type, affinePartSettings));
    }
    size_t pp = 0;
    while (settings.hasSub("component." + Stuff::Common::toString(pp))
           && settings.hasSub("coefficient." + Stuff::Common::toString(pp))) {
      auto componentSettings = settings.sub("component." + Stuff::Common::toString(pp));
      if (!componentSettings.hasKey("type"))
        DUNE_PYMOR_THROW(Exception::wrong_option_given,
                         "no 'type' given in the following 'component." << pp << "' settings:\n\n"
                         << componentSettings.reportString());
      if (!componentSettings.hasKey("name"))
        componentSettings["name"] = name + ", component " + Stuff::Common::toString(pp);
      const std::string componentType = componentSettings.get< std::string >("type");
      const auto coefficientSettings = settings.sub("coefficient." + Stuff::Common::toString(pp));
      if (!coefficientSettings.hasKey("expression"))
        DUNE_PYMOR_THROW(Exception::wrong_option_given,
                         "no 'expression' given in the following 'coefficient." << pp << "' settings:\n\n"
                         << coefficientSettings.reportString());
      const std::string coefficientExpression = coefficientSettings.get< std::string >("expression");
      ParameterType coefficientMu;
      for (std::string key : coefficientSettings.getValueKeys()) {
        if (key != "expression")
          coefficientMu.set(key, coefficientSettings.get< int >(key));
      }
      if (coefficientMu.empty())
        DUNE_PYMOR_THROW(Pymor::Exception::wrong_option_given,
                         "no 'key = size' pair given in the following 'coefficient." << pp << "' settings:\n\n"
                         << coefficientSettings.reportString());
      ret->register_component(NonparametricFunctions::create(componentType, componentSettings),
                              new ParameterFunctional(coefficientMu, coefficientExpression));
      ++pp;
    }
    if (settings.hasSub("component." + Stuff::Common::toString(pp))
        && !settings.hasSub("coefficient." + Stuff::Common::toString(pp)))
      DUNE_PYMOR_THROW(Exception::wrong_option_given,
                       "missing 'coefficient." << pp << "' to match 'component." << pp
                       << "' in the following settings:\n\n" << settings.reportString());
    if (!settings.hasSub("component." + Stuff::Common::toString(pp))
        && settings.hasSub("coefficient." + Stuff::Common::toString(pp)))
      DUNE_PYMOR_THROW(Exception::wrong_option_given,
                       "missing 'component." << pp << "' to match 'coefficient." << pp
                       << "' in the following settings:\n\n" << settings.reportString());
    return ret;
  } // ... create(...)

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
