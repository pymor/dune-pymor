#include "checkerboard.hh"

namespace Dune {
namespace Pymor {
namespace Function {


template< class D, int d, class R >
std::string Checkerboard< D, d, R >::static_id()
{
  return ParametricFunctionInterface< DomainFieldType, dimDomain,
                                      RangeFieldType, 1, 1 >::static_id() + ".checkerboard";
}

template< class D, int d, class R >
Checkerboard< D, d, R >::Checkerboard(const DomainType& lowLeft,
                                      const DomainType& upRight,
                                      const std::vector< size_t >& numElements,
                                      const std::string parameterName,
                                      const std::string name)
  : BaseType(name, 0)
{
  // check input
  if (numElements.size() < dimDomain)
    DUNE_PYMOR_THROW(Exception::sizes_do_not_match,
                     "numElements has to be at least of size " << dimDomain << " is(" << numElements.size()
                     << ")!");
  size_t parameterSize = 1;
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
  const ParameterType parameterType(parameterName, parameterSize);
  // create the coefficients and components
  typedef Dune::Stuff::FunctionCheckerboard< DomainFieldType, dimDomain, RangeFieldType, 1 > NonparametricType;
  for (size_t ii = 0; ii < parameterSize; ++ii) {
    std::vector< RangeType > indicator(parameterSize, RangeType(0));
    indicator[ii] = RangeType(1);
    BaseType::register_component(new NonparametricType(lowLeft, upRight, numElements, indicator),
                                 new ParameterFunctional(parameterType,
                                                         parameterName + "[" + DSC::toString(ii) + "]"));
  } // create the coefficients and components
} // Checkerboard()

template< class D, int d, class R >
Dune::ParameterTree Checkerboard< D, d, R >::defaultSettings(const std::string subName)
{
  Dune::ParameterTree description;
  description["lowerLeft"] = "[0.0; 0.0; 0.0]";
  description["upperRight"] = "[1.0; 1.0; 1.0]";
  description["numElements"] = "[2; 2; 2]";
  description["parameterName"] = "value";
  description["name"] = static_id();
  if (subName.empty())
    return description;
  else {
    DSC::ExtendedParameterTree extendedDescription;
    extendedDescription.add(description, subName);
    return extendedDescription;
  }
} // ... defaultSettings(...)

template< class D, int d, class R >
typename Checkerboard< D, d, R >::ThisType* Checkerboard< D, d, R >::create(const DSC::ExtendedParameterTree settings)
throw (Exception::this_does_not_make_any_sense)
{
  // get data
  const std::string nm = settings.get< std::string >("name", static_id());
  const std::string paramName = settings.get< std::string >("parameterName", "value");
  const std::vector< DomainFieldType > lowerLefts = settings.getVector("lowerLeft", DomainFieldType(0), dimDomain);
  const std::vector< DomainFieldType > upperRights = settings.getVector("upperRight",
                                                                           DomainFieldType(1),
                                                                           dimDomain);
  const std::vector< size_t > numElements = settings.getVector("numElements", size_t(1), dimDomain);
  // get paramSize
  size_t paramSize = 1u;
  for (size_t dd = 0; dd < dimDomain; ++dd) {
    if (numElements[dd] <= 0)
      DUNE_PYMOR_THROW(Exception::this_does_not_make_any_sense,
                       "numElements[" << dd << "] has to be posititve (is " << numElements[dd] << ")!");
    paramSize *= numElements[dd];
  }
  // convert and leave the checks to the constructor
  DomainType lowerLeft;
  DomainType upperRight;
  for (size_t dd = 0; dd < dimDomain; ++dd) {
    lowerLeft[dd] = lowerLefts[dd];
    upperRight[dd] = upperRights[dd];
  }
  // create and return
  return new ThisType(lowerLeft, upperRight, numElements, paramName, nm);
} // ... create(...)

template class Checkerboard< double, 1, double >;
template class Checkerboard< double, 2, double >;
template class Checkerboard< double, 3, double >;


} // namespace Function
} // namespace Pymor
} // namespace Dune
