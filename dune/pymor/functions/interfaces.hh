// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_FUNCTIONS_INTERFACES_HH
#define DUNE_PYMOR_FUNCTIONS_INTERFACES_HH

#include <dune/common/fmatrix.hh>
#include <dune/common/fvector.hh>

#include <dune/pymor/parameters/base.hh>
#include <dune/pymor/parameters/functional.hh>
#include <dune/pymor/common/exceptions.hh>

namespace Dune {
namespace Pymor {


template< class DomainFieldImp, int domainDim, class RangeFieldImp, int rangeDimRows, int rangeDimCols >
class ParametricFunctionInterface
  : public Parametric
{
public:
  typedef DomainFieldImp                                  DomainFieldType;
  static const unsigned int                               dimDomain = domainDim;
  typedef Dune::FieldVector< DomainFieldType, dimDomain > DomainType;

  typedef RangeFieldImp                                                   RangeFieldType;
  static const unsigned int                                               dimRangeRows = rangeDimRows;
  static const unsigned int                                               dimRangeCols = rangeDimCols;
  typedef Dune::FieldMatrix< RangeFieldType, dimRangeRows, dimRangeCols > RangeType;

  template< class... Args >
  ParametricFunctionInterface(Args&& ...args)
    : Parametric(std::forward< Args >(args)...)
  {}

  static const std::string static_id()
  {
    return "parametricfunction";
  }

  virtual std::string id() const
  {
    return static_id();
  }

  virtual int order() const
  {
    return -1;
  }

  virtual evaluate(const DomainType& x, RangeType& ret, const Parameter mu = Parameter()) const = 0;

  virtual RangeType evaluate(const DomainType& x, const Parameter mu = Parameter()) const
  {
    if (mu.type() != Parametric::parameter_type())
      DUNE_PYMOR_THROW(Exception::wrong_parameter_type,
                       "type of mu (" << mu.type() << ") does not match the parameter_type of this ("
                       << Parametric::parameter_type() << ")!");
    RangeType ret;
    evaluate(x, ret, mu);
    return ret;
  }
}; // class ParametricFunctionInterface


template< class DomainFieldImp, int domainDim, class RangeFieldImp, int rangeDimRows >
class ParametricFunctionInterface< DomainFieldImp, domainDim, RangeFieldImp, rangeDimRows, 1 >
  : public Parametric
{
public:
  typedef DomainFieldImp                                  DomainFieldType;
  static const unsigned int                               dimDomain = domainDim;
  typedef Dune::FieldVector< DomainFieldType, dimDomain > DomainType;

  typedef RangeFieldImp                                 RangeFieldType;
  static const unsigned int                             dimRangeRows = rangeDimRows;
  static const unsigned int                             dimRangeCols = 1;
  static const unsigned int                             dimRange = 1;
  typedef Dune::FieldVector< RangeFieldType, dimRange > RangeType;

  template< class... Args >
  ParametricFunctionInterface(Args&& ...args)
    : Parametric(std::forward< Args >(args)...)
  {}

  static const std::string static_id()
  {
    return "parametricfunction";
  }

  virtual std::string id() const
  {
    return static_id();
  }

  virtual int order() const
  {
    return -1;
  }

  virtual evaluate(const DomainType& x, RangeType& ret, const Parameter mu = Parameter()) const = 0;

  virtual RangeType evaluate(const DomainType& x, const Parameter mu = Parameter()) const
  {
    if (mu.type() != Parametric::parameter_type())
      DUNE_PYMOR_THROW(Exception::wrong_parameter_type,
                       "type of mu (" << mu.type() << ") does not match the parameter_type of this ("
                       << Parametric::parameter_type() << ")!");
    RangeType ret;
    evaluate(x, ret, mu);
    return ret;
  }
}; // class ParametricFunctionInterface< ..., 1 >


template< class DomainFieldImp, int domainDim, class RangeFieldImp, int rangeDimRows, int rangeDimCols >
class AffineParametricFunctionInterface
  : public ParametricFunctionInterface< DomainFieldImp, domainDim, RangeFieldImp, rangeDimRows, rangeDimCols >
{
public:
  typedef ParametricFunctionInterface<  DomainFieldImp, domainDim,
                                        RangeFieldImp, rangeDimRows, rangeDimCols > ParametricFunctionType;

  static const std::string static_id()
  {
    return "affineparametricfunction";
  }

  virtual std::string id() const
  {
    return static_id();
  }

  virtual bool hasAffinePart() const = 0;

  ParametricFunctionType* affinePart() const = 0;

  unsigned int size() const = 0;

  ParameterFunctional* coefficient(int ii) const = 0;

  ParametricFunctionType* component(int ii) const = 0;
}; // class AffineParametricFunctionInterface


} // namespace Pymor
} // namespace Dune

#endif // DUNE_PYMOR_FUNCTIONS_INTERFACES_HH
