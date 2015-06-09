// This file is part of the dune-pymor project:
//   https://github.com/pymor/dune-pymor
// Copyright holders: Stephan Rave, Felix Schindler
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_FUNCTIONALS_DEFAULT_HH
#define DUNE_PYMOR_FUNCTIONALS_DEFAULT_HH

#include <type_traits>

#include <dune/common/typetraits.hh>

#include <dune/stuff/la/container/interfaces.hh>
#include <dune/stuff/la/container/istl.hh>
#include <dune/stuff/la/container/common.hh>
#include <dune/stuff/la/container/eigen.hh>

#include <dune/pymor/parameters/functional.hh>
#include "interfaces.hh"

namespace Dune {
namespace Pymor {
namespace Functionals {


template< class VectorImp, class SpaceImp >
class VectorBased;


template< class VectorImp, class SpaceImp >
class VectorBasedTraits
{
public:
  typedef VectorBased< VectorImp, SpaceImp >        derived_type;
  typedef VectorImp                       ContainerType;
  typedef VectorImp                       SourceType;
  typedef derived_type                    FrozenType;
  typedef typename SourceType::ScalarType ScalarType;
  static_assert(std::is_base_of< Dune::Stuff::LA::VectorInterface< typename VectorImp::Traits >, ContainerType >::value,
                "VectorImp must be derived from Dune::Pymor::LA::VectorInterface!");
};



template< class VectorImp, class SpaceImp >
class VectorBased
  : public FunctionalInterface< VectorBasedTraits< VectorImp, SpaceImp > >
  , public Stuff::LA::ProvidesContainer< VectorBasedTraits< VectorImp, SpaceImp > >
{
public:
  typedef VectorBasedTraits< VectorImp, SpaceImp >  Traits;
  typedef typename Traits::derived_type   ThisType;
  typedef typename Traits::ContainerType  ContainerType;
  typedef typename Traits::SourceType     SourceType;
  typedef typename Traits::ScalarType     ScalarType;
  typedef typename Traits::FrozenType     FrozenType;

  /**
   * \attention This class takes ownership of vector_ptr (in the sense, that you must not delete it manually)!
   */
  VectorBased(const ContainerType* vector_ptr, const SpaceImp& space)
    : vector_(vector_ptr)
    , space_(space)
  {}

  VectorBased(const std::shared_ptr< const ContainerType > vector_ptr, const SpaceImp& space)
    : vector_(vector_ptr)
    , space_(space)
  {}

  bool linear() const
  {
    return true;
  }

  DUNE_STUFF_SSIZE_T dim_source() const
  {
    return vector_->dim();
  }

  ScalarType apply(const SourceType& source, const Parameter mu = Parameter()) const
  {
    if (!mu.empty())
      DUNE_THROW(Exceptions::this_is_not_parametric, "mu has to be empty if parametric() == false (is " << mu << ")!");
    if ((std::make_signed< size_t >::type)(source.dim()) != dim_source())
      DUNE_THROW(Stuff::Exceptions::shapes_do_not_match,
                 "the dim of source (" << source.dim() << ") does not match the dim_source of this (" << dim_source()
                 << ")!");
    return communicated_dot(*vector_, source, space_);
  }

  FrozenType freeze_parameter(const Parameter mu = Parameter()) const
  {
    DUNE_THROW(Exceptions::this_is_not_parametric,
               "do not call freeze_parameter(" << mu << ")" << "if parametric() == false!");
    return FrozenType(new ContainerType());
  }

  std::shared_ptr< const ContainerType > container() const
  {
    return vector_;
  }

  ContainerType* as_vector_and_return_ptr() const
  {
    return new ContainerType(vector_->copy());
  }

private:
  std::shared_ptr< const ContainerType > vector_;
  const SpaceImp& space_;
}; // class VectorBased


} // namespace Functionals
} // namespace Pymor
} // namespace Dune

#endif // DUNE_PYMOR_FUNCTIONALS_DEFAULT_HH
