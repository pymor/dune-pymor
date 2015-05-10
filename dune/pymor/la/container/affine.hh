// This file is part of the dune-pymor project:
//   https://github.com/pymor/dune-pymor
// Copyright holders: Stephan Rave, Felix Schindler
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_LA_CONTAINER_AFFINE_HH
#define DUNE_PYMOR_LA_CONTAINER_AFFINE_HH

#include <memory>
#include <vector>
#include <type_traits>

#include <boost/numeric/conversion/cast.hpp>

#include <dune/stuff/aliases.hh>
#include <dune/stuff/common/exceptions.hh>
#include <dune/stuff/common/timedlogging.hh>
#include <dune/stuff/common/string.hh>
#include <dune/stuff/la/container/interfaces.hh>

#include <dune/pymor/common/exceptions.hh>
#include <dune/pymor/parameters/base.hh>
#include <dune/pymor/parameters/functional.hh>

namespace Dune {
namespace Pymor {
namespace LA {


template< class ContainerImp >
class AffinelyDecomposedConstContainer
  : public Parametric
{
  static_assert(std::is_base_of< Stuff::LA::ContainerInterface< typename ContainerImp::Traits >, ContainerImp >::value,
                "ContainerType must be derived from ContainerInterface");

  typedef AffinelyDecomposedConstContainer< ContainerImp > ThisType;
public:
  typedef ContainerImp ContainerType;

  static const std::string static_id() { return "pymor.la.affinelydecomposedcontainer"; }

  AffinelyDecomposedConstContainer()
    : hasAffinePart_(false)
    , num_components_(0)
  {}

  AffinelyDecomposedConstContainer(const ThisType& other) = default;

  AffinelyDecomposedConstContainer(ThisType&& source) = default;

  ThisType& operator=(const ThisType& other) = default;

  ThisType& operator=(ThisType&& source) = default;

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

  DUNE_STUFF_SSIZE_T num_components() const
  {
    return num_components_;
  }

  /**
   * \attention This class takes ownership of aff_ptr (in the sense, that you must not delete it manually)!
   */
  void register_affine_part(const ContainerType* aff_ptr)
  {
    register_affine_part(std::shared_ptr< const ContainerType >(aff_ptr));
  }

  void register_affine_part(const std::shared_ptr< const ContainerType > aff_ptr)
  {
    if (hasAffinePart_)
      DUNE_THROW(Stuff::Exceptions::you_are_using_this_wrong,
                 "do not call register_affine_part(aff_ptr) if has_affine_part() == true!");
    if (num_components_ > 0 && !components_[0]->has_equal_shape(*aff_ptr))
      DUNE_THROW(Stuff::Exceptions::shapes_do_not_match,
                            "the shape of aff_ptr does not match the shape of the existing containers!");
    affinePart_ = aff_ptr;
    hasAffinePart_ = true;
  }

  /**
   * \attention This class takes ownership of comp_ptr and coeff_ptr (in the sense, that you must not delete it manually)!
   */
  DUNE_STUFF_SSIZE_T register_component(const ContainerType* comp_ptr,
                                        const ParameterFunctional* coeff_ptr)
  {
    return register_component(std::shared_ptr< const ContainerType >(comp_ptr),
                              std::shared_ptr< const ParameterFunctional >(coeff_ptr));
  }

  /**
   * \attention This class takes ownership of comp_ptr (in the sense, that you must not delete it manually)!
   */
  DUNE_STUFF_SSIZE_T register_component(const ContainerType* comp_ptr,
                                        const std::shared_ptr< const ParameterFunctional > coeff_ptr)
  {
    return register_component(std::shared_ptr< const ContainerType >(comp_ptr), coeff_ptr);
  }

  /**
   * \attention This class takes ownership of coeff_ptr (in the sense, that you must not delete it manually)!
   */
  DUNE_STUFF_SSIZE_T register_component(const std::shared_ptr< const ContainerType > comp_ptr,
                                        const ParameterFunctional* coeff_ptr)
  {
    return register_component(comp_ptr, std::shared_ptr< const ParameterFunctional >(coeff_ptr));
  }

  DUNE_STUFF_SSIZE_T register_component(const std::shared_ptr< const ContainerType > comp_ptr,
                                        const std::shared_ptr< const ParameterFunctional > coeff_ptr)
  {
    if (hasAffinePart_) {
      if (!affinePart_->has_equal_shape(*comp_ptr))
        DUNE_THROW(Stuff::Exceptions::shapes_do_not_match,
                   "the shape of comp_ptr does not match the shape of the existing containers!");
    } else if (num_components_ > 0)
      if (!components_[0]->has_equal_shape(*comp_ptr))
        DUNE_THROW(Stuff::Exceptions::shapes_do_not_match,
                   "the shape of aff_ptr does not match the shape of the existing components!");
    components_.push_back(comp_ptr);
    coefficients_.push_back(coeff_ptr);
    inherit_parameter_type(coeff_ptr->parameter_type(), "coefficient_" + Dune::Stuff::Common::toString(num_components_));
    ++num_components_;
    return num_components_ - 1;
  }

  std::shared_ptr< const ContainerType > affine_part() const
  {
    if (!hasAffinePart_)
      DUNE_THROW(Stuff::Exceptions::requirements_not_met,
                 "do not call affine_part() if has_affine_part() == false!");
    return affinePart_;
  }

  std::shared_ptr< const ContainerType > component(const DUNE_STUFF_SSIZE_T qq) const
  {
    if (num_components_ == 0)
      DUNE_THROW(Stuff::Exceptions::requirements_not_met,
                 "do not call component(" << qq << ") if num_components() == 0!");
    if (qq < 0 || qq >= int(num_components_))
      DUNE_THROW(Stuff::Exceptions::index_out_of_range,
                 "the condition 0 < " << qq << " < num_components() = " << num_components_
                       << " is not satisfied!");
    return components_[qq];
  }

  std::shared_ptr< const ParameterFunctional > coefficient(const DUNE_STUFF_SSIZE_T qq) const
  {
    if (num_components_ == 0)
      DUNE_THROW(Stuff::Exceptions::requirements_not_met,
                 "do not call coefficient(" << qq << ") if num_components() == 0!");
    if (qq < 0 || qq >= int(num_components_))
      DUNE_THROW(Stuff::Exceptions::index_out_of_range,
                 "the condition 0 < " << qq << " < num_components() = " << num_components_
                       << " is not satisfied!");
    return coefficients_[qq];
  }

  ContainerType freeze_parameter(const Parameter mu = Parameter()) const
  {
#ifndef NDEBUG
    auto logger = DSC::TimedLogger().get(static_id() + ".with_mu");
#endif
    if (mu.type() != parameter_type())
      DUNE_THROW(Exceptions::wrong_parameter_type,
                 "the type of mu (" << mu.type() << ") does not match the parameter_type of this ("
                       << parameter_type() << ")!");
    if (num_components_ == 0 && !hasAffinePart_)
      DUNE_THROW(Stuff::Exceptions::requirements_not_met,
                 "do not call freeze_parameter() if num_components() == 0 and has_affine_part() == false!");
    if (components_.size() != boost::numeric_cast< size_t >(num_components_))
     DUNE_THROW(Stuff::Exceptions::internal_error, "");
    if (coefficients_.size() != boost::numeric_cast< size_t >(num_components_))
      DUNE_THROW(Stuff::Exceptions::internal_error, "");
    if (hasAffinePart_) {
      ContainerType result = affinePart_->copy();
      if (num_components_ > 0) {
#ifndef NDEBUG
        logger.debug() << "combining affine_part and " << num_components_ << " component"
                       << (num_components_ == 1 ? "" : "s") << "..." << std::endl;
#endif
        for (DUNE_STUFF_SSIZE_T ii = 0; ii < num_components_; ++ii) {
          const Parameter muCoefficient = map_parameter(mu, "coefficient_" + Dune::Stuff::Common::toString(ii));
          result.axpy(coefficients_[ii]->evaluate(muCoefficient), *(components_[ii]));
        }
      } else {
#ifndef NDEBUG
        logger.debug() << "returning affine_part" << std::endl;
#endif
      }
      return prune(result);
    } else {
#ifndef NDEBUG
      logger.debug() << "combining " << num_components_ << " component"
                     << (num_components_ == 1 ? "" : "s") << "..." << std::endl;
#endif
      ContainerType result = components_[0]->copy();
      const Parameter muCoefficient0 = map_parameter(mu, "coefficient_0");
      result.scal(coefficients_[0]->evaluate(muCoefficient0));
      for (DUNE_STUFF_SSIZE_T ii = 1; ii < num_components_; ++ii) {
        const Parameter muCoefficient = map_parameter(mu, "coefficient_" + Dune::Stuff::Common::toString(ii));
        result.axpy(coefficients_[ii]->evaluate(muCoefficient), *(components_[ii]));
      }
      return prune(result);
    }
  }

  ThisType copy()
  {
    ThisType ret;
    if (hasAffinePart_)
      ret.register_affine_part(new ContainerType(affinePart_->copy()));
    for (DUNE_STUFF_SSIZE_T qq = 0; qq < num_components_; ++qq)
      ret.register_component(new ContainerType(components_[qq]->copy()),
                             new ParameterFunctional(*coefficients_[qq]));
    return ret;
  } // ... copy(...)

protected:
  template< class CC, bool is_matrix = Stuff::LA::is_matrix< CC >::value >
  struct Prune{              static CC call(CC& cc){ return cc.pruned(); }};

  template< class CC >
  struct Prune< CC, false >{ static CC call(CC& cc){ return cc; }};

  template< class CC >
  static CC prune(CC& cc)
  {
    return Prune< CC >::call(cc);
  }

  bool hasAffinePart_;
  DUNE_STUFF_SSIZE_T num_components_;
  std::vector< std::shared_ptr< const ContainerType > > components_;
  std::vector< std::shared_ptr< const ParameterFunctional > > coefficients_;
  std::shared_ptr< const ContainerType > affinePart_;
}; // class AffinelyDecomposedConstContainer


template< class ContainerImp >
class AffinelyDecomposedContainer
  : public AffinelyDecomposedConstContainer< ContainerImp >
{
  typedef AffinelyDecomposedConstContainer< ContainerImp > BaseType;
  typedef AffinelyDecomposedContainer< ContainerImp >      ThisType;
public:
  typedef ContainerImp ContainerType;

  AffinelyDecomposedContainer()
    : BaseType()
  {}

  AffinelyDecomposedContainer(const ThisType& other) = default;

  AffinelyDecomposedContainer(ThisType&& source) = default;

  ThisType& operator=(const ThisType& other) = default;

  ThisType& operator=(ThisType&& source) = default;

  /**
   * \attention This class takes ownership of aff_ptr!
   */
  AffinelyDecomposedContainer(ContainerType* aff_ptr)
    : BaseType()
    , writableAffinePart_(aff_ptr)
  {
    BaseType::register_affine_part(writableAffinePart_);
  }

  AffinelyDecomposedContainer(std::shared_ptr< ContainerType > aff_ptr)
    : BaseType()
    , writableAffinePart_(aff_ptr)
  {
    BaseType::register_affine_part(writableAffinePart_);
  }

  /**
   * \attention This class takes ownership of aff_ptr!
   */
  AffinelyDecomposedContainer(ContainerType* comp_ptr, const ParameterFunctional* coeff_ptr)
    : BaseType()
  {
    writableComponents_.emplace_back(comp_ptr);
    BaseType::register_component(writableComponents_[0], std::shared_ptr< const ParameterFunctional >(coeff_ptr));
  }

  AffinelyDecomposedContainer(std::shared_ptr< ContainerType > comp_ptr, const ParameterFunctional* coeff_ptr)
    : BaseType()
  {
    writableComponents_.push_back(comp_ptr);
    BaseType::register_component(writableComponents_[0], std::shared_ptr< const ParameterFunctional >(coeff_ptr));
  }

  AffinelyDecomposedContainer(ContainerType* comp_ptr, const std::shared_ptr< const ParameterFunctional > coeff_ptr)
    : BaseType()
  {
    writableComponents_.emplace_back(comp_ptr);
    BaseType::register_component(writableComponents_[0], coeff_ptr);
  }

  AffinelyDecomposedContainer(std::shared_ptr< ContainerType > comp_ptr,
                              const std::shared_ptr< const ParameterFunctional > coeff_ptr)
    : BaseType()
  {
    writableComponents_.push_back(comp_ptr);
    BaseType::register_component(writableComponents_[0], coeff_ptr);
  }

  template< class... Args >
  void register_affine_part(Args&& ...args)
  {
    // doing this in two separate steps instead of using std::make_shared for easyer debugging on failure
    ContainerType* container_ptr = new ContainerType(std::forward< Args >(args)...);
    register_affine_part(std::shared_ptr< ContainerType >(container_ptr));
  }

  /**
   * \attention This class takes ownership of aff_ptr!
   */
  void register_affine_part(ContainerType* aff_ptr)
  {
    writableAffinePart_ = std::shared_ptr< ContainerType >(aff_ptr);
    BaseType::register_affine_part(writableAffinePart_);
  }

  void register_affine_part(std::shared_ptr< ContainerType > aff_ptr)
  {
    writableAffinePart_ = aff_ptr;
    BaseType::register_affine_part(writableAffinePart_);
  }

  template< class... Args >
  DUNE_STUFF_SSIZE_T register_component(const ParameterFunctional* coeff_ptr, Args&& ...args)
  {
    // doing this in two separate steps instead of using std::make_shared for easyer debugging on failure
    ContainerType* container_ptr = new ContainerType(std::forward< Args >(args)...);
    return register_component(std::shared_ptr< ContainerType >(container_ptr),
                              std::shared_ptr< const ParameterFunctional >(coeff_ptr));
  }

  template< class... Args >
  DUNE_STUFF_SSIZE_T register_component(const ParameterType& tt, const std::string& exp, Args&& ...args)
  {
    // doing this in two separate steps instead of using std::make_shared for easyer debugging on failure
    ContainerType* container_ptr = new ContainerType(std::forward< Args >(args)...);
    return register_component(std::shared_ptr< ContainerType >(container_ptr),
                              std::make_shared< ParameterFunctional >(tt, exp));
  }

  template< class... Args >
  DUNE_STUFF_SSIZE_T register_component(const std::string& kk, const DUNE_STUFF_SSIZE_T & vv, const std::string& exp,
                                        Args&& ...args)
  {
    // doing this in two separate steps instead of using std::make_shared for easyer debugging on failure
    ContainerType* container_ptr = new ContainerType(std::forward< Args >(args)...);
    return register_component(std::shared_ptr< ContainerType >(container_ptr),
                              std::make_shared< ParameterFunctional >(kk, vv, exp));
  }

  template< class... Args >
  DUNE_STUFF_SSIZE_T register_component(const std::vector< std::string >& kk,
                                        const std::vector< DUNE_STUFF_SSIZE_T >& vv,
                                        const std::string& exp,
                                        Args&& ...args)
  {
    // doing this in two separate steps instead of using std::make_shared for easyer debugging on failure
    ContainerType* container_ptr = new ContainerType(std::forward< Args >(args)...);
    return register_component(std::shared_ptr< ContainerType >(container_ptr),
                              std::make_shared< ParameterFunctional >(kk, vv, exp));
  }

  template< class... Args >
  DUNE_STUFF_SSIZE_T register_component(const ParameterFunctional& other, Args&& ...args)
  {
    // doing this in two separate steps instead of using std::make_shared for easyer debugging on failure
    ContainerType* container_ptr = new ContainerType(std::forward< Args >(args)...);
    return register_component(std::shared_ptr< ContainerType >(container_ptr),
                              std::make_shared< ParameterFunctional >(other));
  }

  DUNE_STUFF_SSIZE_T register_component(ContainerType* comp_ptr, const ParameterFunctional* coeff_ptr)
  {
    return register_component(std::shared_ptr< ContainerType >(comp_ptr),
                              std::shared_ptr< const ParameterFunctional >(coeff_ptr));
  }

  template< class... Args >
  DUNE_STUFF_SSIZE_T register_component(const std::shared_ptr< const ParameterFunctional > coeff_ptr, Args&& ...args)
  {
    // doing this in two separate steps instead of using std::make_shared for easyer debugging on failure
    ContainerType* container_ptr = new ContainerType(std::forward< Args >(args)...);
    return register_component(std::shared_ptr< ContainerType >(container_ptr), coeff_ptr);
  }

  /**
   * \attention This class takes ownership of comp_ptr!
   */
  DUNE_STUFF_SSIZE_T register_component(ContainerType* comp_ptr,
                                        const std::shared_ptr< const ParameterFunctional > coeff_ptr)
  {
    return register_component(std::shared_ptr< ContainerType >(comp_ptr), coeff_ptr);
  }

  /**
   * \attention This class takes ownership of coeff_ptr!
   */
  DUNE_STUFF_SSIZE_T register_component(std::shared_ptr< ContainerType > comp_ptr,
                                        const ParameterFunctional* coeff_ptr)
  {
    return register_component(comp_ptr, std::shared_ptr< const ParameterFunctional >(coeff_ptr));
  }

  DUNE_STUFF_SSIZE_T register_component(std::shared_ptr< ContainerType > comp_ptr,
                                        const std::shared_ptr< const ParameterFunctional > coeff_ptr)
  {
    writableComponents_.push_back(comp_ptr);
    return BaseType::register_component(comp_ptr, coeff_ptr);
  }

  std::shared_ptr< ContainerType > affine_part() const
  {
    if (!BaseType::has_affine_part())
      DUNE_THROW(Stuff::Exceptions::requirements_not_met,
                 "do not call affine_part() if has_affine_part() == false!");
    return writableAffinePart_;
  }

  using BaseType::component;

  std::shared_ptr< ContainerType > component(const DUNE_STUFF_SSIZE_T qq)
  {
    if (BaseType::num_components() == 0)
      DUNE_THROW(Stuff::Exceptions::requirements_not_met,
                 "do not call component(" << qq << ") if num_components() == 0!");
    if (qq < 0 || qq >= int(BaseType::num_components()))
      DUNE_THROW(Stuff::Exceptions::index_out_of_range,
                 "the condition 0 < " << qq << " < num_components() = " << BaseType::num_components()
                            << " is not satisfied!");
    return writableComponents_[qq];
  }

  ThisType copy()
  {
    ThisType ret;
    if (this->hasAffinePart_)
      ret.register_affine_part(new ContainerType(writableAffinePart_->copy()));
    for (DUNE_STUFF_SSIZE_T qq = 0; qq < this->num_components_; ++qq)
      ret.register_component(new ContainerType(writableComponents_[qq]->copy()),
                             new ParameterFunctional(*(this->coefficients_[qq])));
    return ret;
  } // ... copy(...)

private:
  std::vector< std::shared_ptr< ContainerType > > writableComponents_;
  std::shared_ptr< ContainerType > writableAffinePart_;
}; // class AffinelyDecomposedContainer


} // namespace LA
} // namespace Pymor
} // namespace Dune

#endif // DUNE_PYMOR_LA_CONTAINER_AFFINE_HH
