// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_OPERATORS_EIGEN_HH
#define DUNE_PYMOR_OPERATORS_EIGEN_HH

#if HAVE_EIGEN

#include <dune/pymor/la/container/eigen.hh>
#include <dune/pymor/parameters/base.hh>

#include "interfaces.hh"

namespace Dune {
namespace Pymor {
namespace Operators {


template< class ScalarImp = double >
class EigenRowMajorSparse;

template< class ScalarImp = double >
class EigenRowMajorSparseInverse;


template< class ScalarImp >
class EigenRowMajorSparseInverseTraits
{
public:
  typedef ScalarImp                                   ScalarType;
  typedef EigenRowMajorSparseInverse< ScalarType >    derived_type;
  typedef LA::EigenRowMajorSparseMatrix< ScalarType > ContainerType;
  typedef LA::EigenDenseVector< ScalarType >          SourceType;
  typedef LA::EigenDenseVector< ScalarType >          RangeType;
  typedef derived_type                                FrozenType;
  typedef EigenRowMajorSparse< ScalarType >           InverseType;
};


template< class ScalarImp >
class EigenRowMajorSparseInverse
  : public OperatorInterface< EigenRowMajorSparseInverseTraits< ScalarImp > >
{
  typedef OperatorInterface< EigenRowMajorSparseInverseTraits< ScalarImp > > BaseType;
public:
  typedef EigenRowMajorSparseInverseTraits< ScalarImp > Traits;
  typedef typename Traits::derived_type   ThisType;
  typedef typename Traits::ScalarType     ScalarType;
  typedef typename Traits::SourceType     SourceType;
  typedef typename Traits::RangeType      RangeType;
  typedef typename Traits::ContainerType  ContainerType;
  typedef typename Traits::FrozenType     FrozenType;
  typedef typename Traits::InverseType    InverseType;

  EigenRowMajorSparseInverse(const ContainerType* matrix_ptr, const std::string option = "bicgstab.ilut");

  EigenRowMajorSparseInverse(const std::shared_ptr< const ContainerType > matrix_ptr, const std::string option = "bicgstab.ilut");

  bool linear() const;

  DUNE_PYMOR_SSIZE_T dim_source() const;

  DUNE_PYMOR_SSIZE_T dim_range() const;

  void apply(const SourceType& source, RangeType& range, const Parameter mu = Parameter()) const;

  using BaseType::apply;

  static std::vector< std::string > invert_options();

  InverseType invert(const std::string option = invert_options()[0],
                     const Parameter mu = Parameter()) const;

  FrozenType freeze_parameter(const Parameter mu = Parameter()) const;

private:
  std::shared_ptr< const ContainerType > matrix_;
  const std::string option_;
}; // class EigenRowMajorSparseInverse


template< class ScalarImp >
class EigenRowMajorSparseTraits
{
public:
  typedef ScalarImp                                   ScalarType;
  typedef EigenRowMajorSparse< ScalarType >           derived_type;
  typedef LA::EigenRowMajorSparseMatrix< ScalarType > ContainerType;
  typedef LA::EigenDenseVector< ScalarType >          SourceType;
  typedef LA::EigenDenseVector< ScalarType >          RangeType;
  typedef derived_type                                FrozenType;
  typedef EigenRowMajorSparseInverse< ScalarType >    InverseType;
};


template< class ScalarImp >
class EigenRowMajorSparse
  : public OperatorInterface< EigenRowMajorSparseTraits< ScalarImp > >
  , public LA::ProvidesContainer< EigenRowMajorSparseTraits< ScalarImp > >
{
  typedef OperatorInterface< EigenRowMajorSparseTraits< ScalarImp > > BaseType;
public:
  typedef EigenRowMajorSparseTraits< ScalarImp >  Traits;
  typedef typename Traits::derived_type           ThisType;
  typedef typename Traits::ScalarType             ScalarType;
  typedef typename Traits::SourceType             SourceType;
  typedef typename Traits::RangeType              RangeType;
  typedef typename Traits::ContainerType          ContainerType;
  typedef typename Traits::FrozenType             FrozenType;
  typedef typename Traits::InverseType            InverseType;

  /**
   * \attention This class takes ownership of matrix_ptr!
   */
  EigenRowMajorSparse(const ContainerType* matrix_ptr);

  EigenRowMajorSparse(const std::shared_ptr< const ContainerType > matrix_ptr);

  bool linear() const;

  DUNE_PYMOR_SSIZE_T dim_source() const;

  DUNE_PYMOR_SSIZE_T dim_range() const;

  void apply(const SourceType& source, RangeType& range, const Parameter mu = Parameter()) const;

  using BaseType::apply;

  static std::vector< std::string > invert_options();

  InverseType invert(const std::string option = invert_options()[0],
                     const Parameter mu = Parameter()) const;

  FrozenType freeze_parameter(const Parameter mu = Parameter()) const;

  std::shared_ptr< const ContainerType > container() const;

private:
  std::shared_ptr< const ContainerType > matrix_;
}; // class EigenRowMajorSparse


} // namespace Operators
} // namespace Pymor
} // namespace Dune

#endif // HAVE_EIGEN

#endif // DUNE_PYMOR_OPERATORS_EIGEN_HH
