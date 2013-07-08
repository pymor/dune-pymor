// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_OPERATORS_AFFINE_HH
#define DUNE_PYMOR_OPERATORS_AFFINE_HH

#include <dune/pymor/common/exceptions.hh>
#include <dune/pymor/parameters/base.hh>
#include <dune/pymor/parameters/functional.hh>
#include "interfaces.hh"

namespace Dune {
namespace Pymor {
namespace Operators {


class AffinelyDecomposed
  : public OperatorInterface
{
public:
  AffinelyDecomposed(const ParameterType& tt = ParameterType())
    : OperatorInterface(tt)
    , linear_(true)
    , size_(0)
    , hasAffinePart_(false)
    , dim_source_(0)
    , dim_range_(0)
    , type_source_("")
    , type_range_("")
  {}

  AffinelyDecomposed(OperatorInterface* aff, const ParameterType& tt = ParameterType())
    throw (Exception::requirements_not_met)
    : OperatorInterface(tt)
    , linear_(aff->linear())
    , size_(0)
    , hasAffinePart_(true)
    , dim_source_(aff->dim_source())
    , dim_range_(aff->dim_range())
    , type_source_(aff->type_source())
    , type_range_(aff->type_range())
    , affinePart_(aff)
  {
    if (affinePart_->parametric())
      DUNE_PYMOR_THROW(Exception::requirements_not_met,
                       "the affinePart must not be parametric!");
  }

  virtual ~AffinelyDecomposed()
  {
    if (hasAffinePart_)
      delete affinePart_;
    for (auto& element : components_)
      delete element;
    for (auto& element : coefficients_)
      delete element;
  }

  /**
   * \attention This class takes ownership of aff!
   */
  virtual void register_component(OperatorInterface* aff) throw (Exception::this_does_not_make_any_sense,
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
      type_source_ = aff->type_source();
      type_range_ = aff->type_range();
      linear_ = aff->linear();
    } else {
      if (aff->dim_source() != dim_source_)
        DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                         "the dim_source of aff (" << aff->dim_source() << ") does not match the dim_source of this ("
                         << dim_source_ << ")!");
      if (aff->dim_range() != dim_range_)
        DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                         "the dim_range of aff (" << aff->dim_range() << ") does not match the dim_range of this ("
                         << dim_range_ << ")!");
      if (aff->type_source() != type_source_)
        DUNE_PYMOR_THROW(Exception::types_are_not_compatible,
                         "the type_source of aff (" << aff->type_source() << ") does not match the type_source of this ("
                         << type_source_ << ")!");
      if (aff->type_range() != type_range_)
        DUNE_PYMOR_THROW(Exception::types_are_not_compatible,
                         "the type_range of aff (" << aff->type_range() << ") does not match the type_range of this ("
                         << type_range_ << ")!");
      if (!aff->linear() != linear_)
        linear_ = false;
    }
    affinePart_ = aff;
    hasAffinePart_ = true;
  } // ... register_component(...)

