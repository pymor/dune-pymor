// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_OPERATORS_EIGEN_HH
#define DUNE_PYMOR_OPERATORS_EIGEN_HH

#if HAVE_EIGEN
#include <dune/stuff/la/container/eigen.hh>

#include <dune/pymor/la/container/eigen.hh>
#include <dune/pymor/parameters/base.hh>

#include "interfaces.hh"

namespace Dune {
namespace Pymor {
namespace Operators {


class EigenDenseMatrix
  : public Dune::Stuff::LA::EigenDenseMatrix< double >
  , public Dune::Pymor::OperatorInterface
{
  typedef Dune::Pymor::OperatorInterface              InterfaceType;
public:
  typedef Dune::Stuff::LA::EigenDenseMatrix< double > BaseType;
  typedef EigenDenseMatrix                            ThisType;
  typedef Dune::Pymor::LA::EigenDenseVector           SourceType;
  typedef Dune::Pymor::LA::EigenDenseVector           RangeType;

  EigenDenseMatrix()
    : BaseType()
    , InterfaceType()
  {}

  EigenDenseMatrix(const BaseType& other)
    : BaseType(other)
    , InterfaceType()
  {}

  EigenDenseMatrix(const int rr, const int cc)
    : BaseType(assert_is_positive(rr), assert_is_positive(cc))
    , InterfaceType()
  {}

  virtual bool linear() const
  {
    return true;
  }

  virtual unsigned int dim_source() const
  {
    return BaseType::cols();
  }

  virtual unsigned int dim_range() const
  {
    return BaseType::rows();
  }

  virtual std::string type_source() const
  {
    return SourceType::static_type();
  }

  virtual std::string type_range() const
  {
    return RangeType::static_type();
  }

  virtual void apply(const LA::VectorInterface* source,
                     LA::VectorInterface* range,
                     const Parameter /*mu*/ = Parameter()) const throw (Exception::types_are_not_compatible,
                                                                        Exception::you_have_to_implement_this,
                                                                        Exception::sizes_do_not_match,
                                                                        Exception::wrong_parameter_type,
                                                                        Exception::requirements_not_met)
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

  virtual void apply(const SourceType* source, RangeType* range, const Parameter mu = Parameter()) const
    throw (Exception::types_are_not_compatible,
           Exception::you_have_to_implement_this,
           Exception::sizes_do_not_match,
           Exception::wrong_parameter_type)
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

  virtual double apply2(const RangeType* range,
                        const SourceType* source,
                        const Parameter mu = Parameter()) const
    throw (Exception::types_are_not_compatible,
           Exception::you_have_to_implement_this,
           Exception::sizes_do_not_match,
           Exception::wrong_parameter_type)
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

  virtual ThisType* freeze_parameter(const Parameter /*mu*/ = Parameter()) const
    throw (Exception::this_is_not_parametric)
  {
    DUNE_PYMOR_THROW(Exception::this_is_not_parametric, "do not call freeze_parameter if parametric() == false!");
    return nullptr;
  }

private:
  static int assert_is_positive(const int ii)
  {
    if (ii <= 0) DUNE_PYMOR_THROW(Exception::index_out_of_range, "ii has to be positive (is " << ii << ")!");
    return ii;
  }
}; // class EigenDenseMatrix


class EigenRowMajorSparseMatrix
  : public Dune::Stuff::LA::EigenRowMajorSparseMatrix< double >
  , public Dune::Pymor::OperatorInterface
{
  typedef Dune::Stuff::LA::EigenRowMajorSparseMatrix< double >  BaseType;
  typedef Dune::Pymor::OperatorInterface                        InterfaceType;
public:
  typedef EigenRowMajorSparseMatrix                             ThisType;
  typedef Dune::Pymor::LA::EigenDenseVector                     SourceType;
  typedef Dune::Pymor::LA::EigenDenseVector                     RangeType;

  EigenRowMajorSparseMatrix()
    : BaseType()
    , InterfaceType()
  {}

  EigenRowMajorSparseMatrix(const BaseType& other)
    : BaseType(other)
    , InterfaceType()
  {}

  EigenRowMajorSparseMatrix(const int rr, const int cc, const Dune::Stuff::LA::SparsityPatternDefault& pattern)
    : BaseType(assert_is_positive(rr), assert_is_positive(cc), pattern)
    , InterfaceType()
  {}

  virtual bool linear() const
  {
    return true;
  }

  virtual unsigned int dim_source() const
  {
    return BaseType::cols();
  }

  virtual unsigned int dim_range() const
  {
    return BaseType::rows();
  }

  virtual std::string type_source() const
  {
    return SourceType::static_type();
  }

  virtual std::string type_range() const
  {
    return RangeType::static_type();
  }

  virtual void apply(const LA::VectorInterface* source,
                     LA::VectorInterface* range,
                     const Parameter /*mu*/ = Parameter()) const throw (Exception::types_are_not_compatible,
                                                                        Exception::you_have_to_implement_this,
                                                                        Exception::sizes_do_not_match,
                                                                        Exception::wrong_parameter_type,
                                                                        Exception::requirements_not_met)
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

  virtual void apply(const SourceType* source, RangeType* range, const Parameter mu = Parameter()) const
    throw (Exception::types_are_not_compatible,
           Exception::you_have_to_implement_this,
           Exception::sizes_do_not_match,
           Exception::wrong_parameter_type,
           Exception::requirements_not_met)
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

  virtual double apply2(const SourceType* range, const RangeType* source, const Parameter /*mu*/ = Parameter()) const
    throw (Exception::types_are_not_compatible,
           Exception::sizes_do_not_match,
           Exception::wrong_parameter_type)
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

  virtual ThisType* freeze_parameter(const Parameter /*mu*/ = Parameter()) const
    throw (Exception::this_is_not_parametric)
  {
    DUNE_PYMOR_THROW(Exception::this_is_not_parametric, "do not call freeze_parameter if parametric() == false!");
    return nullptr;
  }

private:
  static int assert_is_positive(const int ii)
  {
    if (ii <= 0) DUNE_PYMOR_THROW(Exception::index_out_of_range, "ii has to be positive (is " << ii << ")!");
    return ii;
  }
}; // class EigenRowMajorSparseMatrix


} // namespace Operator
} // namespace Pymor
} // namespace Dune

#endif // HAVE_EIGEN
#endif // DUNE_PYMOR_OPERATORS_EIGEN_HH
