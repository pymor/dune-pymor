// This file is part of the dune-pymor project:
//   https://github.com/pymor/dune-pymor
// Copyright holders: Stephan Rave, Felix Schindler
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#include <dune/stuff/test/test_common.hh>

#include <utility>
#include <type_traits>

#include <dune/common/float_cmp.hh>

#include <dune/stuff/la/container.hh>

#include <dune/pymor/common/exceptions.hh>
#include <dune/pymor/parameters/base.hh>
#include <dune/pymor/parameters/functional.hh>
#include <dune/pymor/operators/interfaces.hh>
#include <dune/pymor/operators/affine.hh>
#include <dune/pymor/operators/base.hh>

using namespace Dune;
using namespace Pymor;

static const size_t dim = 4;


typedef testing::Types<
                        Pymor::Operators::MatrixBasedDefault< Stuff::LA::CommonDenseMatrix< double >,
                                                              Stuff::LA::CommonDenseVector< double > >
#if HAVE_EIGEN
                      , Pymor::Operators::MatrixBasedDefault< Stuff::LA::EigenDenseMatrix< double >,
                                                              Stuff::LA::EigenDenseVector< double > >
                      , Pymor::Operators::MatrixBasedDefault< Stuff::LA::EigenRowMajorSparseMatrix< double >,
                                                              Stuff::LA::EigenDenseVector< double > >
#endif // HAVE_EIGEN
#if HAVE_DUNE_ISTL
                      , Pymor::Operators::MatrixBasedDefault< Stuff::LA::IstlRowMajorSparseMatrix< double >,
                                                              Stuff::LA::IstlDenseVector< double > >
#endif
                      > MatrixBasedOperatorTypes;


template< class OperatorType >
struct MatrixBasedOperatorTests
  : public ::testing::Test
{
  void fulfills_interface() const
  {
    // static tests
    typedef typename OperatorType::Traits         Traits;
    // * of the traits
    typedef typename Traits::derived_type T_derived_type;
    static_assert(std::is_base_of< T_derived_type, OperatorType >::value,
                  "OperatorType::Traits::derived_type is wrong!");
    typedef typename Traits::SourceType   T_SourceType;
    typedef typename Traits::RangeType    T_RangeType;
    typedef typename Traits::ScalarType   T_ScalarType;
    typedef typename Traits::InverseType  T_InverseType;
    // * of the class itself (aka the derived type)
    typedef typename OperatorType::SourceType     D_SourceType;
    typedef typename OperatorType::RangeType      D_RangeType;
    typedef typename OperatorType::ScalarType     D_ScalarType;
    typedef typename OperatorType::InverseType    D_InverseType;
    typedef typename OperatorType::ContainerType  D_ContainerType;
    // * of the class as the operator interface
    typedef OperatorInterface< Traits >         InterfaceType;
    static_assert(std::is_base_of< OperatorInterface< Traits >, OperatorType >::value,
                  "OperatorType has to be derived from OperaotrInterface is wrong!");
    typedef typename InterfaceType::SourceType  I_SourceType;
    typedef typename InterfaceType::RangeType   I_RangeType;
    typedef typename InterfaceType::ScalarType  I_ScalarType;
    typedef typename InterfaceType::InverseType I_InverseType;
    // * of the class as provides the container interface
    static_assert(std::is_base_of< Stuff::LA::ProvidesConstContainer< Traits >, OperatorType >::value,
                  "OperatorType has to be derived from Stuff::LA::ProvidesConstContainer!");
    // dynamic tests
    // * of the class itself (aka the derived type)
    OperatorType d_from_ptr(new D_ContainerType(Stuff::LA::Container< D_ContainerType >::create(dim)));
    OperatorType d_from_shared_ptr(std::make_shared< D_ContainerType >(Stuff::LA::Container< D_ContainerType >::create(dim)));
    d_from_shared_ptr = d_from_ptr;
    OperatorType DUNE_UNUSED(d_copy_constructor)(d_from_ptr); // <- at this point, all operators share the same matrix!
    const bool d_linear = d_from_ptr.linear();
    if (!d_linear) DUNE_PYMOR_THROW(PymorException, "");
    if (d_from_ptr.parametric()) DUNE_PYMOR_THROW(PymorException, "");
    const DUNE_STUFF_SSIZE_T d_dim_source = d_from_ptr.dim_source();
    if (d_dim_source != dim) DUNE_PYMOR_THROW(PymorException, d_dim_source);
    const DUNE_STUFF_SSIZE_T d_dim_range = d_from_ptr.dim_range();
    if (d_dim_range != dim) DUNE_PYMOR_THROW(PymorException, d_dim_range);
    D_SourceType source = Stuff::LA::Container< D_SourceType >::create(dim);
    D_SourceType range = d_from_ptr.apply(source);
    d_from_ptr.apply(source, range);
    D_ScalarType DUNE_UNUSED(d_apply2) = d_from_ptr.apply2(range, source);
    std::vector< std::string > d_invert_options = d_from_ptr.invert_options();
    D_InverseType d_inverse = d_from_ptr.invert(d_invert_options[0]);
    try {
      d_inverse.apply(range, source);
      d_from_ptr.apply_inverse(source, range, d_invert_options[0]);
    } catch (Dune::Pymor::Exception::linear_solver_failed) {}
  } // ... fulfills_interface(...)
}; // struct MatrixBasedOperatorTests


