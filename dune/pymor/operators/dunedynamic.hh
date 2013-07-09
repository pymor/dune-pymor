// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_OPERATORS_DUNEDYNAMIC_HH
#define DUNE_PYMOR_OPERATORS_DUNEDYNAMIC_HH

#include <dune/common/dynmatrix.hh>

#include <dune/pymor/common/exceptions.hh>
#include <dune/pymor/la/container/dunedynamic.hh>
#include "interfaces.hh"

namespace Dune {
namespace Pymor {
namespace Operators {


class DuneDynamic
  : public Dune::DynamicMatrix< double >
  , public OperatorInterface
{
public:
  typedef Dune::DynamicMatrix< double >       BaseType;
  typedef DuneDynamic                         ThisType;
  typedef Dune::Pymor::LA::DuneDynamicVector  SourceType;
  typedef Dune::Pymor::LA::DuneDynamicVector  RangeType;

  DuneDynamic()
    : BaseType()
    , OperatorInterface()
  {}

  DuneDynamic(const BaseType& other)
    : BaseType(other)
    , OperatorInterface()
  {}

  DuneDynamic(const int rr, const int cc)
    : BaseType(assert_is_positive(rr), assert_is_positive(cc))
    , OperatorInterface()
  {}

  virtual bool linear() const
  {
    return true;
  }

  virtual unsigned int dim_source() const
  {
    return BaseType::cols();
  }

  virtual unsigned int dim_range() const
  {
    return BaseType::rows();
  }

  virtual std::string type_source() const
  {
    return SourceType::static_type();
  }

  virtual std::string type_range() const
  {
    return RangeType::static_type();
  }

  virtual void apply(const LA::VectorInterface* source,
                     LA::VectorInterface* range,
                     const Parameter /*mu*/ = Parameter()) const throw (Exception::types_are_not_compatible,
                                                                        Exception::you_have_to_implement_this,
                                                                        Exception::sizes_do_not_match,
                                                                        Exception::wrong_parameter_type,
                                                                        Exception::requirements_not_met)
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

  virtual void apply(const SourceType* source, RangeType* range, const Parameter mu = Parameter()) const
    throw (Exception::types_are_not_compatible,
           Exception::you_have_to_implement_this,
           Exception::sizes_do_not_match,
           Exception::wrong_parameter_type)
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

  virtual double apply2(const RangeType* range,
                        const SourceType* source,
                        const Parameter mu = Parameter()) const
    throw (Exception::types_are_not_compatible,
           Exception::you_have_to_implement_this,
           Exception::sizes_do_not_match,
           Exception::wrong_parameter_type)
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

  virtual ThisType* freeze_parameter(const Parameter /*mu*/ = Parameter()) const
    throw (Exception::this_is_not_parametric)
  {
    DUNE_PYMOR_THROW(Exception::this_is_not_parametric, "do not call freeze_parameter if parametric() == false!");
    return nullptr;
  }

private:
  static int assert_is_positive(const int ii)
  {
    if (ii <= 0) DUNE_PYMOR_THROW(Exception::index_out_of_range, "ii has to be positive (is " << ii << ")!");
    return ii;
  }
}; // class DuneDynamic


} // namespace Operators
} // namespace Pymor
} // namespace Dune

#endif // DUNE_PYMOR_OPERATORS_DUNEDYNAMIC_HH
