// This file is part of the dune-pymor project:
//   https://github.com/pymor/dune-pymor
// Copyright holders: Stephan Rave, Felix Schindler
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_COMMON_EXCEPTIONS_HH
#define DUNE_PYMOR_COMMON_EXCEPTIONS_HH

#include <dune/stuff/common/exceptions.hh>

namespace Dune {
namespace Pymor {
namespace Exceptions {


class wrong_parameter_type : public Exception {};

class wrong_parameter : public Exception {};


} // namespace Exceptions
} // namespace Pymor
} // namespace Dune


/**
 *  \brief Macro to throw an exception in dune-pymor.
 *
\code
#include <dune/pymor/common/exceptions.hh>
\endcode
 *  \param  E Exception class, derived from Dune::PymorException.
 *  \param  m Message in ostream notation.
 *
 *  Example:
\code
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
  std::ostringstream __attribute__((deprecated)) /* use DUNE_THROW instead (will be removed on 03.09.2014)! */ th__msg; \
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
\code
class foo_is_not_bar : public Dune::PymorException {};
\endcode
 *
 *        Instead of throwing a Dune::PymorException directly, it is convenient to use the macro DUNE_PYMOR_THROW().
 * \see   DUNE_PYMOR_THROW, Dune::Exception
 */
class
  DUNE_DEPRECATED_MSG("Use one of the exceptions in Stuff::Exceptions:: or Pymor::Exceptions instead (will be removed on 03.09.2014)!")
      PymorException
  : public Dune::Exception
{
public:
  //! needed, to avoid ambiguity of message()
  void pymorMessage(const std::string& msg)
  {
    Dune::Exception::message(msg);
  }

  //! needed for the python bindings
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
class
  DUNE_DEPRECATED_MSG("Use one of the exceptions in Stuff::Exceptions:: or Pymor::Exceptions instead (will be removed on 03.09.2014)!")
     types_are_not_compatible
  : public Dune::NotImplemented
  , public Dune::PymorException
{};

/**
 * \brief Exception for the case where two arguments do not match in size, i.e. two vectors do not have the same lenght.
 */
class
  DUNE_DEPRECATED_MSG("Use one of the exceptions in Stuff::Exceptions:: or Pymor::Exceptions instead (will be removed on 03.09.2014)!")
     sizes_do_not_match
  : public Dune::RangeError
  , public Dune::PymorException
{};

/**
 * \brief Exception for indices which are out of the admissible range.
 */
class
  DUNE_DEPRECATED_MSG("Use one of the exceptions in Stuff::Exceptions:: or Pymor::Exceptions instead (will be removed on 03.09.2014)!")
     index_out_of_range
  : public Dune::RangeError
  , public Dune::PymorException
{};

/**
 * \brief Exception for invalied keys in a dictionary/map context, i.e. an empty string "".
 */
class
  DUNE_DEPRECATED_MSG("Use one of the exceptions in Stuff::Exceptions:: or Pymor::Exceptions instead (will be removed on 03.09.2014)!")
      key_is_not_valid
  : public Dune::RangeError
  , public Dune::PymorException
{};

/**
 * \brief Exception for methods the user has to implement.
 */
class
  DUNE_DEPRECATED_MSG("Use one of the exceptions in Stuff::Exceptions:: or Pymor::Exceptions instead (will be removed on 03.09.2014)!")
     you_have_to_implement_this
  : public Dune::NotImplemented
  , public Dune::PymorException
{};

/**
 * \brief Exception for a wrong Dune::Pymor::ParameterType.
 */
class
  DUNE_DEPRECATED_MSG("Use one of the exceptions in Stuff::Exceptions:: or Pymor::Exceptions instead (will be removed on 03.09.2014)!")
     wrong_parameter_type
  : public Dune::RangeError
  , public Dune::PymorException
{};

/**
 * \brief Exception for nonparametric objects that are treated as if they were parametric.
 */
class
  DUNE_DEPRECATED_MSG("Use one of the exceptions in Stuff::Exceptions:: or Pymor::Exceptions instead (will be removed on 03.09.2014)!")
      this_is_not_parametric
  : public Dune::InvalidStateException
  , public Dune::PymorException
{};

/**
 * \brief Exception for unlogical situations.
 */
class
  DUNE_DEPRECATED_MSG("Use one of the exceptions in Stuff::Exceptions:: or Pymor::Exceptions instead (will be removed on 03.09.2014)!")
     this_does_not_make_any_sense
  : public Dune::InvalidStateException
  , public Dune::PymorException
{};

/**
 * \brief Exception for calling an object in an unsuitable state, i.e. if an Dune::Pymor::AffinelyDecomposedOperato
 *        reports hasAffinePart() == false and the user calls affinePart();
 */
class
  DUNE_DEPRECATED_MSG("Use one of the exceptions in Stuff::Exceptions:: or Pymor::Exceptions instead (will be removed on 03.09.2014)!")
      requirements_not_met
  : public Dune::InvalidStateException
  , public Dune::PymorException
{};

/**
 * \brief Exception for wrong input arguments.
 */
class
  DUNE_DEPRECATED_MSG("Use one of the exceptions in Stuff::Exceptions:: or Pymor::Exceptions instead (will be removed on 03.09.2014)!")
      wrong_input
  : public Dune::RangeError
  , public Dune::PymorException
{};

/**
 * \brief Exception for operators/matrices which are not invertible.
 */
class
  DUNE_DEPRECATED_MSG("Use one of the exceptions in Stuff::Exceptions:: or Pymor::Exceptions instead (will be removed on 03.09.2014)!")
     not_invertible
  : public Dune::MathError
  , public Dune::PymorException
{};

/**
 * \brief Exception for linear solvers which did not succeed.
 */
class
  DUNE_DEPRECATED_MSG("Use one of the exceptions in Stuff::Exceptions:: or Pymor::Exceptions instead (will be removed on 03.09.2014)!")
     linear_solver_failed
  : public Dune::MathError
  , public Dune::PymorException
{};

/**
 * \brief Exception for input/output errors.
 */
class
  DUNE_DEPRECATED_MSG("Use one of the exceptions in Stuff::Exceptions:: or Pymor::Exceptions instead (will be removed on 03.09.2014)!")
     io_error
  : public Dune::IOError
  , public Dune::PymorException
{};

/**
 * \brief Exception for wrong options.
 */
class
  DUNE_DEPRECATED_MSG("Use one of the exceptions in Stuff::Exceptions:: or Pymor::Exceptions instead (will be removed on 03.09.2014)!")
      wrong_option_given
  : public Dune::RangeError
  , public Dune::PymorException
{};


} // namespace Exceptions
} // namespace Pymor
} // namespace Dune

#endif // DUNE_PYMOR_COMMON_EXCEPTIONS_HH
