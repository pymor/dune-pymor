// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_LA_CONTAINER_DYNAMICVECTOR_HH
#define DUNE_PYMOR_LA_CONTAINER_DYNAMICVECTOR_HH

#include <memory>

#include <dune/common/dynvector.hh>
#include <dune/stuff/common/disable_warnings.hh>
  #include <dune/common/dynmatrix.hh>
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

  DuneDynamicVector(const int size = 0, const ScalarType value = ScalarType(0));

  /**
   * \attention This class takes ownership of backend_ptr!
   */
  DuneDynamicVector(BackendType* backend_ptr);

  DuneDynamicVector(std::unique_ptr< BackendType >&& backend_ptr);

  DuneDynamicVector(const ThisType& other) = delete;

  ThisType& operator=(const ThisType& other) = delete;

  DuneDynamicVector(ThisType&& source);

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

  friend class Operators::DuneDynamic< ScalarType >;
  friend class Operators::DuneDynamicInverse< ScalarType >;

  std::unique_ptr< BackendType > backend_;
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
  DuneDynamicMatrix(const int rr = 0, const int cc = 0, const ScalarType value = ScalarType(0));

  /**
   * \attention This class takes ownership of backend_ptr!
   */
  DuneDynamicMatrix(BackendType* backend_ptr);

  DuneDynamicMatrix(std::unique_ptr< BackendType >&& backend_ptr);

  DuneDynamicMatrix(const ThisType& other) = delete;

  ThisType& operator=(const ThisType& other) = delete;

  DuneDynamicMatrix(ThisType&& source);

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
  static int assert_is_not_negative(const int ii) throw (Exception::index_out_of_range);

  friend class Operators::DuneDynamic< ScalarType >;
  friend class Operators::DuneDynamicInverse< ScalarType >;

  std::unique_ptr< BackendType > backend_;
}; // class DuneDynamicMatrix


template< class S >
static DuneDynamicVector< S > createContainer(const DuneDynamicVector< S >&, const size_t size)
{
  return DuneDynamicVector< S >(size, S(1));
}


template< class S >
static DuneDynamicMatrix< S > createContainer(const DuneDynamicMatrix< S >&, const size_t size)
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
