// This file is part of the dune-pymor project:
//   https://github.com/pymor/dune-pymor
// Copyright Holders: Stephan Rave, Felix Schindler
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_PARAMETERS_FUNCTIONAL_HH
#define DUNE_PYMOR_PARAMETERS_FUNCTIONAL_HH

#ifndef DUNE_STUFF_SSIZE_T
# define DUNE_STUFF_SSIZE_T long int
#endif

#include <vector>
#include <memory>

#include <dune/stuff/functions/expression/mathexpr.hh>

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
  ParameterFunctional(const ParameterType& tt, const std::string& exp);

  ParameterFunctional(const std::string& kk, const DUNE_STUFF_SSIZE_T & vv, const std::string& exp);

  ParameterFunctional(const std::vector< std::string >& kk,
                      const std::vector< DUNE_STUFF_SSIZE_T >& vv,
                      const std::string& exp);

  ParameterFunctional(const ParameterFunctional& other);

  ~ParameterFunctional();

  ParameterFunctional& operator=(const ParameterFunctional& other);

  const std::string& expression() const;

  std::string report(const std::string name = "ParameterFunctional") const;

  void evaluate(const Parameter& mu, double& ret) const;

  double evaluate(const Parameter& mu) const;

private:
  void setup();

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
