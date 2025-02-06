/** 
 *  Codac binding (core)
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <codac2_SepInverse.h>
#include "codac2_Sep.h"
#include "codac2_py_Sep.h"
#include "codac2_py_SepInverse_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):
#include "codac2_py_SepCtcPair_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):
#include "codac2_py_AnalyticFunction.h"
#include "codac2_py_cast.h"

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

template<typename T>
void export_SepInverse_type(py::class_<SepInverse>& exported)
{
  exported
    .def(py::init(
        [](const py::object& f, const typename T::Domain& y, bool with_centered_form)
        {
          return std::make_unique<SepInverse>(
            cast<AnalyticFunction<T>>(f),
            y, with_centered_form);
        }
      ),
      SEPINVERSE_SEPINVERSE_CONST_ANALYTICFUNCTION_T_REF_CONST_TYPENAME_T_DOMAIN_REF_BOOL,
      "f"_a, "y"_a, "with_centered_form"_a = true);

  if constexpr(std::is_same_v<T,IntervalVector>) // separators only associated with interval vectors
  {
    exported
    .def(py::init(
        [](const py::object& f, const pySep& s, bool with_centered_form)
        {
          return std::make_unique<SepInverse>(
            cast<AnalyticFunction<T>>(f),
            s, with_centered_form);
        }
      ),
      SEPINVERSE_SEPINVERSE_CONST_ANALYTICFUNCTION_T_REF_CONST_S_REF_BOOL,
      "f"_a, "s"_a, "with_centered_form"_a = true);
  }
}

void export_SepInverse(py::module& m, py::class_<SepBase,pySep>& pysep)
{
  py::class_<SepInverse> exported(m, "SepInverse", pysep, SEPINVERSE_MAIN);

  export_SepInverse_type<ScalarType>(exported);
  export_SepInverse_type<VectorType>(exported);

  exported

    .def("separate", &SepInverse::separate,
      BOXPAIR_SEPCTCPAIR_SEPARATE_CONST_INTERVALVECTOR_REF_CONST,
      "x"_a)
  ;
}