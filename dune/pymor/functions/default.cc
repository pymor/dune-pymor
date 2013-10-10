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
template< class E, class D, int d, class R, int r, int rC >
NonparametricDefault< E, D, d, R, r, rC >::NonparametricDefault(const NonparametricType* nonparametric_ptr)
  : BaseType()
  , nonparametric_(std::shared_ptr< const NonparametricType >(nonparametric_ptr))
{}

template< class E, class D, int d, class R, int r, int rC >
NonparametricDefault< E, D, d, R, r, rC >::NonparametricDefault(const std::shared_ptr< const NonparametricType > nonparametric_ptr)
  : BaseType()
  , nonparametric_(nonparametric_ptr)
{}

template< class E, class D, int d, class R, int r, int rC >
std::string NonparametricDefault< E, D, d, R, r, rC >::name() const
{
  return nonparametric_->name();
}

template< class E, class D, int d, class R, int r, int rC >
bool NonparametricDefault< E, D, d, R, r, rC >::affinely_decomposable() const
{
  return true;
}

template< class E, class D, int d, class R, int r, int rC >
bool NonparametricDefault< E, D, d, R, r, rC >::has_affine_part() const
{
  return true;
}

template< class E, class D, int d, class R, int r, int rC >
std::shared_ptr< const typename NonparametricDefault< E, D, d, R, r, rC >::NonparametricType >
NonparametricDefault< E, D, d, R, r, rC >::affine_part() const
{
  return nonparametric_;
}

template< class E, class D, int d, class R, int r, int rC >
unsigned int NonparametricDefault< E, D, d, R, r, rC >::num_components() const
{
  return 0;
}


//// ===================================
//// ===== NonparametricExpression =====
//// ===================================
//template< class D, int d, class R, int r >
//NonparametricExpression< D, d, R, r >::NonparametricExpression(const std::string var,
//                                                               const std::string expr,
//                                                               const int oo,
//                                                               const std::string nm)
//  : BaseType()
//  , expressionFunction_(std::make_shared< const ExpressionFunctionType >(var, expr, oo, nm))
//{}

//template< class D, int d, class R, int r >
//NonparametricExpression< D, d, R, r >::NonparametricExpression(const std::string var,
//                                                               const std::vector< std::string > exprs,
//                                                               const int oo,
//                                                               const std::string nm)
//  : BaseType()
//  , expressionFunction_(std::make_shared< const ExpressionFunctionType >(var, exprs, oo, nm))
//{}

//template< class D, int d, class R, int r >
//std::string NonparametricExpression< D, d, R, r >::name() const
//{
//  return expressionFunction_->name();
//}

//template< class D, int d, class R, int r >
//int NonparametricExpression< D, d, R, r >::order() const
//{
//  return expressionFunction_->order();
//}

//template< class D, int d, class R, int r >
//void NonparametricExpression< D, d, R, r >::evaluate(const DomainType& x, RangeType& ret, const Parameter mu) const
//  throw (Exception::wrong_parameter_type)
//{
//  if (!mu.type().empty())
//    DUNE_PYMOR_THROW(Exception::wrong_parameter_type,
//                     "do not call evaluate with mu = " << mu << " if parametric() == false!");
//  expressionFunction_->evaluate(x, ret);
//}

//template< class D, int d, class R, int r >
//bool NonparametricExpression< D, d, R, r >::affinely_decomposable() const
//{
//  return true;
//}

//template< class D, int d, class R, int r >
//bool NonparametricExpression< D, d, R, r >::has_affine_part() const
//{
//  return true;
//}

//template< class D, int d, class R, int r >
//std::shared_ptr< const typename NonparametricExpression< D, d, R, r >::NonparametricType >
//NonparametricExpression< D, d, R, r >::affine_part() const
//{
//  return expressionFunction_;
//}

//template< class D, int d, class R, int r >
//unsigned int NonparametricExpression< D, d, R, r >::num_components() const
//{
//  return 0;
//}

//template class NonparametricExpression< double, 1, double, 1 >;
//template class NonparametricExpression< double, 1, double, 2 >;
//template class NonparametricExpression< double, 1, double, 3 >;
//template class NonparametricExpression< double, 2, double, 1 >;
//template class NonparametricExpression< double, 2, double, 2 >;
//template class NonparametricExpression< double, 2, double, 3 >;
//template class NonparametricExpression< double, 3, double, 1 >;
//template class NonparametricExpression< double, 3, double, 2 >;
//template class NonparametricExpression< double, 3, double, 3 >;


