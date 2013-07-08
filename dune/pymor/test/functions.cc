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
  typedef typename NonparametricType::RangeFieldType RangeFieldType;
  static const unsigned int dimRangeRows = NonparametricType::dimRangeRows;
  static const unsigned int dimRangeCols = NonparametricType::dimRangeCols;
  typedef Functions::ParametricDefault< DomainFieldType, dimDomain,
                                        RangeFieldType, dimRangeRows, dimRangeCols > DefaultParametricFunctionType;

  void check() const
  {
    const NonparametricType* nonparametric = NonparametricType::creat(NonparametricType::defaultSettings());
    const DefaultParametricFunctionType function(nonparametric);
  }
}; // struct ParametricDefaultTest


//template< class FunctionType >
//struct ParametricFunctionTest
//  : public ::testing::Test
//{
//  typedef typename FunctionType::DomainFieldType  DomainFieldType;
//  static const int                                dimDomain = FunctionType::dimDomain;
//  typedef typename FunctionType::DomainType       DomainType;
//  typedef typename FunctionType::RangeFieldType   RangeFieldType;
//  static const int                                dimRangeRows = FunctionType::dimRangeRows;
//  static const int                                dimRangeCols = FunctionType::dimRangeCols;
//  typedef typename FunctionType::RangeType        RangeType;
//  typedef typename FunctionType::ParamFieldType   ParamFieldType;
//  static const int                                maxParamDim = FunctionType::maxParamDim;
//  typedef typename FunctionType::ParamType        ParamType;

//  typedef FunctionFixedParameter< DomainFieldType, dimDomain, RangeFieldType, dimRangeRows, dimRangeCols >
//    FixedParameterFunctionType;

//  void check() const
//  {
//    DomainType x(1);
//    RangeType ret(0);
//    const std::shared_ptr< const FunctionType >
//        function(FunctionType::create(FunctionType::defaultSettings()));
//    const std::string DUNE_UNUSED(name) = function->name();
//    const int DUNE_UNUSED(order) = function->order();
//    const bool parametric = function->parametric();
//    assert(parametric);
//    const size_t paramSize = function->paramSize();
//    assert(paramSize > 0);
//    const std::vector< ParamType >& paramRange = function->paramRange();
//    assert(paramRange.size() == 2);
//    assert(paramRange[0].size() == paramSize);
//    assert(paramRange[1].size() == paramSize);
//    const std::vector< std::string >& paramExplanation = function->paramExplanation();
//    assert(paramExplanation.size() == paramSize);
//    function->evaluate(x, paramRange[0], ret);
//    const FixedParameterFunctionType fixedFunction(function, paramRange[0]);
//    const std::string DUNE_UNUSED(f_name) = fixedFunction.name();
//    const int DUNE_UNUSED(f_order) = fixedFunction.order();
//    const bool f_parametric = fixedFunction.parametric();
//    assert(!f_parametric);
//    fixedFunction.evaluate(x, ret);
//  }
//}; // struct ParametricFunctionTest


//template< class FunctionType >
//struct AffineParametricFunctionTest
//  : public ::testing::Test
//{
//  typedef typename FunctionType::DomainFieldType  DomainFieldType;
//  static const int                                dimDomain = FunctionType::dimDomain;
//  typedef typename FunctionType::DomainType       DomainType;
//  typedef typename FunctionType::RangeFieldType   RangeFieldType;
//  static const int                                dimRangeRows = FunctionType::dimRangeRows;
//  static const int                                dimRangeCols = FunctionType::dimRangeCols;
//  typedef typename FunctionType::RangeType        RangeType;
//  typedef typename FunctionType::ParamFieldType   ParamFieldType;
//  static const int                                maxParamDim = FunctionType::maxParamDim;
//  typedef typename FunctionType::ParamType        ParamType;
//  typedef typename FunctionType::ComponentType    ComponentType;
//  typedef typename FunctionType::CoefficientType  CoefficientType;

