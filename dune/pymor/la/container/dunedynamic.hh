// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_LA_CONTAINER_DYNAMICVECTOR_HH
#define DUNE_PYMOR_LA_CONTAINER_DYNAMICVECTOR_HH

#include <utility>
#include <vector>
#include <string>

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
  typedef DuneDynamicVector             ThisType;

  DuneDynamicVector(const int ss)
    : BaseType(ss, 0)
  {}

  DuneDynamicVector(const BaseType& other)
    : BaseType(other)
  {}

  static ThisType* create(const int ss)
  {
    return new ThisType(ss);
  }

  //! \copydoc Dune::Pymor::LA::VectorInterface::type
  virtual std::string type() const
  {
    return "dunepymor.vector.dunedynamic";
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
    BaseType::operator*=(alpha);
  }

  //! \copydoc Dune::Pymor::LA::VectorInterface::axpy
  virtual void axpy(const double alpha, const ThisType* x) throw (Exception::types_are_not_compatible,
                                                                  Exception::sizes_do_not_match)
  {
    if (dim() != x->dim())
      DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                       "size of x (" << x->dim() << ") does not match size of this (" << dim() << ")!");
    for (int ii = 0; ii < dim(); ++ii)
      BaseType::operator[](ii) += alpha * x->operator[](ii);
  } // ... axpy(...)

  //! \copydoc Dune::Pymor::LA::VectorInterface::dot
  virtual double dot(const ThisType* other) const throw (Exception::types_are_not_compatible,
                                                         Exception::sizes_do_not_match)
  {
    if (dim() != other->dim())
      DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                       "size of other (" << other->dim() << ") does not match size of this (" << dim() << ")!");
    double result = 0;
    for (int ii = 0; ii < dim(); ++ii)
      result += BaseType::operator[](ii) * other->operator[](ii);
    return result;
  } // ... dot(...)

  //! \copydoc Dune::Pymor::LA::VectorInterface::l1_norm
  virtual double l1_norm() const
  {
    return BaseType::one_norm();
  }

  //! \copydoc Dune::Pymor::LA::VectorInterface::l2_norm
  virtual double l2_norm() const
  {
    return BaseType::two_norm();
  }

  //! \copydoc Dune::Pymor::LA::VectorInterface::sup_norm
  virtual double sup_norm() const
  {
    return BaseType::infinity_norm();
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
      values[ii] = BaseType::operator[](component);
    }
    return values;
  } // ... components(...)

  //! \copydoc Dune::Pymor::LA::VectorInterface::amax
  virtual std::vector< double > amax() const
  {
    std::vector< double > result(2, 0.0);
    for (int ii = 0; ii < dim(); ++ii) {
      const double value = std::abs(BaseType::operator[](ii));
      if (value > result[1]) {
        result[0] = ii;
        result[1] = value;
      }
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
    ThisType* result = create(other->dim());
    for (int ii = 0; ii < dim(); ++ii)
      result->operator[](ii) = BaseType::operator[](ii) + other->operator[](ii);
    return result;
  } // ... add(...)

  //! \copydoc Dune::Pymor::LA::VectorInterface::iadd
  virtual void iadd(const ThisType* other) throw (Exception::types_are_not_compatible,
                                                  Exception::sizes_do_not_match)
  {
    if (dim() != other->dim())
      DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                       "size of other (" << other->dim() << ") does not match size of this (" << dim() << ")!");
    for (int ii = 0; ii < dim(); ++ii)
      BaseType::operator[](ii) += other->operator[](ii);
  } // ... iadd(...)

  //! \copydoc Dune::Pymor::LA::VectorInterface::sub
  virtual ThisType* sub(const ThisType* other) const throw (Exception::types_are_not_compatible,
                                                            Exception::sizes_do_not_match)
  {
    if (dim() != other->dim())
      DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                       "size of other (" << other->dim() << ") does not match size of this (" << dim() << ")!");
    ThisType* result = create(other->dim());
    for (int ii = 0; ii < dim(); ++ii)
      result->operator[](ii) = BaseType::operator[](ii) - other->operator[](ii);
    return result;
  } // ... sub(...)

  //! \copydoc Dune::Pymor::LA::VectorInterface::isub
  virtual void isub(const ThisType* other) throw (Exception::types_are_not_compatible,
                                                  Exception::sizes_do_not_match)
  {
    if (dim() != other->dim())
      DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                       "size of other (" << other->dim() << ") does not match size of this (" << dim() << ")!");
    for (int ii = 0; ii < dim(); ++ii)
      BaseType::operator[](ii) -= other->operator[](ii);
  } // ... isub(...)
}; // class DuneDynamicVector


} // namespace LA
} // namespace Pymor
} // namespace Dune

#endif // DUNE_PYMOR_LA_CONTAINER_DYNAMICVECTOR_HH
