// This file is part of the dune-pymor project:
//   https://github.com/pymor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_LA_CONTAINER_DYNAMICVECTOR_HH
#define DUNE_PYMOR_LA_CONTAINER_DYNAMICVECTOR_HH

#ifndef DUNE_STUFF_SSIZE_T
# define DUNE_STUFF_SSIZE_T long int
#endif

#include <memory>

#include <dune/common/dynvector.hh>
#include <dune/stuff/common/disable_warnings.hh>
# include <dune/common/dynmatrix.hh>
#include <dune/stuff/common/reenable_warnings.hh>
#include <dune/common/float_cmp.hh>

#include "interfaces.hh"

namespace Dune {
namespace Pymor {

namespace Operators {

template< class ScalarImp >
class DuneDynamic;

template< class ScalarImp >
class DuneDynamicInverse;

}

namespace LA {


template< class ScalarImp = double >
class DuneDynamicVector;


template< class ScalarImp = double >
class DuneDynamicVectorTraits
{
public:
  typedef ScalarImp                         ScalarType;
  typedef DuneDynamicVector< ScalarType >   derived_type;
  typedef Dune::DynamicVector< ScalarType > BackendType;
};


/**
 * \brief An implementation of Dune::Pymor::LA::VectorInterface using Dune::DynamicVector< double >.
 *
 * \see   Dune::Pymor::LA::VectorInterface
 */
template< class ScalarImp >
class DuneDynamicVector
  : public Dune::Pymor::LA::VectorInterface< DuneDynamicVectorTraits< ScalarImp > >
  , public ProvidesBackend< DuneDynamicVectorTraits< ScalarImp > >
{
  typedef Dune::Pymor::LA::VectorInterface< DuneDynamicVectorTraits< ScalarImp > > BaseType;
public:
  typedef DuneDynamicVectorTraits< ScalarImp >  Traits;
  typedef typename Traits::derived_type         ThisType;
  typedef typename Traits::ScalarType           ScalarType;
  typedef typename Traits::BackendType          BackendType;

  DuneDynamicVector(const DUNE_STUFF_SSIZE_T size = 0, const ScalarType value = ScalarType(0));

  /**
   * \attention This class takes ownership of backend_ptr in the sense, that you must not remove it manually!
   */
  DuneDynamicVector(BackendType* backend_ptr);

  DuneDynamicVector(std::shared_ptr< BackendType >& backend_ptr);

  DuneDynamicVector(const ThisType& other);

  ThisType& operator=(const ThisType& other);

  ThisType copy() const;

  DUNE_STUFF_SSIZE_T dim() const;

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

  std::vector< ScalarType > components(const std::vector< DUNE_STUFF_SSIZE_T >& component_indices) const;

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
  static DUNE_STUFF_SSIZE_T assert_is_not_negative(const DUNE_STUFF_SSIZE_T ii);
  void ensure_uniqueness();

  friend class Operators::DuneDynamic< ScalarType >;
  friend class Operators::DuneDynamicInverse< ScalarType >;

  std::shared_ptr< BackendType > backend_;
}; // class DuneDynamicVector


template< class ScalarImp = double >
class DuneDynamicMatrix;


template< class ScalarImp = double >
class DuneDynamicMatrixTraits
{
public:
  typedef ScalarImp                         ScalarType;
  typedef DuneDynamicMatrix< ScalarType >   derived_type;
  typedef Dune::DynamicMatrix< ScalarType > BackendType;
};


template< class ScalarImp >
class DuneDynamicMatrix
  : public Dune::Pymor::LA::MatrixInterface< DuneDynamicMatrixTraits< ScalarImp > >
{
public:
  typedef DuneDynamicMatrixTraits< ScalarImp >  Traits;
  typedef typename Traits::derived_type         ThisType;
  typedef typename Traits::ScalarType           ScalarType;
  typedef typename Traits::BackendType          BackendType;

public:
  DuneDynamicMatrix(const DUNE_STUFF_SSIZE_T rr = 0,
                    const DUNE_STUFF_SSIZE_T cc = 0,
                    const ScalarType value = ScalarType(0));

  /**
   * \attention This class takes ownership of backend_ptr!
   */
  DuneDynamicMatrix(BackendType* backend_ptr);

  DuneDynamicMatrix(std::shared_ptr< BackendType >& backend_ptr);

  DuneDynamicMatrix(const ThisType& other);

  ThisType& operator=(const ThisType& other);

  ThisType copy() const;

  DUNE_STUFF_SSIZE_T dim_source() const;

  DUNE_STUFF_SSIZE_T dim_range() const;

  bool has_equal_shape(const ThisType& other) const;

  void scal(const ScalarType& alpha);

  void axpy(const ScalarType& alpha, const ThisType& x);

  BackendType& backend();

  const BackendType& backend() const;

private:
  static DUNE_STUFF_SSIZE_T assert_is_not_negative(DUNE_STUFF_SSIZE_T ii);
  void ensure_uniqueness();

  friend class Operators::DuneDynamic< ScalarType >;
  friend class Operators::DuneDynamicInverse< ScalarType >;

  std::shared_ptr< BackendType > backend_;
}; // class DuneDynamicMatrix


template< class S >
static DuneDynamicVector< S > createContainer(const DuneDynamicVector< S >&, const DUNE_STUFF_SSIZE_T size)
{
  return DuneDynamicVector< S >(size, S(1));
}


template< class S >
static DuneDynamicMatrix< S > createContainer(const DuneDynamicMatrix< S >&, const DUNE_STUFF_SSIZE_T size)
{
  typedef Dune::DynamicMatrix< S > BackendType;
  BackendType* matrix = new BackendType(size, size, S(0));
  for (size_t ii = 0; ii < size; ++ii)
    matrix->operator[](ii)[ii] = S(1);
  return DuneDynamicMatrix< S >(matrix);
}


} // namespace LA
} // namespace Pymor
} // namespace Dune

#endif // DUNE_PYMOR_LA_CONTAINER_DYNAMICVECTOR_HH
