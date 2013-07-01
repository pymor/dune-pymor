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

#include <dune/pymor/la/container/interfaces.hh>

#if HAVE_EIGEN
#include <dune/pymor/la/container/eigen.cc>

typedef testing::Types< Dune::Pymor::LA::EigenDenseVector< double >
                      > VectorTypes;

template< class VectorType >
struct VectorTest
  : public ::testing::Test
{
  typedef typename VectorType::ScalarType ScalarType;
  void check() const
  {
    VectorType* vector = VectorType::create(4);
    const std::string DUNE_UNUSED(type) = vector->type();
    const int DUNE_UNUSED(dim) = vector->dim();
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

#else // HAVE_EIGEN

int main(int /*argc*/, char** /*argv*/)
{
  return 0;
}

#endif // HAVE_EIGEN
