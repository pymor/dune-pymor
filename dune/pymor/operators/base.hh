// This file is part of the dune-pymor project:
//   https://github.com/pymor/dune-pymor
// Copyright Holders: Stephan Rave, Felix Schindler
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_OPERATORS_BASE_HH
#define DUNE_PYMOR_OPERATORS_BASE_HH

#ifndef DUNE_STUFF_SSIZE_T
# define DUNE_STUFF_SSIZE_T long int
#endif

#include <type_traits>

#include <dune/stuff/la/container/interfaces.hh>
#include <dune/stuff/la/solver.hh>
#include <dune/stuff/common/exceptions.hh>

#include "interfaces.hh"

namespace Dune {
namespace Pymor {
namespace Operators {


template< class ImpTraits >
class MatrixBasedInverseBase
  : public OperatorInterface< ImpTraits >
{
  typedef OperatorInterface< ImpTraits > BaseType;
public:
  typedef ImpTraits Traits;
  typedef typename Traits::ScalarType   ScalarType;
  typedef typename Traits::SourceType   SourceType;
  typedef typename Traits::RangeType    RangeType;
  typedef typename Traits::MatrixType   MatrixType;
  typedef typename Traits::FrozenType   FrozenType;
  typedef typename Traits::InverseType  InverseType;
protected:
  typedef Stuff::LA::Solver< MatrixType >  LinearSolverType;
  static_assert(std::is_base_of< Stuff::LA::MatrixInterface< typename MatrixType::Traits >, MatrixType >::value,
                "MatrixType has to be derived from Stuff::LA::MatrixInterface!");

public:
  MatrixBasedInverseBase(const MatrixType* matrix_ptr, const std::string type = LinearSolverType::options()[0])
    : matrix_(matrix_ptr)
    , options_(LinearSolverType::options(type))
  {}

  MatrixBasedInverseBase(const MatrixType* matrix_ptr, const Stuff::Common::ConfigTree& options)
    : matrix_(matrix_ptr)
    , options_(options)
  {}

  MatrixBasedInverseBase(const std::shared_ptr< const MatrixType > matrix_ptr,
                         const std::string type = LinearSolverType::options()[0])
    : matrix_(matrix_ptr)
    , options_(LinearSolverType::options(type))
  {}

  MatrixBasedInverseBase(const std::shared_ptr< const MatrixType > matrix_ptr,
                         const Stuff::Common::ConfigTree& options)
    : matrix_(matrix_ptr)
    , options_(options)
  {}

  bool linear() const
  {
    return true;
  }

  DUNE_STUFF_SSIZE_T dim_source() const
  {
    return matrix_->pb_rows();
  }

  DUNE_STUFF_SSIZE_T dim_range() const
  {
    return matrix_->pb_cols();
  }

  void apply(const SourceType& source, RangeType& range, const Parameter mu = Parameter()) const
  {
    if (!mu.empty()) DUNE_PYMOR_THROW(Exception::this_is_not_parametric,
                                      "mu has to be empty if parametric() == false (is " << mu << ")!");
    if (source.pb_dim() != dim_source())
      DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                       "the dim of source (" << source.pb_dim() << ") does not match the dim_source of this ("
                       << dim_source() << ")!");
    if (range.pb_dim() != dim_range())
      DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                       "the dim of range (" << range.pb_dim() << ") does not match the dim_range of this ("
                       << dim_range() << ")!");
    return LinearSolverType(*matrix_).apply(source, range, options_);
  } // ... apply(...)

  using BaseType::apply;

  static std::vector< std::string > invert_options()
  {
    return { "exact" };
  }

  static Stuff::Common::ConfigTree invert_options(const std::string& type)
  {
    Stuff::LA::SolverUtils::check_given(type, invert_options());
    Stuff::Common::ConfigTree options("type", type);
    return options;
  } // invert_options(...)

  InverseType invert(const Stuff::Common::ConfigTree& options, const Parameter mu = Parameter()) const
  {
    if (!mu.empty()) DUNE_PYMOR_THROW(Exception::this_is_not_parametric,
                                      "mu has to be empty if parametric() == false (is " << mu << ")!");
    if (!options.has_key("type"))
      DUNE_THROW_COLORFULLY(Stuff::Exceptions::configuration_error,
                            "Given options (see below) need to have at least the key 'type' set!\n\n" << options);
    Stuff::LA::SolverUtils::check_given(options.get< std::string >("type"), invert_options());
    return InverseType(matrix_);
  } // ... invert(...)

  using BaseType::invert;

