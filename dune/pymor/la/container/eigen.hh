// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_LA_CONTAINER_EIGEN_HH
#define DUNE_PYMOR_LA_CONTAINER_EIGEN_HH

#if HAVE_EIGEN

#include <dune/stuff/la/container/eigen.hh>

#include "interfaces.hh"

namespace Dune {
namespace Pymor {
namespace LA {


class EigenDenseVector
  : public Dune::Stuff::LA::EigenDenseVector< double >
  , public Dune::Pymor::LA::VectorInterface
{
  typedef Dune::Stuff::LA::EigenDenseVector< double > BaseType;
public:
  typedef EigenDenseVector ThisType;

  EigenDenseVector(const int ss)
    : BaseType(ss)
  {}

  static ThisType* create(const int ss)
  {
    return new ThisType(ss);
  }

  virtual std::string type() const
  {
    return "dunepymor.vector.eigendense";
  }

  virtual int dim() const
  {
    return BaseType::size();
  }
}; // class EigenDenseVector

} // namespace LA
} // namespace Pymor
} // namespace Dune

#endif // HAVE_EIGEN

#endif // DUNE_PYMOR_LA_CONTAINER_EIGEN_HH
