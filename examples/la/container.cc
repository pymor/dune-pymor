// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#include "container.hh"

Dune::Pymor::LA::DuneDynamicVector* createDuneDynamicVector(const int ss)
{
  return Dune::Pymor::LA::DuneDynamicVector::create(ss);
}

#if HAVE_EIGEN
Dune::Pymor::LA::EigenDenseVector* createEigenDenseVector(const int ss)
{
  return Dune::Pymor::LA::EigenDenseVector::create(ss);
}
#endif
