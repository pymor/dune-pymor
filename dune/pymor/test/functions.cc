// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifdef HAVE_CMAKE_CONFIG
  #include "cmake_config.h"
#elif defined (HAVE_CONFIG_H)
  #include <config.h>
#endif // HAVE_CMAKE_CONFIG

#include <dune/stuff/test/test_common.hh>
#include <dune/stuff/function.hh>
#include <dune/stuff/common/float_cmp.hh>

#include <dune/pymor/common/exceptions.hh>
#include <dune/pymor/functions/default.hh>
//#include <dune/pymor/functions/checkerboard.hh>


using namespace Dune;
using namespace Dune::Pymor;


typedef testing::Types<
                        Dune::Stuff::FunctionExpression< double, 1, double, 1 >
                      , Dune::Stuff::FunctionCheckerboard< double, 1, double, 1 >
                      , Dune::Stuff::FunctionCheckerboard< double, 2, double, 1 >
                      , Dune::Stuff::FunctionCheckerboard< double, 3, double, 1 >
                      , Dune::Stuff::FunctionConstant< double, 1, double, 1 >
                      , Dune::Stuff::FunctionConstant< double, 2, double, 1 >
                      , Dune::Stuff::FunctionConstant< double, 3, double, 1 >
                      > NonparametricFunctions;


template< class NonparametricType >
struct NonparametricWrapperTest
  : public ::testing::Test
{
  typedef typename NonparametricType::DomainFieldType DomainFieldType;
  static const unsigned int dimDomain = NonparametricType::dimDomain;
  typedef typename NonparametricType::DomainType DomainType;
  typedef typename NonparametricType::RangeFieldType RangeFieldType;
  static const unsigned int dimRangeRows = NonparametricType::dimRangeRows;
  static const unsigned int dimRangeCols = NonparametricType::dimRangeCols;
  typedef Functions::NonparametricWrapper<  DomainFieldType,
                                            dimDomain,
                                            RangeFieldType,
                                            dimRangeRows,
                                            dimRangeCols > NonparametricWrapperFunctionType;

  void check() const
  {
    const NonparametricType* nonparametric = NonparametricType::create(NonparametricType::defaultSettings());
    const NonparametricWrapperFunctionType function(nonparametric);
    if (function.parametric()) DUNE_PYMOR_THROW(PymorException, "");
    const Parameter mu = Parameter();
    if (function.parameter_type() != mu.type()) DUNE_PYMOR_THROW(PymorException, "");
    const DomainType x(0);
    auto ret = function.evaluate(x);
    ret = function.evaluate(x, mu);
  }
}; // struct NonparametricWrapperTest


TYPED_TEST_CASE(NonparametricWrapperTest, NonparametricFunctions);
TYPED_TEST(NonparametricWrapperTest, Functions) {
  this->check();
}


TEST(NonparametricExpressionTest, Functions)
{
  typedef Functions::NonparametricExpression< double, 2, double, 1 > NonparametricExpressionType;
  NonparametricExpressionType function("x", "x[0] + x[1]");
  if (function.parametric()) DUNE_PYMOR_THROW(PymorException, "");
  const Parameter mu = Parameter();
  if (function.parameter_type() != mu.type()) DUNE_PYMOR_THROW(PymorException, "");
  const typename NonparametricExpressionType::DomainType x(0);
  auto ret = function.evaluate(x);
  if (!Dune::Stuff::Common::FloatCmp::eq(ret, typename NonparametricExpressionType::RangeType(0)))
    DUNE_PYMOR_THROW(PymorException, "");
}


TEST(AffineParametricDefaultTest, Functions)
{
  typedef Functions::AffineParametricDefault< double, 3, double, 1 > AffineParametricDefaultType;
  typedef Functions::NonparametricExpression< double, 3, double, 1 > NonparametricExpressionType;
  const Parameter mu = {"diffusion", {1.0, 1.0}};
  AffineParametricDefaultType function(new NonparametricExpressionType("x", "x[0]", 1));
  function.register_component(new NonparametricExpressionType("x", "x[1]", 1),
                              new ParameterFunctional(mu.type(), "diffusion[0]"));
  function.register_component(new NonparametricExpressionType("x", "x[2]", 1),
                              new ParameterFunctional(mu.type(), "diffusion[1]"));
  if (!function.hasAffinePart()) DUNE_PYMOR_THROW(PymorException, "");
  if (function.num_components() != 2) DUNE_PYMOR_THROW(PymorException, "");
  if (!function.parametric()) DUNE_PYMOR_THROW(PymorException, "");
  if (function.parameter_type() != mu.type()) DUNE_PYMOR_THROW(PymorException, "");
  if (function.order() != 1) DUNE_PYMOR_THROW(PymorException, "");
  typename AffineParametricDefaultType::DomainType x(1);
  double result = function.evaluate(x, mu);
  if (!Dune::FloatCmp::eq(result, 3.0)) DUNE_PYMOR_THROW(PymorException, "");

  AffineParametricDefaultType function2;
  function2.register_component(new NonparametricExpressionType("x", "x[0]", 1));
  if (!function2.hasAffinePart()) DUNE_PYMOR_THROW(PymorException, "");
  if (function2.num_components() != 0) DUNE_PYMOR_THROW(PymorException, "");
  if (function2.parametric()) DUNE_PYMOR_THROW(PymorException, "");
  if (function2.parameter_type() != Parameter().type()) DUNE_PYMOR_THROW(PymorException, "");
  if (function2.order() != 1) DUNE_PYMOR_THROW(PymorException, "");
  result = function2.evaluate(x);
  if (!Dune::FloatCmp::eq(result, 1.0)) DUNE_PYMOR_THROW(PymorException, "");

  AffineParametricDefaultType function3;
  function3.register_component(new NonparametricExpressionType("x", "x[1]", 1),
                               new ParameterFunctional(mu.type(), "diffusion[0]"));
  if (function3.hasAffinePart()) DUNE_PYMOR_THROW(PymorException, "");
  if (function3.num_components() != 1) DUNE_PYMOR_THROW(PymorException, "");
  if (!function3.parametric()) DUNE_PYMOR_THROW(PymorException, "");
  if (function3.parameter_type() != mu.type()) DUNE_PYMOR_THROW(PymorException, "");
  if (function3.order() != 1) DUNE_PYMOR_THROW(PymorException, "");
  result = function3.evaluate(x, mu);
  if (!Dune::FloatCmp::eq(result, 1.0)) DUNE_PYMOR_THROW(PymorException, "");
}


int main(int argc, char** argv)
{
  test_init(argc, argv);
  return RUN_ALL_TESTS();
}
