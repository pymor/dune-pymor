// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_LA_CONTAINER_AFFINE_HH
#define DUNE_PYMOR_LA_CONTAINER_AFFINE_HH

#include <memory>
#include <vector>
#include <type_traits>

#include <dune/stuff/common/string.hh>

#include <dune/pymor/parameters/base.hh>
#include <dune/pymor/parameters/functional.hh>
#include "interfaces.hh"

namespace Dune {
namespace Pymor {
namespace LA {


template< class ContainerImp >
class AffinelyDecomposedConstContainer
  : public Parametric
{
  static_assert(std::is_base_of< ContainerInterface< typename ContainerImp::Traits >, ContainerImp >::value,
                "ContainerType must be derived from ContainerInterface");
public:
  typedef ContainerImp                        ContainerType;

  AffinelyDecomposedConstContainer()
    : hasAffinePart_(false)
    , num_components_(0)
  {}

  /**
   * \attention This class takes ownership of aff_ptr (in the sense, that you must not delete it manually)!
   */
  AffinelyDecomposedConstContainer(const ContainerType* aff_ptr)
    : hasAffinePart_(true)
    , num_components_(0)
    , affinePart_(aff_ptr)
  {}

  AffinelyDecomposedConstContainer(const std::shared_ptr< const ContainerType > aff_ptr)
    : hasAffinePart_(true)
    , num_components_(0)
    , affinePart_(aff_ptr)
  {}

  /**
   * \attention This class takes ownership of comp_ptr and coeff_ptr (in the sense, that you must not delete it manually)!
   */
  AffinelyDecomposedConstContainer(const ContainerType* comp_ptr, const ParameterFunctional* coeff_ptr)
    : hasAffinePart_(false)
    , num_components_(1)
  {
    components_.emplace_back(comp_ptr);
    coefficients_.emplace_back(coeff_ptr);
    inherit_parameter_type(coeff_ptr->parameter_type(), "coefficient_0");
  }

  /**
   * \attention This class takes ownership of coeff_ptr (in the sense, that you must not delete it manually)!
   */
  AffinelyDecomposedConstContainer(const std::shared_ptr< const ContainerType > comp_ptr,
                                   const ParameterFunctional* coeff_ptr)
    : hasAffinePart_(false)
    , num_components_(1)
  {
    components_.push_back(comp_ptr);
    coefficients_.emplace_back(coeff_ptr);
    inherit_parameter_type(coeff_ptr->parameter_type(), "coefficient_0");
  }

  /**
   * \attention This class takes ownership of comp_ptr (in the sense, that you must not delete it manually)!
   */
  AffinelyDecomposedConstContainer(const ContainerType* comp_ptr,
                                   const std::shared_ptr< const ParameterFunctional > coeff_ptr)
    : hasAffinePart_(false)
    , num_components_(1)
  {
    components_.emplace_back(comp_ptr);
    coefficients_.push_back(coeff_ptr);
    inherit_parameter_type(coeff_ptr->parameter_type(), "coefficient_0");
  }

  AffinelyDecomposedConstContainer(const std::shared_ptr< const ContainerType > comp_ptr,
                                   const std::shared_ptr< const ParameterFunctional > coeff_ptr)
    : hasAffinePart_(false)
    , num_components_(1)
  {
    components_.push_back(comp_ptr);
    coefficients_.push_back(coeff_ptr);
    inherit_parameter_type(coeff_ptr->parameter_type(), "coefficient_0");
  }

  bool has_affine_part() const
  {
    return hasAffinePart_;
  }

  unsigned int num_components() const
  {
    return num_components_;
  }

  /**
   * \attention This class takes ownership of aff_ptr (in the sense, that you must not delete it manually)!
   */
  void register_affine_part(const ContainerType* aff_ptr) throw (Exception::this_does_not_make_any_sense,
                                                                 Exception::sizes_do_not_match)
  {
    if (hasAffinePart_)
      DUNE_PYMOR_THROW(Exception::this_does_not_make_any_sense,
                       "do not call register_affine_part(aff_ptr) if has_affine_part() == true!");
    if (num_components_ > 0)
      if (!components_[0]->has_equal_shape(*aff_ptr))
        DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                         "the shape of aff_ptr does not match the shape of the existing containers!");
    affinePart_ = std::shared_ptr< const ContainerType >(aff_ptr);
  }

