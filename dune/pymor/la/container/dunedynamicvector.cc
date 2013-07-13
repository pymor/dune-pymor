// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#include <utility>
#include <vector>
#include <string>

#include <dune/stuff/common/float_cmp.hh>

#include "dunedynamicvector.hh"

namespace Dune {
namespace Pymor {
namespace LA {


DuneDynamicVector::DuneDynamicVector(const int ss, const double value)
  : BaseType(ss, value)
{}

DuneDynamicVector::DuneDynamicVector(const BaseType& other)
  : BaseType(other)
{}

DuneDynamicVector* DuneDynamicVector::create(const int ss)
{
  return new DuneDynamicVector(ss);
}

std::string DuneDynamicVector::type() const
{
  return "dunepymor.vector.dunedynamic";
}

std::string DuneDynamicVector::static_type()
{
  return "dunepymor.vector.dunedynamic";
}

std::vector< std::string > DuneDynamicVector::compatibleTypes() const
{
  return { type() };
}

DuneDynamicVector* DuneDynamicVector::copy() const
{
  return new DuneDynamicVector(*this);
}

unsigned int DuneDynamicVector::dim() const
{
  return BaseType::size();
}

bool DuneDynamicVector::almost_equal(const DuneDynamicVector* other,
                                     const double epsilon) const
  throw (Exception::types_are_not_compatible, Exception::sizes_do_not_match)
{
  if (dim() != other->dim())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "size of other (" << other->dim() << ") does not match size of this (" << dim() << ")!");
  return Dune::Stuff::Common::FloatCmp::eq(*this, *other, epsilon);
} // ... almost_equal(...)

void DuneDynamicVector::scal(const double alpha)
{
  BaseType::operator*=(alpha);
}

void DuneDynamicVector::axpy(const double alpha, const DuneDynamicVector* x) throw (Exception::types_are_not_compatible,
                                                                                    Exception::sizes_do_not_match)
{
  if (dim() != x->dim())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "size of x (" << x->dim() << ") does not match size of this (" << dim() << ")!");
  for (size_t ii = 0; ii < dim(); ++ii)
    BaseType::operator[](ii) += alpha * x->operator[](ii);
} // ... axpy(...)

double DuneDynamicVector::dot(const DuneDynamicVector* other) const throw (Exception::types_are_not_compatible,
                                                                           Exception::sizes_do_not_match)
{
  if (dim() != other->dim())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "size of other (" << other->dim() << ") does not match size of this (" << dim() << ")!");
  double result = 0;
  for (size_t ii = 0; ii < dim(); ++ii)
    result += BaseType::operator[](ii) * other->operator[](ii);
  return result;
} // ... dot(...)

double DuneDynamicVector::l1_norm() const
{
  return BaseType::one_norm();
}

double DuneDynamicVector::l2_norm() const
{
  return BaseType::two_norm();
}

double DuneDynamicVector::sup_norm() const
{
  return BaseType::infinity_norm();
}

std::vector< double > DuneDynamicVector::components(const std::vector< int >& component_indices) const
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
    values[ii] = BaseType::operator[](component);
  }
  return values;
} // ... components(...)

std::vector< double > DuneDynamicVector::amax() const
{
  std::vector< double > result(2, 0.0);
  for (size_t ii = 0; ii < dim(); ++ii) {
    const double value = std::abs(BaseType::operator[](ii));
    if (value > result[1]) {
      result[0] = ii;
      result[1] = value;
    }
  }
  return result;
} // ... amax(...)

DuneDynamicVector* DuneDynamicVector::add(const DuneDynamicVector* other) const
  throw (Exception::types_are_not_compatible,
         Exception::sizes_do_not_match)
{
  if (dim() != other->dim())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "size of other (" << other->dim() << ") does not match size of this (" << dim() << ")!");
  DuneDynamicVector* result = create(other->dim());
  for (size_t ii = 0; ii < dim(); ++ii)
    result->operator[](ii) = BaseType::operator[](ii) + other->operator[](ii);
  return result;
} // ... add(...)

void DuneDynamicVector::iadd(const DuneDynamicVector* other) throw (Exception::types_are_not_compatible,
                                                                    Exception::sizes_do_not_match)
{
  if (dim() != other->dim())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "size of other (" << other->dim() << ") does not match size of this (" << dim() << ")!");
  for (size_t ii = 0; ii < dim(); ++ii)
    BaseType::operator[](ii) += other->operator[](ii);
} // ... iadd(...)

DuneDynamicVector* DuneDynamicVector::sub(const DuneDynamicVector* other) const
  throw (Exception::types_are_not_compatible, Exception::sizes_do_not_match)
{
  if (dim() != other->dim())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "size of other (" << other->dim() << ") does not match size of this (" << dim() << ")!");
  DuneDynamicVector* result = create(other->dim());
  for (size_t ii = 0; ii < dim(); ++ii)
    result->operator[](ii) = BaseType::operator[](ii) - other->operator[](ii);
  return result;
} // ... sub(...)

void DuneDynamicVector::isub(const DuneDynamicVector* other) throw (Exception::types_are_not_compatible,
                                                                    Exception::sizes_do_not_match)
{
  if (dim() != other->dim())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "size of other (" << other->dim() << ") does not match size of this (" << dim() << ")!");
  for (size_t ii = 0; ii < dim(); ++ii)
    BaseType::operator[](ii) -= other->operator[](ii);
} // ... isub(...)

double DuneDynamicVector::apply(const DuneDynamicVector* source,
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
