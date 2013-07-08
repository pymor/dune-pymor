// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_FUNCTIONS_CHECKERBOARD_HH
#define DUNE_PYMOR_FUNCTIONS_CHECKERBOARD_HH

#include <vector>

#include <dune/stuff/function/checkerboard.hh>

#include <dune/pymor/common/exceptions.hh>
#include <dune/pymor/parameters/functional.hh>
#include <dune/pymor/functions/default.hh>

#include "interfaces.hh"

namespace Dune {
namespace Pymor {
namespace Functions {


// forward, to allow for specialization
template< class DomainFieldImp, int domainDim, class RangeFieldImp, int rangeDimRows, int rangeDimCols = 1 >
class Checkerboard;


template< class DomainFieldImp, int domainDim, class RangeFieldImp >
class Checkerboard< DomainFieldImp, domainDim, RangeFieldImp, 1, 1 >
  : public Functions::AffineParametricDefault< DomainFieldImp, domainDim, RangeFieldImp, 1, 1 >
{
  typedef Functions::AffineParametricDefault< DomainFieldImp, domainDim, RangeFieldImp, 1, 1 > BaseType;
public:
  typedef Checkerboard< DomainFieldImp, domainDim, RangeFieldImp, 1, 1 > ThisType;

  typedef typename BaseType::DomainFieldType  DomainFieldType;
  static const int                            dimDomain = BaseType::dimDomain;
  typedef typename BaseType::DomainType       DomainType;
  typedef typename BaseType::RangeFieldType   RangeFieldType;
  typedef typename BaseType::RangeType        RangeType;

  static const std::string static_id()
  {
    return "affineparametricfunction.checkerboard";
  }

  virtual std::string id() const
  {
    return static_id();
  }

  Checkerboard(const DomainType& lowLeft,
               const DomainType& upRight,
               const std::vector< size_t >& numElements,
               const std::string parameterName = static_id())
    : Parametric()
  {
    // check input
    if (numElements.size() < dimDomain)
      DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                       "numElements has to be at least of size " << dimDomain << " is(" << numElements.size()
                       << ")!");
    int parameterSize = 1;
    for (size_t dd = 0; dd < dimDomain; ++dd) {
      if (lowLeft[dd] >= upRight[dd])
        DUNE_PYMOR_THROW(Exception::this_does_not_make_any_sense,
                         "lowLeft[" << dd << "] = " << lowLeft[dd] << " has to be smaller than upRight["
                         << dd << "]!");
      if (numElements[dd] <= 0)
        DUNE_PYMOR_THROW(Exception::index_out_of_range,
                         "numElements[" << dd << "] has to be positive (is " << numElements[dd] << ")!");
      parameterSize *= numElements[dd];
    }
    if (parameterName.empty())
      DUNE_PYMOR_THROW(Exception::wrong_input, "parameterName must not be empty!");
    // build parameter
    Parametric::parameter_type().set(parameterName, parameterSize);
    // create the coefficients and components
    typedef Dune::Stuff::FunctionCheckerboard< DomainFieldType, dimDomain, RangeFieldType, 1 > NonparametricType;
    for (size_t ii = 0; ii < parameterSize; ++ii) {
      std::vector< RangeType > indicator(parameterSize, RangeType(0));
      indicator[ii] = RangeType(1);
      BaseType::register_component();
      components_.emplace_back(new NonparametricType(_lowerLeft, _upperRight, _numElements, indicator));
      coefficients_.emplace_back(new CoefficientType("mu[" + DSC::toString(ii) + "]"));
    } // create the coefficients and components
    parameterExplanation_ = std::vector< std::string >(paramSize_, "");
    // create the explanations
    for (size_t ii = 0; ii < _numElements[0]; ++ii) {
      if (dimDomain == 1)
        parameterExplanation_[ii] = "value_in_subdomain_" + DSC::toString(ii);
      else
        for (unsigned int jj = 0; jj < _numElements[1]; ++jj) {
          if (dimDomain == 2)
            parameterExplanation_[ii + jj*_numElements[0]] = "value_in_subdomain_"
                + DSC::toString(ii)
                + "_" + DSC::toString(jj);
          else
            for (unsigned int kk = 0; kk < _numElements[2]; ++kk) {
              parameterExplanation_[ii + jj*_numElements[0] + kk*_numElements[0]*_numElements[1]] = "value_in_subdomain_"
                  + DSC::toString(ii)
                  + "_" + DSC::toString(jj)
                  + "_" + DSC::toString(kk);
            }
        }
    }
  } // AffineParametricFunctionCheckerboard(...)

//  static Dune::ParameterTree defaultSettings(const std::string subName = "")
//  {
//    Dune::ParameterTree description;
//    description["lowerLeft"] = "[0.0; 0.0; 0.0]";
//    description["upperRight"] = "[1.0; 1.0; 1.0]";
//    description["numElements"] = "[2; 2; 2]";
//    description["paramMin"] = "[1.0; 2.0; 3.0; 4.0; 5.0; 6.0; 7.0; 8.0]";
//    description["paramMax"] = "[10.0; 20.0; 30.0; 40.0; 50.0; 60.0; 70.0; 80.0]";
//    description["name"] = id();
//    if (subName.empty())
//      return description;
//    else {
//      DSC::ExtendedParameterTree extendedDescription;
//      extendedDescription.add(description, subName);
//      return extendedDescription;
//    }
//  } // ... defaultSettings(...)

//  static ThisType* create(const DSC::ExtendedParameterTree settings)
//  {
//    // get data
//    const std::string _name = settings.get< std::string >("name", id());
//    const std::vector< DomainFieldType > lowerLefts = settings.getVector("lowerLeft", DomainFieldType(0), dimDomain);
//    const std::vector< DomainFieldType > upperRights = settings.getVector("upperRight",
//                                                                             DomainFieldType(1),
//                                                                             dimDomain);
//    const std::vector< size_t > numElements = settings.getVector("numElements", size_t(1), dimDomain);
//    // get paramSize
//    size_t paramSize = 1u;
//    for (int dd = 0; dd < dimDomain; ++dd) {
//      assert(numElements[dd] > 0 && "Please provide positive number of elements per dim!");
//      paramSize *= numElements[dd];
//    }
//    const std::vector< ParamFieldType > paramMins = settings.getVector("paramMin",
//                                                                          ParamFieldType(1),
//                                                                          paramSize);
//    const std::vector< ParamFieldType > paramMaxs = settings.getVector("paramMax",
//                                                                          ParamFieldType(1),
//                                                                          paramSize);
//    // convert and leave the checks to the constructor
//    DomainType lowerLeft;
//    DomainType upperRight;
//    for (int dd = 0; dd < dimDomain; ++dd) {
//      lowerLeft[dd] = lowerLefts[dd];
//      upperRight[dd] = upperRights[dd];
//    }
//    ParamType paramMin(paramSize);
//    ParamType paramMax(paramSize);
//    for (size_t qq = 0; qq < paramSize; ++qq) {
//      paramMin[qq] = paramMins[qq];
//      paramMax[qq] = paramMaxs[qq];
//    }
//    const std::vector< ParamType > paramRange = {paramMin, paramMax};
//    // create and return
//    return new ThisType(lowerLeft, upperRight, numElements, paramRange, _name);
//  } // ... create(...)

  virtual int order() const
  {
    return 0;
  }

//  virtual std::string name() const
//  {
//    return name_;
//  }

//  virtual void evaluate(const DomainType& _x, const ParamType& _mu, RangeType& _ret) const
//  {
//    assert(_mu.size() == paramSize());
//    _ret = RangeFieldType(0);
//    RangeType tmpComponentValue;
//    for (unsigned int qq = 0; qq < paramSize_; ++qq) {
//      components_[qq]->evaluate(_x, tmpComponentValue);
//      tmpComponentValue *= coefficients_[qq]->evaluate(_mu);
//      _ret += tmpComponentValue;
//    }
//  } // virtual void evaluate(const DomainType& x, const ParamType& mu, RangeType& ret) const

private:
//  unsigned int paramSize_;
//  std::vector< ParamType > paramRange_;
//  std::string name_;
//  std::vector< std::string > parameterExplanation_;
//  std::vector< std::shared_ptr< const ComponentType > > components_;
//  std::vector< std::shared_ptr< const CoefficientType > > coefficients_;
}; // class Checkerboard


} // namespace Functions
} // namespace Pymor
} // namespace Dune

#endif // DUNE_PYMOR_FUNCTIONS_CHECKERBOARD_HH
