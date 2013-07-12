// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifdef HAVE_CMAKE_CONFIG
  #include "cmake_config.h"
#elif defined (HAVE_CONFIG_H)
  #include "config.h"
#endif // HAVE_CMAKE_CONFIG

#if HAVE_EIGEN
#include <algorithm>

#include <Eigen/Dense>
#include <Eigen/SparseCore>
#include <Eigen/IterativeLinearSolvers>
#include <Eigen/SparseCholesky>
#if HAVE_SUPERLU
  #include <Eigen/SuperLUSupport>
#endif

#include "eigen.hh"

namespace Dune {
namespace Pymor {
namespace Operators {

// ===================================
// ===== EigenDenseMatrixInverse =====
// ===================================
EigenDenseMatrixInverse::EigenDenseMatrixInverse(const EigenDenseMatrix* op, const std::string type)
  throw (Exception::key_is_not_valid)
  : OperatorInterface()
  , op_(op)
  , type_(type)
{
  const auto invert_ops = EigenDenseMatrix::invert_options();
  const auto result = std::find(invert_ops.begin(), invert_ops.end(), type);
  if (result == invert_ops.end()) {
    std::stringstream msg;
    msg << "type has to be one of {";
    for (size_t ii = 0; ii < invert_ops.size() - 1; ++ii)
      msg << "'" << invert_ops[ii] << "'', ";
    msg << "'" << invert_ops[invert_ops.size()] << "''} (is '" << type << "'')!";
    DUNE_PYMOR_THROW(Exception::key_is_not_valid, msg.str());
  }
}

bool EigenDenseMatrixInverse::linear() const
{
  return true;
}

unsigned int EigenDenseMatrixInverse::dim_source() const
{
  return op_->dim_range();
}

unsigned int EigenDenseMatrixInverse::dim_range() const
{
  return op_->dim_range();
}

std::string EigenDenseMatrixInverse::type_source() const
{
  return SourceType::static_type();
}

std::string EigenDenseMatrixInverse::type_range() const
{
  return RangeType::static_type();
}

void EigenDenseMatrixInverse::apply(const LA::VectorInterface* source,
                                    LA::VectorInterface* range,
                                    const Parameter /*mu*/) const throw (Exception::types_are_not_compatible,
                                                                         Exception::you_have_to_implement_this,
                                                                         Exception::sizes_do_not_match,
                                                                         Exception::wrong_parameter_type,
                                                                         Exception::requirements_not_met,
                                                                         Exception::linear_solver_failed)
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
  DUNE_PYMOR_THROW(Exception::types_are_not_compatible, msg.str());
}

void EigenDenseMatrixInverse::apply(const SourceType* source, RangeType* range, const Parameter mu) const
  throw (Exception::types_are_not_compatible,
         Exception::you_have_to_implement_this,
         Exception::sizes_do_not_match,
         Exception::wrong_parameter_type,
         Exception::requirements_not_met,
         Exception::linear_solver_failed)
{
  if (source->dim() != dim_source())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "dim of source (" << source->dim() << ") does not match dim_source of this (" << dim_source()
                     << ")!");
  if (range->dim() != dim_range())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "dim of range (" << range->dim() << ") does not match dim_range of this (" << dim_range()
                     << ")!");
  if (mu.type() != Parameter().type())
    DUNE_PYMOR_THROW(Exception::wrong_parameter_type,
                     "since parametric() == false mu has to be empty (is " << mu.report() << ")!");
  if (type_ == "householderQR.columnpivoting")
    range->backend() = op_->backend().colPivHouseholderQr().solve(source->backend());
  else if (type_ == "LU.fullpivoting")
    range->backend() = op_->backend().fullPivLu().solve(source->backend());
  else if (type_ == "householderQR.fullpivoting")
    range->backend() = op_->backend().fullPivHouseholderQr().solve(source->backend());
  else if (type_ == "householderQR")
    range->backend() = op_->backend().householderQr().solve(source->backend());
  else if (type_ == "cholesky.LDLT")
    range->backend() = op_->backend().ldlt().solve(source->backend());
  else if (type_ == "cholesky.LLT")
    range->backend() = op_->backend().llt().solve(source->backend());
  else if (type_ == "LU.partialpivoting")
    range->backend() = op_->backend().partialPivLu().solve(source->backend());
  else
    DUNE_PYMOR_THROW(Exception::this_does_not_make_any_sense, "I checked this in the constructor!");
}

