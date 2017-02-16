// This file is part of the dune-pymor project:
//   https://github.com/pymor/dune-pymor
// Copyright holders: Stephan Rave, Felix Schindler
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_OPERATORS_BASE_HH
#define DUNE_PYMOR_OPERATORS_BASE_HH

#include <type_traits>

#include <dune/stuff/common/exceptions.hh>
#include <dune/stuff/common/profiler.hh>
#include <dune/stuff/la/container.hh>
#include <dune/stuff/la/container/interfaces.hh>
#include <dune/stuff/la/solver.hh>

#include "interfaces.hh"

namespace Dune {
namespace Pymor {
namespace Operators {


// forwards
template< class MatrixType, class VectorType, class Communicator >
class MatrixBasedInverseDefault;

template< class MatrixType, class VectorType, class Communicator >
class MatrixBasedDefault;


namespace internal {


template< class MatrixType, class VectorType, class CommunicatorType >
class MatrixBasedDefaultTraits
{
  static_assert(std::is_base_of< Stuff::LA::MatrixInterface< typename MatrixType::Traits >, MatrixType >::value,
                "MatrixType has to be derived from Stuff::LA::MatrixInterface!");
  static_assert(std::is_base_of< Stuff::LA::VectorInterface< typename VectorType::Traits >, VectorType >::value,
                "VectorType has to be derived from Stuff::LA::VectorInterface!");
  static_assert(std::is_same< typename MatrixType::ScalarType, typename VectorType::ScalarType >::value,
                "Types do not match!");
public:
  typedef MatrixBasedDefault< MatrixType, VectorType, CommunicatorType > derived_type;
  typedef MatrixType ContainerType;
  typedef VectorType SourceType;
  typedef VectorType RangeType;
  typedef typename MatrixType::ScalarType ScalarType;
  typedef MatrixBasedDefault< MatrixType, VectorType, CommunicatorType > FrozenType;
  typedef MatrixBasedInverseDefault< MatrixType, VectorType, CommunicatorType > InverseType;
}; // class MatrixBasedDefaultTraits


template< class MatrixType, class VectorType, class CommunicatorType >
class MatrixBasedInverseDefaultTraits
{
  static_assert(std::is_base_of< Stuff::LA::MatrixInterface< typename MatrixType::Traits >, MatrixType >::value,
                "MatrixType has to be derived from Stuff::LA::MatrixInterface!");
  static_assert(std::is_base_of< Stuff::LA::VectorInterface< typename VectorType::Traits >, VectorType >::value,
                "VectorType has to be derived from Stuff::LA::VectorInterface!");
  static_assert(std::is_same< typename MatrixType::ScalarType, typename VectorType::ScalarType >::value,
                "Types do not match!");
public:
  typedef MatrixBasedInverseDefault< MatrixType, VectorType, CommunicatorType > derived_type;
  typedef VectorType SourceType;
  typedef VectorType RangeType;
  typedef typename MatrixType::ScalarType ScalarType;
  typedef MatrixBasedInverseDefault< MatrixType, VectorType, CommunicatorType > FrozenType;
  typedef MatrixBasedDefault< MatrixType, VectorType, CommunicatorType > InverseType;
}; // class MatrixBasedInverseDefaultTraits


} // namespace internal


template< class MatrixImp, class VectorType, class CommunicatorType >
class MatrixBasedInverseDefault
  : public OperatorInterface< internal::MatrixBasedInverseDefaultTraits< MatrixImp, VectorType, CommunicatorType > >
{
  typedef OperatorInterface< internal::MatrixBasedInverseDefaultTraits< MatrixImp, VectorType, CommunicatorType > > BaseType;
public:
  typedef internal::MatrixBasedInverseDefaultTraits< MatrixImp, VectorType, CommunicatorType > Traits;
  typedef typename Traits::ScalarType   ScalarType;
  typedef typename Traits::SourceType   SourceType;
  typedef typename Traits::RangeType    RangeType;
  typedef typename Traits::FrozenType   FrozenType;
  typedef typename Traits::InverseType  InverseType;
protected:
  typedef MatrixImp MatrixType;
  typedef Stuff::LA::Solver< MatrixType, CommunicatorType > LinearSolverType;

public:
  MatrixBasedInverseDefault(const CommunicatorType& comm, const MatrixType* matrix_ptr, const std::string type = LinearSolverType::options()[0])
    : communicator_(comm)
    , matrix_(matrix_ptr)
    , options_(LinearSolverType::options(type))
  {}

  MatrixBasedInverseDefault(const CommunicatorType& comm, const MatrixType* matrix_ptr, const Stuff::Common::Configuration& options)
    : communicator_(comm)
    , matrix_(matrix_ptr)
    , options_(options)
  {}

  MatrixBasedInverseDefault(const CommunicatorType& comm, const std::shared_ptr< const MatrixType > matrix_ptr,
                            const std::string type = LinearSolverType::options()[0])
    : communicator_(comm)
    , matrix_(matrix_ptr)
    , options_(LinearSolverType::options(type))
  {}

  MatrixBasedInverseDefault(const CommunicatorType& comm, const std::shared_ptr< const MatrixType > matrix_ptr,
                            const Stuff::Common::Configuration& options)
    : communicator_(comm)
    , matrix_(matrix_ptr)
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
    if (!mu.empty())
      DUNE_THROW(Exceptions::this_is_not_parametric,
                 "mu has to be empty if parametric() == false (is " << mu << ")!");
    if (source.pb_dim() != dim_source())
      DUNE_THROW(Stuff::Exceptions::shapes_do_not_match,
                 "the dim of source (" << source.pb_dim() << ") does not match the dim_source of this ("
                 << dim_source() << ")!");
    if (range.pb_dim() != dim_range())
      DUNE_THROW(Stuff::Exceptions::shapes_do_not_match,
                 "the dim of range (" << range.pb_dim() << ") does not match the dim_range of this ("
                 << dim_range() << ")!");
    LinearSolverType(*matrix_, communicator_).apply(source, range, options_);
  } // ... apply(...)

