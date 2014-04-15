// This file is part of the dune-pymor project:
//   https://github.com/pymor/dune-pymor
// Copyright Holders: Stephan Rave, Felix Schindler
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#include "config.h"

#if HAVE_DUNE_ISTL

#include "affine.hh"
#include "istl.hh"

namespace Dune {
namespace Pymor {
namespace Operators {


template class IstlRowMajorSparseInverse< double >;
template class IstlRowMajorSparse< double >;
template class LinearAffinelyDecomposedContainerBased< IstlRowMajorSparse< double > >;


} // namespace Operators
} // namespace Pymor
} // namespace Dune

#endif // HAVE_DUNE_ISTL
