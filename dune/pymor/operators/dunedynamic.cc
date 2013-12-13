// This file is part of the dune-pymor project:
//   https://github.com/pymor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#include "config.h"

#include "affine.hh"
#include "dunedynamic.hh"

namespace Dune {
namespace Pymor {
namespace Operators {


// ==============================
// ===== DuneDynamicInverse =====
// ==============================
template< class S >
DuneDynamicInverse< S >::DuneDynamicInverse(const ContainerType* matrix_ptr,
                                            const std::string option)
  : matrix_(matrix_ptr)
{
  if (option != "superlu")
    DUNE_PYMOR_THROW(Exception::wrong_option_given, "option has to be 'superlu' (is " << option << ")!");
}

template< class S >
DuneDynamicInverse< S >::DuneDynamicInverse(const std::shared_ptr< const ContainerType > matrix_ptr,
                                            const std::string option)
  : matrix_(matrix_ptr)
{
  if (option != "superlu")
    DUNE_PYMOR_THROW(Exception::wrong_option_given, "option has to be 'superlu' (is " << option << ")!");
}

template< class S >
bool DuneDynamicInverse< S >::linear() const
{
  return true;
}

template< class S >
DUNE_STUFF_SSIZE_T DuneDynamicInverse< S >::dim_source() const
{
  return matrix_->rows();
}

template< class S >
DUNE_STUFF_SSIZE_T DuneDynamicInverse< S >::dim_range() const
{
  return matrix_->cols();
}

template< class S >
void DuneDynamicInverse< S >::apply(const SourceType& source, RangeType& range, const Parameter mu) const
{
  if (!mu.empty()) DUNE_PYMOR_THROW(Exception::this_is_not_parametric,
                                    "mu has to be empty if parametric() == false (is " << mu << ")!");
  if (source.dim() != dim_source())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "the dim of source (" << source.dim() << ") does not match the dim_source of this ("
                     << dim_source() << ")!");
  if (range.dim() != dim_range())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "the dim of range (" << range.dim() << ") does not match the dim_range of this ("
                     << dim_range() << ")!");
  matrix_->backend_->solve(*(range.backend_), *(source.backend_));
}

template< class S >
std::vector< std::string > DuneDynamicInverse< S >::invert_options()
{
  return { "exact" };
}

template< class S >
typename DuneDynamicInverse< S >::InverseType DuneDynamicInverse< S >::invert(const std::string option,
                                                                              const Parameter mu) const
{
  if (!mu.empty()) DUNE_PYMOR_THROW(Exception::this_is_not_parametric,
                                    "mu has to be empty if parametric() == false (is " << mu << ")!");
  if (option != invert_options()[0])
    DUNE_PYMOR_THROW(Exception::wrong_option_given,
                     "option has to be '" << invert_options()[0] << "' (is " << option << ")!");
  return InverseType(matrix_);
}

template< class S >
typename DuneDynamicInverse< S >::FrozenType DuneDynamicInverse< S >::freeze_parameter(const Parameter mu) const
{
  if (!mu.empty())
    DUNE_PYMOR_THROW(Exception::this_is_not_parametric,
                     "do not call freeze_parameter(" << mu << ") is parametric() == false!");
  return FrozenType(nullptr, invert_options()[0]);
}

template class DuneDynamicInverse< double >;


// =======================
// ===== DuneDynamic =====
// =======================
template< class S >
DuneDynamic< S >::DuneDynamic(const ContainerType* matrix_ptr)
  : matrix_(matrix_ptr)
{}

template< class S >
DuneDynamic< S >::DuneDynamic(const std::shared_ptr< const ContainerType > matrix_ptr)
  : matrix_(matrix_ptr)
{}

template< class S >
bool DuneDynamic< S >::linear() const
{
  return true;
}

template< class S >
DUNE_STUFF_SSIZE_T DuneDynamic< S >::dim_source() const
{
  return matrix_->cols();
}

template< class S >
DUNE_STUFF_SSIZE_T DuneDynamic< S >::dim_range() const
{
  return matrix_->rows();
}

template< class S >
void DuneDynamic< S >::apply(const SourceType& source, RangeType& range, const Parameter mu) const
{
  if (!mu.empty()) DUNE_PYMOR_THROW(Exception::this_is_not_parametric,
                                    "mu has to be empty if parametric() == false (is " << mu << ")!");
  if (source.dim() != dim_source())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "the dim of source (" << source.dim() << ") does not match the dim_source of this ("
                     << dim_source() << ")!");
  if (range.dim() != dim_range())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "the dim of range (" << range.dim() << ") does not match the dim_range of this ("
                     << dim_range() << ")!");
  matrix_->backend_->mv(*(source.backend_), *(range.backend_));
}

template< class S >
std::vector< std::string > DuneDynamic< S >::invert_options()
{
  return { "superlu" };
}

template< class S >
typename DuneDynamic< S >::InverseType DuneDynamic< S >::invert(const std::string option, const Parameter mu) const
{
  if (!mu.empty()) DUNE_PYMOR_THROW(Exception::this_is_not_parametric,
                                    "mu has to be empty if parametric() == false (is " << mu << ")!");
  if (option != invert_options()[0])
    DUNE_PYMOR_THROW(Exception::wrong_option_given,
                     "option has to be '" << invert_options()[0] << "' (is " << option << ")!");
  return InverseType(matrix_);
}

template< class S >
typename DuneDynamic< S >::FrozenType DuneDynamic< S >::freeze_parameter(const Parameter mu) const
{
  if (!mu.empty())
    DUNE_PYMOR_THROW(Exception::this_is_not_parametric,
                     "do not call freeze_parameter(" << mu << ") is parametric() == false!");
  return FrozenType(nullptr);
}

template< class S >
std::shared_ptr< const typename DuneDynamic< S >::ContainerType > DuneDynamic< S >::container() const
{
  return matrix_;
}

template class DuneDynamic< double >;
template class LinearAffinelyDecomposedContainerBased< DuneDynamic< double > >;


} // namespace Operators
} // namespace Pymor
} // namespace Dune

