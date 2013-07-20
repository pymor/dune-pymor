// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_FUNCTIONS_DEFAULT_HH
#define DUNE_PYMOR_FUNCTIONS_DEFAULT_HH

#include <dune/stuff/functions/interfaces.hh>
#include <dune/stuff/functions/expression.hh>

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
  typedef typename NonparametricType::DomainType DomainType;
  typedef typename NonparametricType::RangeType  RangeType;

  /**
   * \attention This class takes ownership of nonparametric!
   */
  NonparametricWrapper(const NonparametricType* nonparametric);

  virtual ~NonparametricWrapper();

  virtual std::string name() const;

  virtual int order() const;

  virtual void evaluate(const DomainType& x, RangeType& ret, const Parameter mu = Parameter()) const
    throw (Exception::wrong_parameter_type);

  using BaseType::evaluate;

private:
  const NonparametricType* nonparametric_;
}; // class NonparametricWrapper


template< class DomainFieldImp, int domainDim, class RangeFieldImp, int rangeDimRows >
class NonparametricExpression
  : public ParametricFunctionInterface< DomainFieldImp, domainDim, RangeFieldImp, rangeDimRows, 1 >
{
  typedef ParametricFunctionInterface< DomainFieldImp, domainDim, RangeFieldImp, rangeDimRows, 1 > BaseType;
  typedef Dune::Stuff::FunctionExpression<  DomainFieldImp, domainDim,
                                            RangeFieldImp, rangeDimRows > ExpressionFunctionType;
public:
  typedef typename ExpressionFunctionType::DomainType DomainType;
  typedef typename ExpressionFunctionType::RangeType  RangeType;

  NonparametricExpression(const std::string var,
                          const std::string expr,
                          const int oo = -1,
                          const std::string nm = ExpressionFunctionType::id());

  NonparametricExpression(const std::string var,
                          const std::vector< std::string > exprs,
                          const int oo = -1,
                          const std::string nm = ExpressionFunctionType::id());

  virtual std::string name() const;

  virtual int order() const;

  virtual void evaluate(const DomainType& x, RangeType& ret, const Parameter mu = Parameter()) const
    throw (Exception::wrong_parameter_type);

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

  AffineParametricDefault();

  /**
   * \attention This class takes ownership of aff!
   */
  AffineParametricDefault(const ParametricFunctionType* aff);

  virtual ~AffineParametricDefault();

  /**
   * \attention This class takes ownership of aff!
   */
  virtual void register_component(const ParametricFunctionType* aff) throw (Exception::this_does_not_make_any_sense);

  /**
   * \attention This class takes ownership of comp and coeff!
   */
  virtual void register_component(const ParametricFunctionType* comp, const ParameterFunctional* coeff)
    throw (Exception::this_does_not_make_any_sense);

  virtual unsigned int num_components() const;

  /**
   * \attention The ownership of the component remains with this class!
   */
  virtual const ParametricFunctionType* component(const int ii) const throw (Exception::requirements_not_met,
                                                                             Exception::index_out_of_range);

  /**
   * \attention The ownership of the coefficient remains with this class!
   */
  virtual const ParameterFunctional* coefficient(const int ii) const throw (Exception::requirements_not_met,
                                                                            Exception::index_out_of_range);

  virtual bool hasAffinePart() const;

  /**
   * \attention The ownership of affinePart() remains in this class!
   */
  virtual const ParametricFunctionType* affinePart() const throw(Exception::requirements_not_met);

  virtual int order() const;

  virtual void evaluate(const DomainType& x, RangeType& ret, const Parameter mu = Parameter()) const
    throw (Exception::wrong_parameter_type, Exception::this_does_not_make_any_sense);

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
