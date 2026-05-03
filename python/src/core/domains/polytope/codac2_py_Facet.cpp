/** 
 *  Codac binding (core)
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Damien Massé
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <codac2_Facet.h>
#include "codac2_py_Facet_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):
#include "codac2_py_cast.h"

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_Facet(py::module& m)
{
  py::class_<FacetBase> 
    exported1(m, "FacetBase", FACETBASE_MAIN);
  exported1

    .def(py::init<const Row &>(),
      FACETBASE_FACETBASE_CONST_ROW_REF, "row"_a)

    .def(py::init<const FacetBase &>(),
      FACETBASE_FACETBASE_CONST_FACETBASE_REF,"fc"_a)

    .def("get_row",&FacetBase::get_row,
	CONST_ROW_REF_FACETBASE_GET_ROW_CONST)

    .def("is_null",&FacetBase::is_null,
	BOOL_FACETBASE_IS_NULL_CONST)
  
    .def("is_coord",&FacetBase::is_coord,
	BOOL_FACETBASE_IS_COORD_CONST)
  
    .def("gt_dim",&FacetBase::gt_dim,
	INDEX_FACETBASE_GT_DIM_CONST)
  
  ;

  py::class_<FacetRhs> 
    exported2(m, "FacetRhs", FACETRHS_MAIN);
  exported2

    .def(py::init<double, bool, Index>(),
      FACETRHS_FACETRHS_DOUBLE_BOOL_INDEX, "rhs"_a, "eqcst"_a, "Id"_a)

    .def("get_rhs",&FacetRhs::get_rhs,
      DOUBLE_FACETRHS_GET_RHS_CONST)

    .def("is_eqcst",&FacetRhs::is_eqcst,
	BOOL_FACETRHS_IS_EQCST_CONST)
  
    .def("get_Id",&FacetRhs::get_Id,
	INDEX_FACETRHS_GET_ID_CONST)
  
  ;
  
  m.def("Facet_make",[](const Row &row, double rhs, bool eqcst)
		{ return Facet_::make(row,rhs,eqcst); });

}
