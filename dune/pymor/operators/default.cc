// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifdef HAVE_CMAKE_CONFIG
  #include "cmake_config.h"
#elif defined (HAVE_CONFIG_H)
  #include "config.h"
#endif // HAVE_CMAKE_CONFIG

#include <dune/pymor/operators/dunedynamicmatrix.hh>
#if HAVE_EIGEN
  #include <dune/pymor/operators/eigen.hh>
#endif
#include "default.hh"

namespace Dune {
namespace Pymor {
namespace Operators {


template< class LinearOperatorType >
LinearAffinelyDecomposedDefault< LinearOperatorType >
  ::LinearAffinelyDecomposedDefault()
  : AffinelyDecomposedOperatorInterface()
  , size_(0)
  , hasAffinePart_(false)
  , dim_source_(0)
  , dim_range_(0)
  , type_source_("")
  , type_range_("")
{
  static_assert(Dune::IsBaseOf< LinearOperatorInterface, LinearOperatorType >::value,
                "given LinearOperatorType is not derived from LinearOperatorInterface!");
}

template< class LinearOperatorType >
LinearAffinelyDecomposedDefault< LinearOperatorType >
  ::LinearAffinelyDecomposedDefault(LinearOperatorType* aff)
  throw (Exception::requirements_not_met)
  : AffinelyDecomposedOperatorInterface()
  , size_(0)
  , hasAffinePart_(true)
  , dim_source_(aff->dim_source())
  , dim_range_(aff->dim_range())
  , type_source_(aff->type_source())
  , type_range_(aff->type_range())
  , affinePart_(aff)
{
  static_assert(Dune::IsBaseOf< LinearOperatorInterface, LinearOperatorType >::value,
                "given LinearOperatorType is not derived from LinearOperatorInterface!");
  if (affinePart_->parametric())
    DUNE_PYMOR_THROW(Exception::requirements_not_met,
                     "the affinePart must not be parametric!");
}

template< class LinearOperatorType >
LinearAffinelyDecomposedDefault< LinearOperatorType >::~LinearAffinelyDecomposedDefault()
{
  if (hasAffinePart_)
    delete affinePart_;
  for (auto& element : components_)
    delete element;
  for (auto& element : coefficients_)
    delete element;
}

template< class LinearOperatorType >
void LinearAffinelyDecomposedDefault< LinearOperatorType >::register_component(LinearOperatorType* aff)
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
    dim_range_ = aff->dim_range();
    type_source_ = aff->type_source();
    type_range_ = aff->type_range();
  } else {
    if (aff->dim_source() != dim_source_)
      DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                       "the dim_source of aff (" << aff->dim_source() << ") does not match the dim_source of this ("
                       << dim_source_ << ")!");
    if (aff->dim_range() != dim_range_)
      DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                       "the dim_range of aff (" << aff->dim_range() << ") does not match the dim_range of this ("
                       << dim_range_ << ")!");
    if (aff->type_source() != type_source_)
      DUNE_PYMOR_THROW(Exception::types_are_not_compatible,
                       "the type_source of aff (" << aff->type_source() << ") does not match the type_source of this ("
                       << type_source_ << ")!");
    if (aff->type_range() != type_range_)
      DUNE_PYMOR_THROW(Exception::types_are_not_compatible,
                       "the type_range of aff (" << aff->type_range() << ") does not match the type_range of this ("
                       << type_range_ << ")!");
  }
  affinePart_ = aff;
  hasAffinePart_ = true;
} // ... register_component(...)

