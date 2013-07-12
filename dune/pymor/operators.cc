// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifdef HAVE_CMAKE_CONFIG
  #include "cmake_config.h"
#elif defined (HAVE_CONFIG_H)
  #include "config.h"
#endif // HAVE_CMAKE_CONFIG

#include "operators.hh"

namespace Dune {
namespace Pymor {


Operators::DuneDynamicMatrix* createLinearOperator(const Operators::DuneDynamicMatrix&,
                                                   const int dim,
                                                   const bool identity)
{
  Operators::DuneDynamicMatrix* ret = new Operators::DuneDynamicMatrix(dim, dim);
  if (identity)
    for (int ii = 0; ii < dim; ++ii)
      ret->operator[](ii)[ii] = 1.0;
  return ret;
}


#if HAVE_EIGEN
Operators::EigenDenseMatrix* createLinearOperator(const Operators::EigenDenseMatrix&,
                                                  const int dim,
                                                  const bool identity)
{
  Operators::EigenDenseMatrix* ret = new Operators::EigenDenseMatrix(dim, dim);
  if (identity)
    for (int ii = 0; ii < dim; ++ii)
      ret->set(ii, ii, 1.0);
  return ret;
}

Operators::EigenRowMajorSparseMatrix* createLinearOperator(const Operators::EigenRowMajorSparseMatrix&,
                                                           const int dim,
                                                           const bool identity)
{
  Operators::EigenRowMajorSparseMatrix* ret
      = new Operators::EigenRowMajorSparseMatrix(*(Dune::Stuff::LA::createIdentityEigenRowMajorSparseMatrix(dim)));
  if (!identity)
    for (int ii = 0; ii < dim; ++ii)
      ret->set(ii, ii, 0.0);
  return ret;
}
#endif // HAVE_EIGEN


} // namespace Pymor
} // namespace Dune

