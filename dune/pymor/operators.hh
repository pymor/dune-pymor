// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_OPERATORS_HH
#define DUNE_PYMOR_OPERATORS_HH

#include <string>
#include <vector>

#include "operators/dunedynamicmatrix.hh"
#if HAVE_EIGEN
  #include "operators/eigen.hh"
#endif

namespace Dune {
namespace Pymor {


Operators::DuneDynamicMatrix* createLinearOperator(const Operators::DuneDynamicMatrix&,
                                                   const int dim,
                                                   const bool identity = true);


#if HAVE_EIGEN
Operators::EigenDenseMatrix* createLinearOperator(const Operators::EigenDenseMatrix&,
                                                  const int dim,
                                                  const bool identity = true);


Operators::EigenRowMajorSparseMatrix* createLinearOperator(const Operators::EigenRowMajorSparseMatrix&,
                                                           const int dim,
                                                           const bool identity = true);
#endif // HAVE_EIGEN


} // namespace Pymor
} // namespace Dune

#endif // DUNE_PYMOR_OPERATORS_HH
