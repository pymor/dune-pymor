// This file is part of the dune-pymor project:
//   https://github.com/pymor/dune-pymor
// Copyright holders: Stephan Rave, Felix Schindler
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_PYMOR_FUNCTIONS_INTERFACES_HH
#define DUNE_PYMOR_FUNCTIONS_INTERFACES_HH

#include <memory>
#include <ostream>

#include <dune/common/fmatrix.hh>
#include <dune/common/fvector.hh>

#include <dune/stuff/functions/interfaces.hh>
#include <dune/stuff/common/string.hh>

#include <dune/pymor/parameters/base.hh>
#include <dune/pymor/parameters/functional.hh>
#include <dune/pymor/common/exceptions.hh>

namespace Dune {
namespace Pymor {


/**
 * \brief Interface for global valued (possibly parametric) functions.
 */
template< class EntityImp, class DomainFieldImp, int domainDim, class RangeFieldImp, int rangeDim, int rangeDimCols = 1 >
class AffinelyDecomposableFunctionInterface
  : public Parametric
{
  typedef AffinelyDecomposableFunctionInterface
      < EntityImp, DomainFieldImp, domainDim, RangeFieldImp, rangeDim, rangeDimCols > ThisType;
public:
  typedef Stuff::LocalizableFunctionInterface
      < EntityImp, DomainFieldImp, domainDim, RangeFieldImp, rangeDim, rangeDimCols > NonparametricType;
  typedef typename NonparametricType::EntityType EntityType;

  typedef typename NonparametricType::DomainFieldType DomainFieldType;
  static const unsigned int                           dimDomain = NonparametricType::dimDomain;
  typedef typename NonparametricType::DomainType      DomainType;

  typedef typename NonparametricType::RangeFieldType  RangeFieldType;
  static const unsigned int                           dimRange = NonparametricType::dimRange;
  static const unsigned int                           dimRangeCols = NonparametricType::dimRangeCols;
  typedef typename NonparametricType::RangeType       RangeType;

  typedef typename NonparametricType::JacobianRangeType JacobianRangeType;

  AffinelyDecomposableFunctionInterface(const ParameterType tt = ParameterType())
    : Parametric(tt)
  {}

  AffinelyDecomposableFunctionInterface(const Parametric& other)
    : Parametric(other)
  {}

  virtual ~AffinelyDecomposableFunctionInterface() {}

  static std::string static_id()
  {
    return "pymor.function";
  }

  virtual std::string type() const
  {
    return "pymor.function";
  }

  virtual std::string name() const
  {
    return "pymor.function";
  }

  /**
   * \note  This will make sense in the future, once we have parametric functions which are not affinely decomposable.
   */
  virtual bool affinely_decomposable() const
  {
    return true;
  }

  virtual bool has_affine_part() const
  {
    return !parametric();
  }

  virtual const std::shared_ptr< const NonparametricType >& affine_part() const
  {
    if (has_affine_part())
      DUNE_PYMOR_THROW(Exception::you_have_to_implement_this,
                       "since has_affine_part() == true, you really do!");
    else
      DUNE_PYMOR_THROW(Exception::requirements_not_met,
                       "Do not call affine_part() if has_affine_part() == false!");
    return nullptr;
  } // ... affine_part(...)

  virtual DUNE_STUFF_SSIZE_T num_components() const
  {
    if (!has_affine_part())
      return 0;
    else
      DUNE_PYMOR_THROW(Exception::you_have_to_implement_this,
                       "since affinely_decomposable() == true and has_affine_part() == false, you really do!");
    return false;
  } // ... num_components(...)

  virtual const std::shared_ptr< const NonparametricType >& component(const DUNE_STUFF_SSIZE_T qq) const
  {
    if (!parametric())
      DUNE_PYMOR_THROW(Exception::this_is_not_parametric,
                       "Do not call component(" << qq << ") if parametric() == false!");
    if (num_components() > 0)
      DUNE_PYMOR_THROW(Exception::you_have_to_implement_this,
                       "since num_components() (= " << num_components() << ") > 0, you really do!");
    else
      DUNE_PYMOR_THROW(Exception::requirements_not_met,
                       "Do not call component(" << qq << ") if num_components() == 0!");
    return nullptr;
  } // ... component(...)

  virtual const std::shared_ptr< const ParameterFunctional >& coefficient(const DUNE_STUFF_SSIZE_T qq) const
  {
    if (!parametric())
      DUNE_PYMOR_THROW(Exception::this_is_not_parametric,
                       "Do not call coefficient(" << qq << ") if parametric() == false!");
    if (num_components() > 0)
      DUNE_PYMOR_THROW(Exception::you_have_to_implement_this,
                       "since num_coefficients() (= " << num_components() << ") > 0, you really do!");
    else
      DUNE_PYMOR_THROW(Exception::requirements_not_met,
                       "Do not call coefficient(" << qq << ") if num_coefficients() == 0!");
    return nullptr;
  } // ... coefficient(...)

  virtual void report(std::ostream& out, const std::string prefix = "") const
  {
    out << prefix << "affinely decomposable function '" << name() << "' (of type " << type() << "):";
    if (parametric())
      out << "\n" << prefix << "  parameter_type: " << parameter_type();
    if (has_affine_part()) {
      out << "\n";
      affine_part()->report(out, prefix + "  affine_part: ");
    }
    for (size_t qq = 0; qq < num_components(); ++qq) {
      out << "\n";
      component(qq)->report(out, prefix + "  component " + Stuff::Common::toString(qq) + ": ");
      out << "\n" << prefix << "  coefficient " << qq << ": " << coefficient(qq)->expression();
    }
  } // ... report(...)

private:
  template< class T >
  friend std::ostream& operator<<(std::ostream& /*out*/, const ThisType& /*function*/);
}; // class AffinelyDecomposableFunctionInterface


template< class E, class D, int d, class R, int r, int rC >
std::ostream& operator<<(std::ostream& out,
                         const AffinelyDecomposableFunctionInterface< E, D, d, R, r, rC >& function)
{
  function.report(out);
  return out;
} // ... operator<<(...)


} // namespace Pymor
} // namespace Dune

