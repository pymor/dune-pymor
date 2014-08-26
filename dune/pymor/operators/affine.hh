// This file is part of the dune-pymor project:
//   https://github.com/pymor/dune-pymor
// Copyright holders: Stephan Rave, Felix Schindler
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_OPERATORS_AFFINE_HH
#define DUNE_PYMOR_OPERATORS_AFFINE_HH

#include <type_traits>

#include <dune/stuff/la/container.hh>
#include <dune/stuff/la/container/interfaces.hh>

#include <dune/pymor/la/container/affine.hh>

#include "base.hh"
#include "interfaces.hh"

namespace Dune {
namespace Pymor {
namespace Operators {


template< class MatrixImp, class VectorImp >
class LinearAffinelyDecomposedContainerBased;


template< class MatrixImp, class VectorImp >
class LinearAffinelyDecomposedContainerBasedTraits
{
public:
  typedef LinearAffinelyDecomposedContainerBased< MatrixImp, VectorImp > derived_type;
  typedef Operators::MatrixBasedDefault< MatrixImp, VectorImp > ComponentType;
  typedef ComponentType                       FrozenType;
  typedef typename ComponentType::SourceType  SourceType;
  typedef typename ComponentType::RangeType   RangeType;
  typedef typename ComponentType::ScalarType  ScalarType;
  typedef typename ComponentType::InverseType InverseType;
}; // class LinearAffinelyDecomposedContainerBasedTraits


template< class MatrixImp, class VectorImp >
class LinearAffinelyDecomposedContainerBased
  : public AffinelyDecomposedOperatorInterface< LinearAffinelyDecomposedContainerBasedTraits< MatrixImp, VectorImp > >
{
  typedef AffinelyDecomposedOperatorInterface< LinearAffinelyDecomposedContainerBasedTraits< MatrixImp, VectorImp > > BaseType;
public:
  typedef LinearAffinelyDecomposedContainerBasedTraits< MatrixImp, VectorImp > Traits;
  typedef typename Traits::derived_type   ThisType;
  typedef typename Traits::ComponentType  ComponentType;
  typedef typename Traits::SourceType     SourceType;
  typedef typename Traits::RangeType      RangeType;
  typedef typename Traits::ScalarType     ScalarType;
  typedef typename Traits::FrozenType     FrozenType;
  typedef typename Traits::InverseType    InverseType;

private:
  typedef LA::AffinelyDecomposedConstContainer< MatrixImp > AffinelyDecomposedContainerType;

public:
  LinearAffinelyDecomposedContainerBased(const AffinelyDecomposedContainerType affinelyDecomposedContainer)
    : BaseType(affinelyDecomposedContainer)
    , affinelyDecomposedContainer_(affinelyDecomposedContainer)
  {
    if (!affinelyDecomposedContainer_.has_affine_part() && affinelyDecomposedContainer_.num_components() == 0)
      DUNE_THROW(Stuff::Exceptions::requirements_not_met, "affinelyDecomposedContainer must not be empty!");
    if (affinelyDecomposedContainer_.has_affine_part()) {
      dim_source_ = affinelyDecomposedContainer_.affine_part()->cols();
      dim_range_ = affinelyDecomposedContainer_.affine_part()->rows();
    } else {
      dim_source_ = affinelyDecomposedContainer_.component(0)->cols();
      dim_range_ = affinelyDecomposedContainer_.component(0)->rows();
    }
  }

  DUNE_STUFF_SSIZE_T num_components() const
  {
    return affinelyDecomposedContainer_.num_components();
  }

  ComponentType component(const DUNE_STUFF_SSIZE_T qq) const
  {
    return ComponentType(affinelyDecomposedContainer_.component(qq));
  }

  ParameterFunctional coefficient(const DUNE_STUFF_SSIZE_T qq) const
  {
    return ParameterFunctional(*(affinelyDecomposedContainer_.coefficient(qq)));
  }

  bool has_affine_part() const
  {
    return affinelyDecomposedContainer_.has_affine_part();
  }

  ComponentType affine_part() const
  {
    return ComponentType(affinelyDecomposedContainer_.affine_part());
  }

  bool linear() const
  {
    return true;
  }

  DUNE_STUFF_SSIZE_T dim_source() const
  {
    return dim_source_;
  }

  DUNE_STUFF_SSIZE_T dim_range() const
  {
    return dim_range_;
  }

  void apply(const SourceType& source, RangeType& range, const Parameter mu = Parameter()) const
  {
    if (mu.type() != Parametric::parameter_type())
      DUNE_THROW(Exceptions::wrong_parameter_type, "the type of mu (" << mu.type()
                 << ") does not match the parameter_type of this (" << Parametric::parameter_type() << ")!");
    if (!Parametric::parametric())
      ComponentType(affinelyDecomposedContainer_.affine_part()).apply(source, range);
    else
      freeze_parameter(mu).apply(source, range);
  }

  using BaseType::apply;

  static std::vector< std::string > invert_options()
  {
    return ComponentType::invert_options();
  }

  static Stuff::Common::Configuration invert_options(const std::string& type)
  {
    return ComponentType::invert_options(type);
  }

  InverseType invert(const Stuff::Common::Configuration& option, const Parameter mu = Parameter()) const
  {
    return freeze_parameter(mu).invert(option);
  }

  FrozenType freeze_parameter(const Parameter mu = Parameter()) const
  {
    if (!Parametric::parametric())
      DUNE_THROW(Exceptions::this_is_not_parametric,
                 "do not call freeze_parameter(" << mu << ")" << " if parametric() == false!");
    if (mu.type() != Parametric::parameter_type())
      DUNE_THROW(Exceptions::wrong_parameter_type,
                 "the type of mu (" << mu.type() << ") does not match the parameter_type of this ("
                 << Parametric::parameter_type() << ")!");
    return FrozenType(new MatrixImp(affinelyDecomposedContainer_.freeze_parameter(mu)));
  }

private:
  AffinelyDecomposedContainerType affinelyDecomposedContainer_;
  DUNE_STUFF_SSIZE_T dim_source_;
  DUNE_STUFF_SSIZE_T dim_range_;
}; // class LinearAffinelyDecomposedContainerBased


extern template class LinearAffinelyDecomposedContainerBased< Stuff::LA::CommonDenseMatrix< double >,
                                                              Stuff::LA::CommonDenseVector< double > >;

#if HAVE_EIGEN

extern template class LinearAffinelyDecomposedContainerBased< Stuff::LA::EigenDenseMatrix< double >,
                                                              Stuff::LA::EigenDenseVector< double > >;

extern template class LinearAffinelyDecomposedContainerBased< Stuff::LA::EigenDenseMatrix< double >,
                                                              Stuff::LA::EigenMappedDenseVector< double > >;

extern template class LinearAffinelyDecomposedContainerBased< Stuff::LA::EigenRowMajorSparseMatrix< double >,
                                                              Stuff::LA::EigenDenseVector< double > >;

extern template class LinearAffinelyDecomposedContainerBased< Stuff::LA::EigenRowMajorSparseMatrix< double >,
                                                              Stuff::LA::EigenMappedDenseVector< double > >;

#endif // HAVE_EIGEN
#if HAVE_DUNE_ISTL

extern template class LinearAffinelyDecomposedContainerBased< Stuff::LA::IstlRowMajorSparseMatrix< double >,
                                                              Stuff::LA::IstlDenseVector< double > >;

#endif // HAVE_DUNE_ISTL

} // namespace Operators
} // namespace Pymor
} // namespace Dune

#endif // DUNE_PYMOR_OPERATORS_AFFINE_HH
