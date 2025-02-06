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
#include <codac2_CtcProj.h>
#include "codac2_py_Ctc.h"
#include "codac2_py_CtcProj_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_CtcProj(py::module& m, py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector>& pyctc)
{
  py::class_<CtcProj> exported(m, "CtcProj", pyctc, CTCPROJ_MAIN);
  exported

    .def(py::init(
        [](const CtcBase<IntervalVector>& c, std::vector<Index_type> proj_indices, double default_eps)
        {
          std::transform(std::begin(proj_indices),std::end(proj_indices),std::begin(proj_indices),
            [](Index_type x)
            {
              matlab::test_integer(x);
              return matlab::input_index(x);
            });

          return std::make_unique<CtcProj>(c.copy(), proj_indices, default_eps);
        }),
      CTCPROJ_CTCPROJ_CONST_C_REF_CONST_VECTOR_INDEX_REF_DOUBLE,
      "c"_a, "proj_indices"_a, "default_eps"_a=0.01)

    .def(py::init(
        [](const CtcBase<IntervalVector>& c, std::vector<Index_type> proj_indices, const IntervalVector& y, double default_eps)
        {
          std::transform(std::begin(proj_indices),std::end(proj_indices),std::begin(proj_indices),
            [](Index_type x)
            {
              matlab::test_integer(x);
              return matlab::input_index(x);
            });

          return std::make_unique<CtcProj>(c.copy(), proj_indices, y, default_eps);
        }),
      CTCPROJ_CTCPROJ_CONST_C_REF_CONST_VECTOR_INDEX_REF_CONST_INTERVALVECTOR_REF_DOUBLE,
      "c"_a, "proj_indices"_a, "y"_a, "default_eps"_a=0.01)

    .def("contract", (void(CtcProj::*)(IntervalVector&,double) const) &CtcProj::contract,
      VOID_CTCPROJ_CONTRACT_INTERVALVECTOR_REF_DOUBLE_CONST,
      "x"_a, "eps"_a)

    .def(CONTRACT_BOX_METHOD(CtcProj,
      VOID_CTCPROJ_CONTRACT_INTERVALVECTOR_REF_CONST))
  ;
}