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
#include "affine.hh"

namespace Dune {
namespace Pymor {
namespace Operators {


class EigenDenseMatrix
  : public Dune::Stuff::LA::EigenDenseMatrix< double >
  , public Dune::Pymor::OperatorInterface
{
  typedef Dune::Stuff::LA::EigenDenseMatrix< double > BaseType;
  typedef Dune::Pymor::OperatorInterface              InterfaceType;
public:
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

  virtual RangeType* apply(const SourceType* U, const Parameter mu = Parameter()) const
    throw (Exception::types_are_not_compatible,
           Exception::you_have_to_implement_this,
           Exception::sizes_do_not_match,
           Exception::wrong_parameter_type)
  {
    if (U->dim() != dim_source())
      DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                       "size of U (" << U->dim() << ") does not match dim_source() of this (" << dim_source() << ")!");
    if (mu.type() != Parameter().type())
      DUNE_PYMOR_THROW(Exception::wrong_parameter_type,
                       "since parametric() == false mu has to be empty (is " << mu.report() << ")!");
    RangeType* ret = new RangeType(dim_range());
    ret->backend() = BaseType::backend() * U->backend();
    return ret;
  }

  virtual double apply2(const RangeType* V,
                        const SourceType* U,
                        const Parameter mu = Parameter()) const
    throw (Exception::types_are_not_compatible,
           Exception::you_have_to_implement_this,
           Exception::sizes_do_not_match,
           Exception::wrong_parameter_type)
  {
    if (U->dim() != dim_source())
      DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                       "size of U (" << U->dim() << ") does not match dim_source() of this (" << dim_source() << ")!");
    if (V->dim() != dim_range())
      DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                       "size of V (" << V->dim() << ") does not match dim_range() of this (" << dim_range() << ")!");
    if (mu.type() != Parameter().type())
      DUNE_PYMOR_THROW(Exception::wrong_parameter_type,
                       "since parametric() == false mu has to be empty (is " << mu.report() << ")!");
    return V->backend().transpose() * BaseType::backend() * U->backend();
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

  virtual double apply2(const SourceType* range, const RangeType* source, const Parameter mu = Parameter()) const
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


class AffinelyDecomposedEigenRowMajorSparseMatrix
  : public Operators::AffinelyDecomposed
{
public:
  AffinelyDecomposedEigenRowMajorSparseMatrix(const ParameterType& tt = ParameterType())
    : Operators::AffinelyDecomposed(tt)
    , size_(0)
    , hasAffinePart_(false)
    , dim_source_(0)
    , dim_range_(0)
  {}

  AffinelyDecomposedEigenRowMajorSparseMatrix(EigenRowMajorSparseMatrix* aff, const ParameterType& tt = ParameterType())
    throw (Exception::requirements_not_met)
    : Operators::AffinelyDecomposed(tt)
    , size_(0)
    , hasAffinePart_(true)
    , dim_source_(aff->dim_source())
    , dim_range_(aff->dim_range())
    , affinePart_(aff)
  {
    if (affinePart_->parametric())
      DUNE_PYMOR_THROW(Exception::requirements_not_met,
                       "the affinePart must not be parametric!");
  }

  AffinelyDecomposedEigenRowMajorSparseMatrix(OperatorInterface* /*aff*/, const ParameterType& /*tt*/ = ParameterType())
    throw (Exception::types_are_not_compatible)
  {
    DUNE_PYMOR_THROW(Exception::this_does_not_make_any_sense,
                     "not implemented for arbitrary implementations of OperatorInterface");
  }


  virtual ~AffinelyDecomposedEigenRowMajorSparseMatrix()
  {
    if (hasAffinePart_)
      delete affinePart_;
    for (auto& element : components_)
      delete element;
    for (auto& element : coefficients_)
      delete element;
  }

  virtual void register_component(OperatorInterface* /*aff*/) throw (Exception::this_does_not_make_any_sense,
                                                                     Exception::sizes_do_not_match,
                                                                     Exception::types_are_not_compatible)
  {
    DUNE_PYMOR_THROW(Exception::this_does_not_make_any_sense,
                     "not implemented for arbitrary implementations of OperatorInterface");
  }

  virtual void register_component(EigenRowMajorSparseMatrix* aff) throw (Exception::this_does_not_make_any_sense,
                                                                         Exception::sizes_do_not_match,
                                                                         Exception::types_are_not_compatible)
  {
    if (hasAffinePart_)
      DUNE_PYMOR_THROW(Exception::this_does_not_make_any_sense,
                       "do not call register_component(affinePart) if hasAffinePart() == true!");
    if (aff->parametric())
      DUNE_PYMOR_THROW(Exception::this_does_not_make_any_sense,
                       "the affinePart must not be parametric!");
    if (size_ == 0) {
      dim_source_ = aff->dim_source();
      dim_range_ = aff->dim_range();
    } else {
      if (aff->dim_source() != dim_source_)
        DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                         "the dim_source of aff (" << aff->dim_source() << ") does not match the dim_source of this ("
                         << dim_source_ << ")!");
      if (aff->dim_range() != dim_range_)
        DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                         "the dim_range of aff (" << aff->dim_range() << ") does not match the dim_range of this ("
                         << dim_range_ << ")!");
    }
    affinePart_ = aff;
    hasAffinePart_ = true;
  } // ... register_component(...)

