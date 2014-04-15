// This file is part of the dune-pymor project:
//   https://github.com/pymor/dune-pymor
// Copyright Holders: Stephan Rave, Felix Schindler
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_OPERATORS_COMMONDENSE_HH
#define DUNE_PYMOR_OPERATORS_COMMONDENSE_HH

#include <dune/stuff/common/configtree.hh>
#include <dune/stuff/la/container/common.hh>

#include "base.hh"

namespace Dune {
namespace Pymor {
namespace Operators {


template< class ScalarImp = double >
class CommonDense;

template< class ScalarImp = double >
class CommonDenseInverse;


template< class ScalarImp >
class CommonDenseInverseTraits
{
public:
  typedef ScalarImp                           ScalarType;
  typedef CommonDenseInverse< ScalarType >    derived_type;
  typedef Stuff::LA::CommonDenseMatrix< ScalarType > MatrixType;
  typedef Stuff::LA::CommonDenseVector< ScalarType > SourceType;
  typedef Stuff::LA::CommonDenseVector< ScalarType > RangeType;
  typedef derived_type                        FrozenType;
  typedef CommonDense< ScalarType >           InverseType;
};


template< class ScalarImp >
class CommonDenseInverse
  : public MatrixBasedInverseBase< CommonDenseInverseTraits< ScalarImp > >
{
  typedef MatrixBasedInverseBase< CommonDenseInverseTraits< ScalarImp > > BaseType;
  using typename BaseType::MatrixType;
  using typename BaseType::LinearSolverType;

public:
  CommonDenseInverse(const MatrixType* matrix_ptr, const std::string type = LinearSolverType::options()[0])
    : BaseType(matrix_ptr, type)
  {}

  CommonDenseInverse(const MatrixType* matrix_ptr, const Stuff::Common::ConfigTree& options)
    : BaseType(matrix_ptr, options)
  {}

  CommonDenseInverse(const std::shared_ptr< const MatrixType > matrix_ptr,
                             const std::string type = LinearSolverType::options()[0])
    : BaseType(matrix_ptr, type)
  {}

  CommonDenseInverse(const std::shared_ptr< const MatrixType > matrix_ptr,
                             const Stuff::Common::ConfigTree& options)
    : BaseType(matrix_ptr, options)
  {}
}; // class CommonDenseInverse


template< class ScalarImp >
class CommonDenseTraits
{
public:
  typedef ScalarImp                           ScalarType;
  typedef CommonDense< ScalarType >           derived_type;
  typedef Stuff::LA::CommonDenseMatrix< ScalarType > ContainerType;
  typedef Stuff::LA::CommonDenseVector< ScalarType > SourceType;
  typedef Stuff::LA::CommonDenseVector< ScalarType > RangeType;
  typedef derived_type                        FrozenType;
  typedef CommonDenseInverse< ScalarType >    InverseType;
};


template< class ScalarImp >
class CommonDense
  : public MatrixBasedBase< CommonDenseTraits< ScalarImp > >
{
  typedef MatrixBasedBase< CommonDenseTraits< ScalarImp > > BaseType;
  using typename BaseType::MatrixType;

public:
  CommonDense(const MatrixType* matrix_ptr)
    : BaseType(matrix_ptr)
  {}

  CommonDense(const std::shared_ptr< const MatrixType > matrix_ptr)
    : BaseType(matrix_ptr)
  {}
}; // class CommonDense


} // namespace Operators
} // namespace Pymor
} // namespace Dune

#endif // DUNE_PYMOR_OPERATORS_COMMONDENSE_HH
