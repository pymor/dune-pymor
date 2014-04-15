// This file is part of the dune-pymor project:
//   https://github.com/pymor/dune-pymor
// Copyright Holders: Stephan Rave, Felix Schindler
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#include "config.h"

#if HAVE_EIGEN

#include "eigen.hh"
#include "affine.hh"

template class Dune::Pymor::Operators::EigenRowMajorSparseInverse< double >;
template class Dune::Pymor::Operators::EigenRowMajorSparse< double >;
template class Dune::Pymor::Operators::LinearAffinelyDecomposedContainerBased< Dune::Pymor::Operators::EigenRowMajorSparse< double > >;

#endif // HAVE_EIGEN
