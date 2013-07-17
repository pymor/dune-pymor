// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifdef HAVE_CMAKE_CONFIG
  #include "cmake_config.h"
#elif defined (HAVE_CONFIG_H)
  #include "config.h"
#endif // HAVE_CMAKE_CONFIG

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
DuneDynamicVector< S >::DuneDynamicVector(const int size, const ScalarType value)
  : backend_(new BackendType(size, value))
{}

template< class S >
DuneDynamicVector< S >::DuneDynamicVector(BackendType* backend_ptr)
  : backend_(backend_ptr)
{}

template< class S >
DuneDynamicVector< S >::DuneDynamicVector(std::unique_ptr< BackendType >&& backend_ptr)
  : backend_(std::move(backend_ptr))
{}

template< class S >
DuneDynamicVector< S >::DuneDynamicVector(DuneDynamicVector&& source)
  : backend_(std::move(source.backend_))
{}

template< class S >
typename DuneDynamicVector< S >::ThisType& DuneDynamicVector< S >::operator=(ThisType&& source)
{
  if (this != &source) {
    backend_ = std::move(source.backend_);
  }
  return *this;
}

template< class S >
DuneDynamicVector< S > DuneDynamicVector< S >::copy() const
{
  return DuneDynamicVector< S >(new BackendType(*backend_));
}

template< class S >
void DuneDynamicVector< S >::scal(const ScalarType& alpha)
{
  backend_->operator*=(alpha);
}

template< class S >
void DuneDynamicVector< S >::axpy(const ScalarType& alpha, const ThisType& xx) throw (Exception::sizes_do_not_match)
{
  auto& thisRef = *backend_;
  const auto& xRef = *(xx.backend_);
  for (size_t ii = 0; ii < thisRef.size(); ++ii)
    thisRef[ii] += alpha * xRef[ii];
}

template< class S >
unsigned int DuneDynamicVector< S >::dim() const
{
  return backend_->size();
}

template< class S >
bool DuneDynamicVector< S >::has_equal_shape(const ThisType& other) const
{
  return dim() == other.dim();
}

template< class S >
bool DuneDynamicVector< S >::almost_equal(const ThisType& other,
                                          const ScalarType epsilon) const
{
  if (dim() != other.dim())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "dim of other (" << other.dim() << ") does not match the dim of this (" << dim() << ")!");
  return Dune::Stuff::Common::FloatCmp::eq(*backend_, *(other.backend_), epsilon);
} // ... almost_equal(...)

template< class S >
typename DuneDynamicVector< S >::ScalarType DuneDynamicVector< S >::dot(const ThisType& other) const
  throw (Exception::sizes_do_not_match)
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
DuneDynamicVector< S >::components(const std::vector< int >& component_indices) const
  throw (Exception::sizes_do_not_match, Exception::index_out_of_range)
{
  if (component_indices.size() > dim())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "size of component_indices (" << component_indices.size()
                     << ") is larger than the dim of this (" << dim() << ")!");
  std::vector< double > values(component_indices.size(), 0);
  for (size_t ii = 0; ii < component_indices.size(); ++ii) {
    const int component = component_indices[ii];
    if (component < 0)
      DUNE_PYMOR_THROW(Exception::index_out_of_range,
                       "component_indices[" << ii << "] is negative (" << component << ")!");
    if (component >= int(dim()))
      DUNE_PYMOR_THROW(Exception::index_out_of_range,
                       "component_indices[" << ii << "] is too large for this (" << dim() << ")!");
    values[ii] = backend_->operator[](component);
  }
  return values;
} // ... components(...)

template< class S >
std::vector< typename DuneDynamicVector< S >::ScalarType > DuneDynamicVector< S >::amax() const
{
  std::vector< double > result(2, 0.0);
  for (size_t ii = 0; ii < dim(); ++ii) {
    const double value = std::abs(backend_->operator[](ii));
    if (value > result[1]) {
      result[0] = ii;
      result[1] = value;
    }
  }
  return result;
} // ... amax(...)


