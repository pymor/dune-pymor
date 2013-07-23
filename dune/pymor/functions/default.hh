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

#include <dune/pymor/common/exceptions.hh>
#include <dune/pymor/parameters/base.hh>
#include <dune/pymor/parameters/functional.hh>
#include "interfaces.hh"

namespace Dune {
namespace Pymor {
namespace Function {


template< class DomainFieldImp, int domainDim, class RangeFieldImp, int rangeDimRows, int rangeDimCols = 1 >
class NonparametricDefault
  : public ParametricFunctionInterface< DomainFieldImp, domainDim, RangeFieldImp, rangeDimRows, rangeDimCols >
{
  typedef ParametricFunctionInterface< DomainFieldImp, domainDim, RangeFieldImp, rangeDimRows, rangeDimCols > BaseType;
public:
  typedef NonparametricDefault< DomainFieldImp, domainDim, RangeFieldImp, rangeDimRows, rangeDimCols >        ThisType;
  typedef typename BaseType::NonparametricType    NonparametricType;
  typedef typename NonparametricType::DomainType  DomainType;
  typedef typename NonparametricType::RangeType   RangeType;

  /**
   * \attention This class takes ownership of nonparametric_ptr (in the sense, that you must not delete it manually)!
   */
  NonparametricDefault(const NonparametricType* nonparametric_ptr);

  NonparametricDefault(const std::shared_ptr< const NonparametricType > nonparametric_ptr);

  virtual std::string name() const;

  virtual int order() const;

  virtual void evaluate(const DomainType& x, RangeType& ret, const Parameter mu = Parameter()) const
    throw (Exception::wrong_parameter_type);

  using BaseType::evaluate;

  virtual bool affinely_decomposable() const;

  virtual bool has_affine_part() const;

  virtual std::shared_ptr< const NonparametricType > affine_part() const;

  virtual unsigned int num_components() const;

private:
  std::shared_ptr< const NonparametricType > nonparametric_;
}; // class NonparametricDefault


template< class DomainFieldImp, int domainDim, class RangeFieldImp, int rangeDim >
class NonparametricExpression
  : public ParametricFunctionInterface< DomainFieldImp, domainDim, RangeFieldImp, rangeDim, 1 >
{
  typedef ParametricFunctionInterface< DomainFieldImp, domainDim, RangeFieldImp, rangeDim, 1 > BaseType;
  typedef Dune::Stuff::FunctionExpression<  DomainFieldImp, domainDim,
                                            RangeFieldImp, rangeDim > ExpressionFunctionType;
public:
  typedef typename BaseType::NonparametricType        NonparametricType;
  typedef typename ExpressionFunctionType::DomainType DomainType;
  typedef typename ExpressionFunctionType::RangeType  RangeType;

  NonparametricExpression(const std::string var,
                          const std::string expr,
                          const int oo = -1,
                          const std::string nm = ExpressionFunctionType::static_id());

  NonparametricExpression(const std::string var,
                          const std::vector< std::string > exprs,
                          const int oo = -1,
                          const std::string nm = ExpressionFunctionType::static_id());

  virtual std::string name() const;

  virtual int order() const;

  virtual void evaluate(const DomainType& x, RangeType& ret, const Parameter mu = Parameter()) const
    throw (Exception::wrong_parameter_type);

  using BaseType::evaluate;

  virtual bool affinely_decomposable() const;

  virtual bool has_affine_part() const;

  virtual std::shared_ptr< const NonparametricType > affine_part() const;

  virtual unsigned int num_components() const;

private:
  std::shared_ptr< const ExpressionFunctionType > expressionFunction_;
}; // class NonparametricExpression


template< class DomainFieldImp, int domainDim, class RangeFieldImp, int rangeDimRows, int rangeDimCols = 1 >
class AffinelyDecomposableDefault
  : public ParametricFunctionInterface< DomainFieldImp, domainDim, RangeFieldImp, rangeDimRows, rangeDimCols >
{
  typedef ParametricFunctionInterface<  DomainFieldImp, domainDim,
                                        RangeFieldImp, rangeDimRows, rangeDimCols > BaseType;
  static const std::string my_name_;
public:
  typedef typename BaseType::NonparametricType  NonparametricType;
  typedef typename BaseType::DomainType         DomainType;
  typedef typename BaseType::RangeType          RangeType;

  AffinelyDecomposableDefault(const std::string nm = my_name_,
                              const int ord = -1);

  /**
   * \attention This class takes ownership of aff_ptr (in the sense, that you must not delete it manually)!
   */
  AffinelyDecomposableDefault(const NonparametricType* aff_ptr,
                              const std::string nm = my_name_,
                              const int ord = 0);

  AffinelyDecomposableDefault(const std::shared_ptr< const NonparametricType > aff_ptr,
                              const std::string nm = my_name_,
                              const int ord = 0);

  /**
   * \attention This class takes ownership of comp_ptr and coeff_ptr (in the sense, that you must not delete it
   *            manually)!
   */
  AffinelyDecomposableDefault(const NonparametricType* comp_ptr,
                              const ParameterFunctional* coeff_ptr,
                              const std::string nm = my_name_,
                              const int ord = 0);

  /**
   * \attention This class takes ownership of comp_ptr (in the sense, that you must not delete it manually)!
   */
  AffinelyDecomposableDefault(const NonparametricType* comp_ptr,
                              const std::shared_ptr< const ParameterFunctional > coeff_ptr,
                              const std::string nm = my_name_,
                              const int ord = 0);

  /**
   * \attention This class takes ownership of coeff_ptr (in the sense, that you must not delete it manually)!
   */
  AffinelyDecomposableDefault(const std::shared_ptr< const NonparametricType > comp_ptr,
                              const ParameterFunctional* coeff_ptr,
                              const std::string nm = my_name_,
                              const int ord = 0);

  AffinelyDecomposableDefault(const std::shared_ptr< const NonparametricType > comp_ptr,
                              const std::shared_ptr< const ParameterFunctional > coeff_ptr,
                              const std::string nm = my_name_,
                              const int ord = 0);

  /**
   * \attention This class takes ownership of aff_ptr (in the sense, that you must not delete it manually)!
   */
  void register_affine_part(const NonparametricType* aff_ptr)
    throw (Exception::this_does_not_make_any_sense);

  void register_affine_part(const std::shared_ptr< const NonparametricType > aff_ptr)
    throw (Exception::this_does_not_make_any_sense);

  /**
   * \attention This class takes ownership of comp_ptr and coeff_ptr (in the sense, that you must not delete it
   *            manually)!
   */
  void register_component(const NonparametricType* comp_ptr, const ParameterFunctional* coeff_ptr);

  /**
   * \attention This class takes ownership of comp_ptr (in the sense, that you must not delete it manually)!
   */
  void register_component(const NonparametricType* comp_ptr,
                          const std::shared_ptr< const ParameterFunctional > coeff_ptr);

  /**
   * \attention This class takes ownership of coeff_ptr (in the sense, that you must not delete it manually)!
   */
  void register_component(const std::shared_ptr< const NonparametricType > comp_ptr,
                          const ParameterFunctional* coeff_ptr);

  void register_component(const std::shared_ptr< const NonparametricType > comp_ptr,
                          const std::shared_ptr< const ParameterFunctional > coeff_ptr);

  virtual int order() const;

  virtual std::string name() const;

  virtual void evaluate(const DomainType& x, RangeType& ret, const Parameter mu = Parameter()) const
    throw (Exception::wrong_parameter_type, Exception::requirements_not_met);

  using BaseType::evaluate;

  virtual bool affinely_decomposable() const;

  virtual bool has_affine_part() const;

  virtual std::shared_ptr< const NonparametricType > affine_part() const throw(Exception::requirements_not_met);

  virtual unsigned int num_components() const;

  virtual std::shared_ptr< const NonparametricType > component(const int qq) const
    throw (Exception::requirements_not_met, Exception::index_out_of_range);

  virtual std::shared_ptr< const ParameterFunctional > coefficient(const int qq) const
    throw (Exception::requirements_not_met, Exception::index_out_of_range);

public:
  std::string name_;
  int order_;
  unsigned int num_components_;
  bool hasAffinePart_;
  std::vector< std::shared_ptr< const NonparametricType > > components_;
  std::vector< std::shared_ptr< const ParameterFunctional > > coefficients_;
  std::shared_ptr< const NonparametricType > affinePart_;
}; // class AffinelyDecomposableDefault

} // namespace Function
} // namespace Pymor
} // namespace Dune

#endif // DUNE_PYMOR_FUNCTIONS_DEFAULT_HH
