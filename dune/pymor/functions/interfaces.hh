// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_FUNCTIONS_INTERFACES_HH
#define DUNE_PYMOR_FUNCTIONS_INTERFACES_HH

#include <memory>

#include <dune/common/fmatrix.hh>
#include <dune/common/fvector.hh>

#include <dune/stuff/functions/interfaces.hh>

#include <dune/pymor/parameters/base.hh>
#include <dune/pymor/parameters/functional.hh>
#include <dune/pymor/common/exceptions.hh>

namespace Dune {
namespace Pymor {


/**
 * \brief Interface for matrix valued (possibly parametric) functions.
 * \note  See specialization (rangeDimCols = 1) for scalar and vector valued functions.
 */
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

  typedef Stuff::FunctionInterface< DomainFieldType, dimDomain,
                                    RangeFieldType, dimRangeRows, dimRangeCols > NonparametricType;

  ParametricFunctionInterface(const ParameterType tt = ParameterType());

  ParametricFunctionInterface(const Parametric& other);

  virtual ~ParametricFunctionInterface();

  static std::string static_id();

  virtual std::string name() const;

  virtual int order() const;

  virtual void evaluate(const DomainType& x, RangeType& ret, const Parameter mu = Parameter()) const = 0;

  virtual RangeType evaluate(const DomainType& x, const Parameter mu = Parameter()) const
    throw (Exception::wrong_parameter_type);

  virtual bool affinely_decomposable() const;

  virtual bool has_affine_part() const;

  virtual std::shared_ptr< const NonparametricType > affine_part() const;

  virtual unsigned int num_components() const;

  virtual std::shared_ptr< const NonparametricType > component(int qq) const;

  virtual std::shared_ptr< const ParameterFunctional > coefficient(int qq) const;
}; // class ParametricFunctionInterface


/**
 * \brief Interface for scalar and vector valued (possibly parametric) functions.
 */
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
  static const unsigned int                             dimRange = dimRangeRows;
  typedef Dune::FieldVector< RangeFieldType, dimRange > RangeType;

  typedef Stuff::FunctionInterface< DomainFieldType, dimDomain, RangeFieldType, dimRange > NonparametricType;

  ParametricFunctionInterface(const ParameterType tt = ParameterType());

  ParametricFunctionInterface(const Parametric& other);

  virtual ~ParametricFunctionInterface();

  static std::string static_id();

  virtual std::string name() const;

  virtual int order() const;

  virtual void evaluate(const DomainType& x, RangeType& ret, const Parameter mu = Parameter()) const = 0;

  virtual RangeType evaluate(const DomainType& x, const Parameter mu = Parameter()) const
    throw (Exception::wrong_parameter_type);

  virtual bool affinely_decomposable() const;

  virtual bool has_affine_part() const;

  virtual std::shared_ptr< const NonparametricType > affine_part() const;

  virtual unsigned int num_components() const;

  virtual std::shared_ptr< const NonparametricType > component(int ii) const;

  virtual std::shared_ptr< const ParameterFunctional > coefficient(int ii) const;
}; // class ParametricFunctionInterface< ..., 1 >


} // namespace Pymor
} // namespace Dune

#endif // DUNE_PYMOR_FUNCTIONS_INTERFACES_HH
