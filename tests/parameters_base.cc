// This file is part of the dune-pymor project:
//   https://github.com/pymor/dune-pymor
// Copyright Holders: Stephan Rave, Felix Schindler
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#include <dune/stuff/test/test_common.hh>
#include <dune/stuff/common/float_cmp.hh>

#include <dune/pymor/common/exceptions.hh>
#include <dune/pymor/parameters/base.hh>

using namespace Dune;
using namespace Dune::Pymor;

TEST(ParameterType, Parameters_Base)
{
  typedef ParameterType::KeyType KeyType;
  typedef ParameterType::ValueType ValueType;
  ParameterType param1("diffusion", 1);
  ParameterType param2({"diffusion", "force"},
                       {1, 2});
  ParameterType param3 = {"diffusion", 1};
  ParameterType param4 = {{"diffusion", "force"}, {1, 2}};
  if (param1 != param3) throw PymorException();
  if (param2 != param4) throw PymorException();
  std::string ss = param1.report();
//  DSC_LOG_DEBUG << ss << std::endl;
  std::vector< KeyType > keys = param1.keys();
  if (keys.size() != 1) throw PymorException();
  if (keys[0] != "diffusion") throw PymorException();
  if (!param1.hasKey("diffusion")) throw PymorException();
  std::vector< ValueType > values = param1.values();
  if (values.size() != 1) throw PymorException();
  if (values[0] != 1) throw PymorException();
  if (param1.get("diffusion") != 1) throw PymorException();
  if (param1 == param2) throw PymorException();
  if (!(param1 != param2)) throw PymorException();
  param1.set("force", 2);
  keys = param1.keys();
  if (keys.size() != 2) throw PymorException();
  values = param1.values();
  if (values.size() != 2) throw PymorException();
  if (!param1.hasKey("diffusion")) throw PymorException();
  if (!param1.hasKey("force")) throw PymorException();
  if (param1.get("diffusion") != 1) throw PymorException();
  if (param1.get("force") != 2) throw PymorException();
  if (param1 != param2) throw PymorException();
  if (!(param1 == param2)) throw PymorException();
//  DSC_LOG_DEBUG << param2 << std::endl;
}

TEST(Parameter, Parameters_Base)
{
  ParameterType type1 = {"diffusion", 1};
  ParameterType type2 = {{"diffusion", "force"}, {1, 2}};
  typedef Parameter::KeyType KeyType;
  typedef Parameter::ValueType ValueType;
  Parameter param1("diffusion", 1.0);
  Parameter param2({"diffusion", "force"},
                   {{1.0}, {1.0, 2.0}});
  Parameter param3 = {"diffusion", 1.0};
  Parameter param4 = {{"diffusion", "force"},
                      {{1.0}, {1.0, 2.0}}};
  Parameter DUNE_UNUSED(param5)(type1, 1.0);
  Parameter DUNE_UNUSED(param6)(type1, {1.0});
  Parameter DUNE_UNUSED(param8)(type2, {{1.0}, {1.0, 2.0}});
  if (param1.type() != type1) throw PymorException();
  if (param1 != param3) throw PymorException();
  if (param2 != param4) throw PymorException();
  std::string ss = param1.report();
//  DSC_LOG_DEBUG << ss << std::endl;
  std::vector< KeyType > keys = param1.keys();
  if (keys.size() != 1) throw PymorException();
  if (keys[0] != "diffusion") throw PymorException();
  if (!param1.hasKey("diffusion")) throw PymorException();
  std::vector< ValueType > values = param1.values();
  if (values.size() != 1) throw PymorException();
  if (values[0].size() != 1) throw PymorException();
  if (Dune::Stuff::Common::FloatCmp::ne(values[0][0], 1.0)) throw PymorException();
  if (param1.get("diffusion") != ValueType({1.0})) throw PymorException();
  if (param1 == param2) throw PymorException();
  if (!(param1 != param2)) throw PymorException();
  param1.set("force", {1.0, 2.0});
  keys = param1.keys();
  if (keys.size() != 2) throw PymorException();
  values = param1.values();
  if (values.size() != 2) throw PymorException();
  if (!param1.hasKey("diffusion")) throw PymorException();
  if (!param1.hasKey("force")) throw PymorException();
  if (param1.get("diffusion") != ValueType({1.0})) throw PymorException();
  if (param1.get("force") != ValueType({1.0, 2.0})) throw PymorException();
  if (param1 != param2) throw PymorException();
  if (!(param1 == param2)) throw PymorException();
  //  DSC_LOG_DEBUG << param2 << std::endl;
}

TEST(Parametric, Parameters_Base)
{
  class Foo
    : public Parametric
  {
  public:
    Foo()
      : Parametric()
    {
      Parameter muDiffusion = {"diffusion", {1.0, 2.0}};
      Parameter muForce = {{"first_force", "second_force"},
                          {{1.0},          {1.0, 2.0, 3.0}}};
      Parameter mu = {{"diffusion", "first_force", "second_force"},
                     {{1.0, 2.0},   {1.0},         {1.0, 2.0, 3.0}}};
      inherit_parameter_type(muDiffusion, "a");
      inherit_parameter_type(muForce, "b");
      const ParameterType& typeDiffusion = map_parameter_type("a");
      if (typeDiffusion != muDiffusion.type()) DUNE_PYMOR_THROW(PymorException, "");
      const ParameterType& typeForce = map_parameter_type("b");
      if (typeForce != muForce.type()) DUNE_PYMOR_THROW(PymorException, "");
      const Parameter mappedMuDiffusion = map_parameter(mu, "a");
      if (mappedMuDiffusion != muDiffusion) DUNE_PYMOR_THROW(PymorException, "");
      const Parameter mappedMuForce = map_parameter(mu, "b");
      if (mappedMuForce != muForce) DUNE_PYMOR_THROW(PymorException, "");
    }
  };

  Foo DUNE_UNUSED(foo);
}

int main(int argc, char** argv)
{
  test_init(argc, argv);
  return RUN_ALL_TESTS();
}