  using BaseType::apply;

  static std::vector< std::string > invert_options()
  {
    return { "exact" };
  }

  static Stuff::Common::Configuration invert_options(const std::string& type)
  {
    Stuff::LA::SolverUtils::check_given(type, invert_options());
    Stuff::Common::Configuration options("type", type);
    return options;
  } // invert_options(...)

  InverseType invert(const Stuff::Common::Configuration& options, const Parameter mu = Parameter()) const
  {
    if (!mu.empty())
      DUNE_THROW(Exceptions::this_is_not_parametric, "mu has to be empty if parametric() == false (is " << mu << ")!");
    if (!options.has_key("type"))
      DUNE_THROW(Stuff::Exceptions::configuration_error,
                 "Given options (see below) need to have at least the key 'type' set!\n\n" << options);
    Stuff::LA::SolverUtils::check_given(options.get< std::string >("type"), invert_options());
    return InverseType(matrix_);
  } // ... invert(...)

  using BaseType::invert;

  FrozenType freeze_parameter(const Parameter mu = Parameter()) const
  {
    if (!mu.empty())
      DUNE_THROW(Stuff::Exceptions::internal_error, "This must not happen");
    DUNE_THROW(Exceptions::this_is_not_parametric,
               "do not call freeze_parameter(" << mu << ") if parametric() == false!");
    return FrozenType(nullptr, invert_options()[0]);
  } // ... freeze_parameter(...)

private:
  std::shared_ptr< const MatrixType > matrix_;
  const CommunicatorType& communicator_;
  const Stuff::Common::Configuration options_;
}; // class MatrixBasedInverseDefault


template< class MatrixImp, class VectorType, class CommunicatorType >
class MatrixBasedDefault
  : public OperatorInterface< internal::MatrixBasedDefaultTraits< MatrixImp, VectorType, CommunicatorType > >
  , public Stuff::LA::ProvidesConstContainer< internal::MatrixBasedDefaultTraits< MatrixImp, VectorType, CommunicatorType > >
{
  typedef OperatorInterface< internal::MatrixBasedDefaultTraits< MatrixImp, VectorType, CommunicatorType > > BaseType;
public:
  typedef internal::MatrixBasedDefaultTraits< MatrixImp, VectorType, CommunicatorType > Traits;
  typedef typename Traits::derived_type   ThisType;
  typedef typename Traits::ScalarType     ScalarType;
  typedef typename Traits::SourceType     SourceType;
  typedef typename Traits::RangeType      RangeType;
  typedef typename Traits::ContainerType  ContainerType;
  typedef typename Traits::FrozenType     FrozenType;
  typedef typename Traits::InverseType    InverseType;

protected:
  typedef MatrixImp MatrixType;
private:
  typedef Stuff::LA::Solver< MatrixImp >  LinearSolverType;
  static_assert(std::is_base_of< Stuff::LA::MatrixInterface< typename MatrixType::Traits >, MatrixType >::value,
                "MatrixType has to be derived from Stuff::LA::MatrixInterface!");

public:
  static std::string static_id() { return "pymor.operators.matrixbaseddefault"; }

  /**
   * \attention This class takes ownership of matrix_ptr!
   */
  MatrixBasedDefault(const CommunicatorType& comm, const MatrixType* matrix_ptr)
    : communicator_(comm)
    , matrix_(matrix_ptr)
  {}

  MatrixBasedDefault(const CommunicatorType& comm, const std::shared_ptr< const MatrixType > matrix_ptr)
    : communicator_(comm)
    , matrix_(matrix_ptr)
  {}

  MatrixBasedDefault(const CommunicatorType& comm, const MatrixType& matrix)
    : communicator_(comm)
    , matrix_(new MatrixType(matrix))
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
    DUNE_STUFF_PROFILE_SCOPE(static_id() + ".apply");
    if (!mu.empty()) DUNE_THROW(Exceptions::this_is_not_parametric,
                                "mu has to be empty if parametric() == false (is " << mu << ")!");
    if (source.pb_dim() != dim_source())
      DUNE_THROW(Stuff::Exceptions::shapes_do_not_match,
                 "the dim of source (" << source.pb_dim() << ") does not match the dim_source of this ("
                 << dim_source() << ")!");
    if (range.pb_dim() != dim_range())
      DUNE_THROW(Stuff::Exceptions::shapes_do_not_match,
                 "the dim of range (" << range.pb_dim() << ") does not match the dim_range of this ("
                 << dim_range() << ")!");
    matrix_->mv(source, range);
  } // ... apply(...)

