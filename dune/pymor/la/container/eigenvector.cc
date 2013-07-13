// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifdef HAVE_CMAKE_CONFIG
  #include "cmake_config.h"
#elif defined (HAVE_CONFIG_H)
  #include "config.h"
#endif // HAVE_CMAKE_CONFIG

#if HAVE_EIGEN

#include <dune/stuff/common/float_cmp.hh>

#include "eigenvector.hh"

namespace Dune {
namespace Pymor {
namespace LA {


EigenDenseVector::EigenDenseVector(const int ss, const double value)
  : BaseType(ss, value)
{}

EigenDenseVector::EigenDenseVector(const BaseType& other)
  : BaseType(other)
{}

EigenDenseVector* EigenDenseVector::create(const int ss)
{
  return new EigenDenseVector(ss);
}

std::string EigenDenseVector::type() const
{
  return "dunepymor.vector.eigendense";
}

std::string EigenDenseVector::static_type()
{
  return "dunepymor.vector.eigendense";
}

std::vector< std::string > EigenDenseVector::compatibleTypes() const
{
  return { type() };
}

EigenDenseVector* EigenDenseVector::copy() const
{
  return new EigenDenseVector(*this);
}

unsigned int EigenDenseVector::dim() const
{
  return BaseType::size();
}

bool EigenDenseVector::almost_equal(const EigenDenseVector* other, const double epsilon) const
  throw (Exception::types_are_not_compatible, Exception::sizes_do_not_match)
{
  if (dim() != other->dim())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "size of other (" << other->dim() << ") does not match size of this (" << dim() << ")!");
  return Dune::Stuff::Common::FloatCmp::eq(*this, *other, epsilon);
} // ... almost_equal(...)

void EigenDenseVector::scal(const double alpha)
{
  BaseType::backend() *= alpha;
}

void EigenDenseVector::axpy(const double alpha, const EigenDenseVector* x) throw (Exception::types_are_not_compatible,
                                                                                  Exception::sizes_do_not_match)
{
  if (dim() != x->dim())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "size of x (" << x->dim() << ") does not match size of this (" << dim() << ")!");
  BaseType::backend() += x->backend() * alpha;
} // ... axpy(...)

double EigenDenseVector::dot(const EigenDenseVector* other) const throw (Exception::types_are_not_compatible,
                                                                         Exception::sizes_do_not_match)
{
  if (dim() != other->dim())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "size of other (" << other->dim() << ") does not match size of this (" << dim() << ")!");
  return BaseType::backend().transpose() * other->backend();
} // ... dot(...)

double EigenDenseVector::l1_norm() const
{
  return BaseType::backend().lpNorm< 1 >();
}

double EigenDenseVector::l2_norm() const
{
  return BaseType::backend().norm();
}

double EigenDenseVector::sup_norm() const
{
  return BaseType::backend().lpNorm< ::Eigen::Infinity >();
}

std::vector< double > EigenDenseVector::components(const std::vector< int >& component_indices) const
  throw (Exception::sizes_do_not_match, Exception::index_out_of_range)
{
  if (component_indices.size() > dim())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "size of component_indices (" << component_indices.size() << ") is larger than this (" << dim() << ")!");
  std::vector< double > values(component_indices.size(), 0);
  for (size_t ii = 0; ii < component_indices.size(); ++ii) {
    const int component = component_indices[ii];
    if (component < 0)
      DUNE_PYMOR_THROW(Exception::index_out_of_range,
                       "component_indices[" << ii << "] is negative (" << component << ")!");
    if (component >= int(dim()))
      DUNE_PYMOR_THROW(Exception::index_out_of_range,
                       "component_indices[" << ii << "] is too large for this (" << dim() << ")!");
    values[ii] = BaseType::backend()[component];
  }
  return values;
} // ... components(...)

std::vector< double > EigenDenseVector::amax() const
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

EigenDenseVector* EigenDenseVector::add(const EigenDenseVector* other) const throw (Exception::types_are_not_compatible,
                                                                                    Exception::sizes_do_not_match)
{
  if (dim() != other->dim())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "size of other (" << other->dim() << ") does not match size of this (" << dim() << ")!");
  EigenDenseVector* result = new EigenDenseVector(*this);
  result->backend() += other->backend();
  return result;
} // ... add(...)

void EigenDenseVector::iadd(const EigenDenseVector* other) throw (Exception::types_are_not_compatible,
                                                                  Exception::sizes_do_not_match)
{
  if (dim() != other->dim())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "size of other (" << other->dim() << ") does not match size of this (" << dim() << ")!");
  BaseType::backend() += other->backend();
} // ... iadd(...)

EigenDenseVector* EigenDenseVector::sub(const EigenDenseVector* other) const throw (Exception::types_are_not_compatible,
                                                                                    Exception::sizes_do_not_match)
{
  if (dim() != other->dim())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "size of other (" << other->dim() << ") does not match size of this (" << dim() << ")!");
  EigenDenseVector* result = new EigenDenseVector(*this);
  result->backend() -= other->backend();
  return result;
} // ... sub(...)

void EigenDenseVector::isub(const EigenDenseVector* other) throw (Exception::types_are_not_compatible,
                                                                  Exception::sizes_do_not_match)
{
  if (dim() != other->dim())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "size of other (" << other->dim() << ") does not match size of this (" << dim() << ")!");
  BaseType::backend() -= other->backend();
} // ... isub(...)

double EigenDenseVector::apply(const EigenDenseVector* source,
                               const Parameter mu) const throw (Exception::types_are_not_compatible,
                                                                Exception::you_have_to_implement_this,
                                                                Exception::sizes_do_not_match,
                                                                Exception::wrong_parameter_type,
                                                                Exception::requirements_not_met,
                                                                Exception::linear_solver_failed,
                                                                Exception::this_does_not_make_any_sense)
{
  if (!mu.empty())
    DUNE_PYMOR_THROW(Exception::wrong_parameter_type, "since parametric() == false, mu has to be empty (is "
                     << mu << ")!");
  return dot(source);
}


} // namespace LA
} // namespace Pymor
} // namespace Dune

#endif // HAVE_EIGEN
