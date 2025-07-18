/** 
 *  IV_templ binding
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <sstream>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>

#include <codac2_IntervalVector.h>

#include "codac2_py_MatrixBase.h"
#include "codac2_py_IntervalMatrixBase.h"
#include "codac2_py_VectorBase.h"

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;


template<typename IV,typename V>
void export_IntervalVector_(py::module& m, py::class_<IV>& pyclass)
{
  export_IntervalMatrixBase<IV,V,true>(m, pyclass);
  export_VectorBase<IV,IntervalMatrix,Interval>(m, pyclass);

  pyclass

    .def(py::init(
        [](Index_type n)
        {
          matlab::test_integer(n);
          return std::make_unique<IV>((int)n);
        }),
      DOC_TO_BE_DEFINED,
      "n"_a)

    .def(py::init<const IV&>(),
      "x"_a)

    .def(py::init<const V&>(),
      "x"_a)

    .def(py::init<const V&,const V&>(),
      MATRIX_ADDONS_INTERVALMATRIXBASE_MATRIX_CONST_MATRIX_DOUBLERC_REF_CONST_MATRIX_DOUBLERC_REF,
      "lb"_a, "ub"_a)
    
    .def(py::init(
        [](const std::vector<std::vector<double>>& v)
        {
          auto iv = std::make_unique<IV>(v.size());
          for(size_t i = 0 ; i < v.size() ; i++)
          {
            if(v[i].size() == 1)
              (*iv)[i] = Interval(v[i][0]);
            else if(v[i].size() == 2)
              (*iv)[i] = Interval(v[i][0],v[i][1]);
            else
              throw invalid_argument("Interval is not made of one or two values.");
          }
          return iv;
        }),
      DOC_TO_BE_DEFINED,
      "v"_a)

    .def(py::init( // this constructor must be the last one to be declared
        [](const std::vector<Interval>& v)
        {
          auto iv = std::make_unique<IV>(v.size());
          for(size_t i = 0 ; i < v.size() ; i++)
            (*iv)[i] = v[i];
          return iv;
        }),
      MATRIX_ADDONS_INTERVALVECTOR_MATRIX_INITIALIZER_LIST_INTERVAL,
      "v"_a)

    .def("complementary", [](const IV& x) { return x.complementary(); },
      MATRIXBASE_ADDONS_INTERVALVECTOR_AUTO_COMPLEMENTARY_CONST)

    .def("diff", [](const IV& x, const IV& y, bool compactness = true) { return x.diff(y,compactness); },
      MATRIXBASE_ADDONS_INTERVALVECTOR_LIST_MATRIX_INTERVALRC_DIFF_CONST_MATRIXBASE_OTHERDERIVED_REF_BOOL_CONST,
      "y"_a, "compactness"_a = true)
    
    .def_static("empty", [](Index_type n)
        {
          matlab::test_integer(n);
          return IV::empty(n);
        },
      MATRIX_ADDONS_INTERVALVECTOR_STATIC_AUTO_EMPTY_INDEX,
      "n"_a)

    .def("__repr__", [](const IV& x)
        {
          std::ostringstream s;
          s << x;
          return string(s.str()); 
        },
      OSTREAM_REF_OPERATOROUT_OSTREAM_REF_CONST_INTERVALVECTOR_REF)

  ;

  py::implicitly_convertible<py::list,IV>();
  py::implicitly_convertible<V,IV>();
}