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

#include <dune/pymor/la/container/dunedynamicvector.hh>
#if HAVE_EIGEN
  #include <dune/pymor/la/container/eigenvector.hh>
#endif
#include "default.hh"

namespace Dune {
namespace Pymor {
namespace Functionals {


template< class LinearFunctionalType >
LinearAffinelyDecomposedDefault< LinearFunctionalType >
  ::LinearAffinelyDecomposedDefault()
  : AffinelyDecomposedFunctionalInterface()
  , size_(0)
  , hasAffinePart_(false)
  , dim_source_(0)
  , type_source_("")
{
  static_assert(Dune::IsBaseOf< LA::VectorInterface, LinearFunctionalType >::value,
                "given LinearFunctionalType is not derived from LA::VectorInterface!");
}

template< class LinearFunctionalType >
LinearAffinelyDecomposedDefault< LinearFunctionalType >
  ::LinearAffinelyDecomposedDefault(LinearFunctionalType* aff)
  throw (Exception::requirements_not_met)
  : AffinelyDecomposedFunctionalInterface()
  , size_(0)
  , hasAffinePart_(true)
  , dim_source_(aff->dim_source())
  , type_source_(aff->type_source())
  , affinePart_(aff)
{
  static_assert(Dune::IsBaseOf< LA::VectorInterface, LinearFunctionalType >::value,
                "given LinearFunctionalType is not derived from LA::VectorInterface!");
  if (affinePart_->parametric())
    DUNE_PYMOR_THROW(Exception::requirements_not_met,
                     "the affinePart must not be parametric!");
}

template< class LinearFunctionalType >
LinearAffinelyDecomposedDefault< LinearFunctionalType >::~LinearAffinelyDecomposedDefault()
{
  if (hasAffinePart_)
    delete affinePart_;
  for (auto& element : components_)
    delete element;
  for (auto& element : coefficients_)
    delete element;
}

template< class LinearFunctionalType >
void LinearAffinelyDecomposedDefault< LinearFunctionalType >::register_component(LinearFunctionalType* aff)
  throw (Exception::this_does_not_make_any_sense,
         Exception::sizes_do_not_match,
         Exception::types_are_not_compatible)
{
  if (hasAffinePart_)
    DUNE_PYMOR_THROW(Exception::this_does_not_make_any_sense,
                     "do not call register_component(affinePart) if hasAffinePart() == true!");
  if (aff->parametric())
    DUNE_PYMOR_THROW(Exception::this_does_not_make_any_sense,
                     "the affinePart must not be parametric!");
  if (size_ == 0) {
    dim_source_ = aff->dim_source();
    type_source_ = aff->type_source();
  } else {
    if (aff->dim_source() != dim_source_)
      DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                       "the dim_source of aff (" << aff->dim_source() << ") does not match the dim_source of this ("
                       << dim_source_ << ")!");
    if (aff->type_source() != type_source_)
      DUNE_PYMOR_THROW(Exception::types_are_not_compatible,
                       "the type_source of aff (" << aff->type_source() << ") does not match the type_source of this ("
                       << type_source_ << ")!");
  }
  affinePart_ = aff;
  hasAffinePart_ = true;
} // ... register_component(...)

template< class LinearFunctionalType >
void LinearAffinelyDecomposedDefault< LinearFunctionalType >::register_component(LinearFunctionalType* comp,
                                                                                 const ParameterFunctional* coeff)
  throw (Exception::this_does_not_make_any_sense,
         Exception::sizes_do_not_match,
         Exception::types_are_not_compatible,
         Exception::wrong_parameter_type)
{
  if (comp->parametric())
    DUNE_PYMOR_THROW(Exception::this_does_not_make_any_sense,
                     "a component must not be parametric!");
  if (size_ == 0 && !hasAffinePart_) {
    dim_source_ = comp->dim_source();
    type_source_ = comp->type_source();
  } else {
    if (comp->dim_source() != dim_source_)
      DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                       "the dim_source of comp (" << comp->dim_source() << ") does not match the dim_source of this ("
                       << dim_source_ << ")!");
    if (comp->type_source() != type_source_)
      DUNE_PYMOR_THROW(Exception::types_are_not_compatible,
                       "the type_source of comp (" << comp->type_source() << ") does not match the type_source of this ("
                       << type_source_ << ")!");
  }
  inherit_parameter_type(coeff->parameter_type(), "coefficient_" + Dune::Stuff::Common::toString(size_));
  components_.push_back(comp);
  coefficients_.push_back(coeff);
  ++size_;
} // ... register_component(..., ...)

template< class LinearFunctionalType >
unsigned int LinearAffinelyDecomposedDefault< LinearFunctionalType >::num_components() const
{
  return size_;
}

template< class LinearFunctionalType >
LinearFunctionalType* LinearAffinelyDecomposedDefault< LinearFunctionalType >::component(const int ii)
  throw (Exception::requirements_not_met, Exception::index_out_of_range)
{
  if (size_ == 0)
    DUNE_PYMOR_THROW(Exception::requirements_not_met,
                     "do not call component(ii) if num_components() == 0!");
  if (ii < 0 || ii >= int(size_))
    DUNE_PYMOR_THROW(Exception::index_out_of_range,
                     "the condition 0 < ii < num_components() is not fulfilled for ii = " << ii
                     << "and num_components() = " << size_ << "!");
  return components_[ii];
}

template< class LinearFunctionalType >
const LinearFunctionalType* LinearAffinelyDecomposedDefault< LinearFunctionalType >::component(const int ii) const
  throw (Exception::requirements_not_met, Exception::index_out_of_range)
{
  if (size_ == 0)
    DUNE_PYMOR_THROW(Exception::requirements_not_met,
                     "do not call component(ii) if num_components() == 0!");
  if (ii < 0 || ii >= int(size_))
    DUNE_PYMOR_THROW(Exception::index_out_of_range,
                     "the condition 0 < ii < size() is not fulfilled for ii = " << ii << "and num_components() = "
                     << size_ << "!");
  return components_[ii];
}

template< class LinearFunctionalType >
const ParameterFunctional* LinearAffinelyDecomposedDefault< LinearFunctionalType >::coefficient(const int ii) const
  throw (Exception::requirements_not_met, Exception::index_out_of_range)
{
  if (size_ == 0)
    DUNE_PYMOR_THROW(Exception::requirements_not_met,
                     "do not call coefficient(ii) if num_components() == 0!");
  if (ii < 0 || ii >= int(size_))
    DUNE_PYMOR_THROW(Exception::index_out_of_range,
                     "the condition 0 < ii < size() is not fulfilled for ii = " << ii << "and num_components() = "
                     << size_ << "!");
  return coefficients_[ii];

}

template< class LinearFunctionalType >
bool LinearAffinelyDecomposedDefault< LinearFunctionalType >::hasAffinePart() const
{
  return hasAffinePart_;
}

template< class LinearFunctionalType >
LinearFunctionalType* LinearAffinelyDecomposedDefault< LinearFunctionalType >::affinePart()
  throw (Exception::requirements_not_met)
{
  if (!hasAffinePart())
    DUNE_PYMOR_THROW(Exception::requirements_not_met,
                     "do not call affinePart() if hasAffinePart() == false!");
  return affinePart_;
}

template< class LinearFunctionalType >
const LinearFunctionalType* LinearAffinelyDecomposedDefault< LinearFunctionalType >::affinePart() const
  throw (Exception::requirements_not_met)
{
  if (!hasAffinePart())
    DUNE_PYMOR_THROW(Exception::requirements_not_met,
                     "do not call affinePart() if hasAffinePart() == false!");
  return affinePart_;
}

template< class LinearFunctionalType >
bool LinearAffinelyDecomposedDefault< LinearFunctionalType >::linear() const
{
  return true;
}

template< class LinearFunctionalType >
unsigned int LinearAffinelyDecomposedDefault< LinearFunctionalType >::dim_source() const
{
  return dim_source_;
}

template< class LinearFunctionalType >
std::string LinearAffinelyDecomposedDefault< LinearFunctionalType >::type_source() const
{
  return type_source_;
}

template< class LinearFunctionalType >
double LinearAffinelyDecomposedDefault< LinearFunctionalType >::apply(const LA::VectorInterface* source,
                                                                      const Parameter /*mu*/) const
  throw (Exception::types_are_not_compatible,
         Exception::you_have_to_implement_this,
         Exception::sizes_do_not_match,
         Exception::wrong_parameter_type,
         Exception::requirements_not_met,
         Exception::linear_solver_failed,
         Exception::this_does_not_make_any_sense)
{
  DUNE_PYMOR_THROW(Exception::types_are_not_compatible,
                   "source (" << source->type() << ") is not a compatible type_source (" << type_source() << ")!");
}

template< class LinearFunctionalType >
double LinearAffinelyDecomposedDefault< LinearFunctionalType >::apply(const SourceType* source,
                                                                      const Parameter mu) const
  throw (Exception::types_are_not_compatible,
         Exception::you_have_to_implement_this,
         Exception::sizes_do_not_match,
         Exception::wrong_parameter_type,
         Exception::requirements_not_met,
         Exception::linear_solver_failed,
         Exception::this_does_not_make_any_sense)
{
  if (source->dim() != dim_source())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "the dim of source (" << source->dim() << ") does not match the dim of this ("
                     << dim_source() << ")!");
  if (mu.type() != Parametric::parameter_type())
    DUNE_PYMOR_THROW(Exception::wrong_parameter_type,
                     "the type of mu (" << mu.type() << ") does not match the parameter_type of this ("
                     << Parametric::parameter_type() << ")!");
  if (size_ == 0 && !hasAffinePart_)
    DUNE_PYMOR_THROW(Exception::requirements_not_met,
               "do not call apply() if num_components() == 0 and hasAffinePart() == false!");
  if (components_.size() != size_) DUNE_PYMOR_THROW(Exception::this_does_not_make_any_sense, "");
  if (coefficients_.size() != size_) DUNE_PYMOR_THROW(Exception::this_does_not_make_any_sense, "");
  if (hasAffinePart_) {
    double ret = affinePart_->apply(source);
    if (size_ > 0) {
      for (size_t ii = 0; ii < size_; ++ii) {
        const Parameter muCoefficient = map_parameter(mu, "coefficient_" + Dune::Stuff::Common::toString(ii));
        ret += components_[ii]->apply(source) * coefficients_[ii]->evaluate(muCoefficient);
      }
    }
    return ret;
  } else {
    double ret = 0.0;
    for (size_t ii = 0; ii < size_; ++ii) {
      const Parameter muCoefficient = map_parameter(mu, "coefficient_" + Dune::Stuff::Common::toString(ii));
      ret += components_[0]->apply(source) * coefficients_[0]->evaluate(muCoefficient);
    }
    return ret;
  }
} // void apply(...)

