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
namespace LA {


VectorInterface::~VectorInterface()
{}

bool VectorInterface::almost_equal(const Dune::Pymor::LA::VectorInterface* other,
                                   const double /*epsilon*/) const throw (Exception::types_are_not_compatible,
                                                                          Exception::sizes_do_not_match,
                                                                          Exception::you_have_to_implement_this)
{
  if (other->type() == type())
    DUNE_PYMOR_THROW(Exception::you_have_to_implement_this, "you really do!");
  else
    DUNE_PYMOR_THROW(Exception::types_are_not_compatible,
                     "this (" << type() << ") is not compatible with other (" << other->type() << ")!");
  return false;
}

void VectorInterface::axpy(const double /*alpha*/, const Dune::Pymor::LA::VectorInterface* x)
  throw (Exception::types_are_not_compatible, Exception::sizes_do_not_match, Exception::you_have_to_implement_this)
{
  if (x->type() == type())
    DUNE_PYMOR_THROW(Exception::you_have_to_implement_this, "you really do!");
  else
    DUNE_PYMOR_THROW(Exception::types_are_not_compatible,
                     "this (" << type() << ") is not compatible with x (" << x->type() << ")!");
}

double VectorInterface::dot(const Dune::Pymor::LA::VectorInterface* other) const
  throw (Exception::types_are_not_compatible, Exception::sizes_do_not_match, Exception::you_have_to_implement_this)
{
  if (other->type() == type())
    DUNE_PYMOR_THROW(Exception::you_have_to_implement_this, "you really do!");
  else
    DUNE_PYMOR_THROW(Exception::types_are_not_compatible,
                     "this (" << type() << ") is not compatible with other (" << other->type() << ")!");
  return 0.0;
}

Dune::Pymor::LA::VectorInterface* VectorInterface::add(const Dune::Pymor::LA::VectorInterface* other) const
  throw (Exception::types_are_not_compatible, Exception::sizes_do_not_match, Exception::you_have_to_implement_this)
{
  if (other->type() == type())
    DUNE_PYMOR_THROW(Exception::you_have_to_implement_this, "you really do!");
  else
    DUNE_PYMOR_THROW(Exception::types_are_not_compatible,
                     "this (" << type() << ") is not compatible with other (" << other->type() << ")!");
  return nullptr;
}

void VectorInterface::iadd(const Dune::Pymor::LA::VectorInterface* other)
  throw (Exception::types_are_not_compatible, Exception::sizes_do_not_match, Exception::you_have_to_implement_this)
{
  if (other->type() == type())
    DUNE_PYMOR_THROW(Exception::you_have_to_implement_this, "you really do!");
  else
    DUNE_PYMOR_THROW(Exception::types_are_not_compatible,
                     "this (" << type() << ") is not compatible with other (" << other->type() << ")!");
}

Dune::Pymor::LA::VectorInterface* VectorInterface::sub(const Dune::Pymor::LA::VectorInterface* other) const
  throw (Exception::types_are_not_compatible, Exception::sizes_do_not_match, Exception::you_have_to_implement_this)
{
  if (other->type() == type())
    DUNE_PYMOR_THROW(Exception::you_have_to_implement_this, "you really do!");
  else
    DUNE_PYMOR_THROW(Exception::types_are_not_compatible,
                     "this (" << type() << ") is not compatible with other (" << other->type() << ")!");
  return nullptr;
}

void VectorInterface::isub(const Dune::Pymor::LA::VectorInterface* other)
  throw (Exception::types_are_not_compatible, Exception::sizes_do_not_match, Exception::you_have_to_implement_this)
{
  if (other->type() == type())
    DUNE_PYMOR_THROW(Exception::you_have_to_implement_this, "you really do!");
  else
    DUNE_PYMOR_THROW(Exception::types_are_not_compatible,
                     "this (" << type() << ") is not compatible with other (" << other->type() << ")!");
}

bool VectorInterface::linear() const
{
  return true;
}

unsigned int VectorInterface::dim_source() const
{
  return dim();
}

std::string VectorInterface::type_source() const
{
  return type();
}

double VectorInterface::apply(const LA::VectorInterface* source,
                              const Parameter /*mu*/) const throw (Exception::types_are_not_compatible,
                                                                   Exception::you_have_to_implement_this,
                                                                   Exception::sizes_do_not_match,
                                                                   Exception::wrong_parameter_type,
                                                                   Exception::requirements_not_met,
                                                                   Exception::linear_solver_failed,
                                                                   Exception::this_does_not_make_any_sense)
{
  if (source->type() == type_source())
    DUNE_PYMOR_THROW(Exception::you_have_to_implement_this, "you really do!");
  else
    DUNE_PYMOR_THROW(Exception::types_are_not_compatible,
                     "the type of source (" << source->type() << ") does not match the type_source of this ("
                     << type_source() << ")!");
  return 0.0;
}


} // namespace LA
} // namespace Pymor
} // namespace Dune
