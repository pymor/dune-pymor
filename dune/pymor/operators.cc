// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#include "operators.hh"

namespace Dune {
namespace Pymor {


Operators::DuneDynamic* createLinearOperator(const Operators::DuneDynamic&,
                                             const int dim,
                                             const bool identity)
{
  Operators::DuneDynamic* ret = new Operators::DuneDynamic(dim, dim);
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

