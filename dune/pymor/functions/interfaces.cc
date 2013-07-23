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
ParametricFunctionInterface< D, d, R, rR, rC >::ParametricFunctionInterface(const ParameterType tt)
  : Parametric(tt)
{}

template< class D, int d, class R, int rR, int rC >
ParametricFunctionInterface< D, d, R, rR, rC >::ParametricFunctionInterface(const Parametric& other)
  : Parametric(other)
{}

template< class D, int d, class R, int rR, int rC >
ParametricFunctionInterface< D, d, R, rR, rC >::~ParametricFunctionInterface()
{}

template< class D, int d, class R, int rR, int rC >
std::string ParametricFunctionInterface< D, d, R, rR, rC >::static_id()
{
  return "dune.pymor.parametricfunction";
}

template< class D, int d, class R, int rR, int rC >
std::string ParametricFunctionInterface< D, d, R, rR, rC >::name() const
{
  return "dune.pymor.parametricfunction";
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

template< class D, int d, class R, int rR, int rC >
bool ParametricFunctionInterface< D, d, R, rR, rC >::affinely_decomposable() const
{
  if (!parametric())
    DUNE_PYMOR_THROW(Exception::this_is_not_parametric,
                     "Do not call affinely_decomposable() if parametric() == false!");
  return false;
}

template< class D, int d, class R, int rR, int rC >
bool ParametricFunctionInterface< D, d, R, rR, rC >::has_affine_part() const
{
  if (!parametric())
    DUNE_PYMOR_THROW(Exception::this_is_not_parametric,
                     "Do not call affinely_decomposable() if parametric() == false!");
  if (affinely_decomposable())
    DUNE_PYMOR_THROW(Exception::you_have_to_implement_this,
                     "since affinely_decomposable() == true, youe really do!");
  else
    DUNE_PYMOR_THROW(Exception::requirements_not_met,
                     "Do not call has_affine_part() if affinely_decomposable() == false!");
  return false;
}

template< class D, int d, class R, int rR, int rC >
std::shared_ptr< const typename ParametricFunctionInterface< D, d, R, rR, rC >::NonparametricType >
ParametricFunctionInterface< D, d, R, rR, rC >::affine_part() const
{
  if (!parametric())
    DUNE_PYMOR_THROW(Exception::this_is_not_parametric,
                     "Do not call affine_part() if parametric() == false!");
  if (affinely_decomposable()) {
    if (has_affine_part())
      DUNE_PYMOR_THROW(Exception::you_have_to_implement_this,
                       "since affinely_decomposable() == true, youe really do!");
    else
      DUNE_PYMOR_THROW(Exception::requirements_not_met,
                       "Do not call affine_part() if has_affine_part() == false!");
  } else
    DUNE_PYMOR_THROW(Exception::requirements_not_met,
                     "Do not call affine_part() if affinely_decomposable() == false!");
  return nullptr;
}

template< class D, int d, class R, int rR, int rC >
unsigned int ParametricFunctionInterface< D, d, R, rR, rC >::num_components() const
{
  if (!parametric())
    DUNE_PYMOR_THROW(Exception::this_is_not_parametric,
                     "Do not call num_components() if parametric() == false!");
  if (affinely_decomposable())
    DUNE_PYMOR_THROW(Exception::you_have_to_implement_this,
                     "since affinely_decomposable() == true, youe really do!");
  else
    DUNE_PYMOR_THROW(Exception::requirements_not_met,
                     "Do not call num_components() if affinely_decomposable() == false!");
  return false;
}

template< class D, int d, class R, int rR, int rC >
std::shared_ptr< const typename ParametricFunctionInterface< D, d, R, rR, rC >::NonparametricType >
ParametricFunctionInterface< D, d, R, rR, rC >::component(int qq) const
{
  if (!parametric())
    DUNE_PYMOR_THROW(Exception::this_is_not_parametric,
                     "Do not call component(" << qq << ") if parametric() == false!");
  if (affinely_decomposable()) {
    if (num_components() > 0)
      DUNE_PYMOR_THROW(Exception::you_have_to_implement_this,
                       "since num_components() == " << num_components() << ", youe really do!");
    else
      DUNE_PYMOR_THROW(Exception::requirements_not_met,
                       "Do not call component(" << qq << ") if num_components() == 0!");
  } else
    DUNE_PYMOR_THROW(Exception::requirements_not_met,
                     "Do not call component(" << qq << ") if affinely_decomposable() == false!");
  return nullptr;
}

template< class D, int d, class R, int rR, int rC >
std::shared_ptr< const ParameterFunctional > ParametricFunctionInterface< D, d, R, rR, rC >::coefficient(int qq) const
{
  if (!parametric())
    DUNE_PYMOR_THROW(Exception::this_is_not_parametric,
                     "Do not call coefficient(" << qq << ") if parametric() == false!");
  if (affinely_decomposable()) {
    if (num_components() > 0)
      DUNE_PYMOR_THROW(Exception::you_have_to_implement_this,
                       "since num_components() == " << num_components() << ", youe really do!");
    else
      DUNE_PYMOR_THROW(Exception::requirements_not_met,
                       "Do not call coefficient(" << qq << ") if num_components() == 0!");
  } else
    DUNE_PYMOR_THROW(Exception::requirements_not_met,
                     "Do not call coefficient(" << qq << ") if affinely_decomposable() == false!");
  return nullptr;
}

template class ParametricFunctionInterface< double, 1, double, 1, 2 >;
template class ParametricFunctionInterface< double, 1, double, 1, 3 >;
template class ParametricFunctionInterface< double, 1, double, 2, 2 >;
template class ParametricFunctionInterface< double, 1, double, 2, 3 >;
template class ParametricFunctionInterface< double, 1, double, 3, 2 >;
template class ParametricFunctionInterface< double, 1, double, 3, 3 >;
template class ParametricFunctionInterface< double, 2, double, 1, 2 >;
template class ParametricFunctionInterface< double, 2, double, 1, 3 >;
template class ParametricFunctionInterface< double, 2, double, 2, 2 >;
template class ParametricFunctionInterface< double, 2, double, 2, 3 >;
template class ParametricFunctionInterface< double, 2, double, 3, 2 >;
template class ParametricFunctionInterface< double, 2, double, 3, 3 >;
template class ParametricFunctionInterface< double, 3, double, 1, 2 >;
template class ParametricFunctionInterface< double, 3, double, 1, 3 >;
template class ParametricFunctionInterface< double, 3, double, 2, 2 >;
template class ParametricFunctionInterface< double, 3, double, 2, 3 >;
template class ParametricFunctionInterface< double, 3, double, 3, 2 >;
template class ParametricFunctionInterface< double, 3, double, 3, 3 >;


// =================================================
// ===== ParametricFunctionInterface< ..., 1 > =====
// =================================================
template< class D, int d, class R, int r >
ParametricFunctionInterface< D, d, R, r >::ParametricFunctionInterface(const ParameterType tt)
  : Parametric(tt)
{}

template< class D, int d, class R, int r >
ParametricFunctionInterface< D, d, R, r >::ParametricFunctionInterface(const Parametric& other)
  : Parametric(other)
{}

template< class D, int d, class R, int r >
ParametricFunctionInterface< D, d, R, r >::~ParametricFunctionInterface()
{}

template< class D, int d, class R, int r >
std::string ParametricFunctionInterface< D, d, R, r >::static_id()
{
  return "dune.pymor.parametricfunction";
}

template< class D, int d, class R, int r >
std::string ParametricFunctionInterface< D, d, R, r >::name() const
{
  return "dune.pymor.parametricfunction";
}

template< class D, int d, class R, int r >
int ParametricFunctionInterface< D, d, R, r >::order() const
{
  return -1;
}

template< class D, int d, class R, int r >
typename ParametricFunctionInterface< D, d, R, r >::RangeType
ParametricFunctionInterface< D, d, R, r >::evaluate(const DomainType& x, const Parameter mu) const
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

template< class D, int d, class R, int r >
bool ParametricFunctionInterface< D, d, R, r >::affinely_decomposable() const
{
  if (!parametric())
    DUNE_PYMOR_THROW(Exception::this_is_not_parametric,
                     "Do not call affinely_decomposable() if parametric() == false!");
  return false;
}

template< class D, int d, class R, int r >
bool ParametricFunctionInterface< D, d, R, r >::has_affine_part() const
{
  if (!parametric())
    DUNE_PYMOR_THROW(Exception::this_is_not_parametric,
                     "Do not call affinely_decomposable() if parametric() == false!");
  if (affinely_decomposable())
    DUNE_PYMOR_THROW(Exception::you_have_to_implement_this,
                     "since affinely_decomposable() == true, youe really do!");
  else
    DUNE_PYMOR_THROW(Exception::requirements_not_met,
                     "Do not call has_affine_part() if affinely_decomposable() == false!");
  return false;
}

template< class D, int d, class R, int r >
std::shared_ptr< const typename ParametricFunctionInterface< D, d, R, r >::NonparametricType >
ParametricFunctionInterface< D, d, R, r >::affine_part() const
{
  if (!parametric())
    DUNE_PYMOR_THROW(Exception::this_is_not_parametric,
                     "Do not call affine_part() if parametric() == false!");
  if (affinely_decomposable()) {
    if (has_affine_part())
      DUNE_PYMOR_THROW(Exception::you_have_to_implement_this,
                       "since affinely_decomposable() == true, youe really do!");
    else
      DUNE_PYMOR_THROW(Exception::requirements_not_met,
                       "Do not call affine_part() if has_affine_part() == false!");
  } else
    DUNE_PYMOR_THROW(Exception::requirements_not_met,
                     "Do not call affine_part() if affinely_decomposable() == false!");
  return nullptr;
}

template< class D, int d, class R, int r >
unsigned int ParametricFunctionInterface< D, d, R, r >::num_components() const
{
  if (!parametric())
    DUNE_PYMOR_THROW(Exception::this_is_not_parametric,
                     "Do not call num_components() if parametric() == false!");
  if (affinely_decomposable())
    DUNE_PYMOR_THROW(Exception::you_have_to_implement_this,
                     "since affinely_decomposable() == true, youe really do!");
  else
    DUNE_PYMOR_THROW(Exception::requirements_not_met,
                     "Do not call num_components() if affinely_decomposable() == false!");
  return false;
}

template< class D, int d, class R, int r >
std::shared_ptr< const typename ParametricFunctionInterface< D, d, R, r >::NonparametricType >
ParametricFunctionInterface< D, d, R, r >::component(int qq) const
{
  if (!parametric())
    DUNE_PYMOR_THROW(Exception::this_is_not_parametric,
                     "Do not call component(" << qq << ") if parametric() == false!");
  if (affinely_decomposable()) {
    if (num_components() > 0)
      DUNE_PYMOR_THROW(Exception::you_have_to_implement_this,
                       "since num_components() == " << num_components() << ", youe really do!");
    else
      DUNE_PYMOR_THROW(Exception::requirements_not_met,
                       "Do not call component(" << qq << ") if num_components() == 0!");
  } else
    DUNE_PYMOR_THROW(Exception::requirements_not_met,
                     "Do not call component(" << qq << ") if affinely_decomposable() == false!");
  return nullptr;
}

template< class D, int d, class R, int r >
std::shared_ptr< const ParameterFunctional > ParametricFunctionInterface< D, d, R, r >::coefficient(int qq) const
{
  if (!parametric())
    DUNE_PYMOR_THROW(Exception::this_is_not_parametric,
                     "Do not call coefficient(" << qq << ") if parametric() == false!");
  if (affinely_decomposable()) {
    if (num_components() > 0)
      DUNE_PYMOR_THROW(Exception::you_have_to_implement_this,
                       "since num_components() == " << num_components() << ", youe really do!");
    else
      DUNE_PYMOR_THROW(Exception::requirements_not_met,
                       "Do not call coefficient(" << qq << ") if num_components() == 0!");
  } else
    DUNE_PYMOR_THROW(Exception::requirements_not_met,
                     "Do not call coefficient(" << qq << ") if affinely_decomposable() == false!");
  return nullptr;
}

template class ParametricFunctionInterface< double, 1, double, 1 >;
template class ParametricFunctionInterface< double, 1, double, 2 >;
template class ParametricFunctionInterface< double, 1, double, 3 >;
template class ParametricFunctionInterface< double, 2, double, 1 >;
template class ParametricFunctionInterface< double, 2, double, 2 >;
template class ParametricFunctionInterface< double, 2, double, 3 >;
template class ParametricFunctionInterface< double, 3, double, 1 >;
template class ParametricFunctionInterface< double, 3, double, 2 >;
template class ParametricFunctionInterface< double, 3, double, 3 >;


} // namespace Pymor
} // namespace Dune
