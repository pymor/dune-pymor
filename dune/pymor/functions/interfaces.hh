// This file is part of the dune-pymor project:
//   https://github.com/pymor/dune-pymor
// Copyright holders: Stephan Rave, Felix Schindler
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_FUNCTIONS_INTERFACES_HH
#define DUNE_PYMOR_FUNCTIONS_INTERFACES_HH

#include <memory>
#include <ostream>
#include <limits>

#include <dune/stuff/common/disable_warnings.hh>
# include <dune/common/fmatrix.hh>
# include <dune/common/fvector.hh>
#include <dune/stuff/common/reenable_warnings.hh>

#include <dune/stuff/functions/interfaces.hh>
#include <dune/stuff/common/string.hh>
#include <dune/stuff/common/memory.hh>

#include <dune/pymor/parameters/base.hh>
#include <dune/pymor/parameters/functional.hh>
#include <dune/pymor/common/exceptions.hh>

namespace Dune {
namespace Pymor {
namespace internal {


template< class ParametricFunctionType >
class FunctionWithParameter;


} // namespace internal


/**
 * \brief Interface for global valued (possibly parametric) functions.
 */
template< class EntityImp, class DomainFieldImp, int domainDim, class RangeFieldImp, int rangeDim, int rangeDimCols = 1 >
class AffinelyDecomposableFunctionInterface
  : public Parametric
{
  typedef AffinelyDecomposableFunctionInterface
      < EntityImp, DomainFieldImp, domainDim, RangeFieldImp, rangeDim, rangeDimCols > ThisType;
public:
  typedef Stuff::LocalizableFunctionInterface
      < EntityImp, DomainFieldImp, domainDim, RangeFieldImp, rangeDim, rangeDimCols > NonparametricType;
  typedef typename NonparametricType::EntityType EntityType;

  typedef typename NonparametricType::DomainFieldType DomainFieldType;
  static const unsigned int                           dimDomain = NonparametricType::dimDomain;
  typedef typename NonparametricType::DomainType      DomainType;

  typedef typename NonparametricType::RangeFieldType  RangeFieldType;
  static const unsigned int                           dimRange = NonparametricType::dimRange;
  static const unsigned int                           dimRangeCols = NonparametricType::dimRangeCols;
  typedef typename NonparametricType::RangeType       RangeType;

  typedef typename NonparametricType::JacobianRangeType JacobianRangeType;

  static const bool available = false;

  static std::string static_id()
  {
    return "pymor.function";
  }

  AffinelyDecomposableFunctionInterface(const ParameterType tt = ParameterType())
    : Parametric(tt)
    , nullptr_1_(nullptr)
    , nullptr_2_(nullptr)
  {}

  AffinelyDecomposableFunctionInterface(const Parametric& other)
    : Parametric(other)
    , nullptr_1_(nullptr)
    , nullptr_2_(nullptr)
  {}

  virtual ~AffinelyDecomposableFunctionInterface() {}

  virtual std::string type() const
  {
    return "pymor.function";
  }

  virtual std::string name() const
  {
    return "pymor.function";
  }

  /**
   * \note  This will make sense in the future, once we have parametric functions which are not affinely decomposable.
   */
  virtual bool affinely_decomposable() const
  {
    return true;
  }

  virtual bool has_affine_part() const
  {
    return !parametric();
  }

  virtual const std::shared_ptr< const NonparametricType >& affine_part() const
  {
    if (has_affine_part())
      DUNE_THROW(Stuff::Exceptions::you_have_to_implement_this, "since has_affine_part() == true, you really do!");
    else
      DUNE_THROW(Stuff::Exceptions::requirements_not_met, "Do not call affine_part() if has_affine_part() == false!");
    return nullptr_1_;
  } // ... affine_part(...)

  virtual DUNE_STUFF_SSIZE_T num_components() const
  {
    if (!has_affine_part())
      return 0;
    else
      DUNE_THROW(Stuff::Exceptions::you_have_to_implement_this,
                 "since affinely_decomposable() == true and has_affine_part() == false, you really do!");
    return false;
  } // ... num_components(...)

  virtual const std::shared_ptr< const NonparametricType >& component(const DUNE_STUFF_SSIZE_T qq) const
  {
    if (!parametric())
      DUNE_THROW(Exceptions::this_is_not_parametric, "Do not call component(" << qq << ") if parametric() == false!");
    if (num_components() > 0)
      DUNE_THROW(Stuff::Exceptions::you_have_to_implement_this,
                 "since num_components() (= " << num_components() << ") > 0, you really do!");
    else
      DUNE_THROW(Stuff::Exceptions::requirements_not_met,
                 "Do not call component(" << qq << ") if num_components() == 0!");
    return nullptr_1_;
  } // ... component(...)

  virtual const std::shared_ptr< const ParameterFunctional >& coefficient(const DUNE_STUFF_SSIZE_T qq) const
  {
    if (!parametric())
      DUNE_THROW(Exceptions::this_is_not_parametric,
                 "Do not call coefficient(" << qq << ") if parametric() == false!");
    if (num_components() > 0)
      DUNE_THROW(Stuff::Exceptions::you_have_to_implement_this,
                 "since num_coefficients() (= " << num_components() << ") > 0, you really do!");
    else
      DUNE_THROW(Stuff::Exceptions::requirements_not_met,
                 "Do not call coefficient(" << qq << ") if num_coefficients() == 0!");
    return nullptr_2_;
  } // ... coefficient(...)

  virtual void report(std::ostream& out, const std::string prefix = "") const
  {
    out << prefix << "affinely decomposable function '" << name() << "' (of type " << type() << "):";
    if (parametric())
      out << "\n" << prefix << "  parameter_type: " << parameter_type();
    if (has_affine_part()) {
      out << "\n";
      affine_part()->report(out, prefix + "  affine_part: ");
    }
    for (DUNE_STUFF_SSIZE_T qq = 0; qq < num_components(); ++qq) {
      out << "\n";
      component(qq)->report(out, prefix + "  component " + Stuff::Common::toString(qq) + ": ");
      out << "\n" << prefix << "  coefficient " << qq << ": " << coefficient(qq)->expression();
    }
  } // ... report(...)

  virtual std::shared_ptr< const NonparametricType > with_mu(const Parameter mu = Parameter()) const
  {
    if (this->parametric()) {
      if (mu.type() != this->parameter_type())
        DUNE_THROW(Pymor::Exceptions::wrong_parameter_type,
                   "mu is " << mu.type() << ", should be " << this->parameter_type() << "!");
      return std::make_shared< internal::FunctionWithParameter< ThisType > >(*this, mu);
    } else {
      assert(has_affine_part());
      return affine_part();
    }
  } // ... with_mu(...)

  virtual double gamma(const Parameter& mu_1, const Parameter& mu_2) const
  {
    if (parametric()) {
      if (mu_1.type() != this->parameter_type())
        DUNE_THROW(Exceptions::wrong_parameter_type,
                   "The type of mu_1 is " << mu_1 << " and should be " << this->parameter_type());
      if (mu_2.type() != this->parameter_type())
        DUNE_THROW(Exceptions::wrong_parameter_type,
                   "The type of mu_2 is " << mu_2 << " and should be " << this->parameter_type());
      if (mu_1 == mu_2)
        return 1.0;
      else {
        double ret = std::numeric_limits< double >::min();
        assert(num_components() > 0);
        for (DUNE_STUFF_SSIZE_T qq = 0; qq < num_components(); ++qq) {
          const double theta_mu_1 = coefficient(qq)->evaluate(mu_1);
          const double theta_mu_2 = coefficient(qq)->evaluate(mu_2);
          ret = std::max(ret, theta_mu_1 / theta_mu_2);
        }
        if (has_affine_part())
          ret = std::max(ret, 1.0);
        return ret;
      }
    } else
      return 1.0;
  } // ... gamma(...)

  virtual double alpha(const Parameter& mu_1, const Parameter& mu_2) const
  {
    if (parametric()) {
      if (mu_1.type() != this->parameter_type())
        DUNE_THROW(Exceptions::wrong_parameter_type,
                   "The type of mu_1 is " << mu_1 << " and should be " << this->parameter_type());
      if (mu_2.type() != this->parameter_type())
        DUNE_THROW(Exceptions::wrong_parameter_type,
                   "The type of mu_2 is " << mu_2 << " and should be " << this->parameter_type());
      if (mu_1 == mu_2)
        return 1.0;
      else {
        double ret = std::numeric_limits< double >::max();
        assert(num_components() > 0);
        for (DUNE_STUFF_SSIZE_T qq = 0; qq < num_components(); ++qq) {
          const double theta_mu_1 = coefficient(qq)->evaluate(mu_1);
          const double theta_mu_2 = coefficient(qq)->evaluate(mu_2);
          ret = std::min(ret, theta_mu_1 / theta_mu_2);
        }
        if (has_affine_part())
          ret = std::min(ret, 1.0);
        return ret;
      }
    } else
      return 1.0;
  } // ... alpha(...)

private:
  template< class T >
  friend std::ostream& operator<<(std::ostream& /*out*/, const ThisType& /*function*/);

  const std::shared_ptr< const NonparametricType > nullptr_1_;
  const std::shared_ptr< const ParameterFunctional > nullptr_2_;
}; // class AffinelyDecomposableFunctionInterface


template< class E, class D, int d, class R, int r, int rC >
std::ostream& operator<<(std::ostream& out,
                         const AffinelyDecomposableFunctionInterface< E, D, d, R, r, rC >& function)
{
  function.report(out);
  return out;
} // ... operator<<(...)


namespace internal {


template< class ParametricFunctionType >
class FunctionWithParameter
  : public Stuff::LocalizableFunctionInterface< typename ParametricFunctionType::EntityType
                                              , typename ParametricFunctionType::DomainFieldType
                                              , ParametricFunctionType::dimDomain
                                              , typename ParametricFunctionType::RangeFieldType
                                              , ParametricFunctionType::dimRange
                                              , ParametricFunctionType::dimRangeCols >
{
  typedef FunctionWithParameter< ParametricFunctionType > ThisType;
  typedef typename ParametricFunctionType::EntityType EE;
  typedef typename ParametricFunctionType::DomainFieldType DD;
  static const unsigned int dd = ParametricFunctionType::dimDomain;
  typedef typename ParametricFunctionType::RangeFieldType RR;
  static const unsigned int rr = ParametricFunctionType::dimRange;
  static const unsigned int rC = ParametricFunctionType::dimRangeCols;
  typedef Stuff::LocalizableFunctionInterface< EE, DD, dd, RR, rr, rC > BaseType;

  class LocalFunction
    : public Stuff::LocalfunctionInterface< EE, DD, dd, RR, rr, rC >
  {
    typedef Stuff::LocalfunctionInterface< EE, DD, dd, RR, rr, rC > BaseType;
    typedef typename BaseType::DomainType DomainType;
    typedef typename BaseType::RangeType  RangeType;
    typedef typename BaseType::JacobianRangeType JacobianRangeType;

  public:
    LocalFunction(const EE& entity, const ParametricFunctionType& function, const std::vector< double >& coefficients)
      : BaseType(entity)
      , coefficients_(coefficients)
      , local_components_(function.num_components(), nullptr)
      , order_(0)
      , tmp_range_(0)
      , tmp_jacobian_range_(0)
    {
      for (DUNE_STUFF_SSIZE_T qq = 0; qq < function.num_components(); ++qq) {
        local_components_[qq] = function.component(qq)->local_function(entity);
        order_ = std::max(order_, local_components_[qq]->order());
      }
      if (function.has_affine_part())
        affine_part_.push_back(function.affine_part()->local_function(entity));
    } // LocalFunction(...)

    LocalFunction(const LocalFunction& /*other*/) = delete;

    LocalFunction& operator=(const LocalFunction& /*other*/) = delete;

    virtual size_t order() const override
    {
      return order_;
    }

    virtual void evaluate(const DomainType& xx, RangeType& ret) const override
    {
      ret *= 0.0;
      for (size_t qq = 0; qq < local_components_.size(); ++qq) {
        local_components_[qq]->evaluate(xx, tmp_range_);
        tmp_range_ *= coefficients_[qq];
        ret += tmp_range_;
      }
      if (affine_part_.size() > 0) {
        affine_part_[0]->evaluate(xx, tmp_range_);
        ret += tmp_range_;
      }
    } // ... evaluate(...)

    virtual void jacobian(const DomainType& xx, JacobianRangeType& ret) const override
    {
      ret *= 0.0;
      for (size_t qq = 0; qq < local_components_.size(); ++qq) {
        local_components_[qq]->jacobian(xx, tmp_jacobian_range_);
        tmp_jacobian_range_ *= coefficients_[qq];
        ret += tmp_jacobian_range_;
      }
      if (affine_part_.size() > 0) {
        affine_part_[0]->jacobian(xx, tmp_jacobian_range_);
        ret += tmp_jacobian_range_;
      }
    } // ... jacobian(...)

  private:
    const std::vector< double >& coefficients_;
    std::vector< std::shared_ptr< BaseType > > local_components_;
    size_t order_;
    mutable RangeType tmp_range_;
    mutable JacobianRangeType tmp_jacobian_range_;
    std::vector< std::shared_ptr< BaseType > > affine_part_;
  }; // class LocalFunction

public:
  typedef typename BaseType::EntityType EntityType;
  typedef typename BaseType::LocalfunctionType LocalfunctionType;

  FunctionWithParameter(const ParametricFunctionType& parametric_function,
                        const Parameter mu,
                        const std::string nm = "")
    : parametric_function_(parametric_function)
    , name_(nm.empty() ? parametric_function_.name() : nm)
    , type_(parametric_function_.type())
    , coefficients_(parametric_function_.num_components())
  {
    assert(parametric_function_.parametric());
    assert(mu.type() == parametric_function_.parameter_type());
    for (DUNE_STUFF_SSIZE_T qq = 0; qq < parametric_function_.num_components(); ++qq)
      coefficients_[qq] = parametric_function_.coefficient(qq)->evaluate(mu);
  }

  FunctionWithParameter(const ThisType& other)
    : parametric_function_(other.parametric_function_)
    , name_(other.name_)
    , type_(other.type_)
    , coefficients_(other.coefficients_)
  {}

  ThisType& operator=(const ThisType& other) = delete;

  virtual std::unique_ptr< LocalfunctionType > local_function(const EntityType& entity) const override
  {
    return Stuff::Common::make_unique< LocalFunction >(entity, parametric_function_, coefficients_);
  }

  virtual ThisType* copy() const override
  {
    DUNE_THROW(NotImplemented, "Implement me!");
  }

  virtual std::string type() const override
  {
    return type_;
  }

  virtual std::string name() const override
  {
    return name_;
  }

private:
  const ParametricFunctionType& parametric_function_;
  const std::string name_;
  const std::string type_;
  std::vector< double > coefficients_;
}; // class FunctionWithParameter


} // namespace internal
} // namespace Pymor
} // namespace Dune

#endif // DUNE_PYMOR_FUNCTIONS_INTERFACES_HH
