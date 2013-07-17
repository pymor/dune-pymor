// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_LA_CONTAINER_DYNAMICVECTOR_HH
#define DUNE_PYMOR_LA_CONTAINER_DYNAMICVECTOR_HH

#include <memory>

#include <dune/common/dynvector.hh>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include <dune/common/dynmatrix.hh>
#pragma GCC diagnostic pop
#include <dune/common/float_cmp.hh>

#include "interfaces.hh"

namespace Dune {
namespace Pymor {
namespace LA {


template< class ScalarImp = double >
class DuneDynamicVector;


template< class ScalarImp = double >
class DuneDynamicVectorTraits
{
public:
  typedef ScalarImp                       ScalarType;
  typedef DuneDynamicVector< ScalarType > derived_type;
};


/**
 * \brief An implementation of Dune::Pymor::LA::VectorInterface using Dune::DynamicVector< double >.
 *
 * \see   Dune::Pymor::LA::VectorInterface
 */
template< class ScalarImp >
class DuneDynamicVector
  : public Dune::Pymor::LA::VectorInterface< DuneDynamicVectorTraits< ScalarImp > >
{
  typedef Dune::Pymor::LA::VectorInterface< DuneDynamicVectorTraits< ScalarImp > > BaseType;
public:
  typedef DuneDynamicVectorTraits< ScalarImp >  Traits;
  typedef typename Traits::derived_type         ThisType;
  typedef typename Traits::ScalarType           ScalarType;
private:
  typedef Dune::DynamicVector< ScalarType >     BackendType;

public:
  DuneDynamicVector(const int size = 0, const ScalarType value = ScalarType(0));

  /**
   * \attention This class takes ownership of backend_ptr!
   */
  DuneDynamicVector(BackendType* backend_ptr);

  DuneDynamicVector(std::unique_ptr< BackendType >&& backend_ptr);

  DuneDynamicVector(const ThisType& other) = delete;

  ThisType& operator=(const ThisType& other) = delete;

  DuneDynamicVector(DuneDynamicVector&& source);

  ThisType& operator=(ThisType&& source);

  DuneDynamicVector copy() const;

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

private:
  static int assert_is_positive(const int ii) throw (Exception::index_out_of_range);

  std::unique_ptr< BackendType > backend_;
}; // class DuneDynamicVector


//class DuneDynamicMatrix
//{
//public:
//  typedef Dune::DynamicMatrix< double > BackendType;

//  DuneDynamicMatrix(const int rr, const int cc, const double value = 0.0);

//  DuneDynamicMatrix(std::shared_ptr< BackendType > backendPtr);

//  DuneDynamicMatrix(DuneDynamicMatrix& other);

//private:
//  DuneDynamicMatrix(const DuneDynamicMatrix& other);

//public:
//  virtual DuneDynamicMatrix copy() const;

//  unsigned int dim_source() const;

//  unsigned int dim_range() const;

//  void scal(const double alpha);

//  void axpy(const double alpha, const DuneDynamicMatrix& x) throw (Exception::sizes_do_not_match);

//  std::shared_ptr< BackendType > backend();

//  std::shared_ptr< const BackendType > backend() const;

//private:
//  static int assert_is_positive(const int ii) throw (Exception::index_out_of_range);

//  std::shared_ptr< BackendType > backend_;
//}; // class DuneDynamicMatrix


} // namespace LA
} // namespace Pymor
} // namespace Dune

#endif // DUNE_PYMOR_LA_CONTAINER_DYNAMICVECTOR_HH
