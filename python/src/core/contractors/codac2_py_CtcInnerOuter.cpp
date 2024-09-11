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
#include <codac2_CtcInnerOuter.h>
#include "codac2_py_Ctc.h"
#include "codac2_py_CtcInnerOuter_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_CtcInnerOuter(py::module& m, py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector>& pyctc)
{
  py::class_<CtcInner> exported_inner(m, "CtcInner", pyctc, CTCINNER_MAIN);
  exported_inner

    .def(py::init(
        [](const SepBase& s)
        {
          return std::make_unique<CtcInner>(s.copy());
        }),
      CTCINNER_CTCINNER_CONST_S_REF,
      "s"_a)

    .def(CONTRACT_BOX_METHOD(CtcInner,
      VOID_CTCINNER_CONTRACT_INTERVALVECTOR_REF_CONST))

  ;

  py::class_<CtcOuter> exported_outer(m, "CtcOuter", pyctc, CTCOUTER_MAIN);
  exported_outer

    .def(py::init(
        [](const SepBase& s)
        {
          return std::make_unique<CtcOuter>(s.copy());
        }),
      CTCOUTER_CTCOUTER_CONST_S_REF,
      "s"_a)

    .def(CONTRACT_BOX_METHOD(CtcOuter,
      VOID_CTCOUTER_CONTRACT_INTERVALVECTOR_REF_CONST))

  ;
}