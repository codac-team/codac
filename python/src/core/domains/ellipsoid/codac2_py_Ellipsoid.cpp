/** 
 *  Codac binding (core)
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <sstream>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <codac2_Ellipsoid.h>
#include "codac2_py_Ellipsoid_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):
#include "codac2_py_AnalyticFunction.h"

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_Ellipsoid(py::module& m)
{
  py::class_<Ellipsoid> exported_ellipsoid_class(m, "Ellipsoid", ELLIPSOID_MAIN);
  exported_ellipsoid_class

    .def_readwrite("mu", &Ellipsoid::mu,
      VECTOR_ELLIPSOID_MU)
    .def_readwrite("G", &Ellipsoid::G,
      MATRIX_ELLIPSOID_G)
  
    .def(py::init<Index>(),
      ELLIPSOID_ELLIPSOID_INDEX,
      "n"_a)

    .def(py::init<const Vector&,const Matrix&>(),
      ELLIPSOID_ELLIPSOID_CONST_VECTOR_REF_CONST_MATRIX_REF,
      "mu"_a, "G"_a)
  
    .def(py::init<const Ellipsoid&>(),
      "e"_a)

    .def("size", &Ellipsoid::size,
      INDEX_ELLIPSOID_SIZE_CONST)

    .def("rand", &Ellipsoid::rand,
      VECTOR_ELLIPSOID_RAND_CONST)

    .def("hull_box", &Ellipsoid::hull_box,
      INTERVALVECTOR_ELLIPSOID_HULL_BOX_CONST)

    .def("is_concentric_subset", &Ellipsoid::is_concentric_subset,
      BOOLINTERVAL_ELLIPSOID_IS_CONCENTRIC_SUBSET_CONST_ELLIPSOID_REF_CONST,
      "e"_a)

    .def("proj_2d", &Ellipsoid::proj_2d,
      ELLIPSOID_ELLIPSOID_PROJ_2D_CONST_VECTOR_REF_CONST_VECTOR_REF_CONST_VECTOR_REF_CONST,
      "d"_a, "v"_a, "u"_a)

    .def("__repr__", [](const Ellipsoid& e) {
          std::ostringstream s;
          s << e;
          return string(s.str()); 
        },
      OSTREAM_REF_OPERATOROUT_OSTREAM_REF_CONST_ELLIPSOID_REF)

    .def(py::self + py::self,
      ELLIPSOID_OPERATORPLUS_CONST_ELLIPSOID_REF_CONST_ELLIPSOID_REF,
      py::is_operator())

  ;

  m

    .def("linear_mapping", &codac2::linear_mapping,
      ELLIPSOID_LINEAR_MAPPING_CONST_ELLIPSOID_REF_CONST_MATRIX_REF_CONST_VECTOR_REF,
      "e"_a, "A"_a, "b"_a)

    .def("unreliable_linear_mapping", &codac2::unreliable_linear_mapping,
      ELLIPSOID_UNRELIABLE_LINEAR_MAPPING_CONST_ELLIPSOID_REF_CONST_MATRIX_REF_CONST_VECTOR_REF,
      "e"_a, "A"_a, "b"_a)

    .def("nonlinear_mapping", [](const Ellipsoid& e, py::object f)
        {
          return nonlinear_mapping(e, cast<AnalyticFunction<VectorType>>(f));
        },
      ELLIPSOID_NONLINEAR_MAPPING_CONST_ELLIPSOID_REF_CONST_ANALYTICFUNCTION_VECTORTYPE_REF,
      "e"_a, "f"_a)

    .def("nonlinear_mapping", [](const Ellipsoid& e, py::object f, const Vector &trig, const Vector &q)
        {
          return nonlinear_mapping(e, cast<AnalyticFunction<VectorType>>(f), trig, q);
        },
      ELLIPSOID_NONLINEAR_MAPPING_CONST_ELLIPSOID_REF_CONST_ANALYTICFUNCTION_VECTORTYPE_REF_CONST_VECTOR_REF_CONST_VECTOR_REF,
      "e"_a, "f"_a, "trig"_a, "q"_a)

    .def("nonlinear_mapping_base", &codac2::nonlinear_mapping_base,
      MATRIX_NONLINEAR_MAPPING_BASE_CONST_MATRIX_REF_CONST_MATRIX_REF_CONST_INTERVALMATRIX_REF_CONST_VECTOR_REF_CONST_VECTOR_REF,
      "G"_a, "J"_a, "J_box"_a, "trig"_a, "q"_a)

  ;
}