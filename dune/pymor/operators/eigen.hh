// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_OPERATORS_EIGEN_HH
#define DUNE_PYMOR_OPERATORS_EIGEN_HH

//#if HAVE_EIGEN
#include <dune/stuff/la/container/eigen.hh>

#include <dune/pymor/la/container/eigen.hh>
#include <dune/pymor/parameters/base.hh>

#include "interfaces.hh"

namespace Dune {
namespace Pymor {
namespace Operators {


// forward, needed for EigenDenseMatrixInverse
class EigenDenseMatrix;


class EigenDenseMatrixInverse
  : public OperatorInterface
{
public:
  typedef Dune::Pymor::LA::EigenDenseVector           SourceType;
  typedef Dune::Pymor::LA::EigenDenseVector           RangeType;

  EigenDenseMatrixInverse(const EigenDenseMatrix* op, const std::string type) throw (Exception::key_is_not_valid);

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

  static std::vector< std::string > invert_options() throw(Exception::not_invertible);

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

  virtual EigenDenseMatrixInverse* freeze_parameter(const Parameter /*mu*/ = Parameter()) const
    throw (Exception::this_is_not_parametric);

private:
  const EigenDenseMatrix* op_;
  const std::string type_;
}; // class EigenDenseMatrixInverse


class EigenDenseMatrix
  : public Dune::Stuff::LA::EigenDenseMatrix< double >
  , public LinearOperatorInterface
{
public:
  typedef Dune::Stuff::LA::EigenDenseMatrix< double > BaseType;
  typedef Dune::Pymor::LA::EigenDenseVector           SourceType;
  typedef Dune::Pymor::LA::EigenDenseVector           RangeType;

  EigenDenseMatrix();

  EigenDenseMatrix(const BaseType& other);

  EigenDenseMatrix(const int rr, const int cc);

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

  virtual const EigenDenseMatrixInverse* invert(const std::string type = invert_options()[0],
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

  virtual EigenDenseMatrix* freeze_parameter(const Parameter /*mu*/ = Parameter()) const
    throw (Exception::this_is_not_parametric);

  virtual EigenDenseMatrix* copy() const;

  virtual void scal(const double alpha);

  virtual void axpy(const double /*alpha*/, const LinearOperatorInterface* /*x*/)
    throw (Exception::sizes_do_not_match, Exception::types_are_not_compatible);

  virtual void axpy(const double alpha, const EigenDenseMatrix* x)
    throw (Exception::sizes_do_not_match, Exception::types_are_not_compatible);

private:
  static int assert_is_positive(const int ii);
}; // class EigenDenseMatrix



// forward, needed in EigenRowMajorSparseMatrixInverse
class EigenRowMajorSparseMatrix;


class EigenRowMajorSparseMatrixInverse
  : public OperatorInterface
{
public:
  typedef Dune::Pymor::LA::EigenDenseVector SourceType;
  typedef Dune::Pymor::LA::EigenDenseVector RangeType;

  EigenRowMajorSparseMatrixInverse(const EigenRowMajorSparseMatrix* op, const std::string type);

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

  static std::vector< std::string > invert_options();

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

  virtual EigenRowMajorSparseMatrixInverse* freeze_parameter(const Parameter /*mu*/ = Parameter()) const
    throw (Exception::this_is_not_parametric);

private:
  const EigenRowMajorSparseMatrix* op_;
  const std::string type_;
}; // class EigenRowMajorSparseMatrixInverse


class EigenRowMajorSparseMatrix
  : public Dune::Stuff::LA::EigenRowMajorSparseMatrix< double >
  , public LinearOperatorInterface
{
  typedef Dune::Stuff::LA::EigenRowMajorSparseMatrix< double >  BaseType;
public:
  typedef Dune::Pymor::LA::EigenDenseVector                     SourceType;
  typedef Dune::Pymor::LA::EigenDenseVector                     RangeType;

  EigenRowMajorSparseMatrix();

  EigenRowMajorSparseMatrix(const BaseType& other);

  EigenRowMajorSparseMatrix(const int rr, const int cc, const Dune::Stuff::LA::SparsityPatternDefault& pattern);

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

  virtual double apply2(const SourceType* range, const RangeType* source, const Parameter /*mu*/ = Parameter()) const
    throw (Exception::types_are_not_compatible,
           Exception::you_have_to_implement_this,
           Exception::sizes_do_not_match,
           Exception::wrong_parameter_type,
           Exception::requirements_not_met,
           Exception::linear_solver_failed);

  static std::vector< std::string > invert_options();

  virtual const EigenRowMajorSparseMatrixInverse* invert(const std::string type = invert_options()[0],
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

  virtual EigenRowMajorSparseMatrix* freeze_parameter(const Parameter /*mu*/ = Parameter()) const
    throw (Exception::this_is_not_parametric);

  virtual EigenRowMajorSparseMatrix* copy() const;

  virtual void scal(const double alpha);

  virtual void axpy(const double /*alpha*/, const LinearOperatorInterface* /*x*/)
    throw (Exception::sizes_do_not_match, Exception::types_are_not_compatible);

  virtual void axpy(const double alpha, const EigenRowMajorSparseMatrix* x)
    throw (Exception::sizes_do_not_match, Exception::types_are_not_compatible);

private:
  static int assert_is_positive(const int ii);
}; // class EigenRowMajorSparseMatrix


} // namespace Operators
} // namespace Pymor
} // namespace Dune

//#endif // HAVE_EIGEN
#endif // DUNE_PYMOR_OPERATORS_EIGEN_HH
