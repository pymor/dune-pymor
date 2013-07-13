// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#include <fstream>

#include <dune/stuff/common/string.hh>

#include "stationarylinear.hh"

namespace Example {


const size_t AnalyticalProblem::dim = 4;

AnalyticalProblem::AnalyticalProblem() throw (Dune::Pymor::Exception::wrong_input)
  : Dune::Pymor::Parametric()
{
  // build the data functions
  // * diffusion
  const Dune::Pymor::ParameterType muDiffusion = {"diffusion", dim};
  diffusion_ = new FunctionType(muDiffusion);
  diffusion_->register_component(new ExpressionFunctionType("x", "1.0"));
  for (size_t ii = 0; ii < dim; ++ii)
    diffusion_->register_component(new ExpressionFunctionType("x", "1.0"),
                                   new Dune::Pymor::ParameterFunctional(muDiffusion,
                                                                        "diffusion["
                                                                        + Dune::Stuff::Common::toString(ii) + "]"));
  inherit_parameter_type(muDiffusion, "diffusion");
  // * force
  const Dune::Pymor::ParameterType muForce = {"force", dim};
  force_ = new FunctionType(muForce);
  force_->register_component(new ExpressionFunctionType("x", "1.0"));
  for (size_t ii = 0; ii < dim; ++ii)
    force_->register_component(new ExpressionFunctionType("x", "1.0"),
                               new Dune::Pymor::ParameterFunctional(muForce,
                                                                    "force["
                                                                    + Dune::Stuff::Common::toString(ii) + "]"));
  inherit_parameter_type(muForce, "force");
  // * dirichlet
  const Dune::Pymor::ParameterType muDirichlet = {"dirichlet", dim};
  dirichlet_ = new FunctionType(muDirichlet);
  dirichlet_->register_component(new ExpressionFunctionType("x", "1.0"));
  for (size_t ii = 0; ii < dim; ++ii)
    dirichlet_->register_component(new ExpressionFunctionType("x", "1.0"),
                                   new Dune::Pymor::ParameterFunctional(muDirichlet,
                                                                        "dirichlet["
                                                                        + Dune::Stuff::Common::toString(ii) + "]"));
  inherit_parameter_type(muDirichlet, "dirichlet");
  // * neumann
  const Dune::Pymor::ParameterType muNeumann = {"neumann", dim};
  neumann_ = new FunctionType(muNeumann);
  neumann_->register_component(new ExpressionFunctionType("x", "1.0"));
  for (size_t ii = 0; ii < dim; ++ii)
    neumann_->register_component(new ExpressionFunctionType("x", "1.0"),
                                 new Dune::Pymor::ParameterFunctional(muNeumann,
                                                                      "neumann["
                                                                      + Dune::Stuff::Common::toString(ii) + "]"));
  inherit_parameter_type(muNeumann, "neumann");
}

AnalyticalProblem::~AnalyticalProblem()
{
  delete neumann_;
  delete dirichlet_;
  delete force_;
  delete diffusion_;
}

const AnalyticalProblem::FunctionType* AnalyticalProblem::diffusion() const
{
  return diffusion_;
}

const AnalyticalProblem::FunctionType* AnalyticalProblem::force() const
{
  return force_;
}

const AnalyticalProblem::FunctionType* AnalyticalProblem::dirichlet() const
{
  return dirichlet_;
}

const AnalyticalProblem::FunctionType* AnalyticalProblem::neumann() const
{
  return neumann_;
}


SimpleDiscretization::SimpleDiscretization(const AnalyticalProblem* prob)
  throw (Dune::Pymor::Exception::this_does_not_make_any_sense,
         Dune::Pymor::Exception::sizes_do_not_match,
         Dune::Pymor::Exception::types_are_not_compatible,
         Dune::Pymor::Exception::wrong_parameter_type)
  : BaseType()
  , problem_(prob)
  , dim_(prob->dim)
{
  // left hand side
  // * diffusion operator
  const auto& diffusion = *(problem_->diffusion());
  assert(int(diffusion.size()) == dim_);
  const auto& muDiffusion = diffusion.parameter_type();
  op_ = new OperatorType(muDiffusion);
  for (int ii = 0; ii < dim_; ++ii) {
    OperatorComponentType* comp = new OperatorComponentType(dim_, dim_);
    comp->operator[](ii)[ii] = 1.0;
    op_->register_component(comp,
                            new Dune::Pymor::ParameterFunctional(*(diffusion.coefficient(ii))));
  }
  if (diffusion.hasAffinePart()) {
    OperatorComponentType* aff = new OperatorComponentType(dim_, dim_);
    for (int ii = 0; ii < dim_; ++ii)
      aff->operator[](ii)[ii] = 1.0;
    op_->register_component(aff);
  }
  inherit_parameter_type(op_->parameter_type(), "lhs");
  // right hand side
  const auto& force = *(problem_->force());
  assert(int(force.size()) == dim_);
  const auto& dirichlet = *(problem_->dirichlet());
  assert(int(dirichlet.size()) == dim_);
  const auto& neumann = *(problem_->neumann());
  assert(int(neumann.size()) == dim_);
  func_ = new FunctionalType();
  VectorType* aff;
  const VectorType* ones = new VectorType(dim_, 1.0);
  if (diffusion.hasAffinePart() || force.hasAffinePart() || dirichlet.hasAffinePart() || neumann.hasAffinePart())
    aff = new VectorType(dim_);
  // * force
  if (force.hasAffinePart())
    aff->iadd(ones);
  for (size_t qq = 0; qq < force.size(); ++qq) {
    VectorType* comp = new VectorType(dim_);
    comp->operator[](qq) = 1.0;
    func_->register_component(comp,
                              new Dune::Pymor::ParameterFunctional(*(force.coefficient(qq))));
  }
  // * neumann
  if (neumann.hasAffinePart())
    aff->iadd(ones);
  for (size_t qq = 0; qq < neumann.size(); ++qq) {
    VectorType* comp = new VectorType(dim_);
    comp->operator[](qq) = 1.0;
    func_->register_component(comp,
                              new Dune::Pymor::ParameterFunctional(*(neumann.coefficient(qq))));
  }
  // * dirichlet/diffusion
  VectorType* tmp = create_vector();
  if (diffusion.hasAffinePart()) {
    if (dirichlet.hasAffinePart()) {
      op_->affinePart()->apply(ones, tmp);
      aff->iadd(tmp);
    }
    for (size_t qq = 0; qq < dirichlet.size(); ++qq) {
      VectorType* comp = create_vector();
      comp->operator[](qq) = 1.0;
      func_->register_component(comp,
                                new Dune::Pymor::ParameterFunctional(*(dirichlet.coefficient(qq))));
    }
  }
  Dune::Pymor::ParameterType diffusionDirichletMu;
  for (auto key : diffusion.parameter_type().keys())
    diffusionDirichletMu.set(key, diffusion.parameter_type().get(key));
  for (auto key : dirichlet.parameter_type().keys())
    diffusionDirichletMu.set(key, dirichlet.parameter_type().get(key));
  for (size_t pp = 0; pp < diffusion.size(); ++pp) {
    for (size_t qq = 0; qq < dirichlet.size(); ++qq) {
      VectorType* comp = create_vector();
      VectorType* dirichletComp = create_vector();
      dirichletComp->operator[](qq) = 1.0;
      op_->component(pp)->apply(dirichletComp, comp);
      const std::string expression = "-1.0*(" + diffusion.coefficient(pp)->expression()
                                     + ")*(" + dirichlet.coefficient(qq)->expression() + ")";
      func_->register_component(comp,
                                new Dune::Pymor::ParameterFunctional(diffusionDirichletMu, expression));
      delete dirichletComp;
    }
  }
  inherit_parameter_type(func_->parameter_type(), "rhs");
  delete tmp;
}

SimpleDiscretization::~SimpleDiscretization()
{
  delete func_;
  delete op_;
  delete problem_;
}

std::vector< std::string > SimpleDiscretization::available_operators() const
{
  return { "lhs" };
}

const SimpleDiscretization::OperatorType* SimpleDiscretization::get_operator(const std::string id) const
{
  if (id != "lhs")
    DUNE_PYMOR_THROW(Dune::Pymor::Exception::key_is_not_valid, "id has to be 'lhs' (is " << id << ")!");
  return op_;
}

std::vector< std::string > SimpleDiscretization::available_functionals() const
{
  return { "rhs" };
}

const SimpleDiscretization::FunctionalType* SimpleDiscretization::get_functional(const std::string id) const
{
  if (id != "rhs")
    DUNE_PYMOR_THROW(Dune::Pymor::Exception::key_is_not_valid, "id has to be 'rhs' (is " << id << ")!");
  return func_;
}

SimpleDiscretization::VectorType* SimpleDiscretization::create_vector() const
{
  return new VectorType(dim_);
}

std::vector< std::string > SimpleDiscretization::solver_options() const
{
  return { "problem" };
}

std::string SimpleDiscretization::solver_options(const std::string context) const
{
  if (context != solver_options()[0])
    DUNE_PYMOR_THROW(Dune::Pymor::Exception::key_is_not_valid,
                     "context has to be '" << solver_options()[0] << "' (is '" << context << "')!");
  return OperatorComponentType::invert_options()[0];
}

void SimpleDiscretization::solve(Dune::Pymor::LA::VectorInterface* vector,
                                 const Dune::Pymor::Parameter /*mu*/) const
  throw (Dune::Pymor::Exception::wrong_parameter_type,
         Dune::Pymor::Exception::types_are_not_compatible,
         Dune::Pymor::Exception::you_have_to_implement_this,
         Dune::Pymor::Exception::sizes_do_not_match,
         Dune::Pymor::Exception::wrong_parameter_type,
         Dune::Pymor::Exception::requirements_not_met,
         Dune::Pymor::Exception::linear_solver_failed,
         Dune::Pymor::Exception::this_does_not_make_any_sense)
{
  DUNE_PYMOR_THROW(Dune::Pymor::Exception::types_are_not_compatible,
                   "not implemented for vector of type '" << vector->type() << "'!");
}

void SimpleDiscretization::solve(VectorType* vector, const Dune::Pymor::Parameter mu) const
  throw (Dune::Pymor::Exception::wrong_parameter_type,
         Dune::Pymor::Exception::types_are_not_compatible,
         Dune::Pymor::Exception::you_have_to_implement_this,
         Dune::Pymor::Exception::sizes_do_not_match,
         Dune::Pymor::Exception::wrong_parameter_type,
         Dune::Pymor::Exception::requirements_not_met,
         Dune::Pymor::Exception::linear_solver_failed,
         Dune::Pymor::Exception::this_does_not_make_any_sense)
{
  if (mu.type() != parameter_type())
    DUNE_PYMOR_THROW(Dune::Pymor::Exception::wrong_parameter_type,
                     "type of mu (" << mu.type() << ") does not match the parameter_type of this ("
                     << parameter_type() << ")!");
  // freeze lhs and rhs
  const Dune::Pymor::Parameter mu_lhs = map_parameter(mu, "lhs");
  const auto* lhs = op_->freeze_parameter(mu_lhs);
  const Dune::Pymor::Parameter mu_rhs = map_parameter(mu, "rhs");
  const auto *rhs = func_->freeze_parameter(mu_rhs);
  // solve linear system
  const std::string invert_options = solver_options("problem");
  lhs->apply_inverse(rhs, vector, invert_options);
  // clean up
  delete rhs;
  delete lhs;
}

void SimpleDiscretization::visualize(const Dune::Pymor::LA::VectorInterface* vector,
                                      const std::string /*filename*/,
                                      const std::string /*name*/) const
{
  DUNE_PYMOR_THROW(Dune::Pymor::Exception::types_are_not_compatible,
                   "not implemented for vector of type '" << vector->type() << "'!");
}

void SimpleDiscretization::visualize(const VectorType* vector,
                                     const std::string filename,
                                     const std::string name) const
  throw (Dune::Pymor::Exception::sizes_do_not_match)
{
  if (int(vector->dim()) != dim_)
    DUNE_PYMOR_THROW(Dune::Pymor::Exception::sizes_do_not_match,
                     "size of vector has to be " << dim_ << " is (" << vector->dim() << ")!");
  if (filename.empty())
    DUNE_PYMOR_THROW(Dune::Pymor::Exception::wrong_input, "filename musto not be empty!");
  std::ofstream file;
  file.open(filename);
  if (!file.is_open())
    DUNE_PYMOR_THROW(Dune::Pymor::Exception::io_error, "could not open '" << filename << "' for writing!");
  file << name << " = [";
  for (size_t ii = 0; ii < vector->dim() - 1; ++ii)
    file << vector->operator[](ii) << ", ";
  file << vector->operator[](vector->dim() - 1) << "]" << std::endl;
}


} // namespace Example
