// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#include "config.h"

#include <utility>
#include <vector>
#include <string>

#include <dune/stuff/common/float_cmp.hh>

#include <dune/pymor/functionals/default.hh>
#include <dune/pymor/functionals/affine.hh>

#include "affine.hh"
#include "dunedynamic.hh"

namespace Dune {
namespace Pymor {
namespace LA {


// =============================
// ===== DuneDynamicVector =====
// =============================
template< class S >
DuneDynamicVector< S >::DuneDynamicVector(const DUNE_PYMOR_SSIZE_T size, const ScalarType value)
  : backend_(new BackendType(assert_is_not_negative(size), value))
{}

template< class S >
DuneDynamicVector< S >::DuneDynamicVector(BackendType* backend_ptr)
  : backend_(backend_ptr)
{}

template< class S >
DuneDynamicVector< S >::DuneDynamicVector(std::shared_ptr< BackendType >& backend_ptr)
  : backend_(std::move(backend_ptr))
{}

template< class S >
DuneDynamicVector< S >::DuneDynamicVector(const ThisType& other)
  : backend_(other.backend_)
{}

template< class S >
typename DuneDynamicVector< S >::ThisType& DuneDynamicVector< S >::operator=(const ThisType& other)
{
  backend_ = other.backend_;
  return *this;
}

template< class S >
typename DuneDynamicVector< S >::ThisType DuneDynamicVector< S >::copy() const
{
  return DuneDynamicVector< S >(new BackendType(*backend_));
}

template< class S >
void DuneDynamicVector< S >::scal(const ScalarType& alpha)
{
  ensure_uniqueness();
  backend_->operator*=(alpha);
}

template< class S >
void DuneDynamicVector< S >::axpy(const ScalarType& alpha, const ThisType& xx)
{
  ensure_uniqueness();
  auto& thisRef = *backend_;
  const auto& xRef = *(xx.backend_);
  for (size_t ii = 0; ii < thisRef.size(); ++ii)
    thisRef[ii] += alpha * xRef[ii];
}

template< class S >
DUNE_PYMOR_SSIZE_T DuneDynamicVector< S >::dim() const
{
  return backend_->size();
}

template< class S >
bool DuneDynamicVector< S >::has_equal_shape(const ThisType& other) const
{
  return dim() == other.dim();
}

template< class S >
bool DuneDynamicVector< S >::almost_equal(const ThisType& other, const ScalarType epsilon) const
{
  if (dim() != other.dim())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "dim of other (" << other.dim() << ") does not match the dim of this (" << dim() << ")!");
  return Dune::Stuff::Common::FloatCmp::eq(*backend_, *(other.backend_), epsilon);
} // ... almost_equal(...)

template< class S >
typename DuneDynamicVector< S >::ScalarType DuneDynamicVector< S >::dot(const ThisType& other) const
{
  if (dim() != other.dim())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "dim of other (" << other.dim() << ") does not match the dim of this (" << dim() << ")!");
  return backend_->operator*(*(other.backend_));
}

template< class S >
typename DuneDynamicVector< S >::ScalarType DuneDynamicVector< S >::l1_norm() const
{
  return backend_->one_norm();
}

template< class S >
typename DuneDynamicVector< S >::ScalarType DuneDynamicVector< S >::l2_norm() const
{
  return backend_->two_norm();
}

template< class S >
typename DuneDynamicVector< S >::ScalarType DuneDynamicVector< S >::sup_norm() const
{
  return backend_->infinity_norm();
}

template< class S >
std::vector< typename DuneDynamicVector< S >::ScalarType >
DuneDynamicVector< S >::components(const std::vector< DUNE_PYMOR_SSIZE_T >& component_indices) const
{
  if (component_indices.size() > dim())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "size of component_indices (" << component_indices.size()
                     << ") is larger than the dim of this (" << dim() << ")!");
  std::vector< ScalarType > values(component_indices.size(), 0);
  for (size_t ii = 0; ii < component_indices.size(); ++ii) {
    const auto& component = component_indices[ii];
    if (component < 0)
      DUNE_PYMOR_THROW(Exception::index_out_of_range,
                       "component_indices[" << ii << "] is negative (" << component << ")!");
    if (component >= dim())
      DUNE_PYMOR_THROW(Exception::index_out_of_range,
                       "component_indices[" << ii << "] is too large for this (" << dim() << ")!");
    values[ii] = backend_->operator[](component);
  }
  return values;
} // ... components(...)

template< class S >
std::vector< typename DuneDynamicVector< S >::ScalarType > DuneDynamicVector< S >::amax() const
{
  std::vector< ScalarType > result(2, 0.0);
  for (size_t ii = 0; ii < dim(); ++ii) {
    const ScalarType value = std::abs(backend_->operator[](ii));
    if (value > result[1]) {
      result[0] = ii;
      result[1] = value;
    }
  }
  return result;
} // ... amax(...)


template< class S >
void DuneDynamicVector< S >::add(const ThisType& other, ThisType& result) const
{
  if (dim() != other.dim())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "dim of other (" << other.dim() << ") does not match the dim of this (" << dim() << ")!");
  for (DUNE_PYMOR_SSIZE_T ii = 0; ii < dim(); ++ii)
    result.backend_->operator[](ii) = backend_->operator[](ii) + other.backend_->operator[](ii);
}

template< class S >
void DuneDynamicVector< S >::iadd(const ThisType& other)
{
  if (dim() != other.dim())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "dim of other (" << other.dim() << ") does not match the dim of this (" << dim() << ")!");
  ensure_uniqueness();
  backend_->operator+=(*(other.backend_));
}

