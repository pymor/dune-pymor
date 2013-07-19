// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifdef HAVE_CMAKE_CONFIG
  #include "cmake_config.h"
#elif defined (HAVE_CONFIG_H)
  #include "config.h"
#endif // HAVE_CMAKE_CONFIG

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
unsigned int DuneDynamicInverse< S >::dim_source() const
{
  return matrix_->dim_range();
}

template< class S >
unsigned int DuneDynamicInverse< S >::dim_range() const
{
  return matrix_->dim_source();
}

template< class S >
void DuneDynamicInverse< S >::apply(const SourceType& source, RangeType& range, const Parameter mu) const
  throw (Exception::sizes_do_not_match,
         Exception::wrong_parameter_type,
         Exception::requirements_not_met,
         Exception::linear_solver_failed,
         Exception::this_does_not_make_any_sense)
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
  return FrozenType(nullptr, "superlu");
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
unsigned int DuneDynamic< S >::dim_source() const
{
  return matrix_->dim_source();
}

template< class S >
unsigned int DuneDynamic< S >::dim_range() const
{
  return matrix_->dim_range();
}

template< class S >
void DuneDynamic< S >::apply(const SourceType& source, RangeType& range, const Parameter mu) const
  throw (Exception::sizes_do_not_match,
         Exception::wrong_parameter_type,
         Exception::requirements_not_met,
         Exception::linear_solver_failed,
         Exception::this_does_not_make_any_sense)
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

