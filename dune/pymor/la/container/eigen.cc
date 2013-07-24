// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifdef HAVE_CMAKE_CONFIG
  #include "cmake_config.h"
#elif defined (HAVE_CONFIG_H)
  #include "config.h"
#endif // HAVE_CMAKE_CONFIG

#if HAVE_EIGEN

#include <dune/stuff/common/float_cmp.hh>

#include "affine.hh"
#include "eigen.hh"

namespace Dune {
namespace Pymor {
namespace LA {


// ============================
// ===== EigenDenseVector =====
// ============================
template< class S >
EigenDenseVector< S >::EigenDenseVector(const int size, const ScalarType value)
  : backend_(new BackendType(assert_is_not_negative(size), value))
{}

template< class S >
EigenDenseVector< S >::EigenDenseVector(BackendType* backend_ptr)
  : backend_(backend_ptr)
{}

template< class S >
EigenDenseVector< S >::EigenDenseVector(std::unique_ptr< BackendType >&& backend_ptr)
  : backend_(std::move(backend_ptr))
{}

template< class S >
EigenDenseVector< S >::EigenDenseVector(ThisType&& source)
  : backend_(std::move(source.backend_))
{}

template< class S >
typename EigenDenseVector< S >::ThisType& EigenDenseVector< S >::operator=(ThisType&& source)
{
  if (this != &source) {
    backend_ = std::move(source.backend_);
  }
  return *this;
}

template< class S >
typename EigenDenseVector< S >::ThisType EigenDenseVector< S >::copy() const
{
  return EigenDenseVector< S >(new BackendType(*backend_));
}

template< class S >
void EigenDenseVector< S >::scal(const ScalarType& alpha)
{
  backend_->backend() *= alpha;
}

template< class S >
void EigenDenseVector< S >::axpy(const ScalarType& alpha, const ThisType& xx) throw (Exception::sizes_do_not_match)
{
  auto& thisRef = *backend_;
  const auto& xRef = *(xx.backend_);
  thisRef.backend() += alpha * xRef.backend();
}

template< class S >
unsigned int EigenDenseVector< S >::dim() const
{
  return backend_->size();
}

template< class S >
bool EigenDenseVector< S >::has_equal_shape(const ThisType& other) const
{
  return dim() == other.dim();
}

template< class S >
bool EigenDenseVector< S >::almost_equal(const ThisType& other,
                                          const ScalarType epsilon) const
{
  if (dim() != other.dim())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "dim of other (" << other.dim() << ") does not match the dim of this (" << dim() << ")!");
  return Dune::Stuff::Common::FloatCmp::eq(*backend_, *(other.backend_), epsilon);
} // ... almost_equal(...)

template< class S >
typename EigenDenseVector< S >::ScalarType EigenDenseVector< S >::dot(const ThisType& other) const
  throw (Exception::sizes_do_not_match)
{
  if (dim() != other.dim())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "dim of other (" << other.dim() << ") does not match the dim of this (" << dim() << ")!");
  return backend_->backend().transpose() * other.backend_->backend();
}

template< class S >
typename EigenDenseVector< S >::ScalarType EigenDenseVector< S >::l1_norm() const
{
  return backend_->backend().template lpNorm< 1 >();
}

template< class S >
typename EigenDenseVector< S >::ScalarType EigenDenseVector< S >::l2_norm() const
{
  return backend_->backend().norm();
}

template< class S >
typename EigenDenseVector< S >::ScalarType EigenDenseVector< S >::sup_norm() const
{
  return backend_->backend().template lpNorm< ::Eigen::Infinity >();
}

template< class S >
std::vector< typename EigenDenseVector< S >::ScalarType >
EigenDenseVector< S >::components(const std::vector< int >& component_indices) const
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
    values[ii] = backend_->backend()[component];
  }
  return values;
} // ... components(...)

template< class S >
std::vector< typename EigenDenseVector< S >::ScalarType > EigenDenseVector< S >::amax() const
{
  std::vector< ScalarType > result(2, ScalarType(0));
  size_t minIndex = 0;
  size_t maxIndex = 0;
  const ScalarType minimum = backend_->backend().minCoeff(&minIndex);
  const ScalarType maximum = backend_->backend().maxCoeff(&maxIndex);
  if (std::abs(maximum) >= std::abs(minimum)) {
    result[0] = minIndex;
    result[1] = std::abs(minimum);
  } else {
    result[0] = maxIndex;
    result[1] = maximum;
  }
  return result;
} // ... amax(...)