template< class S >
void DuneDynamicVector< S >::sub(const ThisType& other, ThisType& result) const
{
  if (dim() != other.dim())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "dim of other (" << other.dim() << ") does not match the dim of this (" << dim() << ")!");
  for (DUNE_PYMOR_SSIZE_T ii = 0; ii < dim(); ++ii)
    result.backend_->operator[](ii) = backend_->operator[](ii) - other.backend_->operator[](ii);
}

template< class S >
void DuneDynamicVector< S >::isub(const ThisType& other)
{
  if (dim() != other.dim())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "dim of other (" << other.dim() << ") does not match the dim of this (" << dim() << ")!");
  ensure_uniqueness();
  backend_->operator-=(*(other.backend_));
}

template< class S >
typename DuneDynamicVector< S >::BackendType& DuneDynamicVector< S >::backend()
{
  ensure_uniqueness();
  return *backend_;
}

template< class S >
const typename DuneDynamicVector< S >::BackendType& DuneDynamicVector< S >::backend() const
{
  const_cast< ThisType& >(*this).ensure_uniqueness();
  return *backend_;
}

template< class S >
DUNE_PYMOR_SSIZE_T DuneDynamicVector< S >::assert_is_not_negative(const DUNE_PYMOR_SSIZE_T ii)
{
  if (ii < 0) DUNE_PYMOR_THROW(Exception::index_out_of_range, "ii has to be positive (is " << ii << ")!");
  return ii;
}

template< class S >
inline void DuneDynamicVector< S >::ensure_uniqueness()
{
  if (!backend_.unique())
    backend_ = std::make_shared< BackendType >(*backend_);
}

template class DuneDynamicVector< double >;
template class AffinelyDecomposedConstContainer< DuneDynamicVector< double > >;
template class AffinelyDecomposedContainer< DuneDynamicVector< double > >;


// =============================
// ===== DuneDynamicMatrix =====
// =============================
template< class S >
DuneDynamicMatrix< S >::DuneDynamicMatrix(const DUNE_PYMOR_SSIZE_T rr,
                                          const DUNE_PYMOR_SSIZE_T cc,
                                          const ScalarType value)
  : backend_(new BackendType(assert_is_not_negative(rr), assert_is_not_negative(cc), value))
{}

template< class S >
DuneDynamicMatrix< S >::DuneDynamicMatrix(BackendType* backend_ptr)
  : backend_(backend_ptr)
{}

template< class S >
DuneDynamicMatrix< S >::DuneDynamicMatrix(std::shared_ptr< BackendType >& backend_ptr)
  : backend_(backend_ptr)
{}

template< class S >
DuneDynamicMatrix< S >::DuneDynamicMatrix(const ThisType& other)
  : backend_(other.backend_)
{}

template< class S >
typename DuneDynamicMatrix< S >::ThisType& DuneDynamicMatrix< S >::operator=(const ThisType& other)
{
  backend_ = other.backend_;
  return *this;
}

template< class S >
typename DuneDynamicMatrix< S >::ThisType DuneDynamicMatrix< S >::copy() const
{
  return DuneDynamicMatrix< S >(new BackendType(*backend_));
}

template< class S >
DUNE_PYMOR_SSIZE_T DuneDynamicMatrix< S >::dim_source() const
{
  return backend_->cols();
}

template< class S >
DUNE_PYMOR_SSIZE_T DuneDynamicMatrix< S >::dim_range() const
{
  return backend_->rows();
}

template< class S >
bool DuneDynamicMatrix< S >::has_equal_shape(const ThisType& other) const
{
  return dim_source() == other.dim_source() && dim_range() == other.dim_range();
}

template< class S >
void DuneDynamicMatrix< S >::scal(const ScalarType& alpha)
{
  ensure_uniqueness();
  backend_->operator*=(alpha);
}

template< class S >
void DuneDynamicMatrix< S >::axpy(const ScalarType& alpha, const ThisType& xx)
{
  if (xx.dim_source() != dim_source())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "the dim_source of xx (" << xx.dim_source() << ") does not match the dim_source of this ("
                     << dim_source() << ")!");
  if (xx.dim_range() != dim_range())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "the dim_range of xx (" << xx.dim_range() << ") does not match the dim_range of this ("
                     << dim_range() << ")!");
  ensure_uniqueness();
  backend_->axpy(alpha, *(xx.backend_));
}

template< class S >
typename DuneDynamicMatrix< S >::BackendType& DuneDynamicMatrix< S >::backend()
{
  ensure_uniqueness();
  return *backend_;
}

template< class S >
const typename DuneDynamicMatrix< S >::BackendType& DuneDynamicMatrix< S >::backend() const
{
  const_cast< ThisType& >(*this).ensure_uniqueness();
  return *backend_;
}

template< class S >
DUNE_PYMOR_SSIZE_T DuneDynamicMatrix< S >::assert_is_not_negative(const DUNE_PYMOR_SSIZE_T ii)
{
  if (ii < 0) DUNE_PYMOR_THROW(Exception::index_out_of_range, "ii has to be positive (is " << ii << ")!");
  return ii;
}

template< class S >
inline void DuneDynamicMatrix< S >::ensure_uniqueness()
{
  if (!backend_.unique())
    backend_ = std::make_shared< BackendType >(*backend_);
}

template class DuneDynamicMatrix< double >;
template class AffinelyDecomposedConstContainer< DuneDynamicMatrix< double > >;
template class AffinelyDecomposedContainer< DuneDynamicMatrix< double > >;


} // namespace LA

namespace Functionals {

template class VectorBased< LA::DuneDynamicVector< double > >;
template class LinearAffinelyDecomposedVectorBased< LA::DuneDynamicVector< double > >;

}

} // namespace Pymor
} // namespace Dune
