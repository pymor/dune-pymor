// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_DISCRETIZATIONS_INTERFACES_HH
#define DUNE_PYMOR_DISCRETIZATIONS_INTERFACES_HH

#include <vector>
#include <string>

#include <dune/pymor/common/crtp.hh>
#include <dune/pymor/parameters/base.hh>
#include <dune/pymor/la/container/interfaces.hh>
#include <dune/pymor/operators/interfaces.hh>
#include <dune/pymor/functionals/interfaces.hh>

namespace Dune {
namespace Pymor {


template< class Traits >
class StationaryDiscretizationInterface
  : public Parametric
  , public CRTPInterface< StationaryDiscretizationInterface< Traits >, Traits >
{
  typedef CRTPInterface< StationaryDiscretizationInterface< Traits >, Traits > CRTP;
public:
  typedef typename Traits::derived_type   derived_type;
  typedef typename Traits::OperatorType   OperatorType;
  typedef typename Traits::FunctionalType FunctionalType;
  typedef typename Traits::VectorType     VectorType;

  StationaryDiscretizationInterface(const ParameterType tt = ParameterType())
    : Parametric(tt)
  {}

  StationaryDiscretizationInterface(const Parametric& other)
    : Parametric(other)
  {}

  std::vector< std::string > available_operators() const
  {
    CHECK_INTERFACE_IMPLEMENTATION(CRTP::as_imp(*this).available_operators());
    return CRTP::as_imp(*this).available_operators();
  }

  OperatorType get_operator(const std::string id = available_operators()[0]) const
  {
    CHECK_INTERFACE_IMPLEMENTATION(CRTP::as_imp(*this).get_operator(id));
    return CRTP::as_imp(*this).get_operator(id);
  }

  std::vector< std::string > available_functionals() const
  {
    CHECK_INTERFACE_IMPLEMENTATION(CRTP::as_imp(*this).available_functionals());
    return CRTP::as_imp(*this).available_functionals();
  }

  FunctionalType get_functional(const std::string id = available_functionals()[0]) const
  {
    CHECK_INTERFACE_IMPLEMENTATION(CRTP::as_imp(*this).get_functional(id));
    return CRTP::as_imp(*this).get_functional(id);
  }

  VectorType create_vector() const
  {
    CHECK_INTERFACE_IMPLEMENTATION(CRTP::as_imp(*this).create_vector());
    return CRTP::as_imp(*this).create_vector();
  }

  std::vector< std::string > solver_options() const
  {
    CHECK_INTERFACE_IMPLEMENTATION(CRTP::as_imp(*this).solver_options());
    return CRTP::as_imp(*this).solver_options();
  }

  std::string solver_options(const std::string context) const
  {
    CHECK_INTERFACE_IMPLEMENTATION(CRTP::as_imp(*this).solver_options(context));
    return CRTP::as_imp(*this).solver_options(context);
  }

  void solve(VectorType& vector, const Parameter mu = Parameter()) const
  {
    CHECK_AND_CALL_INTERFACE_IMPLEMENTATION(CRTP::as_imp(*this).solve(vector, mu));
  }

  void visualize(const VectorType& vector, const std::string filename, const std::string name) const
  {
    CHECK_AND_CALL_INTERFACE_IMPLEMENTATION(CRTP::as_imp(*this).visualize(vector, filename, name));
  }
}; // class StationaryDiscretizationInterface


} // namespace Pymor
} // namespace Dune

#endif // DUNE_PYMOR_DISCRETIZATIONS_INTERFACES_HH
