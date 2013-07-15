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


template< class DomainFieldImp, int domainDim, class RangeFieldImp, int rangeDimRows, int rangeDimCols = 1 >
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

  ParametricFunctionInterface();

  ParametricFunctionInterface(const ParameterType& tt);

  ParametricFunctionInterface(const std::string& kk, const int& vv) throw (Exception::key_is_not_valid,
                                                                           Exception::index_out_of_range);

  ParametricFunctionInterface(const std::vector< std::string >& kk,
                              const std::vector< int >& vv) throw (Exception::key_is_not_valid,
                                                                   Exception::index_out_of_range,
                                                                   Exception::sizes_do_not_match);

  ParametricFunctionInterface(const Parametric& other);

  virtual std::string name() const;

  virtual int order() const;

  virtual void evaluate(const DomainType& x, RangeType& ret, const Parameter mu = Parameter()) const = 0;

  virtual RangeType evaluate(const DomainType& x, const Parameter mu = Parameter()) const
    throw (Exception::wrong_parameter_type);
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

  ParametricFunctionInterface();

  ParametricFunctionInterface(const ParameterType& tt);

  ParametricFunctionInterface(const std::string& kk, const int& vv) throw (Exception::key_is_not_valid,
                                                                           Exception::index_out_of_range);

  ParametricFunctionInterface(const std::vector< std::string >& kk,
                              const std::vector< int >& vv) throw (Exception::key_is_not_valid,
                                                                   Exception::index_out_of_range,
                                                                   Exception::sizes_do_not_match);

  ParametricFunctionInterface(const Parametric& other);

  virtual std::string name() const;

  virtual int order() const;

  virtual void evaluate(const DomainType& x, RangeType& ret, const Parameter mu = Parameter()) const = 0;

  virtual RangeType evaluate(const DomainType& x, const Parameter mu = Parameter()) const
    throw (Exception::wrong_parameter_type);
}; // class ParametricFunctionInterface< ..., 1 >


template< class DomainFieldImp, int domainDim, class RangeFieldImp, int rangeDimRows, int rangeDimCols = 1 >
class AffineParametricFunctionInterface
  : public ParametricFunctionInterface< DomainFieldImp, domainDim, RangeFieldImp, rangeDimRows, rangeDimCols >
{
public:
  typedef ParametricFunctionInterface<  DomainFieldImp, domainDim,
                                        RangeFieldImp, rangeDimRows, rangeDimCols > ParametricFunctionType;

  AffineParametricFunctionInterface();

  AffineParametricFunctionInterface(const ParameterType& tt);

  AffineParametricFunctionInterface(const std::string& kk, const int& vv) throw (Exception::key_is_not_valid,
                                                                                 Exception::index_out_of_range);

  AffineParametricFunctionInterface(const std::vector< std::string >& kk,
                                    const std::vector< int >& vv) throw (Exception::key_is_not_valid,
                                                                         Exception::index_out_of_range,
                                                                         Exception::sizes_do_not_match);

  AffineParametricFunctionInterface(const Parametric& other);

  virtual std::string name() const;

  virtual bool hasAffinePart() const = 0;

  virtual const ParametricFunctionType* affinePart() const = 0;

  virtual unsigned int num_components() const = 0;

  virtual const ParameterFunctional* coefficient(int ii) const = 0;

  virtual const ParametricFunctionType* component(int ii) const = 0;
}; // class AffineParametricFunctionInterface


} // namespace Pymor
} // namespace Dune

#endif // DUNE_PYMOR_FUNCTIONS_INTERFACES_HH
