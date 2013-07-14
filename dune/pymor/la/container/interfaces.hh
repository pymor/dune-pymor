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

#include <dune/pymor/common/exceptions.hh>
#include <dune/pymor/functionals/interfaces.hh>

namespace Dune {
namespace Pymor {
namespace LA {


/**
 * \brief Interface for all vectors in dune-pymor.
 *
 *        Python bindings for any class DerivedFoo derived from this VectorInterface can be obtained by calling
 * \code
from dune.pymor.la.container import add_VectorInterface as add_dune_Vector
module, Vector = add_dune_Vector(module, 'DerivedFoo')
\endcode
 *        in your python code, where DerivedFoo is a derived class, i.e. Dune::Pymor::LA::DuneDynamicVector and module
 *        is a pybindgen.module.Module.
 * \note  All derived classes are expected to implement the following static method:
 * \code
static std::string static_type();
\endcode
 *        where static_type() is expected to return the same as type();
 */
class VectorInterface
  : public Dune::Pymor::FunctionalInterface
{
public:
  virtual ~VectorInterface();

  /**
   * \brief   A unique (per class, not per instance) identifier.
   * \return  A unique (per class, not per instance) identifier.
   */
  virtual std::string type() const = 0;

  /**
   * \brief   The dimension of the vector.
   * \return  The dimension of the vector.
   */
  virtual unsigned int dim() const = 0;

  virtual VectorInterface* copy() const = 0;

  /**
   * \brief   Check vectors for equality.
   *          Equality of two vectors is defined as in Dune::FloatCmp componentwise.
   * \param   other   A vector of same dimension to compare with.
   * \param   epsilon See Dune::FloatCmp.
   * \return          Truth value of the comparison.
   */
  virtual bool almost_equal(const Dune::Pymor::LA::VectorInterface* other,
                            const double /*epsilon*/ = Dune::FloatCmp::DefaultEpsilon< double >::value()) const
    throw (Exception::types_are_not_compatible, Exception::sizes_do_not_match, Exception::you_have_to_implement_this);

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
  virtual void axpy(const double /*alpha*/, const Dune::Pymor::LA::VectorInterface* x)
    throw (Exception::types_are_not_compatible, Exception::sizes_do_not_match, Exception::you_have_to_implement_this);

  /**
   * \brief   Computes the scalar products between two vectors.
   * \param   other The second factor.
   * \return        The scalar product.
   */
  virtual double dot(const Dune::Pymor::LA::VectorInterface* other) const
    throw (Exception::types_are_not_compatible, Exception::sizes_do_not_match, Exception::you_have_to_implement_this);

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
   * \return                  A std::vector< double > `result` such that `result[i]` is the `component_indices[i]`-th
                              component of the vector.
   */
  virtual std::vector< double > components(const std::vector< int >& component_indices) const
    throw (Exception::sizes_do_not_match, Exception::index_out_of_range) = 0;

  /**
   * \brief   The maximum absolute value of the vector.
   * \return  A std::vector< double > result, where int(result[0]) is the index at which the maximum is attained and
   *          result[1] is the absolute maximum value.
   */
  virtual std::vector< double > amax() const = 0;

  /**
   * \brief   Adds two vectors.
   * \param   other The right summand.
   * \return        The sum of this and other.
   */
  virtual Dune::Pymor::LA::VectorInterface* add(const Dune::Pymor::LA::VectorInterface* other) const
    throw (Exception::types_are_not_compatible, Exception::sizes_do_not_match, Exception::you_have_to_implement_this);

  /**
   * \brief Inplace variant of add().
   * \param other The right summand.
   */
  virtual void iadd(const Dune::Pymor::LA::VectorInterface* other)
    throw (Exception::types_are_not_compatible, Exception::sizes_do_not_match, Exception::you_have_to_implement_this);

  /**
   * \brief   Subtracts two vectors.
   * \param   other The subtrahend.
   * \return        The difference between this and other.
   */
  virtual Dune::Pymor::LA::VectorInterface* sub(const Dune::Pymor::LA::VectorInterface* other) const
    throw (Exception::types_are_not_compatible, Exception::sizes_do_not_match, Exception::you_have_to_implement_this);

  /**
   * \brief Inplace variant of sub().
   * \param other The subtrahend.
   */
  virtual void isub(const Dune::Pymor::LA::VectorInterface* other)
    throw (Exception::types_are_not_compatible, Exception::sizes_do_not_match, Exception::you_have_to_implement_this);

  virtual bool linear() const;

  virtual unsigned int dim_source() const;

  virtual std::string type_source() const;

  virtual double apply(const LA::VectorInterface* source,
                       const Parameter /*mu*/ = Parameter()) const throw (Exception::types_are_not_compatible,
                                                                          Exception::you_have_to_implement_this,
                                                                          Exception::sizes_do_not_match,
                                                                          Exception::wrong_parameter_type,
                                                                          Exception::requirements_not_met,
                                                                          Exception::linear_solver_failed,
                                                                          Exception::this_does_not_make_any_sense);
}; // class VectorInterface


} // namespace LA
} // namespace Pymor
} // namespace Dune

#endif // DUNE_PYMOR_LA_CONTAINER_INTERFACES_HH
