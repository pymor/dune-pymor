// This file is part of the dune-pymor project:
//   https://github.com/pymor/dune-pymor
// Copyright holders: Stephan Rave, Felix Schindler
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_DISCRETIZATIONS_INTERFACES_HH
#define DUNE_PYMOR_DISCRETIZATIONS_INTERFACES_HH

#include <vector>
#include <string>

#include <dune/stuff/common/crtp.hh>
#include <dune/stuff/common/configuration.hh>
#include <dune/stuff/common/exceptions.hh>
#include <dune/stuff/la/container/interfaces.hh>

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
    CHECK_CRTP(this->as_imp().get_operator());
    return this->as_imp().get_operator();
  }

  FunctionalType get_rhs() const
  {
    CHECK_CRTP(this->as_imp().get_rhs());
    return this->as_imp().get_rhs();
  }

  std::vector< std::string > available_products() const
  {
    CHECK_CRTP(this->as_imp().available_products());
    return this->as_imp().available_products();
  }

  ProductType get_product(const std::string id) const
  {
    CHECK_CRTP(this->as_imp().get_product(id));
    return this->as_imp().get_product(id);
  }

  std::vector< std::string > available_vectors() const
  {
    CHECK_CRTP(this->as_imp().available_vectors());
    return this->as_imp().available_vectors();
  }

  AffinelyDecomposedVectorType get_vector(const std::string id) const
  {
    CHECK_CRTP(this->as_imp().get_vector(id));
    return this->as_imp().get_vector(id);
  }

  VectorType pb_get_vector(const std::string id) const
  {
    auto non_parametric_vector = get_vector(id);
    if (non_parametric_vector.parametric())
      DUNE_THROW(NotImplemented, "Requested vector '" << id << "' is parametric!");
    return *non_parametric_vector.affine_part();
  }

  VectorType create_vector() const
  {
    CHECK_CRTP(this->as_imp().create_vector());
    return this->as_imp().create_vector();
  }

  std::vector< std::string > solver_types() const
  {
    CHECK_CRTP(this->as_imp().solver_types());
    return this->as_imp().solver_types();
  }

  DSC::Configuration solver_options(const std::string type = "") const
  {
    const std::string tp = !type.empty() ? type : solver_types()[0];
    CHECK_CRTP(this->as_imp().solver_options(tp));
    return this->as_imp().solver_options(tp);
  }

  void solve(VectorType& vector, const Parameter mu = Parameter()) const
  {
    solve(solver_types()[0], vector, mu);
  }

  void solve(const std::string type, VectorType& vector, const Parameter mu = Parameter()) const
  {
    solve(solver_options(type), vector, mu);
  }

  void solve(const DSC::Configuration options, VectorType& vector, const Parameter mu = Parameter()) const
  {
    CHECK_AND_CALL_CRTP(this->as_imp().solve(options, vector, mu));
  }

  VectorType solve(const Parameter mu = Parameter()) const
  {
    VectorType ret = create_vector();
    solve(ret, mu);
    return ret;
  }

  VectorType solve(const std::string type, const Parameter mu = Parameter()) const
  {
    VectorType ret = create_vector();
    solve(type, ret, mu);
    return ret;
  }

  VectorType solve(const DSC::Configuration options, const Parameter mu = Parameter()) const
  {
    VectorType ret = create_vector();
    solve(options, ret, mu);
    return ret;
  }

  void visualize(const VectorType& vector, const std::string filename, const std::string name) const
  {
    CHECK_AND_CALL_CRTP(this->as_imp().visualize(vector, filename, name));
  }

  OperatorType* get_operator_and_return_ptr() const
  {
    return new OperatorType(get_operator());
  }

  FunctionalType* get_rhs_and_return_ptr() const
  {
    return new FunctionalType(get_rhs());
  }

  ProductType* get_product_and_return_ptr(const std::string id) const
  {
    return new ProductType(get_product(id));
  }

  VectorType* get_vector_and_return_ptr(const std::string id) const
  {
    auto vec = get_vector(id);
    if (vec.parametric())
      DUNE_THROW(NotImplemented, "Not implemented yet for parametric vectors!");
    return new VectorType(*(vec.affine_part()));
  }

  VectorType* create_vector_and_return_ptr() const
  {
    return new VectorType(create_vector());
  }

  VectorType* solve_and_return_ptr(const Parameter mu = Parameter()) const
  {
    return new VectorType(solve(mu));
  }

  VectorType* solve_and_return_ptr(const std::string type, const Parameter mu = Parameter()) const
  {
    return new VectorType(solve(type, mu));
  }

  VectorType* solve_and_return_ptr(const DSC::Configuration options, const Parameter mu = Parameter()) const
  {
    return new VectorType(solve(options, mu));
  }
}; // class StationaryDiscretizationInterface


} // namespace Pymor
} // namespace Dune

#endif // DUNE_PYMOR_DISCRETIZATIONS_INTERFACES_HH
