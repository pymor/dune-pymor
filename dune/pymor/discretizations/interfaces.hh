// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_DISCRETIZATIONS_INTERFACES_HH
#define DUNE_PYMOR_DISCRETIZATIONS_INTERFACES_HH

#include <vector>
#include <string>

#include <dune/pymor/parameters/base.hh>
#include <dune/pymor/common/exceptions.hh>
#include <dune/pymor/la/container/interfaces.hh>
#include <dune/pymor/operators/interfaces.hh>

namespace Dune {
namespace Pymor {


class StationaryDiscretizationInterface
  : public Parametric
{
public:
  StationaryDiscretizationInterface();

  StationaryDiscretizationInterface(const ParameterType& tt);

  StationaryDiscretizationInterface(const std::string& kk, const int& vv) throw (Exception::key_is_not_valid,
                                                                                 Exception::index_out_of_range);

  StationaryDiscretizationInterface(const std::vector< std::string >& kk,
                                    const std::vector< int >& vv) throw (Exception::key_is_not_valid,
                                                                         Exception::index_out_of_range,
                                                                         Exception::sizes_do_not_match);

  StationaryDiscretizationInterface(const Parametric& pp);

  virtual ~StationaryDiscretizationInterface();

  virtual std::vector< std::string > available_operators() const = 0;

  virtual const OperatorInterface* get_operator(const std::string id) const = 0;

  virtual LA::VectorInterface* create_vector() const = 0;

  virtual std::vector< std::string > solver_options() const = 0;

  virtual std::string solver_options(const std::string context) const = 0;

  virtual void solve(LA::VectorInterface* vector, const Parameter mu = Parameter()) const = 0;

  virtual void visualize(const LA::VectorInterface* vector,
                         const std::string filename,
                         const std::string name) const = 0;
}; // class StationaryDiscretizationInterface


} // namespace Pymor
} // namespace Dune

#endif // DUNE_PYMOR_DISCRETIZATIONS_INTERFACES_HH
