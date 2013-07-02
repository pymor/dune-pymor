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

#include <memory>

#include <dune/pymor/la/container/dunedynamic.hh>

//#if HAVE_EIGEN
//  #include <dune/pymor/la/container/eigen.hh>
//#endif // HAVE_EIGEN

typedef testing::Types< Dune::Pymor::LA::DuneDynamicVector
//#if HAVE_EIGEN
//                        Dune::Pymor::LA::EigenDenseVector< double >
//#endif // HAVE_EIGEN
                      > VectorTypes;

template< class VectorType >
struct VectorTest
  : public ::testing::Test
{
  void check() const
  {
    VectorType* vector = VectorType::create(4);
    const std::string DUNE_UNUSED(type) = vector->type();
    const int DUNE_UNUSED(dim) = vector->dim();
    const bool DUNE_UNUSED(almost_equal) = vector->almost_equal(vector);
    vector->scal(1.0);
    vector->axpy(1.0, vector);
    const double DUNE_UNUSED(dot) = vector->dot(vector);
    const double DUNE_UNUSED(l1_norm) = vector->l1_norm();
    const double DUNE_UNUSED(l2_norm) = vector->l2_norm();
    const double DUNE_UNUSED(sup_norm) = vector->sup_norm();
    std::vector< int >* component_indices = new std::vector< int >(2);
    component_indices->operator[](0) = 3;
    component_indices->operator[](1) = 1;
    std::vector< double >* components = vector->components(component_indices);
    delete component_indices;
    delete components;
    std::pair< int, double >* DUNE_UNUSED(amax) = vector->amax();
    VectorType* add = vector->add(vector);
    delete add;
    vector->iadd(vector);
    VectorType* sub = vector->sub(vector);
    delete sub;
    vector->isub(vector);
    delete vector;
  }
}; // struct VectorTest


TYPED_TEST_CASE(VectorTest, VectorTypes);
TYPED_TEST(VectorTest, LA_CONTAINER) {
  this->check();
}


int main(int argc, char** argv)
{
  test_init(argc, argv);
  return RUN_ALL_TESTS();
}
