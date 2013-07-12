// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#include "functional.hh"

namespace Dune {
namespace Pymor {


ParameterFunctional::ParameterFunctional(const ParameterType& tt, const std::string& exp)
  : Parametric(tt)
  , expression_(exp)
{
  setup();
}

ParameterFunctional::~ParameterFunctional()
{
  cleanup();
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
  throw (Exception::wrong_parameter_type)
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
}

double ParameterFunctional::evaluate(const Parameter& mu) const throw (Exception::wrong_parameter_type)
{
  double ret = 0.0;
  evaluate(mu, ret);
  return ret;
}

void ParameterFunctional::setup() throw (Exception::sizes_do_not_match)
{
  // create variables from parameter type
  const ParameterType& type = parameter_type();
  for (auto variable_prefix : type.keys()) {
    const size_t variable_size = type.get(variable_prefix);
    if (variable_size == 1)
      variables_.push_back(variable_prefix);
    else {
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