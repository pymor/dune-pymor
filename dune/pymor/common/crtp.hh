// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_COMMON_CRTP_HH
#define DUNE_PYMOR_COMMON_CRTP_HH

#include <type_traits>
#include <dune/common/bartonnackmanifcheck.hh>

namespace Dune {
namespace Pymor {


template< class InterfaceType, class Traits >
class CRTPInterface
{
protected:
  typedef typename Traits::derived_type derived_type;

  static derived_type& as_imp(InterfaceType& that)
  {
    return static_cast< derived_type& >(that);
  }

   const derived_type& as_imp(const InterfaceType& that) const
  {
    return static_cast< const derived_type& >(that);
  }
}; // CRTPInterface


} // namespace Pymor
} // namespace Dune

#endif // DUNE_PYMOR_COMMON_CRTP_HH