template< class S >
void DuneDynamicVector< S >::add(const ThisType& other, ThisType& result) const
  throw (Exception::sizes_do_not_match)
{
  if (dim() != other.dim())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "dim of other (" << other.dim() << ") does not match the dim of this (" << dim() << ")!");
  for (unsigned int ii = 0; ii < dim(); ++ii)
    result.backend_->operator[](ii) = backend_->operator[](ii) + other.backend_->operator[](ii);
}

template< class S >
void DuneDynamicVector< S >::iadd(const ThisType& other) throw (Exception::sizes_do_not_match)
{
  if (dim() != other.dim())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "dim of other (" << other.dim() << ") does not match the dim of this (" << dim() << ")!");
  backend_->operator+=(*(other.backend_));
}

template< class S >
void DuneDynamicVector< S >::sub(const ThisType& other, ThisType& result) const
  throw (Exception::sizes_do_not_match)
{
  if (dim() != other.dim())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "dim of other (" << other.dim() << ") does not match the dim of this (" << dim() << ")!");
  for (unsigned int ii = 0; ii < dim(); ++ii)
    result.backend_->operator[](ii) = backend_->operator[](ii) - other.backend_->operator[](ii);
}

template< class S >
void DuneDynamicVector< S >::isub(const ThisType& other) throw (Exception::sizes_do_not_match)
{
  if (dim() != other.dim())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "dim of other (" << other.dim() << ") does not match the dim of this (" << dim() << ")!");
  backend_->operator-=(*(other.backend_));
}


//// =============================
//// ===== DuneDynamicMatrix =====
//// =============================
//DuneDynamicMatrix::DuneDynamicMatrix(const int rr, const int cc, const double value)
//  : backend_(std::make_shared< BackendType >(assert_is_positive(rr), assert_is_positive(cc), value))
//{}

//DuneDynamicMatrix::DuneDynamicMatrix(std::shared_ptr< BackendType > backendPtr)
//  : backend_(backendPtr)
//{}

//DuneDynamicMatrix::DuneDynamicMatrix(DuneDynamicMatrix& other)
//  : backend_(other.backend_)
//{}

//DuneDynamicMatrix DuneDynamicMatrix::copy() const
//{
//  return DuneDynamicMatrix(std::make_shared< BackendType >(*backend_));
//}

//unsigned int DuneDynamicMatrix::dim_source() const
//{
//  return backend_->cols();
//}

//unsigned int DuneDynamicMatrix::dim_range() const
//{
//  return backend_->rows();
//}

//void DuneDynamicMatrix::scal(const double alpha)
//{
//  backend_->operator*=(alpha);
//}

//void DuneDynamicMatrix::axpy(const double alpha, const DuneDynamicMatrix& x) throw (Exception::sizes_do_not_match)
//{
//  if (x.dim_source() != dim_source())
//    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
//                     "the dim_source of x (" << x.dim_source() << ") does not match the dim_source of this ("
//                     << dim_source() << ")!");
//  if (x.dim_range() != dim_range())
//    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
//                     "the dim_range of x (" << x.dim_range() << ") does not match the dim_range of this ("
//                     << dim_range() << ")!");
//  backend_->axpy(alpha, *(x.backend()));
//}

//std::shared_ptr< typename DuneDynamicMatrix::BackendType > DuneDynamicMatrix::backend()
//{
//  return backend_;
//}

//std::shared_ptr< const typename DuneDynamicMatrix::BackendType > DuneDynamicMatrix::backend() const
//{
//  return backend_;
//}

//int DuneDynamicMatrix::assert_is_positive(const int ii) throw (Exception::index_out_of_range)
//{
//  if (ii <= 0) DUNE_PYMOR_THROW(Exception::index_out_of_range, "ii has to be positive (is " << ii << ")!");
//  return ii;
//}

//template class ContainerInterface< DuneDynamicVectorTraits< double > >;
template class DuneDynamicVector< double >;
template class AffinelyDecomposedConstContainer< DuneDynamicVector< double > >;
template class AffinelyDecomposedContainer< DuneDynamicVector< double > >;


} // namespace LA

namespace Functionals {

template class VectorBased< LA::DuneDynamicVector< double > >;
template class LinearAffinelyDecomposedVectorBased< LA::DuneDynamicVector< double > >;

}

} // namespace Pymor
} // namespace Dune