  void register_affine_part(const std::shared_ptr< const ContainerType > aff_ptr)
    throw (Exception::this_does_not_make_any_sense, Exception::sizes_do_not_match)
  {
    if (hasAffinePart_)
      DUNE_PYMOR_THROW(Exception::this_does_not_make_any_sense,
                       "do not call register_affine_part(aff_ptr) if has_affine_part() == true!");
    if (num_components_ > 0)
      if (!components_[0]->has_equal_shape(*aff_ptr))
        DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                         "the shape of aff_ptr does not match the shape of the existing containers!");
    affinePart_ = aff_ptr;
  }

  /**
   * \attention This class takes ownership of comp_ptr and coeff_ptr (in the sense, that you must not delete it manually)!
   */
  void register_component(const ContainerType* comp_ptr,
                          const ParameterFunctional* coeff_ptr) throw (Exception::sizes_do_not_match)
  {
    register_component(std::shared_ptr< const ContainerType >(comp_ptr),
                       std::shared_ptr< const ParameterFunctional >(coeff_ptr));
  }

  /**
   * \attention This class takes ownership of comp_ptr (in the sense, that you must not delete it manually)!
   */
  void register_component(const ContainerType* comp_ptr,
                          const std::shared_ptr< const ParameterFunctional > coeff_ptr)
    throw (Exception::sizes_do_not_match)
  {
    register_component(std::shared_ptr< const ContainerType >(comp_ptr), coeff_ptr);
  }

  /**
   * \attention This class takes ownership of coeff_ptr (in the sense, that you must not delete it manually)!
   */
  void register_component(const std::shared_ptr< const ContainerType > comp_ptr,
                          const ParameterFunctional* coeff_ptr) throw (Exception::sizes_do_not_match)
  {
    register_component(comp_ptr, std::shared_ptr< const ParameterFunctional >(coeff_ptr));
  }

