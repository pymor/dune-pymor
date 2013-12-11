// This file is part of the dune-pymor project:
//   https://github.com/pymor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#include "config.h"

#if HAVE_EIGEN

#include <algorithm>

#include <Eigen/Dense>
#include <Eigen/SparseCore>
#include <Eigen/IterativeLinearSolvers>
#include <Eigen/SparseCholesky>
#if HAVE_SUPERLU
# include <Eigen/SuperLUSupport>
#endif

#include "eigen.hh"
#include "affine.hh"

namespace Dune {
namespace Pymor {
namespace Operators {


//// ===================================
//// ===== EigenDenseMatrixInverse =====
//// ===================================
//EigenDenseMatrixInverse::EigenDenseMatrixInverse(const EigenDenseMatrix* op, const std::string type)
//  throw (Exception::key_is_not_valid)
//  : OperatorInterface()
//  , op_(op)
//  , type_(type)
//{
//  const auto invert_ops = EigenDenseMatrix::invert_options();
//  const auto result = std::find(invert_ops.begin(), invert_ops.end(), type);
//  if (result == invert_ops.end()) {
//    std::stringstream msg;
//    msg << "type has to be one of {";
//    for (size_t ii = 0; ii < invert_ops.size() - 1; ++ii)
//      msg << "'" << invert_ops[ii] << "'', ";
//    msg << "'" << invert_ops[invert_ops.size()] << "''} (is '" << type << "'')!";
//    DUNE_PYMOR_THROW(Exception::key_is_not_valid, msg.str());
//  }
//}

//bool EigenDenseMatrixInverse::linear() const
//{
//  return true;
//}

//unsigned int EigenDenseMatrixInverse::dim_source() const
//{
//  return op_->dim_range();
//}

//unsigned int EigenDenseMatrixInverse::dim_range() const
//{
//  return op_->dim_range();
//}

//std::string EigenDenseMatrixInverse::type_source() const
//{
//  return SourceType::static_type();
//}

//std::string EigenDenseMatrixInverse::type_range() const
//{
//  return RangeType::static_type();
//}

//void EigenDenseMatrixInverse::apply(const LA::VectorInterface* source,
//                                    LA::VectorInterface* range,
//                                    const Parameter /*mu*/) const throw (Exception::types_are_not_compatible,
//                                                                         Exception::you_have_to_implement_this,
//                                                                         Exception::sizes_do_not_match,
//                                                                         Exception::wrong_parameter_type,
//                                                                         Exception::requirements_not_met,
//                                                                         Exception::linear_solver_failed,
//                                                                         Exception::this_does_not_make_any_sense)
//{
//  std::stringstream msg;
//  size_t throw_up = 0;
//  if (source->type() != type_source()) {
//    msg << "source (" << source->type() << ") is not a compatible type_source (" << type_source() << ")";
//    ++throw_up;
//  }
//  if (range->type() != type_range()) {
//    if (throw_up)
//      msg << " and ";
//    msg << "range (" << range->type() << ") is not a compatible type_range (" << type_range() << ")";
//  }
//  DUNE_PYMOR_THROW(Exception::types_are_not_compatible, msg.str());
//}

//void EigenDenseMatrixInverse::apply(const SourceType* source, RangeType* range, const Parameter mu) const
//  throw (Exception::types_are_not_compatible,
//         Exception::you_have_to_implement_this,
//         Exception::sizes_do_not_match,
//         Exception::wrong_parameter_type,
//         Exception::requirements_not_met,
//         Exception::linear_solver_failed,
//         Exception::this_does_not_make_any_sense)
//{
//  if (source->dim() != dim_source())
//    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
//                     "dim of source (" << source->dim() << ") does not match dim_source of this (" << dim_source()
//                     << ")!");
//  if (range->dim() != dim_range())
//    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
//                     "dim of range (" << range->dim() << ") does not match dim_range of this (" << dim_range()
//                     << ")!");
//  if (mu.type() != Parameter().type())
//    DUNE_PYMOR_THROW(Exception::wrong_parameter_type,
//                     "since parametric() == false mu has to be empty (is " << mu.report() << ")!");
//  if (type_ == "householderQR.columnpivoting")
//    range->backend() = op_->backend().colPivHouseholderQr().solve(source->backend());
//  else if (type_ == "LU.fullpivoting")
//    range->backend() = op_->backend().fullPivLu().solve(source->backend());
//  else if (type_ == "householderQR.fullpivoting")
//    range->backend() = op_->backend().fullPivHouseholderQr().solve(source->backend());
//  else if (type_ == "householderQR")
//    range->backend() = op_->backend().householderQr().solve(source->backend());
//  else if (type_ == "cholesky.LDLT")matrix_->backend_
//    range->backend() = op_->backend().ldlt().solve(source->backend());
//  else if (type_ == "cholesky.LLT")
//    range->backend() = op_->backend().llt().solve(source->backend());
//  else if (type_ == "LU.partialpivoting")
//    range->backend() = op_->backend().partialPivLu().solve(source->backend());
//  else
//    DUNE_PYMOR_THROW(Exception::this_does_not_make_any_sense, "I checked this in the constructor!");
//}

//std::vector< std::string > EigenDenseMatrixInverse::invert_options() throw(Exception::not_invertible)
//{
//  return { "exact" };
//}

//const OperatorInterface* EigenDenseMatrixInverse::invert(const std::string type,
//                                                         const Parameter mu) const
//  throw(Exception::not_invertible, Exception::key_is_not_valid)
//{
//  if (!mu.type().empty())
//    DUNE_PYMOR_THROW(Exception::this_is_not_parametric,
//                     "do not call invert with mu = " << mu << " if parametric() == false!");
//  if (type != invert_options()[0])
//    DUNE_PYMOR_THROW(Exception::key_is_not_valid,
//                     "type has to be '" << invert_options()[0] << "' (is " << type << ")!");
//  return op_;
//}

//void EigenDenseMatrixInverse::apply_inverse(const LA::VectorInterface* range,
//                                            LA::VectorInterface* source,
//                                            const std::string /*type*/,
//                                            const Parameter /*mu*/) const
//  throw (Exception::types_are_not_compatible,
//         Exception::you_have_to_implement_this,
//         Exception::sizes_do_not_match,
//         Exception::wrong_parameter_type,
//         Exception::requirements_not_met,
//         Exception::linear_solver_failed,
//         Exception::this_does_not_make_any_sense)
//{
//  std::stringstream msg;
//  size_t throw_up = 0;
//  if (source->type() != type_source()) {
//    msg << "source (" << source->type() << ") is not a compatible type_source (" << type_source() << ")";
//    ++throw_up;
//  }
//  if (range->type() != type_range()) {
//    if (throw_up)
//      msg << " and ";
//    msg << "range (" << range->type() << ") is not a compatible type_range (" << type_range() << ")";
//  }
//  DUNE_PYMOR_THROW(Exception::types_are_not_compatible, msg.str());
//}

//void EigenDenseMatrixInverse::apply_inverse(const RangeType* range,
//                                            SourceType* source,
//                                            const std::string type,
//                                            const Parameter mu) const
//  throw (Exception::types_are_not_compatible,
//         Exception::you_have_to_implement_this,
//         Exception::sizes_do_not_match,
//         Exception::wrong_parameter_type,
//         Exception::requirements_not_met,
//         Exception::linear_solver_failed,
//         Exception::this_does_not_make_any_sense)
//{
//  if (type != invert_options()[0])
//    DUNE_PYMOR_THROW(Exception::key_is_not_valid,
//                     "type has to be '" << invert_options()[0] << "' (is " << type << ")!");
//  op_->apply(range, source, mu);
//}

//EigenDenseMatrixInverse* EigenDenseMatrixInverse::freeze_parameter(const Parameter /*mu*/) const
//  throw (Exception::this_is_not_parametric)
//{
//  DUNE_PYMOR_THROW(Exception::this_is_not_parametric, "do not call freeze_parameter if parametric() == false!");
//  return nullptr;
//}


//// ============================
//// ===== EigenDenseMatrix =====
//// ============================
//EigenDenseMatrix::EigenDenseMatrix()
//  : BaseType()
//  , LinearOperatorInterface()
//{}

//EigenDenseMatrix::EigenDenseMatrix(const BaseType& other)
//  : BaseType(other)
//  , LinearOperatorInterface()
//{}

//EigenDenseMatrix::EigenDenseMatrix(const int rr, const int cc)
//  : BaseType(assert_is_positive(rr), assert_is_positive(cc))
//  , LinearOperatorInterface()
//{}

//unsigned int EigenDenseMatrix::dim_source() const
//{
//  return BaseType::cols();
//}

//unsigned int EigenDenseMatrix::dim_range() const
//{
//  return BaseType::rows();
//}

//std::string EigenDenseMatrix::type_source() const
//{
//  return SourceType::static_type();
//}

//std::string EigenDenseMatrix::type_range() const
//{
//  return RangeType::static_type();
//}

//void EigenDenseMatrix::apply(const LA::VectorInterface* source,
//                             LA::VectorInterface* range,
//                             const Parameter /*mu*/) const throw (Exception::types_are_not_compatible,
//                                                                  Exception::you_have_to_implement_this,
//                                                                  Exception::sizes_do_not_match,
//                                                                  Exception::wrong_parameter_type,
//                                                                  Exception::requirements_not_met,
//                                                                  Exception::linear_solver_failed,
//                                                                  Exception::this_does_not_make_any_sense)
//{
//  std::stringstream msg;
//  size_t throw_up = 0;
//  if (source->type() != type_source()) {
//    msg << "source (" << source->type() << ") is not a compatible type_source (" << type_source() << ")";
//    ++throw_up;
//  }
//  if (range->type() != type_range()) {
//    if (throw_up)
//      msg << " and ";
//    msg << "range (" << range->type() << ") is not a compatible type_range (" << type_range() << ")";
//  }
//  DUNE_PYMOR_THROW(Exception::types_are_not_compatible, msg.str());
//}

//void EigenDenseMatrix::apply(const SourceType* source, RangeType* range, const Parameter mu) const
//  throw (Exception::types_are_not_compatible,
//         Exception::you_have_to_implement_this,
//         Exception::sizes_do_not_match,
//         Exception::wrong_parameter_type,
//         Exception::requirements_not_met,
//         Exception::linear_solver_failed,
//         Exception::this_does_not_make_any_sense)
//{
//  if (source->dim() != dim_source())
//    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
//                     "dim of source (" << source->dim() << ") does not match dim_source of this (" << dim_source()
//                     << ")!");
//  if (range->dim() != dim_range())
//    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
//                     "dim of range (" << range->dim() << ") does not match dim_range of this (" << dim_range()
//                     << ")!");
//  if (mu.type() != Parameter().type())
//    DUNE_PYMOR_THROW(Exception::wrong_parameter_type,
//                     "since parametric() == false mu has to be empty (is " << mu.report() << ")!");
//  range->backend() = BaseType::backend() * source->backend();
//}

//double EigenDenseMatrix::apply2(const RangeType* range,
//                                const SourceType* source,
//                                const Parameter mu) const
//  throw (Exception::types_are_not_compatible,
//         Exception::you_have_to_implement_this,
//         Exception::sizes_do_not_match,
//         Exception::wrong_parameter_type,
//         Exception::requirements_not_met,
//         Exception::linear_solver_failed,
//         Exception::this_does_not_make_any_sense)
//{
//  if (source->dim() != dim_source())
//    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
//                     "size of U (" << source->dim() << ") does not match dim_source() of this (" << dim_source() << ")!");
//  if (range->dim() != dim_range())
//    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
//                     "size of V (" << range->dim() << ") does not match dim_range() of this (" << dim_range() << ")!");
//  if (mu.type() != Parameter().type())
//    DUNE_PYMOR_THROW(Exception::wrong_parameter_type,
//                     "since parametric() == false mu has to be empty (is " << mu.report() << ")!");
//  return range->backend().transpose() * BaseType::backend() * source->backend();
//}

//std::vector< std::string > EigenDenseMatrix::invert_options() throw(Exception::not_invertible)
//{
//  return {
//      "householderQR.columnpivoting"
//    , "LU.fullpivoting"
//    , "householderQR.fullpivoting"
//    , "householderQR"
//    , "cholesky.LDLT"
//    , "cholesky.LLT"
//    , "LU.partialpivoting"
//  };
//}

//const EigenDenseMatrixInverse* EigenDenseMatrix::invert(const std::string type,
//                                                        const Parameter mu) const
//  throw(Exception::not_invertible, Exception::key_is_not_valid)
//{
//  if (!mu.type().empty())
//    DUNE_PYMOR_THROW(Exception::this_is_not_parametric,
//                     "do not call invert with mu = " << mu << " if parametric() == false!");
//  const auto invert_ops = invert_options();
//  const auto result = std::find(invert_ops.begin(), invert_ops.end(), type);
//  if (result == invert_ops.end()) {
//    std::stringstream msg;
//    msg << "type has to be one of {";
//    for (size_t ii = 0; ii < invert_ops.size() - 1; ++ii)
//      msg << "'" << invert_ops[ii] << "'', ";
//    msg << "'" << invert_ops[invert_ops.size()] << "''} (is '" << type << "'')!";
//    DUNE_PYMOR_THROW(Exception::key_is_not_valid, msg.str());
//  }
//  return new EigenDenseMatrixInverse(this, type);
//}

//void EigenDenseMatrix::apply_inverse(const LA::VectorInterface* range,
//                                     LA::VectorInterface* source,
//                                     const std::string /*type*/,
//                                     const Parameter /*mu*/) const
//  throw (Exception::types_are_not_compatible,
//         Exception::you_have_to_implement_this,
//         Exception::sizes_do_not_match,
//         Exception::wrong_parameter_type,
//         Exception::requirements_not_met,
//         Exception::linear_solver_failed,
//         Exception::this_does_not_make_any_sense)
//{
//  std::stringstream msg;
//  size_t throw_up = 0;
//  if (source->type() != type_source()) {
//    msg << "source (" << source->type() << ") is not a compatible type_source (" << type_source() << ")";
//    ++throw_up;
//  }
//  if (range->type() != type_range()) {
//    if (throw_up)
//      msg << " and ";
//    msg << "range (" << range->type() << ") is not a compatible type_range (" << type_range() << ")";
//  }
//  DUNE_PYMOR_THROW(Exception::types_are_not_compatible, msg.str());
//}

//void EigenDenseMatrix::apply_inverse(const RangeType* range,
//                                     SourceType* source,
//                                     const std::string type,
//                                     const Parameter mu) const
//  throw (Exception::types_are_not_compatible,
//         Exception::you_have_to_implement_this,
//         Exception::sizes_do_not_match,
//         Exception::wrong_parameter_type,
//         Exception::requirements_not_met,
//         Exception::linear_solver_failed,
//         Exception::this_does_not_make_any_sense)
//{
//  const EigenDenseMatrixInverse* inverseOp = invert(type, mu);
//  inverseOp->apply(range, source);;
//  delete inverseOp;
//}

//EigenDenseMatrix* EigenDenseMatrix::freeze_parameter(const Parameter /*mu*/) const
//  throw (Exception::this_is_not_parametric)
//{
//  DUNE_PYMOR_THROW(Exception::this_is_not_parametric, "do not call freeze_parameter if parametric() == false!");
//  return nullptr;
//}

//EigenDenseMatrix* EigenDenseMatrix::copy() const
//{
//  return new EigenDenseMatrix(*this);
//}

//void EigenDenseMatrix::scal(const double alpha)
//{
//  backend() *= alpha;
//}

//void EigenDenseMatrix::axpy(const double /*alpha*/, const LinearOperatorInterface* /*x*/)
//  throw (Exception::sizes_do_not_match, Exception::types_are_not_compatible)
//{
//  DUNE_PYMOR_THROW(Exception::types_are_not_compatible,
//                   "not implemented for arbitrary x!");
//}

//void EigenDenseMatrix::axpy(const double alpha, const EigenDenseMatrix* x) throw (Exception::sizes_do_not_match,
//                                                                                  Exception::types_are_not_compatible)
//{
//  if (x->dim_range() != dim_range())
//    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
//                     "the dim_range of x (" << x->dim_range() << ") does not match the dim_range of this ("
//                     << dim_range() << ")!");
//  if (x->dim_source() != dim_source())
//    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
//                     "the dim_source of x (" << x->dim_source() << ") does not match the dim_source of this ("
//                     << dim_source() << ")!");
//  backend() += alpha * x->backend();
//}

//int EigenDenseMatrix::assert_is_positive(const int ii)
//{
//  if (ii <= 0) DUNE_PYMOR_THROW(Exception::index_out_of_range, "ii has to be positive (is " << ii << ")!");
//  return ii;
//}


// ============================================
// ===== EigenRowMajorSparseMatrixInverse =====
// ============================================
template< class S >
EigenRowMajorSparseInverse< S >::EigenRowMajorSparseInverse(const ContainerType* matrix_ptr,
                                                            const std::string option)
  : matrix_(matrix_ptr)
  , option_(option)
{
  const auto invert_ops = InverseType::invert_options();
  const auto result = std::find(invert_ops.begin(), invert_ops.end(), option);
  if (result == invert_ops.end()) {
    std::stringstream msg;
    msg << "option has to be one of {";
    for (size_t ii = 0; ii < invert_ops.size() - 1; ++ii)
      msg << "'" << invert_ops[ii] << "', ";
    msg << "'" << invert_ops[invert_ops.size()] << "'} (is '" << option << "')!";
    DUNE_PYMOR_THROW(Exception::key_is_not_valid, msg.str());
  }
}

template< class S >
EigenRowMajorSparseInverse< S >::EigenRowMajorSparseInverse(const std::shared_ptr< const ContainerType > matrix_ptr,
                                                            const std::string option)
  : matrix_(matrix_ptr)
  , option_(option)
{
  const auto invert_ops = InverseType::invert_options();
  const auto result = std::find(invert_ops.begin(), invert_ops.end(), option);
  if (result == invert_ops.end()) {
    std::stringstream msg;
    msg << "option has to be one of {";
    for (size_t ii = 0; ii < invert_ops.size() - 1; ++ii)
      msg << "'" << invert_ops[ii] << "', ";
    msg << "'" << invert_ops[invert_ops.size()] << "'} (is '" << option << "')!";
    DUNE_PYMOR_THROW(Exception::key_is_not_valid, msg.str());
  }
}

template< class S >
bool EigenRowMajorSparseInverse< S >::linear() const
{
  return true;
}

template< class S >
DUNE_STUFF_SSIZE_T EigenRowMajorSparseInverse< S >::dim_source() const
{
  return matrix_->dim_range();
}

template< class S >
DUNE_STUFF_SSIZE_T EigenRowMajorSparseInverse< S >::dim_range() const
{
  return matrix_->dim_source();
}

template< class S >
void EigenRowMajorSparseInverse< S >::apply(const SourceType& source, RangeType& range, const Parameter mu) const
{
  if (!mu.empty()) DUNE_PYMOR_THROW(Exception::this_is_not_parametric,
                                    "mu has to be empty if parametric() == false (is " << mu << ")!");
  if (source.dim() != dim_source())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "the dim of source (" << source.dim() << ") does not match the dim_source of this ("
                     << dim_source() << ")!");
  if (range.dim() != dim_range())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "the dim of range (" << range.dim() << ") does not match the dim_range of this ("
                     << dim_range() << ")!");
  if (option_ == "cg.diagonal.lower") {
    typedef ::Eigen::ConjugateGradient< typename ContainerType::BackendType::BackendType,
                                        ::Eigen::Lower,
                                        ::Eigen::DiagonalPreconditioner< double > > EigenSolverType;
    EigenSolverType eigenSolver(matrix_->backend_->backend());
    eigenSolver.setMaxIterations(dim_source());
    eigenSolver.setTolerance(1e-15);
    range.backend_->backend() = eigenSolver.solve(source.backend_->backend());
    if (eigenSolver.info() != ::Eigen::Success)
      DUNE_PYMOR_THROW(Exception::linear_solver_failed,
                       "EIGEN solver reported error code '" << eigenSolver.info() << "',\n"
                       "see: http://eigen.tuxfamily.org/dox/group__enums.html#ga51bc1ac16f26ebe51eae1abb77bd037b !");
  } else if (option_ == "cg.diagonal.upper") {
      typedef ::Eigen::ConjugateGradient< typename ContainerType::BackendType::BackendType,
                                          ::Eigen::Upper,
                                          ::Eigen::DiagonalPreconditioner< double > > EigenSolverType;
      EigenSolverType eigenSolver(matrix_->backend_->backend());
      eigenSolver.setMaxIterations(dim_source());
      eigenSolver.setTolerance(1e-15);
      range.backend_->backend() = eigenSolver.solve(source.backend_->backend());
      if (eigenSolver.info() != ::Eigen::Success)
        DUNE_PYMOR_THROW(Exception::linear_solver_failed,
                         "EIGEN solver reported error code '" << eigenSolver.info() << "',\n"
                         "see: http://eigen.tuxfamily.org/dox/group__enums.html#ga51bc1ac16f26ebe51eae1abb77bd037b !");
  } else if (option_ == "bicgstab.identity") {
      typedef ::Eigen::BiCGSTAB<  typename ContainerType::BackendType::BackendType,
                                  ::Eigen::IdentityPreconditioner > EigenSolverType;
      EigenSolverType eigenSolver(matrix_->backend_->backend());
      eigenSolver.setMaxIterations(dim_source());
      eigenSolver.setTolerance(1e-15);
      range.backend_->backend() = eigenSolver.solve(source.backend_->backend());
      if (eigenSolver.info() != ::Eigen::Success)
        DUNE_PYMOR_THROW(Exception::linear_solver_failed,
                         "EIGEN solver reported error code '" << eigenSolver.info() << "',\n"
                         "see: http://eigen.tuxfamily.org/dox/group__enums.html#ga51bc1ac16f26ebe51eae1abb77bd037b !");
  } else if (option_ == "bicgstab.diagonal") {
      typedef ::Eigen::BiCGSTAB<  typename ContainerType::BackendType::BackendType,
                                  ::Eigen::DiagonalPreconditioner< double > > EigenSolverType;
      EigenSolverType eigenSolver(matrix_->backend_->backend());
      eigenSolver.setMaxIterations(dim_source());
      eigenSolver.setTolerance(1e-15);
      range.backend_->backend() = eigenSolver.solve(source.backend_->backend());
      if (eigenSolver.info() != ::Eigen::Success)
        DUNE_PYMOR_THROW(Exception::linear_solver_failed,
                         "EIGEN solver reported error code '" << eigenSolver.info() << "',\n"
                         "see: http://eigen.tuxfamily.org/dox/group__enums.html#ga51bc1ac16f26ebe51eae1abb77bd037b !");
  } else if (option_ == "bicgstab.ilut") {
      typedef ::Eigen::BiCGSTAB<  typename ContainerType::BackendType::BackendType,
                                  ::Eigen::IncompleteLUT< double > > EigenSolverType;
      EigenSolverType eigenSolver(matrix_->backend_->backend());
      eigenSolver.setMaxIterations(dim_source());
      eigenSolver.setTolerance(1e-15);
      eigenSolver.preconditioner().setDroptol(1e-4);
      eigenSolver.preconditioner().setFillfactor(10);
      range.backend_->backend() = eigenSolver.solve(source.backend_->backend());
      if (eigenSolver.info() != ::Eigen::Success)
        DUNE_PYMOR_THROW(Exception::linear_solver_failed,
                         "EIGEN solver reported error code '" << eigenSolver.info() << "',\n"
                         "see: http://eigen.tuxfamily.org/dox/group__enums.html#ga51bc1ac16f26ebe51eae1abb77bd037b !");
#if HAVE_SUPERLU
  } else if (option_ == "superlu") {
      typedef ::Eigen::SuperLU< typename ContainerType::BackendType::BackendType > EigenSolverType;
      EigenSolverType eigenSolver;
      eigenSolver.compute(matrix_->backend_->backend());
      range.backend_->backend() = eigenSolver.solve(source.backend_->backend());
      if (eigenSolver.info() != ::Eigen::Success)
        DUNE_PYMOR_THROW(Exception::linear_solver_failed,
                         "EIGEN solver reported error code '" << eigenSolver.info() << "',\n"
                         "see: http://eigen.tuxfamily.org/dox/group__enums.html#ga51bc1ac16f26ebe51eae1abb77bd037b !");
#endif // HAVE_SUPERLU
  } else
    DUNE_PYMOR_THROW(Exception::this_does_not_make_any_sense, "I checked this in the constructor!");
}

template< class S >
std::vector< std::string > EigenRowMajorSparseInverse< S >::invert_options()
{
  return { "exact" };
}

template< class S >
typename EigenRowMajorSparseInverse< S >::InverseType EigenRowMajorSparseInverse< S >::invert(const std::string option,
                                                                                              const Parameter mu) const
{
  if (!mu.empty()) DUNE_PYMOR_THROW(Exception::this_is_not_parametric,
                                    "mu has to be empty if parametric() == false (is " << mu << ")!");
  if (option != invert_options()[0])
    DUNE_PYMOR_THROW(Exception::wrong_option_given,
                     "option has to be '" << invert_options()[0] << "' (is " << option << ")!");
  return InverseType(matrix_);
}

template< class S >
typename EigenRowMajorSparseInverse< S >::FrozenType EigenRowMajorSparseInverse< S >::freeze_parameter(const Parameter mu) const
{
  if (!mu.empty())
    DUNE_PYMOR_THROW(Exception::this_is_not_parametric,
                     "do not call freeze_parameter(" << mu << ") if parametric() == false!");
  return FrozenType(nullptr, invert_options()[0]);
}

template class EigenRowMajorSparseInverse< double >;


// =====================================
// ===== EigenRowMajorSparseMatrix =====
// =====================================
template< class S >
EigenRowMajorSparse< S >::EigenRowMajorSparse(const ContainerType* matrix_ptr)
  : matrix_(matrix_ptr)
{}

template< class S >
EigenRowMajorSparse< S >::EigenRowMajorSparse(const std::shared_ptr< const ContainerType > matrix_ptr)
  : matrix_(matrix_ptr)
{}

template< class S >
bool EigenRowMajorSparse< S >::linear() const
{
  return true;
}

template< class S >
DUNE_STUFF_SSIZE_T EigenRowMajorSparse< S >::dim_source() const
{
  return matrix_->dim_source();
}

template< class S >
DUNE_STUFF_SSIZE_T EigenRowMajorSparse< S >::dim_range() const
{
  return matrix_->dim_range();
}

template< class S >
void EigenRowMajorSparse< S >::apply(const SourceType& source, RangeType& range, const Parameter mu) const
{
  if (!mu.empty()) DUNE_PYMOR_THROW(Exception::this_is_not_parametric,
                                    "mu has to be empty if parametric() == false (is " << mu << ")!");
  if (source.dim() != dim_source())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "the dim of source (" << source.dim() << ") does not match the dim_source of this ("
                     << dim_source() << ")!");
  if (range.dim() != dim_range())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "the dim of range (" << range.dim() << ") does not match the dim_range of this ("
                     << dim_range() << ")!");
  range.backend_->backend() = matrix_->backend_->backend() * source.backend_->backend();
}

