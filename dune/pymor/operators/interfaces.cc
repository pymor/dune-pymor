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


// =============================
// ===== OperatorInterface =====
// =============================
OperatorInterface::OperatorInterface()
  : Parametric()
{}

OperatorInterface::OperatorInterface(const ParameterType& tt)
  : Parametric(tt)
{}

OperatorInterface::OperatorInterface(const std::string& kk, const int& vv) throw (Exception::key_is_not_valid,
                                                                                  Exception::index_out_of_range)
  : Parametric(kk, vv)
{}

OperatorInterface::OperatorInterface(const std::vector< std::string >& kk,
                                     const std::vector< int >& vv) throw (Exception::key_is_not_valid,
                                                                          Exception::index_out_of_range,
                                                                          Exception::sizes_do_not_match)
  : Parametric(kk, vv)
{}

OperatorInterface::OperatorInterface(const Parametric& other)
  : Parametric(other)
{}

OperatorInterface::~OperatorInterface()
{}

double OperatorInterface::apply2(const LA::VectorInterface* range,
                                 const LA::VectorInterface* source,
                                 const Parameter mu) const throw (Exception::types_are_not_compatible,
                                                                  Exception::you_have_to_implement_this,
                                                                  Exception::sizes_do_not_match,
                                                                  Exception::wrong_parameter_type,
                                                                  Exception::requirements_not_met,
                                                                  Exception::linear_solver_failed,
                                                                  Exception::this_does_not_make_any_sense)
{
  std::stringstream msg;
  size_t throw_up = 0;
  if (source->type() != type_source()) {
    msg << "source (" << source->type() << ") is not a compatible type_source (" << type_source() << ")";
    ++throw_up;
  }
  if (range->type() != type_range()) {
    if (throw_up)
      msg << " and ";
    msg << "range (" << range->type() << ") is not a compatible type_range (" << type_range() << ")";
  }
  if (throw_up) DUNE_PYMOR_THROW(Exception::types_are_not_compatible, msg.str() << "!");
  if (source->dim() != dim_source())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "dim of source (" << source->dim() << ") does not match dim_source of this (" << dim_source()
                     << ")!");
  if (range->dim() != dim_range())
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "dim of range (" << range->dim() << ") does not match dim_range of this (" << dim_range()
                     << ")!");
  LA::VectorInterface* tmp = LA::createVector(type_range(), dim_range());
  apply(source, tmp, mu);
  return range->dot(tmp);
}

OperatorInterface* OperatorInterface::freeze_parameter(const Parameter /*mu*/) const
  throw (Exception::this_is_not_parametric,
         Exception::you_have_to_implement_this,
         Exception::this_does_not_make_any_sense)
{
  DUNE_PYMOR_THROW(Exception::you_have_to_implement_this, "you really do!");
}


// ===================================
// ===== LinearOperatorInterface =====
// ===================================
LinearOperatorInterface::LinearOperatorInterface()
  : OperatorInterface()
{}

LinearOperatorInterface::LinearOperatorInterface(const ParameterType& tt)
  : OperatorInterface(tt)
{}

LinearOperatorInterface::LinearOperatorInterface(const std::string& kk, const int& vv)
  throw (Exception::key_is_not_valid, Exception::index_out_of_range)
  : OperatorInterface(kk, vv)
{}

LinearOperatorInterface::LinearOperatorInterface(const std::vector< std::string >& kk,
                                                 const std::vector< int >& vv) throw (Exception::key_is_not_valid,
                                                                                      Exception::index_out_of_range,
                                                                                      Exception::sizes_do_not_match)
  : OperatorInterface(kk, vv)
{}

LinearOperatorInterface::LinearOperatorInterface(const Parametric& other)
  : OperatorInterface(other)
{}

LinearOperatorInterface::~LinearOperatorInterface()
{}

bool LinearOperatorInterface::linear() const
{
  return true;
}


// ===============================================
// ===== AffinelyDecomposedOperatorInterface =====
// ===============================================
AffinelyDecomposedOperatorInterface::AffinelyDecomposedOperatorInterface()
  : OperatorInterface()
{}

AffinelyDecomposedOperatorInterface::AffinelyDecomposedOperatorInterface(const ParameterType& tt)
  : OperatorInterface(tt)
{}

AffinelyDecomposedOperatorInterface::AffinelyDecomposedOperatorInterface(const std::string& kk, const int& vv)
  throw (Exception::key_is_not_valid, Exception::index_out_of_range)
  : OperatorInterface(kk, vv)
{}

AffinelyDecomposedOperatorInterface::AffinelyDecomposedOperatorInterface(const std::vector< std::string >& kk,
                                                                         const std::vector< int >& vv)
  throw (Exception::key_is_not_valid, Exception::index_out_of_range, Exception::sizes_do_not_match)
  : OperatorInterface(kk, vv)
{}

AffinelyDecomposedOperatorInterface::AffinelyDecomposedOperatorInterface(const Parametric& other)
  : OperatorInterface(other)
{}

AffinelyDecomposedOperatorInterface::~AffinelyDecomposedOperatorInterface()\
{}


} // namespace Pymor
} // namespace Dune
