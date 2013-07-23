// This file is part of the dune-pymor project:
//   https://github.com/pyMor/dune-pymor
// Copyright Holders: Felix Albrecht, Stephan Rave
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_FUNCTIONS_CHECKERBOARD_HH
#define DUNE_PYMOR_FUNCTIONS_CHECKERBOARD_HH

#include <vector>

#include <dune/stuff/functions/checkerboard.hh>
#include <dune/stuff/common/parameter/tree.hh>

#include <dune/pymor/common/exceptions.hh>
#include <dune/pymor/parameters/functional.hh>
#include <dune/pymor/functions/default.hh>

#include "interfaces.hh"

namespace Dune {
namespace Pymor {
namespace Function {


template< class DomainFieldImp, int domainDim, class RangeFieldImp >
class Checkerboard
  : public Function::AffinelyDecomposableDefault< DomainFieldImp, domainDim, RangeFieldImp, 1, 1 >
{
  typedef Function::AffinelyDecomposableDefault< DomainFieldImp, domainDim, RangeFieldImp, 1, 1 > BaseType;
public:
  typedef Checkerboard< DomainFieldImp, domainDim, RangeFieldImp > ThisType;

  typedef typename BaseType::DomainFieldType  DomainFieldType;
  static const unsigned int                   dimDomain = BaseType::dimDomain;
  typedef typename BaseType::DomainType       DomainType;
  typedef typename BaseType::RangeFieldType   RangeFieldType;
  typedef typename BaseType::RangeType        RangeType;

  static std::string static_id();

  Checkerboard(const DomainType& lowLeft,
               const DomainType& upRight,
               const std::vector< size_t >& numElements,
               const std::string parameterName = "value",
               const std::string name = static_id());

  static Dune::ParameterTree defaultSettings(const std::string subName = "");

  static ThisType* create(const DSC::ExtendedParameterTree settings = defaultSettings())
  throw (Exception::this_does_not_make_any_sense);
}; // class Checkerboard


} // namespace Function
} // namespace Pymor
} // namespace Dune

#endif // DUNE_PYMOR_FUNCTIONS_CHECKERBOARD_HH
