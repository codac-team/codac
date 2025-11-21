/** 
 *  \file codac2_py_GaussJordan.cpp
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Luc Jaulin, Simon Rohou, Damien Massé
 *  \copyright  Copyright 2025 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <sstream>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <codac2_Matrix.h>
#include <codac2_IntervalMatrix.h>
#include <codac2_GaussJordan.h>

#include "codac2_py_doc.h"
#include "codac2_py_GaussJordan_docs.h" // Generated file from Doxygen 


using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_GaussJordan(py::module& m)
{
  m

    .def("gauss_jordan", [](const Matrix& A) { return gauss_jordan(A); },
      MATRIX_GAUSS_JORDAN_CONST_MATRIX_REF,
      "A"_a)
   ;
}
