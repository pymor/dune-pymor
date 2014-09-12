// This file is part of the dune-pymor project:
//   https://github.com/pymor/dune-pymor
// Copyright holders: Stephan Rave, Felix Schindler
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_FUNCTIONS_HH
#define DUNE_PYMOR_FUNCTIONS_HH

#include <string>
#include <vector>

#include <dune/stuff/common/configuration.hh>
#include <dune/stuff/common/memory.hh>
#include <dune/stuff/functions.hh>

#include "functions/interfaces.hh"
#include "functions/default.hh"
#include "functions/checkerboard.hh"

namespace Dune {
namespace Pymor {


/**
 * \note If you want to add a new function FooBar, do the following: provide a definition that is available for all
 *       template arguments, like:
\code
template< class E, class D, int d, class R, int r, int rC = 1 >
class FooBar
  : public AffinelyDecomposableFunctionInterface< E, D, d, R, r, rC >
{
  FooBar() { static_assert(AlwaysFalse< E >::value, "Not available for these dimensions!"); }
};
\endcode
 *       Every specialization that can be provided by the provider then has to define:
\code
static const bool available = true;
\endcode
 *       This is all you have to do when implementing the function. In addition you have to add the appropriate include
 *       in this file (of course) and the appropriate type below (just like the rest, should be obvious).
 */
template< class E, class D, int d, class R, int r, int rC = 1 >
class AffinelyDecomposableFunctionsProvider
{
public:
  typedef AffinelyDecomposableFunctionInterface< E, D, d, R, r, rC > InterfaceType;

private:
  template< class F, bool available = false >
  struct Call
  {
    static std::vector< std::string > append(std::vector< std::string > in)
    {
      return in;
    }

    static bool compare(const std::string& /*type*/)
    {
      return false;
    }

    static Stuff::Common::Configuration default_config(const std::string /*sub_name*/)
    {
      DUNE_THROW(Stuff::Exceptions::internal_error, "This should not happen!");
      return Stuff::Common::Configuration(0);
    }

    static std::unique_ptr< F > create(const Stuff::Common::Configuration& /*cfg*/)
    {
      DUNE_THROW(Stuff::Exceptions::internal_error, "This should not happen!");
      return std::unique_ptr< F >(nullptr);
    }
  }; // struct Call

  template< class F >
  struct Call< F, true >
  {
    static std::vector< std::string > append(std::vector< std::string > in)
    {
      in.push_back(F::static_id());
      return in;
    }

    static bool compare(const std::string& type)
    {
      return type == F::static_id();
    }

    static Stuff::Common::Configuration default_config(const std::string sub_name)
    {
      return F::default_config(sub_name);
    }

    static std::unique_ptr< F > create(const Stuff::Common::Configuration& cfg)
    {
      if (cfg.empty())
        return F::create();
      else
        return F::create(cfg);
    }
  }; // struct Call< ..., true >

  template< class F >
  static std::vector< std::string > call_append(std::vector< std::string > in)
  {
    return Call< F, F::available >::append(in);
  }

  template< class F >
  static bool call_compare(const std::string& type)
  {
    return Call< F, F::available >::compare(type);
  }

  template< class F >
  static Stuff::Common::Configuration call_default_config(const std::string sub_name)
  {
    return Call< F, F::available >::default_config(sub_name);
  }

  template< class F >
  static std::unique_ptr< F > call_create(const Stuff::Common::Configuration& cfg)
  {
    return Call< F, F::available >::create(cfg);
  }

  static std::string available_as_str()
  {
    std::string ret = "";
    const auto vals = available();
    if (vals.size() > 0) {
      ret += vals[0];
      for (size_t ii = 1; ii < vals.size(); ++ii)
        ret += "\n   " + vals[ii];
    }
    return ret;
  } // ... available_as_str(...)

  typedef Functions::AffinelyDecomposableDefault< E, D, d, R, r, rC > AffinelyDecomposableDefaultType;
  typedef Functions::Checkerboard< E, D, d, R, r, rC >                CheckerboardType;

  typedef Stuff::FunctionsProvider< E, D, d, R, r, rC >       NonparametricProvider;
  typedef Functions::NonparametricDefault< E, D, d, R, r, rC > WrapperType;

public:
  static std::vector< std::string > available()
  {
    auto ret = NonparametricProvider::available();
    ret = call_append< AffinelyDecomposableDefaultType >(ret);
    ret = call_append< CheckerboardType >(ret);
    return ret;
  } // ... available(...)

  static Stuff::Common::Configuration default_config(const std::string type, const std::string sub_name = "")
  {
    if (call_compare< AffinelyDecomposableDefaultType >(type))
      return call_default_config< AffinelyDecomposableDefaultType >(sub_name);
    else if (call_compare< CheckerboardType >(type))
      return call_default_config< CheckerboardType >(sub_name);
    else
      return NonparametricProvider::default_config(type, sub_name);
  } // ... default_config(...)

  static std::unique_ptr< InterfaceType > create(const std::string type = available()[0],
                                                 const Stuff::Common::Configuration cfg = Stuff::Common::Configuration())
  {
    if (call_compare< AffinelyDecomposableDefaultType >(type))
      return call_create< AffinelyDecomposableDefaultType >(cfg);
    else if (call_compare< CheckerboardType >(type))
      return call_create< CheckerboardType >(cfg);
    else
      return Stuff::Common::make_unique< WrapperType >(NonparametricProvider::create(type, cfg));
  } // ... create(...)
}; // class AffinelyDecomposableFunctionsProvider


} // namespace Pymor
} // namespace Dune

#endif // DUNE_PYMOR_FUNCTIONS_HH