  void register_component(const std::shared_ptr< const ContainerType > comp_ptr,
                          const std::shared_ptr< const ParameterFunctional > coeff_ptr)
    throw (Exception::sizes_do_not_match)
  {
    if (hasAffinePart_) {
      if (!affinePart_->has_equal_shape(*comp_ptr))
        DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                         "the shape of comp_ptr does not match the shape of the existing containers!");
    } else if (num_components_ > 0)
      if (!components_[0]->has_equal_shape(*comp_ptr))
        DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                         "the shape of aff_ptr does not match the shape of the existing components!");
    components_.push_back(comp_ptr);
    coefficients_.push_back(coeff_ptr);
    inherit_parameter_type(coeff_ptr->parameter_type(), "coefficient_" + Dune::Stuff::Common::toString(num_components_));
    ++num_components_;
  }

  std::shared_ptr< const ContainerType > affine_part() const throw (Exception::requirements_not_met)
  {
    if (!hasAffinePart_)
      DUNE_PYMOR_THROW(Exception::requirements_not_met,
                       "do not call affine_part() if has_affine_part() == false!");
    return affinePart_;
  }

  std::shared_ptr< const ContainerType > component(const int qq) const throw (Exception::requirements_not_met,
                                                                              Exception::index_out_of_range)
  {
    if (num_components_ == 0)
      DUNE_PYMOR_THROW(Exception::requirements_not_met,
                       "do not call component(" << qq << ") if num_components() == 0!");
    if (qq < 0 || qq >= int(num_components_))
      DUNE_PYMOR_THROW(Exception::index_out_of_range,
                       "the condition 0 < " << qq << " < num_components() = " << num_components_
                       << " is not satisfied!");
    return components_[qq];
  }

  std::shared_ptr< const ParameterFunctional > coefficient(const int qq) const throw (Exception::requirements_not_met,
                                                                                      Exception::index_out_of_range)
  {
    if (num_components_ == 0)
      DUNE_PYMOR_THROW(Exception::requirements_not_met,
                       "do not call coefficient(" << qq << ") if num_components() == 0!");
    if (qq < 0 || qq >= int(num_components_))
      DUNE_PYMOR_THROW(Exception::index_out_of_range,
                       "the condition 0 < " << qq << " < num_components() = " << num_components_
                       << " is not satisfied!");
    return coefficients_[qq];
  }

  ContainerType freeze_parameter(const Parameter mu = Parameter()) const throw (Exception::wrong_parameter_type,
                                                                                Exception::requirements_not_met)
  {
    if (mu.type() != parameter_type())
      DUNE_PYMOR_THROW(Exception::wrong_parameter_type,
                       "the type of mu (" << mu.type() << ") does not match the parameter_type of this ("
                       << parameter_type() << ")!");
    if (num_components_ == 0 && !hasAffinePart_)
      DUNE_PYMOR_THROW(Exception::requirements_not_met,
                 "do not call freeze_parameter() if num_components() == 0 and has_affine_part() == false!");
    if (components_.size() != num_components_) DUNE_PYMOR_THROW(Exception::this_does_not_make_any_sense, "");
    if (coefficients_.size() != num_components_) DUNE_PYMOR_THROW(Exception::this_does_not_make_any_sense, "");
    if (hasAffinePart_) {
      ContainerType result = affinePart_->copy();
      if (num_components_ > 0)
        for (unsigned int ii = 0; ii < num_components_; ++ii) {
          const Parameter muCoefficient = map_parameter(mu, "coefficient_" + Dune::Stuff::Common::toString(ii));
          result.axpy(coefficients_[ii]->evaluate(muCoefficient), *(components_[ii]));
        }
      return result;
    } else {
      ContainerType result = components_[0]->copy();
      const Parameter muCoefficient0 = map_parameter(mu, "coefficient_0");
      result.scal(coefficients_[0]->evaluate(muCoefficient0));
      for (unsigned int ii = 1; ii < num_components_; ++ii) {
        const Parameter muCoefficient = map_parameter(mu, "coefficient_" + Dune::Stuff::Common::toString(ii));
        result.axpy(coefficients_[ii]->evaluate(muCoefficient), *(components_[ii]));
      }
      return result;
    }
  }

private:
  bool hasAffinePart_;
  unsigned int num_components_;
  std::vector< std::shared_ptr< const ContainerType > > components_;
  std::vector< std::shared_ptr< const ParameterFunctional > > coefficients_;
  std::shared_ptr< const ContainerType > affinePart_;
}; // class AffinelyDecomposedConstContainer


