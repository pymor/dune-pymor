// This file is part of the dune-pymor project:
//   https://github.com/pymor/dune-pymor
// Copyright holders: Stephan Rave, Felix Schindler
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_OPERATORS_AFFINE_HH
#define DUNE_PYMOR_OPERATORS_AFFINE_HH

#include <type_traits>

#include <dune/stuff/la/container/interfaces.hh>

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
  static_assert(std::is_base_of< Stuff::LA::ProvidesConstContainer< typename OperatorType::Traits >, OperatorType > ::value,
                "OperatorType has to be derived from Stuff::LA::ProvidesContainer!");
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
      dim_source_ = affinelyDecomposedContainer_.affine_part()->cols();
      dim_range_ = affinelyDecomposedContainer_.affine_part()->rows();
    } else {
      dim_source_ = affinelyDecomposedContainer_.component(0)->cols();
      dim_range_ = affinelyDecomposedContainer_.component(0)->rows();
    }
  }

  DUNE_STUFF_SSIZE_T num_components() const
  {
    return affinelyDecomposedContainer_.num_components();
  }

  ComponentType component(const DUNE_STUFF_SSIZE_T qq) const
  {
    return ComponentType(affinelyDecomposedContainer_.component(qq));
  }

  ParameterFunctional coefficient(const DUNE_STUFF_SSIZE_T qq) const
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

  DUNE_STUFF_SSIZE_T dim_source() const
  {
    return dim_source_;
  }

  DUNE_STUFF_SSIZE_T dim_range() const
  {
    return dim_range_;
  }

  void apply(const SourceType& source, RangeType& range, const Parameter mu = Parameter()) const
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

  static Stuff::Common::ConfigTree invert_options(const std::string& type)
  {
    return ComponentType::invert_options(type);
  }

  InverseType invert(const Stuff::Common::ConfigTree& option, const Parameter mu = Parameter()) const
  {
    return freeze_parameter(mu).invert(option);
  }

  FrozenType freeze_parameter(const Parameter mu = Parameter()) const
  {
    if (!Parametric::parametric())
      DUNE_PYMOR_THROW(Exception::this_is_not_parametric, "do not call freeze_parameter(" << mu << ")"
                       << " if parametric() == false!");
    if (mu.type() != Parametric::parameter_type())
      DUNE_PYMOR_THROW(Exception::wrong_parameter_type, "the type of mu (" << mu.type()
                       << ") does not match the parameter_type of this (" << Parametric::parameter_type() << ")!");
    return FrozenType(new ContainerType(affinelyDecomposedContainer_.freeze_parameter(mu)));
  }

private:
  AffinelyDecomposedContainerType affinelyDecomposedContainer_;
  DUNE_STUFF_SSIZE_T dim_source_;
  DUNE_STUFF_SSIZE_T dim_range_;
}; // class LinearAffinelyDecomposedContainerBased


} // namespace Operators
} // namespace Pymor
} // namespace Dune

#endif // DUNE_PYMOR_OPERATORS_AFFINE_HH
