// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_EXAMPLES_LA_CONTAINER_HH
#define DUNE_PYMOR_EXAMPLES_LA_CONTAINER_HH

#ifdef HAVE_CMAKE_CONFIG
  #include "cmake_config.h"
#elif defined (HAVE_CONFIG_H)
  #include "config.h"
#endif

#include <dune/pymor/la/container/dunedynamic.hh>
#if HAVE_EIGEN
  #include <dune/pymor/la/container/eigen.hh>
#endif

using namespace Dune::Pymor::LA;

DuneDynamicVector* createDuneDynamicVector(const int ss);

#if HAVE_EIGEN
EigenDenseVector* createEigenDenseVector(const int ss);
#endif

#endif // DUNE_PYMOR_EXAMPLES_LA_CONTAINER_HH
