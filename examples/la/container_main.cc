// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#include "container.hh"

#include"dune/common/mpihelper.hh"

using namespace Dune::Pymor;

int main(int argc, char** argv)
{
  try {
    Dune::MPIHelper& DUNE_UNUSED(helper) = Dune::MPIHelper::instance(argc, argv);

    LA::DuneDynamicVector* a = createDuneDynamicVector(2);
    LA::DuneDynamicVector* b = createDuneDynamicVector(3);
    LA::DuneDynamicVector* c = a->add(b);
    delete c;
    delete b;
    delete a;

    return 0;
  } catch (Dune::PymorException &e) {
    std::cerr << Dune::Stuff::Common::colorStringRed("dune-pymor reported: ") << e << std::endl;
  } catch (Dune::Exception &e) {
    std::cerr << Dune::Stuff::Common::colorStringRed("Dune reported error: ") << e << std::endl;
  } catch (...) {
    std::cerr << Dune::Stuff::Common::colorStringRed("Unknown exception thrown!") << std::endl;
  }
}