std::vector< std::string > EigenDenseMatrixInverse::invert_options() throw(Exception::not_invertible)
{
  return { "exact" };
}

const OperatorInterface* EigenDenseMatrixInverse::invert(const std::string type,
                                                         const Parameter mu) const
  throw(Exception::not_invertible, Exception::key_is_not_valid)
{
  if (!mu.type().empty())
    DUNE_PYMOR_THROW(Exception::this_is_not_parametric,
                     "do not call invert with mu = " << mu << " if parametric() == false!");
  if (type != invert_options()[0])
    DUNE_PYMOR_THROW(Exception::key_is_not_valid,
                     "type has to be '" << invert_options()[0] << "' (is " << type << ")!");
  return op_;
}

void EigenDenseMatrixInverse::apply_inverse(const LA::VectorInterface* range,
                                            LA::VectorInterface* source,
                                            const std::string /*type*/,
                                            const Parameter /*mu*/) const
  throw (Exception::types_are_not_compatible,
         Exception::you_have_to_implement_this,
         Exception::sizes_do_not_match,
         Exception::wrong_parameter_type,
         Exception::requirements_not_met,
         Exception::linear_solver_failed)
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
  DUNE_PYMOR_THROW(Exception::types_are_not_compatible, msg.str());
}

void EigenDenseMatrixInverse::apply_inverse(const RangeType* range,
                                            SourceType* source,
                                            const std::string type,
                                            const Parameter mu) const
  throw (Exception::types_are_not_compatible,
         Exception::you_have_to_implement_this,
         Exception::sizes_do_not_match,
         Exception::wrong_parameter_type,
         Exception::requirements_not_met,
         Exception::linear_solver_failed)
{
  if (type != invert_options()[0])
    DUNE_PYMOR_THROW(Exception::key_is_not_valid,
                     "type has to be '" << invert_options()[0] << "' (is " << type << ")!");
  op_->apply(range, source, mu);
}

EigenDenseMatrixInverse* EigenDenseMatrixInverse::freeze_parameter(const Parameter /*mu*/) const
  throw (Exception::this_is_not_parametric)
{
  DUNE_PYMOR_THROW(Exception::this_is_not_parametric, "do not call freeze_parameter if parametric() == false!");
  return nullptr;
}


// ============================
// ===== EigenDenseMatrix =====
// ============================
EigenDenseMatrix::EigenDenseMatrix()
  : BaseType()
  , LinearOperatorInterface()
{}

EigenDenseMatrix::EigenDenseMatrix(const BaseType& other)
  : BaseType(other)
  , LinearOperatorInterface()
{}

EigenDenseMatrix::EigenDenseMatrix(const int rr, const int cc)
  : BaseType(assert_is_positive(rr), assert_is_positive(cc))
  , LinearOperatorInterface()
{}

unsigned int EigenDenseMatrix::dim_source() const
{
  return BaseType::cols();
}

unsigned int EigenDenseMatrix::dim_range() const
{
  return BaseType::rows();
}

std::string EigenDenseMatrix::type_source() const
{
  return SourceType::static_type();
}

std::string EigenDenseMatrix::type_range() const
{
  return RangeType::static_type();
}

void EigenDenseMatrix::apply(const LA::VectorInterface* source,
                             LA::VectorInterface* range,
                             const Parameter /*mu*/) const throw (Exception::types_are_not_compatible,
                                                                  Exception::you_have_to_implement_this,
                                                                  Exception::sizes_do_not_match,
                                                                  Exception::wrong_parameter_type,
                                                                  Exception::requirements_not_met,
                                                                  Exception::linear_solver_failed)
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
  DUNE_PYMOR_THROW(Exception::types_are_not_compatible, msg.str());
}