  /**
   * \attention This class takes ownership of comp and coeff!
   */
  virtual void register_component(OperatorInterface* comp, const ParameterFunctional* coeff)
  {
    if (comp->parametric())
      DUNE_PYMOR_THROW(Exception::this_does_not_make_any_sense,
                       "a component must not be parametric!");
    if (size_ == 0 && !hasAffinePart_) {
      dim_source_ = comp->dim_source();
      dim_range_ = comp->dim_range();
      type_source_ = comp->type_source();
      type_range_ = comp->type_range();
      linear_ = comp->linear();
    } else {
    if (comp->dim_source() != dim_source_)
      DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                       "the dim_source of comp (" << comp->dim_source() << ") does not match the dim_source of this ("
                       << dim_source_ << ")!");
    if (comp->dim_range() != dim_range_)
      DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                       "the dim_range of comp (" << comp->dim_range() << ") does not match the dim_range of this ("
                       << dim_range_ << ")!");
    if (comp->type_source() != type_source_)
      DUNE_PYMOR_THROW(Exception::types_are_not_compatible,
                       "the type_source of comp (" << comp->type_source() << ") does not match the type_source of this ("
                       << type_source_ << ")!");
    if (comp->type_range() != type_range_)
      DUNE_PYMOR_THROW(Exception::types_are_not_compatible,
                       "the type_range of comp (" << comp->type_range() << ") does not match the type_range of this ("
                       << type_range_ << ")!");
    }
    if (!comp->linear() != linear_)
      linear_ = false;
    if (coeff->parameter_type() != Parametric::parameter_type())
      DUNE_PYMOR_THROW(Exception::wrong_parameter_type,
                       "different parameter types for coeff (" << coeff->parameter_type() << ") and this ("
                       << Parametric::parameter_type() << ") is not yet supported!");
    components_.push_back(comp);
    coefficients_.push_back(coeff);
    ++size_;
  } // ... register_component(..., ...)

  virtual unsigned int size() const
  {
    return size_;
  }

  /**
   * \attention The ownership of the component remains with this class!
   */
  virtual OperatorInterface* component(const int ii) throw (Exception::requirements_not_met,
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
  virtual const OperatorInterface* component(const int ii) const throw (Exception::requirements_not_met,
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
  virtual OperatorInterface* affinePart() throw(Exception::requirements_not_met)
  {
    if (!hasAffinePart())
      DUNE_PYMOR_THROW(Exception::requirements_not_met,
                       "do not call affinePart() if hasAffinePart() == false!");
    return affinePart_;
  }

  /**
   * \attention The ownership of affinePart() remains in this class!
   */
  virtual const OperatorInterface* affinePart() const throw(Exception::requirements_not_met)
  {
    if (!hasAffinePart())
      DUNE_PYMOR_THROW(Exception::requirements_not_met,
                       "do not call affinePart() if hasAffinePart() == false!");
    return affinePart_;
  }

  virtual bool linear() const
  {
    return linear_;
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
    return type_source_;
  }

  virtual std::string type_range() const
  {
    return type_range_;
  }

  virtual void apply(const LA::VectorInterface* source,
                     LA::VectorInterface* range,
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
    if (hasAffinePart_) {
      affinePart_->apply(source, range);
      if (size_ > 0) {
        LA::VectorInterface* tmp = LA::createVector(range->type(), dim_range());
        assert(components_.size() == size_ && "This should not happen!");
        assert(coefficients_.size() == size_ && "This should not happen!");
        for (size_t ii = 0; ii < size_; ++ii) {
          components_[ii]->apply(source, tmp);
          tmp->scal(coefficients_[ii]->evaluate(mu));
          range->iadd(tmp);
        }
      }
    } else {
      assert(components_.size() == size_ && "This should not happen!");
      assert(coefficients_.size() == size_ && "This should not happen!");
      components_[0]->apply(source, range);
      range->scal(coefficients_[0]->evaluate(mu));
      if (size_ > 1) {
        LA::VectorInterface* tmp = LA::createVector(range->type(), dim_range());
        for (size_t ii = 1; ii < size_; ++ii) {
          components_[ii]->apply(source, tmp);
          tmp->scal(coefficients_[ii]->evaluate(mu));
          range->iadd(tmp);
        }
      }
    }
  } // void apply(...)

private:
  bool linear_;
  unsigned int size_;
  bool hasAffinePart_;
  unsigned int dim_source_;
  unsigned int dim_range_;
  std::string type_source_;
  std::string type_range_;
  std::vector< OperatorInterface* > components_;
  std::vector< const ParameterFunctional* > coefficients_;
  OperatorInterface* affinePart_;
}; // class AffinelyDecomposed


} // namespace Operators
} // namespace Pymor
} // namespace Dune

#endif // DUNE_PYMOR_OPERATORS_AFFINE_HH
