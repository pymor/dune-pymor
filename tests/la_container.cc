// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifdef HAVE_CMAKE_CONFIG
  #include "cmake_config.h"
#elif defined (HAVE_CONFIG_H)
  #include <config.h>
#endif // HAVE_CMAKE_CONFIG

#include <type_traits>
#include <memory>

#include <dune/stuff/test/test_common.hh>

#include <dune/pymor/common/exceptions.hh>
#include <dune/pymor/la/container/interfaces.hh>
#include <dune/pymor/la/container/dunedynamic.hh>
#include <dune/pymor/la/container/eigen.hh>

using namespace Dune;
using namespace Dune::Pymor;

static const size_t dim = 4;

typedef testing::Types<
                        Dune::Pymor::LA::DuneDynamicVector< double >
#if HAVE_EIGEN
                      , Dune::Pymor::LA::EigenDenseVector< double >
#endif
                      > VectorTypes;

typedef testing::Types<
                        Dune::Pymor::LA::DuneDynamicMatrix< double >
#if HAVE_EIGEN
                      , Dune::Pymor::LA::EigenRowMajorSparseMatrix< double >
#endif
                      > MatrixTypes;

typedef testing::Types<
                        Dune::Pymor::LA::DuneDynamicVector< double >
                      , Dune::Pymor::LA::DuneDynamicMatrix< double >
#if HAVE_EIGEN
                      , Dune::Pymor::LA::EigenDenseVector< double >
                      , Dune::Pymor::LA::EigenRowMajorSparseMatrix< double >
#endif
                      > ContainerTypes;


template< class ContainerImp >
struct ContainerTest
  : public ::testing::Test
{
  void check() const
  {
    // static tests
    typedef typename ContainerImp::Traits     Traits;
    // * of the traits
    typedef typename Traits::derived_type     T_derived_type;
    static_assert(std::is_same< ContainerImp, T_derived_type >::value, "derived_type has to be the correct Type!");
    typedef typename Traits::ScalarType       T_ScalarType;
    // * of the container as itself (aka the derived type)
    typedef typename ContainerImp::ScalarType D_ScalarType;
    // * of the container as the interface
    typedef typename LA::ContainerInterface< Traits > InterfaceType;
    typedef typename InterfaceType::derived_type      I_derived_type;
    typedef typename InterfaceType::ScalarType        I_ScalarType;
    // dynamic tests
    // * of the container as itself (aka the derived type)
    ContainerImp DUNE_UNUSED(d_empty);
    ContainerImp d_by_size = LA::createContainer(ContainerImp(), dim);
    //ContainerImp d_copy_constructor(d_by_size); // <-- this is not allowed!
    //ContainerImp d_copy_assignment = d_by_size; // <-- this is not allowed!
    ContainerImp d_deep_copy = d_by_size.copy();
    d_by_size.scal(D_ScalarType(1));
    d_by_size.axpy(D_ScalarType(1), d_deep_copy);
    if (!d_by_size.has_equal_shape(d_deep_copy)) DUNE_PYMOR_THROW(PymorException, "");
    // * of the container as the interface
    InterfaceType& i_by_size = static_cast< InterfaceType& >(d_by_size);
    ContainerImp i_deep_copy = i_by_size.copy();
    i_by_size.scal(I_ScalarType(1));
    i_by_size.axpy(I_ScalarType(1), i_deep_copy);
  }
}; // struct ContainerTest


TYPED_TEST_CASE(ContainerTest, ContainerTypes);
TYPED_TEST(ContainerTest, LA_CONTAINER) {
  this->check();
}


