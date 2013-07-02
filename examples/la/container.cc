// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#include "container.hh"

DuneDynamicVector* createDuneDynamicVector(const int ss)
{
  return DuneDynamicVector::create(ss);
}

#if HAVE_EIGEN
EigenDenseVector* createEigenDenseVector(const int ss)
{
  return EigenDenseVector::create(ss);
}
#endif
