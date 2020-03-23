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

#include "tubex_TubeVector.h"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <pybind11/functional.h>

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
      .def(py::init< const ibex::Interval&,int>(), "domain"_a, "n"_a=1)
      .def(py::init< const ibex::Interval&, const ibex::IntervalVector&>(), "domain"_a, "codomain"_a)
      .def(py::init< const ibex::Interval&,double,int>(), "domain"_a, "timestep"_a, "n"_a=1)
      .def(py::init< const ibex::Interval&,double, const ibex::IntervalVector&>(), "domain"_a, "timestep"_a, "codomain"_a)
      // .def(py::init< const ibex::Interval&,double, const tubex::Fnc&>(), "domain"_a, "timestep"_a, "f"_a)
      .def(py::init(
          [](const ibex::Interval& domain,double timestep, const tubex::Function& f){
              return new TubeVector(domain, timestep, f );
          }), "domain"_a, "timestep"_a, "f"_a, py::keep_alive<1,4>())

      .def(py::init< const tubex::TubeVector&>(), "x"_a)
      .def(py::init< const tubex::TubeVector&, const ibex::IntervalVector&>(), "x"_a, "codomain"_a)
      .def(py::init<int, const tubex::Tube&>(), "n"_a, "x"_a)
      .def(py::init< const TrajectoryVector&,double>(), "traj"_a, "timestep"_a=0)
      .def(py::init< const TrajectoryVector&, const TrajectoryVector&,double>(), "lb"_a, "ub"_a, "timestep"_a=0)
      .def(py::init< const std::vector<ibex::Interval>&, const std::vector<ibex::IntervalVector>&>(), "v_domains"_a, "v_codomains"_a)
      .def(py::init< const std::string&>(), "binary_file_name"_a)
      // .def(py::init< const std::string&,TrajectoryVector&*>(), "binary_file_name"_a, "traj"_a)

   
      // .def("__eq__", [](TubeVector& s, const tubex::TubeVector& o) { s r= o;})
      // .def("__call__", [](TubeVector& s,int o) { s [] o;})
      // .def("__call__", [](TubeVector& s,int o) { s [] o;})

      // allow [] operator
      .def("__len__", &TubeVector::size )
      .def("__getitem__", [](TubeVector& s, size_t index){
              if (index >= s.size()){
                  throw py::index_error();
              }
                return s[static_cast<int>(index)];
          }, py::return_value_policy::reference_internal)
      .def("__setitem__", [](TubeVector& s, size_t index, Tube& t){
              if (index >= s.size()){
                throw py::index_error();
              }
              s[static_cast<int>(index)] = t;
          })

     .def("__call__", [](TubeVector& s,int o) { return s(o);})
      .def("__call__", [](TubeVector& s,double o) { return s(o);})
      .def("__call__", [](TubeVector& s, const ibex::Interval& o) { return s(o);})
      .def("__eq__", [](TubeVector& s, const tubex::TubeVector& o) { return s == o;})
      .def("__ne__", [](TubeVector& s, const tubex::TubeVector& o) { return s != o;})
      .def("__iadd__", [](TubeVector& s, const ibex::Interval& o) { return s += o;})
      // .def("__iadd__", [](TubeVector& s, const tubex::Trajectory& o) { return s += o;})
      .def("__iadd__", [](TubeVector& s, const tubex::Tube& o) { return s += o;})
      .def("__iadd__", [](TubeVector& s, const ibex::IntervalVector& o) { return s += o;})
      // .def("__iadd__", [](TubeVector& s, const TrajectoryVector& o) { return s += o;})
      .def("__iadd__", [](TubeVector& s, const tubex::TubeVector& o) { return s += o;})
      .def("__isub__", [](TubeVector& s, const ibex::Interval& o) { return s -= o;})
      // .def("__isub__", [](TubeVector& s, const tubex::Trajectory& o) { return s -= o;})
      .def("__isub__", [](TubeVector& s, const tubex::Tube& o) { return s -= o;})
      .def("__isub__", [](TubeVector& s, const ibex::IntervalVector& o) { return s -= o;})
      // .def("__isub__", [](TubeVector& s, const TrajectoryVector& o) { return s -= o;})
      .def("__isub__", [](TubeVector& s, const tubex::TubeVector& o) { return s -= o;})
      .def("__imul__", [](TubeVector& s, const ibex::Interval& o) { return s *= o;})
      // .def("__imul__", [](TubeVector& s, const tubex::Trajectory& o) { return s *= o;})
      .def("__imul__", [](TubeVector& s, const tubex::Tube& o) { return s *= o;})
      .def("__itruediv__", [](TubeVector& s, const ibex::Interval& o) { return s /= o;})
      // .def("__itruediv__", [](TubeVector& s, const tubex::Trajectory& o) { return s /= o;})
      .def("__itruediv__", [](TubeVector& s, const tubex::Tube& o) { return s /= o;})
      .def("__ior__", [](TubeVector& s, const ibex::IntervalVector& o) { return s |= o;})
      // .def("__ior__", [](TubeVector& s, const TrajectoryVector& o) { return s |= o;})
      .def("__ior__", [](TubeVector& s, const tubex::TubeVector& o) { return s |= o;})
      .def("__iand__", [](TubeVector& s, const ibex::IntervalVector& o) { return s &= o;})
      // .def("__iand__", [](TubeVector& s, const TrajectoryVector& o) { return s &= o;})
      .def("__iand__", [](TubeVector& s, const tubex::TubeVector& o) { return s &= o;})


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
      .def("__mul__", [](const Tube& x, const ibex::IntervalVector& y){return x*y;})
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

      // .def(py::init<std::string,TrajectoryVector>(), "binary_file_name"_a, "traj"_a)
      .def("size", &TubeVector::size)
      .def("resize", &TubeVector::resize, "n"_a)
      .def("subvector", &TubeVector::subvector, "start_index"_a, "end_index"_a)
      .def("put", &TubeVector::put, "start_index"_a, "subvec"_a)
      .def("primitive", (const TubeVector (TubeVector::*)() const)&TubeVector::primitive)
      .def("primitive", (const TubeVector (TubeVector::*)( const ibex::IntervalVector&) const)&TubeVector::primitive, "c"_a)
      .def("domain", &TubeVector::domain)
      .def("nb_slices", &TubeVector::nb_slices)
      .def("input2index", &TubeVector::input2index, "t"_a)
      .def("sample", (void (TubeVector::*)(double) )&TubeVector::sample, "t"_a)
      .def("sample", (void (TubeVector::*)(double, const ibex::IntervalVector&) )&TubeVector::sample, "t"_a, "gate"_a)
      .def("sample", (void (TubeVector::*)( const tubex::Tube&) )&TubeVector::sample, "x"_a)
      .def("sample", (void (TubeVector::*)( const tubex::TubeVector&) )&TubeVector::sample, "x"_a)
      .def("codomain", &TubeVector::codomain)
      .def("volume", &TubeVector::volume)
      .def("eval", &TubeVector::eval, "t"_a=ibex::Interval::ALL_REALS)
      .def("interpol", (const ibex::IntervalVector (TubeVector::*)(double, const tubex::TubeVector&) const)&TubeVector::interpol, "t"_a, "v"_a)
      .def("interpol", (const ibex::IntervalVector (TubeVector::*)( const ibex::Interval&, const tubex::TubeVector&) const)&TubeVector::interpol, "t"_a, "v"_a)
      .def("invert", (const ibex::Interval (TubeVector::*)( const ibex::IntervalVector&, const ibex::Interval&) const)&TubeVector::invert, "y"_a, "search_domain"_a=ibex::Interval::ALL_REALS)
      .def("invert", (void (TubeVector::*)( const ibex::IntervalVector&,std::vector<ibex::Interval>&, const ibex::Interval&) const)&TubeVector::invert, "y"_a, "v_t"_a, "search_domain"_a=ibex::Interval::ALL_REALS)
      .def("invert", (const ibex::Interval (TubeVector::*)( const ibex::IntervalVector&, const tubex::TubeVector&, const ibex::Interval&) const)&TubeVector::invert, "y"_a, "v"_a, "search_domain"_a=ibex::Interval::ALL_REALS)
      .def("invert", (void (TubeVector::*)( const ibex::IntervalVector&,std::vector<ibex::Interval>&, const tubex::TubeVector&, const ibex::Interval&) const)&TubeVector::invert, "y"_a, "v_t"_a, "v"_a, "search_domain"_a=ibex::Interval::ALL_REALS)
      .def("max_diam", &TubeVector::max_diam)
      .def("diam", (const TrajectoryVector (TubeVector::*)(bool) const)&TubeVector::diam, "gates_thicknesses"_a=false)
      .def("diam", (const TrajectoryVector (TubeVector::*)( const tubex::TubeVector&) const)&TubeVector::diam, "v"_a)
      .def("diag", (const Trajectory (TubeVector::*)(bool) const)&TubeVector::diag, "gates_diag"_a=false)
      .def("diag", (const Trajectory (TubeVector::*)(int,int,bool) const)&TubeVector::diag, "start_index"_a, "end_index"_a, "gates_diag"_a=false)
      .def("is_subset", &TubeVector::is_subset, "x"_a)
      .def("is_strict_subset", &TubeVector::is_strict_subset, "x"_a)
      .def("is_interior_subset", &TubeVector::is_interior_subset, "x"_a)
      .def("is_strict_interior_subset", &TubeVector::is_strict_interior_subset, "x"_a)
      .def("is_superset", &TubeVector::is_superset, "x"_a)
      .def("is_strict_superset", &TubeVector::is_strict_superset, "x"_a)
      .def("is_empty", &TubeVector::is_empty)
      .def("contains", &TubeVector::contains, "x"_a)
      .def("overlaps", &TubeVector::overlaps, "x"_a, "ratio"_a=1)
      .def("set", (void (TubeVector::*)( const ibex::IntervalVector&) )&TubeVector::set, "y"_a)
      .def("set", (void (TubeVector::*)( const ibex::IntervalVector&,int) )&TubeVector::set, "y"_a, "slice_id"_a)
      .def("set", (void (TubeVector::*)( const ibex::IntervalVector&,double) )&TubeVector::set, "y"_a, "t"_a)
      .def("set", (void (TubeVector::*)( const ibex::IntervalVector&, const ibex::Interval&) )&TubeVector::set, "y"_a, "t"_a)
      .def("set_empty", &TubeVector::set_empty)
      .def("inflate", (const TubeVector & (TubeVector::*)(double) )&TubeVector::inflate, "rad"_a)
      .def("inflate", (const TubeVector & (TubeVector::*)( const ibex::Vector&) )&TubeVector::inflate, "rad"_a)
      .def("inflate", (const TubeVector & (TubeVector::*)( const TrajectoryVector&) )&TubeVector::inflate, "rad"_a)
      .def("shift_domain", &TubeVector::shift_domain, "a"_a)
      .def("bisect", (const std::pair<TubeVector, TubeVector> (TubeVector::*)(double,float) const)&TubeVector::bisect, "t"_a, "ratio"_a=0.49)
      .def("bisect", (const std::pair<TubeVector, TubeVector> (TubeVector::*)(double,int,float) const)&TubeVector::bisect, "t"_a, "dim"_a, "ratio"_a=0.49)
      // .def("class_name", &TubeVector::class_name)
      .def("enable_synthesis", &TubeVector::enable_synthesis, "enable"_a=true)
      .def("integral", (const ibex::IntervalVector (TubeVector::*)(double) const)&TubeVector::integral, "t"_a)
      .def("integral", (const ibex::IntervalVector (TubeVector::*)( const ibex::Interval&) const)&TubeVector::integral, "t"_a)
      .def("integral", (const ibex::IntervalVector (TubeVector::*)( const ibex::Interval&, const ibex::Interval&) const)&TubeVector::integral, "t1"_a, "t2"_a)
      .def("partial_integral", (const std::pair<ibex::IntervalVector, ibex::IntervalVector> (TubeVector::*)( const ibex::Interval&) const)&TubeVector::partial_integral, "t"_a)
      .def("partial_integral", (const std::pair<ibex::IntervalVector, ibex::IntervalVector> (TubeVector::*)( const ibex::Interval&, const ibex::Interval&) const)&TubeVector::partial_integral, "t1"_a, "t2"_a)
      .def("serialize", (void (TubeVector::*)( const std::string&,int) const)&TubeVector::serialize, "binary_file_name"_a="x.tube", "version_number"_a=SERIALIZATION_VERSION)
      .def("serialize", (void (TubeVector::*)( const std::string&, const TrajectoryVector&,int) const)&TubeVector::serialize, "binary_file_name"_a, "traj"_a, "version_number"_a=SERIALIZATION_VERSION)
      // .def_static("same_slicing", (bool (TubeVector::*)(const tubex::TubeVector&, const tubex::Tube&) )&TubeVector::same_slicing)
      // m.def("same_slicing", (bool (TubeVector::*)( const tubex::TubeVector&, const tubex::TubeVector&) )&TubeVector::same_slicing, "x1"_a, "x2"_a)
      .def_static("hull", &TubeVector::hull, "l_tubes"_a);
    ;


     /// @}
  /// \name Vector outputs
  /// @{

    // m.def("__add__",const TubeVector operator+(const TubeVector& x);
    // m.def("__add__",const TubeVector operator+(const TubeVector& x, const TubeVector& y);
    // m.def("__add__",const TubeVector operator+(const TubeVector& x, const ibex::IntervalVector& y);
    // m.def("__add__",const TubeVector operator+(const ibex::IntervalVector& x, const TubeVector& y);
    // m.def("__add__",const TubeVector operator+(const TubeVector& x, const TrajectoryVector& y);
    // m.def("__add__",const TubeVector operator+(const TrajectoryVector& x, const TubeVector& y);

    // const TubeVector operator-(const TubeVector& x);
    // const TubeVector operator-(const TubeVector& x, const TubeVector& y);
    // const TubeVector operator-(const TubeVector& x, const ibex::IntervalVector& y);
    // const TubeVector operator-(const ibex::IntervalVector& x, const TubeVector& y);
    // const TubeVector operator-(const TubeVector& x, const TrajectoryVector& y);
    // const TubeVector operator-(const TrajectoryVector& x, const TubeVector& y);

    // const TubeVector operator*(const Tube& x, const TubeVector& y);
    // const TubeVector operator*(const ibex::Interval& x, const TubeVector& y);
    // const TubeVector operator*(const Tube& x, const ibex::IntervalVector& y);
    // const TubeVector operator*(const Trajectory& x, const TubeVector& y);

    // const TubeVector operator/(const TubeVector& x, const Tube& y);
    // const TubeVector operator/(const TubeVector& x, const ibex::Interval& y);
    // const TubeVector operator/(const ibex::IntervalVector& x, const Tube& y);
    // const TubeVector operator/(const TubeVector& x, const Trajectory& y);

    // const TubeVector operator|(const TubeVector& x, const TubeVector& y);
    // const TubeVector operator|(const TubeVector& x, const ibex::IntervalVector& y);
    // const TubeVector operator|(const ibex::IntervalVector& x, const TubeVector& y);
    // const TubeVector operator|(const TubeVector& x, const TrajectoryVector& y);
    // const TubeVector operator|(const TrajectoryVector& x, const TubeVector& y);

    // const TubeVector operator&(const TubeVector& x, const TubeVector& y);
    // const TubeVector operator&(const TubeVector& x, const ibex::IntervalVector& y);
    // const TubeVector operator&(const ibex::IntervalVector& x, const TubeVector& y);
    // const TubeVector operator&(const TubeVector& x, const TrajectoryVector& y);
    // const TubeVector operator&(const TrajectoryVector& x, const TubeVector& y);

}

