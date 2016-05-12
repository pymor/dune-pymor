// This file is part of the dune-Pymor project:
//   https://github.com/wwu-numerik/dune-Pymor/
// Copyright holders: Rene Milk, Felix Schindler
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_FUNCTIONS_SPE10MODEL2_HH
#define DUNE_PYMOR_FUNCTIONS_SPE10MODEL2_HH

#include <dune/stuff/common/fvector.hh>
#include <dune/stuff/playground/functions/indicator.hh>
#include <dune/stuff/functions/spe10model2.hh>

#include "default.hh"


namespace Dune {
namespace Pymor {
namespace Functions {
namespace Spe10 {


// default, to allow for specialization
template< class E, class D, size_t d, class R, size_t r, size_t rC = 1 >
class Model2
  : public AffinelyDecomposableFunctionInterface< E, D, d, R, r, rC >
{
  Model2() { static_assert(AlwaysFalse< E >::value, "Not available for these dimensions!"); }
};


template< class E, class D, class R >
class Model2< E, D, 3, R, 3, 3 >
  : public AffinelyDecomposableDefault< E, D, 3, R, 3, 3 >
{
  static const size_t d = 3;
  static const size_t r = 3;
  static const size_t rC = 3;
  typedef AffinelyDecomposableDefault< E, D, d, R, r, rC > BaseType;
  typedef Model2< E, D, d, R, r, rC >                      ThisType;
  typedef Stuff::Functions::Spe10::Model2< E, D, d, R, r, rC > Spe10FunctionType;
public:
  using typename BaseType::DomainType;

  static const bool available = true;

  static std::string static_id()
  {
    return BaseType::BaseType::static_id() + ".spe10.model2";
  }

  static Stuff::Common::Configuration default_config(const std::string sub_name = "")
  {
    auto config = Spe10FunctionType::default_config();
    config["type"] = static_id();
    config["blockade_width"] = "0";
    config["blockade_value"] = config["min"];
    if (sub_name.empty())
      return config;
    else {
      Stuff::Common::Configuration tmp;
      tmp.add(config, sub_name);
      return tmp;
    }
  } // ... default_config(...)

  static std::unique_ptr< ThisType > create(const Stuff::Common::Configuration config = BaseType::default_config(),
                                            const std::string sub_name = BaseType::static_id())
  {
    // get correct config
    const Stuff::Common::Configuration cfg = config.has_sub(sub_name) ? config.sub(sub_name) : config;
    const Stuff::Common::Configuration default_cfg = default_config();
    // create
    return Stuff::Common::make_unique< ThisType >(
          cfg.get("filename",       default_cfg.get< std::string >("filename")),
          cfg.get("name",           default_cfg.get< std::string >("name")),
          cfg.get("lower_left",     default_cfg.get< DomainType >("lower_left")),
          cfg.get("upper_right",    default_cfg.get< DomainType >("upper_right")),
          cfg.get("blockade_width", default_cfg.get< D >("blockade_width")),
          cfg.get("blockade_value", default_cfg.get< R >("blockade_value")),
          cfg.get("anisotropic",    default_cfg.get< bool >("anisotropic")),
          cfg.get("min",            default_cfg.get< R >("min")),
          cfg.get("max",            default_cfg.get< R >("max")));
  } // ... create(...)

  Model2(const std::string& filename = default_config().get< std::string >("filename"),
         const std::string nm = default_config().get< std::string >("name"),
         const Stuff::Common::FieldVector< D, d >& lower_left
            = default_config().get< DomainType >("lower_left"),
         const Stuff::Common::FieldVector< D, d >& upper_right
            = default_config().get< DomainType >("upper_right"),
         const D& blockade_width = default_config().get< D >("blockade_width"),
         const R& blockade_value = default_config().get< R >("blockade_value"),
         const bool anisotropic = default_config().get< bool >("anisotropic"),
         const R& min = default_config().get< R >("min"),
         const R& max = default_config().get< R >("max"))
    : BaseType(nm)
  {
    typedef Stuff::Functions::Constant<E, D, 3, R, 1>     ScalarConstantFunctionType;
    typedef Stuff::Functions::DomainIndicator<E, D, 3, R, 1>    ScalarIndicatorFunctionType;
    typedef Stuff::Functions::DomainIndicator<E, D, 3, R, 3, 3> TensorIndicatorFunctionType;

    auto spe10 = std::make_shared<Spe10FunctionType>(filename, "spe10", lower_left, upper_right, anisotropic, min, max);

    if (blockade_width > 0) {
      auto one = std::make_shared<ScalarConstantFunctionType>(1, "one");
      auto blockade = std::shared_ptr<ScalarIndicatorFunctionType>(new ScalarIndicatorFunctionType(
          {{{{lower_left[0],
                lower_left[1] + (upper_right[1] - lower_left[1])/2. - blockade_width/2.,
                  lower_left[2]},
             {upper_right[0],
                lower_left[1] + (upper_right[1] - lower_left[1])/2. + blockade_width/2.,
                  upper_right[2]}},
            1.}}));
      typename TensorIndicatorFunctionType::RangeType blockade_scaled_value(0.);
      blockade_scaled_value[0][0] = blockade_scaled_value[1][1] = blockade_scaled_value[2][2] = blockade_value;
      auto blockade_scaled = std::shared_ptr<TensorIndicatorFunctionType>(new TensorIndicatorFunctionType(
          {{{{lower_left[0],
                lower_left[1] + (upper_right[1] - lower_left[1])/2. - blockade_width/2.,
                  lower_left[2]},
             {upper_right[0],
                lower_left[1] + (upper_right[1] - lower_left[1])/2. + blockade_width/2.,
                  upper_right[2]}},
            blockade_scaled_value}}));
      auto blockade_remover = Stuff::Functions::make_difference(one, blockade, "blockade_remover");
      auto spe10_wo_blockade = Stuff::Functions::make_product(blockade_remover, spe10, "spe10_wo_blockade");
      auto spe10_w_scaled_blockade = Stuff::Functions::make_sum(spe10_wo_blockade,
                                                                 blockade_scaled,
                                                                 "spe10_w_scaled_blockade");
      this->register_affine_part(spe10_w_scaled_blockade);
      this->register_component(Stuff::Functions::make_difference(spe10, spe10_w_scaled_blockade, "component"),
                               new Pymor::ParameterFunctional("blockade", 1, "blockade[0]"));
    } else {
      this->register_affine_part(spe10);
    }
  } // Model2(...)

  virtual std::string type() const override final
  {
    return BaseType::BaseType::static_id() + ".spe10.model2";
  }
}; // class Model2


} // namespace Spe10
} // namespace Functions
} // namespace Pymor
} // namespace Dune

#endif // DUNE_PYMOR_FUNCTIONS_SPE10MODEL2_HH
