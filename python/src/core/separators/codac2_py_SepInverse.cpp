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
#include <codac2_AnalyticFunction.h>
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

void export_SepInverse(py::module& m, py::class_<SepBase,pySep>& pysep)
{
  py::class_<SepInverse> exported(m, "SepInverse", pysep, SEPINVERSE_MAIN);
  exported

    .def(py::init(
        [](const py::object& f, const SepBase& s, bool with_centered_form)
        {
          assert_release(is_instance<AnalyticFunction<VectorType>>(f));
          return std::make_unique<SepInverse>(
            cast<AnalyticFunction<VectorType>>(f),
            s.copy(), with_centered_form);
        }
      ),
      SEPINVERSE_SEPINVERSE_CONST_ANALYTICFUNCTION_T_REF_CONST_S_REF_BOOL,
      "f"_a, "s"_a, "with_centered_form"_a = true)

    .def(py::init(
        [](const py::object& f, const py::object& y, bool with_centered_form)
        {
          if(is_instance<AnalyticFunction<ScalarType>>(f))
          {
            return std::make_unique<SepInverse>(
              cast<AnalyticFunction<ScalarType>>(f),
              cast<Interval>(y), with_centered_form);
          }

          else if(is_instance<AnalyticFunction<VectorType>>(f))
          {
            return std::make_unique<SepInverse>(
              cast<AnalyticFunction<VectorType>>(f),
              cast<IntervalVector>(y), with_centered_form);
          }

          // not yet available else if(is_instance<AnalyticFunction<MatrixType>>(f))
          // not yet available {
          // not yet available   return std::make_unique<SepInverse>(
          // not yet available     cast<AnalyticFunction<MatrixType>>(f),
          // not yet available     cast<IntervalMatrix>(y), with_centered_form);
          // not yet available }

          else
          {
            assert_release_constexpr(false && "case error - SepInverse: cannot deal with f");
            return std::unique_ptr<SepInverse>(nullptr);
          }
        }
      ),
      SEPINVERSE_SEPINVERSE_CONST_ANALYTICFUNCTION_T_REF_CONST_TYPENAME_T_DOMAIN_REF_BOOL,
      "f"_a, "y"_a, "with_centered_form"_a = true)

    .def("separate", &SepInverse::separate,
      BOXPAIR_SEPCTCPAIR_SEPARATE_CONST_INTERVALVECTOR_REF_CONST,
      "x"_a)
  ;
}