// =======================================
// ===== AffinelyDecomposableDefault =====
// =======================================
template< class E, class D, int d, class R, int r, int rC >
std::string AffinelyDecomposableDefault< E, D, d, R, r, rC >::static_id()
{
  return ParametricFunctionInterface< E, D, d, R, r, rC >::static_id() + ".affinelydecomposabledefault";
}

template< class E, class D, int d, class R, int r, int rC >
AffinelyDecomposableDefault< E, D, d, R, r, rC >::AffinelyDecomposableDefault(const std::string nm)
  : BaseType()
  , name_(nm)
  , num_components_(0)
  , hasAffinePart_(false)
{}

template< class E, class D, int d, class R, int r, int rC >
AffinelyDecomposableDefault< E, D, d, R, r, rC >::AffinelyDecomposableDefault(const NonparametricType* aff_ptr,
                                                                            const std::string nm)
  : BaseType()
  , name_(nm)
  , num_components_(0)
  , hasAffinePart_(true)
  , affinePart_(std::shared_ptr< const NonparametricType >(aff_ptr))
{}

template< class E, class D, int d, class R, int r, int rC >
AffinelyDecomposableDefault< E, D, d, R, r, rC >::AffinelyDecomposableDefault(const std::shared_ptr< const NonparametricType > aff_ptr,
                                                                            const std::string nm)
  : BaseType()
  , name_(nm)
  , num_components_(0)
  , hasAffinePart_(true)
  , affinePart_(aff_ptr)
{}

template< class E, class D, int d, class R, int r, int rC >
AffinelyDecomposableDefault< E, D, d, R, r, rC >::AffinelyDecomposableDefault(const NonparametricType* comp_ptr,
                                                                            const ParameterFunctional* coeff_ptr,
                                                                            const std::string nm)
  : BaseType()
  , name_(nm)
  , num_components_(1)
  , hasAffinePart_(false)
{
  components_.emplace_back(comp_ptr);
  coefficients_.emplace_back(coeff_ptr);
  Parametric::inherit_parameter_type(coeff_ptr->parameter_type(), "coefficient_0");
}

template< class E, class D, int d, class R, int r, int rC >
AffinelyDecomposableDefault< E, D, d, R, r, rC >::AffinelyDecomposableDefault(const NonparametricType* comp_ptr,
                                                                            const std::shared_ptr< const ParameterFunctional > coeff_ptr,
                                                                            const std::string nm)
  : BaseType()
  , name_(nm)
  , num_components_(1)
  , hasAffinePart_(false)
{
  components_.emplace_back(comp_ptr);
  coefficients_.push_back(coeff_ptr);
  Parametric::inherit_parameter_type(coeff_ptr->parameter_type(), "coefficient_0");
}

template< class E, class D, int d, class R, int r, int rC >
AffinelyDecomposableDefault< E, D, d, R, r, rC >::AffinelyDecomposableDefault(const std::shared_ptr< const NonparametricType > comp_ptr,
                                                                            const ParameterFunctional* coeff_ptr,
                                                                            const std::string nm)
  : BaseType()
  , name_(nm)
  , num_components_(1)
  , hasAffinePart_(false)
{
  components_.push_back(comp_ptr);
  coefficients_.emplace_back(coeff_ptr);
  Parametric::inherit_parameter_type(coeff_ptr->parameter_type(), "coefficient_0");
}

template< class E, class D, int d, class R, int r, int rC >
AffinelyDecomposableDefault< E, D, d, R, r, rC >::AffinelyDecomposableDefault(const std::shared_ptr< const NonparametricType > comp_ptr,
                                                                            const std::shared_ptr< const ParameterFunctional > coeff_ptr,
                                                                            const std::string nm)
  : BaseType()
  , name_(nm)
  , num_components_(1)
  , hasAffinePart_(false)
{
  components_.push_back(comp_ptr);
  coefficients_.push_back(coeff_ptr);
  Parametric::inherit_parameter_type(coeff_ptr->parameter_type(), "coefficient_0");
}

template< class E, class D, int d, class R, int r, int rC >
void AffinelyDecomposableDefault< E, D, d, R, r, rC >::register_affine_part(const std::shared_ptr< const NonparametricType > aff_ptr)
{
  if (hasAffinePart_)
    DUNE_PYMOR_THROW(Exception::this_does_not_make_any_sense,
                     "do not call register_affine_part() if has_affine_part() == true!");
  affinePart_ = aff_ptr;
  hasAffinePart_ = true;
} // ... register_component(...)

