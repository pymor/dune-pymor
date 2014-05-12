// This file is part of the dune-pymor project:
//   https://github.com/pymor/dune-pymor
// Copyright holders: Stephan Rave, Felix Schindler
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#include "config.h"

#include <limits>
#include <sstream>

#include <dune/stuff/common/print.hh>
#include <dune/stuff/common/exceptions.hh>

#include "functional.hh"

namespace Dune {
namespace Pymor {


ParameterFunctional::ParameterFunctional(const ParameterType& tt, const std::string& exp)
  : Parametric(tt)
  , expression_(exp)
  , actual_size_(DUNE_PYMOR_PARAMETERS_FUNCTIONAL_MAX_SIZE)
{
  setup();
}

ParameterFunctional::ParameterFunctional(const std::string& kk,
                                         const DUNE_STUFF_SSIZE_T & vv,
                                         const std::string& exp)
  : Parametric(ParameterType(kk, vv))
  , expression_(exp)
  , actual_size_(DUNE_PYMOR_PARAMETERS_FUNCTIONAL_MAX_SIZE)
{
  setup();
}

ParameterFunctional::ParameterFunctional(const std::vector< std::string >& kk,
                                         const std::vector< DUNE_STUFF_SSIZE_T >& vv,
                                         const std::string& exp)
  : Parametric(ParameterType(kk, vv))
  , expression_(exp)
  , actual_size_(DUNE_PYMOR_PARAMETERS_FUNCTIONAL_MAX_SIZE)
{
  setup();
}

ParameterFunctional::ParameterFunctional(const ParameterFunctional& other)
  : Parametric(other.parameter_type())
  , expression_(other.expression_)
  , actual_size_(DUNE_PYMOR_PARAMETERS_FUNCTIONAL_MAX_SIZE)
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
  assert(serialized_mu.size() == actual_size_);
  for (size_t ii = 0; ii < actual_size_; ++ii)
    *(arg_[ii]) = serialized_mu[ii];
  // copy ret
  ret = op_->Val();
  // perform sanity check
  if (std::abs(ret) > (0.9 * std::numeric_limits< double >::max())) {
    std::stringstream ss;
    for (size_t ii = 0; ii < actual_size_; ++ii)
      ss << "  " << variables_[ii] << std::endl;
    DUNE_THROW_COLORFULLY(Stuff::Exceptions::internal_error,
                          "evaluating this functional yielded an unlikely value!\n"
                          << "The parameter_type() of this functional is:\n  " << parameter_type() << "\n"
                          << "The variables of this functional are:\n" << ss.str()
                          << "The expression of this functional is:\n  " << expression_ << "\n"
                          << "You tried to evaluate it with:\n  mu = " << mu << "\n"
                          << "The result was:\n  " << ret);
  }
} // ... evaluate(...)

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
      if (expression_.find(variable_prefix + "[") != std::string::npos)
        DUNE_THROW_COLORFULLY(Stuff::Exceptions::wrong_input_given,
                              "There was a problem setting up this parameter functional:\n"
                              << "At least one part of the parameter is scalar and the expression you gave indicates"
                              << " that you expect it to be vector valued!\n"
                              << "The parameter_type you provided is:\n  " << parameter_type() << "\n"
                              << "The expression you provided is:\n  " << expression_ << "\n");
      variables_.push_back(variable_prefix);
    } else {
      for (size_t ii = 0; ii < variable_size; ++ii) {
        std::stringstream ss;
        ss << variable_prefix << "[" << ii << "]";
        variables_.push_back(ss.str());
      }
    }
  }
  actual_size_ = variables_.size();
  if (actual_size_ > DUNE_PYMOR_PARAMETERS_FUNCTIONAL_MAX_SIZE)
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "the given ParameterType requires " << variables_.size()
                     << " variables, but DUNE_PYMOR_PARAMETERS_FUNCTIONAL_MAX_SIZE is "
                     << DUNE_PYMOR_PARAMETERS_FUNCTIONAL_MAX_SIZE
                     << "! Recompile with larger DUNE_PYMOR_PARAMETERS_FUNCTIONAL_MAX_SIZE!");
  for (size_t ii = variables_.size(); ii < DUNE_PYMOR_PARAMETERS_FUNCTIONAL_MAX_SIZE; ++ii)
    variables_.push_back("this_is_a_dummy_expression");
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