  virtual void register_component(OperatorInterface* comp, const ParameterFunctional* coeff)
    throw(Exception::sizes_do_not_match,
          Exception::types_are_not_compatible,
          Exception::wrong_parameter_type,
          Exception::this_does_not_make_any_sense)
  {
    DUNE_PYMOR_THROW(Exception::this_does_not_make_any_sense,
                     "not implemented for arbitrary implementations of OperatorInterface");
  }

  virtual void register_component(EigenRowMajorSparseMatrix* comp, const ParameterFunctional* coeff)
    throw(Exception::sizes_do_not_match,
          Exception::types_are_not_compatible,
          Exception::wrong_parameter_type,
          Exception::this_does_not_make_any_sense)
  {
    if (comp->parametric())
      DUNE_PYMOR_THROW(Exception::this_does_not_make_any_sense,
                       "a component must not be parametric!");
    if (size_ == 0 && !hasAffinePart_) {
      dim_source_ = comp->dim_source();
      dim_range_ = comp->dim_range();
    } else {
      if (comp->dim_source() != dim_source_)
        DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                         "the dim_source of comp (" << comp->dim_source() << ") does not match the dim_source of this ("
                         << dim_source_ << ")!");
      if (comp->dim_range() != dim_range_)
        DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                         "the dim_range of comp (" << comp->dim_range() << ") does not match the dim_range of this ("
                         << dim_range_ << ")!");
      if (coeff->parameter_type() != Parametric::parameter_type())
        DUNE_PYMOR_THROW(Exception::wrong_parameter_type,
                         "a different parameter types for coeff (" << coeff->parameter_type() << ") and this ("
                         << Parametric::parameter_type() << ") is not yet supported!");
      components_.push_back(comp);
      coefficients_.push_back(coeff);
      ++size_;
    }
  } // ... register_component(..., ...)

  unsigned int size() const
  {
    return size_;
  }

  /**
   * \attention The ownership of the component remains with this class!
   */
  virtual EigenRowMajorSparseMatrix* component(const int ii) throw (Exception::requirements_not_met,
                                                                    Exception::index_out_of_range)
  {
    if (size() == 0)
      DUNE_PYMOR_THROW(Exception::requirements_not_met,
                       "do not call component(ii) if size() == 0!");
    if (ii < 0 || ii >= size())
      DUNE_PYMOR_THROW(Exception::index_out_of_range,
                       "the condition 0 < ii < size() is not fulfilled for ii = " << ii << "and size() = "
                       << size() << "!");
    return components_[ii];
  }

  /**
   * \attention The ownership of the component remains with this class!
   */
  virtual const EigenRowMajorSparseMatrix* component(const int ii) const throw (Exception::requirements_not_met,
                                                                                Exception::index_out_of_range)
  {
    if (size() == 0)
      DUNE_PYMOR_THROW(Exception::requirements_not_met,
                       "do not call component(ii) if size() == 0!");
    if (ii < 0 || ii >= size())
      DUNE_PYMOR_THROW(Exception::index_out_of_range,
                       "the condition 0 < ii < size() is not fulfilled for ii = " << ii << "and size() = "
                       << size() << "!");
    return components_[ii];
  }

  /**
   * \attention The ownership of the coefficient remains with this class!
   */
  virtual const ParameterFunctional* coefficient(const int ii) const throw (Exception::requirements_not_met,
                                                                            Exception::index_out_of_range)
  {
    if (size() == 0)
      DUNE_PYMOR_THROW(Exception::requirements_not_met,
                       "do not call coefficient(ii) if size() == 0!");
    if (ii < 0 || ii >= size())
      DUNE_PYMOR_THROW(Exception::index_out_of_range,
                       "the condition 0 < ii < size() is not fulfilled for ii = " << ii << "and size() = "
                       << size() << "!");
    return coefficients_[ii];

  }

  virtual bool hasAffinePart() const
  {
    return hasAffinePart_;
  }

  /**
   * \attention The ownership of the affinePart remains with this class!
   */
  virtual EigenRowMajorSparseMatrix* affinePart() throw(Exception::requirements_not_met)
  {
    if (!hasAffinePart())
      DUNE_PYMOR_THROW(Exception::requirements_not_met,
                       "do not call affinePart() if hasAffinePart() == false!");
    return affinePart_;
  }

  /**
   * \attention The ownership of affinePart() remains in this class!
   */
  const EigenRowMajorSparseMatrix* affinePart() const throw(Exception::requirements_not_met)
  {
    if (!hasAffinePart())
      DUNE_PYMOR_THROW(Exception::requirements_not_met,
                       "do not call affinePart() if hasAffinePart() == false!");
    return affinePart_;
  }

  virtual bool linear() const
  {
    return true;
  }

  virtual unsigned int dim_source() const
  {
    return dim_source_;
  }

  virtual unsigned int dim_range() const
  {
    return dim_range_;
  }

  virtual std::string type_source() const
  {
    return LA::EigenDenseVector::static_type();
  }

  virtual std::string type_range() const
  {
    return LA::EigenDenseVector::static_type();
  }

  virtual void apply(const LA::VectorInterface* source,
                     LA::VectorInterface* range,
                     const Parameter /*mu*/ = Parameter()) const
    throw (Exception::types_are_not_compatible,
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
    DUNE_PYMOR_THROW(Exception::types_are_not_compatible, msg.str() << "!");
  }

  virtual void apply(const LA::EigenDenseVector* source,
                     LA::EigenDenseVector* range,
                     const Parameter mu = Parameter()) const
    throw (Exception::types_are_not_compatible,
           Exception::you_have_to_implement_this,
           Exception::sizes_do_not_match,
           Exception::wrong_parameter_type,
           Exception::requirements_not_met)
  {
    if (source->dim() != dim_source())
      DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                       "the dim of source (" << source->dim() << ") does not match the dim of this ("
                       << dim_source() << ")!");
    if (range->dim() != dim_range())
      DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                       "the dim of range (" << range->dim() << ") does not match the dim of this ("
                       << dim_range() << ")!");
    if (mu.type() != Parametric::parameter_type())
      DUNE_PYMOR_THROW(Exception::wrong_parameter_type,
                       "the type of mu (" << mu.type() << "does not match the parameter_type of this ("
                       << Parametric::parameter_type() << ")!");
    if (size() == 0 && !hasAffinePart_)
      DUNE_PYMOR_THROW(Exception::requirements_not_met,
                 "do not call apply() if size() == 0 and hasAffinePart() == false!");
    assert(false && "implement");
