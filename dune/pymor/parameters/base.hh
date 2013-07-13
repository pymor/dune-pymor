// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_PARAMETERS_BASE_HH
#define DUNE_PYMOR_PARAMETERS_BASE_HH

#include <string>
#include <map>
#include <vector>
#include <initializer_list>
#include <sstream>
#include <ostream>

#include <dune/pymor/common/exceptions.hh>

namespace Dune {
namespace Pymor {


/**
 * \brief Base class for ParameterType and Parameter.
 * \see   Dune::Pymor::ParameterType, Dune::Pymor::Parameter
 */
template< class KeyType, class ValueType >
class KeyValueBase
{
public:
  KeyValueBase();

  virtual ~KeyValueBase();

  KeyValueBase(const KeyType& kk, const ValueType& vv);

  KeyValueBase(const std::vector< KeyType >& kk,
               const std::vector< ValueType >& vv) throw (Exception::key_is_not_valid,
                                                          Exception::sizes_do_not_match);

  bool empty() const;

  const std::vector< KeyType >& keys() const;

  const std::vector< ValueType >& values() const;

  bool hasKey(const KeyType& key) const;

  virtual void set(const KeyType& key, const ValueType& value) throw (Exception::index_out_of_range,
                                                                      Exception::key_is_not_valid,
                                                                      Exception::sizes_do_not_match) = 0;

  const ValueType& get(const KeyType& key) const throw (Exception::key_is_not_valid);

  bool operator==(const KeyValueBase< KeyType, ValueType >& other) const;

  bool operator!=(const KeyValueBase< KeyType, ValueType >& other) const;

  unsigned int size() const;

  virtual std::string report() const = 0;

protected:
  void update();

  std::map< KeyType, ValueType > dict_;
  std::vector< KeyType > keys_;
  std::vector< ValueType > values_;
}; // class KeyValueBase


/**
 * \brief Determines the type of any parameter as a set of components and their length.
 *
 *        This class is implemented using a std::map, where the components are std::string keys and their length are
 *        int values. We use int instead of size_t because pybindgen does not offer any bindings for size_t. Usage
 *        example:
\code
ParameterType type1("diffusion", 1);
ParameterType type2({"diffusion", "force"},
                    {1, 2});
ParameterType type3 = {"diffusion", 1};
ParameterType type4 = {{"diffusion", "force"}, {1, 2}};
\code
 *
 * \see   Dune::Pymor::Parameter, Dune::Pymor::Parametric
 */
class ParameterType
  : public KeyValueBase< std::string, int >
{
  typedef KeyValueBase< std::string, int > BaseType;
public:
  typedef std::string KeyType;
  typedef int         ValueType;

  /**
   * \brief Empty constructor to please pybindgen.
   */
  ParameterType();

  ParameterType(const KeyType& kk, const ValueType& vv) throw (Exception::key_is_not_valid,
                                                               Exception::index_out_of_range);

  ParameterType(const std::vector< KeyType >& kk,
                const std::vector< ValueType >& vv) throw (Exception::key_is_not_valid,
                                                           Exception::index_out_of_range,
                                                           Exception::sizes_do_not_match);

  virtual void set(const KeyType& key, const ValueType& value) throw (Exception::key_is_not_valid,
                                                                      Exception::index_out_of_range);

  virtual std::string report() const;

  using BaseType::keys;
  using BaseType::values;
  using BaseType::hasKey;
  using BaseType::get;
  using BaseType::operator==;
  using BaseType::operator!=;
  using BaseType::size;
}; // class ParameterType


std::ostream& operator<<(std::ostream& oo, const ParameterType& pp);


class Parameter
  : public KeyValueBase< std::string, std::vector< double > >
{
  typedef KeyValueBase< std::string, std::vector< double > > BaseType;
public:
  typedef std::string           KeyType;
  typedef std::vector< double > ValueType;

  Parameter();

  Parameter(const KeyType& kk, const double& vv);

  Parameter(const ParameterType& tt, const double& vv) throw (Exception::sizes_do_not_match);

  Parameter(const KeyType& kk, const ValueType& vv) throw (Exception::key_is_not_valid, Exception::sizes_do_not_match);

  Parameter(const ParameterType& tt, const ValueType& vv) throw (Exception::sizes_do_not_match);

  Parameter(const std::vector< KeyType >& kk,
            const std::vector< ValueType >& vv) throw (Exception::key_is_not_valid,
                                                       Exception::sizes_do_not_match);

  Parameter(const ParameterType& tt,
            const std::vector< ValueType >& vv) throw (Exception::key_is_not_valid,
                                                       Exception::sizes_do_not_match);

  const ParameterType& type() const;

  void set(const KeyType& key, const ValueType& value) throw (Exception::key_is_not_valid,
                                                              Exception::sizes_do_not_match);

  std::string report() const;

  ValueType serialize() const;

  using BaseType::keys;
  using BaseType::values;
  using BaseType::hasKey;
  using BaseType::get;
  using BaseType::operator==;
  using BaseType::operator!=;
  using BaseType::size;

private:
  ParameterType type_;
}; // class Parameter


std::ostream& operator<<(std::ostream& oo, const Parameter& pp);


/**
 * \brief Base class for everything that can possibly parametric.
 *
 * \attention A derived class Foo has to provide a method
\code
Foo* freeze_parameter(const Parameter mu = Parameter()) const
\endcode
 *            which returns a nonparametric version of the object, if possible.
 * \attention Any derived class FooInterface which serves as base class should provide a constructor, which forwards
 *            the creation of the ParameterType to Parametric, i.e. something like
\code
template< class... Args >
FooInterface(Args&& ...args)
  : Parametric(std::forward< Args >(args)...)
{}
\endcode
 */
class Parametric
{
public:
  Parametric();

  Parametric(const ParameterType& tt);

  Parametric(const std::string& kk, const int& vv) throw (Exception::key_is_not_valid,
                                                          Exception::index_out_of_range);

  Parametric(const std::vector< std::string >& kk,
             const std::vector< int >& vv) throw (Exception::key_is_not_valid,
                                                  Exception::index_out_of_range,
                                                  Exception::sizes_do_not_match);

  Parametric(const Parametric& other);

  virtual ~Parametric();

  const ParameterType& parameter_type() const;

  bool parametric() const;

protected:
  void inherit_parameter_type(const ParameterType& tt, const std::string id) throw (Exception::sizes_do_not_match,
                                                                                    Exception::key_is_not_valid);

  void inherit_parameter_type(const Parameter& mu, const std::string id) throw (Exception::sizes_do_not_match,
                                                                                Exception::key_is_not_valid);

  const ParameterType& map_parameter_type(const std::string id) const throw (Exception::key_is_not_valid,
                                                                             Exception::requirements_not_met);

  Parameter map_parameter(const Parameter& mu, const std::string id) const throw (Exception::key_is_not_valid,
                                                                                  Exception::requirements_not_met);

  void replace_parameter_type(const ParameterType tt = ParameterType());

private:
  ParameterType type_;
  std::map< std::string, ParameterType > inherits_map_;
}; // class Parametric


} // namespace Pymor
} // namespace Dune

#endif // DUNE_PYMOR_PARAMETERS_BASE_HH
