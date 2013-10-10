// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

namespace Dune {
namespace Pymor {


// =======================================
// ===== ParametricFunctionInterface =====
// =======================================
template< class E, class D, int d, class R, int r, int rC >
ParametricFunctionInterface< E, D, d, R, r, rC >::ParametricFunctionInterface(const ParameterType tt)
  : Parametric(tt)
{}

template< class E, class D, int d, class R, int r, int rC >
ParametricFunctionInterface< E, D, d, R, r, rC >::ParametricFunctionInterface(const Parametric& other)
  : Parametric(other)
{}

template< class E, class D, int d, class R, int r, int rC >
ParametricFunctionInterface< E, D, d, R, r, rC >::~ParametricFunctionInterface()
{}

template< class E, class D, int d, class R, int r, int rC >
std::string ParametricFunctionInterface< E, D, d, R, r, rC >::static_id()
{
  return "dune.pymor.parametricfunction";
}

template< class E, class D, int d, class R, int r, int rC >
std::string ParametricFunctionInterface< E, D, d, R, r, rC >::name() const
{
  return "dune.pymor.parametricfunction";
}

template< class E, class D, int d, class R, int r, int rC >
bool ParametricFunctionInterface< E, D, d, R, r, rC >::affinely_decomposable() const
{
  if (!parametric())
    DUNE_PYMOR_THROW(Exception::this_is_not_parametric,
                     "Do not call affinely_decomposable() if parametric() == false!");
  return false;
}

template< class E, class D, int d, class R, int r, int rC >
bool ParametricFunctionInterface< E, D, d, R, r, rC >::has_affine_part() const
{
  if (!parametric())
    return true;
  if (affinely_decomposable())
    DUNE_PYMOR_THROW(Exception::you_have_to_implement_this,
                     "Since affinely_decomposable() == true, youe really do!");
  else
    DUNE_PYMOR_THROW(Exception::requirements_not_met,
                     "Do not call has_affine_part() if affinely_decomposable() == false!");
  return false;
}

template< class E, class D, int d, class R, int r, int rC >
std::shared_ptr< const typename ParametricFunctionInterface< E, D, d, R, r, rC >::NonparametricType >
ParametricFunctionInterface< E, D, d, R, r, rC >::affine_part() const
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

template< class E, class D, int d, class R, int r, int rC >
unsigned int ParametricFunctionInterface< E, D, d, R, r, rC >::num_components() const
{
  if (!parametric())
    return 0;
  if (affinely_decomposable())
    DUNE_PYMOR_THROW(Exception::you_have_to_implement_this,
                     "since affinely_decomposable() == true, youe really do!");
  else
    DUNE_PYMOR_THROW(Exception::requirements_not_met,
                     "Do not call num_components() if affinely_decomposable() == false!");
  return false;
}

template< class E, class D, int d, class R, int r, int rC >
std::shared_ptr< const typename ParametricFunctionInterface< E, D, d, R, r, rC >::NonparametricType >
ParametricFunctionInterface< E, D, d, R, r, rC >::component(int qq) const
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

template< class E, class D, int d, class R, int r, int rC >
std::shared_ptr< const ParameterFunctional > ParametricFunctionInterface< E, D, d, R, r, rC >::coefficient(int qq) const
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


} // namespace Pymor
} // namespace Dune
