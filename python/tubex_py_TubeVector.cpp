/** 
 *  \file
 *  Tube binding
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou, Benoît Desrochers
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <pybind11/functional.h>

#include "tubex_TubeVector.h"
#include "tubex_py_TubeVector_docs.h"

namespace py = pybind11;
using namespace pybind11::literals;
using py::class_;
using py::init;

using namespace tubex;
using ibex::Interval;
using ibex::IntervalVector;

void export_TubeVector(py::module& m){

    py::class_<TubeVector> tubevector(m, "TubeVector");
    tubevector
      .def(py::init<const ibex::Interval &,int>(),
          DOCS_TUBEVECTOR_TUBEVECTOR_INTERVAL_INT, "domain"_a, "n"_a=1)
      .def(py::init<const ibex::Interval &,const ibex::IntervalVector &>(),
          DOCS_TUBEVECTOR_TUBEVECTOR_INTERVAL_INTERVALVECTOR, "domain"_a, "codomain"_a)
      .def(py::init<const ibex::Interval &,double,int>(),
          DOCS_TUBEVECTOR_TUBEVECTOR_INTERVAL_DOUBLE_INT, "domain"_a, "timestep"_a, "n"_a=1)
      .def(py::init<const ibex::Interval &,double,const ibex::IntervalVector &>(),
          DOCS_TUBEVECTOR_TUBEVECTOR_INTERVAL_DOUBLE_INTERVALVECTOR, "domain"_a, "timestep"_a, "codomain"_a)
      .def(py::init<const ibex::Interval &,double,const TFnc &>(),
          DOCS_TUBEVECTOR_TUBEVECTOR_INTERVAL_DOUBLE_FNC, "domain"_a, "timestep"_a, "f"_a)
      .def(py::init<const TrajectoryVector &,double>(),
          DOCS_TUBEVECTOR_TUBEVECTOR_TRAJECTORYVECTOR_DOUBLE, "traj"_a, "timestep"_a=0)
    //   .def(py::init(
    //       [](const ibex::Intervalconst ibex::Interval& domain tdomain,double timestep, const tubex::Function& f){
    //           return new TubeVector(domain, timestep, f );
    //       }), "domain"_a, "timestep"_a, "f"_a, py::keep_alive<1,4>())
      .def(py::init<const std::vector<ibex::Interval> &,const std::vector<ibex::IntervalVector> &>(),
          DOCS_TUBEVECTOR_TUBEVECTOR_VECTOR_INTERVAL__VECTOR_INTERVALVECTOR_, "v_domains"_a, "v_codomains"_a)
      .def(py::init<std::initializer_list<Tube>>(),
          DOCS_TUBEVECTOR_TUBEVECTOR_INITIALIZER_LIST_TUBE_, "list"_a)
      .def(py::init<const TubeVector &>(),DOCS_TUBEVECTOR_TUBEVECTOR_TUBEVECTOR, "x"_a)
      .def(py::init<const TubeVector &,const ibex::IntervalVector &>(),
          DOCS_TUBEVECTOR_TUBEVECTOR_TUBEVECTOR_INTERVALVECTOR, "x"_a, "codomain"_a)
      .def(py::init<int,const Tube &>(),DOCS_TUBEVECTOR_TUBEVECTOR_INT_TUBE, "n"_a, "x"_a)
      .def(py::init<const TrajectoryVector &,const TrajectoryVector &,double>(),
          DOCS_TUBEVECTOR_TUBEVECTOR_TRAJECTORYVECTOR_TRAJECTORYVECTOR_DOUBLE, "lb"_a, "ub"_a, "timestep"_a=0)
      .def(py::init<const std::string &>(),
          DOCS_TUBEVECTOR_TUBEVECTOR_STRING, "binary_file_name"_a)
    //   .def(py::init<const std::string &,TrajectoryVector * &>(),
        //   DOCS_TUBEVECTOR_TUBEVECTOR_STRING_TRAJECTORYVECTOR, "binary_file_name"_a, "traj"_a)
      .def("size", &TubeVector::size,DOCS_TUBEVECTOR_SIZE)
      .def("resize", &TubeVector::resize,DOCS_TUBEVECTOR_RESIZE_INT, "n"_a)
      .def("subvector", &TubeVector::subvector,
          DOCS_TUBEVECTOR_SUBVECTOR_INT_INT, "start_index"_a, "end_index"_a)
      .def("put", &TubeVector::put,
          DOCS_TUBEVECTOR_PUT_INT_TUBEVECTOR, "start_index"_a, "subvec"_a)
      .def("primitive", (const TubeVector (TubeVector::*)() const)&TubeVector::primitive,
          DOCS_TUBEVECTOR_PRIMITIVE)
      .def("primitive", (const TubeVector (TubeVector::*)(const ibex::IntervalVector &) const)&TubeVector::primitive,
          DOCS_TUBEVECTOR_PRIMITIVE_INTERVALVECTOR, "c"_a)
    
      .def("tdomain", &TubeVector::tdomain,DOCS_TUBEVECTOR_TDOMAIN)
      .def("nb_slices", &TubeVector::nb_slices,DOCS_TUBEVECTOR_NB_SLICES)
      .def("input2index", &TubeVector::input2index,DOCS_TUBEVECTOR_INPUT2INDEX_DOUBLE, "t"_a)
      .def("sample", (void (TubeVector::*)(double) )&TubeVector::sample,
          DOCS_TUBEVECTOR_SAMPLE_DOUBLE, "t"_a)
      .def("sample", (void (TubeVector::*)(double,const ibex::IntervalVector &) )&TubeVector::sample,
          DOCS_TUBEVECTOR_SAMPLE_DOUBLE_INTERVALVECTOR, "t"_a, "gate"_a)
      .def("sample", (void (TubeVector::*)(const Tube &) )&TubeVector::sample,
          DOCS_TUBEVECTOR_SAMPLE_TUBE, "x"_a)
      .def("sample", (void (TubeVector::*)(const TubeVector &) )&TubeVector::sample,
          DOCS_TUBEVECTOR_SAMPLE_TUBEVECTOR, "x"_a)
      .def("codomain", &TubeVector::codomain,DOCS_TUBEVECTOR_CODOMAIN)
      .def("volume", &TubeVector::volume,DOCS_TUBEVECTOR_VOLUME)
      .def("__call__", [](TubeVector& s,int o) { return s(o);}, 
          DOCS_TUBEVECTOR_OPERATOR_CALL_INT)
      .def("__call__", [](TubeVector& s,double o) { return s(o);}, 
          DOCS_TUBEVECTOR_OPERATOR_CALL_DOUBLE)
      .def("__call__", [](TubeVector& s,const ibex::Interval & o) { return s(o);}, 
          DOCS_TUBEVECTOR_OPERATOR_CALL_INTERVAL)
      .def("eval", &TubeVector::eval,
          DOCS_TUBEVECTOR_EVAL_INTERVAL, "t"_a=ibex::Interval::ALL_REALS)
      .def("interpol", (const ibex::IntervalVector (TubeVector::*)(double,const TubeVector &) const)&TubeVector::interpol,
          DOCS_TUBEVECTOR_INTERPOL_DOUBLE_TUBEVECTOR, "t"_a, "v"_a)
      .def("interpol", (const ibex::IntervalVector (TubeVector::*)(const ibex::Interval &,const TubeVector &) const)&TubeVector::interpol,
          DOCS_TUBEVECTOR_INTERPOL_INTERVAL_TUBEVECTOR, "t"_a, "v"_a)
      .def("invert", (const ibex::Interval (TubeVector::*)(const ibex::IntervalVector &,const ibex::Interval &) const)&TubeVector::invert,
          DOCS_TUBEVECTOR_INVERT_INTERVALVECTOR_INTERVAL, "y"_a, "search_domain"_a=ibex::Interval::ALL_REALS)
      .def("invert", (void (TubeVector::*)(const ibex::IntervalVector &,std::vector<ibex::Interval> &,const ibex::Interval &) const)&TubeVector::invert,
          DOCS_TUBEVECTOR_INVERT_INTERVALVECTOR_VECTOR_INTERVAL__INTERVAL, "y"_a, "v_t"_a, "search_domain"_a=ibex::Interval::ALL_REALS)
      .def("invert", (const ibex::Interval (TubeVector::*)(const ibex::IntervalVector &,const TubeVector &,const ibex::Interval &) const)&TubeVector::invert,
          DOCS_TUBEVECTOR_INVERT_INTERVALVECTOR_TUBEVECTOR_INTERVAL, "y"_a, "v"_a, "search_domain"_a=ibex::Interval::ALL_REALS)
      .def("invert", (void (TubeVector::*)(const ibex::IntervalVector &,std::vector<ibex::Interval> &,const TubeVector &,const ibex::Interval &) const)&TubeVector::invert,
          DOCS_TUBEVECTOR_INVERT_INTERVALVECTOR_VECTOR_INTERVAL__TUBEVECTOR_INTERVAL, "y"_a, "v_t"_a, "v"_a, "search_domain"_a=ibex::Interval::ALL_REALS)
      .def("max_diam", &TubeVector::max_diam,DOCS_TUBEVECTOR_MAX_DIAM)
      .def("diam", (const TrajectoryVector (TubeVector::*)(bool) const)&TubeVector::diam,
          DOCS_TUBEVECTOR_DIAM_BOOL, "gates_thicknesses"_a=false)
      .def("diam", (const TrajectoryVector (TubeVector::*)(const TubeVector &) const)&TubeVector::diam,
          DOCS_TUBEVECTOR_DIAM_TUBEVECTOR, "v"_a)
      .def("diag", (const Trajectory (TubeVector::*)(bool) const)&TubeVector::diag,
          DOCS_TUBEVECTOR_DIAG_BOOL, "gates_diag"_a=false)
      .def("diag", (const Trajectory (TubeVector::*)(int,int,bool) const)&TubeVector::diag,
          DOCS_TUBEVECTOR_DIAG_INT_INT_BOOL, "start_index"_a, "end_index"_a, "gates_diag"_a=false)
      .def("__eq__", [](TubeVector& s,const TubeVector & o) { return s == o;}, 
          DOCS_TUBEVECTOR_EQ_TUBEVECTOR)
      .def("__ne__", [](TubeVector& s,const TubeVector & o) { return s != o;}, 
          DOCS_TUBEVECTOR_NE_TUBEVECTOR)
      .def("is_subset", &TubeVector::is_subset,DOCS_TUBEVECTOR_IS_SUBSET_TUBEVECTOR, "x"_a)
      .def("is_strict_subset", &TubeVector::is_strict_subset,
          DOCS_TUBEVECTOR_IS_STRICT_SUBSET_TUBEVECTOR, "x"_a)
      .def("is_interior_subset", &TubeVector::is_interior_subset,
          DOCS_TUBEVECTOR_IS_INTERIOR_SUBSET_TUBEVECTOR, "x"_a)
      .def("is_strict_interior_subset", &TubeVector::is_strict_interior_subset,
          DOCS_TUBEVECTOR_IS_STRICT_INTERIOR_SUBSET_TUBEVECTOR, "x"_a)
      .def("is_superset", &TubeVector::is_superset,
          DOCS_TUBEVECTOR_IS_SUPERSET_TUBEVECTOR, "x"_a)
      .def("is_strict_superset", &TubeVector::is_strict_superset,
          DOCS_TUBEVECTOR_IS_STRICT_SUPERSET_TUBEVECTOR, "x"_a)
      .def("is_empty", &TubeVector::is_empty,DOCS_TUBEVECTOR_IS_EMPTY)
      .def("contains", &TubeVector::contains,DOCS_TUBEVECTOR_CONTAINS_TRAJECTORYVECTOR, "x"_a)
      .def("overlaps", &TubeVector::overlaps,
          DOCS_TUBEVECTOR_OVERLAPS_TUBEVECTOR_FLOAT, "x"_a, "ratio"_a=1)
      .def("set", (void (TubeVector::*)(const ibex::IntervalVector &) )&TubeVector::set,
          DOCS_TUBEVECTOR_SET_INTERVALVECTOR, "y"_a)
      .def("set", (void (TubeVector::*)(const ibex::IntervalVector &,int) )&TubeVector::set,
          DOCS_TUBEVECTOR_SET_INTERVALVECTOR_INT, "y"_a, "slice_id"_a)
      .def("set", (void (TubeVector::*)(const ibex::IntervalVector &,double) )&TubeVector::set,
          DOCS_TUBEVECTOR_SET_INTERVALVECTOR_DOUBLE, "y"_a, "t"_a)
      .def("set", (void (TubeVector::*)(const ibex::IntervalVector &,const ibex::Interval &) )&TubeVector::set,
          DOCS_TUBEVECTOR_SET_INTERVALVECTOR_INTERVAL, "y"_a, "t"_a)
      .def("set_empty", &TubeVector::set_empty,DOCS_TUBEVECTOR_SET_EMPTY)
      .def("inflate", (const TubeVector & (TubeVector::*)(double) )&TubeVector::inflate,
          DOCS_TUBEVECTOR_INFLATE_DOUBLE, "rad"_a)
      .def("inflate", (const TubeVector & (TubeVector::*)(const ibex::Vector &) )&TubeVector::inflate,
          DOCS_TUBEVECTOR_INFLATE_VECTOR, "rad"_a)
      .def("inflate", (const TubeVector & (TubeVector::*)(const TrajectoryVector &) )&TubeVector::inflate,
          DOCS_TUBEVECTOR_INFLATE_TRAJECTORYVECTOR, "rad"_a)
      .def("shift_tdomain", &TubeVector::shift_tdomain,
          DOCS_TUBEVECTOR_SHIFT_TDOMAIN_DOUBLE, "a"_a)
      .def("bisect", (const std::pair<TubeVector, TubeVector> (TubeVector::*)(double,float) const)&TubeVector::bisect,
          DOCS_TUBEVECTOR_BISECT_DOUBLE_FLOAT, "t"_a, "ratio"_a=0.49)
      .def("bisect", (const std::pair<TubeVector, TubeVector> (TubeVector::*)(double,int,float) const)&TubeVector::bisect,
          DOCS_TUBEVECTOR_BISECT_DOUBLE_INT_FLOAT, "t"_a, "dim"_a, "ratio"_a=0.49)
      .def("__iadd__", [](TubeVector& s,const ibex::Interval & o) { return s += o;}, 
          DOCS_TUBEVECTOR_IADD_INTERVAL)
      .def("__iadd__", [](TubeVector& s,const Trajectory & o) { return s += o;}, 
          DOCS_TUBEVECTOR_IADD_TRAJECTORY)
      .def("__iadd__", [](TubeVector& s,const Tube & o) { return s += o;}, 
          DOCS_TUBEVECTOR_IADD_TUBE)
      .def("__iadd__", [](TubeVector& s,const ibex::IntervalVector & o) { return s += o;}, 
          DOCS_TUBEVECTOR_IADD_INTERVALVECTOR)
      .def("__iadd__", [](TubeVector& s,const TrajectoryVector & o) { return s += o;}, 
          DOCS_TUBEVECTOR_IADD_TRAJECTORYVECTOR)
      .def("__iadd__", [](TubeVector& s,const TubeVector & o) { return s += o;}, 
          DOCS_TUBEVECTOR_IADD_TUBEVECTOR)
      .def("__isub__", [](TubeVector& s,const ibex::Interval & o) { return s -= o;}, 
          DOCS_TUBEVECTOR_ISUB_INTERVAL)
      .def("__isub__", [](TubeVector& s,const Trajectory & o) { return s -= o;}, 
          DOCS_TUBEVECTOR_ISUB_TRAJECTORY)
      .def("__isub__", [](TubeVector& s,const Tube & o) { return s -= o;}, 
          DOCS_TUBEVECTOR_ISUB_TUBE)
      .def("__isub__", [](TubeVector& s,const ibex::IntervalVector & o) { return s -= o;}, 
          DOCS_TUBEVECTOR_ISUB_INTERVALVECTOR)
      .def("__isub__", [](TubeVector& s,const TrajectoryVector & o) { return s -= o;}, 
          DOCS_TUBEVECTOR_ISUB_TRAJECTORYVECTOR)
      .def("__isub__", [](TubeVector& s,const TubeVector & o) { return s -= o;}, 
          DOCS_TUBEVECTOR_ISUB_TUBEVECTOR)
      .def("__imul__", [](TubeVector& s,const ibex::Interval & o) { return s *= o;}, 
          DOCS_TUBEVECTOR_IMUL_INTERVAL)
      .def("__imul__", [](TubeVector& s,const Trajectory & o) { return s *= o;}, 
          DOCS_TUBEVECTOR_IMUL_TRAJECTORY)
      .def("__imul__", [](TubeVector& s,const Tube & o) { return s *= o;}, 
          DOCS_TUBEVECTOR_IMUL_TUBE)
      .def("__itruediv__", [](TubeVector& s,const ibex::Interval & o) { return s /= o;}, 
          DOCS_TUBEVECTOR_ITRUEDIV_INTERVAL)
      .def("__itruediv__", [](TubeVector& s,const Trajectory & o) { return s /= o;}, 
          DOCS_TUBEVECTOR_ITRUEDIV_TRAJECTORY)
      .def("__itruediv__", [](TubeVector& s,const Tube & o) { return s /= o;}, 
          DOCS_TUBEVECTOR_ITRUEDIV_TUBE)
      .def("__ior__", [](TubeVector& s,const ibex::IntervalVector & o) { return s |= o;}, 
          DOCS_TUBEVECTOR_IOR_INTERVALVECTOR)
      .def("__ior__", [](TubeVector& s,const TrajectoryVector & o) { return s |= o;}, 
          DOCS_TUBEVECTOR_IOR_TRAJECTORYVECTOR)
      .def("__ior__", [](TubeVector& s,const TubeVector & o) { return s |= o;}, 
          DOCS_TUBEVECTOR_IOR_TUBEVECTOR)
      .def("__iand__", [](TubeVector& s,const ibex::IntervalVector & o) { return s &= o;}, 
          DOCS_TUBEVECTOR_IAND_INTERVALVECTOR)
      .def("__iand__", [](TubeVector& s,const TrajectoryVector & o) { return s &= o;}, 
          DOCS_TUBEVECTOR_IAND_TRAJECTORYVECTOR)
      .def("__iand__", [](TubeVector& s,const TubeVector & o) { return s &= o;}, 
          DOCS_TUBEVECTOR_IAND_TUBEVECTOR)
      .def("class_name", &TubeVector::class_name,DOCS_TUBEVECTOR_CLASS_NAME)
    
      .def("enable_synthesis", &TubeVector::enable_synthesis,
          DOCS_TUBEVECTOR_ENABLE_SYNTHESIS_BOOL, "enable"_a=true)
      .def("integral", (const ibex::IntervalVector (TubeVector::*)(double) const)&TubeVector::integral,
          DOCS_TUBEVECTOR_INTEGRAL_DOUBLE, "t"_a)
      .def("integral", (const ibex::IntervalVector (TubeVector::*)(const ibex::Interval &) const)&TubeVector::integral,
          DOCS_TUBEVECTOR_INTEGRAL_INTERVAL, "t"_a)
      .def("integral", (const ibex::IntervalVector (TubeVector::*)(const ibex::Interval &,const ibex::Interval &) const)&TubeVector::integral,
          DOCS_TUBEVECTOR_INTEGRAL_INTERVAL_INTERVAL, "t1"_a, "t2"_a)
      .def("partial_integral", (const std::pair<ibex::IntervalVector, ibex::IntervalVector> (TubeVector::*)(const ibex::Interval &) const)&TubeVector::partial_integral,
          DOCS_TUBEVECTOR_PARTIAL_INTEGRAL_INTERVAL, "t"_a)
      .def("partial_integral", (const std::pair<ibex::IntervalVector, ibex::IntervalVector> (TubeVector::*)(const ibex::Interval &,const ibex::Interval &) const)&TubeVector::partial_integral,
          DOCS_TUBEVECTOR_PARTIAL_INTEGRAL_INTERVAL_INTERVAL, "t1"_a, "t2"_a)
      .def("serialize", (void (TubeVector::*)(const std::string &,int) const)&TubeVector::serialize,
          DOCS_TUBEVECTOR_SERIALIZE_STRING_INT, "binary_file_name"_a="x.tube", "version_number"_a=SERIALIZATION_VERSION)
      .def("serialize", (void (TubeVector::*)(const std::string &,const TrajectoryVector &,int) const)&TubeVector::serialize,
          DOCS_TUBEVECTOR_SERIALIZE_STRING_TRAJECTORYVECTOR_INT, "binary_file_name"_a, "traj"_a, "version_number"_a=SERIALIZATION_VERSION)
    //   .def_static("same_slicing", (bool (TubeVector::*)(const TubeVector &,const Tube &) )&TubeVector::same_slicing,
        //   DOCS_TUBEVECTOR_SAME_SLICING_TUBEVECTOR_TUBE, "x1"_a, "x2"_a)
    //   .def_static("same_slicing", (bool (TubeVector::*)(const TubeVector &,const TubeVector &) )&TubeVector::same_slicing,
        //   DOCS_TUBEVECTOR_SAME_SLICING_TUBEVECTOR_TUBEVECTOR, "x1"_a, "x2"_a)
      .def_static("hull", &TubeVector::hull,DOCS_TUBEVECTOR_HULL_LIST_TUBEVECTOR_, "l_tubes"_a)
    
        // allow [] operator
      .def("__len__", &TubeVector::size )
      .def("__getitem__", [](TubeVector& s, size_t index) -> Tube&{
              if (index >= static_cast<size_t>(s.size())){
                  throw py::index_error();
              }
                return s[static_cast<int>(index)];
          }, DOCS_TUBEVECTOR_OPERATOR_INDEX_INT, py::return_value_policy::reference_internal)

      .def("__getitem__", [](const TubeVector& s, py::slice slice) -> TubeVector {
            size_t start, stop, step, slicelength;
            if (!slice.compute(s.size(), &start, &stop, &step, &slicelength))
                throw py::error_already_set();
            if (step != 1){
                std::cout << "Warning slice step must be equal to 1\n";
            }
            
            // to respect the python convention, the stop index 
            // is not included in slice
            return s.subvector(start, start + slicelength-1);
        })
      .def("__setitem__", [](TubeVector& s, size_t index, Tube& t){
              if (index >= static_cast<size_t>(s.size())){
                throw py::index_error();
              }
              s[static_cast<int>(index)] = t;
          }, DOCS_TUBEVECTOR_OPERATOR_INDEX_INT1)

        // .def("__add__",[](const TubeVector& x){return x+y;});
        .def("__add__",[](const TubeVector& x, const TubeVector& y){return x+y;})
        .def("__add__",[](const TubeVector& x, const ibex::IntervalVector& y){return x+y;})
        .def("__radd__",[](const TubeVector& x, const ibex::IntervalVector& y){return y+x;})
        // .def("__add__",[](const ibex::IntervalVector& x, const TubeVector& y){return x+y;})
        // .def("__add__",[](const TubeVector& x, const TrajectoryVector& y){return x+y;})
        // .def("__add__",[](const TrajectoryVector& x, const TubeVector& y){return x+y;})

        .def("__neg__", [](const TubeVector& x){return -x;})
        .def("__sub__", [](const TubeVector& x, const TubeVector& y){return x-y;})
        .def("__sub__", [](const TubeVector& x, const ibex::IntervalVector& y){return x-y;})
        .def("__rsub__", [](const TubeVector& x, const ibex::IntervalVector& y){return y-x;})
        // .def("__sub__", [](const ibex::IntervalVector& x, const TubeVector& y){return x-y;})
        // .def("__sub__", [](const TubeVector& x, const TrajectoryVector& y){return x-y;})
        // .def("__sub__", [](const TrajectoryVector& x, const TubeVector& y){return x-y;})

        // .def("__mul__", [](const TubeVector& y, const Tube& x){return x*y;})
        // .def("__rmul__", [](const TubeVector& y, const Tube& x){return x*y;})
        // .def("__mul__", [](const ibex::Interval& x, const TubeVector& y){return x*y;})
        .def("__rmul__", [](const TubeVector& y, const ibex::Interval& x){return x*y;})
        // .def("__mul__", [](const Trajectory& x, const TubeVector& y){return x*y})

        .def("__truediv__", [](const TubeVector& x, const Tube& y){return x/y;})
        .def("__truediv__", [](const TubeVector& x, const ibex::Interval& y){return x/y;})
        .def("__truediv__", [](const ibex::IntervalVector& x, const Tube& y){return x/y;})
        // .def("__truediv__", [](const TubeVector& x, const Trajectory& y){return x/y;})

        .def("__or__", [](const TubeVector& x, const TubeVector& y){return x|y;})
        .def("__or__", [](const TubeVector& x, const ibex::IntervalVector& y){return x|y;})
        .def("__ror__", [](const TubeVector& x, const ibex::IntervalVector& y){return y|x;})
        // .def("__or__", [](const ibex::IntervalVector& x, const TubeVector& y){return x|y;})
        // .def("__or__", [](const TubeVector& x, const TrajectoryVector& y){return x|y;})
        // .def("__or__", [](const TrajectoryVector& x, const TubeVector& y){return x|y;})

        .def("__and__",[](const TubeVector& x, const TubeVector& y){return x&y;})
        .def("__and__",[](const TubeVector& x, const ibex::IntervalVector& y){return x&y;})
        .def("__rand__",[](const TubeVector& x, const ibex::IntervalVector& y){return y&x;})
        // .def("__and__",[](const ibex::IntervalVector& x, const TubeVector& y){return x&y;})
        // .def("__and__",[](const TubeVector& x, const TrajectoryVector& y){return x&y;})
        // .def("__and__",[](const TrajectoryVector& x, const TubeVector& y){return x&y;})
    ;

}

