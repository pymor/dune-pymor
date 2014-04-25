// This file is part of the dune-pymor project:
//   https://github.com/pymor/dune-pymor
// Copyright holders: Stephan Rave, Felix Schindler
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#include <dune/stuff/test/test_common.hh>

#include <utility>

#include <dune/common/typetraits.hh>

#include <dune/stuff/common/float_cmp.hh>
#include <dune/stuff/la/container/common.hh>
#include <dune/stuff/la/container/eigen.hh>

#include <dune/pymor/common/exceptions.hh>
#include <dune/pymor/parameters/base.hh>
#include <dune/pymor/parameters/functional.hh>
#include <dune/pymor/functionals/interfaces.hh>
#include <dune/pymor/functionals/default.hh>
#include <dune/pymor/functionals/affine.hh>


using namespace Dune;
using namespace Dune::Pymor;

typedef testing::Types<
                        Stuff::LA::CommonDenseVector< double >
#if HAVE_EIGEN
                      , Stuff::LA::EigenDenseVector< double >
#endif
                      > VectorTypes;


static const size_t dim = 4;


template< class VectorType >
struct VectorBasedTest
  : public ::testing::Test
{
  void check() const
  {
    // static tests
    typedef Functionals::VectorBased< VectorType >  FunctionalType;
    typedef typename FunctionalType::Traits         Traits;
    // * of the traits
    typedef typename Traits::derived_type T_derived_type;
    static_assert(std::is_same< FunctionalType, T_derived_type >::value,
                  "FunctionalType::Traits::derived_type is wrong!");
    typedef typename Traits::SourceType   T_SourceType;
    typedef typename Traits::ScalarType   T_ScalarType;
    typedef typename Traits::FrozenType   T_FrozenType;
    // * of the class itself (aka the derived type)
    typedef typename FunctionalType::SourceType     D_SourceType;
    typedef typename FunctionalType::ScalarType     D_ScalarType;
    typedef typename FunctionalType::ContainerType  D_ContainerType;
    typedef typename FunctionalType::FrozenType     D_FrozenType;
    static_assert(std::is_same< D_ContainerType, VectorType >::value,
                  "FunctionalType::ContainerType is wrong!");
    // * of the class as the interface
    typedef FunctionalInterface< Traits >       InterfaceType;
    typedef typename InterfaceType::SourceType  I_SourceType;
    typedef typename InterfaceType::ScalarType  I_ScalarType;
    typedef typename InterfaceType::FrozenType  I_FrozenType;
    // dynamic tests
    // * of the class itself (aka the derived type)
    const FunctionalType d_from_ptr(new VectorType(dim, D_ScalarType(1)));
    FunctionalType d_from_shared_ptr(std::make_shared< VectorType >(dim));
    d_from_shared_ptr = d_from_ptr;
    FunctionalType DUNE_UNUSED(d_copy_constructor)(d_from_ptr); // <- at this point, all functionals share the same vector!
    const bool d_linear = d_from_ptr.linear();
    if (!d_linear) DUNE_PYMOR_THROW(PymorException, "");
    const DUNE_STUFF_SSIZE_T d_dim_source = d_from_ptr.dim_source();
    if (d_dim_source != dim) DUNE_PYMOR_THROW(PymorException, d_dim_source);
    const VectorType source(dim, D_ScalarType(1));
    const D_ScalarType d_apply = d_from_ptr.apply(source);
    if (Stuff::Common::FloatCmp::ne(d_apply, D_ScalarType(dim))) DUNE_PYMOR_THROW(PymorException, d_apply);
    // * of the class as the interface
    const InterfaceType& i_from_ptr = static_cast< const InterfaceType& >(d_from_ptr);
    const bool i_linear = i_from_ptr.linear();
    if (!i_linear) DUNE_PYMOR_THROW(PymorException, "");
    const DUNE_STUFF_SSIZE_T i_dim_source = i_from_ptr.dim_source();
    if (i_dim_source != dim) DUNE_PYMOR_THROW(PymorException, i_dim_source);
    const I_ScalarType i_apply = i_from_ptr.apply(source);
    if (Stuff::Common::FloatCmp::ne(i_apply, I_ScalarType(dim))) DUNE_PYMOR_THROW(PymorException, i_apply);
  }
}; // struct VectorBasedTest


TYPED_TEST_CASE(VectorBasedTest, VectorTypes);
TYPED_TEST(VectorBasedTest, FUNCTIONALS) {
  this->check();
}


