// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifdef HAVE_CMAKE_CONFIG
  #include "cmake_config.h"
#elif defined (HAVE_CONFIG_H)
  #include "config.h"
#endif // HAVE_CMAKE_CONFIG

#include <limits>
#include <sstream>

#include <dune/stuff/common/print.hh>

#include "functional.hh"

namespace Dune {
namespace Pymor {


ParameterFunctional::ParameterFunctional(const ParameterType& tt, const std::string& exp)
  : Parametric(tt)
  , expression_(exp)
{
  setup();
}

ParameterFunctional::ParameterFunctional(const std::string& kk, const int& vv, const std::string& exp)
  : Parametric(ParameterType(kk, vv))
  , expression_(exp)
{
  setup();
}

ParameterFunctional::ParameterFunctional(const std::vector< std::string >& kk,
                                         const std::vector< int >& vv,
                                         const std::string& exp)
  : Parametric(ParameterType(kk, vv))
  , expression_(exp)
{
  setup();
}

ParameterFunctional::ParameterFunctional(const ParameterFunctional& other)
  : Parametric(other.parameter_type())
  , expression_(other.expression_)
{
  setup();
}

ParameterFunctional::~ParameterFunctional()
{
  cleanup();
}

ParameterFunctional& ParameterFunctional::operator=(const ParameterFunctional& other)
{
  if (this != &other) {
    cleanup();
    replace_parameter_type(other.parameter_type());
    expression_ = other.expression_;
    setup();
  }
  return *this;
}

const std::string& ParameterFunctional::expression() const
{
  return expression_;
}

std::string ParameterFunctional::report(const std::string name) const
{
  return name + "(" + Parametric::parameter_type().report() + ") = \"" + expression_ + "\"";
}

void ParameterFunctional::evaluate(const Parameter& mu, double& ret) const
{
  if (mu.type() != parameter_type())
    DUNE_PYMOR_THROW(Exception::wrong_parameter_type,
                     "the type of mu (" << mu.type().report() << ") does not match the parameter_type of this ("
                     << parameter_type().report() << ")!");
  // parse argument
  const auto serialized_mu = mu.serialize();
  for (size_t ii = 0; ii < serialized_mu.size(); ++ii)
    *(arg_[ii]) = serialized_mu[ii];
  // copy ret
  ret = op_->Val();
  // perform sanity check
  if (std::abs(ret) > (0.9 * std::numeric_limits< double >::max())) {
    std::stringstream ss;
    Stuff::Common::print(variables_, "the variables of this functional thus are", ss);
    DUNE_PYMOR_THROW(Exception::this_does_not_make_any_sense,
                     "evaluating this functional yielded an unlikely value!\n"
                     << "The parameter_type() of this functional is: " << parameter_type() << "\n,"
                     << ss.str() << ",\n"
                     << "you tried to evaluate it with mu = " << mu << ",\n"
                     << "and the result was " << ret << "!");
  }
}

double ParameterFunctional::evaluate(const Parameter& mu) const
{
  double ret = 0.0;
  evaluate(mu, ret);
  return ret;
}

void ParameterFunctional::setup()
{
  // create variables from parameter type
  const ParameterType& type = parameter_type();
  for (auto variable_prefix : type.keys()) {
    const size_t variable_size = type.get(variable_prefix);
    if (variable_size == 1) {
      variables_.push_back(variable_prefix);
      variables_.push_back(variable_prefix + "[0]");
    } else {
      for (size_t ii = 0; ii < variable_size; ++ii) {
        std::stringstream ss;
        ss << variable_prefix << "[" << ii << "]";
        variables_.push_back(ss.str());
      }
    }
  }
  if (variables_.size() > DUNE_PYMOR_PARAMETERS_FUNCTIONAL_MAX_SIZE)
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "the given ParameterType requires " << variables_.size()
                     << " variables, but DUNE_PYMOR_PARAMETERS_FUNCTIONAL_MAX_SIZE is "
                     << DUNE_PYMOR_PARAMETERS_FUNCTIONAL_MAX_SIZE
                     << "! Recompile with larger DUNE_PYMOR_PARAMETERS_FUNCTIONAL_MAX_SIZE!");
  for (size_t ii = variables_.size(); ii < DUNE_PYMOR_PARAMETERS_FUNCTIONAL_MAX_SIZE; ++ii) {
    std::stringstream ss;
    ss << "dummy_expression[" << variables_.size() - ii << "]";
    variables_.push_back(ss.str());
  }
  assert(variables_.size() == DUNE_PYMOR_PARAMETERS_FUNCTIONAL_MAX_SIZE && "This should not happen!");
  // create epressions
  for (size_t ii = 0; ii < variables_.size(); ++ii) {
    arg_[ii] = new double(0.0);
    var_arg_[ii] = new RVar(variables_[ii].c_str(), arg_[ii]);
    vararray_[ii] = var_arg_[ii];
  }
  // build operation
  op_ = new ROperation(expression_.c_str(), DUNE_PYMOR_PARAMETERS_FUNCTIONAL_MAX_SIZE, vararray_);
} // void setup(const std::string& _variable, const std::vector< std::string >& expressions)

void ParameterFunctional::cleanup()
{
  delete op_;
  for (size_t ii = 0; ii < DUNE_PYMOR_PARAMETERS_FUNCTIONAL_MAX_SIZE; ++ii) {
    delete var_arg_[ii];
    delete arg_[ii];
  }
} // void cleanup()


} // namespace Pymor
} // namespace Dune
