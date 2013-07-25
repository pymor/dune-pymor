// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
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

  EigenDenseVector(const int size = 0, const ScalarType value = ScalarType(0));

  /**
   * \attention This class takes ownership of backend_ptr!
   */
  EigenDenseVector(BackendType* backend_ptr);

  EigenDenseVector(std::unique_ptr< BackendType >&& backend_ptr);

  EigenDenseVector(const ThisType& other) = delete;

  ThisType& operator=(const ThisType& other) = delete;

  EigenDenseVector(ThisType&& source);

  ThisType& operator=(ThisType&& source);

  ThisType copy() const;

  unsigned int dim() const;

  bool has_equal_shape(const ThisType& other) const;

  bool almost_equal(const ThisType& other,
                    const ScalarType epsilon = Dune::FloatCmp::DefaultEpsilon< ScalarType >::value()) const;

  using BaseType::almost_equal;

  void scal(const ScalarType& alpha);

  void axpy(const ScalarType& alpha, const ThisType& xx) throw (Exception::sizes_do_not_match);

  using BaseType::axpy;

  ScalarType dot(const ThisType& other) const throw (Exception::sizes_do_not_match);

  using BaseType::dot;

  ScalarType l1_norm() const;

  ScalarType l2_norm() const;

  ScalarType sup_norm() const;

  std::vector< ScalarType > components(const std::vector< int >& component_indices) const
    throw (Exception::sizes_do_not_match, Exception::index_out_of_range);

  std::vector< ScalarType > amax() const;

  void add(const ThisType& other, ThisType& result) const throw (Exception::sizes_do_not_match);

  using BaseType::add;

  void iadd(const ThisType& other) throw (Exception::sizes_do_not_match);

  using BaseType::iadd;

  void sub(const ThisType& other, ThisType& result) const throw (Exception::sizes_do_not_match);

  using BaseType::sub;

  void isub(const ThisType& other) throw (Exception::sizes_do_not_match);

  using BaseType::isub;

  BackendType& backend();

  const BackendType& backend() const;

private:
  static int assert_is_not_negative(const int ii) throw (Exception::index_out_of_range);

  friend class Operators::EigenRowMajorSparse< ScalarType >;
  friend class Operators::EigenRowMajorSparseInverse< ScalarType >;

  std::unique_ptr< BackendType > backend_;
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

  EigenRowMajorSparseMatrix(std::unique_ptr< BackendType >&& backend_ptr);

  EigenRowMajorSparseMatrix(const ThisType& other) = delete;

  ThisType& operator=(const ThisType& other) = delete;

  EigenRowMajorSparseMatrix(ThisType&& source);

  ThisType& operator=(ThisType&& source);

  ThisType copy() const;

  unsigned int dim_source() const;

  unsigned int dim_range() const;

  bool has_equal_shape(const ThisType& other) const;

  void scal(const ScalarType& alpha);

  void axpy(const ScalarType& alpha, const ThisType& x) throw (Exception::sizes_do_not_match);

  BackendType& backend();

  const BackendType& backend() const;

private:
  friend class Operators::EigenRowMajorSparse< ScalarType >;
  friend class Operators::EigenRowMajorSparseInverse< ScalarType >;

  std::unique_ptr< BackendType > backend_;
}; // class EigenRowMajorSparseMatrix


template< class S >
static EigenDenseVector< S > createContainer(const EigenDenseVector< S >&, const size_t size)
{
  return EigenDenseVector< S >(size, S(1));
}


template< class S >
static EigenRowMajorSparseMatrix< S > createContainer(const EigenRowMajorSparseMatrix< S >&, const size_t size)
{
  return EigenRowMajorSparseMatrix< S >(Dune::Stuff::LA::createIdentityEigenRowMajorSparseMatrix(size));
}


} // namespace LA
} // namespace Pymor
} // namespace Dune

#endif // HAVE_EIGEN

#endif // DUNE_PYMOR_LA_CONTAINER_EIGEN_HH
