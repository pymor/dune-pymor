// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_OPERATORS_DEFAULT_HH
#define DUNE_PYMOR_OPERATORS_DEFAULT_HH

#include <dune/common/typetraits.hh>

#include <dune/pymor/parameters/functional.hh>
#include "interfaces.hh"

namespace Dune {
namespace Pymor {
namespace Operators {


template< class LinearOperatorType >
class LinearAffinelyDecomposedDefault
  : public AffinelyDecomposedOperatorInterface
{
public:
  typedef typename LinearOperatorType::SourceType SourceType;
  typedef typename LinearOperatorType::RangeType  RangeType;

  LinearAffinelyDecomposedDefault();

  LinearAffinelyDecomposedDefault(LinearOperatorType* aff) throw (Exception::requirements_not_met);

  virtual ~LinearAffinelyDecomposedDefault();

  /**
   * \attention This class takes ownership of aff!
   */
  void register_component(LinearOperatorType* aff) throw (Exception::this_does_not_make_any_sense,
                                                          Exception::sizes_do_not_match,
                                                          Exception::types_are_not_compatible);

  /**
   * \attention This class takes ownership of comp and coeff!
   */
  void register_component(LinearOperatorType* comp, const ParameterFunctional* coeff)
    throw (Exception::this_does_not_make_any_sense,
           Exception::sizes_do_not_match,
           Exception::types_are_not_compatible,
           Exception::wrong_parameter_type);

  virtual unsigned int num_components() const;

  virtual LinearOperatorType* component(const int ii) throw (Exception::requirements_not_met,
                                                             Exception::index_out_of_range);

  virtual const LinearOperatorType* component(const int ii) const throw (Exception::requirements_not_met,
                                                                         Exception::index_out_of_range);

  virtual const ParameterFunctional* coefficient(const int ii) const throw (Exception::requirements_not_met,
                                                                            Exception::index_out_of_range);

  virtual bool hasAffinePart() const;

  virtual LinearOperatorType* affinePart() throw (Exception::requirements_not_met);

  virtual const LinearOperatorType* affinePart() const throw(Exception::requirements_not_met);

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
                                                                        Exception::linear_solver_failed/*,
                                                                        Exception::this_does_not_make_any_sense*/);

  virtual void apply(const SourceType* source,
                     RangeType* range,
                     const Parameter mu = Parameter()) const
    throw (Exception::types_are_not_compatible,
           Exception::you_have_to_implement_this,
           Exception::sizes_do_not_match,
           Exception::wrong_parameter_type,
           Exception::requirements_not_met,
           Exception::linear_solver_failed/*,
           Exception::this_does_not_make_any_sense*/);

  virtual double apply2(const SourceType* range,
                        const RangeType* source,
                        const Parameter mu = Parameter()) const throw (Exception::types_are_not_compatible,
                                                                       Exception::you_have_to_implement_this,
                                                                       Exception::sizes_do_not_match,
                                                                       Exception::wrong_parameter_type,
                                                                       Exception::requirements_not_met,
                                                                       Exception::linear_solver_failed,
                                                                       Exception::this_does_not_make_any_sense);

  static std::vector< std::string > invert_options();

  virtual const OperatorInterface* invert(const std::string /*type*/ = invert_options()[0],
                                          const Parameter /*mu*/ = Parameter()) const
    throw (Exception::not_invertible, Exception::key_is_not_valid);

  virtual void apply_inverse(const LA::VectorInterface* /*range*/,
                             LA::VectorInterface* /*source*/,
                             const std::string /*type*/ = invert_options()[0],
                             const Parameter /*mu*/ = Parameter()) const
    throw (Exception::types_are_not_compatible,
           Exception::you_have_to_implement_this,
           Exception::sizes_do_not_match,
           Exception::wrong_parameter_type,
           Exception::requirements_not_met,
           Exception::linear_solver_failed,
           Exception::this_does_not_make_any_sense);

  LinearOperatorType* freeze_parameter(const Parameter mu = Parameter()) const
    throw (Exception::this_is_not_parametric,
           Exception::you_have_to_implement_this,
           Exception::this_does_not_make_any_sense);

private:
  unsigned int size_;
  bool hasAffinePart_;
  unsigned int dim_source_;
  unsigned int dim_range_;
  std::string type_source_;
  std::string type_range_;
  std::vector< LinearOperatorType* > components_;
  std::vector< const ParameterFunctional* > coefficients_;
  LinearOperatorType* affinePart_;
}; // class LinearAffinelyDecomposedDefault


} // namespace Operators
} // namespace Pymor
} // namespace Dune

#endif // DUNE_PYMOR_OPERATORS_DEFAULT_HH
