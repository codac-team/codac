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

#include "tubex_Trajectory.h"
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

void export_Trajectory(py::module& m){

  py::class_<Trajectory> trajectory(m, "Trajectory");
    trajectory
      .def(py::init<>())
      .def(py::init< const ibex::Interval&, const tubex::Function&>(), "domain"_a, "f"_a, py::keep_alive<1,3>())
      .def(py::init< const ibex::Interval&, const tubex::Function&,double>(), "domain"_a, "f"_a, "timestep"_a, py::keep_alive<1,3>())
      .def(py::init< const std::map<double, double>&>(), "m_map_values"_a)
      .def(py::init< const tubex::Trajectory&>(), "traj"_a)

      
      .def("size", &Trajectory::size)
      .def("domain", &Trajectory::domain)
      .def("sampled_map", &Trajectory::sampled_map)
      .def("function", &Trajectory::function)
      .def("codomain", &Trajectory::codomain)
      .def("first_value", &Trajectory::first_value)
      .def("last_value", &Trajectory::last_value)
      .def("not_defined", &Trajectory::not_defined)
      .def("set", &Trajectory::set, "y"_a, "t"_a)
      .def("truncate_domain", &Trajectory::truncate_domain, "domain"_a)
      .def("shift_domain", &Trajectory::shift_domain, "a"_a)
      .def("sample", (void (Trajectory::*)(double) )&Trajectory::sample, "timestep"_a)
      .def("sample", (void (Trajectory::*)( const tubex::Trajectory&) )&Trajectory::sample, "x"_a)
      .def("primitive", (const Trajectory (Trajectory::*)(double) const)&Trajectory::primitive, "c"_a=0)
      .def("primitive", (const Trajectory (Trajectory::*)(double,double) const)&Trajectory::primitive, "c"_a, "timestep"_a)
      .def("diff", &Trajectory::diff)
      .def("finite_diff", &Trajectory::finite_diff, "t"_a)
      .def("class_name", &Trajectory::class_name)
  ;

    py::class_<TrajectoryVector> trajectoryvector(m, "TrajectoryVector");
    trajectoryvector
      .def(py::init<int>(), "n"_a)
      .def(py::init< const ibex::Interval&, const tubex::Function&>(), "domain"_a, "f"_a)
      .def(py::init< const std::map<double, ibex::Vector>&>(), "m_map_values"_a)
      .def(py::init< const std::vector<std::map<double, double> >&>(), "v_map_values"_a)
      .def(py::init< const tubex::TrajectoryVector&>(), "traj"_a)
      .def(py::init<int, const tubex::Trajectory&>(), "n"_a, "x"_a)
      // .def("__eq__", [](TrajectoryVector& s, const tubex::TrajectoryVector& o) { s r= o;})
      .def("size", &TrajectoryVector::size)
      .def("domain", &TrajectoryVector::domain)
      .def("resize", &TrajectoryVector::resize, "n"_a)
      .def("subvector", &TrajectoryVector::subvector, "start_index"_a, "end_index"_a)
      .def("put", &TrajectoryVector::put, "start_index"_a, "subvec"_a)
      .def("codomain", &TrajectoryVector::codomain)
      .def("__call__", [](TrajectoryVector& s,int o) { return s[o];})
      .def("__call__", [](TrajectoryVector& s,int o) { return s[o] ;})
      .def("__call__", [](TrajectoryVector& s,double o) { return s(o);})
      .def("__call__", [](TrajectoryVector& s, const ibex::Interval& o) { return s(o);})
      .def("first_value", &TrajectoryVector::first_value)
      .def("last_value", &TrajectoryVector::last_value)
      .def("not_defined", &TrajectoryVector::not_defined)
      .def("__eq__", [](TrajectoryVector& s, const tubex::TrajectoryVector& o) { s == o;})
      .def("__ne__", [](TrajectoryVector& s, const tubex::TrajectoryVector& o) { s != o;})
      .def("set", &TrajectoryVector::set, "y"_a, "t"_a)
      .def("truncate_domain", &TrajectoryVector::truncate_domain, "domain"_a)
      .def("shift_domain", &TrajectoryVector::shift_domain, "a"_a)
      .def("sample", &TrajectoryVector::sample, "timestep"_a)
      .def("primitive", (const TrajectoryVector (TrajectoryVector::*)( const ibex::Vector&) const)&TrajectoryVector::primitive, "c"_a)
      .def("primitive", (const TrajectoryVector (TrajectoryVector::*)( const ibex::Vector&,double) const)&TrajectoryVector::primitive, "c"_a, "timestep"_a)
      .def("diff", &TrajectoryVector::diff)
      .def("__iadd__", [](TrajectoryVector& s,double o) { s += o;})
      .def("__iadd__", [](TrajectoryVector& s, const tubex::Trajectory& o) { s += o;})
      .def("__iadd__", [](TrajectoryVector& s, const ibex::Vector& o) { s += o;})
      .def("__iadd__", [](TrajectoryVector& s, const tubex::TrajectoryVector& o) { s += o;})
      .def("__isub__", [](TrajectoryVector& s,double o) { s -= o;})
      .def("__isub__", [](TrajectoryVector& s, const tubex::Trajectory& o) { s -= o;})
      .def("__isub__", [](TrajectoryVector& s, const ibex::Vector& o) { s -= o;})
      .def("__isub__", [](TrajectoryVector& s, const tubex::TrajectoryVector& o) { s -= o;})
      .def("__imul__", [](TrajectoryVector& s,double o) { s *= o;})
      .def("__imul__", [](TrajectoryVector& s, const tubex::Trajectory& o) { s *= o;})
      .def("__itruediv__", [](TrajectoryVector& s,double o) { s /= o;})
      .def("__itruediv__", [](TrajectoryVector& s, const tubex::Trajectory& o) { s /= o;})
      .def("class_name", &TrajectoryVector::class_name)
    ;
      // .def("__repr__", [](TrajectoryVector& s,std::ostream&, const tubex::TrajectoryVector& o) { s << o;});
    

}
