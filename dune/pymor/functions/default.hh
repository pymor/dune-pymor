// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_FUNCTIONS_DEFAULT_HH
#define DUNE_PYMOR_FUNCTIONS_DEFAULT_HH

#include <dune/stuff/function/interface.hh>
#include <dune/stuff/function/expression.hh>

#include <dune/pymor/common/exceptions.hh>
#include <dune/pymor/parameters/base.hh>
#include <dune/pymor/parameters/functional.hh>
#include "interfaces.hh"

namespace Dune {
namespace Pymor {
namespace Functions {


template< class DomainFieldImp, int domainDim, class RangeFieldImp, int rangeDimRows, int rangeDimCols = 1 >
class NonparametricWrapper
  : public ParametricFunctionInterface< DomainFieldImp, domainDim, RangeFieldImp, rangeDimRows, rangeDimCols >
{
  typedef ParametricFunctionInterface< DomainFieldImp, domainDim, RangeFieldImp, rangeDimRows, rangeDimCols > BaseType;
public:
  typedef Dune::Stuff::FunctionInterface< DomainFieldImp, domainDim,
                                          RangeFieldImp, rangeDimRows, rangeDimCols > NonparametricType;
  typedef typename BaseType::DomainType DomainType;
  typedef typename BaseType::RangeType  RangeType;

  NonparametricWrapper(const NonparametricType* nonparametric)
    : BaseType()
    , nonparametric_(nonparametric)
  {}

  ~NonparametricWrapper()
  {
    delete nonparametric_;
  }

  int order() const
  {
    return nonparametric_->order();
  }

  virtual void evaluate(const DomainType& x, RangeType& ret, const Parameter mu = Parameter()) const
  {
    if (mu.type() != Parametric::parameter_type())
      DUNE_PYMOR_THROW(Exception::wrong_parameter_type,
                       "the type of mu must be trivial (is " << mu.type() << ")!");
    nonparametric_->evaluate(x, ret);
  }

  using BaseType::evaluate;

private:
  const NonparametricType* nonparametric_;
}; // class NonparametricWrapper


template< class DomainFieldImp, int domainDim, class RangeFieldImp, int rangeDimRows, int rangeDimCols = 1 >
class NonparametricExpression
  : public ParametricFunctionInterface< DomainFieldImp, domainDim, RangeFieldImp, rangeDimRows, rangeDimCols >
{
  typedef ParametricFunctionInterface< DomainFieldImp, domainDim, RangeFieldImp, rangeDimRows, rangeDimCols > BaseType;
  typedef Dune::Stuff::FunctionExpression<  DomainFieldImp, domainDim,
                                            RangeFieldImp, rangeDimRows, rangeDimCols > ExpressionFunctionType;
public:
  typedef typename BaseType::DomainType DomainType;
  typedef typename BaseType::RangeType  RangeType;

  template< class... Args >
  NonparametricExpression(Args&& ...args)
    : BaseType()
    , expressionFunction_(std::forward< Args >(args)...)
  {}

  int order() const
  {
    return expressionFunction_.order();
  }

  virtual void evaluate(const DomainType& x, RangeType& ret, const Parameter mu = Parameter()) const
  {
    if (mu.type() != Parametric::parameter_type())
      DUNE_PYMOR_THROW(Exception::wrong_parameter_type,
                       "the type of mu must be trivial (is " << mu.type() << ")!");
    expressionFunction_.evaluate(x, ret);
  }

  using BaseType::evaluate;

private:
  const ExpressionFunctionType expressionFunction_;
}; // class NonparametricExpression


template< class DomainFieldImp, int domainDim, class RangeFieldImp, int rangeDimRows, int rangeDimCols = 1 >
class AffineParametricDefault
  : public AffineParametricFunctionInterface< DomainFieldImp, domainDim, RangeFieldImp, rangeDimRows, rangeDimCols >
{
  typedef AffineParametricFunctionInterface<  DomainFieldImp, domainDim,
                                              RangeFieldImp, rangeDimRows, rangeDimCols > BaseType;
public:
  typedef ParametricFunctionInterface<  DomainFieldImp, domainDim,
                                        RangeFieldImp, rangeDimRows, rangeDimCols > ParametricFunctionType;
  typedef typename BaseType::DomainType DomainType;
  typedef typename BaseType::RangeType  RangeType;

  AffineParametricDefault(const ParameterType mu = ParameterType())
    : BaseType(mu)
    , size_(0)
    , hasAffinePart_(false)
    , order_(-1)
  {}

  virtual ~AffineParametricDefault()
  {
    if (hasAffinePart_)
      delete affinePart_;
    for (auto& element : components_)
      delete element;
    for (auto& element : coefficients_)
      delete element;
  }

  static const std::string static_id()
  {
    return BaseType::static_id() + ".default";
  }

  virtual std::string id() const
  {
    return static_id();
  }

  /**
   * \attention This class takes ownership of aff!
   */
  virtual void register_component(const ParametricFunctionType* aff) throw (Exception::this_does_not_make_any_sense)
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

  /**
   * \attention This class takes ownership of comp and coeff!
   */
  virtual void register_component(const ParametricFunctionType* comp, const ParameterFunctional* coeff)
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
    if (coeff->parameter_type() != Parametric::parameter_type())
      DUNE_PYMOR_THROW(Exception::wrong_parameter_type,
                       "different parameter types for coeff (" << coeff->parameter_type() << ") and this ("
                       << Parametric::parameter_type() << ") is not yet supported!");
    components_.push_back(comp);
    coefficients_.push_back(coeff);
    ++size_;
  } // ... register_component(..., ...)

