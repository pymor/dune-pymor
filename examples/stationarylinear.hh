// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_EXAMPLES_STATIONARYLINEAR_HH
#define DUNE_PYMOR_EXAMPLES_STATIONARYLINEAR_HH

#include <dune/pymor/parameters/base.hh>
#include <dune/pymor/discretizations/interfaces.hh>
#include <dune/pymor/la/container/dunedynamic.hh>
#include <dune/pymor/operators/dunedynamic.hh>
#include <dune/pymor/operators/affine.hh>
#include <dune/pymor/functions.hh>
#include <dune/pymor/functionals/default.hh>
#include <dune/pymor/functionals/affine.hh>

#include <dune/pymor/python-bindings.hh>

namespace Example {


class AnalyticalProblem
  : public Dune::Pymor::Parametric
{
public:
  typedef Dune::Stuff::FunctionExpression< double, 1, double, 1 > ExpressionFunctionType;
  typedef Dune::Pymor::Function::AffinelyDecomposableDefault< double, 1, double, 1 > FunctionType;

  AnalyticalProblem(const int dd = 4);

  ~AnalyticalProblem();

  unsigned int dim() const;

  const FunctionType* diffusion() const;

  const FunctionType* force() const;

  const FunctionType* dirichlet() const;

  const FunctionType* neumann() const;

private:
  unsigned int dim_;
  FunctionType* diffusion_;
  FunctionType* force_;
  FunctionType* dirichlet_;
  FunctionType* neumann_;
}; // class AnalyticalProblem


class SimpleDiscretization;

class SimpleDiscretizationTraits
{
public:
  typedef SimpleDiscretization derived_type;
  typedef Dune::Pymor::LA::DuneDynamicVector< double > VectorType;
  typedef Dune::Pymor::Operators::LinearAffinelyDecomposedContainerBased< typename Dune::Pymor::Operators::DuneDynamic< double > > OperatorType;
  typedef Dune::Pymor::Functionals::LinearAffinelyDecomposedVectorBased< VectorType > FunctionalType;
  typedef OperatorType ProductType;
};


class SimpleDiscretization
  : public Dune::Pymor::StationaryDiscretizationInterface< SimpleDiscretizationTraits >
{
  typedef SimpleDiscretizationTraits Traits;
public:
  typedef typename Traits::VectorType     VectorType;
  typedef typename Traits::OperatorType   OperatorType;
  typedef typename Traits::FunctionalType FunctionalType;
  typedef typename Traits::ProductType    ProductType;

  SimpleDiscretization(const AnalyticalProblem* prob);

  ~SimpleDiscretization();

  OperatorType get_operator() const;

  FunctionalType get_rhs() const;

  std::vector< std::string > available_products() const;

  ProductType get_product(const std::string id) const;

  VectorType create_vector() const;

//  std::vector< std::string > solver_options() const;

//  std::string solver_options(const std::string context) const;

  void solve(VectorType& vector, const Dune::Pymor::Parameter mu = Dune::Pymor::Parameter()) const;

  void visualize(const VectorType& vector, const std::string filename, const std::string name) const;

private:
  const AnalyticalProblem* problem_;
  const int dim_;
  OperatorType* op_;
  FunctionalType* func_;
}; // class SimpleDiscretization


} // namespace Example

#endif // DUNE_PYMOR_EXAMPLES_STATIONARYLINEAR_HH