template< class LinearFunctionalType >
LinearFunctionalType* LinearAffinelyDecomposedDefault< LinearFunctionalType >
  ::freeze_parameter(const Parameter mu) const throw (Exception::this_is_not_parametric,
                                                      Exception::you_have_to_implement_this,
                                                      Exception::this_does_not_make_any_sense)
{
  if (mu.type() != parameter_type())
    DUNE_PYMOR_THROW(Exception::wrong_parameter_type,
                     "the type of mu (" << mu.type() << ") does not match the parameter_type of this ("
                     << parameter_type() << ")!");
  if (size_ == 0 && !hasAffinePart_)
    DUNE_PYMOR_THROW(Exception::requirements_not_met,
               "do not call freeze_parameter() if num_components() == 0 and hasAffinePart() == false!");
  if (components_.size() != size_) DUNE_PYMOR_THROW(Exception::this_does_not_make_any_sense, "");
  if (coefficients_.size() != size_) DUNE_PYMOR_THROW(Exception::this_does_not_make_any_sense, "");
  if (hasAffinePart_) {
    LinearFunctionalType* ret = affinePart_->copy();
    if (size_ > 0)
      for (size_t ii = 0; ii < size_; ++ii) {
        const Parameter muCoefficient = map_parameter(mu, "coefficient_" + Dune::Stuff::Common::toString(ii));
        ret->axpy(coefficients_[ii]->evaluate(muCoefficient), components_[ii]);
      }
    return ret;
  } else {
    LinearFunctionalType* ret = components_[0]->copy();
    const Parameter muCoefficient0 = map_parameter(mu, "coefficient_0");
    ret->scal(coefficients_[0]->evaluate(muCoefficient0));
    if (size_ > 1)
      for (size_t ii = 1; ii < size_; ++ii) {
        const Parameter muCoefficient = map_parameter(mu, "coefficient_" + Dune::Stuff::Common::toString(ii));
        ret->axpy(coefficients_[ii]->evaluate(muCoefficient), components_[ii]);
      }
    return ret;
  }
}

template class LinearAffinelyDecomposedDefault< LA::DuneDynamicVector >;
#if HAVE_EIGEN
template class LinearAffinelyDecomposedDefault< LA::EigenDenseVector >;
#endif // HAVE_EIGEN


} // namespace Functionals
} // namespace Pymor
} // namespace Dune
