// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_LA_CONTAINER_DYNAMICVECTOR_HH
#define DUNE_PYMOR_LA_CONTAINER_DYNAMICVECTOR_HH

#include <dune/common/dynvector.hh>

#include <dune/stuff/common/float_cmp.hh>

#include "interfaces.hh"

namespace Dune {
namespace Pymor {
namespace LA {


/**
 * \brief An implementation of Dune::Pymor::LA::VectorInterface using Dune::DynamicVector< double >.
 *
 * \see   Dune::Pymor::LA::VectorInterface
 */
class DuneDynamicVector
  : public Dune::DynamicVector< double >
  , public Dune::Pymor::LA::VectorInterface
{
  typedef Dune::DynamicVector< double > BaseType;
public:
  DuneDynamicVector(const int ss, const double value = 0.0);

  DuneDynamicVector(const BaseType& other);

  static DuneDynamicVector* create(const int ss);

  /**
   * \copydoc Dune::Pymor::LA::VectorInterface::type
   * \return  "dunepymor.vector.dunedynamic"
   */
  virtual std::string type() const;

  /**
   * \brief  "dunepymor.vector.dunedynamic"
   * \return "dunepymor.vector.dunedynamic"
   */
  static std::string static_type();

  //! \copydoc Dune::Pymor::LA::VectorInterface::compatibleTypes
  virtual std::vector< std::string > compatibleTypes() const;

  //! \copydoc Dune::Pymor::LA::VectorInterface::dim
  virtual unsigned int dim() const;

  //! \copydoc Dune::Pymor::LA::VectorInterface::almost_equal
  virtual bool almost_equal(const DuneDynamicVector* other,
                            const double epsilon = Dune::FloatCmp::DefaultEpsilon< double >::value()) const
    throw (Exception::types_are_not_compatible, Exception::sizes_do_not_match);

  //! \copydoc Dune::Pymor::LA::VectorInterface::scal
  virtual void scal(const double alpha);

  //! \copydoc Dune::Pymor::LA::VectorInterface::axpy
  virtual void axpy(const double alpha, const DuneDynamicVector* x) throw (Exception::types_are_not_compatible,
                                                                           Exception::sizes_do_not_match);

  //! \copydoc Dune::Pymor::LA::VectorInterface::dot
  virtual double dot(const DuneDynamicVector* other) const throw (Exception::types_are_not_compatible,
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
  virtual DuneDynamicVector* add(const DuneDynamicVector* other) const throw (Exception::types_are_not_compatible,
                                                                              Exception::sizes_do_not_match);

  //! \copydoc Dune::Pymor::LA::VectorInterface::iadd
  virtual void iadd(const DuneDynamicVector* other) throw (Exception::types_are_not_compatible,
                                                           Exception::sizes_do_not_match);

  //! \copydoc Dune::Pymor::LA::VectorInterface::sub
  virtual DuneDynamicVector* sub(const DuneDynamicVector* other) const throw (Exception::types_are_not_compatible,
                                                                              Exception::sizes_do_not_match);

  //! \copydoc Dune::Pymor::LA::VectorInterface::isub
  virtual void isub(const DuneDynamicVector* other) throw (Exception::types_are_not_compatible,
                                                           Exception::sizes_do_not_match);
}; // class DuneDynamicVector


} // namespace LA
} // namespace Pymor
} // namespace Dune

#endif // DUNE_PYMOR_LA_CONTAINER_DYNAMICVECTOR_HH
