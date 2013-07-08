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

#include <dune/pymor/common/exceptions.hh>
#include <dune/pymor/functions/default.hh>

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
struct ParametricDefaultTest
  : public ::testing::Test
{
  typedef typename NonparametricType::DomainFieldType DomainFieldType;
  static const unsigned int dimDomain = NonparametricType::dimDomain;
  typedef typename NonparametricType::DomainType DomainType;
  typedef typename NonparametricType::RangeFieldType RangeFieldType;
  static const unsigned int dimRangeRows = NonparametricType::dimRangeRows;
  static const unsigned int dimRangeCols = NonparametricType::dimRangeCols;
  typedef Functions::ParametricDefault< DomainFieldType, dimDomain,
                                        RangeFieldType, dimRangeRows, dimRangeCols > DefaultParametricFunctionType;

  void check() const
  {
    const NonparametricType* nonparametric = NonparametricType::create(NonparametricType::defaultSettings());
    const DefaultParametricFunctionType function(nonparametric);
    if (function.parametric()) DUNE_PYMOR_THROW(PymorException, "");
    const Parameter mu = Parameter();
    if (function.parameter_type() != mu.type()) DUNE_PYMOR_THROW(PymorException, "");
    const DomainType x(0);
    auto ret = function.evaluate(x);
    ret = function.evaluate(x, mu);
  }
}; // struct ParametricDefaultTest


TYPED_TEST_CASE(ParametricDefaultTest, NonparametricFunctions);
TYPED_TEST(ParametricDefaultTest, FUNCTIONS) {
  this->check();
}


int main(int argc, char** argv)
{
  test_init(argc, argv);
  return RUN_ALL_TESTS();
}