  virtual unsigned int size() const
  {
    return size_;
  }

  /**
   * \attention The ownership of the component remains with this class!
   */
  virtual const ParametricFunctionType* component(const int ii) const throw (Exception::requirements_not_met,
                                                                             Exception::index_out_of_range)
  {
    if (size() == 0)
      DUNE_PYMOR_THROW(Exception::requirements_not_met,
                       "do not call component(ii) if size() == 0!");
    if (ii < 0 || ii >= size())
      DUNE_PYMOR_THROW(Exception::index_out_of_range,
                       "the condition 0 < ii < size() is not fulfilled for ii = " << ii << "and size() = "
                       << size() << "!");
    return components_[ii];
  }

  /**
   * \attention The ownership of the coefficient remains with this class!
   */
  virtual const ParameterFunctional* coefficient(const int ii) const throw (Exception::requirements_not_met,
                                                                            Exception::index_out_of_range)
  {
    if (size() == 0)
      DUNE_PYMOR_THROW(Exception::requirements_not_met,
                       "do not call coefficient(ii) if size() == 0!");
    if (ii < 0 || ii >= size())
      DUNE_PYMOR_THROW(Exception::index_out_of_range,
                       "the condition 0 < ii < size() is not fulfilled for ii = " << ii << "and size() = "
                       << size() << "!");
    return coefficients_[ii];

  }

  virtual bool hasAffinePart() const
  {
    return hasAffinePart_;
  }

  /**
   * \attention The ownership of affinePart() remains in this class!
   */
  virtual const ParametricFunctionType* affinePart() const throw(Exception::requirements_not_met)
  {
    if (!hasAffinePart())
      DUNE_PYMOR_THROW(Exception::requirements_not_met,
                       "do not call affinePart() if hasAffinePart() == false!");
    return affinePart_;
  }

  virtual int order() const
  {
    return order_;
  }

  virtual void evaluate(const DomainType& x, RangeType& ret, const Parameter mu = Parameter()) const
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
      coefficients_[ii]->evaluate(mu, tmpCoeff);
      tmpComp *= tmpCoeff;
      ret += tmpComp;
    }
  }

  using BaseType::evaluate;

public:
  size_t size_;
  bool hasAffinePart_;
  int order_;
  std::vector< const ParametricFunctionType* > components_;
  std::vector< const ParameterFunctional* > coefficients_;
  const ParametricFunctionType* affinePart_;
}; // class AffineParametricDefault

} // namespace Functions
} // namespace Pymor
} // namespace Dune

#endif // DUNE_PYMOR_FUNCTIONS_DEFAULT_HH
