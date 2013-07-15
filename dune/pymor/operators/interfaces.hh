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


/**
 * \note  A derived class has to implement the following static method:
\code
static std::vector< std::string > invert_options() throw(Exception::not_invertible)
\endcode
 */
class OperatorInterface
  : public Parametric
{
public:
  OperatorInterface();

  OperatorInterface(const ParameterType& tt);

  OperatorInterface(const std::string& kk, const int& vv) throw (Exception::key_is_not_valid,
                                                                 Exception::index_out_of_range);

  OperatorInterface(const std::vector< std::string >& kk,
                    const std::vector< int >& vv) throw (Exception::key_is_not_valid,
                                                         Exception::index_out_of_range,
                                                         Exception::sizes_do_not_match);

  OperatorInterface(const Parametric& other);

  virtual ~OperatorInterface();

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
                                                                        Exception::requirements_not_met,
                                                                        Exception::linear_solver_failed,
                                                                        Exception::this_does_not_make_any_sense) = 0;

  virtual double apply2(const LA::VectorInterface* range,
                        const LA::VectorInterface* source,
                        const Parameter mu = Parameter()) const throw (Exception::types_are_not_compatible,
                                                                       Exception::you_have_to_implement_this,
                                                                       Exception::sizes_do_not_match,
                                                                       Exception::wrong_parameter_type,
                                                                       Exception::requirements_not_met,
                                                                       Exception::linear_solver_failed,
                                                                       Exception::this_does_not_make_any_sense);

//  virtual Options* invert_options(const std::string& type) const = 0;

  virtual const OperatorInterface* invert(const std::string type = "",
                                          const Parameter mu = Parameter()) const
    throw (Exception::not_invertible, Exception::key_is_not_valid) = 0;

//  virtual InverseOperatorInterface* invert(const Parameter mu = Parameter(),
//                                           const Options* = invert_options(invert_options()[0])) const = 0;

  virtual void apply_inverse(const LA::VectorInterface* range,
                             LA::VectorInterface* source,
                             const std::string type = "",
                             const Parameter mu = Parameter()) const
    throw (Exception::types_are_not_compatible,
           Exception::you_have_to_implement_this,
           Exception::sizes_do_not_match,
           Exception::wrong_parameter_type,
           Exception::requirements_not_met,
           Exception::linear_solver_failed,
           Exception::this_does_not_make_any_sense) = 0;

//  virtual void apply_inverse(const LA::VectorInterface* range,
//                             const LA::VectorInterface* source,
//                             const Parameter mu = Parameter(),
//                             const Options* options = ) const = 0;

  virtual OperatorInterface* freeze_parameter(const Parameter /*mu*/ = Parameter()) const
    throw (Exception::this_is_not_parametric,
           Exception::you_have_to_implement_this,
           Exception::this_does_not_make_any_sense,
           Exception::this_does_not_make_any_sense);
}; // class OperatorInterface


class LinearOperatorInterface
  : public OperatorInterface
{
public:
  LinearOperatorInterface();

  LinearOperatorInterface(const ParameterType& tt);

  LinearOperatorInterface(const std::string& kk, const int& vv) throw (Exception::key_is_not_valid,
                                                                       Exception::index_out_of_range);

  LinearOperatorInterface(const std::vector< std::string >& kk,
                          const std::vector< int >& vv) throw (Exception::key_is_not_valid,
                                                               Exception::index_out_of_range,
                                                               Exception::sizes_do_not_match);

  LinearOperatorInterface(const Parametric& other);

  virtual ~LinearOperatorInterface();

  virtual bool linear() const;

  virtual LinearOperatorInterface* copy() const = 0;

  virtual void scal(const double alpha) = 0;

  virtual void axpy(const double alpha, const LinearOperatorInterface* x)
    throw (Exception::sizes_do_not_match, Exception::types_are_not_compatible) = 0;
}; // class LinearOperatorInterface


class AffinelyDecomposedOperatorInterface
  : public OperatorInterface
{
public:
  AffinelyDecomposedOperatorInterface();

  AffinelyDecomposedOperatorInterface(const ParameterType& tt);

  AffinelyDecomposedOperatorInterface(const std::string& kk, const int& vv) throw (Exception::key_is_not_valid,
                                                                                   Exception::index_out_of_range);

  AffinelyDecomposedOperatorInterface(const std::vector< std::string >& kk,
                                      const std::vector< int >& vv) throw (Exception::key_is_not_valid,
                                                                           Exception::index_out_of_range,
                                                                           Exception::sizes_do_not_match);

  AffinelyDecomposedOperatorInterface(const Parametric& other);

  virtual ~AffinelyDecomposedOperatorInterface();

  virtual unsigned int num_components() const = 0;

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
