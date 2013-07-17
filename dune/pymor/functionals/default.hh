// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_FUNCTIONALS_DEFAULT_HH
#define DUNE_PYMOR_FUNCTIONALS_DEFAULT_HH

#include <dune/common/typetraits.hh>

#include <dune/pymor/parameters/functional.hh>
#include <dune/pymor/la/container/interfaces.hh>
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
  typedef VectorImp                       VectorType;
  typedef VectorType                      SourceType;
  typedef derived_type                    FrozenType;
  typedef typename SourceType::ScalarType ScalarType;
  static_assert(std::is_base_of< Dune::Pymor::LA::VectorInterface< typename VectorImp::Traits >, VectorType >::value,
                "VectorType must be derived from Dune::Pymor::LA::VectorInterface!");
};


template< class VectorImp >
class VectorBased
  : public FunctionalInterface< VectorBasedTraits< VectorImp > >
{
public:
  typedef VectorBasedTraits< VectorImp >  Traits;
  typedef typename Traits::derived_type   ThisType;
  typedef typename Traits::VectorType     VectorType;
  typedef typename Traits::SourceType     SourceType;
  typedef typename Traits::ScalarType     ScalarType;
  typedef typename Traits::FrozenType     FrozenType;

  /**
   * \attention This class takes ownership of vector_ptr!
   */
  VectorBased(const VectorType* vector_ptr)
    : vector_(vector_ptr)
  {}

  VectorBased(const std::shared_ptr< const VectorType > vector_ptr)
    : vector_(vector_ptr)
  {}

  bool linear() const
  {
    return true;
  }

  unsigned int dim_source() const
  {
    return vector_->dim();
  }

  ScalarType apply(const SourceType& source, const Parameter mu = Parameter()) const
    throw (Exception::this_is_not_parametric)
  {
    if (!mu.empty()) DUNE_PYMOR_THROW(Exception::this_is_not_parametric,
                                      "mu has to be empty if parametric() == false (is " << mu << ")!");
    return vector_->dot(source);
  }

  FrozenType freeze_parameter(const Parameter mu = Parameter()) const
    throw (Exception::this_is_not_parametric)
  {
    DUNE_PYMOR_THROW(Exception::this_is_not_parametric, "do not call freeze_parameter(" << mu << ")"
                     << "if parametric() == false!");
    return FrozenType(new VectorType());
  }

  std::shared_ptr< const VectorType > vector() const
  {
    return vector_;
  }
private:
  std::shared_ptr< const VectorType > vector_;
}; // class VectorBased


} // namespace Functionals
} // namespace Pymor
} // namespace Dune

#endif // DUNE_PYMOR_FUNCTIONALS_DEFAULT_HH
