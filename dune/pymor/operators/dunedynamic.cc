// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifdef HAVE_CMAKE_CONFIG
  #include "cmake_config.h"
#elif defined (HAVE_CONFIG_H)
  #include "config.h"
#endif // HAVE_CMAKE_CONFIG

#include "dunedynamic.hh"

namespace Dune {
namespace Pymor {
namespace Operators {


DuneDynamicInverse::DuneDynamicInverse(const DuneDynamic* op)
  : OperatorInterface()
  , op_(op)
{}

bool DuneDynamicInverse::linear() const
{
  return true;
}

unsigned int DuneDynamicInverse::dim_source() const
{
  return op_->dim_range();
}

unsigned int DuneDynamicInverse::dim_range() const
{
  return op_->dim_source();
}

std::string DuneDynamicInverse::type_source() const
{
  return SourceType::static_type();
}

std::string DuneDynamicInverse::type_range() const
{
  return RangeType::static_type();
}

void DuneDynamicInverse::apply(const LA::VectorInterface* source,
                               LA::VectorInterface* range,
                               const Parameter /*mu*/) const throw (Exception::types_are_not_compatible,
                                                                    Exception::you_have_to_implement_this,
                                                                    Exception::sizes_do_not_match,
                                                                    Exception::wrong_parameter_type,
                                                                    Exception::requirements_not_met,
                                                                    Exception::linear_solver_failed)
{
  std::stringstream msg;
  size_t throw_up = 0;
  if (source->type() != type_source()) {
    msg << "source (" << source->type() << ") is not a compatible type_source (" << type_source() << ")";
    ++throw_up;
  }
  if (range->type() != type_range()) {
    if (throw_up)
      msg << " and ";
    msg << "range (" << range->type() << ") is not a compatible type_range (" << type_range() << ")";
  }
  DUNE_PYMOR_THROW(Exception::types_are_not_compatible, msg.str());
}

void DuneDynamicInverse::apply(const SourceType* source, RangeType* range, const Parameter mu) const
  throw (Exception::types_are_not_compatible,
         Exception::you_have_to_implement_this,
         Exception::sizes_do_not_match,
         Exception::wrong_parameter_type,
         Exception::requirements_not_met,
         Exception::linear_solver_failed)
{
  if (source->dim() != dim_source())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "dim of source (" << source->dim() << ") does not match dim_source of this (" << dim_source()
                     << ")!");
  if (range->dim() != dim_range())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "dim of range (" << range->dim() << ") does not match dim_range of this (" << dim_range()
                     << ")!");
  if (mu.type() != Parameter().type())
    DUNE_PYMOR_THROW(Exception::wrong_parameter_type,
                     "since parametric() == false mu has to be empty (is " << mu.report() << ")!");
  op_->solve(*range, *source);
}

std::vector< std::string > DuneDynamicInverse::invert_options() throw (Exception::not_invertible)
{
  return { "exact" };
}

const OperatorInterface* DuneDynamicInverse::invert(const std::string type, const Parameter mu) const
  throw(Exception::not_invertible, Exception::key_is_not_valid)
{
  if (!mu.type().empty())
    DUNE_PYMOR_THROW(Exception::this_is_not_parametric,
                     "do not call invert with mu = " << mu << " if parametric() == false!");
  if (type != DuneDynamicInverse::invert_options()[0])
    DUNE_PYMOR_THROW(Exception::key_is_not_valid,
                     "type has to be '" << invert_options()[0] << "' (is " << type << ")!");
  return op_;
}

void DuneDynamicInverse::apply_inverse(const LA::VectorInterface* range,
                                       LA::VectorInterface* source,
                                       const std::string /*type*/,
                                       const Parameter /*mu*/) const
  throw (Exception::types_are_not_compatible,
         Exception::you_have_to_implement_this,
         Exception::sizes_do_not_match,
         Exception::wrong_parameter_type,
         Exception::requirements_not_met,
         Exception::linear_solver_failed)
{
  std::stringstream msg;
  size_t throw_up = 0;
  if (source->type() != type_source()) {
    msg << "source (" << source->type() << ") is not a compatible type_source (" << type_source() << ")";
    ++throw_up;
  }
  if (range->type() != type_range()) {
    if (throw_up)
      msg << " and ";
    msg << "range (" << range->type() << ") is not a compatible type_range (" << type_range() << ")";
  }
  DUNE_PYMOR_THROW(Exception::types_are_not_compatible, msg.str());
}

void DuneDynamicInverse::apply_inverse(const RangeType* range,
                                       SourceType* source,
                                       const std::string type,
                                       const Parameter mu) const
  throw (Exception::types_are_not_compatible,
         Exception::you_have_to_implement_this,
         Exception::sizes_do_not_match,
         Exception::wrong_parameter_type,
         Exception::requirements_not_met,
         Exception::linear_solver_failed)
{
  if (type != invert_options()[0])
    DUNE_PYMOR_THROW(Exception::key_is_not_valid,
                     "type has to be '" << invert_options()[0] << "' (is " << type << ")!");
  op_->apply(range, source, mu);
}

DuneDynamicInverse* DuneDynamicInverse::freeze_parameter(const Parameter /*mu*/) const
  throw (Exception::this_is_not_parametric, Exception::you_have_to_implement_this)
{
  DUNE_PYMOR_THROW(Exception::this_is_not_parametric, "do not call freeze_parameter if parametric() == false!");
  return nullptr;
}


DuneDynamic::DuneDynamic()
  : BaseType()
  , OperatorInterface()
{}

DuneDynamic::DuneDynamic(const BaseType& other)
  : BaseType(other)
  , OperatorInterface()
{}

DuneDynamic::DuneDynamic(const int rr, const int cc) throw (Exception::index_out_of_range)
  : BaseType(DuneDynamic::assert_is_positive(rr), DuneDynamic::assert_is_positive(cc))
  , OperatorInterface()
{}

bool DuneDynamic::linear() const
{
  return true;
}

unsigned int DuneDynamic::dim_source() const
{
  return BaseType::cols();
}

unsigned int DuneDynamic::dim_range() const
{
  return BaseType::rows();
}

std::string DuneDynamic::type_source() const
{
  return SourceType::static_type();
}

std::string DuneDynamic::type_range() const
{
  return RangeType::static_type();
}

void DuneDynamic::apply(const LA::VectorInterface* source,
                        LA::VectorInterface* range,
                        const Parameter /*mu*/) const throw (Exception::types_are_not_compatible,
                                                             Exception::you_have_to_implement_this,
                                                             Exception::sizes_do_not_match,
                                                             Exception::wrong_parameter_type,
                                                             Exception::requirements_not_met,
                                                             Exception::linear_solver_failed)
{
  std::stringstream msg;
  size_t throw_up = 0;
  if (source->type() != type_source()) {
    msg << "source (" << source->type() << ") is not a compatible type_source (" << type_source() << ")";
    ++throw_up;
  }
  if (range->type() != type_range()) {
    if (throw_up)
      msg << " and ";
    msg << "range (" << range->type() << ") is not a compatible type_range (" << type_range() << ")";
  }
  DUNE_PYMOR_THROW(Exception::types_are_not_compatible, msg.str());
}

void DuneDynamic::apply(const SourceType* source, RangeType* range, const Parameter mu) const
  throw (Exception::types_are_not_compatible,
         Exception::you_have_to_implement_this,
         Exception::sizes_do_not_match,
         Exception::wrong_parameter_type,
         Exception::requirements_not_met,
         Exception::linear_solver_failed)
{
  if (source->dim() != dim_source())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "dim of source (" << source->dim() << ") does not match dim_source of this (" << dim_source()
                     << ")!");
  if (range->dim() != dim_range())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "dim of range (" << range->dim() << ") does not match dim_range of this (" << dim_range()
                     << ")!");
  if (mu.type() != Parameter().type())
    DUNE_PYMOR_THROW(Exception::wrong_parameter_type,
                     "since parametric() == false mu has to be empty (is " << mu.report() << ")!");
  BaseType::mv(*source, *range);
}

