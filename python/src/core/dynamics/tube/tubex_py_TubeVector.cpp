/** 
 *  \file
 *  TubeVector Python binding
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou, Benoît Desrochers
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <sstream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <pybind11/functional.h>
#include "pyIbex_type_caster.h"

#include "tubex_TubeVector.h"
// Generated file from Doxygen XML (doxygen2docstring.py):
#include "tubex_py_TubeVector_docs.h"

using namespace std;
using namespace ibex;
using namespace tubex;
namespace py = pybind11;
using namespace pybind11::literals;


void export_TubeVector(py::module& m)
{
  py::class_<TubeVector> tube_vector(m, "TubeVector", "todo");
  tube_vector

  // Definition

    .def(py::init<const Interval&,int>(),
      TUBEVECTOR_TUBEVECTOR_INTERVAL_INT,
      "domain"_a, "n"_a=1)

    .def(py::init<const Interval&,const IntervalVector &>(),
      TUBEVECTOR_TUBEVECTOR_INTERVAL_INTERVALVECTOR,
      "domain"_a, "codomain"_a)

    .def(py::init<const Interval&,double,int>(),
      TUBEVECTOR_TUBEVECTOR_INTERVAL_DOUBLE_INT,
      "domain"_a, "timestep"_a, "n"_a=1)

    .def(py::init<const Interval&,double,const IntervalVector &>(),
      TUBEVECTOR_TUBEVECTOR_INTERVAL_DOUBLE_INTERVALVECTOR,
      "domain"_a, "timestep"_a, "codomain"_a)

    .def(py::init<const Interval&,double,const TFnc &>(),
      TUBEVECTOR_TUBEVECTOR_INTERVAL_DOUBLE_TFNC,
      "domain"_a, "timestep"_a, "f"_a)

    .def(py::init<const vector<Interval>&,const vector<IntervalVector> &>(),
      TUBEVECTOR_TUBEVECTOR_VECTORINTERVAL_VECTORINTERVALVECTOR,
      "v_tdomains"_a, "v_codomains"_a)

    .def(py::init<initializer_list<Tube>>(),
      TUBEVECTOR_TUBEVECTOR_INITIALIZERLISTTUBE,
      "list"_a)

    .def(py::init<const TubeVector &>(),
      TUBEVECTOR_TUBEVECTOR_TUBEVECTOR,
      "x"_a)

    .def(py::init<const TubeVector&,const IntervalVector &>(),
      TUBEVECTOR_TUBEVECTOR_TUBEVECTOR_INTERVALVECTOR,
      "x"_a, "codomain"_a)

    .def(py::init<int,const Tube &>(),
      TUBEVECTOR_TUBEVECTOR_INT_TUBE,
      "n"_a, "x"_a)

    .def(py::init<const TrajectoryVector&,double>(),
      TUBEVECTOR_TUBEVECTOR_TRAJECTORYVECTOR_DOUBLE,
      "traj"_a, "timestep"_a)

    .def(py::init<const TrajectoryVector&,const TrajectoryVector&,double>(),
      TUBEVECTOR_TUBEVECTOR_TRAJECTORYVECTOR_TRAJECTORYVECTOR_DOUBLE,
      "lb"_a, "ub"_a, "timestep"_a)

    .def(py::init<const string &>(),
      TUBEVECTOR_TUBEVECTOR_STRING,
      "binary_file_name"_a)

    //.def(py::init<const string&,TrajectoryVector * &>(),
    //  TUBEVECTOR_TUBEVECTOR_STRING_TRAJECTORYVECTOR,
    //  "binary_file_name"_a, "traj"_a)

    .def("size", &TubeVector::size,
      TUBEVECTOR_INT_SIZE)

    .def("resize", &TubeVector::resize,
      TUBEVECTOR_VOID_RESIZE_INT,
      "n"_a)

    .def("subvector", &TubeVector::subvector,
      TUBEVECTOR_CONSTTUBEVECTOR_SUBVECTOR_INT_INT,
      "start_index"_a, "end_index"_a)

    .def("put", &TubeVector::put,
      TUBEVECTOR_VOID_PUT_INT_TUBEVECTOR,
      "start_index"_a, "subvec"_a)

    .def("primitive", (const TubeVector (TubeVector::*)() const)&TubeVector::primitive,
      TUBEVECTOR_CONSTTUBEVECTOR_PRIMITIVE)

    .def("primitive", (const TubeVector (TubeVector::*)(const IntervalVector&) const)&TubeVector::primitive,
      TUBEVECTOR_CONSTTUBEVECTOR_PRIMITIVE_INTERVALVECTOR,
      "c"_a)

    .def("tdomain", &TubeVector::tdomain,
      TUBEVECTOR_CONSTINTERVAL_TDOMAIN)

  // Slices structure

    .def("nb_slices", &TubeVector::nb_slices,
      TUBEVECTOR_INT_NB_SLICES)

    .def("time_to_index", &TubeVector::time_to_index,
      TUBEVECTOR_INT_TIME_TO_INDEX_DOUBLE,
      "t"_a)

    .def("sample", (void (TubeVector::*)(double))&TubeVector::sample,
      TUBEVECTOR_VOID_SAMPLE_DOUBLE,
      "t"_a)

    .def("sample", (void (TubeVector::*)(double,const IntervalVector&))&TubeVector::sample,
      TUBEVECTOR_VOID_SAMPLE_DOUBLE_INTERVALVECTOR,
      "t"_a, "gate"_a)

    .def("sample", (void (TubeVector::*)(const Tube&))&TubeVector::sample,
      TUBEVECTOR_VOID_SAMPLE_TUBE,
      "x"_a)

    .def("sample", (void (TubeVector::*)(const TubeVector&))&TubeVector::sample,
      TUBEVECTOR_VOID_SAMPLE_TUBEVECTOR,
      "x"_a)

  // Accessing values

    .def("codomain", &TubeVector::codomain,
      TUBEVECTOR_CONSTINTERVALVECTOR_CODOMAIN)

    .def("volume", &TubeVector::volume,
      TUBEVECTOR_DOUBLE_VOLUME)

    .def("__call__", [](TubeVector& s,int o) { return s(o);}, 
      TUBEVECTOR_CONSTINTERVALVECTOR_OPERATORP_INT)

    .def("__call__", [](TubeVector& s,double o) { return s(o);}, 
      TUBEVECTOR_CONSTINTERVALVECTOR_OPERATORP_DOUBLE)

    .def("__call__", [](TubeVector& s,const Interval& o) { return s(o);}, 
      TUBEVECTOR_CONSTINTERVALVECTOR_OPERATORP_INTERVAL)

    .def("eval", &TubeVector::eval,
      TUBEVECTOR_CONSTPAIRINTERVALVECTORINTERVALVECTOR_EVAL_INTERVAL,
      "t"_a=Interval::all_reals())

    .def("interpol", (const IntervalVector (TubeVector::*)(double,const TubeVector&) const)&TubeVector::interpol,
      TUBEVECTOR_CONSTINTERVALVECTOR_INTERPOL_DOUBLE_TUBEVECTOR, 
      "t"_a, "v"_a)

    .def("interpol", (const IntervalVector (TubeVector::*)(const Interval&,const TubeVector&) const)&TubeVector::interpol,
      TUBEVECTOR_CONSTINTERVALVECTOR_INTERPOL_INTERVAL_TUBEVECTOR,
      "t"_a, "v"_a)

    .def("invert", (const Interval (TubeVector::*)(const IntervalVector&,const Interval&) const)&TubeVector::invert,
      TUBEVECTOR_CONSTINTERVAL_INVERT_INTERVALVECTOR_INTERVAL,
      "y"_a, "search_domain"_a=Interval::all_reals())

    .def("invert", (void (TubeVector::*)(const IntervalVector&,vector<Interval>&,const Interval&) const)&TubeVector::invert,
      TUBEVECTOR_VOID_INVERT_INTERVALVECTOR_VECTORINTERVAL_INTERVAL,
      "y"_a, "v_t"_a, "search_domain"_a=Interval::all_reals())

    .def("invert", (const Interval (TubeVector::*)(const IntervalVector&,const TubeVector&,const Interval&) const)&TubeVector::invert,
      TUBEVECTOR_CONSTINTERVAL_INVERT_INTERVALVECTOR_TUBEVECTOR_INTERVAL,
      "y"_a, "v"_a, "search_domain"_a=Interval::all_reals())

    .def("invert", (void (TubeVector::*)(const IntervalVector&,vector<Interval>&,const TubeVector&,const Interval&) const)&TubeVector::invert,
      TUBEVECTOR_VOID_INVERT_INTERVALVECTOR_VECTORINTERVAL_TUBEVECTOR_INTERVAL,
      "y"_a, "v_t"_a, "v"_a, "search_domain"_a=Interval::all_reals())

    .def("max_diam", &TubeVector::max_diam,
      TUBEVECTOR_CONSTVECTOR_MAX_DIAM)

    .def("diam", (const TrajectoryVector (TubeVector::*)(bool) const)&TubeVector::diam,
      TUBEVECTOR_CONSTTRAJECTORYVECTOR_DIAM_BOOL,
      "gates_thicknesses"_a=false)

    .def("diam", (const TrajectoryVector (TubeVector::*)(const TubeVector&) const)&TubeVector::diam,
      TUBEVECTOR_CONSTTRAJECTORYVECTOR_DIAM_TUBEVECTOR,
      "v"_a)

    .def("diag", (const Trajectory (TubeVector::*)(bool) const)&TubeVector::diag,
      TUBEVECTOR_CONSTTRAJECTORY_DIAG_BOOL,
      "gates_diag"_a=false)

    .def("diag", (const Trajectory (TubeVector::*)(int,int,bool) const)&TubeVector::diag,
      TUBEVECTOR_CONSTTRAJECTORY_DIAG_INT_INT_BOOL,
      "start_index"_a, "end_index"_a, "gates_diag"_a=false)

  // Tests

    .def("__eq__", [](TubeVector& s,const TubeVector& o) { return s == o;}, 
      TUBEVECTOR_BOOL_OPERATOREQ_TUBEVECTOR)

    .def("__ne__", [](TubeVector& s,const TubeVector& o) { return s != o;}, 
      TUBEVECTOR_BOOL_OPERATORNEQ_TUBEVECTOR)

    .def("is_subset", &TubeVector::is_subset,
      TUBEVECTOR_BOOL_IS_SUBSET_TUBEVECTOR,
      "x"_a)

    .def("is_strict_subset", &TubeVector::is_strict_subset,
      TUBEVECTOR_BOOL_IS_STRICT_SUBSET_TUBEVECTOR,
      "x"_a)

    .def("is_interior_subset", &TubeVector::is_interior_subset,
      TUBEVECTOR_BOOL_IS_INTERIOR_SUBSET_TUBEVECTOR,
      "x"_a)

    .def("is_strict_interior_subset", &TubeVector::is_strict_interior_subset,
      TUBEVECTOR_BOOL_IS_STRICT_INTERIOR_SUBSET_TUBEVECTOR,
      "x"_a)

    .def("is_superset", &TubeVector::is_superset,
      TUBEVECTOR_BOOL_IS_SUPERSET_TUBEVECTOR,
      "x"_a)

    .def("is_strict_superset", &TubeVector::is_strict_superset,
      TUBEVECTOR_BOOL_IS_STRICT_SUPERSET_TUBEVECTOR,
      "x"_a)

    .def("is_empty", &TubeVector::is_empty,
      TUBEVECTOR_BOOL_IS_EMPTY)

    .def("contains", &TubeVector::contains,
      TUBEVECTOR_CONSTBOOLINTERVAL_CONTAINS_TRAJECTORYVECTOR,
      "x"_a)

  // Setting values

    .def("overlaps", &TubeVector::overlaps,
      TUBEVECTOR_BOOL_OVERLAPS_TUBEVECTOR_FLOAT,
      "x"_a, "ratio"_a=1)

    .def("set", (void (TubeVector::*)(const IntervalVector&))&TubeVector::set,
      TUBEVECTOR_VOID_SET_INTERVALVECTOR,
      "y"_a)

    .def("set", (void (TubeVector::*)(const IntervalVector&,int))&TubeVector::set,
      TUBEVECTOR_VOID_SET_INTERVALVECTOR_INT,
      "y"_a, "slice_id"_a)

    .def("set", (void (TubeVector::*)(const IntervalVector&,double))&TubeVector::set,
      TUBEVECTOR_VOID_SET_INTERVALVECTOR_DOUBLE,
      "y"_a, "t"_a)

    .def("set", (void (TubeVector::*)(const IntervalVector&,const Interval&))&TubeVector::set,
      TUBEVECTOR_VOID_SET_INTERVALVECTOR_INTERVAL,
      "y"_a, "t"_a)

    .def("set_empty", &TubeVector::set_empty,
      TUBEVECTOR_VOID_SET_EMPTY)

    .def("inflate", (const TubeVector& (TubeVector::*)(double))&TubeVector::inflate,
      TUBEVECTOR_CONSTTUBEVECTOR_INFLATE_DOUBLE,
      "rad"_a)

    .def("inflate", (const TubeVector& (TubeVector::*)(const Vector&))&TubeVector::inflate,
      TUBEVECTOR_CONSTTUBEVECTOR_INFLATE_VECTOR,
      "rad"_a)

    .def("inflate", (const TubeVector& (TubeVector::*)(const TrajectoryVector&))&TubeVector::inflate,
      TUBEVECTOR_CONSTTUBEVECTOR_INFLATE_TRAJECTORYVECTOR,
      "rad"_a)

    .def("shift_tdomain", &TubeVector::shift_tdomain,
      TUBEVECTOR_VOID_SHIFT_TDOMAIN_DOUBLE,
      "a"_a)

  // Bisection

    .def("bisect", (const pair<TubeVector, TubeVector> (TubeVector::*)(double,float) const)&TubeVector::bisect,
      TUBEVECTOR_CONSTPAIRTUBEVECTORTUBEVECTOR_BISECT_DOUBLE_FLOAT,
      "t"_a, "ratio"_a=0.49)

    .def("bisect", (const pair<TubeVector, TubeVector> (TubeVector::*)(double,int,float) const)&TubeVector::bisect,
      TUBEVECTOR_CONSTPAIRTUBEVECTORTUBEVECTOR_BISECT_DOUBLE_INT_FLOAT,
      "t"_a, "dim"_a, "ratio"_a=0.49)

  // Assignments operators

    .def("__iadd__", [](TubeVector& s,const Interval& o) { return s += o;}, 
      TUBEVECTOR_CONSTTUBEVECTOR_OPERATORADDEQ_INTERVAL)

    .def("__iadd__", [](TubeVector& s,const Trajectory& o) { return s += o;}, 
      TUBEVECTOR_CONSTTUBEVECTOR_OPERATORADDEQ_TRAJECTORY)

    .def("__iadd__", [](TubeVector& s,const Tube& o) { return s += o;}, 
      TUBEVECTOR_CONSTTUBEVECTOR_OPERATORADDEQ_TUBE)

    .def("__iadd__", [](TubeVector& s,const IntervalVector& o) { return s += o;}, 
      TUBEVECTOR_CONSTTUBEVECTOR_OPERATORADDEQ_INTERVALVECTOR)

    .def("__iadd__", [](TubeVector& s,const TrajectoryVector& o) { return s += o;}, 
      TUBEVECTOR_CONSTTUBEVECTOR_OPERATORADDEQ_TRAJECTORYVECTOR)

    .def("__iadd__", [](TubeVector& s,const TubeVector& o) { return s += o;}, 
      TUBEVECTOR_CONSTTUBEVECTOR_OPERATORADDEQ_TUBEVECTOR)

    .def("__isub__", [](TubeVector& s,const Interval& o) { return s -= o;}, 
      TUBEVECTOR_CONSTTUBEVECTOR_OPERATORMINEQ_INTERVAL)

    .def("__isub__", [](TubeVector& s,const Trajectory& o) { return s -= o;}, 
      TUBEVECTOR_CONSTTUBEVECTOR_OPERATORMINEQ_TRAJECTORY)

    .def("__isub__", [](TubeVector& s,const Tube& o) { return s -= o;}, 
      TUBEVECTOR_CONSTTUBEVECTOR_OPERATORMINEQ_TUBE)

    .def("__isub__", [](TubeVector& s,const IntervalVector& o) { return s -= o;}, 
      TUBEVECTOR_CONSTTUBEVECTOR_OPERATORMINEQ_INTERVALVECTOR)

    .def("__isub__", [](TubeVector& s,const TrajectoryVector& o) { return s -= o;}, 
      TUBEVECTOR_CONSTTUBEVECTOR_OPERATORMINEQ_TRAJECTORYVECTOR)

    .def("__isub__", [](TubeVector& s,const TubeVector& o) { return s -= o;}, 
      TUBEVECTOR_CONSTTUBEVECTOR_OPERATORMINEQ_TUBEVECTOR)

    .def("__imul__", [](TubeVector& s,const Interval& o) { return s *= o;}, 
      TUBEVECTOR_CONSTTUBEVECTOR_OPERATORMULEQ_INTERVAL)

    .def("__imul__", [](TubeVector& s,const Trajectory& o) { return s *= o;}, 
      TUBEVECTOR_CONSTTUBEVECTOR_OPERATORMULEQ_TRAJECTORY)

    .def("__imul__", [](TubeVector& s,const Tube& o) { return s *= o;}, 
      TUBEVECTOR_CONSTTUBEVECTOR_OPERATORMULEQ_TUBE)

    .def("__itruediv__", [](TubeVector& s,const Interval& o) { return s /= o;}, 
      TUBEVECTOR_CONSTTUBEVECTOR_OPERATORDIVEQ_INTERVAL)

    .def("__itruediv__", [](TubeVector& s,const Trajectory& o) { return s /= o;}, 
      TUBEVECTOR_CONSTTUBEVECTOR_OPERATORDIVEQ_TRAJECTORY)

    .def("__itruediv__", [](TubeVector& s,const Tube& o) { return s /= o;}, 
      TUBEVECTOR_CONSTTUBEVECTOR_OPERATORDIVEQ_TUBE)

    .def("__ior__", [](TubeVector& s,const IntervalVector& o) { return s |= o;}, 
      TUBEVECTOR_CONSTTUBEVECTOR_OPERATORUNIEQ_INTERVALVECTOR)

    .def("__ior__", [](TubeVector& s,const TrajectoryVector& o) { return s |= o;}, 
      TUBEVECTOR_CONSTTUBEVECTOR_OPERATORUNIEQ_TRAJECTORYVECTOR)

    .def("__ior__", [](TubeVector& s,const TubeVector& o) { return s |= o;}, 
      TUBEVECTOR_CONSTTUBEVECTOR_OPERATORUNIEQ_TUBEVECTOR)

    .def("__iand__", [](TubeVector& s,const IntervalVector& o) { return s &= o;}, 
      TUBEVECTOR_CONSTTUBEVECTOR_OPERATORINTEQ_INTERVALVECTOR)

    .def("__iand__", [](TubeVector& s,const TrajectoryVector& o) { return s &= o;}, 
      TUBEVECTOR_CONSTTUBEVECTOR_OPERATORINTEQ_TRAJECTORYVECTOR)

    .def("__iand__", [](TubeVector& s,const TubeVector& o) { return s &= o;}, 
      TUBEVECTOR_CONSTTUBEVECTOR_OPERATORINTEQ_TUBEVECTOR)

  // String

    .def("class_name", &TubeVector::class_name,
      TUBEVECTOR_CONSTSTRING_CLASS_NAME)

    .def("__repr__", [](const TubeVector& x) { ostringstream str; str << x; return str.str(); })

  // Tree synthesis structure

    .def("enable_synthesis", &TubeVector::enable_synthesis,
      TUBEVECTOR_VOID_ENABLE_SYNTHESIS_BOOL,
      "enable"_a=true)

    .def("integral", (const IntervalVector (TubeVector::*)(double) const)&TubeVector::integral,
      TUBEVECTOR_CONSTINTERVALVECTOR_INTEGRAL_DOUBLE,
      "t"_a)

    .def("integral", (const IntervalVector (TubeVector::*)(const Interval&) const)&TubeVector::integral,
      TUBEVECTOR_CONSTINTERVALVECTOR_INTEGRAL_INTERVAL,
      "t"_a)

    .def("integral", (const IntervalVector (TubeVector::*)(const Interval&,const Interval&) const)&TubeVector::integral,
      TUBEVECTOR_CONSTINTERVALVECTOR_INTEGRAL_INTERVAL_INTERVAL,
      "t1"_a, "t2"_a)

    .def("partial_integral", (const pair<IntervalVector,IntervalVector> (TubeVector::*)(const Interval&) const)&TubeVector::partial_integral,
      TUBEVECTOR_CONSTPAIRINTERVALVECTORINTERVALVECTOR_PARTIAL_INTEGRAL_INTERVAL,
      "t"_a)

    .def("partial_integral", (const pair<IntervalVector,IntervalVector> (TubeVector::*)(const Interval&,const Interval&) const)&TubeVector::partial_integral,
      TUBEVECTOR_CONSTPAIRINTERVALVECTORINTERVALVECTOR_PARTIAL_INTEGRAL_INTERVAL_INTERVAL,
      "t1"_a, "t2"_a)

  // Serialization

    .def("serialize", (void (TubeVector::*)(const string&,int) const)&TubeVector::serialize,
      TUBEVECTOR_VOID_SERIALIZE_STRING_INT,
      "binary_file_name"_a="x.tube", "version_number"_a=SERIALIZATION_VERSION)

    .def("serialize", (void (TubeVector::*)(const string&,const TrajectoryVector&,int) const)&TubeVector::serialize,
      TUBEVECTOR_VOID_SERIALIZE_STRING_TRAJECTORYVECTOR_INT,
      "binary_file_name"_a, "traj"_a, "version_number"_a=SERIALIZATION_VERSION)

  // Static methods

    .def_static("same_slicing", [](const TubeVector& x1,const Tube& x2) { TubeVector::same_slicing(x1, x2); },
      TUBEVECTOR_BOOL_SAME_SLICING_TUBEVECTOR_TUBE,
      "x1"_a, "x2"_a)
    
    .def_static("same_slicing", [](const TubeVector& x1,const TubeVector& x2) { TubeVector::same_slicing(x1, x2); },
      TUBEVECTOR_BOOL_SAME_SLICING_TUBEVECTOR_TUBEVECTOR,
      "x1"_a, "x2"_a)
    
    .def_static("hull", &TubeVector::hull,
      TUBEVECTOR_CONSTTUBEVECTOR_HULL_LISTTUBEVECTOR,
      "l_tubes"_a)

  // Python vector methods

    .def("__len__", &TubeVector::size)

    .def("__getitem__", [](TubeVector& s, size_t index) -> Tube&
      {
        if(index >= static_cast<size_t>(s.size()))
          throw py::index_error();
        return s[static_cast<int>(index)];
      },
      TUBEVECTOR_TUBE_OPERATORB_INT,
      py::return_value_policy::reference_internal)

    .def("__getitem__", [](const TubeVector& s, py::slice slice) -> TubeVector
      {
        size_t start, stop, step, slicelength;

        if(!slice.compute(s.size(), &start, &stop, &step, &slicelength))
          throw py::error_already_set();

        if(step != 1)
          cout << "Warning slice step must be equal to 1\n";
        
        // To respect the python convention, the stop index 
        // is not included in slice
        return s.subvector(start, start+slicelength-1);
      },
      TUBEVECTOR_CONSTTUBE_OPERATORB_INT)

    .def("__setitem__", [](TubeVector& s, size_t index, Tube& t)
      {
        if(index >= static_cast<size_t>(s.size()))
          throw py::index_error();
        s[static_cast<int>(index)] = t;
      },
      TUBEVECTOR_TUBE_OPERATORB_INT)

  // Operators

    .def("__add__",      [](const TubeVector& x) { return +x; })
    .def("__add__",      [](const TubeVector& x, const TubeVector& y) { return x+y; })
    .def("__add__",      [](const TubeVector& x, const IntervalVector& y) { return x+y; })
    .def("__add__",      [](const TubeVector& x, const TrajectoryVector& y) { return x+y; })

    .def("__radd__",     [](const TubeVector& y, const IntervalVector& x) { return x+y; })
    .def("__radd__",     [](const TubeVector& y, const TrajectoryVector& x) { return x+y; })

    .def("__neg__",      [](const TubeVector& x) { return -x; })

    .def("__sub__",      [](const TubeVector& x, const TubeVector& y) { return x-y; })
    .def("__sub__",      [](const TubeVector& x, const IntervalVector& y) { return x-y; })
    .def("__sub__",      [](const TubeVector& x, const TrajectoryVector& y) { return x-y; })

    .def("__rsub__",     [](const TubeVector& y, const IntervalVector& x) { return x-y; })
    .def("__rsub__",     [](const TubeVector& y, const TrajectoryVector& x) { return x-y; })

    .def("__rmul__",     [](const TubeVector& y, const Tube& x) { return x*y; })
    .def("__rmul__",     [](const TubeVector& y, const Interval& x) { return x*y; })
    .def("__rmul__",     [](const TubeVector& y, const Trajectory& x) { return x*y; })

    // todo const TubeVector operator*(const Tube& x, const IntervalVector& y);

    .def("__truediv__",  [](const TubeVector& x, const Tube& y) { return x/y; })
    .def("__truediv__",  [](const TubeVector& x, const Interval& y) { return x/y; })
    .def("__truediv__",  [](const TubeVector& x, const Trajectory& y) { return x/y; })

    // todo .def("__truediv__", [](const IntervalVector& x, const Tube& y);

    .def("__or__",       [](const TubeVector& x, const TubeVector& y) { return x|y; })
    .def("__or__",       [](const TubeVector& x, const IntervalVector& y) { return x|y; })
    .def("__or__",       [](const TubeVector& x, const TrajectoryVector& y) { return x|y; })

    .def("__ror__",      [](const TubeVector& y, const IntervalVector& x) { return x|y; })
    .def("__ror__",      [](const TubeVector& y, const TrajectoryVector& x) { return x|y; })

    .def("__and__",      [](const TubeVector& x, const TubeVector& y) { return x&y; })
    .def("__and__",      [](const TubeVector& x, const IntervalVector& y) { return x&y; })
    .def("__and__",      [](const TubeVector& x, const TrajectoryVector& y) { return x&y; })

    .def("__rand__",     [](const TubeVector& y, const IntervalVector& x) { return x&y; })
    .def("__rand__",     [](const TubeVector& y, const TrajectoryVector& x) { return x&y; })
  ;
}