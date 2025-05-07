/** 
 *  Approx Python binding
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <limits>
#include <sstream>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <codac2_transformations.h>
#include "codac2_py_transformations_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):
#include "codac2_py_doc.h"

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_transformations(py::module& m)
{
  py::class_<Eigen::Affine2d> exported_affine2d(m, "Affine2d", DOC_TO_BE_DEFINED);
  exported_affine2d

    .def("linear", [](const Eigen::Affine2d& tr) { return Matrix(tr.linear()); },
      DOC_TO_BE_DEFINED)

    .def("translation", [](const Eigen::Affine2d& tr) { return Vector(tr.translation()); },
      DOC_TO_BE_DEFINED)

    .def("__mul__", [](const Eigen::Affine2d& x1, const Vector& x2)
      {
        assert_release(x2.size() == 2);
        return Vector(x1*Eigen::Vector2d(x2));
      },
      DOC_TO_BE_DEFINED,
      py::is_operator())

  ;
  
  m

    .def("affine_transformation", (Eigen::Affine2d (*)(const std::vector<Vector>&,const std::vector<Vector>&))&codac2::affine_transformation,
      EIGEN_AFFINE2D_AFFINE_TRANSFORMATION_CONST_VECTOR_VECTOR_REF_CONST_VECTOR_VECTOR_REF,
      "src"_a, "dst"_a)

    .def("affine_transformation", (Eigen::Affine2d (*)(const SampledTraj<Vector>&,const SampledTraj<Vector>&))&codac2::affine_transformation,
      EIGEN_AFFINE2D_AFFINE_TRANSFORMATION_CONST_SAMPLEDTRAJ_VECTOR_REF_CONST_SAMPLEDTRAJ_VECTOR_REF,
      "src"_a, "dst"_a)

  ;
}