double DuneDynamic::apply2(const RangeType* range,
                           const SourceType* source,
                           const Parameter mu) const
  throw (Exception::types_are_not_compatible,
         Exception::you_have_to_implement_this,
         Exception::sizes_do_not_match,
         Exception::wrong_parameter_type,
         Exception::requirements_not_met,
         Exception::linear_solver_failed)
{
  if (source->dim() != dim_source())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "size of U (" << source->dim() << ") does not match dim_source() of this (" << dim_source() << ")!");
  if (range->dim() != dim_range())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "size of V (" << range->dim() << ") does not match dim_range() of this (" << dim_range() << ")!");
  if (!mu.type().empty())
    DUNE_PYMOR_THROW(Exception::wrong_parameter_type,
                     "since parametric() == false mu has to be empty (is " << mu.report() << ")!");
  RangeType* tmp = new RangeType(dim_range());
  BaseType::mv(*source, *tmp);
  return range->dot(tmp);
}

std::vector< std::string > DuneDynamic::invert_options() throw (Exception::not_invertible)
{
  return { "superlu" };
}

const DuneDynamicInverse* DuneDynamic::invert(const std::string type,
                                              const Parameter mu) const
  throw(Exception::not_invertible, Exception::key_is_not_valid)
{
  if (!mu.type().empty())
    DUNE_PYMOR_THROW(Exception::this_is_not_parametric,
                     "do not call invert with mu = " << mu << " if parametric() == false!");
  if (type != invert_options()[0])
    DUNE_PYMOR_THROW(Exception::key_is_not_valid,
                     "type has to be '" << invert_options()[0] << "' (is " << type << ")!");
  return new DuneDynamicInverse(this);
}

