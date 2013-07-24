// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_OPERATORS_AFFINE_HH
#define DUNE_PYMOR_OPERATORS_AFFINE_HH

#include <type_traits>

#include <dune/pymor/la/container/interfaces.hh>
#include <dune/pymor/la/container/affine.hh>
#include "interfaces.hh"

namespace Dune {
namespace Pymor {
namespace Operators {


template< class OperatorType >
class LinearAffinelyDecomposedContainerBased;


template< class OperatorType >
class LinearAffinelyDecomposedContainerBasedTraits
{
public:
  typedef LinearAffinelyDecomposedContainerBased< OperatorType >  derived_type;
  static_assert(std::is_base_of< LA::ProvidesContainer< typename OperatorType::Traits >, OperatorType > ::value,
                "OperatorType has to be derived from LA::ProvidesContainer!");
  typedef typename OperatorType::ContainerType                    ContainerType;
  static_assert(std::is_base_of< OperatorInterface< typename OperatorType::Traits >, OperatorType > ::value,
                "OperatorType has to be derived from OperatorInterface!");
  typedef OperatorType                                            ComponentType;
  typedef typename OperatorType::SourceType                       SourceType;
  typedef typename OperatorType::RangeType                        RangeType;
  typedef typename OperatorType::ScalarType                       ScalarType;
  typedef typename OperatorType::FrozenType                       FrozenType;
  typedef typename OperatorType::InverseType                      InverseType;
}; // class LinearAffinelyDecomposedContainerBasedTraits


template< class OperatorType >
class LinearAffinelyDecomposedContainerBased
  : public AffinelyDecomposedOperatorInterface< LinearAffinelyDecomposedContainerBasedTraits < OperatorType > >
{
  typedef AffinelyDecomposedOperatorInterface< LinearAffinelyDecomposedContainerBasedTraits < OperatorType > > BaseType;
public:
  typedef LinearAffinelyDecomposedContainerBasedTraits < OperatorType > Traits;
  typedef typename Traits::derived_type   ThisType;
  typedef typename Traits::ContainerType  ContainerType;
  typedef typename Traits::ComponentType  ComponentType;
  typedef typename Traits::SourceType     SourceType;
  typedef typename Traits::RangeType      RangeType;
  typedef typename Traits::ScalarType     ScalarType;
  typedef typename Traits::FrozenType     FrozenType;
  typedef typename Traits::InverseType    InverseType;
  typedef LA::AffinelyDecomposedConstContainer< ContainerType > AffinelyDecomposedContainerType;

  LinearAffinelyDecomposedContainerBased(const AffinelyDecomposedContainerType affinelyDecomposedContainer)
    : BaseType(affinelyDecomposedContainer)
    , affinelyDecomposedContainer_(affinelyDecomposedContainer)
  {
    if (!affinelyDecomposedContainer_.has_affine_part() && affinelyDecomposedContainer_.num_components() == 0)
      DUNE_PYMOR_THROW(Exception::this_does_not_make_any_sense, "affinelyDecomposedContainer must not be empty!");
    if (affinelyDecomposedContainer_.has_affine_part()) {
      dim_source_ = affinelyDecomposedContainer_.affine_part()->dim_source();
      dim_range_ = affinelyDecomposedContainer_.affine_part()->dim_range();
    } else {
      dim_source_ = affinelyDecomposedContainer_.component(0)->dim_source();
      dim_range_ = affinelyDecomposedContainer_.component(0)->dim_range();
    }
  }

  unsigned int num_components() const
  {
    return affinelyDecomposedContainer_.num_components();
  }

  ComponentType component(const int qq) const
  {
    return ComponentType(affinelyDecomposedContainer_.component(qq));
  }

  ParameterFunctional coefficient(const int qq) const
  {
    return ParameterFunctional(*(affinelyDecomposedContainer_.coefficient(qq)));
  }

  bool has_affine_part() const
  {
    return affinelyDecomposedContainer_.has_affine_part();
  }

  ComponentType affine_part() const
  {
    return ComponentType(affinelyDecomposedContainer_.affine_part());
  }

  bool linear() const
  {
    return true;
  }

  unsigned int dim_source() const
  {
    return dim_source_;
  }

  unsigned int dim_range() const
  {
    return dim_range_;
  }

  void apply(const SourceType& source, RangeType& range, const Parameter mu = Parameter()) const
    throw (Exception::sizes_do_not_match,
           Exception::wrong_parameter_type,
           Exception::requirements_not_met,
           Exception::linear_solver_failed,
           Exception::this_does_not_make_any_sense)
  {
    if (mu.type() != Parametric::parameter_type())
      DUNE_PYMOR_THROW(Exception::wrong_parameter_type, "the type of mu (" << mu.type()
                       << ") does not match the parameter_type of this (" << Parametric::parameter_type() << ")!");
    if (!Parametric::parametric())
      ComponentType(affinelyDecomposedContainer_.affine_part()).apply(source, range);
    else
      freeze_parameter(mu).apply(source, range);
  }

  using BaseType::apply;

  static std::vector< std::string > invert_options()
  {
    return ComponentType::invert_options();
  }

  InverseType invert(const std::string option = invert_options()[0], const Parameter mu = Parameter()) const
  {
    return freeze_parameter(mu).invert(option);
  }

  FrozenType freeze_parameter(const Parameter mu = Parameter()) const
    throw (Exception::this_is_not_parametric, Exception::wrong_parameter_type)
  {
    if (!Parametric::parametric())
      DUNE_PYMOR_THROW(Exception::this_is_not_parametric, "do not call freeze_parameter(" << mu << ")"
                       << "if parametric() == false!");
    if (mu.type() != Parametric::parameter_type())
      DUNE_PYMOR_THROW(Exception::wrong_parameter_type, "the type of mu (" << mu.type()
                       << ") does not match the parameter_type of this (" << Parametric::parameter_type() << ")!");
    return FrozenType(new ContainerType(affinelyDecomposedContainer_.freeze_parameter(mu)));
  }

private:
  AffinelyDecomposedContainerType affinelyDecomposedContainer_;
  unsigned int dim_source_;
  unsigned int dim_range_;
}; // class LinearAffinelyDecomposedContainerBased


} // namespace Operators
} // namespace Pymor
} // namespace Dune

#endif // DUNE_PYMOR_OPERATORS_AFFINE_HH