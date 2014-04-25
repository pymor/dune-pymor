// This file is part of the dune-pymor project:
//   https://github.com/pymor/dune-pymor
// Copyright holders: Stephan Rave, Felix Schindler
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#include "config.h"

#if HAVE_EIGEN

#include "eigen.hh"
#include "affine.hh"

namespace Dune {
namespace Pymor {
namespace Operators {


template class EigenRowMajorSparseInverse< double >;
template class EigenRowMajorSparse< double >;
template class LinearAffinelyDecomposedContainerBased< Stuff::LA::EigenDenseMatrix< double >,
                                                       Stuff::LA::EigenDenseVector< double > >;
template class LinearAffinelyDecomposedContainerBased< Stuff::LA::EigenDenseMatrix< double >,
                                                       Stuff::LA::EigenMappedDenseVector< double > >;
template class LinearAffinelyDecomposedContainerBased< Stuff::LA::EigenRowMajorSparseMatrix< double >,
                                                       Stuff::LA::EigenDenseVector< double > >;
template class LinearAffinelyDecomposedContainerBased< Stuff::LA::EigenRowMajorSparseMatrix< double >,
                                                       Stuff::LA::EigenMappedDenseVector< double > >;


} // namespace Operators
} // namespace Pymor
} // namespace Dune

#endif // HAVE_EIGEN
