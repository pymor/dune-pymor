// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_PARAMETERS_FUNCTIONAL_HH
#define DUNE_PYMOR_PARAMETERS_FUNCTIONAL_HH

#include <vector>
#include <memory>

#include <dune/stuff/function/expression/mathexpr.hh>

#include "base.hh"

#ifndef DUNE_PYMOR_PARAMETERS_FUNCTIONAL_MAX_SIZE
#define DUNE_PYMOR_PARAMETERS_FUNCTIONAL_MAX_SIZE 64
#endif

namespace Dune {
namespace Pymor {


class ParameterFunctional
  : public Parametric
{
public:
  ParameterFunctional(const ParameterType& tt, const std::string& exp) throw (Exception::sizes_do_not_match);

  ParameterFunctional(const ParameterFunctional& other) throw (Exception::sizes_do_not_match);

  ~ParameterFunctional();

  ParameterFunctional& operator=(const ParameterFunctional& other) throw (Exception::sizes_do_not_match);

  const std::string& expression() const;

  std::string report(const std::string name = "ParameterFunctional") const;

  void evaluate(const Parameter& mu, double& ret) const throw (Exception::wrong_parameter_type);

  double evaluate(const Parameter& mu) const throw (Exception::wrong_parameter_type);

private:
  void setup() throw (Exception::sizes_do_not_match);

  void cleanup();

  std::string expression_;
  std::vector< std::string > variables_;
  mutable double* arg_[DUNE_PYMOR_PARAMETERS_FUNCTIONAL_MAX_SIZE];
  RVar* var_arg_[DUNE_PYMOR_PARAMETERS_FUNCTIONAL_MAX_SIZE];
  RVar* vararray_[DUNE_PYMOR_PARAMETERS_FUNCTIONAL_MAX_SIZE];
  ROperation* op_;
}; // class ParameterFunctional


} // namespace Pymor
} // namespace Dune

#endif // DUNE_PYMOR_PARAMETERS_FUNCTIONAL_HH
