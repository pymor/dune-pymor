// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_FUNCTIONALS_INTERFACES_HH
#define DUNE_PYMOR_FUNCTIONALS_INTERFACES_HH

#include <dune/pymor/common/exceptions.hh>
#include <dune/pymor/parameters/base.hh>
#include <dune/pymor/parameters/functional.hh>

namespace Dune {
namespace Pymor {

namespace LA {
class VectorInterface;
}

class FunctionalInterface
  : public Parametric
{
public:
  template< class... Args >
  FunctionalInterface(Args&& ...args)
    : Parametric(std::forward< Args >(args)...)
  {}

  virtual ~FunctionalInterface() {}

  virtual bool linear() const = 0;

  virtual unsigned int dim_source() const = 0;

  virtual std::string type_source() const = 0;

  virtual double apply(const LA::VectorInterface* /*source*/,
                       const Parameter /*mu*/ = Parameter()) const throw (Exception::types_are_not_compatible,
                                                                          Exception::you_have_to_implement_this,
                                                                          Exception::sizes_do_not_match,
                                                                          Exception::wrong_parameter_type,
                                                                          Exception::requirements_not_met,
                                                                          Exception::linear_solver_failed,
                                                                          Exception::this_does_not_make_any_sense) = 0;

  virtual FunctionalInterface* freeze_parameter(const Parameter /*mu*/ = Parameter()) const
    throw (Exception::this_is_not_parametric,
           Exception::you_have_to_implement_this,
           Exception::this_does_not_make_any_sense,
           Exception::this_does_not_make_any_sense)
  {
    DUNE_PYMOR_THROW(Exception::you_have_to_implement_this, "you really do!");
  }
}; // class FunctionalInterface


class AffinelyDecomposedFunctionalInterface
  : public FunctionalInterface
{
public:
  template< class... Args >
  AffinelyDecomposedFunctionalInterface(Args&& ...args)
    : FunctionalInterface(std::forward< Args >(args)...)
  {}

  virtual ~AffinelyDecomposedFunctionalInterface() {}

  virtual unsigned int num_components() const = 0;

  /**
   * \attention The ownership of the component remains within the class!
   */
  virtual const FunctionalInterface* component(const int ii) const throw (Exception::requirements_not_met,
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
  virtual const FunctionalInterface* affinePart() const throw(Exception::requirements_not_met) = 0;
}; // class AffinelyDecomposedFunctionalInterface


} // namespace Pymor
} // namespace Dune

#include <dune/pymor/la/container/interfaces.hh>

#endif // DUNE_PYMOR_FUNCTIONALS_INTERFACES_HH