template< class VectorImp >
struct VectorTest
  : public ::testing::Test
{
  void check() const
  {
    // static tests
    typedef typename VectorImp::Traits      Traits;
    // * of the traits
    typedef typename Traits::derived_type   T_derived_type;
    static_assert(std::is_same< VectorImp,  T_derived_type >::value, "derived_type has to be the correct Type!");
    typedef typename Traits::ScalarType     T_ScalarType;
    // * of the vector as itself (aka the derived type)
    typedef typename VectorImp::ScalarType  D_ScalarType;
    // * of the vector as the interface
    typedef typename LA::VectorInterface< Traits >  InterfaceType;
    typedef typename InterfaceType::derived_type    I_derived_type;
    typedef typename InterfaceType::ScalarType      I_ScalarType;
    // dynamic tests
    // * of the vector as itself (aka the derived type)
    VectorImp d_by_size(dim);
    VectorImp d_by_size_and_value(dim, D_ScalarType(1));
    unsigned int DUNE_UNUSED(d_dim) = d_by_size.dim();
    bool d_almost_equal = d_by_size.almost_equal(d_by_size);
    if (!d_almost_equal) DUNE_PYMOR_THROW(PymorException, "");
    d_by_size_and_value.scal(D_ScalarType(0));
    if (!d_by_size_and_value.almost_equal(d_by_size)) DUNE_PYMOR_THROW(PymorException, "");
    D_ScalarType d_dot = d_by_size.dot(d_by_size_and_value);
    if (!Dune::FloatCmp::eq(d_dot, D_ScalarType(0))) DUNE_PYMOR_THROW(PymorException, d_dot);
    D_ScalarType d_l1_norm = d_by_size.l1_norm();
    if (!Dune::FloatCmp::eq(d_l1_norm, D_ScalarType(0))) DUNE_PYMOR_THROW(PymorException, d_l1_norm);
    D_ScalarType d_l2_norm = d_by_size.l2_norm();
    if (!Dune::FloatCmp::eq(d_l2_norm, D_ScalarType(0))) DUNE_PYMOR_THROW(PymorException, d_l2_norm);
    D_ScalarType d_sup_norm = d_by_size.sup_norm();
    if (!Dune::FloatCmp::eq(d_sup_norm ,D_ScalarType(0))) DUNE_PYMOR_THROW(PymorException, d_sup_norm);
    VectorImp d_ones(dim, D_ScalarType(1));
    std::vector< D_ScalarType > d_amax = d_ones.amax();
    if (int(d_amax[0]) != 0 || !Dune::FloatCmp::eq(d_amax[1], D_ScalarType(1))) DUNE_PYMOR_THROW(PymorException, "");
    d_ones.add(d_by_size, d_by_size_and_value);
    if (!d_by_size_and_value.almost_equal(d_ones)) DUNE_PYMOR_THROW(PymorException, "");
    VectorImp d_added = d_ones.add(d_by_size);
    if (!d_added.almost_equal(d_by_size_and_value)) DUNE_PYMOR_THROW(PymorException, "");
    d_added.iadd(d_by_size);
    if (!d_added.almost_equal(d_ones)) DUNE_PYMOR_THROW(PymorException, "");
    d_ones.sub(d_by_size, d_by_size_and_value);
    if (!d_by_size_and_value.almost_equal(d_ones)) DUNE_PYMOR_THROW(PymorException, "");
    VectorImp d_subtracted = d_ones.sub(d_by_size);
    if (!d_subtracted.almost_equal(d_by_size_and_value)) DUNE_PYMOR_THROW(PymorException, "");
    d_subtracted.isub(d_by_size);
    if (!d_subtracted.almost_equal(d_ones)) DUNE_PYMOR_THROW(PymorException, "");
    // * of the vector as the interface
    VectorImp tmp1(dim);
    VectorImp tmp2(dim, D_ScalarType(1));
    InterfaceType& i_by_size = static_cast< InterfaceType& >(tmp1);
    InterfaceType& i_by_size_and_value = static_cast< InterfaceType& >(tmp2);
    unsigned int DUNE_UNUSED(i_dim) = i_by_size.dim();
    bool i_almost_equal = i_by_size.almost_equal(i_by_size);
    if (!i_almost_equal) DUNE_PYMOR_THROW(PymorException, "");
    i_by_size_and_value.scal(I_ScalarType(0));
    if (!i_by_size_and_value.almost_equal(i_by_size)) DUNE_PYMOR_THROW(PymorException, "");
    I_ScalarType i_dot = i_by_size.dot(i_by_size_and_value);
    if (!Dune::FloatCmp::eq(i_dot, I_ScalarType(0))) DUNE_PYMOR_THROW(PymorException, i_dot);
    I_ScalarType i_l1_norm = i_by_size.l1_norm();
    if (!Dune::FloatCmp::eq(i_l1_norm, I_ScalarType(0))) DUNE_PYMOR_THROW(PymorException, i_l1_norm);
    I_ScalarType i_l2_norm = i_by_size.l2_norm();
    if (!Dune::FloatCmp::eq(i_l2_norm, I_ScalarType(0))) DUNE_PYMOR_THROW(PymorException, i_l2_norm);
    I_ScalarType i_sup_norm = i_by_size.sup_norm();
    if (!Dune::FloatCmp::eq(i_sup_norm ,I_ScalarType(0))) DUNE_PYMOR_THROW(PymorException, i_sup_norm);
    VectorImp i_ones(dim, I_ScalarType(1));
    std::vector< I_ScalarType > i_amax = i_ones.amax();
    if (int(i_amax[0]) != 0 || !Dune::FloatCmp::eq(i_amax[1], I_ScalarType(1))) DUNE_PYMOR_THROW(PymorException, "");
    i_ones.add(i_by_size, i_by_size_and_value);
    if (!i_by_size_and_value.almost_equal(i_ones)) DUNE_PYMOR_THROW(PymorException, "");
    VectorImp i_added = i_ones.add(i_by_size);
    if (!i_added.almost_equal(i_by_size_and_value)) DUNE_PYMOR_THROW(PymorException, "");
    i_added.iadd(i_by_size);
    if (!i_added.almost_equal(i_ones)) DUNE_PYMOR_THROW(PymorException, "");
    i_ones.sub(i_by_size, i_by_size_and_value);
    if (!i_by_size_and_value.almost_equal(i_ones)) DUNE_PYMOR_THROW(PymorException, "");
    VectorImp i_subtracted = i_ones.sub(i_by_size);
    if (!i_subtracted.almost_equal(i_by_size_and_value)) DUNE_PYMOR_THROW(PymorException, "");
    i_subtracted.isub(i_by_size);
    if (!i_subtracted.almost_equal(i_ones)) DUNE_PYMOR_THROW(PymorException, "");
  }
}; // struct VectorTest


TYPED_TEST_CASE(VectorTest, VectorTypes);
TYPED_TEST(VectorTest, LA_CONTAINER) {
  this->check();
}


int main(int argc, char** argv)
{
  try {
    test_init(argc, argv);
    return RUN_ALL_TESTS();
  } catch (Dune::PymorException& e) {
    std::cerr << Dune::Stuff::Common::colorStringRed("dune-pymor reported: ") << e << std::endl;
  } catch (Dune::Exception& e) {
    std::cerr << Dune::Stuff::Common::colorStringRed("Dune reported error: ") << e << std::endl;
  } catch (...) {
    std::cerr << Dune::Stuff::Common::colorStringRed("Unknown exception thrown!") << std::endl;
  }
}