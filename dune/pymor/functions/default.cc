// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifdef HAVE_CMAKE_CONFIG
  #include "cmake_config.h"
#elif defined (HAVE_CONFIG_H)
  #include "config.h"
#endif // HAVE_CMAKE_CONFIG

#include <dune/stuff/common/string.hh>
#include <dune/stuff/functions.hh>

#include <dune/pymor/parameters/base.hh>

#include "default.hh"

namespace Dune {
namespace Pymor {
namespace Function {


// ================================
// ===== NonparametricDeafult =====
// ================================
template< class D, int d, class R, int rR, int rC >
NonparametricDefault< D, d, R, rR, rC >::NonparametricDefault(const NonparametricType* nonparametric_ptr)
  : BaseType()
  , nonparametric_(std::shared_ptr< const NonparametricType >(nonparametric_ptr))
{}

template< class D, int d, class R, int rR, int rC >
NonparametricDefault< D, d, R, rR, rC >::NonparametricDefault(const std::shared_ptr< const NonparametricType > nonparametric_ptr)
  : BaseType()
  , nonparametric_(nonparametric_ptr)
{}

template< class D, int d, class R, int rR, int rC >
std::string NonparametricDefault< D, d, R, rR, rC >::name() const
{
  return nonparametric_->name();
}

template< class D, int d, class R, int rR, int rC >
int NonparametricDefault< D, d, R, rR, rC >::order() const
{
  return nonparametric_->order();
}

template< class D, int d, class R, int rR, int rC >
void NonparametricDefault< D, d, R, rR, rC >::evaluate(const DomainType& x, RangeType& ret, const Parameter mu) const
  throw (Exception::wrong_parameter_type)
{
  if (!mu.type().empty())
    DUNE_PYMOR_THROW(Exception::wrong_parameter_type,
                     "do not call evaluate with mu = " << mu << " if parametric() == false!");
  nonparametric_->evaluate(x, ret);
}

template< class D, int d, class R, int rR, int rC >
bool NonparametricDefault< D, d, R, rR, rC >::affinely_decomposable() const
{
  return true;
}

template< class D, int d, class R, int rR, int rC >
bool NonparametricDefault< D, d, R, rR, rC >::has_affine_part() const
{
  return true;
}

template< class D, int d, class R, int rR, int rC >
std::shared_ptr< const typename NonparametricDefault< D, d, R, rR, rC >::NonparametricType >
NonparametricDefault< D, d, R, rR, rC >::affine_part() const
{
  return nonparametric_;
}

template< class D, int d, class R, int rR, int rC >
unsigned int NonparametricDefault< D, d, R, rR, rC >::num_components() const
{
  return 0;
}

template class NonparametricDefault< double, 1, double, 1, 1 >;
template class NonparametricDefault< double, 1, double, 1, 2 >;
template class NonparametricDefault< double, 1, double, 1, 3 >;
template class NonparametricDefault< double, 1, double, 2, 1 >;
template class NonparametricDefault< double, 1, double, 2, 2 >;
template class NonparametricDefault< double, 1, double, 2, 3 >;
template class NonparametricDefault< double, 1, double, 3, 1 >;
template class NonparametricDefault< double, 1, double, 3, 2 >;
template class NonparametricDefault< double, 1, double, 3, 3 >;
template class NonparametricDefault< double, 2, double, 1, 1 >;
template class NonparametricDefault< double, 2, double, 1, 2 >;
template class NonparametricDefault< double, 2, double, 1, 3 >;
template class NonparametricDefault< double, 2, double, 2, 1 >;
template class NonparametricDefault< double, 2, double, 2, 2 >;
template class NonparametricDefault< double, 2, double, 2, 3 >;
template class NonparametricDefault< double, 2, double, 3, 1 >;
template class NonparametricDefault< double, 2, double, 3, 2 >;
template class NonparametricDefault< double, 2, double, 3, 3 >;
template class NonparametricDefault< double, 3, double, 1, 1 >;
template class NonparametricDefault< double, 3, double, 1, 2 >;
template class NonparametricDefault< double, 3, double, 1, 3 >;
template class NonparametricDefault< double, 3, double, 2, 1 >;
template class NonparametricDefault< double, 3, double, 2, 2 >;
template class NonparametricDefault< double, 3, double, 2, 3 >;
template class NonparametricDefault< double, 3, double, 3, 1 >;
template class NonparametricDefault< double, 3, double, 3, 2 >;
template class NonparametricDefault< double, 3, double, 3, 3 >;


// ===================================
// ===== NonparametricExpression =====
// ===================================
template< class D, int d, class R, int r >
NonparametricExpression< D, d, R, r >::NonparametricExpression(const std::string var,
                                                               const std::string expr,
                                                               const int oo,
                                                               const std::string nm)
  : BaseType()
  , expressionFunction_(std::make_shared< const ExpressionFunctionType >(var, expr, oo, nm))
{}

template< class D, int d, class R, int r >
NonparametricExpression< D, d, R, r >::NonparametricExpression(const std::string var,
                                                               const std::vector< std::string > exprs,
                                                               const int oo,
                                                               const std::string nm)
  : BaseType()
  , expressionFunction_(std::make_shared< const ExpressionFunctionType >(var, exprs, oo, nm))
{}

template< class D, int d, class R, int r >
std::string NonparametricExpression< D, d, R, r >::name() const
{
  return expressionFunction_->name();
}

template< class D, int d, class R, int r >
int NonparametricExpression< D, d, R, r >::order() const
{
  return expressionFunction_->order();
}

template< class D, int d, class R, int r >
void NonparametricExpression< D, d, R, r >::evaluate(const DomainType& x, RangeType& ret, const Parameter mu) const
  throw (Exception::wrong_parameter_type)
{
  if (!mu.type().empty())
    DUNE_PYMOR_THROW(Exception::wrong_parameter_type,
                     "do not call evaluate with mu = " << mu << " if parametric() == false!");
  expressionFunction_->evaluate(x, ret);
}

template< class D, int d, class R, int r >
bool NonparametricExpression< D, d, R, r >::affinely_decomposable() const
{
  return true;
}

template< class D, int d, class R, int r >
bool NonparametricExpression< D, d, R, r >::has_affine_part() const
{
  return true;
}

template< class D, int d, class R, int r >
std::shared_ptr< const typename NonparametricExpression< D, d, R, r >::NonparametricType >
NonparametricExpression< D, d, R, r >::affine_part() const
{
  return expressionFunction_;
}

template< class D, int d, class R, int r >
unsigned int NonparametricExpression< D, d, R, r >::num_components() const
{
  return 0;
}

template class NonparametricExpression< double, 1, double, 1 >;
template class NonparametricExpression< double, 1, double, 2 >;
template class NonparametricExpression< double, 1, double, 3 >;
template class NonparametricExpression< double, 2, double, 1 >;
template class NonparametricExpression< double, 2, double, 2 >;
template class NonparametricExpression< double, 2, double, 3 >;
template class NonparametricExpression< double, 3, double, 1 >;
template class NonparametricExpression< double, 3, double, 2 >;
template class NonparametricExpression< double, 3, double, 3 >;


// =======================================
// ===== AffinelyDecomposableDefault =====
// =======================================
template< class D, int d, class R, int rR, int rC >
std::string AffinelyDecomposableDefault< D, d, R, rR, rC >::static_id()
{
  return ParametricFunctionInterface< D, d, R, rR, rC >::static_id() + ".affinelydecomposabledefault";
}

template< class D, int d, class R, int rR, int rC >
AffinelyDecomposableDefault< D, d, R, rR, rC >::AffinelyDecomposableDefault(const std::string nm,
                                                                            const int ord)
  : BaseType()
  , name_(nm)
  , order_(ord)
  , num_components_(0)
  , hasAffinePart_(false)
{}

template< class D, int d, class R, int rR, int rC >
AffinelyDecomposableDefault< D, d, R, rR, rC >::AffinelyDecomposableDefault(const NonparametricType* aff_ptr,
                                                                            const std::string nm,
                                                                            const int ord)
  : BaseType()
  , name_(nm)
  , order_(ord)
  , num_components_(0)
  , hasAffinePart_(true)
  , affinePart_(std::shared_ptr< const NonparametricType >(aff_ptr))
{
  if (ord < 0 || affinePart_->order() < 0)
    order_ = -1;
  else
    order_ = std::max(order_, affinePart_->order());
}

template< class D, int d, class R, int rR, int rC >
AffinelyDecomposableDefault< D, d, R, rR, rC >::AffinelyDecomposableDefault(const std::shared_ptr< const NonparametricType > aff_ptr,
                                                                            const std::string nm,
                                                                            const int ord)
  : BaseType()
  , name_(nm)
  , order_(ord)
  , num_components_(0)
  , hasAffinePart_(true)
  , affinePart_(aff_ptr)
{  if (ord < 0 || affinePart_->order() < 0)
    order_ = -1;
  else
    order_ = std::max(order_, affinePart_->order());
}

template< class D, int d, class R, int rR, int rC >
AffinelyDecomposableDefault< D, d, R, rR, rC >::AffinelyDecomposableDefault(const NonparametricType* comp_ptr,
                                                                            const ParameterFunctional* coeff_ptr,
                                                                            const std::string nm,
                                                                            const int ord)
  : BaseType()
  , name_(nm)
  , order_(ord)
  , num_components_(1)
  , hasAffinePart_(false)
{
  components_.emplace_back(comp_ptr);
  coefficients_.emplace_back(coeff_ptr);
  Parametric::inherit_parameter_type(coeff_ptr->parameter_type(), "coefficient_0");
  if (ord < 0 || comp_ptr->order() < 0)
    order_ = -1;
  else
    order_ = std::max(order_, comp_ptr->order());
}

template< class D, int d, class R, int rR, int rC >
AffinelyDecomposableDefault< D, d, R, rR, rC >::AffinelyDecomposableDefault(const NonparametricType* comp_ptr,
                                                                            const std::shared_ptr< const ParameterFunctional > coeff_ptr,
                                                                            const std::string nm,
                                                                            const int ord)
  : BaseType()
  , name_(nm)
  , order_(ord)
  , num_components_(1)
  , hasAffinePart_(false)
{
  components_.emplace_back(comp_ptr);
  coefficients_.push_back(coeff_ptr);
  Parametric::inherit_parameter_type(coeff_ptr->parameter_type(), "coefficient_0");
  if (ord < 0 || comp_ptr->order() < 0)
    order_ = -1;
  else
    order_ = std::max(order_, comp_ptr->order());
}

template< class D, int d, class R, int rR, int rC >
AffinelyDecomposableDefault< D, d, R, rR, rC >::AffinelyDecomposableDefault(const std::shared_ptr< const NonparametricType > comp_ptr,
                                                                            const ParameterFunctional* coeff_ptr,
                                                                            const std::string nm,
                                                                            const int ord)
  : BaseType()
  , name_(nm)
  , order_(ord)
  , num_components_(1)
  , hasAffinePart_(false)
{
  components_.push_back(comp_ptr);
  coefficients_.emplace_back(coeff_ptr);
  Parametric::inherit_parameter_type(coeff_ptr->parameter_type(), "coefficient_0");
  if (ord < 0 || comp_ptr->order() < 0)
    order_ = -1;
  else
    order_ = std::max(order_, comp_ptr->order());
}

template< class D, int d, class R, int rR, int rC >
AffinelyDecomposableDefault< D, d, R, rR, rC >::AffinelyDecomposableDefault(const std::shared_ptr< const NonparametricType > comp_ptr,
                                                                            const std::shared_ptr< const ParameterFunctional > coeff_ptr,
                                                                            const std::string nm,
                                                                            const int ord)
  : BaseType()
  , name_(nm)
  , order_(ord)
  , num_components_(1)
  , hasAffinePart_(false)
{
  components_.push_back(comp_ptr);
  coefficients_.push_back(coeff_ptr);
  Parametric::inherit_parameter_type(coeff_ptr->parameter_type(), "coefficient_0");
  if (ord < 0 || comp_ptr->order() < 0)
    order_ = -1;
  else
    order_ = std::max(order_, comp_ptr->order());
}

template< class D, int d, class R, int rR, int rC >
void AffinelyDecomposableDefault< D, d, R, rR, rC >::register_affine_part(const std::shared_ptr< const NonparametricType > aff_ptr)
  throw (Exception::this_does_not_make_any_sense)
{
  if (hasAffinePart_)
    DUNE_PYMOR_THROW(Exception::this_does_not_make_any_sense,
                     "do not call register_affine_part() if has_affine_part() == true!");
  if (num_components_ == 0) {
    order_ = aff_ptr->order();
  } else {
    if (order_ < 0 || aff_ptr->order() < 0)
      order_ = -1;
    else
      order_ = std::max(order_, aff_ptr->order());
  }
  affinePart_ = aff_ptr;
  hasAffinePart_ = true;
} // ... register_component(...)

template< class D, int d, class R, int rR, int rC >
void AffinelyDecomposableDefault< D, d, R, rR, rC >::register_affine_part(const NonparametricType* aff_ptr)
  throw (Exception::this_does_not_make_any_sense)
{
  register_affine_part(std::shared_ptr< const NonparametricType >(aff_ptr));
}

template< class D, int d, class R, int rR, int rC >
void AffinelyDecomposableDefault< D, d, R, rR, rC >::register_component(const NonparametricType* comp_ptr,
                                                                        const ParameterFunctional* coeff_ptr)
{
  register_component(std::shared_ptr< const NonparametricType >(comp_ptr),
                     std::shared_ptr< const ParameterFunctional >(coeff_ptr));
}

template< class D, int d, class R, int rR, int rC >
void AffinelyDecomposableDefault< D, d, R, rR, rC >::register_component(const std::shared_ptr< const NonparametricType > comp_ptr,
                                                                        const ParameterFunctional* coeff_ptr)
{
  register_component(comp_ptr,
                     std::shared_ptr< const ParameterFunctional >(coeff_ptr));
}

template< class D, int d, class R, int rR, int rC >
void AffinelyDecomposableDefault< D, d, R, rR, rC >::register_component(const NonparametricType* comp_ptr,
                                                                        const std::shared_ptr< const ParameterFunctional > coeff_ptr)
{
  register_component(std::shared_ptr< const NonparametricType >(comp_ptr),
                     coeff_ptr);
}

template< class D, int d, class R, int rR, int rC >
void AffinelyDecomposableDefault< D, d, R, rR, rC >::register_component(const std::shared_ptr< const NonparametricType > comp_ptr,
                                                                        const std::shared_ptr< const ParameterFunctional > coeff_ptr)
{
  if (num_components_ == 0 && !hasAffinePart_) {
    order_ = comp_ptr->order();
  } else {
    if (order_ < 0 || comp_ptr->order() < 0)
      order_ = -1;
    else
      order_ = std::max(order_, comp_ptr->order());
  }
  Parametric::inherit_parameter_type(coeff_ptr->parameter_type(), "coefficient_" + Dune::Stuff::Common::toString(num_components_));
  components_.push_back(comp_ptr);
  coefficients_.push_back(coeff_ptr);
  ++num_components_;
} // ... register_component(...)

template< class D, int d, class R, int rR, int rC >
int AffinelyDecomposableDefault< D, d, R, rR, rC >::order() const
{
  return order_;
}

template< class D, int d, class R, int rR, int rC >
std::string AffinelyDecomposableDefault< D, d, R, rR, rC >::name() const
{
  return name_;
}

template< class D, int d, class R, int rR, int rC >
void AffinelyDecomposableDefault< D, d, R, rR, rC >::evaluate(const DomainType& x, RangeType& ret, const Parameter mu) const
  throw (Exception::wrong_parameter_type, Exception::requirements_not_met)
{
  if (mu.type() != Parametric::parameter_type())
    DUNE_PYMOR_THROW(Exception::wrong_parameter_type,
                     "the type of mu (" << mu.type() << ") does not match the parameter_type of this ("
                     << Parametric::parameter_type() << ")!");
  if (!hasAffinePart_ && num_components_ == 0)
    DUNE_PYMOR_THROW(Exception::requirements_not_met,
                     "do not call evaluate() if has_affine_part() == false and num_components() == 0");
  RangeType tmpComp(0);
  double tmpCoeff = 0.0;
  if (hasAffinePart_)
    affinePart_->evaluate(x, ret);
  else
    ret *= 0.0;
  for (unsigned int ii = 0; ii < num_components_; ++ii) {
    components_[ii]->evaluate(x, tmpComp);
    const Parameter muCoefficient = Parametric::map_parameter(mu, "coefficient_" + Dune::Stuff::Common::toString(ii));
    coefficients_[ii]->evaluate(muCoefficient, tmpCoeff);
    tmpComp *= tmpCoeff;
    ret += tmpComp;
  }
} // ... evaluate(...)

template< class D, int d, class R, int rR, int rC >
bool AffinelyDecomposableDefault< D, d, R, rR, rC >::affinely_decomposable() const
{
  return true;
}

template< class D, int d, class R, int rR, int rC >
bool AffinelyDecomposableDefault< D, d, R, rR, rC >::has_affine_part() const
{
  return hasAffinePart_;
}

template< class D, int d, class R, int rR, int rC >
std::shared_ptr< const typename AffinelyDecomposableDefault< D, d, R, rR, rC >::NonparametricType >
AffinelyDecomposableDefault< D, d, R, rR, rC >::affine_part() const
  throw(Exception::requirements_not_met)
{
  if (!hasAffinePart_)
    DUNE_PYMOR_THROW(Exception::requirements_not_met,
                     "do not call affine_part() if has_affine_part() == false!");
  return affinePart_;
}

template< class D, int d, class R, int rR, int rC >
unsigned int AffinelyDecomposableDefault< D, d, R, rR, rC >::num_components() const
{
  return num_components_;
}

template< class D, int d, class R, int rR, int rC >
std::shared_ptr< const typename AffinelyDecomposableDefault< D, d, R, rR, rC >::NonparametricType >
AffinelyDecomposableDefault< D, d, R, rR, rC >::component(const int qq) const
  throw (Exception::requirements_not_met, Exception::index_out_of_range)
{
  if (num_components_ == 0)
    DUNE_PYMOR_THROW(Exception::requirements_not_met,
                     "do not call component(ii) if num_components() == 0!");
  if (qq < 0 || qq >= int(num_components_))
    DUNE_PYMOR_THROW(Exception::index_out_of_range,
                     "the condition 0 < " << qq << " < num_components() = " << num_components_
                     << " is not satisfied!");
  return components_[qq];
}

template< class D, int d, class R, int rR, int rC >
std::shared_ptr< const ParameterFunctional > AffinelyDecomposableDefault< D, d, R, rR, rC >::coefficient(const int qq) const
  throw (Exception::requirements_not_met, Exception::index_out_of_range)
{
  if (num_components_ == 0)
    DUNE_PYMOR_THROW(Exception::requirements_not_met,
                     "do not call coefficient(ii) if size() == 0!");
  if (qq < 0 || qq >= int(num_components_))
    DUNE_PYMOR_THROW(Exception::index_out_of_range,
                     "the condition 0 < " << qq << " < num_components() = " << num_components_
                     << " is not satisfied!");
  return coefficients_[qq];
}

template< class D, int d, class R, int rR, int rC >
Dune::ParameterTree AffinelyDecomposableDefault< D, d, R, rR, rC >::defaultSettings(const std::string subName)
{
  DSC::ExtendedParameterTree settings;
  typedef Stuff::Functions< D, d, R, rR, rC > NonparametricFunctions;
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
}

template< class D, int d, class R, int rR, int rC >
typename AffinelyDecomposableDefault< D, d, R, rR, rC >::ThisType*
AffinelyDecomposableDefault< D, d, R, rR, rC >::create(const DSC::ExtendedParameterTree settings)
{
  typedef Stuff::Functions< D, d, R, rR, rC > NonparametricFunctions;
  ThisType* ret;
  const std::string name = settings.get< std::string >("name", static_id());
  if (settings.hasKey("order"))
    ret = new ThisType(name, settings.get< int >("order"));
  else
    ret = new ThisType(name);
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
}

template class AffinelyDecomposableDefault< double, 1, double, 1, 1 >;
//template class AffinelyDecomposableDefault< double, 1, double, 1, 2 >;
//template class AffinelyDecomposableDefault< double, 1, double, 1, 3 >;
template class AffinelyDecomposableDefault< double, 1, double, 2, 1 >;
//template class AffinelyDecomposableDefault< double, 1, double, 2, 2 >;
//template class AffinelyDecomposableDefault< double, 1, double, 2, 3 >;
template class AffinelyDecomposableDefault< double, 1, double, 3, 1 >;
//template class AffinelyDecomposableDefault< double, 1, double, 3, 2 >;
//template class AffinelyDecomposableDefault< double, 1, double, 3, 3 >;
template class AffinelyDecomposableDefault< double, 2, double, 1, 1 >;
//template class AffinelyDecomposableDefault< double, 2, double, 1, 2 >;
//template class AffinelyDecomposableDefault< double, 2, double, 1, 3 >;
template class AffinelyDecomposableDefault< double, 2, double, 2, 1 >;
//template class AffinelyDecomposableDefault< double, 2, double, 2, 2 >;
//template class AffinelyDecomposableDefault< double, 2, double, 2, 3 >;
template class AffinelyDecomposableDefault< double, 2, double, 3, 1 >;
//template class AffinelyDecomposableDefault< double, 2, double, 3, 2 >;
//template class AffinelyDecomposableDefault< double, 2, double, 3, 3 >;
template class AffinelyDecomposableDefault< double, 3, double, 1, 1 >;
//template class AffinelyDecomposableDefault< double, 3, double, 1, 2 >;
//template class AffinelyDecomposableDefault< double, 3, double, 1, 3 >;
template class AffinelyDecomposableDefault< double, 3, double, 2, 1 >;
//template class AffinelyDecomposableDefault< double, 3, double, 2, 2 >;
//template class AffinelyDecomposableDefault< double, 3, double, 2, 3 >;
template class AffinelyDecomposableDefault< double, 3, double, 3, 1 >;
//template class AffinelyDecomposableDefault< double, 3, double, 3, 2 >;
//template class AffinelyDecomposableDefault< double, 3, double, 3, 3 >;


} // namespace Function
} // namespace Pymor
} // namespace Dune