//  void check() const
//  {
//    DomainType x(1);
//    RangeType ret(0);
//    const std::shared_ptr< const FunctionType >
//        function(FunctionType::create(FunctionType::defaultSettings()));
//    const std::string DUNE_UNUSED(name) = function->name();
//    const int DUNE_UNUSED(order) = function->order();
//    const bool parametric = function->parametric();
//    assert(parametric);
//    const size_t paramSize = function->paramSize();
//    assert(paramSize > 0);
//    const std::vector< ParamType >& paramRange = function->paramRange();
//    assert(paramRange.size() == 2);
//    assert(paramRange[0].size() == paramSize);
//    assert(paramRange[1].size() == paramSize);
//    const std::vector< std::string >& paramExplanation = function->paramExplanation();
//    assert(paramExplanation.size() == paramSize);
//    function->evaluate(x, paramRange[0], ret);
//    const bool affineParametric = function->affineparametric();
//    assert(affineParametric);
//    const std::vector< std::shared_ptr< const ComponentType > >& components = function->components();
//    const std::vector< std::shared_ptr< const CoefficientType > >& coefficients = function->coefficients();
//    assert(components.size() == coefficients.size());
//    const bool hasAffinePart = function->hasAffinePart();
//    if (hasAffinePart)
//      const std::shared_ptr< const ComponentType >& DUNE_UNUSED(affinePart) = function->affinePart();
//  }
//}; // struct AffineParametricFunctionTest


//template< class FunctionType >
//struct TimedependentFunctionTest
//  : public ::testing::Test
//{
//  typedef typename FunctionType::DomainFieldType  DomainFieldType;
//  static const int                                dimDomain = FunctionType::dimDomain;
//  typedef typename FunctionType::DomainType       DomainType;
//  typedef typename FunctionType::RangeFieldType   RangeFieldType;
//  static const int                                dimRangeRows = FunctionType::dimRangeRows;
//  static const int                                dimRangeCols = FunctionType::dimRangeCols;
//  typedef typename FunctionType::RangeType        RangeType;

//  typedef FunctionFixedTime< DomainFieldType, dimDomain, RangeFieldType, dimRangeRows, dimRangeCols >
//      FixedTimeFunctionType;

//  void check() const
//  {
//    DomainType x(1);
//    RangeType ret(0);
//    double t(0);
//    const std::shared_ptr< const FunctionType >
//        function(FunctionType::create(FunctionType::defaultSettings()));
//    const std::string DUNE_UNUSED(name) = function->name();
//    const int DUNE_UNUSED(order) = function->order();
//    function->evaluate(x, t, ret);
//    const FixedTimeFunctionType fixedFunction(function, 0);
//    const std::string DUNE_UNUSED(f_name) = fixedFunction.name();
//    const int DUNE_UNUSED(f_order) = fixedFunction.order();
//    fixedFunction.evaluate(x, ret);
//  }
//}; // struct TimedependentFunctionTest


TYPED_TEST_CASE(ParametricDefaultTest, NonparametricFunctions);
TYPED_TEST(ParametricDefaultTest, FUNCTIONS) {
  this->check();
}


//TYPED_TEST_CASE(ParametricFunctionTest, AffineParametricFunctions);
//TYPED_TEST(ParametricFunctionTest, ParametricFunction) {
//  this->check();
//}


//TYPED_TEST_CASE(AffineParametricFunctionTest, AffineParametricFunctions);
//TYPED_TEST(AffineParametricFunctionTest, AffineParametricFunction) {
//  this->check();
//}


//TYPED_TEST_CASE(TimedependentFunctionTest, TimedependentFunctions);
//TYPED_TEST(TimedependentFunctionTest, TimedependentFunction) {
//  this->check();
//}


int main(int argc, char** argv)
{
  test_init(argc, argv);
  return RUN_ALL_TESTS();
}