#ifdef DUNE_PYMOR_FUNCTIONS_TO_LIB
// we use this macro so we can add ParametricFunctionInterface at some point
#define DUNE_PYMOR_FUNCTION_INTERFACES_LIST_CLASSES(etype, ddim) \
  DUNE_PYMOR_FUNCTION_INTERFACES_LIST_DIMRANGE(Dune::Pymor::AffinelyDecomposableFunctionInterface, etype, ddim)

#define DUNE_PYMOR_FUNCTION_INTERFACES_LIST_DIMRANGE(cname, etype, ddim) \
  DUNE_PYMOR_FUNCTION_INTERFACES_LIST_DIMRANGECOLS(cname, etype, ddim, 1) \
  DUNE_PYMOR_FUNCTION_INTERFACES_LIST_DIMRANGECOLS(cname, etype, ddim, 2) \
  DUNE_PYMOR_FUNCTION_INTERFACES_LIST_DIMRANGECOLS(cname, etype, ddim, 3)

#define DUNE_PYMOR_FUNCTION_INTERFACES_LIST_DIMRANGECOLS(cname, etype, ddim, rdim) \
  DUNE_PYMOR_FUNCTION_INTERFACES_LIST_DOMAINFIELDTYPES(cname, etype, ddim, rdim, 1) \
  DUNE_PYMOR_FUNCTION_INTERFACES_LIST_DOMAINFIELDTYPES(cname, etype, ddim, rdim, 2) \
  DUNE_PYMOR_FUNCTION_INTERFACES_LIST_DOMAINFIELDTYPES(cname, etype, ddim, rdim, 3)

#define DUNE_PYMOR_FUNCTION_INTERFACES_LIST_DOMAINFIELDTYPES(cname, etype, ddim, rdim, rcdim) \
  DUNE_PYMOR_FUNCTION_INTERFACES_LIST_RANGEFIELDTYPES(cname, etype, double, ddim, rdim, rcdim)

#define DUNE_PYMOR_FUNCTION_INTERFACES_LIST_RANGEFIELDTYPES(cname, etype, dftype, ddim, rdim, rcdim) \
  DUNE_PYMOR_FUNCTION_INTERFACES_LAST_EXPANSION(cname, etype, dftype, ddim, double, rdim, rcdim) \
  DUNE_PYMOR_FUNCTION_INTERFACES_LAST_EXPANSION(cname, etype, dftype, ddim, long double, rdim, rcdim)

#define DUNE_PYMOR_FUNCTION_INTERFACES_LAST_EXPANSION(cname, etype, dftype, ddim, rftype, rdim, rcdim) \
  extern template class cname< etype, dftype, ddim, rftype, rdim, rcdim >;

#include <dune/stuff/grid/fakeentity.hh>

typedef Dune::Stuff::Grid::FakeEntity< 1 > DunePymorFunctionsInterfacesStuffFakeGrid1dEntityType;
typedef Dune::Stuff::Grid::FakeEntity< 2 > DunePymorFunctionsInterfacesStuffFakeGrid2dEntityType;
typedef Dune::Stuff::Grid::FakeEntity< 3 > DunePymorFunctionsInterfacesStuffFakeGrid3dEntityType;

DUNE_PYMOR_FUNCTION_INTERFACES_LIST_CLASSES(DunePymorFunctionsInterfacesStuffFakeGrid1dEntityType, 1)
DUNE_PYMOR_FUNCTION_INTERFACES_LIST_CLASSES(DunePymorFunctionsInterfacesStuffFakeGrid2dEntityType, 2)
DUNE_PYMOR_FUNCTION_INTERFACES_LIST_CLASSES(DunePymorFunctionsInterfacesStuffFakeGrid3dEntityType, 3)

#ifdef HAVE_DUNE_GRID

# include <dune/grid/sgrid.hh>

typedef typename Dune::SGrid< 1, 1 >::template Codim< 0 >::Entity DunePymorFunctionsInterfacesSGrid1dEntityType;
typedef typename Dune::SGrid< 2, 2 >::template Codim< 0 >::Entity DunePymorFunctionsInterfacesSGrid2dEntityType;
typedef typename Dune::SGrid< 3, 3 >::template Codim< 0 >::Entity DunePymorFunctionsInterfacesSGrid3dEntityType;

