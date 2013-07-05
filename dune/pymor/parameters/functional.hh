// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_PARAMETERS_FUNCTIONAL_HH
#define DUNE_PYMOR_PARAMETERS_FUNCTIONAL_HH

#include <vector>

#include <dune/stuff/function/expression/base.hh>

#define DUNE_PYMOR_PARAMETERS_FUNCTIONAL_MAX_SIZE 64

namespace Dune {
namespace Pymor {


class ParameterFunctional
  : private Dune::Stuff::FunctionExpressionBase< double, DUNE_PYMOR_PARAMETERS_FUNCTIONAL_MAX_SIZE, double, 1 >
{
  typedef Dune::Stuff::FunctionExpressionBase< double, DUNE_PYMOR_PARAMETERS_FUNCTIONAL_MAX_SIZE, double, 1 > BaseType;
public:
  typedef ParameterFunctional                                                                                 ThisType;

  ParameterFunctional(const std::string& _expression)
    : BaseType("mu", _expression)
  {}

  std::string expression() const
  {
    assert(BaseType::expression().size() == 1);
    return BaseType::expression()[0];
  }

  operator std::string() const
  {
    return expression();
  }

  void evaluate(const std::vector< double >& mu, RangeFieldType& ret) const
  {
    BaseType::evaluate(mu, ret);
  }

  double evaluate(const std::vector< double >& mu) const
  {
    double ret;
    BaseType::evaluate(mu, ret);
    return ret;
  }
}; // class ParameterFunctional


} // namespace Pymor
} // namespace Dune

#endif // DUNE_PYMOR_PARAMETERS_FUNCTIONAL_HH