template< class S >
void EigenDenseVector< S >::add(const ThisType& other, ThisType& result) const
  throw (Exception::sizes_do_not_match)
{
  if (dim() != other.dim())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "dim of other (" << other.dim() << ") does not match the dim of this (" << dim() << ")!");
  result.backend_->backend() = backend_->backend() + other.backend_->backend();
}

template< class S >
void EigenDenseVector< S >::iadd(const ThisType& other) throw (Exception::sizes_do_not_match)
{
  if (dim() != other.dim())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "dim of other (" << other.dim() << ") does not match the dim of this (" << dim() << ")!");
  backend_->backend() += other.backend_->backend();
}

template< class S >
void EigenDenseVector< S >::sub(const ThisType& other, ThisType& result) const
  throw (Exception::sizes_do_not_match)
{
  if (dim() != other.dim())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "dim of other (" << other.dim() << ") does not match the dim of this (" << dim() << ")!");
  for (unsigned int ii = 0; ii < dim(); ++ii)
    result.backend_->backend() = backend_->backend() - other.backend_->backend();
}

template< class S >
void EigenDenseVector< S >::isub(const ThisType& other) throw (Exception::sizes_do_not_match)
{
  if (dim() != other.dim())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "dim of other (" << other.dim() << ") does not match the dim of this (" << dim() << ")!");
  backend_->backend() -= other.backend_->backend();
}

template< class S >
int EigenDenseVector< S >::assert_is_not_negative(const int ii) throw (Exception::index_out_of_range)
{
  if (ii < 0) DUNE_PYMOR_THROW(Exception::index_out_of_range, "ii has to be positive (is " << ii << ")!");
  return ii;
}

template class EigenDenseVector< double >;
template class AffinelyDecomposedConstContainer< EigenDenseVector< double > >;
template class AffinelyDecomposedContainer< EigenDenseVector< double > >;


// =====================================
// ===== EigenRowMajorSparseMatrix =====
// =====================================
template< class S >
EigenRowMajorSparseMatrix< S >::EigenRowMajorSparseMatrix()
  : backend_(new BackendType())
{}

template< class S >
EigenRowMajorSparseMatrix< S >::EigenRowMajorSparseMatrix(BackendType* backend_ptr)
  : backend_(backend_ptr)
{}

template< class S >
EigenRowMajorSparseMatrix< S >::EigenRowMajorSparseMatrix(std::unique_ptr< BackendType >&& backend_ptr)
  : backend_(std::move(backend_ptr))
{}

template< class S >
EigenRowMajorSparseMatrix< S >::EigenRowMajorSparseMatrix(ThisType&& source)
  : backend_(std::move(source.backend_))
{}

template< class S >
typename EigenRowMajorSparseMatrix< S >::ThisType& EigenRowMajorSparseMatrix< S >::operator=(ThisType&& source)
{
  if (this != &source) {
    backend_ = std::move(source.backend_);
  }
  return *this;
}

template< class S >
typename EigenRowMajorSparseMatrix< S >::ThisType EigenRowMajorSparseMatrix< S >::copy() const
{
  return EigenRowMajorSparseMatrix< S >(new BackendType(*backend_));
}

template< class S >
unsigned int EigenRowMajorSparseMatrix< S >::dim_source() const
{
  return backend_->cols();
}

template< class S >
unsigned int EigenRowMajorSparseMatrix< S >::dim_range() const
{
  return backend_->rows();
}

template< class S >
bool EigenRowMajorSparseMatrix< S >::has_equal_shape(const ThisType& other) const
{
  return dim_source() == other.dim_source() && dim_range() == other.dim_range();
}

template< class S >
void EigenRowMajorSparseMatrix< S >::scal(const ScalarType& alpha)
{
  backend_->backend() *= alpha;
}

template< class S >
void EigenRowMajorSparseMatrix< S >::axpy(const ScalarType& alpha, const ThisType& xx) throw (Exception::sizes_do_not_match)
{
  if (xx.dim_source() != dim_source())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "the dim_source of xx (" << xx.dim_source() << ") does not match the dim_source of this ("
                     << dim_source() << ")!");
  if (xx.dim_range() != dim_range())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "the dim_range of xx (" << xx.dim_range() << ") does not match the dim_range of this ("
                     << dim_range() << ")!");
  backend_->backend() += alpha * xx.backend_->backend();
}

template class EigenRowMajorSparseMatrix< double >;
//template class AffinelyDecomposedConstContainer< EigenDenseVector< double > >;
//template class AffinelyDecomposedContainer< EigenDenseVector< double > >;


} // namespace LA
} // namespace Pymor
} // namespace Dune

#endif // HAVE_EIGEN