  using BaseType::apply;

  static std::vector< std::string > invert_options()
  {
    return LinearSolverType::types();
  }

  static Stuff::Common::Configuration invert_options(const std::string& type)
  {
    return LinearSolverType::options(type);
  }

  InverseType invert(const Stuff::Common::Configuration& options, const Parameter mu = Parameter()) const
  {
    if (!mu.empty()) DUNE_THROW(Exceptions::this_is_not_parametric,
                                "mu has to be empty if parametric() == false (is " << mu << ")!");
    if (!options.has_key("type"))
      DUNE_THROW(Stuff::Exceptions::configuration_error,
                 "Given options (see below) need to have at least the key 'type' set!\n\n" << options);
    Stuff::LA::SolverUtils::check_given(options.get< std::string >("type"), invert_options());
    return InverseType(communicator_, matrix_, options);
  } // ... invert(...)

  using BaseType::invert;

  FrozenType freeze_parameter(const Parameter mu = Parameter()) const
  {
    if (!mu.empty())
      DUNE_THROW(Stuff::Exceptions::internal_error, "This must not happen");
    DUNE_THROW(Exceptions::this_is_not_parametric,
               "do not call freeze_parameter(" << mu << ") if parametric() == false!");
    return FrozenType(nullptr);
  } // ... freeze_parameter(...)

  std::shared_ptr< const ContainerType > container() const
  {
    return matrix_;
  }

  ContainerType* pb_container() const
  {
    return new ContainerType(*matrix_);
  }

private:
  std::shared_ptr< const ContainerType > matrix_;
  const CommunicatorType& communicator_;
}; // class MatrixBasedDefault


} // namespace Operators
} // namespace Pymor
} // namespace Dune

#endif // DUNE_PYMOR_OPERATORS_BASE_HH
