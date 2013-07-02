// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_LA_CONTAINER_INTERFACES_HH
#define DUNE_PYMOR_LA_CONTAINER_INTERFACES_HH

#include <utility>
#include <vector>
#include <string>

#include <dune/common/float_cmp.hh>

namespace Dune {
namespace Pymor {
namespace LA {


class VectorInterface
{
public:

  virtual ~VectorInterface() {}

  /**
   * \brief   A unique (per class, not per instance) identifier.
   * \return
   */
  virtual std::string type() const = 0;

  /**
   * \brief   The dimension of the vector.
   * \return  The dimension of the vector.
   */
  virtual int dim() const = 0;

  /**
   * \brief   Check vectors for equality.
   *          Equality of two vectors is defined as in Dune::FloatCmp componentwise.
   * \param   other   A vector of same dimension to compare with.
   * \param   epsilon See Dune::FloatCmp.
   * \return          Truth value of the comparison.
   */
  virtual bool almost_equal(const VectorInterface* other,
                            const double epsilon = Dune::FloatCmp::DefaultEpsilon< double >::value()) const = 0;

  /**
   * \brief BLAS SCAL operation (in-place sclar multiplication).
   * \param alpha The scalar coefficient with which the vector is multiplied.
   */
  virtual void scal(const double alpha) = 0;

  /**
   * \brief BLAS AXPY operation.
   * \param alpha The scalar coefficient with which the vector is multiplied
   * \param x     Vector that is to be added.
   */
  virtual void axpy(const double alpha, const VectorInterface* x) = 0;

  /**
   * \brief   Computes the scalar products between two vectors.
   * \param   other The second factor.
   * \return        The scalar product.
   */
  virtual double dot(const VectorInterface* other) const = 0;

  /**
   * \brief   The l1-norm of the vector.
   * \return  The l1-norm of the vector.
   */
  virtual double l1_norm() const = 0;

  /**
   * \brief   The l2-norm of the vector.
   * \return  The l2-norm of the vector.
   */
  virtual double l2_norm() const = 0;

  /**
   * \brief   The l-infintiy-norm of the vector.
   * \return  The l-infintiy-norm of the vector.
   */
  virtual double sup_norm() const = 0;

  /**
   * \brief Extract components of the vector.
   * \param component_indices Indices of the vector components that are to be returned.
   * \return                  A Dune::DynamicVector `result` such that `result[i]` is the `component_indices[i]`-th
                              component of the vector.
   */
  virtual std::vector< double > components(const std::vector< int >& component_indices) const = 0;

  /**
   * \brief   The maximum absolute value of the vector.
   * \return  A pair< max_ind, max_val >, where max_ind is the index at which the maximum is attained and max_val is the
   *          absolute value.
   */
  virtual std::vector< double > amax() const = 0;

  /**
   * \brief   Adds two vectors.
   * \param   other The right summand.
   * \return        The sum of this and other.
   */
  virtual VectorInterface* add(const VectorInterface* other) const = 0;

  /**
   * \brief Inplace variant of add().
   * \param other The right summand.
   */
  virtual void iadd(const VectorInterface* other) = 0;

  /**
   * \brief   Subtracts two vectors.
   * \param   other The subtrahend.
   * \return        The difference between this and other.
   */
  virtual VectorInterface* sub(const VectorInterface* other) const = 0;

  /**
   * \brief Inplace variant of sub().
   * \param other The subtrahend.
   */
  virtual void isub(const VectorInterface* other) = 0;
}; // class VectorInterface


} // namespace LA
} // namespace Pymor
} // namespace Dune

#endif // DUNE_PYMOR_LA_CONTAINER_INTERFACES_HH
