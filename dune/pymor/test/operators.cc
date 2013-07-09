// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifdef HAVE_CMAKE_CONFIG
  #include "cmake_config.h"
#elif defined (HAVE_CONFIG_H)
  #include <config.h>
#endif // HAVE_CMAKE_CONFIG

#include <utility>

#include <dune/stuff/test/test_common.hh>

#include <dune/common/float_cmp.hh>

#include <dune/pymor/common/exceptions.hh>
#include <dune/pymor/operators/default.hh>
#include <dune/pymor/parameters/base.hh>
#include <dune/pymor/parameters/functional.hh>
#include <dune/pymor/operators/dunedynamic.hh>
#if HAVE_EIGEN
  #include <dune/pymor/operators/eigen.hh>
#endif
#include <dune/pymor/operators.hh>


using namespace Dune;
using namespace Dune::Pymor;

typedef testing::Types<
                        std::pair< Dune::Pymor::Operators::DuneDynamic,
                                   Dune::Pymor::LA::DuneDynamicVector >
#if HAVE_EIGEN
                      , std::pair< Dune::Pymor::Operators::EigenDenseMatrix,
                                   Dune::Pymor::LA::EigenDenseVector >
                      , std::pair< Dune::Pymor::Operators::EigenRowMajorSparseMatrix,
                                   Dune::Pymor::LA::EigenDenseVector >
#endif // HAVE_EIGEN
                      > LinearOperatorTypes;


template< class TypePair >
struct LinearOperatorTest
  : public ::testing::Test
{
  void check() const
  {
    typedef typename TypePair::first_type   OperatorType;
    typedef typename TypePair::second_type  VectorType;
    const size_t dim = 2;
    VectorType* U = new VectorType(dim, 1.0);
    VectorType* V = new VectorType(dim, 1.0);
    OperatorType* op = createLinearOperator(OperatorType(), dim);
    if (!op->linear()) DUNE_PYMOR_THROW(PymorException, "");
    if (op->parametric()) DUNE_PYMOR_THROW(PymorException, "");
    if (op->parameter_type() != Parameter().type()) DUNE_PYMOR_THROW(PymorException, "");
    if (op->dim_source() != dim) DUNE_PYMOR_THROW(PymorException, "");
    if (op->dim_range() != dim) DUNE_PYMOR_THROW(PymorException, "");
    if (op->type_source() != U->type()) DUNE_PYMOR_THROW(PymorException, "");
    if (op->type_range() != U->type()) DUNE_PYMOR_THROW(PymorException, "");
    op->apply(U, V);
    if (V->dim() != op->dim_range()) DUNE_PYMOR_THROW(PymorException, "");
    if (!Dune::FloatCmp::eq(V->components({0})[0], 1.0) || !Dune::FloatCmp::eq(V->components({1})[0], 1.0))
      DUNE_PYMOR_THROW(PymorException, "");
    const double res = op->apply2(U, V);
    if (!Dune::FloatCmp::eq(res, 2.0)) DUNE_PYMOR_THROW(PymorException, "");
    delete V;
    delete op;
    delete U;
  }
}; // struct LinearOperatorTest


TYPED_TEST_CASE(LinearOperatorTest, LinearOperatorTypes);
TYPED_TEST(LinearOperatorTest, OPERATORS) {
  this->check();
}


template< class TypePair >
struct AffineparametricOperatorTest
  : public ::testing::Test
{
  void check() const
  {
    typedef typename TypePair::first_type   OperatorType;
    typedef typename TypePair::second_type  VectorType;
    typedef Operators::AffinelyDecomposedDefault< OperatorType > AffineparametricOperatorType;
    const Parameter mu = {"diffusion", {1.0, 1.0}};
    AffineparametricOperatorType op(mu.type());
    const size_t dim = 2;
    op.register_component(createLinearOperator(OperatorType(), dim));
    op.register_component(createLinearOperator(OperatorType(), dim),
                          new ParameterFunctional(mu.type(), "diffusion[0]"));
    op.register_component(createLinearOperator(OperatorType(), dim),
                          new ParameterFunctional(mu.type(), "diffusion[1]"));
    VectorType* U = new VectorType(dim, 1.0);
    VectorType* V = new VectorType(dim, 1.0);
    if (!op.linear()) DUNE_PYMOR_THROW(PymorException, "");
    if (!op.parametric()) DUNE_PYMOR_THROW(PymorException, "");
    if (op.parameter_type() != mu.type()) DUNE_PYMOR_THROW(PymorException, "");
    if (op.dim_source() != dim) DUNE_PYMOR_THROW(PymorException, "");
    if (op.dim_range() != dim) DUNE_PYMOR_THROW(PymorException, "");
    if (op.type_source() != U->type()) DUNE_PYMOR_THROW(PymorException, "");
    if (op.type_range() != U->type()) DUNE_PYMOR_THROW(PymorException, "");
    op.apply(U, V, mu);
    if (V->dim() != op.dim_range()) DUNE_PYMOR_THROW(PymorException, "");
    if (!Dune::FloatCmp::eq(V->components({0})[0], 3.0) || !Dune::FloatCmp::eq(V->components({1})[0], 3.0))
      DUNE_PYMOR_THROW(PymorException, "");
    const double res = op.apply2(U, V, mu);
    if (!Dune::FloatCmp::eq(res, 18.0)) DUNE_PYMOR_THROW(PymorException, "");
    delete V;
    delete U;
  }
}; // struct AffineparametricOperatorTest


TYPED_TEST_CASE(AffineparametricOperatorTest, LinearOperatorTypes);
TYPED_TEST(AffineparametricOperatorTest, OPERATORS) {
  this->check();
}


int main(int argc, char** argv)
{
//  try {
    test_init(argc, argv);
    return RUN_ALL_TESTS();
//  } catch (Dune::PymorException &e) {
//    std::cerr << Dune::Stuff::Common::colorStringRed("dune-pymor reported: ") << e << std::endl;
//  } catch (Dune::Exception &e) {
//    std::cerr << Dune::Stuff::Common::colorStringRed("Dune reported error: ") << e << std::endl;
//  } catch (...) {
//    std::cerr << Dune::Stuff::Common::colorStringRed("Unknown exception thrown!") << std::endl;
//  }
}
