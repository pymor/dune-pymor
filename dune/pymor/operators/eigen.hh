// This file is part of the dune-pymor project:
//   https://github.com/pymor/dune-pymor
// Copyright Holders: Stephan Rave, Felix Schindler
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_OPERATORS_EIGEN_HH
#define DUNE_PYMOR_OPERATORS_EIGEN_HH

#if HAVE_EIGEN

#include <dune/common/static_assert.hh>

#include <dune/stuff/common/configtree.hh>
#include <dune/stuff/la/container/eigen.hh>

#include <dune/pymor/parameters/base.hh>

#include "base.hh"

namespace Dune {
namespace Pymor {
namespace Operators {


template< class ScalarImp = double >
class EigenRowMajorSparseInverse
  : public MatrixBasedInverseDefault< Stuff::LA::EigenRowMajorSparseMatrix< ScalarImp >
                                    , Stuff::LA::EigenDenseVector< ScalarImp > >
{
  typedef MatrixBasedInverseDefault< Stuff::LA::EigenRowMajorSparseMatrix< ScalarImp >
                                   , Stuff::LA::EigenDenseVector< ScalarImp > > BaseType;
  using typename BaseType::MatrixType;
  using typename BaseType::LinearSolverType;

public:
  EigenRowMajorSparseInverse(const MatrixType* matrix_ptr, const std::string type = LinearSolverType::options()[0])
    : BaseType(matrix_ptr, type)
  {}

  EigenRowMajorSparseInverse(const MatrixType* matrix_ptr, const Stuff::Common::ConfigTree& options)
    : BaseType(matrix_ptr, options)
  {}

  EigenRowMajorSparseInverse(const std::shared_ptr< const MatrixType > matrix_ptr,
                             const std::string type = LinearSolverType::options()[0])
    : BaseType(matrix_ptr, type)
  {}

  EigenRowMajorSparseInverse(const std::shared_ptr< const MatrixType > matrix_ptr,
                             const Stuff::Common::ConfigTree& options)
    : BaseType(matrix_ptr, options)
  {}
}; // class EigenRowMajorSparseInverse


template< class ScalarImp = double >
class EigenRowMajorSparse
  : public MatrixBasedDefault< Stuff::LA::EigenRowMajorSparseMatrix< ScalarImp >
                             , Stuff::LA::EigenDenseVector< ScalarImp > >
{
  typedef MatrixBasedDefault< Stuff::LA::EigenRowMajorSparseMatrix< ScalarImp >
                            , Stuff::LA::EigenDenseVector< ScalarImp > > BaseType;
  using typename BaseType::MatrixType;

public:
  EigenRowMajorSparse(const MatrixType* matrix_ptr)
    : BaseType(matrix_ptr)
  {}

  EigenRowMajorSparse(const std::shared_ptr< const MatrixType > matrix_ptr)
    : BaseType(matrix_ptr)
  {}
}; // class EigenRowMajorSparse


template< class ScalarImp = double >
class EigenDenseInverse
  : public MatrixBasedInverseDefault< Stuff::LA::EigenDenseMatrix< ScalarImp >
                                    , Stuff::LA::EigenDenseVector< ScalarImp > >
{
  typedef MatrixBasedInverseDefault< Stuff::LA::EigenDenseMatrix< ScalarImp >
                                   , Stuff::LA::EigenDenseVector< ScalarImp > > BaseType;
  using typename BaseType::MatrixType;
  using typename BaseType::LinearSolverType;

public:
  EigenDenseInverse(const MatrixType* matrix_ptr, const std::string type = LinearSolverType::options()[0])
    : BaseType(matrix_ptr, type)
  {}

  EigenDenseInverse(const MatrixType* matrix_ptr, const Stuff::Common::ConfigTree& options)
    : BaseType(matrix_ptr, options)
  {}

  EigenDenseInverse(const std::shared_ptr< const MatrixType > matrix_ptr,
                    const std::string type = LinearSolverType::options()[0])
    : BaseType(matrix_ptr, type)
  {}

  EigenDenseInverse(const std::shared_ptr< const MatrixType > matrix_ptr,
                    const Stuff::Common::ConfigTree& options)
    : BaseType(matrix_ptr, options)
  {}
}; // class EigenDenseInverse


template< class ScalarImp = double >
class EigenDense
  : public MatrixBasedDefault< Stuff::LA::EigenDenseMatrix< ScalarImp >
                             , Stuff::LA::EigenDenseVector< ScalarImp > >
{
  typedef MatrixBasedDefault< Stuff::LA::EigenDenseMatrix< ScalarImp >
                            , Stuff::LA::EigenDenseVector< ScalarImp > > BaseType;
  using typename BaseType::MatrixType;

public:
  EigenDense(const MatrixType* matrix_ptr)
    : BaseType(matrix_ptr)
  {}

  EigenDense(const std::shared_ptr< const MatrixType > matrix_ptr)
    : BaseType(matrix_ptr)
  {}
}; // class EigenDense


#else // HAVE_EIGEN


template< class ScalarImp = double >
class EigenRowMajorSparse
{
  static_assert(AlwaysFalse< ScalarImp >::value, "You are missing Eigen!");
};

template< class ScalarImp = double >
class EigenRowMajorSparseInverse
{
  static_assert(AlwaysFalse< ScalarImp >::value, "You are missing Eigen!");
};

template< class ScalarImp = double >
class EigenDense
{
  static_assert(AlwaysFalse< ScalarImp >::value, "You are missing Eigen!");
};

template< class ScalarImp = double >
class EigenDenseInverse
{
  static_assert(AlwaysFalse< ScalarImp >::value, "You are missing Eigen!");
};

#endif // HAVE_EIGEN

} // namespace Operators
} // namespace Pymor
} // namespace Dune

#endif // DUNE_PYMOR_OPERATORS_EIGEN_HH