DUNE_PYMOR_FUNCTION_INTERFACES_LIST_CLASSES(DunePymorFunctionsInterfacesSGrid1dEntityType, 1)
DUNE_PYMOR_FUNCTION_INTERFACES_LIST_CLASSES(DunePymorFunctionsInterfacesSGrid2dEntityType, 2)
DUNE_PYMOR_FUNCTION_INTERFACES_LIST_CLASSES(DunePymorFunctionsInterfacesSGrid3dEntityType, 3)

# include <dune/grid/yaspgrid.hh>

typedef typename Dune::YaspGrid< 1 >::template Codim< 0 >::Entity DunePymorFunctionsInterfacesYaspGrid1dEntityType;
typedef typename Dune::YaspGrid< 2 >::template Codim< 0 >::Entity DunePymorFunctionsInterfacesYaspGrid2dEntityType;
typedef typename Dune::YaspGrid< 3 >::template Codim< 0 >::Entity DunePymorFunctionsInterfacesYaspGrid3dEntityType;

DUNE_PYMOR_FUNCTION_INTERFACES_LIST_CLASSES(DunePymorFunctionsInterfacesYaspGrid1dEntityType, 1)
DUNE_PYMOR_FUNCTION_INTERFACES_LIST_CLASSES(DunePymorFunctionsInterfacesYaspGrid2dEntityType, 2)
DUNE_PYMOR_FUNCTION_INTERFACES_LIST_CLASSES(DunePymorFunctionsInterfacesYaspGrid3dEntityType, 3)

# if HAVE_ALUGRID_SERIAL_H || HAVE_ALUGRID_PARALLEL_H
#   ifdef ALUGRID_CONFORM
#     define DUNE_PYMOR_FUNCTIONS_INTERFACE_ALUGRID_CONFORM_WAS_DEFINED_BEFORE
#   else
#     define ALUGRID_CONFORM 1
#   endif
#   ifdef ENABLE_ALUGRID
#     define DUNE_PYMOR_FUNCTIONS_INTERFACE_ENABLE_ALUGRID_WAS_DEFINED_BEFORE
#   else
#     define ENABLE_ALUGRID 1
#   endif

#   include <dune/grid/alugrid.hh>

typedef typename Dune::ALUSimplexGrid< 2, 2 >::template Codim< 0 >::Entity DunePymorFunctionsInterfacesAluSimplexGrid2dEntityType;
typedef typename Dune::ALUSimplexGrid< 3, 3 >::template Codim< 0 >::Entity DunePymorFunctionsInterfacesAluSimplexGrid3dEntityType;
typedef typename Dune::ALUCubeGrid< 3, 3 >::template Codim< 0 >::Entity DunePymorFunctionsInterfacesAluCubeGrid3dEntityType;

DUNE_PYMOR_FUNCTION_INTERFACES_LIST_CLASSES(DunePymorFunctionsInterfacesAluSimplexGrid2dEntityType, 2)
DUNE_PYMOR_FUNCTION_INTERFACES_LIST_CLASSES(DunePymorFunctionsInterfacesAluSimplexGrid3dEntityType, 3)
DUNE_PYMOR_FUNCTION_INTERFACES_LIST_CLASSES(DunePymorFunctionsInterfacesAluCubeGrid3dEntityType, 3)

#   ifdef DUNE_PYMOR_FUNCTIONS_INTERFACE_ALUGRID_CONFORM_WAS_DEFINED_BEFORE
#     undef DUNE_PYMOR_FUNCTIONS_INTERFACE_ALUGRID_CONFORM_WAS_DEFINED_BEFORE
#   else
#     undef ALUGRID_CONFORM
#   endif
#   ifdef DUNE_PYMOR_FUNCTIONS_INTERFACE_ENABLE_ALUGRID_WAS_DEFINED_BEFORE
#     undef DUNE_PYMOR_FUNCTIONS_INTERFACE_ENABLE_ALUGRID_WAS_DEFINED_BEFORE
#   else
#     undef ENABLE_ALUGRID
#   endif
# endif // HAVE_ALUGRID_SERIAL_H || HAVE_ALUGRID_PARALLEL_H
#endif // HAVE_DUNE_GRID

#undef DUNE_PYMOR_FUNCTION_INTERFACES_LAST_EXPANSION
#undef DUNE_PYMOR_FUNCTION_INTERFACES_LIST_RANGEFIELDTYPES
#undef DUNE_PYMOR_FUNCTION_INTERFACES_LIST_DOMAINFIELDTYPES
#undef DUNE_PYMOR_FUNCTION_INTERFACES_LIST_DIMRANGECOLS
#undef DUNE_PYMOR_FUNCTION_INTERFACES_LIST_DIMRANGE
#undef DUNE_PYMOR_FUNCTION_INTERFACES_LIST_CLASSES
#endif // DUNE_PYMOR_FUNCTIONS_TO_LIB

#endif // DUNE_PYMOR_FUNCTIONS_INTERFACES_HH
