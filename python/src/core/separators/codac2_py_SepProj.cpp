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
#include <codac2_SepProj.h>
#include "codac2_py_Sep.h"
#include "codac2_py_SepProj_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

vector<Index> test_and_convert(const vector<Index_type>& indices)
{
  #if FOR_MATLAB
  
    vector<Index> indices_Index(indices.size());

    for(Index i = 0 ; i < indices.size() ; i++)
    {
      matlab::test_integer(indices[i]);
      indices_Index[i] = matlab::input_index(indices[i]);
    }

  #else // same types: Index_type == Index

    return indices;

  #endif
}

void export_SepProj(py::module& m, py::class_<SepBase,pySep>& pysep)
{
  py::class_<SepProj> exported(m, "SepProj", pysep, SEPPROJ_MAIN);
  exported

    .def(py::init(
        [](const SepBase& s, vector<Index_type> proj_indices, double default_eps)
        {
          return make_unique<SepProj>(s.copy(), test_and_convert(proj_indices), default_eps);
        }),
      SEPPROJ_SEPPROJ_CONST_S_REF_CONST_VECTOR_INDEX_REF_DOUBLE,
      "s"_a, "proj_indices"_a, "default_eps"_a=0.01)

    .def(py::init(
        [](const SepBase& s, vector<Index_type> proj_indices, const IntervalVector& y, double default_eps)
        {
          return make_unique<SepProj>(s.copy(), test_and_convert(proj_indices), y, default_eps);
        }),
      SEPPROJ_SEPPROJ_CONST_S_REF_CONST_VECTOR_INDEX_REF_CONST_INTERVALVECTOR_REF_DOUBLE,
      "s"_a, "proj_indices"_a, "y"_a, "default_eps"_a=0.01)

    .def("separate", (BoxPair(SepProj::*)(const IntervalVector&) const) &SepProj::separate,
      BOXPAIR_SEPPROJ_SEPARATE_CONST_INTERVALVECTOR_REF_CONST,
      "x"_a)

    .def("separate", (BoxPair(SepProj::*)(const IntervalVector&,double) const) &SepProj::separate,
      BOXPAIR_SEPPROJ_SEPARATE_CONST_INTERVALVECTOR_REF_DOUBLE_CONST,
      "x"_a, "eps"_a)
  ;
}