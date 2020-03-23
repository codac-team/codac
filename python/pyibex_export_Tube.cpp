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

#include "tubex_Tube.h"
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



void export_Tube(py::module& m){

    py::class_<Tube> tube(m, "Tube");
    tube

      // ************** CONSTRUCTORS **********************

      .def(py::init< const tubex::Tube&>(), "x"_a, py::keep_alive<1,2>())
      .def(py::init< const ibex::Interval&, const ibex::Interval&>(), "domain"_a, "codomain"_a=ibex::Interval::ALL_REALS)
      .def(py::init< const ibex::Interval&,double, const ibex::Interval&>(), "domain"_a, "timestep"_a, "codomain"_a=ibex::Interval::ALL_REALS)
      .def(py::init< const ibex::Interval&,double, const tubex::Fnc&,int>(), "domain"_a, "timestep"_a, "f"_a, "f_image_id"_a=0)
      .def(py::init< const tubex::Tube&, const ibex::Interval&>(), "x"_a.noconvert(), "codomain"_a.noconvert(), py::keep_alive<1,2>())
      .def(py::init< const std::vector<ibex::Interval>&, const std::vector<ibex::Interval>&>(), "v_domains"_a.noconvert(), "v_codomains"_a.noconvert())
      // .def(py::init< const tubex::Tube&, const tubex::Fnc&,int>(), "x"_a, "f"_a, "f_image_id"_a=0)
      .def(py::init< const tubex::Trajectory&,double>(), "traj"_a, "timestep"_a)
      .def(py::init< const tubex::Trajectory&, const tubex::Trajectory&,double>(), "lb"_a, "ub"_a, "timestep"_a)
      .def(py::init< const std::string&>(), "binary_file_name"_a)
      // .def(py::init< const std::string&,tubex::Trajectory&*>(), "binary_file_name"_a, "traj"_a)


      // .def(py::init<ibex::Interval,ibex::Interval>(), "domain"_a, "codomain"_a=ibex::Interval::ALL_REALS)
      // .def(py::init<ibex::Interval,double,ibex::Interval>(), "domain"_a, "timestep"_a, "codomain"_a=ibex::Interval::ALL_REALS)
      // // .def(py::init<ibex::Interval,double,tubex::Fnc,int>(), "domain"_a, "timestep"_a, "f"_a, "f_image_id"_a=0)
      // .def(py::init(
      //     [](ibex::Interval domain, double timestep, tubex::Function &f,int f_image_id){
      //         return new Tube(domain, timestep, f, f_image_id );
      //     }), "domain"_a, "timestep"_a, "f"_a, "f_image_id"_a=0, py::keep_alive<1,4>())
      
      // .def(py::init(
      //     [](tubex::Tube &x,tubex::Function &f,int f_image_id){
      //       return new Tube(x,f,f_image_id);
      //     }), "x"_a, "f"_a, "f_image_id"_a=0)

      // .def(py::init<std::vector<ibex::Interval>,std::vector<ibex::Interval>>(), "v_domains"_a, "v_codomains"_a)
      // .def(py::init<tubex::Tube>(), "x"_a)
      // .def(py::init<tubex::Tube,ibex::Interval>(), "x"_a, "codomain"_a)
      // // .def(py::init<tubex::Tube,tubex::Fnc,int>(), "x"_a, "f"_a, "f_image_id"_a=0)
      // .def(py::init(
      //     [](tubex::Tube &x,tubex::Function &f,int f_image_id){
      //       return new Tube(x,f,f_image_id);
      //     }), "x"_a, "f"_a, "f_image_id"_a=0)

      // .def(py::init<tubex::Trajectory,double>(), "traj"_a, "timestep"_a)
      // .def(py::init<tubex::Trajectory,tubex::Trajectory,double>(), "lb"_a, "ub"_a, "timestep"_a)
      // .def(py::init<std::string>(), "binary_file_name"_a)
      // .def(py::init<const std::string&, tubex::Trajectory*&>(), "binary_file_name"_a, "traj"_a)

      // ************** OPERATORS **********************
      .def("__call__", [](Tube& s,int o) { return s(o);})
      .def("__call__", [](Tube& s,double o) { return s(o);})
      .def("__call__", [](Tube& s, const ibex::Interval& o) { return s(o);})
      .def("__eq__", [](Tube& s, const tubex::Tube& o) { return s == o;})
      .def("__ne__", [](Tube& s, const tubex::Tube& o) { return s != o;})
      .def("__iadd__", [](Tube& s, const ibex::Interval& o) { return s += o;})
      .def("__iadd__", [](Tube& s, const tubex::Trajectory& o) { return s += o;})
      .def("__iadd__", [](Tube& s, const tubex::Tube& o) { return s += o;})
      .def("__isub__", [](Tube& s, const ibex::Interval& o) { return s -= o;})
      .def("__isub__", [](Tube& s, const tubex::Trajectory& o) { return s -= o;})
      .def("__isub__", [](Tube& s, const tubex::Tube& o) { return s -= o;})
      .def("__imul__", [](Tube& s, const ibex::Interval& o) { return s *= o;})
      .def("__imul__", [](Tube& s, const tubex::Trajectory& o) { return s *= o;})
      .def("__imul__", [](Tube& s, const tubex::Tube& o) { return s *= o;})
      .def("__itruediv__", [](Tube& s, const ibex::Interval& o) { return s /= o;})
      .def("__itruediv__", [](Tube& s, const tubex::Trajectory& o) { return s /= o;})
      .def("__itruediv__", [](Tube& s, const tubex::Tube& o) { return s /= o;})
      .def("__ior__", [](Tube& s, const ibex::Interval& o) { return s |= o;})
      .def("__ior__", [](Tube& s, const tubex::Trajectory& o) { return s |= o;})
      .def("__ior__", [](Tube& s, const tubex::Tube& o) { return s |= o;})
      .def("__iand__", [](Tube& s, const ibex::Interval& o) { return s &= o;})
      .def("__iand__", [](Tube& s, const tubex::Trajectory& o) { return s &= o;})
      .def("__iand__", [](Tube& s, const tubex::Tube& o) { return s &= o;})
      // .def("__repr__", [](Tube& s,std::ostream&, const tubex::Tube& o) { s << o;});

      // .def("__add__", [](const Tube& x){return +x;})
      .def("__add__", [](const Tube& x, const Tube& y){return x+y;})
      .def("__add__", [](const Tube& x, const ibex::Interval& y){return x+y;})
      .def("__radd__", [](const Tube& x, const ibex::Interval& y){return x+y;})
      // .def("__add__", [](const Tube& x, const Trajectory& y){return x+y;})
      // .def("__add__", [](const Trajectory& x, const Tube& y){return x+y;})

      .def("__neg__", [](const Tube& x){return -x;})
      .def("__sub__", [](const Tube& x, const Tube& y){return x-y;})
      .def("__sub__", [](const Tube& x, const ibex::Interval& y){return x-y;})
      .def("__rsub__", [](const Tube& x, const ibex::Interval& y){return y-x;})
      // .def("__rsub__", [](const ibex::Interval& x, const Tube& y){return x-y;})
      // .def("__sub__", [](const Tube& x, const Trajectory& y){return x-y;})
      // .def("__sub__", [](const Trajectory& x, const Tube& y){return x-y;})

      .def("__mul__", [](const Tube& x, const Tube& y){return x*y;}, "y"_a.noconvert()) //, "y"_a.noconvert())
      .def("__mul__", [](const Tube& x, const ibex::Interval& y){return x*y;}, "y"_a.noconvert())//, "y"_a.noconvert())
      .def("__mul__", [](const Tube& x, const ibex::IntervalVector& y){return x*y;}, "y"_a.noconvert())//, "y"_a.noconvert())
      .def("__mul__", [](const Tube& x, const TubeVector& y){return x*y;}, "y"_a.noconvert())//, "y"_a.noconvert())
      .def("__rmul__", [](const Tube& x, const ibex::Interval& y){return x*y;}, "y"_a.noconvert())//, "y"_a.noconvert())

      // .def("__mul__", [](const ibex::Interval& x, const Tube& y){return x*y;})
      // .def("__mul__", [](const Tube& x, const Trajectory& y){return x*y;})
      // .def("__mul__", [](const Trajectory& x, const Tube& y){return x*y;})

      .def("__truediv__", [](const Tube& x, const Tube& y){return x/y;}) //, "x"_a.noconvert(), "y"_a.noconvert())
      .def("__truediv__", [](const Tube& x, const ibex::Interval& y){return x/y;}) //, "x"_a.noconvert(), "y"_a.noconvert())
      .def("__rtruediv__", [](const Tube& x, const ibex::Interval& y){return y/x;}) //, "x"_a.noconvert(), "y"_a.noconvert())
      .def("__rtruediv__", [](const Tube& x, const ibex::IntervalVector& y){return y/x;}) //, "x"_a.noconvert(), "y"_a.noconvert())
      // .def("__truediv__", [](const ibex::Interval& x, const Tube& y){return x/y;})
      // .def("__truediv__", [](const Tube& x, const Trajectory& y){return x/y;})
      // .def("__truediv__", [](const Trajectory& x, const Tube& y){return x/y;})

      .def("__or__", [](const Tube& x, const Tube& y){return x|y;})
      .def("__or__", [](const Tube& x, const ibex::Interval& y){return x|y;})
      .def("__ror__", [](const Tube& x, const ibex::Interval& y){return x|y;})
      // .def("__or__", [](const ibex::Interval& x, const Tube& y){return x|y;})
      // .def("__or__", [](const Tube& x, const Trajectory& y){return x|y;})
      // .def("__or__", [](const Trajectory& x, const Tube& y){return x|y;})

      .def("__and__",[](const Tube& x, const Tube& y){ return x & y;})
      .def("__and__",[](const Tube& x, const ibex::Interval& y){ return x & y;})
      .def("__rand__",[](const Tube& x, const ibex::Interval& y){ return x & y;})
      // .def("__and__",[](const ibex::Interval& x, const Tube& y){ return x & y;})
      // .def("__and__",[](const Tube& x, const Trajectory& y){ return x & y;})
      // .def("__and__",[](const Trajectory& x, const Tube& y){ return x & y;})

      // ************** OTHER FUNCTIONS **********************

      .def("size", &Tube::size)
      .def("primitive", &Tube::primitive, "c"_a=ibex::Interval(0))
      .def("domain", &Tube::domain)
      .def("polygon_envelope", &Tube::polygon_envelope)
      .def("nb_slices", &Tube::nb_slices)
      .def("slice", (Slice * (Tube::*)(int) )&Tube::slice, "slice_id"_a, py::return_value_policy::reference_internal)
      .def("slice", (const Slice * (Tube::*)(int) const)&Tube::slice, "slice_id"_a)
      .def("slice", (Slice * (Tube::*)(double) )&Tube::slice, "t"_a, py::return_value_policy::reference_internal)
      .def("slice", (const Slice * (Tube::*)(double) const)&Tube::slice, "t"_a)
      .def("first_slice", (Slice * (Tube::*)() )&Tube::first_slice)
      .def("first_slice", (const Slice * (Tube::*)() const)&Tube::first_slice)
      .def("last_slice", (Slice * (Tube::*)() )&Tube::last_slice)
      .def("last_slice", (const Slice * (Tube::*)() const)&Tube::last_slice)
      .def("wider_slice", (Slice * (Tube::*)() )&Tube::wider_slice)
      .def("wider_slice", (const Slice * (Tube::*)() const)&Tube::wider_slice)
      .def("largest_slice", (Slice * (Tube::*)() )&Tube::largest_slice)
      .def("largest_slice", (const Slice * (Tube::*)() const)&Tube::largest_slice)
      .def("slice_domain", &Tube::slice_domain, "slice_id"_a)
      .def("input2index", &Tube::input2index, "t"_a)
      .def("index", &Tube::index, "slice"_a)
      .def("sample", (void (Tube::*)(double) )&Tube::sample, "t"_a)
      .def("sample", (void (Tube::*)(double,tubex::Slice*) )&Tube::sample, "t"_a, "slice_to_be_sampled"_a)
      .def("sample", (void (Tube::*)(double, const ibex::Interval&) )&Tube::sample, "t"_a, "gate"_a)
      .def("sample", (void (Tube::*)( const tubex::Tube&) )&Tube::sample, "x"_a)
      .def("gate_exists", &Tube::gate_exists, "t"_a)
      .def("codomain", &Tube::codomain)
      .def("volume", &Tube::volume)
      .def("eval", &Tube::eval, "t"_a=ibex::Interval::ALL_REALS)
      .def("interpol", (const ibex::Interval (Tube::*)(double, const tubex::Tube&) const)&Tube::interpol, "t"_a, "v"_a)
      .def("interpol", (const ibex::Interval (Tube::*)( const ibex::Interval&, const tubex::Tube&) const)&Tube::interpol, "t"_a, "v"_a)
      .def("invert", (const ibex::Interval (Tube::*)( const ibex::Interval&, const ibex::Interval&) const)&Tube::invert, "y"_a, "search_domain"_a=ibex::Interval::ALL_REALS)
      .def("invert", (void (Tube::*)( const ibex::Interval&,std::vector<ibex::Interval>&, const ibex::Interval&) const)&Tube::invert, "y"_a, "v_t"_a, "search_domain"_a=ibex::Interval::ALL_REALS)
      .def("invert", (const ibex::Interval (Tube::*)( const ibex::Interval&, const tubex::Tube&, const ibex::Interval&) const)&Tube::invert, "y"_a, "v"_a, "search_domain"_a=ibex::Interval::ALL_REALS)
      .def("invert", (void (Tube::*)( const ibex::Interval&,std::vector<ibex::Interval>&, const tubex::Tube&, const ibex::Interval&) const)&Tube::invert, "y"_a, "v_t"_a, "v"_a, "search_domain"_a=ibex::Interval::ALL_REALS)
      .def("max_diam", &Tube::max_diam)
      .def("max_gate_diam", &Tube::max_gate_diam, "t"_a)
      .def("diam", (const Trajectory (Tube::*)(bool) const)&Tube::diam, "gates_thicknesses"_a=false)
      .def("diam", (const Trajectory (Tube::*)( const tubex::Tube&) const)&Tube::diam, "v"_a)
      .def("is_subset", &Tube::is_subset, "x"_a)
      .def("is_strict_subset", &Tube::is_strict_subset, "x"_a)
      .def("is_interior_subset", &Tube::is_interior_subset, "x"_a)
      .def("is_strict_interior_subset", &Tube::is_strict_interior_subset, "x"_a)
      .def("is_superset", &Tube::is_superset, "x"_a)
      .def("is_strict_superset", &Tube::is_strict_superset, "x"_a)
      .def("is_empty", &Tube::is_empty)
      .def("contains", &Tube::contains, "x"_a)
      .def("overlaps", &Tube::overlaps, "x"_a, "ratio"_a=1)
      .def("set", (void (Tube::*)( const ibex::Interval&) )&Tube::set, "y"_a)
      .def("set", (void (Tube::*)( const ibex::Interval&,int) )&Tube::set, "y"_a, "slice_id"_a)
      .def("set", (void (Tube::*)( const ibex::Interval&,double) )&Tube::set, "y"_a, "t"_a)
      .def("set", (void (Tube::*)( const ibex::Interval&, const ibex::Interval&) )&Tube::set, "y"_a, "t"_a)
      .def("set_empty", &Tube::set_empty)
      .def("inflate", (const Tube & (Tube::*)(double) )&Tube::inflate, "rad"_a)
      .def("inflate", (const Tube & (Tube::*)( const tubex::Trajectory&) )&Tube::inflate, "rad"_a)
      .def("shift_domain", &Tube::shift_domain, "a"_a)
      .def("remove_gate", &Tube::remove_gate, "t"_a)
      .def("bisect", &Tube::bisect, "t"_a, "ratio"_a=0.49)
      .def("class_name", &Tube::class_name)
      .def("enable_synthesis", &Tube::enable_synthesis, "enable"_a=true)
      .def("integral", (const ibex::Interval (Tube::*)(double) const)&Tube::integral, "t"_a)
      .def("integral", (const ibex::Interval (Tube::*)( const ibex::Interval&) const)&Tube::integral, "t"_a)
      .def("integral", (const ibex::Interval (Tube::*)( const ibex::Interval&, const ibex::Interval&) const)&Tube::integral, "t1"_a, "t2"_a)
      .def("partial_integral", (const std::pair<ibex::Interval, ibex::Interval> (Tube::*)( const ibex::Interval&) const)&Tube::partial_integral, "t"_a)
      .def("partial_integral", (const std::pair<ibex::Interval, ibex::Interval> (Tube::*)( const ibex::Interval&, const ibex::Interval&) const)&Tube::partial_integral, "t1"_a, "t2"_a)
      .def("serialize", (void (Tube::*)( const std::string&,int) const)&Tube::serialize, "binary_file_name"_a="x.tube", "version_number"_a=SERIALIZATION_VERSION)
      .def("serialize", (void (Tube::*)( const std::string&, const tubex::Trajectory&,int) const)&Tube::serialize, "binary_file_name"_a, "traj"_a, "version_number"_a=SERIALIZATION_VERSION)
      .def_static("enable_syntheses", &Tube::enable_syntheses, "enable"_a=true)
      .def_static("same_slicing",  &Tube::same_slicing, "x1"_a, "x2"_a)
      .def_static("hull", &Tube::hull, "l_tubes"_a)
  ;



  m.def("cos", ( const Tube (*) (const Tube& )) &tubex::cos);
  m.def("sin", ( const Tube (*) (const Tube& ) ) &tubex::sin);
  m.def("abs", ( const Tube (*) (const Tube& )) &tubex::abs);
  m.def("sqr", ( const Tube (*) (const Tube& )) &tubex::sqr);
  m.def("sqrt", ( const Tube (*) (const Tube& )) &tubex::sqrt);
  m.def("exp", ( const Tube (*) (const Tube& )) &tubex::exp);
  m.def("log", ( const Tube (*) (const Tube& )) &tubex::log);
  m.def("tan", ( const Tube (*) (const Tube& )) &tubex::tan);
  m.def("acos", ( const Tube (*) (const Tube& )) &tubex::acos);
  m.def("asin", ( const Tube (*) (const Tube& )) &tubex::asin);
  m.def("atan", ( const Tube (*) (const Tube& )) &tubex::atan);
  m.def("cosh", ( const Tube (*) (const Tube& )) &tubex::cosh);
  m.def("sinh", ( const Tube (*) (const Tube& )) &tubex::sinh);
  m.def("tanh", ( const Tube (*) (const Tube& )) &tubex::tanh);
  m.def("acosh", ( const Tube (*) (const Tube& )) &tubex::acosh);
  m.def("asinh", ( const Tube (*) (const Tube& )) &tubex::asinh);
  m.def("atanh", ( const Tube (*) (const Tube& )) &tubex::atanh);

  m.def("atan2", (const Tube (*) (const Tube&, const Tube&)) &tubex::atan2, "y"_a, "x"_a);
  m.def("atan2", (const Tube (*) (const Tube&, const ibex::Interval&)) &tubex::atan2, "y"_a, "x"_a);
  m.def("atan2", (const Tube (*) (const ibex::Interval&, const Tube&)) &tubex::atan2, "y"_a, "x"_a);

  m.def("pow", (const Tube (*) (const Tube& x, int p)) &tubex::pow, "x"_a, "p"_a);
  m.def("pow", (const Tube (*) (const Tube& x, double p)) &tubex::pow, "x"_a, "p"_a);
  m.def("pow", (const Tube (*) (const Tube& x, const ibex::Interval& p)) &tubex::pow, "x"_a, "p"_a);
  m.def("root",(const Tube (*) (const Tube& x, int p)) &tubex::root, "x"_a, "p"_a);

    

 

}