/** 
 *  \file
 *  TrajectoryVector Python binding
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

#include "tubex_TrajectoryVector.h"
// Generated file from Doxygen XML (doxygen2docstring.py):
#include "tubex_py_TrajectoryVector_docs.h"

using namespace std;
using namespace ibex;
using namespace tubex;
namespace py = pybind11;
using namespace pybind11::literals;


void export_TrajectoryVector(py::module& m)
{
  py::class_<TrajectoryVector> trajectoryvector(m, "TrajectoryVector", TRAJECTORYVECTOR_MAIN);
  trajectoryvector

  // Definition

    .def(py::init<int>(),
      TRAJECTORYVECTOR_TRAJECTORYVECTOR_INT,
      "n"_a)

    .def(py::init<const Interval&,const TFunction&>(),
      TRAJECTORYVECTOR_TRAJECTORYVECTOR_INTERVAL_TFUNCTION,
      "domain"_a, "f"_a)

    .def(py::init<const Interval&,const TFunction&,double>(),
      TRAJECTORYVECTOR_TRAJECTORYVECTOR_INTERVAL_TFUNCTION_DOUBLE,
      "domain"_a, "f"_a, "timestep"_a)

    .def(py::init<const map<double,Vector>&>(),
      TRAJECTORYVECTOR_TRAJECTORYVECTOR_MAPDOUBLEVECTOR,
      "m_map_values"_a)

    .def(py::init<const vector<map<double,double> >&>(),
      TRAJECTORYVECTOR_TRAJECTORYVECTOR_VECTORMAPDOUBLEDOUBLE,
      "v_map_values"_a)

    .def(py::init<initializer_list<Trajectory>>(),
      TRAJECTORYVECTOR_TRAJECTORYVECTOR_INITIALIZERLISTTRAJECTORY,
      "list"_a)

    .def(py::init<const TrajectoryVector&>(),
      TRAJECTORYVECTOR_TRAJECTORYVECTOR_TRAJECTORYVECTOR,
      "traj"_a)

    .def(py::init<int,const Trajectory &>(),
      TRAJECTORYVECTOR_TRAJECTORYVECTOR_INT_TRAJECTORY,
      "n"_a, "x"_a)

    .def("size", &TrajectoryVector::size,
      TRAJECTORYVECTOR_INT_SIZE)

    .def("tdomain", &TrajectoryVector::tdomain,
      TRAJECTORYVECTOR_CONSTINTERVAL_TDOMAIN)

    .def("resize", &TrajectoryVector::resize,
      TRAJECTORYVECTOR_VOID_RESIZE_INT,
      "n"_a)

    .def("subvector", &TrajectoryVector::subvector,
      TRAJECTORYVECTOR_CONSTTRAJECTORYVECTOR_SUBVECTOR_INT_INT,
      "start_index"_a, "end_index"_a)

    .def("put", &TrajectoryVector::put,
      TRAJECTORYVECTOR_VOID_PUT_INT_TRAJECTORYVECTOR,
      "start_index"_a, "subvec"_a)
  
  // Accessing values

    .def("codomain", &TrajectoryVector::codomain,
      TRAJECTORYVECTOR_CONSTINTERVALVECTOR_CODOMAIN)

    .def("__call__", [](TrajectoryVector& s,double o) { return s(o);}, 
      TRAJECTORYVECTOR_CONSTVECTOR_OPERATORP_DOUBLE)

    .def("__call__", [](TrajectoryVector& s,const Interval& o) { return s(o);}, 
      TRAJECTORYVECTOR_CONSTINTERVALVECTOR_OPERATORP_INTERVAL)

    .def("first_value", &TrajectoryVector::first_value,
      TRAJECTORYVECTOR_CONSTVECTOR_FIRST_VALUE)

    .def("last_value", &TrajectoryVector::last_value,
      TRAJECTORYVECTOR_CONSTVECTOR_LAST_VALUE)
  
  // Tests


    .def("not_defined", &TrajectoryVector::not_defined,
      TRAJECTORYVECTOR_BOOL_NOT_DEFINED)

    .def("__eq__", [](TrajectoryVector& s,const TrajectoryVector& o) { return s == o;}, 
      TRAJECTORYVECTOR_BOOL_OPERATOREQ_TRAJECTORYVECTOR)

    .def("__ne__", [](TrajectoryVector& s,const TrajectoryVector& o) { return s != o;}, 
      TRAJECTORYVECTOR_BOOL_OPERATORNEQ_TRAJECTORYVECTOR)
  
  // Setting values


    .def("set", &TrajectoryVector::set,
      TRAJECTORYVECTOR_VOID_SET_VECTOR_DOUBLE,
      "y"_a, "t"_a)

    .def("truncate_tdomain", &TrajectoryVector::truncate_tdomain,
      TRAJECTORYVECTOR_TRAJECTORYVECTOR_TRUNCATE_TDOMAIN_INTERVAL,
      "domain"_a)

    .def("shift_tdomain", &TrajectoryVector::shift_tdomain,
      TRAJECTORYVECTOR_TRAJECTORYVECTOR_SHIFT_TDOMAIN_DOUBLE,
      "a"_a)

    .def("sample", (TrajectoryVector & (TrajectoryVector::*)(double))&TrajectoryVector::sample,
      TRAJECTORYVECTOR_TRAJECTORYVECTOR_SAMPLE_DOUBLE,
      "timestep"_a)

    .def("sample", (TrajectoryVector & (TrajectoryVector::*)(const Trajectory&))&TrajectoryVector::sample,
      TRAJECTORYVECTOR_TRAJECTORYVECTOR_SAMPLE_TRAJECTORY,
      "x"_a)

    .def("sample", (TrajectoryVector & (TrajectoryVector::*)(const TrajectoryVector &))&TrajectoryVector::sample,
      TRAJECTORYVECTOR_TRAJECTORYVECTOR_SAMPLE_TRAJECTORYVECTOR,
      "x"_a)
  
  // Integration


    .def("primitive", (const TrajectoryVector (TrajectoryVector::*)(const Vector &) const)&TrajectoryVector::primitive,
      TRAJECTORYVECTOR_CONSTTRAJECTORYVECTOR_PRIMITIVE_VECTOR,
      "c"_a)

    .def("primitive", (const TrajectoryVector (TrajectoryVector::*)(const Vector &,double) const)&TrajectoryVector::primitive,
      TRAJECTORYVECTOR_CONSTTRAJECTORYVECTOR_PRIMITIVE_VECTOR_DOUBLE,
      "c"_a, "timestep"_a)

    .def("diff", &TrajectoryVector::diff,
      TRAJECTORYVECTOR_CONSTTRAJECTORYVECTOR_DIFF)
  
  // Assignments operators


    .def("__iadd__", [](TrajectoryVector& s,double o) { return s += o;}, 
      TRAJECTORYVECTOR_CONSTTRAJECTORYVECTOR_OPERATORADDEQ_DOUBLE)

    .def("__iadd__", [](TrajectoryVector& s,const Trajectory& o) { return s += o;}, 
      TRAJECTORYVECTOR_CONSTTRAJECTORYVECTOR_OPERATORADDEQ_TRAJECTORY)

    .def("__iadd__", [](TrajectoryVector& s,const Vector& o) { return s += o;}, 
      TRAJECTORYVECTOR_CONSTTRAJECTORYVECTOR_OPERATORADDEQ_VECTOR)

    .def("__iadd__", [](TrajectoryVector& s,const TrajectoryVector& o) { return s += o;}, 
      TRAJECTORYVECTOR_CONSTTRAJECTORYVECTOR_OPERATORADDEQ_TRAJECTORYVECTOR)

    .def("__isub__", [](TrajectoryVector& s,double o) { return s -= o;}, 
      TRAJECTORYVECTOR_CONSTTRAJECTORYVECTOR_OPERATORMINEQ_DOUBLE)

    .def("__isub__", [](TrajectoryVector& s,const Trajectory& o) { return s -= o;}, 
      TRAJECTORYVECTOR_CONSTTRAJECTORYVECTOR_OPERATORMINEQ_TRAJECTORY)

    .def("__isub__", [](TrajectoryVector& s,const Vector& o) { return s -= o;}, 
      TRAJECTORYVECTOR_CONSTTRAJECTORYVECTOR_OPERATORMINEQ_VECTOR)

    .def("__isub__", [](TrajectoryVector& s,const TrajectoryVector& o) { return s -= o;}, 
      TRAJECTORYVECTOR_CONSTTRAJECTORYVECTOR_OPERATORMINEQ_TRAJECTORYVECTOR)

    .def("__imul__", [](TrajectoryVector& s,double o) { return s *= o;}, 
      TRAJECTORYVECTOR_CONSTTRAJECTORYVECTOR_OPERATORMULEQ_DOUBLE)

    .def("__imul__", [](TrajectoryVector& s,const Trajectory& o) { return s *= o;}, 
      TRAJECTORYVECTOR_CONSTTRAJECTORYVECTOR_OPERATORMULEQ_TRAJECTORY)

    .def("__itruediv__", [](TrajectoryVector& s,double o) { return s /= o;}, 
      TRAJECTORYVECTOR_CONSTTRAJECTORYVECTOR_OPERATORDIVEQ_DOUBLE)

    .def("__itruediv__", [](TrajectoryVector& s,const Trajectory& o) { return s /= o;}, 
      TRAJECTORYVECTOR_CONSTTRAJECTORYVECTOR_OPERATORDIVEQ_TRAJECTORY)
  
  // String

    .def("class_name", &TrajectoryVector::class_name,
      TRAJECTORYVECTOR_CONSTSTRING_CLASS_NAME)

    .def("__repr__", [](const TrajectoryVector& x) { ostringstream str; str << x; return str.str(); })

  // Python vector methods

    .def("__len__", &TrajectoryVector::size)

    .def("__getitem__", [](TrajectoryVector& s, size_t index) -> Trajectory&
      {
        if(index >= static_cast<size_t>(s.size()))
          throw py::index_error();
        return s[static_cast<int>(index)];
      },
      TRAJECTORYVECTOR_TRAJECTORY_OPERATORB_INT,
      py::return_value_policy::reference_internal)

    .def("__getitem__", [](const TrajectoryVector& s, py::slice slice) -> TrajectoryVector
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
      TRAJECTORYVECTOR_CONSTTRAJECTORYVECTOR_SUBVECTOR_INT_INT)

    .def("__setitem__", [](TrajectoryVector& s, size_t index, Trajectory& t)
      {
        if(index >= static_cast<size_t>(s.size()))
          throw py::index_error();
        s[static_cast<int>(index)] = t;
      },
      TRAJECTORYVECTOR_TRAJECTORY_OPERATORB_INT)

  // Operators

    .def("__add__",      [](const TrajectoryVector& x) { return +x; })
    .def("__add__",      [](const TrajectoryVector& x, const TrajectoryVector& y) { return x+y; })
    .def("__add__",      [](const TrajectoryVector& x, const Vector& y) { return x+y; })

    .def("__radd__",     [](const TrajectoryVector& y, const Vector& x) { return x+y; })

    .def("__neg__",      [](const TrajectoryVector& x) { return -x; })

    .def("__sub__",      [](const TrajectoryVector& x, const TrajectoryVector& y) { return x-y; })
    .def("__sub__",      [](const TrajectoryVector& x, const Vector& y) { return x-y; })

    .def("__rsub__",     [](const TrajectoryVector& y, const Vector& x) { return x-y; })

    .def("__rmul__",     [](const TrajectoryVector& y, double x) { return x*y; })
    .def("__rmul__",     [](const TrajectoryVector& y, const Trajectory& x) { return x*y; })

    // todo const TrajectoryVector operator*(const Trajectory& x, const ibex::Vector& y);
    // todo const TrajectoryVector operator*(const ibex::Matrix& x, const TrajectoryVector& y);

    .def("__truediv__",  [](const TrajectoryVector& x, double y) { return x/y; })
    .def("__truediv__",  [](const TrajectoryVector& x, const Trajectory& y) { return x/y; })

    // todo const TrajectoryVector operator/(const ibex::Vector& x, const Trajectory& y);

    // todo const TrajectoryVector vecto_product(const TrajectoryVector& x, const ibex::Vector& y);
    // todo const TrajectoryVector vecto_product(const ibex::Vector& x, const TrajectoryVector& y);
  ;
}
