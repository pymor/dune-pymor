// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifdef HAVE_CMAKE_CONFIG
  #include "cmake_config.h"
#elif defined (HAVE_CONFIG_H)
  #include <config.h>
#endif // HAVE_CMAKE_CONFIG

#include <dune/common/float_cmp.hh>

#include <dune/stuff/test/test_common.hh>

#include <dune/pymor/common/exceptions.hh>
#include <dune/pymor/parameters/functional.hh>

using namespace Dune;
using namespace Dune::Pymor;

TEST(Functional, Parameters_Functional)
{
  ParameterFunctional theta1("mu[0] + sin(mu[1]) + cos(mu[2])");
  double res = theta1.evaluate({1.0, 0.0, 0.0});
  if (!Dune::FloatCmp::eq(res, 2.0)) DUNE_PYMOR_THROW(PymorException, "");
  std::string exp = theta1;
  if (exp != "mu[0] + sin(mu[1]) + cos(mu[2])") DUNE_PYMOR_THROW(PymorException, "");
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