template< class LinearOperatorType >
void LinearAffinelyDecomposedDefault< LinearOperatorType >::register_component(LinearOperatorType* comp,
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
    dim_range_ = comp->dim_range();
    type_source_ = comp->type_source();
    type_range_ = comp->type_range();
  } else {
  if (comp->dim_source() != dim_source_)
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "the dim_source of comp (" << comp->dim_source() << ") does not match the dim_source of this ("
                     << dim_source_ << ")!");
  if (comp->dim_range() != dim_range_)
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "the dim_range of comp (" << comp->dim_range() << ") does not match the dim_range of this ("
                     << dim_range_ << ")!");
  if (comp->type_source() != type_source_)
    DUNE_PYMOR_THROW(Exception::types_are_not_compatible,
                     "the type_source of comp (" << comp->type_source() << ") does not match the type_source of this ("
                     << type_source_ << ")!");
  if (comp->type_range() != type_range_)
    DUNE_PYMOR_THROW(Exception::types_are_not_compatible,
                     "the type_range of comp (" << comp->type_range() << ") does not match the type_range of this ("
                     << type_range_ << ")!");
  }
  inherit_parameter_type(coeff->parameter_type(), "coefficient_" + Dune::Stuff::Common::toString(size_));
  components_.push_back(comp);
  coefficients_.push_back(coeff);
  ++size_;
} // ... register_component(..., ...)

template< class LinearOperatorType >
unsigned int LinearAffinelyDecomposedDefault< LinearOperatorType >::num_components() const
{
  return size_;
}

template< class LinearOperatorType >
LinearOperatorType* LinearAffinelyDecomposedDefault< LinearOperatorType >::component(const int ii)
  throw (Exception::requirements_not_met, Exception::index_out_of_range)
{
  if (size_ == 0)
    DUNE_PYMOR_THROW(Exception::requirements_not_met,
                     "do not call component(ii) if size() == 0!");
  if (ii < 0 || ii >= int(size_))
    DUNE_PYMOR_THROW(Exception::index_out_of_range,
                     "the condition 0 < ii < size() is not fulfilled for ii = " << ii << "and size() = "
                     << size_ << "!");
  return components_[ii];
}

template< class LinearOperatorType >
const LinearOperatorType* LinearAffinelyDecomposedDefault< LinearOperatorType >::component(const int ii) const
  throw (Exception::requirements_not_met, Exception::index_out_of_range)
{
  if (size_ == 0)
    DUNE_PYMOR_THROW(Exception::requirements_not_met,
                     "do not call component(ii) if size() == 0!");
  if (ii < 0 || ii >= int(size_))
    DUNE_PYMOR_THROW(Exception::index_out_of_range,
                     "the condition 0 < ii < size() is not fulfilled for ii = " << ii << "and size() = "
                     << size_ << "!");
  return components_[ii];
}

template< class LinearOperatorType >
const ParameterFunctional* LinearAffinelyDecomposedDefault< LinearOperatorType >::coefficient(const int ii) const
  throw (Exception::requirements_not_met, Exception::index_out_of_range)
{
  if (size_ == 0)
    DUNE_PYMOR_THROW(Exception::requirements_not_met,
                     "do not call coefficient(ii) if size() == 0!");
  if (ii < 0 || ii >= int(size_))
    DUNE_PYMOR_THROW(Exception::index_out_of_range,
                     "the condition 0 < ii < size() is not fulfilled for ii = " << ii << "and size() = "
                     << size_ << "!");
  return coefficients_[ii];

}

template< class LinearOperatorType >
bool LinearAffinelyDecomposedDefault< LinearOperatorType >::hasAffinePart() const
{
  return hasAffinePart_;
}

template< class LinearOperatorType >
LinearOperatorType* LinearAffinelyDecomposedDefault< LinearOperatorType >::affinePart()
  throw (Exception::requirements_not_met)
{
  if (!hasAffinePart())
    DUNE_PYMOR_THROW(Exception::requirements_not_met,
                     "do not call affinePart() if hasAffinePart() == false!");
  return affinePart_;
}