  FrozenType freeze_parameter(const Parameter mu = Parameter()) const
  {
    if (!mu.empty())
      DUNE_THROW_COLORFULLY(Stuff::Exceptions::internal_error, "This must not happen");
    DUNE_PYMOR_THROW(Exception::this_is_not_parametric,
                     "do not call freeze_parameter(" << mu << ") if parametric() == false!");
    return FrozenType(nullptr, invert_options()[0]);
  } // ... freeze_parameter(...)

private:
  std::shared_ptr< const MatrixType > matrix_;
  const Stuff::Common::ConfigTree options_;
}; // class MatrixBasedInverseBase


template< class ImpTraits >
class MatrixBasedBase
  : public OperatorInterface< ImpTraits >
  , public Stuff::LA::ProvidesConstContainer< ImpTraits >
{
  typedef OperatorInterface< ImpTraits > BaseType;
public:
  typedef ImpTraits Traits;
  typedef typename Traits::derived_type           ThisType;
  typedef typename Traits::ScalarType             ScalarType;
  typedef typename Traits::SourceType             SourceType;
  typedef typename Traits::RangeType              RangeType;
  typedef typename Traits::ContainerType          ContainerType;
  typedef typename Traits::FrozenType             FrozenType;
  typedef typename Traits::InverseType            InverseType;
  typedef ContainerType MatrixType;

private:
  typedef Stuff::LA::Solver< MatrixType >  LinearSolverType;
  static_assert(std::is_base_of< Stuff::LA::MatrixInterface< typename MatrixType::Traits >, MatrixType >::value,
                "MatrixType has to be derived from Stuff::LA::MatrixInterface!");

public:
  /**
   * \attention This class takes ownership of matrix_ptr!
   */
  MatrixBasedBase(const MatrixType* matrix_ptr)
    : matrix_(matrix_ptr)
  {}

  MatrixBasedBase(const std::shared_ptr< const MatrixType > matrix_ptr)
    : matrix_(matrix_ptr)
  {}

  bool linear() const
  {
    return true;
  }

  DUNE_STUFF_SSIZE_T dim_source() const
  {
    return matrix_->pb_cols();
  }

  DUNE_STUFF_SSIZE_T dim_range() const
  {
    return matrix_->pb_rows();
  }

  void apply(const SourceType& source, RangeType& range, const Parameter mu = Parameter()) const
  {
    if (!mu.empty()) DUNE_PYMOR_THROW(Exception::this_is_not_parametric,
                                      "mu has to be empty if parametric() == false (is " << mu << ")!");
    if (source.pb_dim() != dim_source())
      DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                       "the dim of source (" << source.pb_dim() << ") does not match the dim_source of this ("
                       << dim_source() << ")!");
    if (range.pb_dim() != dim_range())
      DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                       "the dim of range (" << range.pb_dim() << ") does not match the dim_range of this ("
                       << dim_range() << ")!");
    matrix_->mv(source, range);
  } // ... apply(...)

  using BaseType::apply;

  static std::vector< std::string > invert_options()
  {
    return LinearSolverType::options();
  }

  static Stuff::Common::ConfigTree invert_options(const std::string& type)
  {
    return LinearSolverType::options(type);
  }

  InverseType invert(const Stuff::Common::ConfigTree& options, const Parameter mu = Parameter()) const
  {
    if (!mu.empty()) DUNE_PYMOR_THROW(Exception::this_is_not_parametric,
                                      "mu has to be empty if parametric() == false (is " << mu << ")!");
    if (!options.has_key("type"))
      DUNE_THROW_COLORFULLY(Stuff::Exceptions::configuration_error,
                            "Given options (see below) need to have at least the key 'type' set!\n\n" << options);
    Stuff::LA::SolverUtils::check_given(options.get< std::string >("type"), invert_options());
    return InverseType(matrix_, options);
  } // ... invert(...)

  using BaseType::invert;

  FrozenType freeze_parameter(const Parameter mu = Parameter()) const
  {
    if (!mu.empty())
      DUNE_THROW_COLORFULLY(Stuff::Exceptions::internal_error, "This must not happen");
    DUNE_PYMOR_THROW(Exception::this_is_not_parametric,
                     "do not call freeze_parameter(" << mu << ") if parametric() == false!");
    return FrozenType(nullptr, invert_options()[0]);
  } // ... freeze_parameter(...)

  std::shared_ptr< const ContainerType > container() const
  {
    return matrix_;
  }

private:
  std::shared_ptr< const ContainerType > matrix_;
}; // class MatrixBasedBase


} // namespace Operators
} // namespace Pymor
} // namespace Dune

#endif // DUNE_PYMOR_OPERATORS_BASE_HH
