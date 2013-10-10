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
 */
template< class EntityImp, class DomainFieldImp, int domainDim, class RangeFieldImp, int rangeDim, int rangeDimCols = 1 >
class ParametricFunctionInterface
  : public Parametric
{
public:
  typedef EntityImp EntityType;

  typedef DomainFieldImp                                  DomainFieldType;
  static const unsigned int                               dimDomain = domainDim;

  typedef RangeFieldImp                                                   RangeFieldType;
  static const unsigned int                                               dimRange = rangeDim;
  static const unsigned int                                               dimRangeCols = rangeDimCols;

  typedef Stuff::LocalizableFunctionInterface
      < EntityType, DomainFieldType, dimDomain, RangeFieldType, dimRange, dimRangeCols > NonparametricType;

  typedef typename NonparametricType::DomainType        DomainType;
  typedef typename NonparametricType::RangeType         RangeType;
  typedef typename NonparametricType::JacobianRangeType JacobianRangeType;

  ParametricFunctionInterface(const ParameterType tt = ParameterType());

  ParametricFunctionInterface(const Parametric& other);

  virtual ~ParametricFunctionInterface();

  static std::string static_id();

  virtual std::string name() const;

  virtual bool affinely_decomposable() const;

  virtual bool has_affine_part() const;

  virtual std::shared_ptr< const NonparametricType > affine_part() const;

  virtual unsigned int num_components() const;

  virtual std::shared_ptr< const NonparametricType > component(int qq) const;

  virtual std::shared_ptr< const ParameterFunctional > coefficient(int qq) const;
}; // class ParametricFunctionInterface


} // namespace Pymor
} // namespace Dune

#include "interfaces.cc"

#endif // DUNE_PYMOR_FUNCTIONS_INTERFACES_HH