void DuneDynamic::apply_inverse(const LA::VectorInterface* range,
                                LA::VectorInterface* source,
                                const std::string /*type*/,
                                const Parameter /*mu*/) const throw (Exception::types_are_not_compatible,
                                                                     Exception::you_have_to_implement_this,
                                                                     Exception::sizes_do_not_match,
                                                                     Exception::wrong_parameter_type,
                                                                     Exception::requirements_not_met,
                                                                     Exception::linear_solver_failed)
{
  std::stringstream msg;
  size_t throw_up = 0;
  if (source->type() != type_source()) {
    msg << "source (" << source->type() << ") is not a compatible type_source (" << type_source() << ")";
    ++throw_up;
  }
  if (range->type() != type_range()) {
    if (throw_up)
      msg << " and ";
    msg << "range (" << range->type() << ") is not a compatible type_range (" << type_range() << ")";
  }
  DUNE_PYMOR_THROW(Exception::types_are_not_compatible, msg.str());
}

void DuneDynamic::apply_inverse(const RangeType* range,
                                SourceType* source,
                                const std::string type,
                                const Parameter mu) const throw (Exception::types_are_not_compatible,
                                                                 Exception::you_have_to_implement_this,
                                                                 Exception::sizes_do_not_match,
                                                                 Exception::wrong_parameter_type,
                                                                 Exception::requirements_not_met,
                                                                 Exception::linear_solver_failed)
{
  const DuneDynamicInverse* inverseOp = invert(type, mu);
  inverseOp->apply(range, source);
  delete inverseOp;
}


DuneDynamic* DuneDynamic::freeze_parameter(const Parameter /*mu*/) const
  throw (Exception::this_is_not_parametric)
{
  DUNE_PYMOR_THROW(Exception::this_is_not_parametric, "do not call freeze_parameter if parametric() == false!");
  return nullptr;
}

int DuneDynamic::assert_is_positive(const int ii) throw (Exception::index_out_of_range)
{
  if (ii <= 0) DUNE_PYMOR_THROW(Exception::index_out_of_range, "ii has to be positive (is " << ii << ")!");
  return ii;
}


} // namespace Operators
} // namespace Pymor
} // namespace Dune
