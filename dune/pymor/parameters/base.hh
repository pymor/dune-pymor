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
  KeyValueBase()
  {}

  virtual ~KeyValueBase() {}

  KeyValueBase(const KeyType& kk, const ValueType& vv)
    : dict_({std::make_pair(kk, vv)})
  {
    update();
  }

  KeyValueBase(const std::vector< KeyType >& kk,
               const std::vector< ValueType >& vv) throw (Exception::key_is_not_valid,
                                                          Exception::sizes_do_not_match)
  {
    if (kk.size() != vv.size())
      DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                       "the size of kk (" << kk.size() << ") has to equal the size of vv (" << vv.size() << ")!");
    if (kk.size() == 0) DUNE_PYMOR_THROW(Exception::sizes_do_not_match, "kk and vv are empty!");
    const auto kk_end = kk.end();
    const auto vv_end = vv.end();
    auto kk_it = kk.begin();
    size_t counter = 0;
    for (auto vv_it = vv.begin(); (kk_it != kk_end) && (vv_it != vv_end); ++kk_it, ++vv_it, ++counter) {
      const KeyType& key = *kk_it;
      if (key.empty()) DUNE_PYMOR_THROW(Exception::key_is_not_valid, "kk[" << counter << "] is empty!");
      const ValueType& value = *vv_it;
      dict_.insert(std::make_pair(key, value));
    }
    update();
  }

  const std::vector< KeyType >& keys() const
  {
    return keys_;
  }

  const std::vector< ValueType >& values() const
  {
    return values_;
  }

  bool hasKey(const KeyType& key) const
  {
    const auto result = dict_.find(key);
    return result != dict_.end();
  }

  virtual void set(const KeyType& key, const ValueType& value) throw (Exception::index_out_of_range,
                                                                      Exception::key_is_not_valid,
                                                                      Exception::sizes_do_not_match) = 0;

  ValueType get(const KeyType& key) const throw (Exception::key_is_not_valid)
  {
    const auto result = dict_.find(key);
    if (result == dict_.end()) DUNE_PYMOR_THROW(Exception::key_is_not_valid, "key does not exist!");
    return result->second;
  }

  bool operator==(const KeyValueBase< KeyType, ValueType >& other) const
  {
    return dict_ == other.dict_;
  }

  bool operator!=(const KeyValueBase< KeyType, ValueType >& other) const
  {
    return dict_ != other.dict_;
  }

  unsigned int size() const
  {
    return dict_.size();
  }

  virtual std::string report() const = 0;

protected:
  void update()
  {
    if (dict_.size() > 0) {
      keys_ = std::vector< KeyType >();
      values_ = std::vector< ValueType >();
      const auto dict_end = dict_.end();
      for (auto dict_it = dict_.begin(); dict_it != dict_end; ++dict_it) {
        keys_.push_back(dict_it->first);
        values_.push_back(dict_it->second);
      }
    }
  } // void update()

  std::map< KeyType, ValueType > dict_;
  std::vector< KeyType > keys_;
  std::vector< ValueType > values_;

//  friend std::ostream& operator<<(std::ostream&, const ParameterType&);
}; // class KeyValueBase


//template< class KeyType, class ValueType >
//std::ostream& operator<<(std::ostream& oo, const KeyValueBase< KeyType, ValueType >& pp)
//{
//  oo << pp.report();
//  return oo;
//}


