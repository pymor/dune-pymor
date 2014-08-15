// This file is part of the dune-pymor project:
//   https://github.com/pymor/dune-pymor
// Copyright holders: Stephan Rave, Felix Schindler
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_OPERATORS_INTERFACES_HH
#define DUNE_PYMOR_OPERATORS_INTERFACES_HH

#include <dune/stuff/common/type_utils.hh>
#include <dune/stuff/la/container/interfaces.hh>
#include <dune/stuff/la/solver.hh>
#include <dune/stuff/common/crtp.hh>
#include <dune/stuff/common/configuration.hh>

#include <dune/pymor/common/exceptions.hh>
#include <dune/pymor/parameters/base.hh>
#include <dune/pymor/parameters/functional.hh>

namespace Dune {
namespace Pymor {

/**
 *  \brief  Contains tags mostly needed for python bindings.
 */
namespace Tags {


class OperatorInterface {};

class AffinelyDecomposedOperatorInterface : public OperatorInterface {};


} // namespace Tags


template< class Traits >
class OperatorInterface
  : public Parametric
  , public Tags::OperatorInterface
  , public Stuff::CRTPInterface< OperatorInterface< Traits >, Traits >
{
public:
  typedef typename Traits::derived_type derived_type;
  typedef typename Traits::SourceType   SourceType;
  typedef typename Traits::RangeType    RangeType;
  typedef typename Traits::ScalarType   ScalarType;
  typedef typename Traits::FrozenType   FrozenType;
  typedef typename Traits::InverseType  InverseType;

  static_assert(std::is_base_of< Stuff::LA::VectorInterface< typename SourceType::Traits >, SourceType >::value,
                "SourceType has to be derived from Stuff::LA::VectorInterface!");
  static_assert(std::is_base_of< Stuff::LA::VectorInterface< typename RangeType::Traits >, RangeType >::value,
                "RangeType has to be derived from Stuff::LA::VectorInterface!");

  static std::string type_this() {    return Stuff::Common::Typename< derived_type >::value(); }
  static std::string type_source() {  return Stuff::Common::Typename< SourceType >::value(); }
  static std::string type_range() {   return Stuff::Common::Typename< RangeType >::value(); }
  static std::string type_scalar() {  return Stuff::Common::Typename< ScalarType >::value(); }
  static std::string type_frozen() {  return Stuff::Common::Typename< FrozenType >::value(); }
  static std::string type_inverse() { return Stuff::Common::Typename< InverseType >::value(); }

  OperatorInterface(const ParameterType mu = ParameterType())
    : Parametric(mu)
  {}

  OperatorInterface(const Parametric& other)
    : Parametric(other)
  {}

  bool linear() const
  {
    CHECK_CRTP(this->as_imp(*this).linear());
    return this->as_imp(*this).linear();
  }

  DUNE_STUFF_SSIZE_T dim_source() const
  {
    CHECK_CRTP(this->as_imp(*this).dim_source());
    return this->as_imp(*this).dim_source();
  }

  DUNE_STUFF_SSIZE_T dim_range() const
  {
    CHECK_CRTP(this->as_imp(*this).dim_range());
    return this->as_imp(*this).dim_range();
  }

  void apply(const SourceType& source, RangeType& range, const Parameter mu = Parameter()) const
  {
    CHECK_AND_CALL_CRTP(this->as_imp(*this).apply(source, range, mu));
  }

  RangeType apply(const SourceType& source, const Parameter mu = Parameter()) const
  {
    RangeType range(dim_range());
    apply(source, range, mu);
    return range;
  }

  RangeType* apply_and_return_ptr(const SourceType& source, const Parameter mu = Parameter()) const
  {
    return new RangeType(apply(source, mu));
  }

  /**
   * \note  This default implementation of apply2 creates a temporary vector. Any derived class which can do better
   *        should implement this method!
   */
  ScalarType apply2(const RangeType& range, const SourceType& source, const Parameter mu = Parameter()) const
  {
    RangeType tmp = range.copy();
    apply(source, tmp, mu);
    return tmp.dot(range);
  }

  static std::vector< std::string > invert_options()
  {
    return derived_type::invert_options();
  }

  static Stuff::Common::Configuration invert_options(const std::string& type)
  {
    return derived_type::invert_options(type);
  }

  InverseType invert(const std::string type = invert_options()[0], const Parameter mu = Parameter()) const
  {
    return invert(invert_options(type), mu);
  }

  InverseType invert(const Stuff::Common::Configuration& option, const Parameter mu = Parameter()) const
  {
    CHECK_CRTP(this->as_imp(*this).invert(option, mu));
    return this->as_imp(*this).invert(option, mu);
  }

  InverseType* invert_and_return_ptr(const std::string type = invert_options()[0],
                                     const Parameter mu = Parameter()) const
  {
    return new InverseType(invert(type, mu));
  }

  InverseType* invert_and_return_ptr(const Stuff::Common::Configuration& option,
                                     const Parameter mu = Parameter()) const
  {
    return new InverseType(invert(option, mu));
  }

  void apply_inverse(const RangeType& range,
                     SourceType& source,
                     const std::string type = invert_options()[0],
                     const Parameter mu = Parameter()) const
  {
    invert(type, mu).apply(range, source);
  }

  void apply_inverse(const RangeType& range,
                     SourceType& source,
                     const Stuff::Common::Configuration& option,
                     const Parameter mu = Parameter()) const
  {
    invert(option, mu).apply(range, source);
  }

  SourceType apply_inverse(const RangeType& range,
                           const std::string type = invert_options()[0],
                           const Parameter mu = Parameter()) const
  {
    SourceType source(dim_source());
    apply_inverse(range, source, type, mu);
    return source;
  }

  SourceType apply_inverse(const RangeType& range,
                           const Stuff::Common::Configuration& option,
                           const Parameter mu = Parameter()) const
  {
    SourceType source(dim_source());
    apply_inverse(range, source, option, mu);
    return source;
  }

  SourceType* apply_inverse_and_return_ptr(const RangeType& range,
                                           const std::string tyep = invert_options()[0],
                                           const Parameter mu = Parameter()) const
  {
    return new SourceType(apply_inverse(range, tyep, mu));
  }

  SourceType* apply_inverse_and_return_ptr(const RangeType& range,
                                           const Stuff::Common::Configuration& option,
                                           const Parameter mu = Parameter()) const
  {
    return new SourceType(apply_inverse(range, option, mu));
  }

  /**
   * \note  May throw Exceptions::this_is_not_parametric.
   */
  FrozenType freeze_parameter(const Parameter mu = Parameter()) const
  {
    CHECK_CRTP(this->as_imp(*this).freeze_parameter(mu));
    return this->as_imp(*this).freeze_parameter(mu);
  }

  FrozenType* freeze_parameter_and_return_ptr(const Parameter mu = Parameter()) const
  {
    return new FrozenType(freeze_parameter(mu));
  }
}; // class OperatorInterface


template< class Traits >
class AffinelyDecomposedOperatorInterface
  : public OperatorInterface< Traits >
  , public Tags::AffinelyDecomposedOperatorInterface
{
  typedef Pymor::OperatorInterface< Traits > BaseType;
public:
  typedef typename Traits::derived_type   derived_type;
  typedef typename Traits::ComponentType  ComponentType;
  static_assert(std::is_base_of< Pymor::OperatorInterface< typename ComponentType::Traits >, ComponentType >::value,
                "ComponentType has to be derived from OperatorInterface");

  AffinelyDecomposedOperatorInterface(const ParameterType mu = ParameterType())
    : BaseType(mu)
  {}

  AffinelyDecomposedOperatorInterface(const Parametric& other)
    : BaseType(other)
  {}

  DUNE_STUFF_SSIZE_T num_components() const
  {
    CHECK_CRTP(this->as_imp(*this).num_components());
    return this->as_imp(*this).num_components();
  }

  /**
   * \note  May throw one of Stuff::Exceptions::requirements_not_met, Stuff::Exceptions::index_out_of_range.
   */
  ComponentType component(const DUNE_STUFF_SSIZE_T qq) const
  {
    CHECK_CRTP(this->as_imp(*this).component(qq));
    return this->as_imp(*this).component(qq);
  }

  ComponentType* component_and_return_ptr(const DUNE_STUFF_SSIZE_T qq) const
  {
    return new ComponentType(component(qq));
  }

  /**
   * \note  May throw one of Stuff::Exceptions::requirements_not_met, Stuff::Exceptions::index_out_of_range.
   */
  ParameterFunctional coefficient(const DUNE_STUFF_SSIZE_T qq) const
  {
    CHECK_CRTP(this->as_imp(*this).coefficient(qq));
    return this->as_imp(*this).coefficient(qq);
  }

  ParameterFunctional* coefficient_and_return_ptr(const DUNE_STUFF_SSIZE_T qq) const
  {
    return new ParameterFunctional(coefficient(qq));
  }

  bool has_affine_part() const
  {
    CHECK_CRTP(this->as_imp(*this).has_affine_part());
    return this->as_imp(*this).has_affine_part();
  }

  /**
   * \note  May throw Stuff::Exceptions::requirements_not_met.
   */
  ComponentType affine_part() const
  {
    CHECK_CRTP(this->as_imp(*this).affine_part());
    return this->as_imp(*this).affine_part();
  }

  ComponentType* affine_part_and_return_ptr() const
  {
    return new ComponentType(affine_part());
  }
}; // class AffinelyDecomposedOperatorInterface


} // namespace Pymor
} // namespace Dune

#endif // DUNE_PYMOR_OPERATORS_INTERFACES_HH
