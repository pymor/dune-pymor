// This file is part of the dune-pymor project:
//   https://github.com/pymor/dune-pymor
// Copyright Holders: Stephan Rave, Felix Schindler
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_OPERATORS_ISTL_HH
#define DUNE_PYMOR_OPERATORS_ISTL_HH

#include <dune/stuff/common/configtree.hh>
#include <dune/stuff/la/container/istl.hh>

#include "base.hh"

namespace Dune {
namespace Pymor {
namespace Operators {

#if HAVE_DUNE_ISTL


template< class ScalarImp = double >
class IstlRowMajorSparse;

template< class ScalarImp = double >
class IstlRowMajorSparseInverse;


template< class ScalarImp >
class IstlRowMajorSparseInverseTraits
{
public:
  typedef ScalarImp                                         ScalarType;
  typedef IstlRowMajorSparseInverse< ScalarType >           derived_type;
  typedef Stuff::LA::IstlRowMajorSparseMatrix< ScalarType > MatrixType;
  typedef Stuff::LA::IstlDenseVector< ScalarType >          SourceType;
  typedef Stuff::LA::IstlDenseVector< ScalarType >          RangeType;
  typedef derived_type                                      FrozenType;
  typedef IstlRowMajorSparse< ScalarType >                  InverseType;
};


template< class ScalarImp >
class IstlRowMajorSparseInverse
  : public MatrixBasedInverseBase< IstlRowMajorSparseInverseTraits< ScalarImp > >
{
  typedef MatrixBasedInverseBase< IstlRowMajorSparseInverseTraits< ScalarImp > > BaseType;
  using typename BaseType::MatrixType;
  using typename BaseType::LinearSolverType;

public:
  IstlRowMajorSparseInverse(const MatrixType* matrix_ptr, const std::string type = LinearSolverType::options()[0])
    : BaseType(matrix_ptr, type)
  {}

  IstlRowMajorSparseInverse(const MatrixType* matrix_ptr, const Stuff::Common::ConfigTree& options)
    : BaseType(matrix_ptr, options)
  {}

  IstlRowMajorSparseInverse(const std::shared_ptr< const MatrixType > matrix_ptr,
                            const std::string type = LinearSolverType::options()[0])
    : BaseType(matrix_ptr, type)
  {}

  IstlRowMajorSparseInverse(const std::shared_ptr< const MatrixType > matrix_ptr,
                            const Stuff::Common::ConfigTree& options)
    : BaseType(matrix_ptr, options)
  {}
}; // class IstlRowMajorSparseInverse


template< class ScalarImp >
class IstlRowMajorSparseTraits
{
public:
  typedef ScalarImp                                         ScalarType;
  typedef IstlRowMajorSparse< ScalarType >                  derived_type;
  typedef Stuff::LA::IstlRowMajorSparseMatrix< ScalarType > ContainerType;
  typedef Stuff::LA::IstlDenseVector< ScalarType >          SourceType;
  typedef Stuff::LA::IstlDenseVector< ScalarType >          RangeType;
  typedef derived_type                                      FrozenType;
  typedef IstlRowMajorSparseInverse< ScalarType >           InverseType;
};


template< class ScalarImp >
class IstlRowMajorSparse
  : public MatrixBasedBase< IstlRowMajorSparseTraits< ScalarImp > >
{
  typedef MatrixBasedBase< IstlRowMajorSparseTraits< ScalarImp > > BaseType;
  using typename BaseType::MatrixType;

public:
  IstlRowMajorSparse(const MatrixType* matrix_ptr)
    : BaseType(matrix_ptr)
  {}

  IstlRowMajorSparse(const std::shared_ptr< const MatrixType > matrix_ptr)
    : BaseType(matrix_ptr)
  {}
}; // class IstlRowMajorSparse


#else // HAVE_DUNE_ISTL


template< class ScalarImp = double >
class IstlRowMajorSparse
{
  static_assert(AlwaysFalse< ScalarImp >::value, "You are missing dune-istl!");
};

template< class ScalarImp = double >
class IstlRowMajorSparseInverse
{
  static_assert(AlwaysFalse< ScalarImp >::value, "You are missing dune-istl!");
};


#endif // HAVE_DUNE_ISTL

} // namespace Operators
} // namespace Pymor
} // namespace Dune

#endif // DUNE_PYMOR_OPERATORS_ISTL_HH
