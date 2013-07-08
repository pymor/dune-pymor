// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_COMMON_EXCEPTIONS_HH
#define DUNE_PYMOR_COMMON_EXCEPTIONS_HH

#include <dune/common/exceptions.hh>

#include <dune/stuff/common/color.hh>

/**
 *  \brief Macro to throw an exception in dune-pymor.
 *
 *  \code
#include <dune/pymor/common/exceptions.hh>
\endcode
 *  \param  E Exception class, derived from Dune::PymorException.
 *  \param  m Message in ostream notation.
 *
 *  Example:
 *  \code
if (a.size() != b.size)
  DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                   "size of a (" << a.size() << ") does not match the size of b (" << b.size() << ")!");
\endcode
 *
 *  This macro is essentially copied from dune-common with added color functionality from dune-stuff.
 *  \see DUNE_THROW, Dune::PymorException
 */
#define DUNE_PYMOR_THROW(E, m) do {\
  E th__ex; \
  std::ostringstream th__msg; \
  th__msg << m; \
  std::ostringstream th__out; \
  th__out << Dune::Stuff::Common::Colors::red << # E << "\033[0m\n"; \
  th__out << Dune::Stuff::Common::Colors::red << "[\033[0m"; \
  th__out << __func__; \
  th__out << Dune::Stuff::Common::Colors::red << "|\033[0m"; \
  th__out << __FILE__ << Dune::Stuff::Common::Colors::red << ":" << __LINE__ << "]\033[0m"; \
  if (!th__msg.str().empty()) th__out << "\n" << Dune::Stuff::Common::Colors::red << "=>\033[0m " << th__msg.str(); \
  th__ex.pymorMessage(th__out.str()); \
  throw th__ex; \
} while (0)

namespace Dune {

/**
 * \brief Common base for all exceptions in dune-pymor.
 *
 *        All dune-pymor exceptions are derived from this class:
 * \code
class foo_is_not_bar : public Dune::PymorException {};
\endcode
 *
 *        Instead of throwing a Dune::PymorException directly, it is convenient to use the macro DUNE_PYMOR_THROW().
 * \see   DUNE_PYMOR_THROW, Dune::Exception
 */
class PymorException
  : public Dune::Exception
{
public:
  //! needed, to avoid ambiguity of message()
  void pymorMessage(const std::string& msg)
  {
    Dune::Exception::message(msg);
  }

  const char* pymorWhat() const
  {
    return Dune::Exception::what().c_str();
  }
};

namespace Pymor {
namespace Exception {

/**
 * \brief Exception for specializations which are not implemented for the given arguments.
 *
 *        This can be used when dynamically deriving from an interface, see Dune::Pymor::La::VectorInterface for an
 *        example.
 */
class types_are_not_compatible
  : public Dune::NotImplemented
  , public Dune::PymorException
{};

/**
 * \brief Exception for the case where two arguments do not match in size, i.e. two vectors do not have the same lenght.
 */
class sizes_do_not_match
  : public Dune::RangeError
  , public Dune::PymorException
{};

/**
 * \brief Exception for indices which are out of the admissible range.
 */
class index_out_of_range
  : public Dune::RangeError
  , public Dune::PymorException
{};

/**
 * \brief Exception for invalied keys in a dictionary/map context, i.e. an empty string "".
 */
class key_is_not_valid
  : public Dune::RangeError
  , public Dune::PymorException
{};

/**
 * \brief Exception for methods the user has to implement.
 */
class you_have_to_implement_this
  : public Dune::NotImplemented
  , public Dune::PymorException
{};

/**
 * \brief Exception for a wrong Dune::Pymor::ParameterType .
 */
class wrong_parameter_type
  : public Dune::RangeError
  , public Dune::PymorException
{};

/**
 * \brief Exception for nonparametric objects that are treated as if they were parametric.
 */
class this_is_not_parametric
    : public Dune::InvalidStateException
    , public Dune::PymorException
  {};

/**
 * \brief Exception for unlogical situations.
 */
class this_does_not_make_any_sense
    : public Dune::InvalidStateException
    , public Dune::PymorException
  {};

/**
 * \brief Exception for calling an object in an unsuitable state, i.e. if an Dune::Pymor::AffinelyDecomposedOperato
 *        reports hasAffinePart() == false and the user calls affinePart();
 */
class requirements_not_met
    : public Dune::InvalidStateException
    , public Dune::PymorException
  {};


} // namespace Exceptions
} // namespace Pymor
} // namespace Dune

#endif // DUNE_PYMOR_COMMON_EXCEPTIONS_HH
