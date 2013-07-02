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
#include <dune/common/float_cmp.hh>

#include <dune/stuff/common/float_cmp.hh>

#include "interfaces.hh"

namespace Dune {
namespace Pymor {
namespace LA {


/**
 * \brief An implementation of Dune::Pymor::LA::VectorInterface using Dune::DynamicVector< double >.
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

  virtual std::string type() const
  {
    return "dunepymor.vector.dunedynamic";
  }

  virtual std::vector< std::string > compatibleTypes() const
  {
    return {
        type()
    };
  }

  virtual int dim() const
  {
    return BaseType::size();
  }

  virtual bool almost_equal(const ThisType* other,
                            const double epsilon = Dune::FloatCmp::DefaultEpsilon< double >::value()) const
  {
    assert(dim() == other->dim() && "Sizes do not match!");
    return Dune::Stuff::Common::FloatCmp::eq(*this, *other, epsilon);
  } // ... almost_equal(...)

  virtual void scal(const double alpha)
  {
    BaseType::operator*=(alpha);
  }

  virtual void axpy(const double alpha, const ThisType* x)
  {
    assert(dim() == x->dim() && "Sizes do not match!");
    for (int ii = 0; ii < dim(); ++ii)
      BaseType::operator[](ii) += alpha * x->operator[](ii);
  } // ... axpy(...)

  virtual double dot(const ThisType* other) const
  {
    assert(dim() == other->dim() && "Sizes do not match!");
    double result = 0;
    for (int ii = 0; ii < dim(); ++ii)
      result += BaseType::operator[](ii) * other->operator[](ii);
    return result;
  } // ... dot(...)

  virtual double l1_norm() const
  {
    return BaseType::one_norm();
  }

  virtual double l2_norm() const
  {
    return BaseType::two_norm();
  }

  virtual double sup_norm() const
  {
    return BaseType::infinity_norm();
  }

  virtual std::vector< double > components(const std::vector< int >& component_indices) const
  {
    assert(int(component_indices.size()) <= dim() && "Sizes do not match!");
    std::vector< double > values(component_indices.size(), 0);
    for (size_t ii = 0; ii < component_indices.size(); ++ii) {
      const int component = component_indices[ii];
      assert(0 <= component && "Wrong component index given!");
      assert(component < dim() && "Wrong component index given!");
      values[ii] = BaseType::operator[](component);
    }
    return values;
  } // ... components(...)

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

  virtual ThisType* add(const ThisType* other) const
  {
    assert(dim() == other->dim() && "Sizes do not match!");
    ThisType* result = create(other->dim());
    for (int ii = 0; ii < dim(); ++ii)
      result->operator[](ii) = BaseType::operator[](ii) + other->operator[](ii);
    return result;
  } // ... add(...)

  virtual void iadd(const ThisType* other)
  {
    assert(dim() == other->dim() && "Sizes do not match!");
    for (int ii = 0; ii < dim(); ++ii)
      BaseType::operator[](ii) += other->operator[](ii);
  } // ... iadd(...)

  virtual ThisType* sub(const ThisType* other) const
  {
    assert(dim() == other->dim() && "Sizes do not match!");
    ThisType* result = create(other->dim());
    for (int ii = 0; ii < dim(); ++ii)
      result->operator[](ii) = BaseType::operator[](ii) - other->operator[](ii);
    return result;
  } // ... sub(...)

  virtual void isub(const ThisType* other)
  {
    assert(dim() == other->dim() && "Sizes do not match!");
    for (int ii = 0; ii < dim(); ++ii)
      BaseType::operator[](ii) -= other->operator[](ii);
  } // ... isub(...)
}; // class DuneDynamicVector


} // namespace LA
} // namespace Pymor
} // namespace Dune

#endif // DUNE_PYMOR_LA_CONTAINER_DYNAMICVECTOR_HH