void EigenDenseMatrix::apply(const SourceType* source, RangeType* range, const Parameter mu) const
  throw (Exception::types_are_not_compatible,
         Exception::you_have_to_implement_this,
         Exception::sizes_do_not_match,
         Exception::wrong_parameter_type,
         Exception::requirements_not_met,
         Exception::linear_solver_failed)
{
  if (source->dim() != dim_source())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "dim of source (" << source->dim() << ") does not match dim_source of this (" << dim_source()
                     << ")!");
  if (range->dim() != dim_range())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "dim of range (" << range->dim() << ") does not match dim_range of this (" << dim_range()
                     << ")!");
  if (mu.type() != Parameter().type())
    DUNE_PYMOR_THROW(Exception::wrong_parameter_type,
                     "since parametric() == false mu has to be empty (is " << mu.report() << ")!");
  range->backend() = BaseType::backend() * source->backend();
}

double EigenDenseMatrix::apply2(const RangeType* range,
                                const SourceType* source,
                                const Parameter mu) const
  throw (Exception::types_are_not_compatible,
         Exception::you_have_to_implement_this,
         Exception::sizes_do_not_match,
         Exception::wrong_parameter_type,
         Exception::requirements_not_met,
         Exception::linear_solver_failed)
{
  if (source->dim() != dim_source())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "size of U (" << source->dim() << ") does not match dim_source() of this (" << dim_source() << ")!");
  if (range->dim() != dim_range())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "size of V (" << range->dim() << ") does not match dim_range() of this (" << dim_range() << ")!");
  if (mu.type() != Parameter().type())
    DUNE_PYMOR_THROW(Exception::wrong_parameter_type,
                     "since parametric() == false mu has to be empty (is " << mu.report() << ")!");
  return range->backend().transpose() * BaseType::backend() * source->backend();
}

std::vector< std::string > EigenDenseMatrix::invert_options() throw(Exception::not_invertible)
{
  return {
      "householderQR.columnpivoting"
    , "LU.fullpivoting"
    , "householderQR.fullpivoting"
    , "householderQR"
    , "cholesky.LDLT"
    , "cholesky.LLT"
    , "LU.partialpivoting"
  };
}

const EigenDenseMatrixInverse* EigenDenseMatrix::invert(const std::string type,
                                                        const Parameter mu) const
  throw(Exception::not_invertible, Exception::key_is_not_valid)
{
  if (!mu.type().empty())
    DUNE_PYMOR_THROW(Exception::this_is_not_parametric,
                     "do not call invert with mu = " << mu << " if parametric() == false!");
  const auto invert_ops = invert_options();
  const auto result = std::find(invert_ops.begin(), invert_ops.end(), type);
  if (result == invert_ops.end()) {
    std::stringstream msg;
    msg << "type has to be one of {";
    for (size_t ii = 0; ii < invert_ops.size() - 1; ++ii)
      msg << "'" << invert_ops[ii] << "'', ";
    msg << "'" << invert_ops[invert_ops.size()] << "''} (is '" << type << "'')!";
    DUNE_PYMOR_THROW(Exception::key_is_not_valid, msg.str());
  }
  return new EigenDenseMatrixInverse(this, type);
}

void EigenDenseMatrix::apply_inverse(const LA::VectorInterface* range,
                                     LA::VectorInterface* source,
                                     const std::string /*type*/,
                                     const Parameter /*mu*/) const
  throw (Exception::types_are_not_compatible,
         Exception::you_have_to_implement_this,
         Exception::sizes_do_not_match,
         Exception::wrong_parameter_type,
         Exception::requirements_not_met,
         Exception::linear_solver_failed)
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
  DUNE_PYMOR_THROW(Exception::types_are_not_compatible, msg.str());
}

