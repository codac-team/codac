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
#include <codac2_Polytope.h>
#include "codac2_py_Polytope_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):
#include "codac2_py_cast.h"

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_Polytope(py::module& m)
{
  py::class_<Polytope> 
    exported(m, "Polytope", POLYTOPE_MAIN);
  exported

    .def(py::init<>(),
      POLYTOPE_POLYTOPE)

    .def(py::init<Index>(),
      POLYTOPE_POLYTOPE_INDEX,"dim"_a)

    .def(py::init<Index, bool>(),
      POLYTOPE_POLYTOPE_INDEX_BOOL,
      "dim"_a,"empty"_a)

    .def(py::init<const IntervalVector &>(),
      POLYTOPE_POLYTOPE_CONST_INTERVALVECTOR_REF,
      "box"_a)

    .def(py::init<const Polytope &>(),
      POLYTOPE_POLYTOPE_CONST_POLYTOPE_REF,
      "P"_a)

    .def(py::init<const std::vector<Vector> &>(),
      POLYTOPE_POLYTOPE_CONST_VECTOR_VECTOR_REF,
      "vertices"_a)

    .def(py::init<const std::vector<IntervalVector> &>(),
      POLYTOPE_POLYTOPE_CONST_VECTOR_INTERVALVECTOR_REF,
      "vertices"_a)

    .def(py::init<const std::vector<IntervalVector> &>(),
      POLYTOPE_POLYTOPE_CONST_VECTOR_INTERVALVECTOR_REF,
      "vertices"_a)

    .def(py::init<const Parallelepiped &>(),
      POLYTOPE_POLYTOPE_CONST_PARALLELEPIPED_REF,
      "par"_a)

    .def(py::init<const Zonotope &>(),
      POLYTOPE_POLYTOPE_CONST_ZONOTOPE_REF,
      "zon"_a)

    .def(py::init<const IntervalVector &,const std::vector<std::pair<Row,double>> &, bool>(),
      POLYTOPE_POLYTOPE_CONST_INTERVALVECTOR_REF_CONST_VECTOR_PAIR_ROWDOUBLE_REF_BOOL,
      "box"_a,"facets"_a,"minimize"_a)

    .def("copy",[](Polytope &Q) { Polytope P=Q; return P; },
	POLYTOPE_REF_POLYTOPE_OPERATORAFF_CONST_POLYTOPE_REF)

    .def("dim",&Polytope::dim,
	INDEX_POLYTOPE_DIM_CONST)

    .def("dim",&Polytope::size,
	INDEX_POLYTOPE_SIZE_CONST)
  
    .def("nb_facets",&Polytope::nb_facets,
	INDEX_POLYTOPE_NB_FACETS_CONST)

    .def("nb_eq_facets",&Polytope::nb_eq_facets,
	INDEX_POLYTOPE_NB_EQ_FACETS_CONST)

    .def("is_empty",[](const Polytope &P) { return P.is_empty(true); },
        BOOL_POLYTOPE_IS_EMPTY_BOOL_CONST)

    .def("is_flat",&Polytope::is_flat,
        BOOL_POLYTOPE_IS_FLAT_CONST)

    .def("bound_row",&Polytope::bound_row,
        BOOL_POLYTOPE_IS_FLAT_CONST, "r"_a)

    .def("contains",&Polytope::contains,
        BOOLINTERVAL_POLYTOPE_CONTAINS_CONST_INTERVALVECTOR_REF_CONST, "p"_a)

    .def("is_subset",[](const Polytope &Q, const Polytope &P) 
			{ return Q.is_subset(P); },
        VIRTUAL_BOOL_POLYTOPE_IS_SUBSET_CONST_POLYTOPE_REF_CONST, "P"_a)

    .def("box",[](const Polytope &Q) 
			{ return Q.box(true); },
        CONST_INTERVALVECTOR_REF_POLYTOPE_BOX_BOOL_CONST)

    .def("meet_with_polytope",&Polytope::meet_with_polytope,
        INT_POLYTOPE_MEET_WITH_POLYTOPE_CONST_POLYTOPE_REF,"P"_a)
    
    .def("inflate",[](Polytope &Q, double rad) { return Q.inflate(rad); },
        POLYTOPE_REF_POLYTOPE_INFLATE_DOUBLE,"rad"_a)

    .def("inflate",[](Polytope &Q, IntervalVector &box) 	
		{ return Q.inflate(box); },
        POLYTOPE_REF_POLYTOPE_INFLATE_CONST_INTERVALVECTOR_REF, "box"_a)

    .def("inflate_ball",[](Polytope &Q, double rad) 
		{ return Q.inflate_ball(rad); },
        POLYTOPE_REF_POLYTOPE_INFLATE_BALL_DOUBLE,"rad"_a)

    .def("unflat",&Polytope::unflat,
        POLYTOPE_REF_POLYTOPE_UNFLAT_INDEX_DOUBLE,"dm"_a,"rad"_a)

    .def("homothety",&Polytope::homothety,
        POLYTOPE_REF_POLYTOPE_HOMOTHETY_CONST_INTERVALVECTOR_REF_DOUBLE,
	"c"_a,"delta"_a)

    .def("minimize_constraints",&Polytope::minimize_constraints,
        VIRTUAL_VOID_POLYTOPE_MINIMIZE_CONSTRAINTS_CONST)

    .def("reverse_affine_transform",&Polytope::reverse_affine_transform,
       POLYTOPE_POLYTOPE_REVERSE_AFFINE_TRANSFORM_CONST_INTERVALMATRIX_REF_CONST_INTERVALVECTOR_REF_CONST_INTERVALVECTOR_REF_CONST,
	"M"_a,"P"_a,"bbox"_a)
 
    .def("affine_transform",&Polytope::affine_transform,
	POLYTOPE_POLYTOPE_AFFINE_TRANSFORM_CONST_INTERVALMATRIX_REF_CONST_INTERVALVECTOR_REF_CONST_INTERVALVECTOR_REF_BOOL_CONST,
	"M"_a,"P"_a,"B"_a,"use_direct"_a)

    .def("direct_affine_transform",&Polytope::direct_affine_transform,
	POLYTOPE_POLYTOPE_DIRECT_AFFINE_TRANSFORM_CONST_INTERVALMATRIX_REF_CONST_INTERVALVECTOR_REF_CONST,
	"M"_a,"P"_a)

    .def("vertices",&Polytope::vertices,
	VECTOR_INTERVALVECTOR_POLYTOPE_VERTICES_CONST)

    .def("vertices_3Dfacets",&Polytope::vertices_3Dfacets,
	VECTOR_VECTOR_VECTOR_POLYTOPE_VERTICES_3DFACETS_CONST)

    .def_static("from_ineFile",&Polytope::from_ineFile,
	STATIC_POLYTOPE_POLYTOPE_FROM_INEFILE_CONST_CHAR_PTR,"filename"_a)

    .def_static("from_extFile",&Polytope::from_extFile,
	STATIC_POLYTOPE_POLYTOPE_FROM_EXTFILE_CONST_CHAR_PTR,"filename"_a)

    .def_static("union_of_polytopes", [](const std::vector<Polytope> &lst)
		{ return Polytope::union_of_polytopes(lst); },
	STATIC_POLYTOPE_POLYTOPE_UNION_OF_POLYTOPES_CONST_VECTOR_POLYTOPE_REF,
	"lst"_a)

    .def("__repr__", [](const Polytope& P) {
          std::ostringstream stream;
          stream << P;
          return string(stream.str()); 
        },
       OSTREAM_REF_OPERATOROUT_OSTREAM_REF_CONST_POLYTOPE_REF)
  ;
}