TYPED_TEST_CASE(MatrixBasedOperatorTests, MatrixBasedOperatorTypes);
TYPED_TEST(MatrixBasedOperatorTests, fulfills_interface) {
  this->fulfills_interface();
}


//template< class OperatorImp >
//struct LinearAffinelyDecomposedContainerBasedOperatorTest
//  : public ::testing::Test
//{
//  void check() const
//  {
//    // static tests
//    typedef Operators::LinearAffinelyDecomposedContainerBased< OperatorImp > OperatorType;
//    typedef typename OperatorType::Traits Traits;
//    // * of the traits
//    typedef typename Traits::derived_type T_derived_type;
//    static_assert(std::is_same< OperatorType, T_derived_type >::value,
//                  "OperatorType::Traits::derived_type is wrong!");
//    typedef typename Traits::ComponentType ComponentType;
//    // * of the class itself (aka the derived type)
//    typedef typename OperatorType::ComponentType  D_ComponentType;
//    typedef typename OperatorType::FrozenType     D_FrozenType;
//    typedef typename OperatorType::ScalarType     D_ScalarType;
//    typedef typename OperatorType::SourceType     D_SourceType;
//    typedef typename OperatorType::RangeType      D_RangeType;
//    typedef typename OperatorType::InverseType    D_InverseType;
//    // * of the class as the interface
//    typedef AffinelyDecomposedOperatorInterface< Traits > InterfaceType;
//    typedef typename InterfaceType::derived_type I_derived_type;
//    static_assert(std::is_same< OperatorType, I_derived_type >::value,
//                  "InterfaceType::derived_type is wrong!");
//    typedef typename InterfaceType::ComponentType I_ComponentType;
//    typedef typename InterfaceType::FrozenType    I_FrozenType;
//    typedef typename InterfaceType::ScalarType    I_ScalarType;
//    typedef typename InterfaceType::SourceType    I_SourceType;
//    typedef typename InterfaceType::RangeType     I_RangeType;
//    typedef typename InterfaceType::InverseType   I_InverseType;
//    // dynamic tests
//    // * of the class itself (aka the derived type)
//    typedef typename OperatorImp::ContainerType MatrixType;
//    typedef LA::AffinelyDecomposedConstContainer< MatrixType > AffinelyDecomposedMatrixType;
//    AffinelyDecomposedMatrixType affinelyDecomposedMatrix(new MatrixType(Stuff::LA::Container< MatrixType >::create(dim)));
//    affinelyDecomposedMatrix.register_component(new MatrixType((Stuff::LA::Container< MatrixType >::create(dim))),
//                                                new ParameterFunctional("diffusion", 1, "diffusion[0]"));
//    affinelyDecomposedMatrix.register_component(new MatrixType((Stuff::LA::Container< MatrixType >::create(dim))),
//                                                new ParameterFunctional("force", 2, "force[0]"));
//    const Parameter mu = {{"diffusion", "force"},
//                          {{1.0}, {1.0, 1.0}}};
//    if (affinelyDecomposedMatrix.parameter_type() != mu.type())
//      DUNE_PYMOR_THROW(PymorException,
//                       "\nmu.type()                                 = " << mu.type()
//                       << "\naffinelyDecomposedMatrix.parameter_type() = "
//                       << affinelyDecomposedMatrix.parameter_type());
//    OperatorType d_operator(affinelyDecomposedMatrix);
//    if (!d_operator.parametric()) DUNE_PYMOR_THROW(PymorException, "");
//    if (d_operator.parameter_type() != mu.type())
//      DUNE_PYMOR_THROW(PymorException,
//                       "\nmu.type()                   = " << mu.type()
//                       << "\nd_operator.parameter_type() = " << d_operator.parameter_type());
//    const DUNE_STUFF_SSIZE_T d_num_components = d_operator.num_components();
//    if (d_num_components != 2) DUNE_PYMOR_THROW(PymorException, d_num_components);
//    for (unsigned int qq = 0; qq < d_num_components; ++qq) {
//      D_ComponentType component = d_operator.component(qq);
//      if (component.parametric()) DUNE_PYMOR_THROW(PymorException, "");
//      ParameterFunctional DUNE_UNUSED(coefficient) = d_operator.coefficient(qq);
//    }
//    if (!d_operator.has_affine_part()) DUNE_PYMOR_THROW(PymorException, "");
//    D_ComponentType d_affine_part = d_operator.affine_part();
//    if (d_affine_part.parametric()) DUNE_PYMOR_THROW(PymorException, "");
//    D_FrozenType d_frozen = d_operator.freeze_parameter(mu);
//    if (d_frozen.parametric()) DUNE_PYMOR_THROW(PymorException, "");
//    D_SourceType d_source(dim, D_ScalarType(1));
//    D_RangeType d_range = d_operator.apply(d_source, mu);
//    D_RangeType d_frozen_apply = d_frozen.apply(d_source);
//    if (!d_range.almost_equal(d_frozen_apply)) DUNE_PYMOR_THROW(PymorException, "");
//    std::vector< std::string > d_invert_options = d_operator.invert_options();
//    D_InverseType d_inverse = d_operator.invert(d_invert_options[0], mu);
//    try {
//      d_inverse.apply(d_range, d_source);
//      d_operator.apply_inverse(d_source, d_range, d_invert_options[0], mu);
//    } catch (Dune::Pymor::Exception::linear_solver_failed) {}
//    // * of the class as the interface
//    InterfaceType& i_operator = static_cast< InterfaceType& >(d_operator);
//    if (!i_operator.parametric()) DUNE_PYMOR_THROW(PymorException, "");
//    if (i_operator.parameter_type() != mu.type())
//      DUNE_PYMOR_THROW(PymorException,
//                       "\nmu.type()                   = " << mu.type()
//                       << "\ni_operator.parameter_type() = " << i_operator.parameter_type());
//    const DUNE_STUFF_SSIZE_T i_num_components = i_operator.num_components();
//    if (i_num_components != 2) DUNE_PYMOR_THROW(PymorException, i_num_components);
//    for (DUNE_STUFF_SSIZE_T qq = 0; qq < i_num_components; ++qq) {
//      I_ComponentType component = i_operator.component(qq);
//      if (component.parametric()) DUNE_PYMOR_THROW(PymorException, "");
//      ParameterFunctional DUNE_UNUSED(coefficient) = i_operator.coefficient(qq);
//    }
//    if (!i_operator.has_affine_part()) DUNE_PYMOR_THROW(PymorException, "");
//    I_ComponentType i_affine_part = i_operator.affine_part();
//    if (i_affine_part.parametric()) DUNE_PYMOR_THROW(PymorException, "");
//    I_FrozenType i_frozen = i_operator.freeze_parameter(mu);
//    if (i_frozen.parametric()) DUNE_PYMOR_THROW(PymorException, "");
//    I_SourceType i_source(dim, I_ScalarType(1));
//    I_RangeType i_range = i_operator.apply(i_source, mu);
//    I_RangeType i_frozen_apply = i_frozen.apply(i_source);
//    if (!i_range.almost_equal(i_frozen_apply)) DUNE_PYMOR_THROW(PymorException, "");
//    std::vector< std::string > i_invert_options = i_operator.invert_options();
//    I_InverseType i_inverse = i_operator.invert(i_invert_options[0], mu);
//    try {
//      i_inverse.apply(i_range, i_source);
//      i_operator.apply_inverse(i_source, i_range, i_invert_options[0], mu);
//    } catch (Dune::Pymor::Exception::linear_solver_failed) {}
//  }
//}; // struct LinearAffinelyDecomposedContainerBasedOperatorTest


//TYPED_TEST_CASE(LinearAffinelyDecomposedContainerBasedOperatorTest, MatrixBasedOperatorTypes);
//TYPED_TEST(LinearAffinelyDecomposedContainerBasedOperatorTest, OPERATORS) {
//  this->check();
//}


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