template< class VectorType >
struct LinearAffinelyDecomposedVectorBasedTest
  : public ::testing::Test
{
  void check() const
  {
    // static tests
    typedef Functionals::LinearAffinelyDecomposedVectorBased< VectorType >  FunctionalType;
    typedef typename FunctionalType::Traits Traits;
    // * of the traits
    typedef typename Traits::derived_type T_derived_type;
    static_assert(std::is_same< FunctionalType, T_derived_type >::value,
                  "FunctionalType::Traits::derived_type is wrong!");
    typedef typename Traits::ComponentType ComponentType;
    // * of the class itself (aka the derived type)
    typedef typename FunctionalType::ComponentType  D_ComponentType;
    typedef typename FunctionalType::FrozenType     D_FrozenType;
    typedef typename FunctionalType::ScalarType     D_ScalarType;
    typedef typename FunctionalType::AffinelyDecomposedVectorType D_AffinelyDecomposedVectorType;
    // * of the class as the interface
    typedef AffinelyDecomposedFunctionalInterface< Traits > InterfaceType;
    typedef typename InterfaceType::derived_type I_derived_type;
    static_assert(std::is_same< FunctionalType, I_derived_type >::value,
                  "InterfaceType::derived_type is wrong!");
    typedef typename InterfaceType::ComponentType I_ComponentType;
    typedef typename InterfaceType::FrozenType    I_FrozenType;
    typedef typename InterfaceType::ScalarType    I_ScalarType;
    // dynamic tests
    // * of the class itself (aka the derived type)
    D_AffinelyDecomposedVectorType affinelyDecomposedVector(new VectorType(dim, 1));
    affinelyDecomposedVector.register_component(new VectorType(dim, 1),
                                                new ParameterFunctional("diffusion", 1, "diffusion[0]"));
    affinelyDecomposedVector.register_component(new VectorType(dim, 1),
                                                new ParameterFunctional("force", 2, "force[0]"));
    const Parameter mu = {{"diffusion", "force"},
                          {{1.0}, {1.0, 1.0}}};
    if (affinelyDecomposedVector.parameter_type() != mu.type())
      DUNE_PYMOR_THROW(PymorException,
                       "\nmu.type()                                 = " << mu.type()
                       << "\naffinelyDecomposedVector.parameter_type() = "
                       << affinelyDecomposedVector.parameter_type());
    FunctionalType d_functional(affinelyDecomposedVector);
    if (!d_functional.parametric()) DUNE_PYMOR_THROW(PymorException, "");
    if (d_functional.parameter_type() != mu.type())
      DUNE_PYMOR_THROW(PymorException,
                       "\nmu.type()         = " << mu.type()
                       << "\n.parameter_type() = " << d_functional.parameter_type());
    const DUNE_STUFF_SSIZE_T d_num_components = d_functional.num_components();
    if (d_num_components != 2) DUNE_PYMOR_THROW(PymorException, d_num_components);
    for (DUNE_STUFF_SSIZE_T qq = 0; qq < d_num_components; ++qq) {
      D_ComponentType component = d_functional.component(qq);
      if (component.parametric()) DUNE_PYMOR_THROW(PymorException, "");
      ParameterFunctional DUNE_UNUSED(coefficient) = d_functional.coefficient(qq);
    }
    if (!d_functional.has_affine_part()) DUNE_PYMOR_THROW(PymorException, "");
    D_ComponentType d_affine_part = d_functional.affine_part();
    if (d_affine_part.parametric()) DUNE_PYMOR_THROW(PymorException, "");
    D_FrozenType d_frozen = d_functional.freeze_parameter(mu);
    if (d_frozen.parametric()) DUNE_PYMOR_THROW(PymorException, "");
    VectorType source(dim, D_ScalarType(1));
    D_ScalarType d_apply = d_functional.apply(source, mu);
    D_ScalarType d_frozen_apply = d_frozen.apply(source);
    if (Stuff::Common::FloatCmp::ne(d_apply, d_frozen_apply))
      DUNE_PYMOR_THROW(PymorException, "\nd_apply        = " << d_apply << "\nd_frozen_apply = " << d_frozen_apply);
    // * of the class as the interface
    InterfaceType& i_functional = static_cast< InterfaceType& >(d_functional);
    if (!i_functional.parametric()) DUNE_PYMOR_THROW(PymorException, "");
    if (i_functional.parameter_type() != mu.type())
      DUNE_PYMOR_THROW(PymorException,
                       "\nmu.type()         = " << mu.type()
                       << "\n.parameter_type() = " << i_functional.parameter_type());
    const DUNE_STUFF_SSIZE_T i_num_components = i_functional.num_components();
    if (i_num_components != 2) DUNE_PYMOR_THROW(PymorException, i_num_components);
    for (int qq = 0; qq < i_num_components; ++qq) {
      I_ComponentType component = i_functional.component(qq);
      if (component.parametric()) DUNE_PYMOR_THROW(PymorException, "");
      ParameterFunctional DUNE_UNUSED(coefficient) = i_functional.coefficient(qq);
    }
    if (!i_functional.has_affine_part()) DUNE_PYMOR_THROW(PymorException, "");
    I_ComponentType i_affine_part = d_functional.affine_part();
    if (i_affine_part.parametric()) DUNE_PYMOR_THROW(PymorException, "");
    I_FrozenType i_frozen = i_functional.freeze_parameter(mu);
    if (i_frozen.parametric()) DUNE_PYMOR_THROW(PymorException, "");
    I_ScalarType i_apply = i_functional.apply(source, mu);
    I_ScalarType i_frozen_apply = i_frozen.apply(source);
    if (Stuff::Common::FloatCmp::ne(i_apply, i_frozen_apply))
      DUNE_PYMOR_THROW(PymorException, "\ni_apply        = " << i_apply << "\ni_frozen_apply = " << i_frozen_apply);
  }
}; // struct LinearAffinelyDecomposedVectorBasedTest


TYPED_TEST_CASE(LinearAffinelyDecomposedVectorBasedTest, VectorTypes);
TYPED_TEST(LinearAffinelyDecomposedVectorBasedTest, FUNCTIONALS) {
  this->check();
}


int main(int argc, char** argv)
{
  try {
    test_init(argc, argv);
    return RUN_ALL_TESTS();
  } catch (Dune::PymorException &e) {
    std::cerr << Dune::Stuff::Common::colorStringRed("dune-pymor reported: ") << e << std::endl;
  } catch (Dune::Exception &e) {
    std::cerr << Dune::Stuff::Common::colorStringRed("Dune reported error: ") << e << std::endl;
  } catch (...) {
    std::cerr << Dune::Stuff::Common::colorStringRed("Unknown exception thrown!") << std::endl;
  }
}
