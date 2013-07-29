// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#include <fstream>

#include <dune/stuff/common/string.hh>

#include <dune/pymor/la/container/affine.hh>
#include "stationarylinear.hh"

namespace Example {


const size_t AnalyticalProblem::dim = 4;

AnalyticalProblem::AnalyticalProblem()
  : Dune::Pymor::Parametric()
{
  // build the data functions
  // * diffusion
  const Dune::Pymor::ParameterType muDiffusion = {"diffusion", dim};
  diffusion_ = new FunctionType(new ExpressionFunctionType("x", "1.0"));
  for (size_t ii = 0; ii < dim; ++ii)
    diffusion_->register_component(new ExpressionFunctionType("x", "1.0"),
                                   new Dune::Pymor::ParameterFunctional(muDiffusion,
                                                                        "diffusion["
                                                                        + Dune::Stuff::Common::toString(ii) + "]"));
  inherit_parameter_type(muDiffusion, "diffusion");
  // * force
  const Dune::Pymor::ParameterType muForce = {"force", dim};
  force_ = new FunctionType();
  force_->register_affine_part(new ExpressionFunctionType("x", "1.0"));
  for (size_t ii = 0; ii < dim; ++ii)
    force_->register_component(new ExpressionFunctionType("x", "1.0"),
                               new Dune::Pymor::ParameterFunctional(muForce,
                                                                    "force["
                                                                    + Dune::Stuff::Common::toString(ii) + "]"));
  inherit_parameter_type(muForce, "force");
  // * dirichlet
  const Dune::Pymor::ParameterType muDirichlet = {"dirichlet", dim};
  dirichlet_ = new FunctionType(new ExpressionFunctionType("x", "1.0"));
  for (size_t ii = 0; ii < dim; ++ii)
    dirichlet_->register_component(new ExpressionFunctionType("x", "1.0"),
                                   new Dune::Pymor::ParameterFunctional(muDirichlet,
                                                                        "dirichlet["
                                                                        + Dune::Stuff::Common::toString(ii) + "]"));
  inherit_parameter_type(muDirichlet, "dirichlet");
  // * neumann
  const Dune::Pymor::ParameterType muNeumann = {"neumann", dim};
  neumann_ = new FunctionType(new ExpressionFunctionType("x", "1.0"));
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
  : problem_(prob)
  , dim_(prob->dim)
{
  typedef typename OperatorType::ContainerType MatrixType;
  typedef typename MatrixType::BackendType MatrixBackendType;
  typedef Dune::Pymor::LA::AffinelyDecomposedConstContainer< MatrixType > AffinelyDecomposedMatrixType;
  AffinelyDecomposedMatrixType diffusionMatrix;
  // left hand side
  // * diffusion operator
  const auto& diffusion = *(problem_->diffusion());
  assert(int(diffusion.num_components()) == dim_);
  for (int ii = 0; ii < dim_; ++ii) {
    MatrixBackendType* compMatrix = new MatrixBackendType(dim_, dim_);
    compMatrix->operator[](ii)[ii] = 1.0;
    diffusionMatrix.register_component(new MatrixType(compMatrix),
                                       new Dune::Pymor::ParameterFunctional(*(diffusion.coefficient(ii))));
  }
  if (diffusion.has_affine_part()) {
    MatrixBackendType* affMatrix = new MatrixBackendType(dim_, dim_);
    for (int ii = 0; ii < dim_; ++ii)
      affMatrix->operator[](ii)[ii] = 1.0;
    diffusionMatrix.register_affine_part(new MatrixType(affMatrix));
  }
  op_ = new OperatorType(diffusionMatrix);
  inherit_parameter_type(op_->parameter_type(), "lhs");
  // right hand side
  typedef typename FunctionalType::VectorType VectorType;
  typedef typename VectorType::BackendType VectorBackendType;
  typedef Dune::Pymor::LA::AffinelyDecomposedConstContainer< VectorType > AffinelyDecomposedVectorType;
  const auto& force = *(problem_->force());
  assert(int(force.num_components()) == dim_);
  const auto& dirichlet = *(problem_->dirichlet());
  assert(int(dirichlet.num_components()) == dim_);
  const auto& neumann = *(problem_->neumann());
  assert(int(neumann.num_components()) == dim_);
  VectorType* affVector;
  const VectorType* ones = new VectorType(dim_, 1.0);
  if (diffusion.has_affine_part() || force.has_affine_part()
      || dirichlet.has_affine_part() || neumann.has_affine_part())
    affVector = new VectorType(dim_);
  // * force
  AffinelyDecomposedVectorType rhsVector;
  if (force.has_affine_part())
    affVector->iadd(*ones);
  for (size_t qq = 0; qq < force.num_components(); ++qq) {
    VectorBackendType* compVector = new VectorBackendType(dim_);
    compVector->operator[](qq) = 1.0;
    rhsVector.register_component(new VectorType(compVector),
                                 new Dune::Pymor::ParameterFunctional(*(force.coefficient(qq))));
  }
  // * neumann
  if (neumann.has_affine_part())
    affVector->iadd(*ones);
  for (size_t qq = 0; qq < neumann.num_components(); ++qq) {
    VectorBackendType* compVector = new VectorBackendType(dim_);
    compVector->operator[](qq) = 1.0;
    rhsVector.register_component(new VectorType(compVector),
                                 new Dune::Pymor::ParameterFunctional(*(neumann.coefficient(qq))));
  }
  // * dirichlet/diffusion
  VectorType tmp = create_vector();
  if (diffusion.has_affine_part() && dirichlet.has_affine_part()) {
      op_->affine_part().apply(*ones, tmp);
      affVector->iadd(tmp);
  }
  if (diffusion.has_affine_part()) {
    for (size_t qq = 0; qq < dirichlet.num_components(); ++qq) {
      VectorBackendType* compVector = new VectorBackendType(dim_);
      compVector->operator[](qq) = 1.0;
      rhsVector.register_component(new VectorType(compVector),
                                   new Dune::Pymor::ParameterFunctional(*(dirichlet.coefficient(qq))));
    }
  }
  if (dirichlet.has_affine_part()) {
    for (size_t qq = 0; qq < diffusion.num_components(); ++qq) {
      VectorType* comp = new VectorType(dim_);
      op_->component(qq).apply(*ones, *comp);
      rhsVector.register_component(comp,
                                   new Dune::Pymor::ParameterFunctional(*(diffusion.coefficient(qq))));
    }
  }
  Dune::Pymor::ParameterType diffusionDirichletMu;
  for (auto key : diffusion.parameter_type().keys())
    diffusionDirichletMu.set(key, diffusion.parameter_type().get(key));
  for (auto key : dirichlet.parameter_type().keys())
    diffusionDirichletMu.set(key, dirichlet.parameter_type().get(key));
  for (size_t pp = 0; pp < diffusion.num_components(); ++pp) {
    for (size_t qq = 0; qq < dirichlet.num_components(); ++qq) {
      VectorType* comp = new VectorType(create_vector());
      VectorBackendType* dirichletCompVector = new VectorBackendType(dim_);
      dirichletCompVector->operator[](qq) = 1.0;
      VectorType* dirichletComp = new VectorType(dirichletCompVector);
      op_->component(pp).apply(*dirichletComp, *comp);
      const std::string expression = "-1.0*(" + diffusion.coefficient(pp)->expression()
                                     + ")*(" + dirichlet.coefficient(qq)->expression() + ")";
      rhsVector.register_component(comp,
                                   new Dune::Pymor::ParameterFunctional(diffusionDirichletMu, expression));
      delete dirichletComp;
    }
  }
  func_ = new FunctionalType(rhsVector);
  inherit_parameter_type(func_->parameter_type(), "rhs");
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

typename SimpleDiscretization::OperatorType SimpleDiscretization::get_operator(const std::string id) const
{
  if (id != "lhs")
    DUNE_PYMOR_THROW(Dune::Pymor::Exception::key_is_not_valid, "id has to be 'lhs' (is " << id << ")!");
  return *op_;
}

std::vector< std::string > SimpleDiscretization::available_functionals() const
{
  return { "rhs" };
}

typename SimpleDiscretization::FunctionalType SimpleDiscretization::get_functional(const std::string id) const
{
  if (id != "rhs")
    DUNE_PYMOR_THROW(Dune::Pymor::Exception::key_is_not_valid, "id has to be 'rhs' (is " << id << ")!");
  return *func_;
}

typename SimpleDiscretization::VectorType SimpleDiscretization::create_vector() const
{
  return VectorType(dim_);
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
  return OperatorType::invert_options()[0];
}

void SimpleDiscretization::solve(VectorType& vector, const Dune::Pymor::Parameter mu) const
{
  if (mu.type() != parameter_type())
    DUNE_PYMOR_THROW(Dune::Pymor::Exception::wrong_parameter_type,
                     "type of mu (" << mu.type() << ") does not match the parameter_type of this ("
                     << parameter_type() << ")!");
  if (int(vector.dim()) != dim_)
    DUNE_PYMOR_THROW(Dune::Pymor::Exception::sizes_do_not_match,
                     "size of vector has to be " << dim_ << " is (" << vector.dim() << ")!");
  // freeze lhs and rhs
  const Dune::Pymor::Parameter mu_lhs = map_parameter(mu, "lhs");
  const auto lhs = op_->freeze_parameter(mu_lhs);
  const Dune::Pymor::Parameter mu_rhs = map_parameter(mu, "rhs");
  const auto rhs = func_->freeze_parameter(mu_rhs);
  // solve linear system
  const std::string invert_options = solver_options("problem");
  lhs.apply_inverse(*(rhs.container()), vector, invert_options);
}

void SimpleDiscretization::visualize(const VectorType& vector,
                                     const std::string filename,
                                     const std::string name) const
{
  if (int(vector.dim()) != dim_)
    DUNE_PYMOR_THROW(Dune::Pymor::Exception::sizes_do_not_match,
                     "size of vector has to be " << dim_ << " is (" << vector.dim() << ")!");
  if (filename.empty())
    DUNE_PYMOR_THROW(Dune::Pymor::Exception::wrong_input, "filename must not be empty!");
  std::ofstream file;
  file.open(filename);
  if (!file.is_open())
    DUNE_PYMOR_THROW(Dune::Pymor::Exception::io_error, "could not open '" << filename << "' for writing!");
  file << name << " = [";
  for (int ii = 0; ii < int(vector.dim()) - 1; ++ii)
    file << vector.components({ii})[0] << ", ";
  file << vector.components({int(vector.dim() - 1)})[0] << "]" << std::endl;
}


} // namespace Example