template< class E, class D, int d, class R, int r, int rC >
void AffinelyDecomposableDefault< E, D, d, R, r, rC >::register_affine_part(const NonparametricType* aff_ptr)
{
  register_affine_part(std::shared_ptr< const NonparametricType >(aff_ptr));
}

template< class E, class D, int d, class R, int r, int rC >
void AffinelyDecomposableDefault< E, D, d, R, r, rC >::register_component(const NonparametricType* comp_ptr,
                                                                        const ParameterFunctional* coeff_ptr)
{
  register_component(std::shared_ptr< const NonparametricType >(comp_ptr),
                     std::shared_ptr< const ParameterFunctional >(coeff_ptr));
}

template< class E, class D, int d, class R, int r, int rC >
void AffinelyDecomposableDefault< E, D, d, R, r, rC >::register_component(const std::shared_ptr< const NonparametricType > comp_ptr,
                                                                        const ParameterFunctional* coeff_ptr)
{
  register_component(comp_ptr,
                     std::shared_ptr< const ParameterFunctional >(coeff_ptr));
}

template< class E, class D, int d, class R, int r, int rC >
void AffinelyDecomposableDefault< E, D, d, R, r, rC >::register_component(const NonparametricType* comp_ptr,
                                                                        const std::shared_ptr< const ParameterFunctional > coeff_ptr)
{
  register_component(std::shared_ptr< const NonparametricType >(comp_ptr),
                     coeff_ptr);
}

template< class E, class D, int d, class R, int r, int rC >
void AffinelyDecomposableDefault< E, D, d, R, r, rC >::register_component(const std::shared_ptr< const NonparametricType > comp_ptr,
                                                                        const std::shared_ptr< const ParameterFunctional > coeff_ptr)
{
  Parametric::inherit_parameter_type(coeff_ptr->parameter_type(), "coefficient_" + Dune::Stuff::Common::toString(num_components_));
  components_.push_back(comp_ptr);
  coefficients_.push_back(coeff_ptr);
  ++num_components_;
} // ... register_component(...)

template< class E, class D, int d, class R, int r, int rC >
std::string AffinelyDecomposableDefault< E, D, d, R, r, rC >::name() const
{
  return name_;
}

template< class E, class D, int d, class R, int r, int rC >
bool AffinelyDecomposableDefault< E, D, d, R, r, rC >::affinely_decomposable() const
{
  return true;
}

template< class E, class D, int d, class R, int r, int rC >
bool AffinelyDecomposableDefault< E, D, d, R, r, rC >::has_affine_part() const
{
  return hasAffinePart_;
}

template< class E, class D, int d, class R, int r, int rC >
std::shared_ptr< const typename AffinelyDecomposableDefault< E, D, d, R, r, rC >::NonparametricType >
AffinelyDecomposableDefault< E, D, d, R, r, rC >::affine_part() const
{
  if (!hasAffinePart_)
    DUNE_PYMOR_THROW(Exception::requirements_not_met,
                     "do not call affine_part() if has_affine_part() == false!");
  return affinePart_;
}

template< class E, class D, int d, class R, int r, int rC >
unsigned int AffinelyDecomposableDefault< E, D, d, R, r, rC >::num_components() const
{
  return num_components_;
}

template< class E, class D, int d, class R, int r, int rC >
std::shared_ptr< const typename AffinelyDecomposableDefault< E, D, d, R, r, rC >::NonparametricType >
AffinelyDecomposableDefault< E, D, d, R, r, rC >::component(const int qq) const
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

template< class E, class D, int d, class R, int r, int rC >
std::shared_ptr< const ParameterFunctional > AffinelyDecomposableDefault< E, D, d, R, r, rC >::coefficient(const int qq) const
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

template< class E, class D, int d, class R, int r, int rC >
Dune::ParameterTree AffinelyDecomposableDefault< E, D, d, R, r, rC >::defaultSettings(const std::string subName)
{
  DSC::ExtendedParameterTree settings;
  typedef Stuff::Functions< E, D, d, R, r, rC > NonparametricFunctions;
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

template< class E, class D, int d, class R, int r, int rC >
typename AffinelyDecomposableDefault< E, D, d, R, r, rC >::ThisType*
AffinelyDecomposableDefault< E, D, d, R, r, rC >::create(const DSC::ExtendedParameterTree settings)
{
  typedef Stuff::Functions< E, D, d, R, r, rC > NonparametricFunctions;
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


} // namespace Function
} // namespace Pymor
} // namespace Dune
