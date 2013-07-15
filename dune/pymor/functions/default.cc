// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#include "default.hh"

#include <type_traits>
namespace Dune {
namespace Pymor {
namespace Functions {


// ================================
// ===== NonparametricWrapper =====
// ================================
template< class D, int d, class R, int rR, int rC >
NonparametricWrapper< D, d, R, rR, rC >::NonparametricWrapper(const NonparametricType* nonparametric)
  : BaseType()
  , nonparametric_(nonparametric)
{}

template< class D, int d, class R, int rR, int rC >
NonparametricWrapper< D, d, R, rR, rC >::~NonparametricWrapper()
{
  delete nonparametric_;
}

template< class D, int d, class R, int rR, int rC >
std::string NonparametricWrapper< D, d, R, rR, rC >::name() const
{
  return nonparametric_->name();
}

template< class D, int d, class R, int rR, int rC >
int NonparametricWrapper< D, d, R, rR, rC >::order() const
{
  return nonparametric_->order();
}

template< class D, int d, class R, int rR, int rC >
void NonparametricWrapper< D, d, R, rR, rC >::evaluate(const DomainType& x, RangeType& ret, const Parameter mu) const
  throw (Exception::wrong_parameter_type)
{
  if (mu.type() != Parametric::parameter_type())
    DUNE_PYMOR_THROW(Exception::wrong_parameter_type,
                     "the type of mu must be trivial (is " << mu.type() << ")!");
  nonparametric_->evaluate(x, ret);
}

template class NonparametricWrapper< double, 1, double, 1, 1 >;
template class NonparametricWrapper< double, 1, double, 1, 2 >;
template class NonparametricWrapper< double, 1, double, 1, 3 >;
template class NonparametricWrapper< double, 1, double, 2, 1 >;
template class NonparametricWrapper< double, 1, double, 2, 2 >;
template class NonparametricWrapper< double, 1, double, 2, 3 >;
template class NonparametricWrapper< double, 1, double, 3, 1 >;
template class NonparametricWrapper< double, 1, double, 3, 2 >;
template class NonparametricWrapper< double, 1, double, 3, 3 >;
template class NonparametricWrapper< double, 2, double, 1, 1 >;
template class NonparametricWrapper< double, 2, double, 1, 2 >;
template class NonparametricWrapper< double, 2, double, 1, 3 >;
template class NonparametricWrapper< double, 2, double, 2, 1 >;
template class NonparametricWrapper< double, 2, double, 2, 2 >;
template class NonparametricWrapper< double, 2, double, 2, 3 >;
template class NonparametricWrapper< double, 2, double, 3, 1 >;
template class NonparametricWrapper< double, 2, double, 3, 2 >;
template class NonparametricWrapper< double, 2, double, 3, 3 >;
template class NonparametricWrapper< double, 3, double, 1, 1 >;
template class NonparametricWrapper< double, 3, double, 1, 2 >;
template class NonparametricWrapper< double, 3, double, 1, 3 >;
template class NonparametricWrapper< double, 3, double, 2, 1 >;
template class NonparametricWrapper< double, 3, double, 2, 2 >;
template class NonparametricWrapper< double, 3, double, 2, 3 >;
template class NonparametricWrapper< double, 3, double, 3, 1 >;
template class NonparametricWrapper< double, 3, double, 3, 2 >;
template class NonparametricWrapper< double, 3, double, 3, 3 >;


// ===================================
// ===== NonparametricExpression =====
// ===================================
template< class D, int d, class R, int r >
NonparametricExpression< D, d, R, r >::NonparametricExpression(const std::string var,
                                                               const std::string expr,
                                                               const int oo,
                                                               const std::string nm)
  : BaseType()
  , expressionFunction_(var, expr, oo, nm)
{}

template< class D, int d, class R, int r >
NonparametricExpression< D, d, R, r >::NonparametricExpression(const std::string var,
                                                               const std::vector< std::string > exprs,
                                                               const int oo,
                                                               const std::string nm)
  : BaseType()
  , expressionFunction_(var, exprs, oo, nm)
{}

template< class D, int d, class R, int r >
std::string NonparametricExpression< D, d, R, r >::name() const
{
  return expressionFunction_.name();
}

template< class D, int d, class R, int r >
int NonparametricExpression< D, d, R, r >::order() const
{
  return expressionFunction_.order();
}

template< class D, int d, class R, int r >
void NonparametricExpression< D, d, R, r >::evaluate(const DomainType& x, RangeType& ret, const Parameter mu) const
  throw (Exception::wrong_parameter_type)
{
  if (mu.type() != Parametric::parameter_type())
    DUNE_PYMOR_THROW(Exception::wrong_parameter_type,
                     "the type of mu must be trivial (is " << mu.type() << ")!");
  expressionFunction_.evaluate(x, ret);
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


// ===================================
// ===== AffineParametricDefault =====
// ===================================
template< class D, int d, class R, int rR, int rC >
AffineParametricDefault< D, d, R, rR, rC >::AffineParametricDefault()
  : BaseType()
  , size_(0)
  , hasAffinePart_(false)
  , order_(-1)
{}

template< class D, int d, class R, int rR, int rC >
AffineParametricDefault< D, d, R, rR, rC >::AffineParametricDefault(const ParametricFunctionType* aff)
  : BaseType()
  , size_(0)
  , hasAffinePart_(true)
  , order_(-1)
  , affinePart_(aff)
{}

template< class D, int d, class R, int rR, int rC >
AffineParametricDefault< D, d, R, rR, rC >::~AffineParametricDefault()
{
  if (hasAffinePart_)
    delete affinePart_;
  for (auto& element : components_)
    delete element;
  for (auto& element : coefficients_)
    delete element;
}

template< class D, int d, class R, int rR, int rC >
void AffineParametricDefault< D, d, R, rR, rC >::register_component(const ParametricFunctionType* aff)
  throw (Exception::this_does_not_make_any_sense)
{
  if (hasAffinePart_)
    DUNE_PYMOR_THROW(Exception::this_does_not_make_any_sense,
                     "do not call register_component(affinePart) if hasAffinePart() == true!");
  if (aff->parametric())
    DUNE_PYMOR_THROW(Exception::this_does_not_make_any_sense,
                     "the affinePart must not be parametric!");
  if (size_ == 0) {
    order_ = aff->order();
  } else {
    if (order_ < 0 || aff->order() < 0)
      order_ = -1;
    else
      order_ = std::max(order_, aff->order());
  }
  affinePart_ = aff;
  hasAffinePart_ = true;
} // ... register_component(...)

template< class D, int d, class R, int rR, int rC >
void AffineParametricDefault< D, d, R, rR, rC >::register_component(const ParametricFunctionType* comp,
                                                                    const ParameterFunctional* coeff)
  throw (Exception::this_does_not_make_any_sense)
{
  if (comp->parametric())
    DUNE_PYMOR_THROW(Exception::this_does_not_make_any_sense,
                     "a component must not be parametric!");
  if (size_ == 0 && !hasAffinePart_) {
    order_ = comp->order();
  } else {
    if (order_ < 0 || comp->order() < 0)
      order_ = -1;
    else
      order_ = std::max(order_, comp->order());
  }
  Parametric::inherit_parameter_type(coeff->parameter_type(), "coefficient_" + Dune::Stuff::Common::toString(size_));
  components_.push_back(comp);
  coefficients_.push_back(coeff);
  ++size_;
} // ... register_component(..., ...)

template< class D, int d, class R, int rR, int rC >
unsigned int AffineParametricDefault< D, d, R, rR, rC >::num_components() const
{
  return size_;
}

template< class D, int d, class R, int rR, int rC >
const typename AffineParametricDefault< D, d, R, rR, rC >::ParametricFunctionType*
AffineParametricDefault< D, d, R, rR, rC >::component(const int ii) const
  throw (Exception::requirements_not_met, Exception::index_out_of_range)
{
  if (size_ == 0)
    DUNE_PYMOR_THROW(Exception::requirements_not_met,
                     "do not call component(ii) if size() == 0!");
  if (ii < 0 || ii >= size_)
    DUNE_PYMOR_THROW(Exception::index_out_of_range,
                     "the condition 0 < ii < size() is not fulfilled for ii = " << ii << "and size() = "
                     << size_ << "!");
  return components_[ii];
}

template< class D, int d, class R, int rR, int rC >
const ParameterFunctional* AffineParametricDefault< D, d, R, rR, rC >::coefficient(const int ii)
  const throw (Exception::requirements_not_met, Exception::index_out_of_range)
{
  if (size_ == 0)
    DUNE_PYMOR_THROW(Exception::requirements_not_met,
                     "do not call coefficient(ii) if size() == 0!");
  if (ii < 0 || ii >= size_)
    DUNE_PYMOR_THROW(Exception::index_out_of_range,
                     "the condition 0 < ii < size() is not fulfilled for ii = " << ii << "and size() = "
                     << size_ << "!");
  return coefficients_[ii];

}

template< class D, int d, class R, int rR, int rC >
bool AffineParametricDefault< D, d, R, rR, rC >::hasAffinePart() const
{
  return hasAffinePart_;
}

template< class D, int d, class R, int rR, int rC >
const typename AffineParametricDefault< D, d, R, rR, rC >::ParametricFunctionType*
AffineParametricDefault< D, d, R, rR, rC >::affinePart() const throw(Exception::requirements_not_met)
{
  if (!hasAffinePart())
    DUNE_PYMOR_THROW(Exception::requirements_not_met,
                     "do not call affinePart() if hasAffinePart() == false!");
  return affinePart_;
}

template< class D, int d, class R, int rR, int rC >
int AffineParametricDefault< D, d, R, rR, rC >::order() const
{
  return order_;
}

template< class D, int d, class R, int rR, int rC >
void AffineParametricDefault< D, d, R, rR, rC >::evaluate(const DomainType& x, RangeType& ret, const Parameter mu) const
  throw (Exception::wrong_parameter_type, Exception::this_does_not_make_any_sense)
{
  if (mu.type() != Parametric::parameter_type())
    DUNE_PYMOR_THROW(Exception::wrong_parameter_type,
                     "the type of mu (" << mu.type() << ") does not math the parameter_type of this ("
                     << Parametric::parameter_type() << ")!");
  if (!hasAffinePart_ && size_ == 0)
    DUNE_PYMOR_THROW(Exception::this_does_not_make_any_sense,
                     "do not call evaluate() if hasAffinePart() == false and size() == 0");
  RangeType tmpComp(0);
  double tmpCoeff = 0.0;
  if (hasAffinePart_)
    affinePart_->evaluate(x, ret);
  else
    ret *= 0.0;
  for (size_t ii = 0; ii < size_; ++ii) {
    components_[ii]->evaluate(x, tmpComp);
    const Parameter muCoefficient = Parametric::map_parameter(mu, "coefficient_" + Dune::Stuff::Common::toString(ii));
    coefficients_[ii]->evaluate(muCoefficient, tmpCoeff);
    tmpComp *= tmpCoeff;
    ret += tmpComp;
  }
}

template class AffineParametricDefault< double, 1, double, 1, 1 >;
template class AffineParametricDefault< double, 1, double, 1, 2 >;
template class AffineParametricDefault< double, 1, double, 1, 3 >;
template class AffineParametricDefault< double, 1, double, 2, 1 >;
template class AffineParametricDefault< double, 1, double, 2, 2 >;
template class AffineParametricDefault< double, 1, double, 2, 3 >;
template class AffineParametricDefault< double, 1, double, 3, 1 >;
template class AffineParametricDefault< double, 1, double, 3, 2 >;
template class AffineParametricDefault< double, 1, double, 3, 3 >;
template class AffineParametricDefault< double, 2, double, 1, 1 >;
template class AffineParametricDefault< double, 2, double, 1, 2 >;
template class AffineParametricDefault< double, 2, double, 1, 3 >;
template class AffineParametricDefault< double, 2, double, 2, 1 >;
template class AffineParametricDefault< double, 2, double, 2, 2 >;
template class AffineParametricDefault< double, 2, double, 2, 3 >;
template class AffineParametricDefault< double, 2, double, 3, 1 >;
template class AffineParametricDefault< double, 2, double, 3, 2 >;
template class AffineParametricDefault< double, 2, double, 3, 3 >;
template class AffineParametricDefault< double, 3, double, 1, 1 >;
template class AffineParametricDefault< double, 3, double, 1, 2 >;
template class AffineParametricDefault< double, 3, double, 1, 3 >;
template class AffineParametricDefault< double, 3, double, 2, 1 >;
template class AffineParametricDefault< double, 3, double, 2, 2 >;
template class AffineParametricDefault< double, 3, double, 2, 3 >;
template class AffineParametricDefault< double, 3, double, 3, 1 >;
template class AffineParametricDefault< double, 3, double, 3, 2 >;
template class AffineParametricDefault< double, 3, double, 3, 3 >;


} // namespace Functions
} // namespace Pymor
} // namespace Dune
