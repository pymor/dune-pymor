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
//#include "dunedynamicinverse.hh"

namespace Dune {
namespace Pymor {
namespace Operators {


class DuneDynamic;


class DuneDynamicInverse
  : public OperatorInterface
{
public:
  typedef DuneDynamicInverse                  ThisType;
  typedef Dune::Pymor::LA::DuneDynamicVector  SourceType;
  typedef Dune::Pymor::LA::DuneDynamicVector  RangeType;

  DuneDynamicInverse(const DuneDynamic* op);

  virtual bool linear() const;

  virtual unsigned int dim_source() const;

  virtual unsigned int dim_range() const;

  virtual std::string type_source() const;

  virtual std::string type_range() const;

  virtual void apply(const LA::VectorInterface* source,
                     LA::VectorInterface* range,
                     const Parameter /*mu*/ = Parameter()) const throw (Exception::types_are_not_compatible,
                                                                        Exception::you_have_to_implement_this,
                                                                        Exception::sizes_do_not_match,
                                                                        Exception::wrong_parameter_type,
                                                                        Exception::requirements_not_met,
                                                                        Exception::linear_solver_failed);

  virtual void apply(const SourceType* source, RangeType* range, const Parameter mu = Parameter()) const
    throw (Exception::types_are_not_compatible,
           Exception::you_have_to_implement_this,
           Exception::sizes_do_not_match,
           Exception::wrong_parameter_type,
           Exception::requirements_not_met,
           Exception::linear_solver_failed);

  static std::vector< std::string > invert_options() throw (Exception::not_invertible);

  virtual const OperatorInterface* invert(const std::string type = invert_options()[0],
                                          const Parameter mu = Parameter()) const
    throw(Exception::not_invertible, Exception::key_is_not_valid);

  virtual void apply_inverse(const LA::VectorInterface* range,
                             LA::VectorInterface* source,
                             const std::string /*type*/ = invert_options()[0],
                             const Parameter /*mu*/ = Parameter()) const
    throw (Exception::types_are_not_compatible,
           Exception::you_have_to_implement_this,
           Exception::sizes_do_not_match,
           Exception::wrong_parameter_type,
           Exception::requirements_not_met,
           Exception::linear_solver_failed);

  virtual void apply_inverse(const RangeType* range,
                             SourceType* source,
                             const std::string type = invert_options()[0],
                             const Parameter mu = Parameter()) const
    throw (Exception::types_are_not_compatible,
           Exception::you_have_to_implement_this,
           Exception::sizes_do_not_match,
           Exception::wrong_parameter_type,
           Exception::requirements_not_met,
           Exception::linear_solver_failed);

  virtual ThisType* freeze_parameter(const Parameter /*mu*/ = Parameter()) const
    throw (Exception::this_is_not_parametric, Exception::you_have_to_implement_this);

private:
  const DuneDynamic* op_;
}; // class DuneDynamicInverse


class DuneDynamic
  : public Dune::DynamicMatrix< double >
  , public OperatorInterface
{
public:
  typedef Dune::DynamicMatrix< double >       BaseType;
  typedef DuneDynamic                         ThisType;
  typedef Dune::Pymor::LA::DuneDynamicVector  SourceType;
  typedef Dune::Pymor::LA::DuneDynamicVector  RangeType;

  DuneDynamic();

  DuneDynamic(const BaseType& other);

  DuneDynamic(const int rr, const int cc) throw (Exception::index_out_of_range);

  virtual bool linear() const;

  virtual unsigned int dim_source() const;

  virtual unsigned int dim_range() const;

  virtual std::string type_source() const;

  virtual std::string type_range() const;

  virtual void apply(const LA::VectorInterface* source,
                     LA::VectorInterface* range,
                     const Parameter /*mu*/ = Parameter()) const throw (Exception::types_are_not_compatible,
                                                                        Exception::you_have_to_implement_this,
                                                                        Exception::sizes_do_not_match,
                                                                        Exception::wrong_parameter_type,
                                                                        Exception::requirements_not_met,
                                                                        Exception::linear_solver_failed);

  virtual void apply(const SourceType* source, RangeType* range, const Parameter mu = Parameter()) const
    throw (Exception::types_are_not_compatible,
           Exception::you_have_to_implement_this,
           Exception::sizes_do_not_match,
           Exception::wrong_parameter_type,
           Exception::requirements_not_met,
           Exception::linear_solver_failed);

  virtual double apply2(const RangeType* range,
                        const SourceType* source,
                        const Parameter mu = Parameter()) const
    throw (Exception::types_are_not_compatible,
           Exception::you_have_to_implement_this,
           Exception::sizes_do_not_match,
           Exception::wrong_parameter_type,
           Exception::requirements_not_met,
           Exception::linear_solver_failed);

  static std::vector< std::string > invert_options() throw(Exception::not_invertible);

  virtual const DuneDynamicInverse* invert(const std::string type = invert_options()[0],
                                           const Parameter mu = Parameter()) const
    throw(Exception::not_invertible, Exception::key_is_not_valid);

  virtual void apply_inverse(const LA::VectorInterface* range,
                             LA::VectorInterface* source,
                             const std::string /*type*/ = invert_options()[0],
                             const Parameter /*mu*/ = Parameter()) const
    throw (Exception::types_are_not_compatible,
           Exception::you_have_to_implement_this,
           Exception::sizes_do_not_match,
           Exception::wrong_parameter_type,
           Exception::requirements_not_met,
           Exception::linear_solver_failed);

  virtual void apply_inverse(const RangeType* range,
                             SourceType* source,
                             const std::string type = invert_options()[0],
                             const Parameter mu = Parameter()) const
    throw (Exception::types_are_not_compatible,
           Exception::you_have_to_implement_this,
           Exception::sizes_do_not_match,
           Exception::wrong_parameter_type,
           Exception::requirements_not_met,
           Exception::linear_solver_failed);

  virtual ThisType* freeze_parameter(const Parameter /*mu*/ = Parameter()) const
    throw (Exception::this_is_not_parametric);

private:
  static int assert_is_positive(const int ii) throw (Exception::index_out_of_range);
}; // class DuneDynamic


} // namespace Operators
} // namespace Pymor
} // namespace Dune

#endif // DUNE_PYMOR_OPERATORS_DUNEDYNAMIC_HH