template< class S >
std::vector< std::string > EigenRowMajorSparse< S >::invert_options()
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

template< class S >
typename EigenRowMajorSparse< S >::InverseType EigenRowMajorSparse< S >::invert(const std::string option,
                                                                                const Parameter mu) const
{
  if (!mu.empty()) DUNE_PYMOR_THROW(Exception::this_is_not_parametric,
                                    "mu has to be empty if parametric() == false (is " << mu << ")!");
  const auto invert_ops = invert_options();
  const auto result = std::find(invert_ops.begin(), invert_ops.end(), option);
  if (result == invert_ops.end()) {
    std::stringstream msg;
    msg << "option has to be one of {";
    for (size_t ii = 0; ii < invert_ops.size() - 1; ++ii)
      msg << "'" << invert_ops[ii] << "', ";
    msg << "'" << invert_ops[invert_ops.size()] << "'} (is '" << option << "')!";
    DUNE_PYMOR_THROW(Exception::key_is_not_valid, msg.str());
  }
  return InverseType(matrix_, option);
}

template< class S >
typename EigenRowMajorSparse< S >::FrozenType EigenRowMajorSparse< S >::freeze_parameter(const Parameter mu) const
{
  if (!mu.empty())
    DUNE_PYMOR_THROW(Exception::this_is_not_parametric,
                     "do not call freeze_parameter(" << mu << ") if parametric() == false!");
  return FrozenType(nullptr);
}

template< class S >
std::shared_ptr< const typename EigenRowMajorSparse< S >::ContainerType > EigenRowMajorSparse< S >::container() const
{
  return matrix_;
}

template class EigenRowMajorSparse< double >;
template class LinearAffinelyDecomposedContainerBased< EigenRowMajorSparse< double > >;


} // namespace Operators
} // namespace Pymor
} // namespace Dune

#endif // HAVE_EIGEN