void EigenDenseMatrix::apply_inverse(const RangeType* range,
                                     SourceType* source,
                                     const std::string type,
                                     const Parameter mu) const
  throw (Exception::types_are_not_compatible,
         Exception::you_have_to_implement_this,
         Exception::sizes_do_not_match,
         Exception::wrong_parameter_type,
         Exception::requirements_not_met,
         Exception::linear_solver_failed)
{
  const EigenDenseMatrixInverse* inverseOp = invert(type, mu);
  inverseOp->apply(range, source);;
  delete inverseOp;
}

EigenDenseMatrix* EigenDenseMatrix::freeze_parameter(const Parameter /*mu*/) const
  throw (Exception::this_is_not_parametric)
{
  DUNE_PYMOR_THROW(Exception::this_is_not_parametric, "do not call freeze_parameter if parametric() == false!");
  return nullptr;
}

EigenDenseMatrix* EigenDenseMatrix::copy() const
{
  return new EigenDenseMatrix(*this);
}

void EigenDenseMatrix::scal(const double alpha)
{
  backend() *= alpha;
}

void EigenDenseMatrix::axpy(const double /*alpha*/, const LinearOperatorInterface* /*x*/)
  throw (Exception::sizes_do_not_match, Exception::types_are_not_compatible)
{
  DUNE_PYMOR_THROW(Exception::types_are_not_compatible,
                   "not implemented for arbitrary x!");
}

void EigenDenseMatrix::axpy(const double alpha, const EigenDenseMatrix* x) throw (Exception::sizes_do_not_match,
                                                                                  Exception::types_are_not_compatible)
{
  if (x->dim_range() != dim_range())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "the dim_range of x (" << x->dim_range() << ") does not match the dim_range of this ("
                     << dim_range() << ")!");
  if (x->dim_source() != dim_source())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "the dim_source of x (" << x->dim_source() << ") does not match the dim_source of this ("
                     << dim_source() << ")!");
  backend() += alpha * x->backend();
}

int EigenDenseMatrix::assert_is_positive(const int ii)
{
  if (ii <= 0) DUNE_PYMOR_THROW(Exception::index_out_of_range, "ii has to be positive (is " << ii << ")!");
  return ii;
}


// ============================================
// ===== EigenRowMajorSparseMatrixInverse =====
// ============================================
EigenRowMajorSparseMatrixInverse::EigenRowMajorSparseMatrixInverse(const EigenRowMajorSparseMatrix* op,
                                                                   const std::string type)
  : OperatorInterface()
  , op_(op)
  , type_(type)
{
  const auto invert_ops = EigenRowMajorSparseMatrix::invert_options();
  const auto result = std::find(invert_ops.begin(), invert_ops.end(), type);
  if (result == invert_ops.end()) {
    std::stringstream msg;
    msg << "type has to be one of {";
    for (size_t ii = 0; ii < invert_ops.size() - 1; ++ii)
      msg << "'" << invert_ops[ii] << "'', ";
    msg << "'" << invert_ops[invert_ops.size()] << "''} (is '" << type << "'')!";
    DUNE_PYMOR_THROW(Exception::key_is_not_valid, msg.str());
  }
}

bool EigenRowMajorSparseMatrixInverse::linear() const
{
  return true;
}

unsigned int EigenRowMajorSparseMatrixInverse::dim_source() const
{
  return op_->dim_range();
}

unsigned int EigenRowMajorSparseMatrixInverse::dim_range() const
{
  return op_->dim_source();
}

std::string EigenRowMajorSparseMatrixInverse::type_source() const
{
  return SourceType::static_type();
}

std::string EigenRowMajorSparseMatrixInverse::type_range() const
{
  return RangeType::static_type();
}

void EigenRowMajorSparseMatrixInverse::apply(const LA::VectorInterface* source,
                                             LA::VectorInterface* range,
                                             const Parameter /*mu*/) const throw (Exception::types_are_not_compatible,
                                                                                  Exception::you_have_to_implement_this,
                                                                                  Exception::sizes_do_not_match,
                                                                                  Exception::wrong_parameter_type,
                                                                                  Exception::requirements_not_met,
                                                                                  Exception::linear_solver_failed)
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
  DUNE_PYMOR_THROW(Exception::types_are_not_compatible, msg.str());
}

