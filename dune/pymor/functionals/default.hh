// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_FUNCTIONALS_DEFAULT_HH
#define DUNE_PYMOR_FUNCTIONALS_DEFAULT_HH

#include <dune/common/typetraits.hh>

#include <dune/pymor/parameters/functional.hh>
#include <dune/pymor/la/container/interfaces.hh>
#include "interfaces.hh"

namespace Dune {
namespace Pymor {
namespace Functionals {


template< class LinearFunctionalType >
class LinearAffinelyDecomposedDefault
  : public AffinelyDecomposedFunctionalInterface
{
public:
  typedef typename LinearFunctionalType::SourceType SourceType;

  LinearAffinelyDecomposedDefault();

  LinearAffinelyDecomposedDefault(LinearFunctionalType* aff) throw (Exception::requirements_not_met);

  virtual ~LinearAffinelyDecomposedDefault();

  /**
   * \attention This class takes ownership of aff!
   */
  void register_component(LinearFunctionalType* aff) throw (Exception::this_does_not_make_any_sense,
                                                            Exception::sizes_do_not_match,
                                                            Exception::types_are_not_compatible);

  /**
   * \attention This class takes ownership of comp and coeff!
   */
  void register_component(LinearFunctionalType* comp, const ParameterFunctional* coeff)
    throw (Exception::this_does_not_make_any_sense,
           Exception::sizes_do_not_match,
           Exception::types_are_not_compatible,
           Exception::wrong_parameter_type);

  virtual unsigned int num_components() const;

  virtual LinearFunctionalType* component(const int ii) throw (Exception::requirements_not_met,
                                                               Exception::index_out_of_range);

  virtual const LinearFunctionalType* component(const int ii) const throw (Exception::requirements_not_met,
                                                                           Exception::index_out_of_range);

  virtual const ParameterFunctional* coefficient(const int ii) const throw (Exception::requirements_not_met,
                                                                            Exception::index_out_of_range);

  virtual bool hasAffinePart() const;

  virtual LinearFunctionalType* affinePart() throw (Exception::requirements_not_met);

  virtual const LinearFunctionalType* affinePart() const throw (Exception::requirements_not_met);

  virtual bool linear() const;

  virtual unsigned int dim_source() const;

  virtual std::string type_source() const;

  virtual double apply(const LA::VectorInterface* source,
                       const Parameter /*mu*/ = Parameter()) const throw (Exception::types_are_not_compatible,
                                                                          Exception::you_have_to_implement_this,
                                                                          Exception::sizes_do_not_match,
                                                                          Exception::wrong_parameter_type,
                                                                          Exception::requirements_not_met,
                                                                          Exception::linear_solver_failed,
                                                                          Exception::this_does_not_make_any_sense);

  virtual double apply(const SourceType* source,
                       const Parameter mu = Parameter()) const
    throw (Exception::types_are_not_compatible,
           Exception::you_have_to_implement_this,
           Exception::sizes_do_not_match,
           Exception::wrong_parameter_type,
           Exception::requirements_not_met,
           Exception::linear_solver_failed,
           Exception::this_does_not_make_any_sense);

  LinearFunctionalType* freeze_parameter(const Parameter mu = Parameter()) const
    throw (Exception::this_is_not_parametric,
           Exception::you_have_to_implement_this,
           Exception::this_does_not_make_any_sense);

private:
  unsigned int size_;
  bool hasAffinePart_;
  unsigned int dim_source_;
  std::string type_source_;
  std::vector< LinearFunctionalType* > components_;
  std::vector< const ParameterFunctional* > coefficients_;
  LinearFunctionalType* affinePart_;
}; // class LinearAffinelyDecomposedDefault


} // namespace Functionals
} // namespace Pymor
} // namespace Dune

#endif // DUNE_PYMOR_FUNCTIONALS_DEFAULT_HH
