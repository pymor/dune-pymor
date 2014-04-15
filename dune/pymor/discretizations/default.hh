// This file is part of the dune-pymor project:
//   https://github.com/pymor/dune-pymor
// Copyright Holders: Stephan Rave, Felix Schindler
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_DISCRETIZATIONS_DEFAULT_HH
#define DUNE_PYMOR_DISCRETIZATIONS_DEFAULT_HH

#include <map>

#include "interfaces.hh"

namespace Dune {
namespace Pymor {
namespace StationaryDiscretization {


template< class Traits >
class CachingDefault
  : public StationaryDiscretizationInterface< Traits >
{
  typedef StationaryDiscretizationInterface< Traits > BaseType;
public:
  typedef typename Traits::derived_type derived_type;
  typedef typename Traits::VectorType   VectorType;

  CachingDefault(const ParameterType tt = ParameterType())
    : BaseType(tt)
  {}

  CachingDefault(const Parametric& other)
    : BaseType(other)
  {}

  void solve(VectorType& vector, const Parameter mu = Parameter()) const
  {
    const auto search_result = cache_.find(mu);
    if (search_result == cache_.end()) {
      uncached_solve(vector, mu);
      cache_.insert(std::make_pair(mu, std::shared_ptr< VectorType >(new VectorType(vector.copy()))));
    } else {
      const auto& result = *(search_result->second);
      vector = result;
    }
  } // ... solve(...)

protected:
  void uncached_solve(VectorType& vector, const Parameter mu = Parameter()) const
  {
    CHECK_AND_CALL_INTERFACE_IMPLEMENTATION(this->as_imp(*this).uncached_solve(vector, mu));
  }

private:
  mutable std::map< Parameter, std::shared_ptr< VectorType > > cache_;
}; // class CachingDefault


} // namespace StationaryDiscretization
} // namespace Pymor
} // namespace Dune

#endif // DUNE_PYMOR_DISCRETIZATIONS_DEFAULT_HH