void EigenRowMajorSparseMatrixInverse::apply(const SourceType* source, RangeType* range, const Parameter mu) const
  throw (Exception::types_are_not_compatible,
         Exception::you_have_to_implement_this,
         Exception::sizes_do_not_match,
         Exception::wrong_parameter_type,
         Exception::requirements_not_met,
         Exception::linear_solver_failed)
{
  if (source->dim() != dim_source())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "dim of source (" << source->dim() << ") does not match dim_source of this ("
                     << dim_source() << ")!");
  if (mu.type() != Parametric::parameter_type())
    DUNE_PYMOR_THROW(Exception::wrong_parameter_type,
                     "since parametric() == false mu has to be empty (is " << mu << ")!");
  if (type_ == "cg.diagonal.lower") {
    typedef ::Eigen::ConjugateGradient< typename EigenRowMajorSparseMatrix::BackendType,
                                        ::Eigen::Lower,
                                        ::Eigen::DiagonalPreconditioner< double > > EigenSolverType;
    EigenSolverType eigenSolver(op_->backend());
    eigenSolver.setMaxIterations(5000);
    eigenSolver.setTolerance(1e-12);
    range->backend() = eigenSolver.solve(source->backend());
    if (eigenSolver.info() != ::Eigen::Success)
      DUNE_PYMOR_THROW(Exception::linear_solver_failed,
                       "EIGEN solver reported error code '" << eigenSolver.info() << "',\n"
                       "see: http://eigen.tuxfamily.org/dox/group__enums.html#ga51bc1ac16f26ebe51eae1abb77bd037b !");
  } else if (type_ == "cg.diagonal.upper") {
      typedef ::Eigen::ConjugateGradient< typename EigenRowMajorSparseMatrix::BackendType,
                                          ::Eigen::Upper,
                                          ::Eigen::DiagonalPreconditioner< double > > EigenSolverType;
      EigenSolverType eigenSolver(op_->backend());
      eigenSolver.setMaxIterations(5000);
      eigenSolver.setTolerance(1e-8);
      range->backend() = eigenSolver.solve(source->backend());
      if (eigenSolver.info() != ::Eigen::Success)
        DUNE_PYMOR_THROW(Exception::linear_solver_failed,
                         "EIGEN solver reported error code '" << eigenSolver.info() << "',\n"
                         "see: http://eigen.tuxfamily.org/dox/group__enums.html#ga51bc1ac16f26ebe51eae1abb77bd037b !");
  } else if (type_ == "bicgstab.identity") {
      typedef ::Eigen::BiCGSTAB<  typename EigenRowMajorSparseMatrix::BackendType,
                                  ::Eigen::IdentityPreconditioner > EigenSolverType;
      EigenSolverType eigenSolver(op_->backend());
      eigenSolver.setMaxIterations(5000);
      eigenSolver.setTolerance(1e-8);
      range->backend() = eigenSolver.solve(source->backend());
      if (eigenSolver.info() != ::Eigen::Success)
        DUNE_PYMOR_THROW(Exception::linear_solver_failed,
                         "EIGEN solver reported error code '" << eigenSolver.info() << "',\n"
                         "see: http://eigen.tuxfamily.org/dox/group__enums.html#ga51bc1ac16f26ebe51eae1abb77bd037b !");
  } else if (type_ == "bicgstab.diagonal") {
      typedef ::Eigen::BiCGSTAB<  typename EigenRowMajorSparseMatrix::BackendType,
                                  ::Eigen::DiagonalPreconditioner< double > > EigenSolverType;
      EigenSolverType eigenSolver(op_->backend());
      eigenSolver.setMaxIterations(5000);
      eigenSolver.setTolerance(1e-8);
      range->backend() = eigenSolver.solve(source->backend());
      if (eigenSolver.info() != ::Eigen::Success)
        DUNE_PYMOR_THROW(Exception::linear_solver_failed,
                         "EIGEN solver reported error code '" << eigenSolver.info() << "',\n"
                         "see: http://eigen.tuxfamily.org/dox/group__enums.html#ga51bc1ac16f26ebe51eae1abb77bd037b !");
  } else if (type_ == "bicgstab.ilut") {
      typedef ::Eigen::BiCGSTAB<  typename EigenRowMajorSparseMatrix::BackendType,
                                  ::Eigen::IncompleteLUT< double > > EigenSolverType;
      EigenSolverType eigenSolver(op_->backend());
      eigenSolver.setMaxIterations(5000);
      eigenSolver.setTolerance(1e-8);
      eigenSolver.preconditioner().setDroptol(1e-4);
      eigenSolver.preconditioner().setFillfactor(10);
      range->backend() = eigenSolver.solve(source->backend());
      if (eigenSolver.info() != ::Eigen::Success)
        DUNE_PYMOR_THROW(Exception::linear_solver_failed,
                         "EIGEN solver reported error code '" << eigenSolver.info() << "',\n"
                         "see: http://eigen.tuxfamily.org/dox/group__enums.html#ga51bc1ac16f26ebe51eae1abb77bd037b !");
#if HAVE_SUPERLU
  } else if (type_ == "superlu") {
      typedef ::Eigen::SuperLU< typename EigenRowMajorSparseMatrix::BackendType > EigenSolverType;
      EigenSolverType eigenSolver;
      eigenSolver.compute(op_->backend());
      range->backend() = eigenSolver.solve(source->backend());
      if (eigenSolver.info() != ::Eigen::Success)
        DUNE_PYMOR_THROW(Exception::linear_solver_failed,
                         "EIGEN solver reported error code '" << eigenSolver.info() << "',\n"
                         "see: http://eigen.tuxfamily.org/dox/group__enums.html#ga51bc1ac16f26ebe51eae1abb77bd037b !");
#endif // HAVE_SUPERLU
  } else
    DUNE_PYMOR_THROW(Exception::this_does_not_make_any_sense, "I checked this in the constructor!");
  assert(false);
}

