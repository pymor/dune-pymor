// This file is part of the dune-pymor project:
//   https://github.com/pymor/dune-pymor
// Copyright holders: Stephan Rave, Felix Schindler
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_DISCRETIZATIONS_INTERFACES_HH
#define DUNE_PYMOR_DISCRETIZATIONS_INTERFACES_HH

#include <vector>
#include <string>

#include <dune/stuff/la/container/interfaces.hh>
#include <dune/stuff/common/crtp.hh>

#include <dune/pymor/parameters/base.hh>
#include <dune/pymor/operators/interfaces.hh>
#include <dune/pymor/functionals/interfaces.hh>
#include <dune/pymor/la/container/affine.hh>

namespace Dune {
namespace Pymor {
namespace Tags {


class StationaryDiscretizationInterface {};

class StationaryMultiscaleDiscretiztionInterface {};


} // namespace Tags


template< class Traits >
class StationaryDiscretizationInterface
  : public Parametric
  , public Stuff::CRTPInterface< StationaryDiscretizationInterface< Traits >, Traits >
  , public Tags::StationaryDiscretizationInterface
{
public:
  typedef typename Traits::derived_type   derived_type;
  typedef typename Traits::OperatorType   OperatorType;
  typedef typename Traits::FunctionalType FunctionalType;
  typedef typename Traits::ProductType    ProductType;
  typedef typename Traits::VectorType     VectorType;

  typedef Pymor::LA::AffinelyDecomposedContainer< VectorType > AffinelyDecomposedVectorType;

  StationaryDiscretizationInterface(const ParameterType tt = ParameterType())
    : Parametric(tt)
  {}

  StationaryDiscretizationInterface(const Parametric& other)
    : Parametric(other)
  {}

  OperatorType get_operator() const
  {
    CHECK_CRTP(this->as_imp(*this).get_operator());
    return this->as_imp(*this).get_operator();
  }

  OperatorType* get_operator_and_return_ptr() const
  {
    return new OperatorType(get_operator());
  }

  FunctionalType get_rhs() const
  {
    CHECK_CRTP(this->as_imp(*this).get_rhs());
    return this->as_imp(*this).get_rhs();
  }

  FunctionalType* get_rhs_and_return_ptr() const
  {
    return new FunctionalType(get_rhs());
  }

  std::vector< std::string > available_products() const
  {
    CHECK_CRTP(this->as_imp(*this).available_products());
    return this->as_imp(*this).available_products();
  }

  ProductType get_product(const std::string id) const
  {
    CHECK_CRTP(this->as_imp(*this).get_product(id));
    return this->as_imp(*this).get_product(id);
  }

  ProductType* get_product_and_return_ptr(const std::string id) const
  {
    return new ProductType(get_product(id));
  }

  std::vector< std::string > available_vectors() const
  {
    CHECK_CRTP(this->as_imp(*this).available_vectors());
    return this->as_imp(*this).available_vectors();
  }

  AffinelyDecomposedVectorType get_vector(const std::string id) const
  {
    CHECK_CRTP(this->as_imp(*this).get_vector(id));
    return this->as_imp(*this).get_vector(id);
  }

  VectorType* get_vector_and_return_ptr(const std::string id) const
  {
    return new VectorType(get_vector(id));
  }

  VectorType create_vector() const
  {
    CHECK_CRTP(this->as_imp(*this).create_vector());
    return this->as_imp(*this).create_vector();
  }

  VectorType* create_vector_and_return_ptr() const
  {
    return new VectorType(create_vector());
  }

//  std::vector< std::string > solver_options() const
//  {
//    CHECK_CRTP(this->as_imp(*this).solver_options());
//    return this->as_imp(*this).solver_options();
//  }

//  std::string solver_options(const std::string context) const
//  {
//    CHECK_CRTP(this->as_imp(*this).solver_options(context));
//    return this->as_imp(*this).solver_options(context);
//  }

  void solve(VectorType& vector, const Parameter mu = Parameter()) const
  {
    CHECK_AND_CALL_CRTP(this->as_imp(*this).solve(vector, mu));
  }

  VectorType solve(const Parameter mu = Parameter()) const
  {
    VectorType ret = create_vector();
    solve(ret, mu);
    return ret;
  }

  VectorType* solve_and_return_ptr(const Parameter mu = Parameter()) const
  {
    return new VectorType(solve(mu));
  }

  void visualize(const VectorType& vector, const std::string filename, const std::string name) const
  {
    CHECK_AND_CALL_CRTP(this->as_imp(*this).visualize(vector, filename, name));
  }
}; // class StationaryDiscretizationInterface


} // namespace Pymor
} // namespace Dune

#endif // DUNE_PYMOR_DISCRETIZATIONS_INTERFACES_HH
