// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_OPERATORS_INTERFACES_HH
#define DUNE_PYMOR_OPERATORS_INTERFACES_HH

#include <type_traits>

#include <dune/pymor/common/exceptions.hh>
#include <dune/pymor/common/crtp.hh>
#include <dune/pymor/parameters/base.hh>
#include <dune/pymor/parameters/functional.hh>
#include <dune/pymor/la/container/interfaces.hh>

namespace Dune {
namespace Pymor {


template< class Traits >
class OperatorInterface
  : public Parametric
  , public CRTPInterface< OperatorInterface< Traits >, Traits >
{
  typedef CRTPInterface< OperatorInterface< Traits >, Traits > CRTP;
public:
  typedef typename Traits::derived_type derived_type;
  typedef typename Traits::SourceType   SourceType;
  typedef typename Traits::RangeType    RangeType;
  typedef typename Traits::ScalarType   ScalarType;
  typedef typename Traits::FrozenType   FrozenType;
  typedef typename Traits::InverseType  InverseType;

  static_assert(std::is_base_of< LA::VectorInterface< typename SourceType::Traits >, SourceType >::value,
                "SourceType has to be derived from LA::VectorInterface!");
  static_assert(std::is_base_of< LA::VectorInterface< typename RangeType::Traits >, RangeType >::value,
                "RangeType has to be derived from LA::VectorInterface!");

  OperatorInterface(const ParameterType mu = ParameterType())
    : Parametric(mu)
  {}

  OperatorInterface(const Parametric& other)
    : Parametric(other)
  {}

  bool linear() const
  {
    CHECK_INTERFACE_IMPLEMENTATION(CRTP::as_imp(*this).linear());
    return CRTP::as_imp(*this).linear();
  }

  unsigned int dim_source() const
  {
    CHECK_INTERFACE_IMPLEMENTATION(CRTP::as_imp(*this).dim_source());
    return CRTP::as_imp(*this).dim_source();
  }

  unsigned int dim_range() const
  {
    CHECK_INTERFACE_IMPLEMENTATION(CRTP::as_imp(*this).dim_range());
    return CRTP::as_imp(*this).dim_range();
  }

  void apply(const SourceType& source, RangeType& range, const Parameter mu = Parameter()) const
    throw (Exception::sizes_do_not_match,
           Exception::wrong_parameter_type,
           Exception::requirements_not_met,
           Exception::linear_solver_failed,
           Exception::this_does_not_make_any_sense)
  {
    CHECK_AND_CALL_INTERFACE_IMPLEMENTATION(CRTP::as_imp(*this).apply(source, range, mu));
  }

  RangeType apply(const SourceType& source, const Parameter mu = Parameter()) const
  throw (Exception::sizes_do_not_match,
         Exception::wrong_parameter_type,
         Exception::requirements_not_met,
         Exception::linear_solver_failed,
         Exception::this_does_not_make_any_sense)
  {
    RangeType range = source.copy();
    apply(source, range, mu);
    return range;
  }

  /**
   * \note  This default implementation of apply2 creates a temporary vector. Any derived class which can do better
   *        should implement this method!
   */
  ScalarType apply2(const RangeType& range, const SourceType& source, const Parameter mu = Parameter()) const
    throw (Exception::sizes_do_not_match,
           Exception::wrong_parameter_type,
           Exception::requirements_not_met,
           Exception::linear_solver_failed,
           Exception::this_does_not_make_any_sense)
  {
    RangeType tmp = range.copy();
    apply(source, tmp, mu);
    return tmp.dot(range);
  }

  static std::vector< std::string > invert_options()
  {
    return derived_type::invert_options();
  }

  InverseType invert(const std::string option = invert_options()[0], const Parameter mu = Parameter()) const
  {
    CHECK_INTERFACE_IMPLEMENTATION(CRTP::as_imp(*this).invert(option, mu));
    return CRTP::as_imp(*this).invert(option, mu);
  }

  void apply_inverse(const RangeType& range, SourceType& source,
                     const std::string option = invert_options()[0],
                     const Parameter mu = Parameter()) const
  {
    invert(option, mu).apply(range, source);
  }

  SourceType apply_inverse(const RangeType& range,
                           const std::string option = invert_options()[0],
                           const Parameter mu = Parameter()) const
  {
    SourceType source = range.copy();
    apply_inverse(range, source, option, mu);
    return source;
  }

  FrozenType freeze_parameter(const Parameter mu = Parameter()) const
    throw (Exception::this_is_not_parametric)
  {
    CHECK_INTERFACE_IMPLEMENTATION(CRTP::as_imp(*this).freeze_parameter(mu));
    return CRTP::as_imp(*this).freeze_parameter(mu);
  }
}; // class OperatorInterface


template< class Traits >
class AffinelyDecomposedOperatorInterface
  : public CRTPInterface< AffinelyDecomposedOperatorInterface< Traits >, Traits >
  , public OperatorInterface< Traits >
{
  typedef CRTPInterface< AffinelyDecomposedOperatorInterface< Traits >, Traits > CRTP;
  typedef OperatorInterface< Traits > BaseType;
public:
  typedef typename Traits::derived_type   derived_type;
  typedef typename Traits::ComponentType  ComponentType;
  static_assert(std::is_base_of< OperatorInterface< typename ComponentType::Traits >, ComponentType >::value,
                "ComponentType has to be derived from FunctionalInterface");

  AffinelyDecomposedOperatorInterface(const ParameterType mu = ParameterType())
    : BaseType(mu)
  {}

  AffinelyDecomposedOperatorInterface(const Parametric& other)
    : BaseType(other)
  {}

  unsigned int num_components() const
  {
    CHECK_INTERFACE_IMPLEMENTATION(CRTP::as_imp(*this).num_components());
    return CRTP::as_imp(*this).num_components();
  }

  ComponentType component(const int qq) const throw (Exception::requirements_not_met, Exception::index_out_of_range)
  {
    CHECK_INTERFACE_IMPLEMENTATION(CRTP::as_imp(*this).component(qq));
    return CRTP::as_imp(*this).component(qq);
  }

  ParameterFunctional coefficient(const int qq) const throw (Exception::requirements_not_met,
                                                             Exception::index_out_of_range)
  {
    CHECK_INTERFACE_IMPLEMENTATION(CRTP::as_imp(*this).coefficient(qq));
    return CRTP::as_imp(*this).coefficient(qq);
  }

  bool has_affine_part() const
  {
    CHECK_INTERFACE_IMPLEMENTATION(CRTP::as_imp(*this).has_affine_part());
    return CRTP::as_imp(*this).has_affine_part();
  }

  ComponentType affine_part() const throw (Exception::requirements_not_met)
  {
    CHECK_INTERFACE_IMPLEMENTATION(CRTP::as_imp(*this).affine_part());
    return CRTP::as_imp(*this).affine_part();
  }
}; // class AffinelyDecomposedOperatorInterface


} // namespace Pymor
} // namespace Dune

#endif // DUNE_PYMOR_OPERATORS_INTERFACES_HH