std::vector< std::string > EigenRowMajorSparseMatrixInverse::invert_options()
{
  return { "exact" };
}

const OperatorInterface* EigenRowMajorSparseMatrixInverse::invert(const std::string type,
                                                                  const Parameter mu) const
  throw(Exception::not_invertible, Exception::key_is_not_valid)
{
  if (!mu.type().empty())
    DUNE_PYMOR_THROW(Exception::this_is_not_parametric,
                     "do not call invert with mu = " << mu << " if parametric() == false!");
  if (type != invert_options()[0])
    DUNE_PYMOR_THROW(Exception::key_is_not_valid,
                     "type has to be '" << invert_options()[0] << "' (is " << type << ")!");
  return op_;
}

void EigenRowMajorSparseMatrixInverse::apply_inverse(const LA::VectorInterface* range,
                                                     LA::VectorInterface* source,
                                                     const std::string /*type*/,
                                                     const Parameter /*mu*/) const
  throw (Exception::types_are_not_compatible,
         Exception::you_have_to_implement_this,
         Exception::sizes_do_not_match,
         Exception::wrong_parameter_type,
         Exception::requirements_not_met,
         Exception::linear_solver_failed)
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
  DUNE_PYMOR_THROW(Exception::types_are_not_compatible, msg.str());
}

void EigenRowMajorSparseMatrixInverse::apply_inverse(const RangeType* range,
                           SourceType* source,
                           const std::string type,
                           const Parameter mu) const
  throw (Exception::types_are_not_compatible,
         Exception::you_have_to_implement_this,
         Exception::sizes_do_not_match,
         Exception::wrong_parameter_type,
         Exception::requirements_not_met,
         Exception::linear_solver_failed)
{
  if (type != invert_options()[0])
    DUNE_PYMOR_THROW(Exception::key_is_not_valid,
                     "type has to be '" << invert_options()[0] << "' (is " << type << ")!");
  op_->apply(range, source, mu);
}

