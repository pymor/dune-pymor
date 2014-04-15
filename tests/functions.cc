// This file is part of the dune-pymor project:
//   https://github.com/pymor/dune-pymor
// Copyright Holders: Stephan Rave, Felix Schindler
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#include <dune/stuff/test/test_common.hh>

#include <dune/stuff/functions.hh>
#include <dune/stuff/common/float_cmp.hh>

#include <dune/pymor/common/exceptions.hh>
#include <dune/pymor/functions/default.hh>


#if HAVE_DUNE_GRID
using namespace Dune;
using namespace Dune::Pymor;


typedef testing::Types<
                        Dune::Stuff::Function::Expression< double, 1, double, 1 >
                      , Dune::Stuff::Function::Expression< double, 2, double, 1 >
                      , Dune::Stuff::Function::Expression< double, 3, double, 1 >
                      , Dune::Stuff::Function::Expression< double, 1, double, 2 >
                      , Dune::Stuff::Function::Expression< double, 2, double, 2 >
                      , Dune::Stuff::Function::Expression< double, 3, double, 2 >
                      , Dune::Stuff::Function::Expression< double, 1, double, 3 >
                      , Dune::Stuff::Function::Expression< double, 2, double, 3 >
                      , Dune::Stuff::Function::Expression< double, 3, double, 3 >
                      , Dune::Stuff::Function::Checkerboard< double, 1, double, 1 >
                      , Dune::Stuff::Function::Checkerboard< double, 2, double, 1 >
                      , Dune::Stuff::Function::Checkerboard< double, 3, double, 1 >
                      , Dune::Stuff::Function::Constant< double, 1, double, 1 >
                      , Dune::Stuff::Function::Constant< double, 2, double, 1 >
                      , Dune::Stuff::Function::Constant< double, 3, double, 1 >
                      > NonparametricFunctions;


template< class NonparametricType >
struct NonparametricDefaultTest
  : public ::testing::Test
{
  typedef typename NonparametricType::DomainFieldType DomainFieldType;
  static const unsigned int dimDomain = NonparametricType::dimDomain;
  typedef typename NonparametricType::DomainType DomainType;
  typedef typename NonparametricType::RangeFieldType RangeFieldType;
  static const unsigned int dimRangeRows = NonparametricType::dimRangeRows;
  static const unsigned int dimRangeCols = NonparametricType::dimRangeCols;
  typedef Function::NonparametricDefault< DomainFieldType, dimDomain,
                                          RangeFieldType, dimRangeRows, dimRangeCols > FunctionType;

  void check() const
  {
    const FunctionType function(NonparametricType::create());
    if (function.parametric()) DUNE_PYMOR_THROW(PymorException, "");
    if (!function.parameter_type().empty()) DUNE_PYMOR_THROW(PymorException, "");
    const DomainType x(0);
    auto ret = function.evaluate(x);
    ret = function.evaluate(x, Parameter());
  }
}; // struct NonparametricWrapperTest


TYPED_TEST_CASE(NonparametricDefaultTest, NonparametricFunctions);
TYPED_TEST(NonparametricDefaultTest, FUNCTIONS) {
  this->check();
}


TEST(NonparametricExpressionTest, FUNCTIONS)
{
  typedef Function::NonparametricExpression< double, 2, double, 1 > NonparametricExpressionType;
  NonparametricExpressionType function("x", "x[0] + x[1]");
  if (function.parametric()) DUNE_PYMOR_THROW(PymorException, "");
  if (!function.parameter_type().empty()) DUNE_PYMOR_THROW(PymorException, "");
  const typename NonparametricExpressionType::DomainType x(0);
  auto ret = function.evaluate(x);
  ret = function.evaluate(x, Parameter());
  if (!Dune::Stuff::Common::FloatCmp::eq(ret, typename NonparametricExpressionType::RangeType(0)))
    DUNE_PYMOR_THROW(PymorException, "");
} // NonparametricExpressionTest