template< class LinearOperatorType >
const LinearOperatorType* LinearAffinelyDecomposedDefault< LinearOperatorType >::affinePart() const
  throw (Exception::requirements_not_met)
{
  if (!hasAffinePart())
    DUNE_PYMOR_THROW(Exception::requirements_not_met,
                     "do not call affinePart() if hasAffinePart() == false!");
  return affinePart_;
}

template< class LinearOperatorType >
bool LinearAffinelyDecomposedDefault< LinearOperatorType >::linear() const
{
  return true;
}

template< class LinearOperatorType >
unsigned int LinearAffinelyDecomposedDefault< LinearOperatorType >::dim_source() const
{
  return dim_source_;
}

template< class LinearOperatorType >
unsigned int LinearAffinelyDecomposedDefault< LinearOperatorType >::dim_range() const
{
  return dim_range_;
}

template< class LinearOperatorType >
std::string LinearAffinelyDecomposedDefault< LinearOperatorType >::type_source() const
{
  return type_source_;
}

template< class LinearOperatorType >
std::string LinearAffinelyDecomposedDefault< LinearOperatorType >::type_range() const
{
  return type_range_;
}

template< class LinearOperatorType >
void LinearAffinelyDecomposedDefault< LinearOperatorType >::apply(const LA::VectorInterface* source,
                                                                  LA::VectorInterface* range,
                                                                  const Parameter /*mu*/) const
  throw (Exception::types_are_not_compatible,
         Exception::you_have_to_implement_this,
         Exception::sizes_do_not_match,
         Exception::wrong_parameter_type,
         Exception::requirements_not_met,
         Exception::linear_solver_failed/*,
         Exception::this_does_not_make_any_sense*/)
{
  std::stringstream msg;
  size_t throw_up = 0;
  if (source->type() != type_source()) {
    msg << "source (" << source->type() << ") is not a compatible type_source (" << type_source() << ")";
    ++throw_up;
  }
  if (range->type() != type_range()) {
    if (throw_up)
      msg << " and ";
    msg << "range (" << range->type() << ") is not a compatible type_range (" << type_range() << ")";
  }
  DUNE_PYMOR_THROW(Exception::types_are_not_compatible, msg.str() << "!");
}

template< class LinearOperatorType >
void LinearAffinelyDecomposedDefault< LinearOperatorType >::apply(const SourceType* source,
                                                                  RangeType* range,
                                                                  const Parameter mu) const
  throw (Exception::types_are_not_compatible,
         Exception::you_have_to_implement_this,
         Exception::sizes_do_not_match,
         Exception::wrong_parameter_type,
         Exception::requirements_not_met,
         Exception::linear_solver_failed/*,
         Exception::this_does_not_make_any_sense*/)
{
  if (source->dim() != dim_source())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "the dim of source (" << source->dim() << ") does not match the dim of this ("
                     << dim_source() << ")!");
  if (range->dim() != dim_range())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "the dim of range (" << range->dim() << ") does not match the dim of this ("
                     << dim_range() << ")!");
  if (mu.type() != Parametric::parameter_type())
    DUNE_PYMOR_THROW(Exception::wrong_parameter_type,
                     "the type of mu (" << mu.type() << ") does not match the parameter_type of this ("
                     << Parametric::parameter_type() << ")!");
  if (size_ == 0 && !hasAffinePart_)
    DUNE_PYMOR_THROW(Exception::requirements_not_met,
               "do not call apply() if size() == 0 and hasAffinePart() == false!");
  if (components_.size() != size_) DUNE_PYMOR_THROW(Exception::this_does_not_make_any_sense, "");
  if (coefficients_.size() != size_) DUNE_PYMOR_THROW(Exception::this_does_not_make_any_sense, "");
  if (hasAffinePart_) {
    affinePart_->apply(source, range);
    if (size_ > 0) {
      RangeType* tmp = new RangeType(dim_range());
      for (size_t ii = 0; ii < size_; ++ii) {
        components_[ii]->apply(source, tmp);
        const Parameter muCoefficient = map_parameter(mu, "coefficient_" + Dune::Stuff::Common::toString(ii));
        tmp->scal(coefficients_[ii]->evaluate(muCoefficient));
        range->iadd(tmp);
      }
      delete tmp;
    }
  } else {
    components_[0]->apply(source, range);
    range->scal(coefficients_[0]->evaluate(mu));
    if (size_ > 1) {
      RangeType* tmp = new RangeType(dim_range());
      for (size_t ii = 1; ii < size_; ++ii) {
        components_[ii]->apply(source, tmp);
        const Parameter muCoefficient = map_parameter(mu, "coefficient_" + Dune::Stuff::Common::toString(ii));
        tmp->scal(coefficients_[ii]->evaluate(muCoefficient));
        range->iadd(tmp);
      }
      delete tmp;
    }
  }
} // void apply(...)

