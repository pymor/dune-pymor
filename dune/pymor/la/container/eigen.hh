// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_LA_CONTAINER_EIGEN_HH
#define DUNE_PYMOR_LA_CONTAINER_EIGEN_HH

//#if HAVE_EIGEN

#include <dune/stuff/common/float_cmp.hh>
#include <dune/stuff/la/container/eigen.hh>

#include "interfaces.hh"

namespace Dune {
namespace Pymor {
namespace LA {


class EigenDenseVector
  : public Dune::Stuff::LA::EigenDenseVector< double >
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

  virtual std::string type() const
  {
    return "dunepymor.vector.eigendense";
  }

  virtual int dim() const
  {
    return BaseType::size();
  }

  bool almost_equal(const ThisType* other,
                    const double epsilon = Dune::FloatCmp::DefaultEpsilon< double >::value()) const
  {
    assert(dim() == other->dim() && "Sizes do not match!");
    return Dune::Stuff::Common::FloatCmp::eq(*this, *other, epsilon);
  } // ... almost_equal(...)

  void scal(const double alpha)
  {
    BaseType::backend() *= alpha;
  }

  void axpy(const double alpha, const ThisType* x)
  {
    assert(dim() == x->dim() && "Sizes do not match!");
    BaseType::backend() += x->backend() * alpha;
  } // ... axpy(...)

  double dot(const ThisType* other) const
  {
    assert(dim() == other->dim() && "Sizes do not match!");
    return BaseType::backend().transpose() * other->backend();
  } // ... dot(...)

  double l1_norm() const
  {
    return BaseType::backend().lpNorm< 1 >();
  }

  double l2_norm() const
  {
    return BaseType::backend().norm();
  }

  double sup_norm() const
  {
    return BaseType::backend().lpNorm< ::Eigen::Infinity >();
  }

  std::vector< double > components(const std::vector< int >& component_indices) const
  {
    assert(int(component_indices.size()) <= dim() && "Sizes do not match!");
    std::vector< double > values(component_indices.size(), 0);
    for (size_t ii = 0; ii < component_indices.size(); ++ii) {
      const int component = component_indices[ii];
      assert(0 <= component && "Wrong component index given!");
      assert(component < dim() && "Wrong component index given!");
      values[ii] = BaseType::backend()[component];
    }
    return values;
  } // ... components(...)

  std::vector< double > amax() const
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

  ThisType* add(const ThisType* other) const
  {
    assert(dim() == other->dim() && "Sizes do not match!");
    ThisType* result = new ThisType(*this);
    result->backend() += other->backend();
    return result;
  } // ... add(...)

  void iadd(const ThisType* other)
  {
    assert(dim() == other->dim() && "Sizes do not match!");
    BaseType::backend() += other->backend();
  } // ... iadd(...)

  ThisType* sub(const ThisType* other) const
  {
    assert(dim() == other->dim() && "Sizes do not match!");
    ThisType* result = new ThisType(*this);
    result->backend() -= other->backend();
    return result;
  } // ... sub(...)

  void isub(const ThisType* other)
  {
    assert(dim() == other->dim() && "Sizes do not match!");
    BaseType::backend() -= other->backend();
  } // ... isub(...)
}; // class EigenDenseVector

} // namespace LA
} // namespace Pymor
} // namespace Dune

//#endif // HAVE_EIGEN

#endif // DUNE_PYMOR_LA_CONTAINER_EIGEN_HH