TEST(AffineParametricDefaultTest, FUNCTIONS)
{
  typedef Function::AffinelyDecomposableDefault< double, 3, double, 1 > AffinelyDecomposableDefaultType;
  typedef Stuff::FunctionExpression< double, 3, double, 1 > NonparametricExpressionType;
  const Parameter mu = {"diffusion", {1.0, 1.0}};
  AffinelyDecomposableDefaultType function(new NonparametricExpressionType("x", "x[0]", 1));
  function.register_component(new NonparametricExpressionType("x", "x[1]", 1),
                              new ParameterFunctional(mu.type(), "diffusion[0]"));
  function.register_component(new NonparametricExpressionType("x", "x[2]", 1),
                              new ParameterFunctional(mu.type(), "diffusion[1]"));
  if (!function.has_affine_part()) DUNE_PYMOR_THROW(PymorException, "");
  if (function.num_components() != 2) DUNE_PYMOR_THROW(PymorException, function.num_components());
  if (!function.parametric()) DUNE_PYMOR_THROW(PymorException, "");
  if (function.parameter_type() != mu.type()) DUNE_PYMOR_THROW(PymorException, function.parameter_type());
  if (function.order() != 1) DUNE_PYMOR_THROW(PymorException, function.order());
  typename AffinelyDecomposableDefaultType::DomainType x(1);
  double result = function.evaluate(x, mu);
  if (!Dune::FloatCmp::eq(result, 3.0)) DUNE_PYMOR_THROW(PymorException, "");

  AffinelyDecomposableDefaultType function2;
  function2.register_affine_part(new NonparametricExpressionType("x", "x[0]", 1));
  if (!function2.has_affine_part()) DUNE_PYMOR_THROW(PymorException, "");
  if (function2.num_components() != 0) DUNE_PYMOR_THROW(PymorException, function2.num_components());
  if (function2.parametric()) DUNE_PYMOR_THROW(PymorException, "");
  if (!function2.parameter_type().empty()) DUNE_PYMOR_THROW(PymorException, "");
  if (function2.order() != 1) DUNE_PYMOR_THROW(PymorException, function2.order());
  result = function2.evaluate(x);
  if (!Dune::FloatCmp::eq(result, 1.0)) DUNE_PYMOR_THROW(PymorException, "");

  AffinelyDecomposableDefaultType function3;
  function3.register_component(new NonparametricExpressionType("x", "x[1]", 1),
                               new ParameterFunctional(mu.type(), "diffusion[0]"));
  if (function3.has_affine_part()) DUNE_PYMOR_THROW(PymorException, "");
  if (function3.num_components() != 1) DUNE_PYMOR_THROW(PymorException, function3.num_components());
  if (!function3.parametric()) DUNE_PYMOR_THROW(PymorException, "");
  if (function3.parameter_type() != mu.type()) DUNE_PYMOR_THROW(PymorException, function3.parameter_type());
  if (function3.order() != 1) DUNE_PYMOR_THROW(PymorException, function3.order());
  result = function3.evaluate(x, mu);
  if (!Dune::FloatCmp::eq(result, 1.0)) DUNE_PYMOR_THROW(PymorException, "");
} // AffineParametricDefaultTest

#endif // HAVE_DUNE_GRID

int main(int argc, char** argv)
{
  try {
    test_init(argc, argv);
    return RUN_ALL_TESTS();
  } catch (Dune::PymorException &e) {
    std::cerr << Dune::Stuff::Common::colorStringRed("dune-pymor reported: ") << e << std::endl;
  } catch (Dune::Exception &e) {
    std::cerr << Dune::Stuff::Common::colorStringRed("Dune reported error: ") << e << std::endl;
  } catch (...) {
    std::cerr << Dune::Stuff::Common::colorStringRed("Unknown exception thrown!") << std::endl;
  }
}
