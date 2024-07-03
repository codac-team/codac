/** 
 *  VectorBase binding
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
#include "codac2_py_core.h"
#include <codac2_VectorBase.h>
#include "codac2_py_VectorBase_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

template<typename S,typename M,typename T>
void export_VectorBase(py::class_<S>& pyclass)
{
  pyclass

    .def("subvector", [](const S& x, size_t_type start_id, size_t_type end_id)
        {
          matlab::test_integer(start_id, end_id);
          matlab::scale_index(start_id, end_id);
          return x.subvector(start_id, end_id);
        },
      S_VECTORBASE_SMT_SUBVECTOR_SIZET_SIZET_CONST,
      "start_id"_a, "end_id"_a)

    .def("resize", [](S& x, size_t_type n)
        {
          matlab::test_integer(n);
          x.resize(n);
        },
      VOID_VECTORBASE_SMT_RESIZE_SIZET,
      "n"_a)

    .def("put", [](S& x, size_t_type start_id, const S& x1)
        {
          matlab::test_integer(start_id);
          matlab::scale_index(start_id);
          x.put(start_id, x1);
        },
      VOID_VECTORBASE_SMT_PUT_SIZET_CONST_S_REF,
      "start_id"_a, "x"_a)

    .def("transpose", &S::transpose,
      M_VECTORBASE_SMT_TRANSPOSE_CONST)

    .def("diag_matrix", &S::diag_matrix,
      M_VECTORBASE_SMT_DIAG_MATRIX_CONST)
    
    .def_static("zeros", [](size_t_type n)
        {
          matlab::test_integer(n);
          return S::zeros(n);
        },
      STATIC_S_VECTORBASE_SMT_ZEROS_SIZET,
      "n"_a)
    
    .def_static("ones", [](size_t_type n)
        {
          matlab::test_integer(n);
          return S::ones(n);
        },
      STATIC_S_VECTORBASE_SMT_ONES_SIZET,
      "n"_a)
  ;
}