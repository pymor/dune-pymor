// This file is part of the dune-pymor project:
//   https://github.com/pymor/dune-pymor
// Copyright Holders: Stephan Rave, Felix Schindler
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_OPERATORS_EIGEN_HH
#define DUNE_PYMOR_OPERATORS_EIGEN_HH

#ifndef DUNE_STUFF_SSIZE_T
# define DUNE_STUFF_SSIZE_T long int
#endif

#if HAVE_EIGEN

#include <dune/common/static_assert.hh>

#include <dune/stuff/common/configtree.hh>
#include <dune/stuff/la/container/eigen.hh>

#include <dune/pymor/parameters/base.hh>

#include "base.hh"

namespace Dune {
namespace Pymor {
namespace Operators {


// forwards
template< class ScalarImp = double >
class EigenRowMajorSparse;

template< class ScalarImp = double >
class EigenRowMajorSparseInverse;

template< class ScalarImp = double >
class EigenDense;

template< class ScalarImp = double >
class EigenDenseInverse;


namespace internal {


template< class ScalarImp >
class EigenRowMajorSparseInverseTraits
{
public:
  typedef ScalarImp                                           ScalarType;
  typedef EigenRowMajorSparseInverse< ScalarType >            derived_type;
  typedef Stuff::LA::EigenRowMajorSparseMatrix< ScalarType >  MatrixType;
  typedef Stuff::LA::EigenDenseVector< ScalarType >           SourceType;
  typedef Stuff::LA::EigenDenseVector< ScalarType >           RangeType;
  typedef derived_type                                        FrozenType;
  typedef EigenRowMajorSparse< ScalarType >                   InverseType;
};


template< class ScalarImp >
class EigenRowMajorSparseTraits
{
public:
  typedef ScalarImp                                           ScalarType;
  typedef EigenRowMajorSparse< ScalarType >                   derived_type;
  typedef Stuff::LA::EigenRowMajorSparseMatrix< ScalarType >  ContainerType;
  typedef Stuff::LA::EigenDenseVector< ScalarType >           SourceType;
  typedef Stuff::LA::EigenDenseVector< ScalarType >           RangeType;
  typedef derived_type                                        FrozenType;
  typedef EigenRowMajorSparseInverse< ScalarType >            InverseType;
};


template< class ScalarImp >
class EigenDenseInverseTraits
{
public:
  typedef ScalarImp                                 ScalarType;
  typedef EigenDenseInverse< ScalarType >           derived_type;
  typedef Stuff::LA::EigenDenseMatrix< ScalarType > MatrixType;
  typedef Stuff::LA::EigenDenseVector< ScalarType > SourceType;
  typedef Stuff::LA::EigenDenseVector< ScalarType > RangeType;
  typedef derived_type                              FrozenType;
  typedef EigenDense< ScalarType >                  InverseType;
};


template< class ScalarImp >
class EigenDenseTraits
{
public:
  typedef ScalarImp                                 ScalarType;
  typedef EigenDense< ScalarType >                  derived_type;
  typedef Stuff::LA::EigenDenseMatrix< ScalarType > ContainerType;
  typedef Stuff::LA::EigenDenseVector< ScalarType > SourceType;
  typedef Stuff::LA::EigenDenseVector< ScalarType > RangeType;
  typedef derived_type                              FrozenType;
  typedef EigenDenseInverse< ScalarType >           InverseType;
};


} // namespace internal


template< class ScalarImp >
class EigenRowMajorSparseInverse
  : public MatrixBasedInverseBase< internal::EigenRowMajorSparseInverseTraits< ScalarImp > >
{
  typedef MatrixBasedInverseBase< internal::EigenRowMajorSparseInverseTraits< ScalarImp > > BaseType;
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


template< class ScalarImp >
class EigenRowMajorSparse
  : public MatrixBasedBase< internal::EigenRowMajorSparseTraits< ScalarImp > >
{
  typedef MatrixBasedBase< internal::EigenRowMajorSparseTraits< ScalarImp > > BaseType;
  using typename BaseType::MatrixType;

public:
  EigenRowMajorSparse(const MatrixType* matrix_ptr)
    : BaseType(matrix_ptr)
  {}

  EigenRowMajorSparse(const std::shared_ptr< const MatrixType > matrix_ptr)
    : BaseType(matrix_ptr)
  {}
}; // class EigenRowMajorSparse


template< class ScalarImp >
class EigenDenseInverse
  : public MatrixBasedInverseBase< internal::EigenDenseInverseTraits< ScalarImp > >
{
  typedef MatrixBasedInverseBase< internal::EigenDenseInverseTraits< ScalarImp > > BaseType;
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


template< class ScalarImp >
class EigenDense
  : public MatrixBasedBase< internal::EigenDenseTraits< ScalarImp > >
{
  typedef MatrixBasedBase< internal::EigenDenseTraits< ScalarImp > > BaseType;
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