template< class LinearOperatorType >
double LinearAffinelyDecomposedDefault< LinearOperatorType >::apply2(const SourceType* range,
                                                                     const RangeType* source,
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
                     "dim of source (" << source->dim() << ") does not match dim_source of this (" << dim_source()
                     << ")!");
  if (range->dim() != dim_range())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "dim of range (" << range->dim() << ") does not match dim_range of this (" << dim_range()
                     << ")!");
  RangeType* tmp = new RangeType(dim_range());
  apply(source, tmp, mu);
  return range->dot(tmp);
}

template< class LinearOperatorType >
std::vector< std::string > LinearAffinelyDecomposedDefault< LinearOperatorType >::invert_options()
{
  return LinearOperatorType::invert_options();
}

template< class LinearOperatorType >
const OperatorInterface* LinearAffinelyDecomposedDefault< LinearOperatorType >::invert(const std::string /*type*/,
                                                                                       const Parameter /*mu*/) const
  throw (Exception::not_invertible, Exception::key_is_not_valid)
{
  assert(false);
  return nullptr;
}

template< class LinearOperatorType >
void LinearAffinelyDecomposedDefault< LinearOperatorType >::apply_inverse(const LA::VectorInterface* /*range*/,
                                                                          LA::VectorInterface* /*source*/,
                                                                          const std::string /*type*/,
                                                                          const Parameter /*mu*/) const
throw (Exception::types_are_not_compatible,
       Exception::you_have_to_implement_this,
       Exception::sizes_do_not_match,
       Exception::wrong_parameter_type,
       Exception::requirements_not_met,
       Exception::linear_solver_failed,
       Exception::this_does_not_make_any_sense)
{
  assert(false);
}

template< class LinearOperatorType >
LinearOperatorType* LinearAffinelyDecomposedDefault< LinearOperatorType >
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
               "do not call freeze_parameter() if size() == 0 and hasAffinePart() == false!");
  if (components_.size() != size_) DUNE_PYMOR_THROW(Exception::this_does_not_make_any_sense, "");
  if (coefficients_.size() != size_) DUNE_PYMOR_THROW(Exception::this_does_not_make_any_sense, "");
  if (hasAffinePart_) {
    LinearOperatorType* ret = affinePart_->copy();
    if (size_ > 0)
      for (size_t ii = 0; ii < size_; ++ii) {
        const Parameter muCoefficient = map_parameter(mu, "coefficient_" + Dune::Stuff::Common::toString(ii));
        ret->axpy(coefficients_[ii]->evaluate(muCoefficient), components_[ii]);
      }
    return ret;
  } else {
    LinearOperatorType* ret = components_[0]->copy();
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

template class LinearAffinelyDecomposedDefault< DuneDynamicMatrix >;
#if HAVE_EIGEN
template class LinearAffinelyDecomposedDefault< EigenDenseMatrix >;
template class LinearAffinelyDecomposedDefault< EigenRowMajorSparseMatrix >;
#endif // HAVE_EIGEN


} // namespace Operators
} // namespace Pymor
} // namespace Dune
