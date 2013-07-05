// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_OPERATORS_INTERFACES_HH
#define DUNE_PYMOR_OPERATORS_INTERFACES_HH

#include <dune/pymor/common/exceptions.hh>
#include <dune/pymor/parameters/base.hh>
#include <dune/pymor/la/container/interfaces.hh>

namespace Dune {
namespace Pymor {


class OperatorInterface
  : public Parametric
{
public:
  template< class... Args >
  OperatorInterface(Args&& ...args)
    : Parametric(std::forward< Args >(args)...)
  {}

  virtual ~OperatorInterface() {}

  virtual bool linear() const = 0;

  virtual unsigned int dim_source() const = 0;

  virtual unsigned int dim_range() const = 0;

  virtual std::string type_source() const = 0;

  virtual std::string type_range() const = 0;

  virtual LA::VectorInterface* apply(const LA::VectorInterface* U, const Parameter /*mu*/ = Parameter()) const
    throw (Exception::types_are_not_compatible,
           Exception::you_have_to_implement_this,
           Exception::sizes_do_not_match,
           Exception::wrong_parameter_type)
  {
    if (U->type() == type_source())
      DUNE_PYMOR_THROW(Exception::you_have_to_implement_this,
                       "this operator has to be implemented for U of type_source() (" << type_source() << ")!");
    DUNE_PYMOR_THROW(Exception::types_are_not_compatible,
                     "U (" << U->type() << ") is not a compatible source (" << type_source() << ")!");
  }

  virtual double apply2(const LA::VectorInterface* V,
                        const LA::VectorInterface* U,
                        const Parameter /*mu*/ = Parameter()) const
    throw (Exception::types_are_not_compatible,
           Exception::you_have_to_implement_this,
           Exception::sizes_do_not_match,
           Exception::wrong_parameter_type)
  {
    if (U->type() == type_source() && V->type() == type_range())
      DUNE_PYMOR_THROW(Exception::you_have_to_implement_this,
                       "this operator has to be implemented for U of type_source() (" << type_source() << ")"
                       << " and V of type_range() (" << type_range() << ")!");
    std::stringstream msg;
    size_t throw_up = 0;
    if (U->type() != type_source()) {
      msg << "U (" << U->type() << ") is not a compatible type_source (" << type_source() << ")";
      ++throw_up;
    }
    if (V->type() != type_range()) {
      if (throw_up)
        msg << " and ";
      msg << "V (" << V->type() << ") is not a compatible type_range (" << type_range() << ")";
    }
    if (throw_up) DUNE_PYMOR_THROW(Exception::types_are_not_compatible, msg.str() << "!");
    return 0.0;
  }

//  OperatorInterface* freeze_parameter(const Parameter mu = Parameter()) const
//    throw (Exception::wrong_parameter_type) = 0;
}; // class OperatorInterface


} // namespace Pymor
} // namespace Dune

#endif // DUNE_PYMOR_OPERATORS_INTERFACES_HH
