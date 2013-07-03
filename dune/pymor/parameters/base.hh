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

#include <dune/pymor/common/exceptions.hh>

namespace Dune {
namespace Pymor {


class ParameterType
{
public:
  typedef std::string KeyType;
  typedef int         ValueType;

  ParameterType(const KeyType& kk, const ValueType& vv) throw (Exception::key_is_not_valid, Exception::index_out_of_range)
    : dict_({std::make_pair(std::string(kk), vv)})
  {
    if (kk.empty()) DUNE_PYMOR_THROW(Exception::key_is_not_valid, "kk is empty!");
    if (vv <= 0) DUNE_PYMOR_THROW(Exception::index_out_of_range, "vv has to be positive (is " << vv << ")!");
  }

  ParameterType(const std::initializer_list< KeyType >& kk,
                const std::initializer_list< ValueType >& vv) throw (Exception::key_is_not_valid,
                                                                     Exception::index_out_of_range,
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
      const ValueType& value = *vv_it;
      if (key.empty()) DUNE_PYMOR_THROW(Exception::key_is_not_valid, "kk[" << counter << "] is empty!");
      if (value <= 0)
        DUNE_PYMOR_THROW(Exception::index_out_of_range,
                         "vv[" << counter << "] has to be positive (is " << value << ")!");
      dict_.insert(std::make_pair(key, value));
    }
  }

  std::vector< KeyType > keys() const
  {
    std::vector< KeyType > ret;
    for (auto element : dict_)
      ret.emplace_back(element.first);
    return ret;
  }

  std::vector< ValueType > values() const
  {
    std::vector< ValueType > ret;
    for (auto element : dict_)
      ret.emplace_back(element.second);
    return ret;
  }

  bool hasKey(const KeyType& key) const
  {
    const auto result = dict_.find(key);
    return result != dict_.end();
  }

  void set(const KeyType& key, const ValueType& value) throw (Exception::key_is_not_valid,
                                                              Exception::index_out_of_range)
  {
    if (key.empty()) DUNE_PYMOR_THROW(Exception::key_is_not_valid, "key is empty!");
    if (value <= 0) DUNE_PYMOR_THROW(Exception::index_out_of_range, "value has to be positive (is " << value << ")!");
    dict_[key] = value;
  }

  ValueType get(const KeyType& key) const throw (Exception::key_is_not_valid)
  {
    const auto result = dict_.find(key);
    if (result == dict_.end()) DUNE_PYMOR_THROW(Exception::key_is_not_valid, "key does not exist!");
    return result->second;
  }

  bool operator==(const ParameterType& other)
  {
    return dict_ == other.dict_;
  }

  bool operator!=(const ParameterType& other)
  {
    return dict_ != other.dict_;
  }

  unsigned int size() const
  {
    return dict_.size();
  }

  std::string report() const
  {
    std::ostringstream ret;
    ret << "Dune::Pymor::ParameterType(";
    if (dict_.size() == 1) {
      const auto element = dict_.begin();
      ret << element->first << ", " << element->second;
    } else if (dict_.size() > 1) {
      std::vector< KeyType > kk;
      std::vector< ValueType > vv;
      for (auto element : dict_) {
        kk.emplace_back(element.first);
        vv.emplace_back(element.second);
      }
      ret << "{";
      for (size_t ii = 0; ii < (kk.size() - 1); ++ii)
        ret << kk[ii] << ", ";
      ret << kk[kk.size() - 1] << "},\n                            {";
      for (size_t ii = 0; ii < (vv.size() - 1); ++ii)
        ret << vv[ii] << ", ";
      ret << vv[vv.size() - 1] << "}";
    }
    ret << ")";
    return ret.str();
  }

private:
  std::map< KeyType, ValueType > dict_;

public:
  auto begin() -> decltype (dict_.begin())
  {
    return dict_.begin();
  }

  auto end() -> decltype (dict_.end())
  {
    return dict_.end();
  }
}; // class ParameterType


} // namespace Pymor
} // namespace Dune

#endif // DUNE_PYMOR_PARAMETERS_BASE_HH
