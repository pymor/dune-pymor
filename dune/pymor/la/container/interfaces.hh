// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_LA_CONTAINER_INTERFACES_HH
#define DUNE_PYMOR_LA_CONTAINER_INTERFACES_HH

#include <utility>
#include <vector>
#include <string>
#include <type_traits>

#include <dune/common/float_cmp.hh>

#include <dune/pymor/common/crtp.hh>
#include <dune/pymor/common/exceptions.hh>
//#include <dune/pymor/functionals/interfaces.hh>

namespace Dune {
namespace Pymor {
namespace LA {


class ContainerInterfacePybindgen {};

template< class Traits >
class ContainerInterface
  : public CRTPInterface< ContainerInterface< Traits >, Traits >
  , public ContainerInterfacePybindgen
{
  typedef CRTPInterface< ContainerInterface< Traits >, Traits > CRTP;
  typedef ContainerInterface< Traits >                          ThisType;
public:
  typedef typename Traits::derived_type derived_type;
  typedef typename Traits::ScalarType   ScalarType;

  /**
   * \brief   Creates a (deep) copy of the underlying resource
   * \return  A new container
   */
  derived_type copy() const
  {
    CHECK_INTERFACE_IMPLEMENTATION(CRTP::as_imp(*this).copy());
    return CRTP::as_imp(*this).copy();
  }

  /**
   * \brief BLAS SCAL operation (in-place sclar multiplication).
   * \param alpha The scalar coefficient with which each element of the container is multiplied.
   */
  void scal(const ScalarType& alpha)
  {
    CHECK_AND_CALL_INTERFACE_IMPLEMENTATION(CRTP::as_imp(*this).scal(alpha));
  }

  /**
   * \brief BLAS AXPY operation.
   * \param alpha The scalar coefficient with which each element of the container is multiplied
   * \param xx    Container that is to be elementwise added.
   */
  void axpy(const ScalarType& alpha, const derived_type& xx)
  {
    CHECK_AND_CALL_INTERFACE_IMPLEMENTATION(CRTP::as_imp(*this).axpy(alpha, xx));
  }

  void axpy(const ScalarType& alpha, const ThisType& xx)
  {
    CHECK_AND_CALL_INTERFACE_IMPLEMENTATION(CRTP::as_imp(*this).axpy(alpha, CRTP::as_imp(xx)));
  }

  bool has_equal_shape(const derived_type& other) const
  {
    CHECK_INTERFACE_IMPLEMENTATION(CRTP::as_imp(*this).has_equal_shape(other));
    return CRTP::as_imp(*this).has_equal_shape(other);
  }
}; // class ContainerInterface


class VectorInterfacePybindgen {};

template< class Traits >
class VectorInterface
  : public ContainerInterface< Traits >
  , public VectorInterfacePybindgen
{
  typedef CRTPInterface< ContainerInterface< Traits >, Traits > CRTP;
  typedef VectorInterface< Traits >                             ThisType;
public:
  typedef typename Traits::derived_type derived_type;
  typedef typename Traits::ScalarType   ScalarType;

  /**
   * \brief   The dimension of the vector.
   * \return  The dimension of the vector.
   */
  unsigned int dim() const
  {
    CHECK_INTERFACE_IMPLEMENTATION(CRTP::as_imp(*this).dim());
    return CRTP::as_imp(*this).dim();
  }

  /**
   * \brief   Check vectors for equality.
   *          Equality of two vectors is defined as in Dune::FloatCmp componentwise.
   * \param   other   A vector of same dimension to compare with.
   * \param   epsilon See Dune::FloatCmp.
   * \return          Truth value of the comparison.
   */
  bool almost_equal(const derived_type& other,
                    const ScalarType epsilon = Dune::FloatCmp::DefaultEpsilon< ScalarType >::value()) const
    throw (Exception::sizes_do_not_match, Exception::you_have_to_implement_this)
  {
    CHECK_INTERFACE_IMPLEMENTATION(CRTP::as_imp(*this).almost_equal(other, epsilon));
    return CRTP::as_imp(*this).almost_equal(other, epsilon);
  }

  bool almost_equal(const ThisType& other,
                    const ScalarType epsilon = Dune::FloatCmp::DefaultEpsilon< ScalarType >::value()) const
    throw (Exception::sizes_do_not_match, Exception::you_have_to_implement_this)
  {
    CHECK_INTERFACE_IMPLEMENTATION(CRTP::as_imp(*this).almost_equal(CRTP::as_imp(other), epsilon));
    return CRTP::as_imp(*this).almost_equal(CRTP::as_imp(other), epsilon);
  }

  /**
   * \brief   Computes the scalar products between two vectors.
   * \param   other The second factor.
   * \return        The scalar product.
   */
  ScalarType dot(const derived_type& other) const
    throw (Exception::sizes_do_not_match, Exception::you_have_to_implement_this)
  {
    CHECK_INTERFACE_IMPLEMENTATION(CRTP::as_imp(*this).dot(other));
    return CRTP::as_imp(*this).dot(other);
  }

  ScalarType dot(const ThisType& other) const
    throw (Exception::sizes_do_not_match, Exception::you_have_to_implement_this)
  {
    CHECK_INTERFACE_IMPLEMENTATION(CRTP::as_imp(*this).dot(CRTP::as_imp(other)));
    return CRTP::as_imp(*this).dot(CRTP::as_imp(other));
  }

  /**
   * \brief   The l1-norm of the vector.
   * \return  The l1-norm of the vector.
   */
  ScalarType l1_norm() const
  {
    CHECK_INTERFACE_IMPLEMENTATION(CRTP::as_imp(*this).l1_norm());
    return CRTP::as_imp(*this).l1_norm();
  }

  /**
   * \brief   The l2-norm of the vector.
   * \return  The l2-norm of the vector.
   */
  ScalarType l2_norm() const
  {
    CHECK_INTERFACE_IMPLEMENTATION(CRTP::as_imp(*this).l2_norm());
    return CRTP::as_imp(*this).l2_norm();
  }

  /**
   * \brief   The l-infintiy-norm of the vector.
   * \return  The l-infintiy-norm of the vector.
   */
  ScalarType sup_norm() const
  {
    CHECK_INTERFACE_IMPLEMENTATION(CRTP::as_imp(*this).sup_norm());
    return CRTP::as_imp(*this).sup_norm();
  }

  /**
   * \brief Extract components of the vector.
   * \param component_indices Indices of the vector components that are to be returned.
   * \return                  A std::vector< ScalarType > `result` such that `result[i]` is the `component_indices[i]`-th
                              component of the vector.
   */
  std::vector< ScalarType > components(const std::vector< int >& component_indices) const
    throw (Exception::sizes_do_not_match, Exception::index_out_of_range)
  {
    CHECK_INTERFACE_IMPLEMENTATION(CRTP::as_imp(*this).components(component_indices));
    return CRTP::as_imp(*this).components(component_indices);
  }

  /**
   * \brief   The maximum absolute value of the vector.
   * \return  A std::vector< ScalarType > result, where int(result[0]) is the index at which the maximum is attained and
   *          result[1] is the absolute maximum value.
   */
  std::vector< ScalarType > amax() const
  {
    CHECK_INTERFACE_IMPLEMENTATION(CRTP::as_imp(*this).amax());
    return CRTP::as_imp(*this).amax();
  }

  /**
   * \brief Adds two vectors.
   * \param other   The right summand.
   * \param result  Vector to write the result of this + other to
   */
  void add(const derived_type& other, derived_type& result) const throw (Exception::sizes_do_not_match)
  {
    CHECK_AND_CALL_INTERFACE_IMPLEMENTATION(CRTP::as_imp(*this).add(other, result));
  }

  void add(const ThisType& other, ThisType& result) const throw (Exception::sizes_do_not_match)
  {
    CHECK_AND_CALL_INTERFACE_IMPLEMENTATION(CRTP::as_imp(*this).add(CRTP::as_imp(other), CRTP::as_imp(result)));
  }

  /**
   * \brief   Adds two vectors.
   * \param   other The right summand.
   * \param
   * \return  The sum of this and other.
   */
  derived_type add(const derived_type& other) const throw (Exception::sizes_do_not_match)
  {
    derived_type result = this->copy();
    result.iadd(other);
    return result;
  }

  derived_type add(const ThisType& other) const throw (Exception::sizes_do_not_match)
  {
    derived_type result = this->copy();
    result.iadd(other);
    return result;
  }

  /**
   * \brief Inplace variant of add().
   * \param other The right summand.
   */
  void iadd(const derived_type& other) throw (Exception::sizes_do_not_match)
  {
    CHECK_AND_CALL_INTERFACE_IMPLEMENTATION(CRTP::as_imp(*this).iadd(other));
  }

  void iadd(const ThisType& other) throw (Exception::sizes_do_not_match)
  {
    CHECK_AND_CALL_INTERFACE_IMPLEMENTATION(CRTP::as_imp(*this).iadd(CRTP::as_imp(other)));
  }

  /**
   * \brief Subtracts two vectors.
   * \param other   The subtrahend.
   * \param result  The vectror to write the difference between this and other to.
   */
  void sub(const derived_type& other, derived_type& result) const throw (Exception::sizes_do_not_match)
  {
    CHECK_AND_CALL_INTERFACE_IMPLEMENTATION(CRTP::as_imp(*this).sub(other, result));
  }

  void sub(const ThisType& other, ThisType& result) const throw (Exception::sizes_do_not_match)
  {
    CHECK_AND_CALL_INTERFACE_IMPLEMENTATION(CRTP::as_imp(*this).sub(CRTP::as_imp(other), CRTP::as_imp(result)));
  }

  /**
   * \brief   Subtracts two vectors.
   * \param   other The subtrahend.
   * \return        The difference between this and other.
   */
  derived_type sub(const derived_type& other) const throw (Exception::sizes_do_not_match)
  {
    derived_type result = this->copy();
    result.isub(other);
    return result;
  }

  derived_type sub(const ThisType& other) const throw (Exception::sizes_do_not_match)
  {
    derived_type result = this->copy();
    result.isub(other);
    return result;
  }

  /**
   * \brief Inplace variant of sub().
   * \param other The subtrahend.
   */
  void isub(const derived_type& other) throw (Exception::sizes_do_not_match)
  {
    CHECK_AND_CALL_INTERFACE_IMPLEMENTATION(CRTP::as_imp(*this).isub(other));
  }

  void isub(const ThisType& other) throw (Exception::sizes_do_not_match)
  {
    CHECK_AND_CALL_INTERFACE_IMPLEMENTATION(CRTP::as_imp(*this).isub(CRTP::as_imp(other)));
  }

//  virtual bool linear() const;

//  virtual unsigned int dim_source() const;

//  virtual std::string type_source() const;

//  virtual double apply(const LA::VectorInterface& source,
//                       const Parameter /*mu*/ = Parameter()) const throw (Exception::types_are_not_compatible,
//                                                                          Exception::you_have_to_implement_this,
//                                                                          Exception::sizes_do_not_match,
//                                                                          Exception::wrong_parameter_type,
//                                                                          Exception::requirements_not_met,
//                                                                          Exception::linear_solver_failed,
//                                                                          Exception::this_does_not_make_any_sense);
}; // class VectorInterface


template< class Traits >
class MatrixInterface
  : public ContainerInterface< Traits >
{
  typedef CRTPInterface< ContainerInterface< Traits >, Traits > CRTP;
  typedef MatrixInterface< Traits >                             ThisType;
public:
  typedef typename Traits::derived_type derived_type;
  typedef typename Traits::ScalarType   ScalarType;

  unsigned int dim_source() const
  {
    CHECK_INTERFACE_IMPLEMENTATION(CRTP::as_imp(*this).dim_source());
    return CRTP::as_imp(*this).dim_source();
  }

  unsigned int dim_range() const
  {
    CHECK_INTERFACE_IMPLEMENTATION(CRTP::as_imp(*this).dim_range());
    return CRTP::as_imp(*this).dim_range();
  }
}; // class MatrixInterface


} // namespace LA
} // namespace Pymor
} // namespace Dune

#endif // DUNE_PYMOR_LA_CONTAINER_INTERFACES_HH
