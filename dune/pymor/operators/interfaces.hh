// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_OPERATORS_INTERFACES_HH
#define DUNE_PYMOR_OPERATORS_INTERFACES_HH

#include <dune/pymor/common/exceptions.hh>
#include <dune/pymor/parameters/base.hh>
#include <dune/pymor/parameters/functional.hh>
#include <dune/pymor/la/container/interfaces.hh>
#include <dune/pymor/la/container.hh>

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

  virtual void apply(const LA::VectorInterface* /*source*/,
                     LA::VectorInterface* /*range*/,
                     const Parameter /*mu*/ = Parameter()) const throw (Exception::types_are_not_compatible,
                                                                        Exception::you_have_to_implement_this,
                                                                        Exception::sizes_do_not_match,
                                                                        Exception::wrong_parameter_type,
                                                                        Exception::requirements_not_met) = 0;

  virtual double apply2(const LA::VectorInterface* range,
                        const LA::VectorInterface* source,
                        const Parameter mu = Parameter()) const throw (Exception::types_are_not_compatible,
                                                                       Exception::sizes_do_not_match,
                                                                       Exception::wrong_parameter_type)
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
    if (throw_up) DUNE_PYMOR_THROW(Exception::types_are_not_compatible, msg.str() << "!");
    if (source->dim() != dim_source())
      DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                       "dim of source (" << source->dim() << ") does not match dim_source of this (" << dim_source()
                       << ")!");
    if (range->dim() != dim_range())
      DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                       "dim of range (" << range->dim() << ") does not match dim_range of this (" << dim_range()
                       << ")!");
    LA::VectorInterface* tmp = LA::createVector(type_range(), dim_range());
    apply(source, tmp, mu);
    return range->dot(tmp);
  }

  virtual OperatorInterface* freeze_parameter(const Parameter /*mu*/ = Parameter()) const
    throw (Exception::this_is_not_parametric, Exception::you_have_to_implement_this)
  {
    DUNE_PYMOR_THROW(Exception::you_have_to_implement_this, "you really do!");
  }
}; // class OperatorInterface


class AffinelyDecomposedOperatorInterface
  : public OperatorInterface
{
public:
  template< class... Args >
  AffinelyDecomposedOperatorInterface(Args&& ...args)
    : OperatorInterface(std::forward< Args >(args)...)
  {}

  virtual ~AffinelyDecomposedOperatorInterface() {}

  virtual unsigned int size() const = 0;

  /**
   * \attention The ownership of the component remains within the class!
   */
  virtual const OperatorInterface* component(const int ii) const throw (Exception::requirements_not_met,
                                                                        Exception::index_out_of_range) = 0;

  /**
   * \attention The ownership of the coefficient remains within the class!
   */
  virtual const ParameterFunctional* coefficient(const int ii) const throw (Exception::requirements_not_met,
                                                                            Exception::index_out_of_range) = 0;

  virtual bool hasAffinePart() const = 0;

  /**
   * \attention The ownership of the affinePart remains within the class!
   */
  virtual const OperatorInterface* affinePart() const throw(Exception::requirements_not_met) = 0;
}; // class AffinelyDecomposedOperatorInterface


} // namespace Pymor
} // namespace Dune

#endif // DUNE_PYMOR_OPERATORS_INTERFACES_HH
