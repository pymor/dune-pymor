// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifdef HAVE_CMAKE_CONFIG
  #include "cmake_config.h"
#elif defined (HAVE_CONFIG_H)
  #include "config.h"
#endif // HAVE_CMAKE_CONFIG

#include "interfaces.hh"

namespace Dune {
namespace Pymor {


// =======================================
// ===== ParametricFunctionInterface =====
// =======================================
template< class D, int d, class R, int rR, int rC >
ParametricFunctionInterface< D, d, R, rR, rC >::ParametricFunctionInterface()
  : Parametric()
{}

template< class D, int d, class R, int rR, int rC >
ParametricFunctionInterface< D, d, R, rR, rC >::ParametricFunctionInterface(const ParameterType& tt)
  : Parametric(tt)
{}

template< class D, int d, class R, int rR, int rC >
ParametricFunctionInterface< D, d, R, rR, rC >::ParametricFunctionInterface(const std::string& kk, const int& vv)
  throw (Exception::key_is_not_valid, Exception::index_out_of_range)
  : Parametric(kk, vv)
{}

template< class D, int d, class R, int rR, int rC >
ParametricFunctionInterface< D, d, R, rR, rC >::ParametricFunctionInterface(const std::vector< std::string >& kk,
                                                                            const std::vector< int >& vv)
  throw (Exception::key_is_not_valid, Exception::index_out_of_range, Exception::sizes_do_not_match)
  : Parametric(kk, vv)
{}

template< class D, int d, class R, int rR, int rC >
ParametricFunctionInterface< D, d, R, rR, rC >::ParametricFunctionInterface(const Parametric& other)
  : Parametric(other)
{}

template< class D, int d, class R, int rR, int rC >
std::string ParametricFunctionInterface< D, d, R, rR, rC >::name() const
{
  return "dunepymor.parametricfunction";
}

template< class D, int d, class R, int rR, int rC >
int ParametricFunctionInterface< D, d, R, rR, rC >::order() const
{
  return -1;
}

template< class D, int d, class R, int rR, int rC >
typename ParametricFunctionInterface< D, d, R, rR, rC >::RangeType
ParametricFunctionInterface< D, d, R, rR, rC >::evaluate(const DomainType& x,
                                                         const Parameter mu) const
  throw (Exception::wrong_parameter_type)
{
  if (mu.type() != Parametric::parameter_type())
    DUNE_PYMOR_THROW(Exception::wrong_parameter_type,
                     "type of mu (" << mu.type() << ") does not match the parameter_type of this ("
                     << Parametric::parameter_type() << ")!");
  RangeType ret;
  evaluate(x, ret, mu);
  return ret;
}


// =================================================
// ===== ParametricFunctionInterface< ..., 1 > =====
// =================================================
template< class D, int d, class R, int rR >
ParametricFunctionInterface< D, d, R, rR >::ParametricFunctionInterface()
  : Parametric()
{}

template< class D, int d, class R, int rR >
ParametricFunctionInterface< D, d, R, rR >::ParametricFunctionInterface(const ParameterType& tt)
  : Parametric(tt)
{}

template< class D, int d, class R, int rR >
ParametricFunctionInterface< D, d, R, rR >::ParametricFunctionInterface(const std::string& kk, const int& vv)
  throw (Exception::key_is_not_valid, Exception::index_out_of_range)
  : Parametric(kk, vv)
{}

template< class D, int d, class R, int rR >
ParametricFunctionInterface< D, d, R, rR >::ParametricFunctionInterface(const std::vector< std::string >& kk,
                                                                            const std::vector< int >& vv)
  throw (Exception::key_is_not_valid, Exception::index_out_of_range, Exception::sizes_do_not_match)
  : Parametric(kk, vv)
{}

template< class D, int d, class R, int rR >
ParametricFunctionInterface< D, d, R, rR >::ParametricFunctionInterface(const Parametric& other)
  : Parametric(other)
{}

template< class D, int d, class R, int rR >
std::string ParametricFunctionInterface< D, d, R, rR >::name() const
{
  return "dunepymor.parametricfunction";
}

template< class D, int d, class R, int rR >
int ParametricFunctionInterface< D, d, R, rR >::order() const
{
  return -1;
}

template< class D, int d, class R, int rR >
typename ParametricFunctionInterface< D, d, R, rR >::RangeType
ParametricFunctionInterface< D, d, R, rR >::evaluate(const DomainType& x, const Parameter mu) const
  throw (Exception::wrong_parameter_type)
{
  if (mu.type() != Parametric::parameter_type())
    DUNE_PYMOR_THROW(Exception::wrong_parameter_type,
                     "type of mu (" << mu.type() << ") does not match the parameter_type of this ("
                     << Parametric::parameter_type() << ")!");
  RangeType ret;
  evaluate(x, ret, mu);
  return ret;
}

template class ParametricFunctionInterface< double, 1, double, 1 >;
template class ParametricFunctionInterface< double, 1, double, 1, 2 >;
template class ParametricFunctionInterface< double, 1, double, 1, 3 >;
template class ParametricFunctionInterface< double, 1, double, 2 >;
template class ParametricFunctionInterface< double, 1, double, 2, 2 >;
template class ParametricFunctionInterface< double, 1, double, 2, 3 >;
template class ParametricFunctionInterface< double, 1, double, 3 >;
template class ParametricFunctionInterface< double, 1, double, 3, 2 >;
template class ParametricFunctionInterface< double, 1, double, 3, 3 >;
template class ParametricFunctionInterface< double, 2, double, 1 >;
template class ParametricFunctionInterface< double, 2, double, 1, 2 >;
template class ParametricFunctionInterface< double, 2, double, 1, 3 >;
template class ParametricFunctionInterface< double, 2, double, 2 >;
template class ParametricFunctionInterface< double, 2, double, 2, 2 >;
template class ParametricFunctionInterface< double, 2, double, 2, 3 >;
template class ParametricFunctionInterface< double, 2, double, 3 >;
template class ParametricFunctionInterface< double, 2, double, 3, 2 >;
template class ParametricFunctionInterface< double, 2, double, 3, 3 >;
template class ParametricFunctionInterface< double, 3, double, 1 >;
template class ParametricFunctionInterface< double, 3, double, 1, 2 >;
template class ParametricFunctionInterface< double, 3, double, 1, 3 >;
template class ParametricFunctionInterface< double, 3, double, 2 >;
template class ParametricFunctionInterface< double, 3, double, 2, 2 >;
template class ParametricFunctionInterface< double, 3, double, 2, 3 >;
template class ParametricFunctionInterface< double, 3, double, 3 >;
template class ParametricFunctionInterface< double, 3, double, 3, 2 >;
template class ParametricFunctionInterface< double, 3, double, 3, 3 >;


// =============================================
// ===== AffineParametricFunctionInterface =====
// =============================================
template< class D, int d, class R, int rR, int rC >
AffineParametricFunctionInterface< D, d, R, rR, rC >::AffineParametricFunctionInterface()
  : ParametricFunctionInterface< D, d, R, rR, rC >()
{}

template< class D, int d, class R, int rR, int rC >
AffineParametricFunctionInterface< D, d, R, rR, rC >::AffineParametricFunctionInterface(const ParameterType& tt)
  : ParametricFunctionInterface< D, d, R, rR, rC >(tt)
{}

template< class D, int d, class R, int rR, int rC >
AffineParametricFunctionInterface< D, d, R, rR, rC >::AffineParametricFunctionInterface(const std::string& kk,
                                                                                        const int& vv)
  throw (Exception::key_is_not_valid, Exception::index_out_of_range)
  : ParametricFunctionInterface< D, d, R, rR, rC >(kk, vv)
{}

template< class D, int d, class R, int rR, int rC >
AffineParametricFunctionInterface< D, d, R, rR, rC >::AffineParametricFunctionInterface(const std::vector< std::string >& kk,
                                                                                        const std::vector< int >& vv)
  throw (Exception::key_is_not_valid, Exception::index_out_of_range, Exception::sizes_do_not_match)
  : ParametricFunctionInterface< D, d, R, rR, rC >(kk, vv)
{}

template< class D, int d, class R, int rR, int rC >
AffineParametricFunctionInterface< D, d, R, rR, rC >::AffineParametricFunctionInterface(const Parametric& other)
  : ParametricFunctionInterface< D, d, R, rR, rC >(other)
{}

template< class D, int d, class R, int rR, int rC >
std::string AffineParametricFunctionInterface< D, d, R, rR, rC >::name() const
{
  return "dunepymor.parametricfunction";
}

template class AffineParametricFunctionInterface< double, 1, double, 1, 1 >;
template class AffineParametricFunctionInterface< double, 1, double, 1, 2 >;
template class AffineParametricFunctionInterface< double, 1, double, 1, 3 >;
template class AffineParametricFunctionInterface< double, 1, double, 2, 1 >;
template class AffineParametricFunctionInterface< double, 1, double, 2, 2 >;
template class AffineParametricFunctionInterface< double, 1, double, 2, 3 >;
template class AffineParametricFunctionInterface< double, 1, double, 3, 1 >;
template class AffineParametricFunctionInterface< double, 1, double, 3, 2 >;
template class AffineParametricFunctionInterface< double, 1, double, 3, 3 >;
template class AffineParametricFunctionInterface< double, 2, double, 1, 1 >;
template class AffineParametricFunctionInterface< double, 2, double, 1, 2 >;
template class AffineParametricFunctionInterface< double, 2, double, 1, 3 >;
template class AffineParametricFunctionInterface< double, 2, double, 2, 1 >;
template class AffineParametricFunctionInterface< double, 2, double, 2, 2 >;
template class AffineParametricFunctionInterface< double, 2, double, 2, 3 >;
template class AffineParametricFunctionInterface< double, 2, double, 3, 1 >;
template class AffineParametricFunctionInterface< double, 2, double, 3, 2 >;
template class AffineParametricFunctionInterface< double, 2, double, 3, 3 >;
template class AffineParametricFunctionInterface< double, 3, double, 1, 1 >;
template class AffineParametricFunctionInterface< double, 3, double, 1, 2 >;
template class AffineParametricFunctionInterface< double, 3, double, 1, 3 >;
template class AffineParametricFunctionInterface< double, 3, double, 2, 1 >;
template class AffineParametricFunctionInterface< double, 3, double, 2, 2 >;
template class AffineParametricFunctionInterface< double, 3, double, 2, 3 >;
template class AffineParametricFunctionInterface< double, 3, double, 3, 1 >;
template class AffineParametricFunctionInterface< double, 3, double, 3, 2 >;
template class AffineParametricFunctionInterface< double, 3, double, 3, 3 >;


} // namespace Pymor
} // namespace Dune
