// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_FUNCTIONALS_AFFINE_HH
#define DUNE_PYMOR_FUNCTIONALS_AFFINE_HH

#include <dune/common/typetraits.hh>

#include <dune/pymor/parameters/functional.hh>
#include <dune/pymor/la/container/interfaces.hh>
#include <dune/pymor/la/container/affine.hh>

#include "interfaces.hh"
#include "default.hh"

namespace Dune {
namespace Pymor {
namespace Functionals {


template< class VectorImp >
class LinearAffinelyDecomposedVectorBased;


template< class VectorImp >
class LinearAffinelyDecomposedVectorBasedTraits
{
public:
  typedef LinearAffinelyDecomposedVectorBased< VectorImp >  derived_type;
  typedef VectorImp                                         VectorType;
  typedef VectorType                                        SourceType;
  typedef VectorBased< VectorImp >                          ComponentType;
  typedef VectorBased< VectorImp >                          FrozenType;
  typedef typename SourceType::ScalarType                   ScalarType;
  static_assert(std::is_base_of< Dune::Pymor::LA::VectorInterface< typename VectorImp::Traits >, VectorType >::value,
                "VectorType must be derived from Dune::Pymor::LA::VectorInterface!");
};


template< class VectorImp >
class LinearAffinelyDecomposedVectorBased
  : public AffinelyDecomposedFunctionalInterface< LinearAffinelyDecomposedVectorBasedTraits< VectorImp > >
{
  typedef AffinelyDecomposedFunctionalInterface< LinearAffinelyDecomposedVectorBasedTraits< VectorImp > > BaseType;
public:
  typedef LinearAffinelyDecomposedVectorBasedTraits< VectorImp >  Traits;
  typedef typename Traits::VectorType     VectorType;
  typedef typename Traits::SourceType     SourceType;
  typedef typename Traits::ComponentType  ComponentType;
  typedef typename Traits::FrozenType     FrozenType;
  typedef typename Traits::ScalarType     ScalarType;
  typedef typename LA::AffinelyDecomposedConstContainer< VectorType > AffinelyDecomposedVectorType;

  LinearAffinelyDecomposedVectorBased(const AffinelyDecomposedVectorType affinelyDecomposedVector)
    : BaseType(affinelyDecomposedVector)
    , affinelyDecomposedVector_(affinelyDecomposedVector)
  {
    if (!affinelyDecomposedVector_.has_affine_part() && affinelyDecomposedVector_.num_components() == 0)
      DUNE_PYMOR_THROW(Exception::this_does_not_make_any_sense, "affinelyDecomposedVector must not be empty!");
    if (affinelyDecomposedVector_.has_affine_part())
      dim_ = affinelyDecomposedVector_.affine_part()->dim();
    else
      dim_ = affinelyDecomposedVector_.component(0)->dim();
  }

  DUNE_PYMOR_SSIZE_T num_components() const
  {
    return affinelyDecomposedVector_.num_components();
  }

  ComponentType component(const DUNE_PYMOR_SSIZE_T qq) const
  {
    return ComponentType(affinelyDecomposedVector_.component(qq));
  }

  ComponentType* component_and_return_ptr(const DUNE_PYMOR_SSIZE_T qq)
  {
    return new ComponentType(component(qq));
  }

  ParameterFunctional coefficient(const DUNE_PYMOR_SSIZE_T qq) const
  {
    return ParameterFunctional(*(affinelyDecomposedVector_.coefficient(qq)));
  }

  ParameterFunctional* coefficient_and_return_ptr(const DUNE_PYMOR_SSIZE_T qq) const
  {
    return new ParameterFunctional(coefficient(qq));
  }

  bool has_affine_part() const
  {
    return affinelyDecomposedVector_.has_affine_part();
  }

  ComponentType affine_part() const
  {
    return ComponentType(affinelyDecomposedVector_.affine_part());
  }

  ComponentType* affine_part_and_return_ptr()
  {
    return new ComponentType(affine_part());
  }

  bool linear() const
  {
    return true;
  }

  DUNE_PYMOR_SSIZE_T dim_source() const
  {
    return dim_;
  }

  ScalarType apply(const SourceType& source, const Parameter mu = Parameter()) const
  {
    if (mu.type() != Parametric::parameter_type())
      DUNE_PYMOR_THROW(Exception::wrong_parameter_type, "the type of mu (" << mu.type()
                       << ") does not match the parameter_type of this (" << Parametric::parameter_type() << ")!");
    if (!Parametric::parametric())
      return affinelyDecomposedVector_.affine_part()->dot(source);
    else
      return freeze_parameter(mu).apply(source);
  }

  FrozenType freeze_parameter(const Parameter mu = Parameter()) const
  {
    if (!Parametric::parametric())
      DUNE_PYMOR_THROW(Exception::this_is_not_parametric, "do not call freeze_parameter(" << mu << ")"
                       << "if parametric() == false!");
    if (mu.type() != Parametric::parameter_type())
      DUNE_PYMOR_THROW(Exception::wrong_parameter_type, "the type of mu (" << mu.type()
                       << ") does not match the parameter_type of this (" << Parametric::parameter_type() << ")!");
    return FrozenType(new VectorType(affinelyDecomposedVector_.freeze_parameter(mu)));
  }

  FrozenType* freeze_parameter_and_return_ptr(const Parameter mu = Parameter()) const
  {
    return new FrozenType(freeze_parameter(mu));
  }

private:
  const AffinelyDecomposedVectorType affinelyDecomposedVector_;
  unsigned int dim_;
}; // class LinearAffinelyDecomposedVectorBased


} // namespace Functionals
} // namespace Pymor
} // namespace Dune

#endif // DUNE_PYMOR_FUNCTIONALS_AFFINE_HH
