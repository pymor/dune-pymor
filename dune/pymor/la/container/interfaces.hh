// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_LA_CONTAINER_INTERFACES_HH
#define DUNE_PYMOR_LA_CONTAINER_INTERFACES_HH

#include <utility>
#include <vector>

#include <dune/common/float_cmp.hh>

namespace Dune {
namespace pymor {
namespace LA {


template< class ScalarImp = double >
class VectorInterface
{
public:
  typedef ScalarImp ScalarType;

  VectorInterface() = delete;

  virtual ~VectorInterface() {}

  /**
   * \brief   A unique (per class, not per instance) identifier.
   * \return
   */
  virtual std::string type() const = delete;

  /**
   * \brief   The dimension of the vector.
   * \return  The dimension of the vector.
   */
  virtual size_t dim() const = delete;

  /**
   * \brief   Check vectors for equality.
   *          Equality of two vectors is defined as in Dune::FloatCmp componentwise.
   * \param   other   A vector of same dimension to compare with.
   * \param   epsilon See Dune::FloatCmp.
   * \return          Truth value of the comparison.
   */
  virtual bool almost_equal(const VectorInterface* other,
                            const ScalarType epsilon = Dune::DefaultEpsilon< ScalarType >::value()) const = delete;

  /**
   * \brief BLAS SCAL operation (in-place sclar multiplication).
   * \param alpha The scalar coefficient with which the vector is multiplied.
   */
  virtual void scal(const ScalarType alpha) = delete;

  /**
   * \brief BLAS AXPY operation.
   * \param alpha The scalar coefficient with which the vector is multiplied
   * \param x     Vector that is to be added.
   */
  virtual void axpy(const ScalarType alpha, const VectorInterface* x) = delete;

  /**
   * \brief   Computes the scalar products between two vectors.
   * \param   other The second factor.
   * \return        The scalar product.
   */
  virtual ScalarType dot(const VectorInterface* other) const = delete;

  /**
   * \brief   The l1-norm of the vector.
   * \return  The l1-norm of the vector.
   */
  virtual ScalarType l1_norm() const = delete;

  /**
   * \brief   The l2-norm of the vector.
   * \return  The l2-norm of the vector.
   */
  virtual ScalarType l2_norm() const = delete;

  /**
   * \brief   The l-infintiy-norm of the vector.
   * \return  The l-infintiy-norm of the vector.
   */
  virtual ScalarType sup_norm() const = delete;

  /**
   * \brief Extract components of the vector.
   * \param component_indices Indices of the vector components that are to be returned.
   * \return                  A Dune::DynamicVector `result` such that `result[i]` is the `component_indices[i]`-th
                              component of the vector.
   */
  virtual std::vector< ScalarType >* components(const std::vector< size_t >* component_indices) const = delete;

  /**
   * \brief   The maximum absolute value of the vector.
   * \return  A pair< max_ind, max_val >, where max_ind is the index at which the maximum is attained and max_val is the
   *          absolute value.
   */
  virtual std::pair< size_t, ScalarType > amax() const = delete;

  /**
   * \brief   Adds two vectors.
   * \param   other The right summand.
   * \return        The sum of this and other.
   */
  virtual VectorInterface* add(const VectorInterface* other) const = delete;

  /**
   * \brief Inplace variant of add().
   * \param other The right summand.
   */
  virtual void iadd(const VectorInterface* other) = delete;

  /**
   * \brief   Subtracts two vectors.
   * \param   other The subtrahend.
   * \return        The difference between this and other.
   */
  virtual VectorInterface* sub(const VectorInterface* other) const = delete;

  /**
   * \brief Inplace variant of sub().
   * \param other The subtrahend.
   */
  virtual void isub(const VectorInterface* other) = delete;
}; // class VectorInterface


} // namespace LA
} // namespace pymor
} // namespace Dune

#endif // DUNE_PYMOR_LA_CONTAINER_INTERFACES_HH