EigenRowMajorSparseMatrixInverse* EigenRowMajorSparseMatrixInverse::freeze_parameter(const Parameter /*mu*/) const
  throw (Exception::this_is_not_parametric)
{
  DUNE_PYMOR_THROW(Exception::this_is_not_parametric, "do not call freeze_parameter if parametric() == false!");
  return nullptr;
}


// =====================================
// ===== EigenRowMajorSparseMatrix =====
// =====================================
EigenRowMajorSparseMatrix::EigenRowMajorSparseMatrix()
  : BaseType()
  , LinearOperatorInterface()
{}

EigenRowMajorSparseMatrix::EigenRowMajorSparseMatrix(const BaseType& other)
  : BaseType(other)
  , LinearOperatorInterface()
{}

EigenRowMajorSparseMatrix::EigenRowMajorSparseMatrix(const int rr,
                                                     const int cc,
                                                     const Dune::Stuff::LA::SparsityPatternDefault& pattern)
  : BaseType(assert_is_positive(rr), assert_is_positive(cc), pattern)
  , LinearOperatorInterface()
{}

bool EigenRowMajorSparseMatrix::linear() const
{
  return true;
}

unsigned int EigenRowMajorSparseMatrix::dim_source() const
{
  return BaseType::cols();
}

unsigned int EigenRowMajorSparseMatrix::dim_range() const
{
  return BaseType::rows();
}

std::string EigenRowMajorSparseMatrix::type_source() const
{
  return SourceType::static_type();
}

std::string EigenRowMajorSparseMatrix::type_range() const
{
  return RangeType::static_type();
}

void EigenRowMajorSparseMatrix::apply(const LA::VectorInterface* source,
                                      LA::VectorInterface* range,
                                      const Parameter /*mu*/) const throw (Exception::types_are_not_compatible,
                                                                           Exception::you_have_to_implement_this,
                                                                           Exception::sizes_do_not_match,
                                                                           Exception::wrong_parameter_type,
                                                                           Exception::requirements_not_met,
                                                                           Exception::linear_solver_failed)
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
  DUNE_PYMOR_THROW(Exception::types_are_not_compatible, msg.str());
}

void EigenRowMajorSparseMatrix::apply(const SourceType* source, RangeType* range, const Parameter mu) const
  throw (Exception::types_are_not_compatible,
         Exception::you_have_to_implement_this,
         Exception::sizes_do_not_match,
         Exception::wrong_parameter_type,
         Exception::requirements_not_met,
         Exception::linear_solver_failed)
{
  if (source->dim() != dim_source())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "dim of source (" << source->dim() << ") does not match dim_source of this ("
                     << dim_source() << ")!");
  if (mu.type() != Parametric::parameter_type())
    DUNE_PYMOR_THROW(Exception::wrong_parameter_type,
                     "since parametric() == false mu has to be empty (is " << mu << ")!");
  range->backend() = BaseType::backend() * source->backend();
}

double EigenRowMajorSparseMatrix::apply2(const SourceType* range, const RangeType* source, const Parameter /*mu*/) const
  throw (Exception::types_are_not_compatible,
         Exception::you_have_to_implement_this,
         Exception::sizes_do_not_match,
         Exception::wrong_parameter_type,
         Exception::requirements_not_met,
         Exception::linear_solver_failed)
{
  if (source->dim() != dim_source())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "dim of source (" << source->dim() << ") does not match dim_source of this (" << dim_source()
                     << ")!");
  if (range->dim() != dim_range())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "dim of range (" << range->dim() << ") does not match dim_range of this (" << dim_range()
                     << ")!");
  return range->backend().transpose() * BaseType::backend() * source->backend();
}

std::vector< std::string > EigenRowMajorSparseMatrix::invert_options()
{
  return {
        "bicgstab.ilut"
      , "bicgstab.diagonal"
      , "bicgstab.identity"
#if HAVE_SUPERLU
      , "superlu"
#endif
      , "cg.diagonal.lower"
      , "cg.diagonal.upper"
  };
}

