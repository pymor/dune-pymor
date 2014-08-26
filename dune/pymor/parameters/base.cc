// This file is part of the dune-pymor project:
//   https://github.com/pymor/dune-pymor
// Copyright holders: Stephan Rave, Felix Schindler
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#include "config.h"

#include <type_traits>

#include <dune/stuff/common/exceptions.hh>

#include "base.hh"

namespace Dune {
namespace Pymor {


// ========================
// ===== KeyValueBase =====
// ========================
template< class KeyType, class ValueType >
KeyValueBase< KeyType, ValueType >::KeyValueBase()
{}

template< class KeyType, class ValueType >
KeyValueBase< KeyType, ValueType >::~KeyValueBase()
{}

template< class KeyType, class ValueType >
KeyValueBase< KeyType, ValueType >::KeyValueBase(const KeyType& kk, const ValueType& vv)
  : dict_({std::make_pair(kk, vv)})
{
  update();
}

template< class KeyType, class ValueType >
KeyValueBase< KeyType, ValueType >::KeyValueBase(const std::vector< KeyType >& kk, const std::vector< ValueType >& vv)
{
  if (kk.size() != vv.size())
    DUNE_THROW(Stuff::Exceptions::shapes_do_not_match,
               "the size of kk (" << kk.size() << ") has to equal the size of vv (" << vv.size() << ")!");
  if (kk.size() == 0) DUNE_THROW(Stuff::Exceptions::shapes_do_not_match, "kk and vv are empty!");
  const auto kk_end = kk.end();
  const auto vv_end = vv.end();
  auto kk_it = kk.begin();
  size_t counter = 0;
  for (auto vv_it = vv.begin(); (kk_it != kk_end) && (vv_it != vv_end); ++kk_it, ++vv_it, ++counter) {
    const KeyType& key = *kk_it;
    if (key.empty()) DUNE_THROW(Stuff::Exceptions::wrong_input_given, "kk[" << counter << "] is empty!");
    const ValueType& value = *vv_it;
    dict_.insert(std::make_pair(key, value));
  }
  update();
}

template< class KeyType, class ValueType >
bool KeyValueBase< KeyType, ValueType >::empty() const
{
  return dict_.size() == 0;
}

template< class KeyType, class ValueType >
const std::vector< KeyType >& KeyValueBase< KeyType, ValueType >::keys() const
{
  return keys_;
}

template< class KeyType, class ValueType >
const std::vector< ValueType >& KeyValueBase< KeyType, ValueType >::values() const
{
  return values_;
}

template< class KeyType, class ValueType >
bool KeyValueBase< KeyType, ValueType >::hasKey(const KeyType& key) const
{
  const auto result = dict_.find(key);
  return result != dict_.end();
}

template< class KeyType, class ValueType >
const ValueType& KeyValueBase< KeyType, ValueType >::get(const KeyType& key) const
{
  const auto result = dict_.find(key);
  if (result == dict_.end()) DUNE_THROW(Stuff::Exceptions::wrong_input_given, "key does not exist!");
  return result->second;
}

template< class KeyType, class ValueType >
bool KeyValueBase< KeyType, ValueType >::operator<(const KeyValueBase< KeyType, ValueType >& other) const
{
  return dict_ < other.dict_;
}

template< class KeyType, class ValueType >
bool KeyValueBase< KeyType, ValueType >::operator==(const KeyValueBase< KeyType, ValueType >& other) const
{
  return dict_ == other.dict_;
}

template< class KeyType, class ValueType >
bool KeyValueBase< KeyType, ValueType >::operator!=(const KeyValueBase< KeyType, ValueType >& other) const
{
  return dict_ != other.dict_;
}

template< class KeyType, class ValueType >
DUNE_STUFF_SSIZE_T KeyValueBase< KeyType, ValueType >::size() const
{
  return (DUNE_STUFF_SSIZE_T)(dict_.size());
}

template< class KeyType, class ValueType >
void KeyValueBase< KeyType, ValueType >::update()
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

template class KeyValueBase< std::string, DUNE_STUFF_SSIZE_T >;
template class KeyValueBase< std::string, std::vector< double > >;


// =========================
// ===== ParameterType =====
// =========================
ParameterType::ParameterType()
{}

ParameterType::ParameterType(const KeyType& kk, const ValueType& vv)
  : BaseType(kk, vv)
{
  if (kk.empty()) DUNE_THROW(Stuff::Exceptions::wrong_input_given, "kk is empty!");
  if (vv <= 0) DUNE_THROW(Stuff::Exceptions::index_out_of_range, "vv has to be positive (is " << vv << ")!");
}

ParameterType::ParameterType(const std::vector< KeyType >& kk, const std::vector< ValueType >& vv)
  : BaseType(kk, vv)
{
  if (kk.size() != vv.size())
    DUNE_THROW(Stuff::Exceptions::shapes_do_not_match,
               "the size of kk (" << kk.size() << ") has to equal the size of vv (" << vv.size() << ")!");
  if (kk.size() == 0) DUNE_THROW(Stuff::Exceptions::shapes_do_not_match, "kk and vv are empty!");
  const auto kk_end = kk.end();
  const auto vv_end = vv.end();
  auto kk_it = kk.begin();
  size_t counter = 0;
  for (auto vv_it = vv.begin(); (kk_it != kk_end) && (vv_it != vv_end); ++kk_it, ++vv_it, ++counter) {
    const KeyType& key = *kk_it;
    const ValueType& value = *vv_it;
    if (key.empty()) DUNE_THROW(Stuff::Exceptions::wrong_input_given, "kk[" << counter << "] is empty!");
    if (value <= 0)
      DUNE_THROW(Stuff::Exceptions::index_out_of_range,
                 "vv[" << counter << "] has to be positive (is " << value << ")!");
  }
}

void ParameterType::set(const KeyType& key, const ValueType& value)
{
  if (key.empty()) DUNE_THROW(Stuff::Exceptions::wrong_input_given, "key is empty!");
  if (value <= 0) DUNE_THROW(Stuff::Exceptions::index_out_of_range, "value has to be positive (is " << value << ")!");
  if (!hasKey(key)) {
    BaseType::dict_[key] = value;
    BaseType::update();
  }
}

std::string ParameterType::report() const
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

std::string ParameterType::report_for_filename() const
{
  std::ostringstream ret;
  if (dict_.size() == 1) {
    const auto element = dict_.begin();
    ret << element->first << "_" << element->second;
  } else if (dict_.size() > 1) {
    const auto& kk = keys();
    const auto& vv = values();
    for (size_t ii = 0; ii < (kk.size() - 1); ++ii)
      ret << kk[ii] << "_";
    ret << kk[kk.size() - 1] << "_";
    for (size_t ii = 0; ii < (vv.size() - 1); ++ii)
      ret << vv[ii] << "_";
    ret << vv[vv.size() - 1];
  }
  return ret.str();
}


std::ostream& operator<<(std::ostream& oo, const ParameterType& pp)
{
  oo << pp.report();
  return oo;
}


// =====================
// ===== Parameter =====
// =====================
Parameter::Parameter()
  : BaseType()
{}

Parameter::Parameter(const KeyType& kk, const double& vv)
  : BaseType(kk, {vv})
  , type_(kk, 1)
{}

Parameter::Parameter(const ParameterType& tt, const double& vv)
  : BaseType(tt.keys(), {{vv}})
  , type_(tt)
{
  if (tt.size() == 0) DUNE_THROW(Stuff::Exceptions::shapes_do_not_match, "tt is empty!");
  if (tt.size() != 1) DUNE_THROW(Stuff::Exceptions::shapes_do_not_match,
                                 "tt should be of size 1 (is " << tt.size() << ")!");
  if (tt.values()[0] != 1)
    DUNE_THROW(Stuff::Exceptions::shapes_do_not_match,
               "tt.get(\"" << tt.keys()[0] << "\") should be 1 (is " << tt.values()[0] << ")!");
}

Parameter::Parameter(const KeyType& kk, const ValueType& vv)
  : BaseType(kk, vv)
  , type_(kk, vv.size())
{
  if (kk.empty()) DUNE_THROW(Stuff::Exceptions::wrong_input_given, "kk is empty!");
  if (vv.size() == 0) DUNE_THROW(Stuff::Exceptions::shapes_do_not_match, "vv is empty!");
}

Parameter::Parameter(const ParameterType& tt, const ValueType& vv)
  : BaseType(tt.keys(), {vv})
  , type_(tt)
{
  if (tt.size() == 0) DUNE_THROW(Stuff::Exceptions::shapes_do_not_match, "tt is empty!");
  if (tt.size() != 1) DUNE_THROW(Stuff::Exceptions::shapes_do_not_match,
                                 "tt should be of size 1 (is " << tt.size() << ")!");
  if (tt.values()[0] != (std::make_signed< size_t >::type)(vv.size()))
    DUNE_THROW(Stuff::Exceptions::shapes_do_not_match,
               "vv should be of size " << tt.values()[0] << " (is " << vv.size() << ")!");
}

Parameter::Parameter(const std::vector< KeyType >& kk,
                     const std::vector< ValueType >& vv)
  : BaseType(kk, vv)
{
  if (kk.size() != vv.size())
    DUNE_THROW(Stuff::Exceptions::shapes_do_not_match,
               "the size of kk (" << kk.size() << ") has to equal the size of vv (" << vv.size() << ")!");
  if (kk.size() == 0) DUNE_THROW(Stuff::Exceptions::shapes_do_not_match, "kk and vv are empty!");
  std::vector< DUNE_STUFF_SSIZE_T > valueSizes(vv.size());
  for (size_t ii = 0; ii < vv.size(); ++ii) {
    if (vv[ii].size() == 0)
      DUNE_THROW(Stuff::Exceptions::shapes_do_not_match, "vv[" << ii << "] is empty!");
    valueSizes[ii] = vv[ii].size();
  }
  type_ = ParameterType(kk, valueSizes);
}

Parameter::Parameter(const ParameterType& tt,
                     const std::vector< ValueType >& vv)
  : BaseType(tt.keys(), vv)
  , type_(tt)
{
  if (tt.size() != (std::make_signed< size_t >::type)(vv.size()))
    DUNE_THROW(Stuff::Exceptions::shapes_do_not_match,
               "the size of t (" << tt.size() << ") has to equal the size of vv (" << vv.size() << ")!");
  if (tt.size() == 0) DUNE_THROW(Stuff::Exceptions::shapes_do_not_match, "tt and vv are empty!");
  const auto& valueSizes = type_.values();
  for (size_t ii = 0; ii < vv.size(); ++ii) {
    if ((std::make_signed< size_t >::type)(vv[ii].size()) != valueSizes[ii])
      DUNE_THROW(Stuff::Exceptions::shapes_do_not_match,
                 "vv[" << ii << "] has to be of size " << valueSizes[ii] << " (is " << vv[ii].size() << ")!");
  }
}

const ParameterType& Parameter::type() const
{
  return type_;
}

void Parameter::set(const KeyType& key, const ValueType& value)
{
  if (key.empty()) DUNE_THROW(Stuff::Exceptions::wrong_input_given, "key is empty!");
  if (value.size() == 0) DUNE_THROW(Stuff::Exceptions::index_out_of_range, "value is empty!");
  BaseType::dict_[key] = value;
  BaseType::update();
  type_.set(key, value.size());
}

std::string Parameter::report() const
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
        ret << vv[ii][0] << ", ";
      } else {
        ret << "{";
        for (size_t jj = 0; jj < (vv[ii].size() - 1); ++jj)
          ret << vv[ii][jj] << ", ";
        ret << vv[ii][vv[ii].size() - 1] << "}, ";
      }
    }
    if (vv[vv.size() - 1].size() == 1) {
      ret << vv[vv.size() - 1][0] << "}";
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

std::string Parameter::report_for_filename() const
{
  std::ostringstream ret;
  if (dict_.size() == 1) {
    const auto element = dict_.begin();
    ret << element->first << "_";
    const auto& second = element->second;
    if (second.size() == 1) {
      ret << second[0];
    } else {
      for (size_t ii = 0; ii < (second.size() - 1); ++ii)
        ret << second[ii] << "_";
      ret << second[second.size() - 1];
    }
  } else if (dict_.size() > 1) {
    const auto& kk = keys();
    const auto& vv = values();
    for (size_t ii = 0; ii < (kk.size() - 1); ++ii)
      ret << kk[ii] << "_";
    ret << kk[kk.size() - 1] << "_";
    for (size_t ii = 0; ii < (vv.size() - 1); ++ii) {
      if (vv[ii].size() == 1) {
        ret << vv[ii][0];
      } else {
        for (size_t jj = 0; jj < (vv[ii].size() - 1); ++jj)
          ret << vv[ii][jj] << "_";
        ret << vv[ii][vv[ii].size() - 1];
      }
    }
    if (vv[vv.size() - 1].size() == 1) {
      ret << vv[vv.size() - 1][0];
    } else {
      for (size_t jj = 0; jj < (vv[vv.size() - 1].size() - 1); ++jj)
        ret << vv[vv.size() - 1][jj] << "_";
      ret << vv[vv.size() - 1][vv[vv.size() - 1].size() - 1];
    }
  }
  return ret.str();
}

Parameter::ValueType Parameter::serialize() const
{
  ValueType ret;
  for (auto key : keys()) {
    const ValueType& vals = get(key);
    for (auto element : vals) {
      ret.push_back(element);
    }
  }
  return ret;
} // ValueType serialize() const

std::ostream& operator<<(std::ostream& oo, const Parameter& pp)
{
  oo << pp.report();
  return oo;
}


// ======================
// ===== Parametric =====
// ======================
Parametric::Parametric()
  : type_()
{}

Parametric::Parametric(const ParameterType& tt)
  : type_(tt)
{}

Parametric::Parametric(const std::string& kk, const DUNE_STUFF_SSIZE_T & vv)
  : type_(kk, vv)
{}

Parametric::Parametric(const std::vector< std::string >& kk, const std::vector< DUNE_STUFF_SSIZE_T >& vv)
  : type_(kk, vv)
{}

Parametric::Parametric(const Parametric& other)
  : type_(other.type_)
  , inherits_map_(other.inherits_map_)
{}

Parametric::~Parametric()
{}

const ParameterType& Parametric::parameter_type() const
{
  return type_;
}

bool Parametric::parametric() const
{
  return !parameter_type().empty();
}

void Parametric::inherit_parameter_type(const ParameterType& tt, const std::string id)
{
  if (id.empty()) DUNE_THROW(Stuff::Exceptions::wrong_input_given, "id must not be empty!");
  const auto result = inherits_map_.find(id);
  if (result != inherits_map_.end() && result->second != tt)
    DUNE_THROW(Stuff::Exceptions::wrong_input_given,
               "inheriting the same id twice with different types does not make any sense (type of tt is "
               << tt << ", while type " << result->second << " is already registered for '" << id << "'!");
  inherits_map_[id] = tt;
  for (auto key : tt.keys()) {
    if (type_.hasKey(key) && (type_.get(key) != tt.get(key)))
      DUNE_THROW(Stuff::Exceptions::shapes_do_not_match,
                 "the size for key '" << key << "' in tt (" << tt.get(key)
                 << ") does not match the size for '" << key << "' in this parameter_type ("
                 << type_.get(key) << ")!");
    else
      type_.set(key, tt.get(key));
  }
}

void Parametric::inherit_parameter_type(const Parameter& mu, const std::string id)
{
  inherit_parameter_type(mu.type(), id);
}

void Parametric::inherit_parameter_type(const Parametric& other, const std::string id)
{
  inherit_parameter_type(other.parameter_type(), id);
}

Parameter Parametric::map_parameter(const Parameter& mu, const std::string id) const
{
  if (inherits_map_.empty())
    DUNE_THROW(Stuff::Exceptions::wrong_input_given, "there is nothing to map!");
  const auto result = inherits_map_.find(id);
  if (result == inherits_map_.end()) {
    return Parameter();
//    std::stringstream msg;
//    msg << "id has to be one of {";
//    size_t count = 0;
//    const auto itEnd = inherits_map_.end();
//    auto it = inherits_map_.begin();
//    if (inherits_map_.size() > 1) {
//      for (; it != itEnd; ++it) {
//        if (count < inherits_map_.size() - 1) {
//          msg << "'" << it->first << "', ";
//          ++count;
//        } else {
//          ++it;
//          break;
//        }
//      }
//    }
//    msg << "'" << it->first << "'} (is '" << id << "')!";
//    DUNE_THROW(Stuff::Exceptions::wrong_input_given, msg.str());
  } else {
    const ParameterType& localType = result->second;
    Parameter muLocal;
    for (auto key : localType.keys())
      muLocal.set(key, mu.get(key));
    return muLocal;
  }
}

const ParameterType& Parametric::map_parameter_type(const std::string id) const
{
  if (inherits_map_.empty())
    DUNE_THROW(Stuff::Exceptions::wrong_input_given, "there is nothing to map!");
  const auto result = inherits_map_.find(id);
  if (result == inherits_map_.end()) {
    std::stringstream msg;
    msg << "id has to be one of {";
    size_t count = 0;
    const auto itEnd = inherits_map_.end();
    auto it = inherits_map_.begin();
    if (inherits_map_.size() > 1) {
      for (; it != itEnd; ++it) {
        if (count < inherits_map_.size() - 1) {
          msg << "'" << it->first << "', ";
          ++count;
        } else {
          ++it;
          break;
        }
      }
    }
    msg << "'" << it->first << "'} (is '" << id << "')!";
    DUNE_THROW(Stuff::Exceptions::wrong_input_given, msg.str());
  }
  return result->second;
}

void Parametric::replace_parameter_type(const ParameterType tt)
{
  inherits_map_.clear();
  type_ = tt;
}


} // namespace Pymor
} // namespace Dune
