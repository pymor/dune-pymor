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
namespace LA {


/**
 * \brief An implementation of Dune::Pymor::LA::VectorInterface using Dune::Stuff::LA::EigenDenseVector< double >.
 *
 * \see   Dune::Pymor::LA::VectorInterface
 */
class EigenDenseVector
  : public Dune::Stuff::LA::EigenDenseVector< double >
  , public Dune::Pymor::LA::VectorInterface
{
  typedef Dune::Stuff::LA::EigenDenseVector< double > BaseType;
public:
  typedef EigenDenseVector SourceType;

  EigenDenseVector(const int ss, const double value = 0.0);

  EigenDenseVector(const BaseType& other);

  static EigenDenseVector* create(const int ss);

  /**
   * \copydoc Dune::Pymor::LA::VectorInterface::type
   * \return  "dunepymor.vector.eigendense"
   */
  virtual std::string type() const;

  /**
   * \brief  "dunepymor.vector.eigendense"
   * \return "dunepymor.vector.eigendense"
   */
  static std::string static_type();

  //! \copydoc Dune::Pymor::LA::VectorInterface::compatibleTypes
  virtual std::vector< std::string > compatibleTypes() const;

  virtual EigenDenseVector* copy() const;

  //! \copydoc Dune::Pymor::LA::VectorInterface::dim
  virtual unsigned int dim() const;

  //! \copydoc Dune::Pymor::LA::VectorInterface::almost_equal
  virtual bool almost_equal(const EigenDenseVector* other,
                            const double epsilon = Dune::FloatCmp::DefaultEpsilon< double >::value()) const
    throw (Exception::types_are_not_compatible, Exception::sizes_do_not_match);

  //! \copydoc Dune::Pymor::LA::VectorInterface::scal
  virtual void scal(const double alpha);

  //! \copydoc Dune::Pymor::LA::VectorInterface::axpy
  virtual void axpy(const double alpha, const EigenDenseVector* x) throw (Exception::types_are_not_compatible,
                                                                          Exception::sizes_do_not_match);

  //! \copydoc Dune::Pymor::LA::VectorInterface::dot
  virtual double dot(const EigenDenseVector* other) const throw (Exception::types_are_not_compatible,
                                                                 Exception::sizes_do_not_match);

  //! \copydoc Dune::Pymor::LA::VectorInterface::l1_norm
  virtual double l1_norm() const;

  //! \copydoc Dune::Pymor::LA::VectorInterface::l2_norm
  virtual double l2_norm() const;

  //! \copydoc Dune::Pymor::LA::VectorInterface::sup_norm
  virtual double sup_norm() const;

  //! \copydoc Dune::Pymor::LA::VectorInterface::components
  virtual std::vector< double > components(const std::vector< int >& component_indices) const
    throw (Exception::sizes_do_not_match, Exception::index_out_of_range);

  //! \copydoc Dune::Pymor::LA::VectorInterface::amax
  virtual std::vector< double > amax() const;

  //! \copydoc Dune::Pymor::LA::VectorInterface::add
  virtual EigenDenseVector* add(const EigenDenseVector* other) const throw (Exception::types_are_not_compatible,
                                                                            Exception::sizes_do_not_match);

  //! \copydoc Dune::Pymor::LA::VectorInterface::iadd
  virtual void iadd(const EigenDenseVector* other) throw (Exception::types_are_not_compatible,
                                                          Exception::sizes_do_not_match);

  //! \copydoc Dune::Pymor::LA::VectorInterface::sub
  virtual EigenDenseVector* sub(const EigenDenseVector* other) const throw (Exception::types_are_not_compatible,
                                                                            Exception::sizes_do_not_match);

  //! \copydoc Dune::Pymor::LA::VectorInterface::isub
  virtual void isub(const EigenDenseVector* other) throw (Exception::types_are_not_compatible,
                                                          Exception::sizes_do_not_match);

  virtual double apply(const EigenDenseVector* source,
                       const Parameter mu = Parameter()) const throw (Exception::types_are_not_compatible,
                                                                      Exception::you_have_to_implement_this,
                                                                      Exception::sizes_do_not_match,
                                                                      Exception::wrong_parameter_type,
                                                                      Exception::requirements_not_met,
                                                                      Exception::linear_solver_failed,
                                                                      Exception::this_does_not_make_any_sense);
}; // class EigenDenseVector

} // namespace LA
} // namespace Pymor
} // namespace Dune

#endif // HAVE_EIGEN
#endif // DUNE_PYMOR_LA_CONTAINER_EIGEN_HH