const EigenRowMajorSparseMatrixInverse* EigenRowMajorSparseMatrix::invert(const std::string type,
                                                                          const Parameter mu) const
  throw(Exception::not_invertible, Exception::key_is_not_valid)
{
  if (!mu.type().empty())
    DUNE_PYMOR_THROW(Exception::this_is_not_parametric,
                     "do not call invert with mu = " << mu << " if parametric() == false!");
  const auto invert_ops = invert_options();
  const auto result = std::find(invert_ops.begin(), invert_ops.end(), type);
  if (result == invert_ops.end()) {
    std::stringstream msg;
    msg << "type has to be one of {";
    for (size_t ii = 0; ii < invert_ops.size() - 1; ++ii)
      msg << "'" << invert_ops[ii] << "'', ";
    msg << "'" << invert_ops[invert_ops.size()] << "''} (is '" << type << "'')!";
    DUNE_PYMOR_THROW(Exception::key_is_not_valid, msg.str());
  }
  return new EigenRowMajorSparseMatrixInverse(this, type);
}

void EigenRowMajorSparseMatrix::apply_inverse(const LA::VectorInterface* range,
                                              LA::VectorInterface* source,
                                              const std::string /*type*/,
                                              const Parameter /*mu*/) const
  throw (Exception::types_are_not_compatible,
         Exception::you_have_to_implement_this,
         Exception::sizes_do_not_match,
         Exception::wrong_parameter_type,
         Exception::requirements_not_met,
         Exception::linear_solver_failed)
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
  DUNE_PYMOR_THROW(Exception::types_are_not_compatible, msg.str());
}

void EigenRowMajorSparseMatrix::apply_inverse(const RangeType* range,
                                              SourceType* source,
                                              const std::string type,
                                              const Parameter mu) const
  throw (Exception::types_are_not_compatible,
         Exception::you_have_to_implement_this,
         Exception::sizes_do_not_match,
         Exception::wrong_parameter_type,
         Exception::requirements_not_met,
         Exception::linear_solver_failed)
{
  const EigenRowMajorSparseMatrixInverse* inverseOp = invert(type, mu);
  inverseOp->apply(range, source);;
  delete inverseOp;
}

EigenRowMajorSparseMatrix* EigenRowMajorSparseMatrix::freeze_parameter(const Parameter /*mu*/) const
  throw (Exception::this_is_not_parametric)
{
  DUNE_PYMOR_THROW(Exception::this_is_not_parametric, "do not call freeze_parameter if parametric() == false!");
  return nullptr;
}

EigenRowMajorSparseMatrix* EigenRowMajorSparseMatrix::copy() const
{
  return new EigenRowMajorSparseMatrix(*this);
}

void EigenRowMajorSparseMatrix::scal(const double alpha)
{
  backend() *= alpha;
}

void EigenRowMajorSparseMatrix::axpy(const double /*alpha*/, const LinearOperatorInterface* /*x*/)
  throw (Exception::sizes_do_not_match, Exception::types_are_not_compatible)
{
  DUNE_PYMOR_THROW(Exception::types_are_not_compatible,
                   "not implemented for arbitrary x!");
}

void EigenRowMajorSparseMatrix::axpy(const double alpha, const EigenRowMajorSparseMatrix* x)
  throw (Exception::sizes_do_not_match, Exception::types_are_not_compatible)
{
  if (x->dim_range() != dim_range())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "the dim_range of x (" << x->dim_range() << ") does not match the dim_range of this ("
                     << dim_range() << ")!");
  if (x->dim_source() != dim_source())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "the dim_source of x (" << x->dim_source() << ") does not match the dim_source of this ("
                     << dim_source() << ")!");
  backend() += alpha * x->backend();
}

int EigenRowMajorSparseMatrix::assert_is_positive(const int ii)
{
  if (ii <= 0) DUNE_PYMOR_THROW(Exception::index_out_of_range, "ii has to be positive (is " << ii << ")!");
  return ii;
}


} // namespace Operators
} // namespace Pymor
} // namespace Dune

#endif // HAVE_EIGEN
