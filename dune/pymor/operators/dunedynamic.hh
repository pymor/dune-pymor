// This file is part of the dune-pymor project:
//   https://github.com/pymor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_OPERATORS_DUNEDYNAMIC_HH
#define DUNE_PYMOR_OPERATORS_DUNEDYNAMIC_HH

#include <dune/stuff/la/container/dunedynamic.hh>

#include <dune//common/exceptions.hh>
#include <dune/pymor/common/crtp.hh>

#include "interfaces.hh"

namespace Dune {
namespace Pymor {
namespace Operators {


template< class ScalarImp = double >
class DuneDynamic;

template< class ScalarImp = double >
class DuneDynamicInverse;


template< class ScalarImp >
class DuneDynamicInverseTraits
{
public:
  typedef ScalarImp                           ScalarType;
  typedef DuneDynamicInverse< ScalarType >    derived_type;
  typedef Stuff::LA::DuneDynamicMatrix< ScalarType > ContainerType;
  typedef Stuff::LA::DuneDynamicVector< ScalarType > SourceType;
  typedef Stuff::LA::DuneDynamicVector< ScalarType > RangeType;
  typedef derived_type                        FrozenType;
  typedef DuneDynamic< ScalarType >           InverseType;
};


template< class ScalarImp >
class DuneDynamicInverse
  : public OperatorInterface< DuneDynamicInverseTraits< ScalarImp > >
{
  typedef OperatorInterface< DuneDynamicInverseTraits< ScalarImp > > BaseType;
public:
  typedef DuneDynamicInverseTraits< ScalarImp > Traits;
  typedef typename Traits::derived_type         ThisType;
  typedef typename Traits::ScalarType           ScalarType;
  typedef typename Traits::SourceType           SourceType;
  typedef typename Traits::RangeType            RangeType;
  typedef typename Traits::ContainerType        ContainerType;
  typedef typename Traits::FrozenType           FrozenType;
  typedef typename Traits::InverseType          InverseType;

  DuneDynamicInverse(const ContainerType* matrix_ptr, const std::string option = "superlu");

  DuneDynamicInverse(const std::shared_ptr< const ContainerType > matrix_ptr, const std::string option = "superlu");

  bool linear() const;

  DUNE_STUFF_SSIZE_T dim_source() const;

  DUNE_STUFF_SSIZE_T dim_range() const;

  void apply(const SourceType& source, RangeType& range, const Parameter mu = Parameter()) const;

  using BaseType::apply;

  static std::vector< std::string > invert_options();

  InverseType invert(const std::string option = invert_options()[0],
                     const Parameter mu = Parameter()) const;

  FrozenType freeze_parameter(const Parameter mu = Parameter()) const;

private:
  std::shared_ptr< const ContainerType > matrix_;
}; // class DuneDynamicInverse


template< class ScalarImp >
class DuneDynamicTraits
{
public:
  typedef ScalarImp                           ScalarType;
  typedef DuneDynamic< ScalarType >           derived_type;
  typedef Stuff::LA::DuneDynamicMatrix< ScalarType > ContainerType;
  typedef Stuff::LA::DuneDynamicVector< ScalarType > SourceType;
  typedef Stuff::LA::DuneDynamicVector< ScalarType > RangeType;
  typedef derived_type                        FrozenType;
  typedef DuneDynamicInverse< ScalarType >    InverseType;
};


template< class ScalarImp >
class DuneDynamic
  : public OperatorInterface< DuneDynamicTraits< ScalarImp > >
  , public Stuff::LA::ProvidesContainer< DuneDynamicTraits< ScalarImp > >
{
  typedef OperatorInterface< DuneDynamicTraits< ScalarImp > > BaseType;
public:
  typedef DuneDynamicTraits< ScalarImp >  Traits;
  typedef typename Traits::derived_type   ThisType;
  typedef typename Traits::ScalarType     ScalarType;
  typedef typename Traits::SourceType     SourceType;
  typedef typename Traits::RangeType      RangeType;
  typedef typename Traits::ContainerType  ContainerType;
  typedef typename Traits::FrozenType     FrozenType;
  typedef typename Traits::InverseType    InverseType;

  /**
   * \attention This class takes ownership of matrix_ptr!
   */
  DuneDynamic(const ContainerType* matrix_ptr);

  DuneDynamic(const std::shared_ptr< const ContainerType > matrix_ptr);

  bool linear() const;

  DUNE_STUFF_SSIZE_T dim_source() const;

  DUNE_STUFF_SSIZE_T dim_range() const;

  void apply(const SourceType& source, RangeType& range, const Parameter mu = Parameter()) const;

  using BaseType::apply;

  static std::vector< std::string > invert_options();

  InverseType invert(const std::string option = invert_options()[0],
                     const Parameter mu = Parameter()) const;

  FrozenType freeze_parameter(const Parameter mu = Parameter()) const;

  std::shared_ptr< const ContainerType > container() const;

private:
  std::shared_ptr< const ContainerType > matrix_;
}; // class DuneDynamic


} // namespace Operators
} // namespace Pymor
} // namespace Dune

#endif // DUNE_PYMOR_OPERATORS_DUNEDYNAMIC_HH