//    if (hasAffinePart_) {
//      affinePart_->apply(source, range);
//      if (size_ > 0) {
//        LA::VectorInterface* tmp = LA::createVector(range->type(), dim_range());
//        assert(components_.size() == size_ && "This should not happen!");
//        assert(coefficients_.size() == size_ && "This should not happen!");
//        for (size_t ii = 0; ii < size_; ++ii) {
//          components_[ii]->apply(source, tmp);
//          tmp->scal(coefficients_[ii]->evaluate(mu));
//          range->iadd(tmp);
//        }
//      }
//    } else {
//      assert(components_.size() == size_ && "This should not happen!");
//      assert(coefficients_.size() == size_ && "This should not happen!");
//      components_[0]->apply(source, range);
//      range->scal(coefficients_[0]->evaluate(mu));
//      if (size_ > 1) {
//        LA::VectorInterface* tmp = LA::createVector(range->type(), dim_range());
//        for (size_t ii = 1; ii < size_; ++ii) {
//          components_[ii]->apply(source, tmp);
//          tmp->scal(coefficients_[ii]->evaluate(mu));
//          range->iadd(tmp);
//        }
//      }
//    }
  } // void apply(...)

private:
  unsigned int size_;
  bool hasAffinePart_;
  unsigned int dim_source_;
  unsigned int dim_range_;
  std::vector< EigenRowMajorSparseMatrix* > components_;
  std::vector< const ParameterFunctional* > coefficients_;
  EigenRowMajorSparseMatrix* affinePart_;
}; // class AffinelyDecomposedEigenRowMajorSparseMatrix


} // namespace Operator
} // namespace Pymor
} // namespace Dune

//#endif // HAVE_EIGEN

#endif // DUNE_PYMOR_OPERATORS_EIGEN_HH
