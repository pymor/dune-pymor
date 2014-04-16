// This file is part of the dune-pymor project:
//   https://github.com/pymor/dune-pymor
// Copyright Holders: Stephan Rave, Felix Schindler
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#include "config.h"

#include "affine.hh"
#include "common.hh"

namespace Dune {
namespace Pymor {
namespace Operators {


template class CommonDenseInverse< double >;
template class CommonDense< double >;
template class LinearAffinelyDecomposedContainerBased< CommonDense< double > >;


} // namespace Operators
} // namespace Pymor
} // namespace Dune