template< class ContainerImp >
class AffinelyDecomposedContainer
  : public AffinelyDecomposedConstContainer< ContainerImp >
{
  typedef AffinelyDecomposedConstContainer< ContainerImp > BaseType;
public:
  typedef ContainerImp                        ContainerType;

  AffinelyDecomposedContainer()
    : BaseType()
  {}

  /**
   * \attention This class takes ownership of aff_ptr!
   */
  AffinelyDecomposedContainer(ContainerType* aff_ptr)
    : BaseType()
    , affinePart_(aff_ptr)
  {
    BaseType::register_affine_part(affinePart_);
  }

  AffinelyDecomposedContainer(std::shared_ptr< ContainerType > aff_ptr)
    : BaseType()
    , affinePart_(aff_ptr)
  {
    BaseType::register_affine_part(affinePart_);
  }

  /**
   * \attention This class takes ownership of aff_ptr!
   */
  AffinelyDecomposedContainer(ContainerType* comp_ptr, const ParameterFunctional* coeff_ptr)
    : BaseType()
  {
    components_.emplace_back(comp_ptr);
    BaseType::register_component(components_[0], std::shared_ptr< const ParameterFunctional >(coeff_ptr));
  }

  AffinelyDecomposedContainer(std::shared_ptr< ContainerType > comp_ptr, const ParameterFunctional* coeff_ptr)
    : BaseType()
  {
    components_.push_back(comp_ptr);
    BaseType::register_component(components_[0], std::shared_ptr< const ParameterFunctional >(coeff_ptr));
  }

  AffinelyDecomposedContainer(ContainerType* comp_ptr, const std::shared_ptr< const ParameterFunctional > coeff_ptr)
    : BaseType()
  {
    components_.emplace_back(comp_ptr);
    BaseType::register_component(components_[0], coeff_ptr);
  }

  AffinelyDecomposedContainer(std::shared_ptr< ContainerType > comp_ptr,
                              const std::shared_ptr< const ParameterFunctional > coeff_ptr)
    : BaseType()
  {
    components_.push_back(comp_ptr);
    BaseType::register_component(components_[0], coeff_ptr);
  }

  /**
   * \attention This class takes ownership of aff_ptr!
   */
  void register_affine_part(ContainerType* aff_ptr) throw (Exception::this_does_not_make_any_sense,
                                                           Exception::sizes_do_not_match)
  {
    affinePart_ = std::shared_ptr< ContainerType >(aff_ptr);
    BaseType::register_affine_part(affinePart_);
  }

  void register_affine_part(std::shared_ptr< ContainerType > aff_ptr) throw (Exception::this_does_not_make_any_sense,
                                                                             Exception::sizes_do_not_match)
  {
    affinePart_ = aff_ptr;
    BaseType::register_affine_part(affinePart_);
  }

  void register_component(ContainerType* comp_ptr,
                          const ParameterFunctional* coeff_ptr) throw (Exception::sizes_do_not_match)
  {
    register_component(std::shared_ptr< ContainerType >(comp_ptr),
                       std::shared_ptr< const ParameterFunctional >(coeff_ptr));
  }

  /**
   * \attention This class takes ownership of comp_ptr!
   */
  void register_component(ContainerType* comp_ptr,
                          const std::shared_ptr< const ParameterFunctional > coeff_ptr)
    throw (Exception::sizes_do_not_match)
  {
    register_component(std::shared_ptr< ContainerType >(comp_ptr), coeff_ptr);
  }

  /**
   * \attention This class takes ownership of coeff_ptr!
   */
  void register_component(std::shared_ptr< ContainerType > comp_ptr,
                          const ParameterFunctional* coeff_ptr) throw (Exception::sizes_do_not_match)
  {
    register_component(comp_ptr, std::shared_ptr< const ParameterFunctional >(coeff_ptr));
  }

  void register_component(std::shared_ptr< ContainerType > comp_ptr,
                          const std::shared_ptr< const ParameterFunctional > coeff_ptr)
    throw (Exception::sizes_do_not_match)
  {
    components_.push_back(comp_ptr);
    BaseType::register_component(comp_ptr, coeff_ptr);
  }

  std::shared_ptr< ContainerType > affine_part() const throw (Exception::requirements_not_met)
  {
    if (!BaseType::has_affine_part())
      DUNE_PYMOR_THROW(Exception::requirements_not_met,
                       "do not call affine_part() if has_affine_part() == false!");
    return affinePart_;
  }

  std::shared_ptr< ContainerType > component(const int qq) throw (Exception::requirements_not_met,
                                                                  Exception::index_out_of_range)
  {
    if (BaseType::num_components() == 0)
      DUNE_PYMOR_THROW(Exception::requirements_not_met,
                       "do not call component(" << qq << ") if num_components() == 0!");
    if (qq < 0 || qq >= int(BaseType::num_components()))
      DUNE_PYMOR_THROW(Exception::index_out_of_range,
                       "the condition 0 < " << qq << " < num_components() = " << BaseType::num_components()
                       << " is not satisfied!");
    return components_[qq];
  }

private:
  std::vector< std::shared_ptr< ContainerType > > components_;
  std::shared_ptr< ContainerType > affinePart_;
}; // class AffinelyDecomposedContainer


} // namespace LA
} // namespace Pymor
} // namespace Dune

#endif // DUNE_PYMOR_LA_CONTAINER_AFFINE_HH
