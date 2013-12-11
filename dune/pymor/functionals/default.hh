// This file is part of the dune-pymor project:
//   https://github.com/pymor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_FUNCTIONALS_DEFAULT_HH
#define DUNE_PYMOR_FUNCTIONALS_DEFAULT_HH

#ifndef DUNE_STUFF_SSIZE_T
# define DUNE_STUFF_SSIZE_T long int
#endif

#include <dune/common/typetraits.hh>
#include <dune/stuff/la/container/interfaces.hh>

#include <dune/pymor/parameters/functional.hh>
#include "interfaces.hh"

namespace Dune {
namespace Pymor {
namespace Functionals {


template< class VectorImp >
class VectorBased;


template< class VectorImp >
class VectorBasedTraits
{
public:
  typedef VectorBased< VectorImp >        derived_type;
  typedef VectorImp                       ContainerType;
  typedef VectorImp                       SourceType;
  typedef derived_type                    FrozenType;
  typedef typename SourceType::ScalarType ScalarType;
  static_assert(std::is_base_of< Dune::Stuff::LA::VectorInterface< typename VectorImp::Traits >, ContainerType >::value,
                "VectorImp must be derived from Dune::Pymor::LA::VectorInterface!");
};


template< class VectorImp >
class VectorBased
  : public FunctionalInterface< VectorBasedTraits< VectorImp > >
  , public Stuff::LA::ProvidesContainer< VectorBasedTraits< VectorImp > >
{
public:
  typedef VectorBasedTraits< VectorImp >  Traits;
  typedef typename Traits::derived_type   ThisType;
  typedef typename Traits::ContainerType  ContainerType;
  typedef typename Traits::SourceType     SourceType;
  typedef typename Traits::ScalarType     ScalarType;
  typedef typename Traits::FrozenType     FrozenType;

  /**
   * \attention This class takes ownership of vector_ptr (in the sense, that you must not delete it manually)!
   */
  VectorBased(const ContainerType* vector_ptr)
    : vector_(vector_ptr)
  {}

  VectorBased(const std::shared_ptr< const ContainerType > vector_ptr)
    : vector_(vector_ptr)
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
    if (!mu.empty()) DUNE_PYMOR_THROW(Exception::this_is_not_parametric,
                                      "mu has to be empty if parametric() == false (is " << mu << ")!");
    if (source.dim() != dim_source())
      DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                       "the dim of source (" << source.dim() << ") does not match the dim_source of this ("
                       << dim_source() << ")!");
    return vector_->dot(source);
  }

  FrozenType freeze_parameter(const Parameter mu = Parameter()) const
  {
    DUNE_PYMOR_THROW(Exception::this_is_not_parametric, "do not call freeze_parameter(" << mu << ")"
                     << "if parametric() == false!");
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
}; // class VectorBased


} // namespace Functionals
} // namespace Pymor
} // namespace Dune

#endif // DUNE_PYMOR_FUNCTIONALS_DEFAULT_HH
