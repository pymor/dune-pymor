// This file is part of the dune-pymor project:
//   https://github.com/pymor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#include <dune/stuff/test/test_common.hh>
#include <dune/stuff/common/float_cmp.hh>

#include <dune/pymor/common/exceptions.hh>
#include <dune/pymor/parameters/functional.hh>

using namespace Dune;
using namespace Dune::Pymor;

TEST(Functional, Parameters_Functional)
{
  const Parameter mu1 = {"diffusion", {1.0, 0.0, 0.0}};
  const ParameterFunctional theta1(mu1.type(), "diffusion[0] + sin(diffusion[1]) + cos(diffusion[2])");
  std::string ss = theta1.report("theta1");
//  std::cout << ss << std::endl;
  double res = theta1.evaluate(mu1);
  if (!Dune::FloatCmp::eq(res, 2.0)) DUNE_PYMOR_THROW(PymorException, "");
  std::string exp = theta1.expression();
  if (exp != "diffusion[0] + sin(diffusion[1]) + cos(diffusion[2])") DUNE_PYMOR_THROW(PymorException, "");

  const Parameter mu2 = {{"diffusion", "force"}, {{1.0}, {0.0, 0.0}}};
  const ParameterFunctional theta2(mu2.type(), "diffusion + sin(force[0]) + exp(force[1])");
  ss = theta2.report("theta2");
//  std::cout << ss << std::endl;
  res = theta2.evaluate(mu2);
  if (!Dune::FloatCmp::eq(res, 2.0)) DUNE_PYMOR_THROW(PymorException, "");
  exp = theta2.expression();
  if (exp != "diffusion + sin(force[0]) + exp(force[1])") DUNE_PYMOR_THROW(PymorException, "");
}

int main(int argc, char** argv)
{
  try {
    test_init(argc, argv);
    return RUN_ALL_TESTS();
  } catch (Dune::PymorException &e) {
    std::cerr << Dune::Stuff::Common::colorStringRed("dune-pymor reported: ") << e << std::endl;
  } catch (Dune::Exception &e) {
    std::cerr << Dune::Stuff::Common::colorStringRed("Dune reported error: ") << e << std::endl;
  } catch (...) {
    std::cerr << Dune::Stuff::Common::colorStringRed("Unknown exception thrown!") << std::endl;
  }
}
