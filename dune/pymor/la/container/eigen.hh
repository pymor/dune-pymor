// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_LA_CONTAINER_EIGEN_HH
#define DUNE_PYMOR_LA_CONTAINER_EIGEN_HH

#if HAVE_EIGEN

#include <dune/stuff/common/float_cmp.hh>
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
  typedef EigenDenseVector                            ThisType;

  EigenDenseVector(const int ss)
    : BaseType(ss)
  {}

  EigenDenseVector(const BaseType& other)
    : BaseType(other)
  {}

  static ThisType* create(const int ss)
  {
    return new ThisType(ss);
  }

  //! \copydoc Dune::Pymor::LA::VectorInterface::type
  virtual std::string type() const
  {
    return "dunepymor.vector.eigendense";
  }

  //! \copydoc Dune::Pymor::LA::VectorInterface::compatibleTypes
  virtual std::vector< std::string > compatibleTypes() const
  {
    return {
          type()
    };
  }

  //! \copydoc Dune::Pymor::LA::VectorInterface::dim
  virtual int dim() const
  {
    return BaseType::size();
  }

  //! \copydoc Dune::Pymor::LA::VectorInterface::almost_equal
  virtual bool almost_equal(const ThisType* other,
                            const double epsilon = Dune::FloatCmp::DefaultEpsilon< double >::value()) const
    throw (Exception::types_are_not_compatible, Exception::sizes_do_not_match)
  {
    if (dim() != other->dim())
      DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                       "size of other (" << other->dim() << ") does not match size of this (" << dim() << ")!");
    return Dune::Stuff::Common::FloatCmp::eq(*this, *other, epsilon);
  } // ... almost_equal(...)

  //! \copydoc Dune::Pymor::LA::VectorInterface::scal
  virtual void scal(const double alpha)
  {
    BaseType::backend() *= alpha;
  }

  //! \copydoc Dune::Pymor::LA::VectorInterface::axpy
  virtual void axpy(const double alpha, const ThisType* x) throw (Exception::types_are_not_compatible,
                                                                  Exception::sizes_do_not_match)
  {
    if (dim() != x->dim())
      DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                       "size of x (" << x->dim() << ") does not match size of this (" << dim() << ")!");
    BaseType::backend() += x->backend() * alpha;
  } // ... axpy(...)

  //! \copydoc Dune::Pymor::LA::VectorInterface::dot
  virtual double dot(const ThisType* other) const throw (Exception::types_are_not_compatible,
                                                         Exception::sizes_do_not_match)
  {
    if (dim() != other->dim())
      DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                       "size of other (" << other->dim() << ") does not match size of this (" << dim() << ")!");
    return BaseType::backend().transpose() * other->backend();
  } // ... dot(...)

  //! \copydoc Dune::Pymor::LA::VectorInterface::l1_norm
  virtual double l1_norm() const
  {
    return BaseType::backend().lpNorm< 1 >();
  }

  //! \copydoc Dune::Pymor::LA::VectorInterface::l2_norm
  virtual double l2_norm() const
  {
    return BaseType::backend().norm();
  }

  //! \copydoc Dune::Pymor::LA::VectorInterface::sup_norm
  virtual double sup_norm() const
  {
    return BaseType::backend().lpNorm< ::Eigen::Infinity >();
  }

  //! \copydoc Dune::Pymor::LA::VectorInterface::components
  virtual std::vector< double > components(const std::vector< int >& component_indices) const
    throw (Exception::sizes_do_not_match, Exception::index_out_of_range)
  {
    if (int(component_indices.size()) > dim())
      DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                       "size of component_indices (" << component_indices.size() << ") is larger than this (" << dim() << ")!");
    std::vector< double > values(component_indices.size(), 0);
    for (size_t ii = 0; ii < component_indices.size(); ++ii) {
      const int component = component_indices[ii];
      if (component < 0)
        DUNE_PYMOR_THROW(Exception::index_out_of_range,
                         "component_indices[" << ii << "] is negative (" << component << ")!");
      if (component >= dim())
        DUNE_PYMOR_THROW(Exception::index_out_of_range,
                         "component_indices[" << ii << "] is too large for this (" << dim() << ")!");
      values[ii] = BaseType::backend()[component];
    }
    return values;
  } // ... components(...)

  //! \copydoc Dune::Pymor::LA::VectorInterface::amax
  virtual std::vector< double > amax() const
  {
    std::vector< double > result(2, 0.0);
    size_t minIndex = 0;
    size_t maxIndex = 0;
    const double minimum = BaseType::backend().minCoeff(&minIndex);
    const double maximum = BaseType::backend().maxCoeff(&maxIndex);
    if (std::abs(maximum) >= std::abs(minimum)) {
      result[0] = minIndex;
      result[1] = std::abs(minimum);
    } else {
      result[0] = maxIndex;
      result[1] = maximum;
    }
    return result;
  } // ... amax(...)

  //! \copydoc Dune::Pymor::LA::VectorInterface::add
  virtual ThisType* add(const ThisType* other) const throw (Exception::types_are_not_compatible,
                                                            Exception::sizes_do_not_match)
  {
    if (dim() != other->dim())
      DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                       "size of other (" << other->dim() << ") does not match size of this (" << dim() << ")!");
    ThisType* result = new ThisType(*this);
    result->backend() += other->backend();
    return result;
  } // ... add(...)

  //! \copydoc Dune::Pymor::LA::VectorInterface::iadd
  virtual void iadd(const ThisType* other) throw (Exception::types_are_not_compatible,
                                                  Exception::sizes_do_not_match)
  {
    if (dim() != other->dim())
      DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                       "size of other (" << other->dim() << ") does not match size of this (" << dim() << ")!");
    BaseType::backend() += other->backend();
  } // ... iadd(...)

  //! \copydoc Dune::Pymor::LA::VectorInterface::sub
  virtual ThisType* sub(const ThisType* other) const throw (Exception::types_are_not_compatible,
                                                            Exception::sizes_do_not_match)
  {
    if (dim() != other->dim())
      DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                       "size of other (" << other->dim() << ") does not match size of this (" << dim() << ")!");
    ThisType* result = new ThisType(*this);
    result->backend() -= other->backend();
    return result;
  } // ... sub(...)

  //! \copydoc Dune::Pymor::LA::VectorInterface::isub
  virtual void isub(const ThisType* other) throw (Exception::types_are_not_compatible,
                                                  Exception::sizes_do_not_match)
  {
    if (dim() != other->dim())
      DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                       "size of other (" << other->dim() << ") does not match size of this (" << dim() << ")!");
    BaseType::backend() -= other->backend();
  } // ... isub(...)
}; // class EigenDenseVector

} // namespace LA
} // namespace Pymor
} // namespace Dune

#endif // HAVE_EIGEN

#endif // DUNE_PYMOR_LA_CONTAINER_EIGEN_HH
