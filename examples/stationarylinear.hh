// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_EXAMPLES_STATIONARYLINEAR_HH
#define DUNE_PYMOR_EXAMPLES_STATIONARYLINEAR_HH

#include <dune/pymor/parameters/base.hh>
#include <dune/pymor/discretizations/interfaces.hh>
#include <dune/pymor/la/container/dunedynamicvector.hh>
#include <dune/pymor/operators/dunedynamicmatrix.hh>
#include <dune/pymor/operators/eigen.hh>
#include <dune/pymor/operators/default.hh>
#include <dune/pymor/functions/default.hh>
#include <dune/pymor/functionals/default.hh>

namespace Example {


class AnalyticalProblem
  : public Dune::Pymor::Parametric
{
public:
  typedef Dune::Pymor::Functions::NonparametricExpression< double, 1, double, 1 > ExpressionFunctionType;
  typedef Dune::Pymor::Functions::AffineParametricDefault< double, 1, double, 1 > FunctionType;

  static const size_t dim;

  AnalyticalProblem();

  ~AnalyticalProblem();

  const FunctionType* diffusion() const;

  const FunctionType* force() const;

  const FunctionType* dirichlet() const;

  const FunctionType* neumann() const;

private:
  FunctionType* diffusion_;
  FunctionType* force_;
  FunctionType* dirichlet_;
  FunctionType* neumann_;
}; // class AnalyticalProblem


class SimpleDiscretization
  : public Dune::Pymor::StationaryDiscretizationInterface
{
  typedef Dune::Pymor::StationaryDiscretizationInterface BaseType;
public:
  typedef Dune::Pymor::LA::DuneDynamicVector                                                VectorType;
  typedef Dune::Pymor::Operators::DuneDynamicMatrix                                         OperatorComponentType;
  typedef Dune::Pymor::Operators::LinearAffinelyDecomposedDefault< OperatorComponentType >  OperatorType;
  typedef Dune::Pymor::Functionals::LinearAffinelyDecomposedDefault< VectorType >           FunctionalType;

  SimpleDiscretization(const AnalyticalProblem* prob)
    throw (Dune::Pymor::Exception::this_does_not_make_any_sense,
           Dune::Pymor::Exception::sizes_do_not_match,
           Dune::Pymor::Exception::types_are_not_compatible,
           Dune::Pymor::Exception::wrong_parameter_type);

  ~SimpleDiscretization();

  virtual std::vector< std::string > available_operators() const;

  virtual const OperatorType* get_operator(const std::string id) const
    throw (Dune::Pymor::Exception::key_is_not_valid);

  virtual std::vector< std::string > available_functionals() const;

  virtual const FunctionalType* get_functional(const std::string id) const
    throw (Dune::Pymor::Exception::key_is_not_valid);

  virtual VectorType* create_vector() const;

  virtual std::vector< std::string > solver_options() const;

  virtual std::string solver_options(const std::string context) const
    throw (Dune::Pymor::Exception::key_is_not_valid);

  virtual void solve(Dune::Pymor::LA::VectorInterface* vector,
                     const Dune::Pymor::Parameter /*mu*/ = Dune::Pymor::Parameter()) const
    throw (Dune::Pymor::Exception::wrong_parameter_type,
           Dune::Pymor::Exception::types_are_not_compatible,
           Dune::Pymor::Exception::you_have_to_implement_this,
           Dune::Pymor::Exception::sizes_do_not_match,
           Dune::Pymor::Exception::wrong_parameter_type,
           Dune::Pymor::Exception::requirements_not_met,
           Dune::Pymor::Exception::linear_solver_failed,
           Dune::Pymor::Exception::this_does_not_make_any_sense);

  virtual void solve(VectorType* vector, const Dune::Pymor::Parameter mu = Dune::Pymor::Parameter()) const
    throw (Dune::Pymor::Exception::wrong_parameter_type,
           Dune::Pymor::Exception::types_are_not_compatible,
           Dune::Pymor::Exception::you_have_to_implement_this,
           Dune::Pymor::Exception::sizes_do_not_match,
           Dune::Pymor::Exception::wrong_parameter_type,
           Dune::Pymor::Exception::requirements_not_met,
           Dune::Pymor::Exception::linear_solver_failed,
           Dune::Pymor::Exception::this_does_not_make_any_sense);

  virtual void visualize(const Dune::Pymor::LA::VectorInterface* vector,
                         const std::string /*filename*/,
                         const std::string /*name*/) const;

  virtual void visualize(const VectorType* vector,
                         const std::string filename,
                         const std::string name = "vector") const throw (Dune::Pymor::Exception::sizes_do_not_match);

private:
  const AnalyticalProblem* problem_;
  const int dim_;
  OperatorType* op_;
  FunctionalType* func_;
}; // class SimpleDiscretization


} // namespace Example

#endif // DUNE_PYMOR_EXAMPLES_STATIONARYLINEAR_HH
