// This file is part of the dune-pymor project:
//   https://github.com/pymor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_LA_CONTAINER_EIGEN_HH
#define DUNE_PYMOR_LA_CONTAINER_EIGEN_HH

#if HAVE_EIGEN

#include <dune/common/float_cmp.hh>
#include <dune/stuff/la/container/eigen.hh>

#include "interfaces.hh"

namespace Dune {
namespace Pymor {

namespace Operators {

template< class ScalarImp >
class EigenRowMajorSparse;

template< class ScalarImp >
class EigenRowMajorSparseInverse;

}

namespace LA {


template< class ScalarImp = double >
class EigenDenseVector;


template< class ScalarImp = double >
class EigenDenseVectorTraits
{
public:
  typedef ScalarImp                       ScalarType;
  typedef EigenDenseVector< ScalarType >  derived_type;
  typedef Dune::Stuff::LA::EigenDenseVector< ScalarType > BackendType;
};


/**
 * \brief An implementation of Dune::Pymor::LA::VectorInterface using Dune::Stuff::LA::EigenDenseVector< double >.
 *
 * \see   Dune::Pymor::LA::VectorInterface
 */
template< class ScalarImp >
class EigenDenseVector
  : public Dune::Pymor::LA::VectorInterface< EigenDenseVectorTraits< ScalarImp > >
  , public ProvidesBackend< EigenDenseVectorTraits< ScalarImp > >
{
  typedef Dune::Pymor::LA::VectorInterface< EigenDenseVectorTraits< ScalarImp > > BaseType;
public:
  typedef EigenDenseVectorTraits< ScalarImp > Traits;
  typedef typename Traits::derived_type       ThisType;
  typedef typename Traits::ScalarType         ScalarType;
  typedef typename Traits::BackendType        BackendType;

  EigenDenseVector(const DUNE_PYMOR_SSIZE_T size = 0, const ScalarType value = ScalarType(0));

  /**
   * \attention This class takes ownership of backend_ptr!
   */
  EigenDenseVector(BackendType* backend_ptr);

  EigenDenseVector(std::shared_ptr< BackendType > backend_ptr);

  EigenDenseVector(const ThisType& other);

  ThisType& operator=(const ThisType& other);

  ThisType copy() const;

  DUNE_PYMOR_SSIZE_T dim() const;

  bool has_equal_shape(const ThisType& other) const;

  bool almost_equal(const ThisType& other,
                    const ScalarType epsilon = Dune::FloatCmp::DefaultEpsilon< ScalarType >::value()) const;

  using BaseType::almost_equal;

  void scal(const ScalarType& alpha);

  void axpy(const ScalarType& alpha, const ThisType& xx);

  using BaseType::axpy;

  ScalarType dot(const ThisType& other) const;

  using BaseType::dot;

  ScalarType l1_norm() const;

  ScalarType l2_norm() const;

  ScalarType sup_norm() const;

  std::vector< ScalarType > components(const std::vector< DUNE_PYMOR_SSIZE_T >& component_indices) const;

  std::vector< ScalarType > amax() const;

  void add(const ThisType& other, ThisType& result) const;

  using BaseType::add;

  void iadd(const ThisType& other);

  using BaseType::iadd;

  void sub(const ThisType& other, ThisType& result) const;

  using BaseType::sub;

  void isub(const ThisType& other);

  using BaseType::isub;

  BackendType& backend();

  const BackendType& backend() const;

private:
  static DUNE_PYMOR_SSIZE_T assert_is_not_negative(const DUNE_PYMOR_SSIZE_T ii);
  void ensure_uniqueness();

  friend class Operators::EigenRowMajorSparse< ScalarType >;
  friend class Operators::EigenRowMajorSparseInverse< ScalarType >;

  std::shared_ptr< BackendType > backend_;
}; // class EigenDenseVector


template< class ScalarImp = double >
class EigenRowMajorSparseMatrix;


template< class ScalarImp = double >
class EigenRowMajorSparseMatrixTraits
{
public:
  typedef ScalarImp                               ScalarType;
  typedef EigenRowMajorSparseMatrix< ScalarType > derived_type;
  typedef Dune::Stuff::LA::EigenRowMajorSparseMatrix< ScalarType > BackendType;
};


template< class ScalarImp >
class EigenRowMajorSparseMatrix
  : public Dune::Pymor::LA::MatrixInterface< EigenRowMajorSparseMatrixTraits< ScalarImp > >
  , public ProvidesBackend< EigenRowMajorSparseMatrixTraits< ScalarImp > >
{
public:
  typedef EigenRowMajorSparseMatrixTraits< ScalarImp >  Traits;
  typedef typename Traits::derived_type                 ThisType;
  typedef typename Traits::ScalarType                   ScalarType;
  typedef typename Traits::BackendType                  BackendType;

public:
  EigenRowMajorSparseMatrix();

  /**
   * \attention This class takes ownership of backend_ptr!
   */
  EigenRowMajorSparseMatrix(BackendType* backend_ptr);

  EigenRowMajorSparseMatrix(std::shared_ptr< BackendType >& backend_ptr);

  EigenRowMajorSparseMatrix(const ThisType& other);

  ThisType& operator=(const ThisType& other);

  ThisType copy() const;

  DUNE_PYMOR_SSIZE_T dim_source() const;

  DUNE_PYMOR_SSIZE_T dim_range() const;

  bool has_equal_shape(const ThisType& other) const;

  void scal(const ScalarType& alpha);

  void axpy(const ScalarType& alpha, const ThisType& x);

  BackendType& backend();

  const BackendType& backend() const;

private:
  void ensure_uniqueness();

  friend class Operators::EigenRowMajorSparse< ScalarType >;
  friend class Operators::EigenRowMajorSparseInverse< ScalarType >;

  std::shared_ptr< BackendType > backend_;
}; // class EigenRowMajorSparseMatrix


template< class S >
static EigenDenseVector< S > createContainer(const EigenDenseVector< S >&, const size_t size)
{
  return EigenDenseVector< S >(size, S(1));
}


template< class S >
static EigenRowMajorSparseMatrix< S > createContainer(const EigenRowMajorSparseMatrix< S >&, const size_t size)
{
  Dune::Stuff::LA::SparsityPatternDefault pattern(size);
  for (size_t ii = 0; ii < size; ++ii)
    pattern.inner(ii).insert(ii);
  auto backend_ptr = new Dune::Stuff::LA::EigenRowMajorSparseMatrix< S >(size, size, pattern);
  backend_ptr->backend().setIdentity();
  return EigenRowMajorSparseMatrix< S >(backend_ptr);
}


} // namespace LA
} // namespace Pymor
} // namespace Dune

#endif // HAVE_EIGEN

#endif // DUNE_PYMOR_LA_CONTAINER_EIGEN_HH
