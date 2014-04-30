// This file is part of the dune-pymor project:
//   https://github.com/pymor/dune-pymor
// Copyright holders: Stephan Rave, Felix Schindler
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#include "config.h"

#include <dune/stuff/la/container.hh>

#include "affine.hh"

namespace Dune {
namespace Pymor {
namespace Operators {


template class LinearAffinelyDecomposedContainerBased< Stuff::LA::CommonDenseMatrix< double >,
                                                       Stuff::LA::CommonDenseVector< double > >;

#if HAVE_EIGEN

template class LinearAffinelyDecomposedContainerBased< Stuff::LA::EigenDenseMatrix< double >,
                                                       Stuff::LA::EigenDenseVector< double > >;

template class LinearAffinelyDecomposedContainerBased< Stuff::LA::EigenDenseMatrix< double >,
                                                       Stuff::LA::EigenMappedDenseVector< double > >;

template class LinearAffinelyDecomposedContainerBased< Stuff::LA::EigenRowMajorSparseMatrix< double >,
                                                       Stuff::LA::EigenDenseVector< double > >;

template class LinearAffinelyDecomposedContainerBased< Stuff::LA::EigenRowMajorSparseMatrix< double >,
                                                       Stuff::LA::EigenMappedDenseVector< double > >;

#endif // HAVE_EIGEN
#if HAVE_ISTL

template class LinearAffinelyDecomposedContainerBased< Stuff::LA::IstlRowMajorSparseMatrix< double >,
                                                       Stuff::LA::IstlDenseVector< double > >;

#endif // HAVE_ISTL


} // namespace Operators
} // namespace Pymor
} // namespace Dune