/**
 * \brief Determines the type of any parameter as a set of components and their length.
 *
 *        This class is implemented using a std::map, where the components are std::string keys and their length are
 *        int values. We use int instead of size_t because pybindgen does not offer any bindings for size_t. Usage
 *        example:
 *        \code
ParameterType param1("diffusion", 1);
ParameterType param2({"diffusion", "force"},
                     {1, 2});
ParameterType param3 = {"diffusion", 1};
ParameterType param4 = {{"diffusion", "force"}, {1, 2}};
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
  ParameterType()
  {}

  ParameterType(const KeyType& kk, const ValueType& vv) throw (Exception::key_is_not_valid,
                                                               Exception::index_out_of_range)
    : BaseType(kk, vv)
  {
    if (kk.empty()) DUNE_PYMOR_THROW(Exception::key_is_not_valid, "kk is empty!");
    if (vv <= 0) DUNE_PYMOR_THROW(Exception::index_out_of_range, "vv has to be positive (is " << vv << ")!");
  }

  ParameterType(const std::vector< KeyType >& kk,
                const std::vector< ValueType >& vv) throw (Exception::key_is_not_valid,
                                                           Exception::index_out_of_range,
                                                           Exception::sizes_do_not_match)
    : BaseType(kk, vv)
  {
    if (kk.size() != vv.size())
      DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                       "the size of kk (" << kk.size() << ") has to equal the size of vv (" << vv.size() << ")!");
    if (kk.size() == 0) DUNE_PYMOR_THROW(Exception::sizes_do_not_match, "kk and vv are empty!");
    const auto kk_end = kk.end();
    const auto vv_end = vv.end();
    auto kk_it = kk.begin();
    size_t counter = 0;
    for (auto vv_it = vv.begin(); (kk_it != kk_end) && (vv_it != vv_end); ++kk_it, ++vv_it, ++counter) {
      const KeyType& key = *kk_it;
      const ValueType& value = *vv_it;
      if (key.empty()) DUNE_PYMOR_THROW(Exception::key_is_not_valid, "kk[" << counter << "] is empty!");
      if (value <= 0)
        DUNE_PYMOR_THROW(Exception::index_out_of_range,
                         "vv[" << counter << "] has to be positive (is " << value << ")!");
    }
  }

  virtual void set(const KeyType& key, const ValueType& value) throw (Exception::key_is_not_valid,
                                                                      Exception::index_out_of_range)
  {
    if (key.empty()) DUNE_PYMOR_THROW(Exception::key_is_not_valid, "key is empty!");
    if (value <= 0) DUNE_PYMOR_THROW(Exception::index_out_of_range, "value has to be positive (is " << value << ")!");
    if (!hasKey(key)) {
      BaseType::dict_[key] = value;
      BaseType::update();
    }
  }

  virtual std::string report() const
  {
    std::ostringstream ret;
    ret << "Dune::Pymor::ParameterType(";
    if (dict_.size() == 1) {
      const auto element = dict_.begin();
      ret << "\"" << element->first << "\", " << element->second;
    } else if (dict_.size() > 1) {
      const auto& kk = keys();
      const auto& vv = values();
      ret << "{\"";
      for (size_t ii = 0; ii < (kk.size() - 1); ++ii)
        ret << kk[ii] << "\", \"";
      ret << kk[kk.size() - 1] << "\"}, {";
      for (size_t ii = 0; ii < (vv.size() - 1); ++ii)
        ret << vv[ii] << ", ";
      ret << vv[vv.size() - 1] << "}";
    }
    ret << ")";
    return ret.str();
  }

  using BaseType::keys;
  using BaseType::values;
  using BaseType::hasKey;
  using BaseType::get;
  using BaseType::operator==;
  using BaseType::operator!=;
  using BaseType::size;
}; // class ParameterType


class Parameter
  : public KeyValueBase< std::string, std::vector< double > >
{
  typedef KeyValueBase< std::string, std::vector< double > > BaseType;
public:
  typedef std::string           KeyType;
  typedef std::vector< double > ValueType;

  Parameter()
    : BaseType()
  {}

  Parameter(const KeyType& kk, const double& vv)
    : BaseType(kk, {vv})
    , type_(kk, 1)
  {}

  Parameter(const ParameterType& tt, const double& vv) throw (Exception::sizes_do_not_match)
    : BaseType(tt.keys(), {{vv}})
    , type_(tt)
  {
    if (tt.size() == 0) DUNE_PYMOR_THROW(Exception::sizes_do_not_match, "tt is empty!");
    if (tt.size() != 1) DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                                         "tt should be of size 1 (is " << tt.size() << ")!");
    if (tt.values()[0] != 1)
      DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                       "tt.get(\"" << tt.keys()[0] << "\") should be 1 (is " << tt.values()[0] << ")!");
  }

  Parameter(const KeyType& kk, const ValueType& vv) throw (Exception::key_is_not_valid, Exception::sizes_do_not_match)
    : BaseType(kk, vv)
    , type_(kk, vv.size())
  {
    if (kk.empty()) DUNE_PYMOR_THROW(Exception::key_is_not_valid, "kk is empty!");
    if (vv.size() == 0) DUNE_PYMOR_THROW(Exception::sizes_do_not_match, "vv is empty!");
  }

  Parameter(const ParameterType& tt, const ValueType& vv) throw (Exception::sizes_do_not_match)
    : BaseType(tt.keys(), {vv})
    , type_(tt)
  {
    if (tt.size() == 0) DUNE_PYMOR_THROW(Exception::sizes_do_not_match, "tt is empty!");
    if (tt.size() != 1) DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                                         "tt should be of size 1 (is " << tt.size() << ")!");
    if (tt.values()[0] != int(vv.size()))
      DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                       "vv should be of size " << tt.values()[0] << " (is " << vv.size() << ")!");
  }

  Parameter(const std::vector< KeyType >& kk,
            const std::vector< ValueType >& vv) throw (Exception::key_is_not_valid,
                                                       Exception::sizes_do_not_match)
    : BaseType(kk, vv)
  {
    if (kk.size() != vv.size())
      DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                       "the size of kk (" << kk.size() << ") has to equal the size of vv (" << vv.size() << ")!");
    if (kk.size() == 0) DUNE_PYMOR_THROW(Exception::sizes_do_not_match, "kk and vv are empty!");
    std::vector< int > valueSizes(vv.size());
    for (size_t ii = 0; ii < vv.size(); ++ii) {
      if (vv[ii].size() == 0)
        DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                         "vv[" << ii << "] is empty!");
      valueSizes[ii] = vv[ii].size();
    }
    type_ = ParameterType(kk, valueSizes);
  }

  Parameter(const ParameterType& tt,
            const std::vector< ValueType >& vv) throw (Exception::key_is_not_valid,
                                                       Exception::sizes_do_not_match)
    : BaseType(tt.keys(), vv)
    , type_(tt)
  {
    if (tt.size() != vv.size())
      DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                       "the size of t (" << tt.size() << ") has to equal the size of vv (" << vv.size() << ")!");
    if (tt.size() == 0) DUNE_PYMOR_THROW(Exception::sizes_do_not_match, "tt and vv are empty!");
    const auto& valueSizes = type_.values();
    for (size_t ii = 0; ii < vv.size(); ++ii) {
      if (int(vv[ii].size()) != valueSizes[ii])
        DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                         "vv[" << ii << "] has to be of size " << valueSizes[ii] << " (is " << vv[ii].size() << ")!");
    }
  }

  const ParameterType& type() const
  {
    return type_;
  }

  void set(const KeyType& key, const ValueType& value) throw (Exception::key_is_not_valid,
                                                              Exception::sizes_do_not_match)
  {
    if (key.empty()) DUNE_PYMOR_THROW(Exception::key_is_not_valid, "key is empty!");
    if (value.size() == 0) DUNE_PYMOR_THROW(Exception::index_out_of_range, "value is empty!");
    BaseType::dict_[key] = value;
    BaseType::update();
    type_.set(key, value.size());
  }

  std::string report() const
  {
    std::ostringstream ret;
    ret << "Dune::Pymor::Parameter(";
    if (dict_.size() == 1) {
      const auto element = dict_.begin();
      ret << "\"" << element->first << "\", ";
      const auto& second = element->second;
      if (second.size() == 1) {
        ret << second[0];
      } else {
        ret << "{";
        for (size_t ii = 0; ii < (second.size() - 1); ++ii)
          ret << second[ii] << ", ";
        ret << second[second.size() - 1] << "}";
      }
    } else if (dict_.size() > 1) {
      const auto& kk = keys();
      const auto& vv = values();
      ret << "{\"";
      for (size_t ii = 0; ii < (kk.size() - 1); ++ii)
        ret << kk[ii] << "\", \"";
      ret << kk[kk.size() - 1] << "\"}, {";
      for (size_t ii = 0; ii < (vv.size() - 1); ++ii) {
        if (vv[ii].size() == 1) {
          ret << vv[ii][0];
        } else {
          ret << "{";
          for (size_t jj = 0; jj < (vv[ii].size() - 1); ++jj)
            ret << vv[ii][jj] << ", ";
          ret << vv[ii][vv[ii].size() - 1] << "}";
        }
      }
      if (vv[vv.size() - 1].size() == 1) {
        ret << vv[vv.size() - 1][0];
      } else {
        ret << "{";
        for (size_t jj = 0; jj < (vv[vv.size() - 1].size() - 1); ++jj)
          ret << vv[vv.size() - 1][jj] << ", ";
        ret << vv[vv.size() - 1][vv[vv.size() - 1].size() - 1] << "}";
      }
    }
    ret << ")";
    return ret.str();
  }

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


/**
 * \brief Base class for everything that can possibly parametric.
 *
 * \attention A derived class Foo has to provide a method
 *            \code
Foo* freeze_parameter(const Parameter mu = Parameter()) const
\endcode
 *            which returns a nonparametric version of the object, if possible.
 */
class Parametric
{
public:
  Parametric()
    : type_()
  {}

//  Parametric(const ParameterType& type)
//    : type_(type)
//  {}

  template< class... Args >
  Parametric(Args&& ...args)
    : type_(std::forward< Args >(args)...)
  {}

  virtual ~Parametric() {}

  const ParameterType& parameter_type() const
  {
    return type_;
  }

  bool parametric() const
  {
    return parameter_type().size() != 0;
  }

private:
  ParameterType type_;
}; // class Parametric


} // namespace Pymor
} // namespace Dune

#endif // DUNE_PYMOR_PARAMETERS_BASE_HH
