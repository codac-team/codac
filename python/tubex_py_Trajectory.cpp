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

#include <sstream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <pybind11/functional.h>
#include "pyIbex_type_caster.h"

#include "tubex_Trajectory.h"
#include "tubex_TrajectoryVector.h"
#include "tubex_RandTrajectory.h"

// Generated files from Doxygen XML (doxygen2docstring.py):
#include "tubex_py_RandTrajectory_docs.h"
#include "tubex_py_Trajectory_docs.h"
#include "tubex_py_TrajectoryVector_docs.h"

namespace py = pybind11;
using namespace pybind11::literals;
using py::class_;
using py::init;

using namespace tubex;
using ibex::Interval;
using ibex::IntervalVector;


std::string to_string(const Trajectory& x)
{
  std::ostringstream str;
  str << x;
  return str.str();
}

void export_Trajectory(py::module& m){

  py::class_<Trajectory> trajectory(m, "Trajectory");
  trajectory
    .def(py::init<>(),TRAJECTORY_TRAJECTORY)
    .def(py::init<const ibex::Interval &,const TFunction &>(),
        TRAJECTORY_TRAJECTORY_INTERVAL_TFUNCTION, "domain"_a, "f"_a, py::keep_alive<1,3>())
    .def(py::init<const ibex::Interval &,const TFunction &,double>(),
        TRAJECTORY_TRAJECTORY_INTERVAL_TFUNCTION_DOUBLE, "domain"_a, "f"_a, "timestep"_a, py::keep_alive<1,3>())
    .def(py::init<const std::map<double, double> &>(),
        TRAJECTORY_TRAJECTORY_MAPDOUBLEDOUBLE, "m_map_values"_a)
    .def(py::init<const Trajectory &>(),
        TRAJECTORY_TRAJECTORY_TRAJECTORY, "traj"_a)

    .def("size", &Trajectory::size,TRAJECTORY_INT_SIZE)
    .def("tdomain", &Trajectory::tdomain,TRAJECTORY_CONSTINTERVAL_TDOMAIN)
    .def("sampled_map", &Trajectory::sampled_map,TRAJECTORY_CONSTMAPDOUBLEDOUBLE_SAMPLED_MAP)
    .def("tfunction", &Trajectory::tfunction,TRAJECTORY_CONSTTFUNCTION_TFUNCTION,py::return_value_policy::reference_internal)
    .def("codomain", &Trajectory::codomain,TRAJECTORY_CONSTINTERVAL_CODOMAIN)
    .def("__call__", [](Trajectory& s,double o) { return s(o);}, 
        TRAJECTORY_DOUBLE_OPERATORP_DOUBLE)
    .def("__call__", [](Trajectory& s,const ibex::Interval & o) { return s(o);}, 
        TRAJECTORY_CONSTINTERVAL_OPERATORP_INTERVAL)
    .def("first_value", &Trajectory::first_value,TRAJECTORY_DOUBLE_FIRST_VALUE)
    .def("last_value", &Trajectory::last_value,TRAJECTORY_DOUBLE_LAST_VALUE)
    .def("not_defined", &Trajectory::not_defined,TRAJECTORY_BOOL_NOT_DEFINED)
    .def("__eq__", [](Trajectory& s,const Trajectory & o) { return s == o;}, 
        TRAJECTORY_BOOL_OPERATOREQ_TRAJECTORY)
    .def("__ne__", [](Trajectory& s,const Trajectory & o) { return s != o;}, 
        TRAJECTORY_BOOL_OPERATORNEQ_TRAJECTORY)
    .def("set", &Trajectory::set,TRAJECTORY_VOID_SET_DOUBLE_DOUBLE, "y"_a, "t"_a)
    .def("truncate_tdomain", &Trajectory::truncate_tdomain,
        TRAJECTORY_TRAJECTORY_TRUNCATE_TDOMAIN_INTERVAL, "domain"_a)
    .def("shift_tdomain", &Trajectory::shift_tdomain,
        TRAJECTORY_TRAJECTORY_SHIFT_TDOMAIN_DOUBLE, "a"_a)
    .def("sample", (Trajectory & (Trajectory::*)(double) )&Trajectory::sample,
        TRAJECTORY_TRAJECTORY_SAMPLE_DOUBLE, "timestep"_a)
    .def("sample", (Trajectory & (Trajectory::*)(const Trajectory &) )&Trajectory::sample,
        TRAJECTORY_TRAJECTORY_SAMPLE_TRAJECTORY, "x"_a)
    .def("make_continuous", (Trajectory & (Trajectory::*)() )&Trajectory::make_continuous,
        TRAJECTORY_TRAJECTORY_MAKE_CONTINUOUS)
    .def("primitive", (const Trajectory (Trajectory::*)(double) const)&Trajectory::primitive,
        TRAJECTORY_CONSTTRAJECTORY_PRIMITIVE_DOUBLE, "c"_a=0)
    .def("primitive", (const Trajectory (Trajectory::*)(double,double) const)&Trajectory::primitive,
        TRAJECTORY_CONSTTRAJECTORY_PRIMITIVE_DOUBLE_DOUBLE, "c"_a, "timestep"_a)
    .def("diff", &Trajectory::diff, TRAJECTORY_CONSTTRAJECTORY_DIFF)
    .def("finite_diff", &Trajectory::finite_diff,
        TRAJECTORY_DOUBLE_FINITE_DIFF_DOUBLE, "t"_a)
//    .def("__iadd__", [](Trajectory& s,double o) { return s += o;}, 
//        DOCS_TRAJECTORY_IADD_DOUBLE)
//    .def("__iadd__", [](Trajectory& s,const Trajectory & o) { return s += o;}, 
//        DOCS_TRAJECTORY_IADD_TRAJECTORY)
//    .def("__isub__", [](Trajectory& s,double o) { return s -= o;}, 
//        DOCS_TRAJECTORY_ISUB_DOUBLE)
//    .def("__isub__", [](Trajectory& s,const Trajectory & o) { return s -= o;}, 
//        DOCS_TRAJECTORY_ISUB_TRAJECTORY)
//    .def("__imul__", [](Trajectory& s,double o) { return s *= o;}, 
//        DOCS_TRAJECTORY_IMUL_DOUBLE)
//    .def("__imul__", [](Trajectory& s,const Trajectory & o) { return s *= o;}, 
//        DOCS_TRAJECTORY_IMUL_TRAJECTORY)
//    .def("__itruediv__", [](Trajectory& s,double o) { return s /= o;}, 
//        DOCS_TRAJECTORY_ITRUEDIV_DOUBLE)
//    .def("__itruediv__", [](Trajectory& s,const Trajectory & o) { return s /= o;}, 
//        DOCS_TRAJECTORY_ITRUEDIV_TRAJECTORY)
    .def("class_name", &Trajectory::class_name,TRAJECTORY_CONSTSTRING_CLASS_NAME)
      .def("__repr__", &to_string)

      // .def(py::init<>())
      // .def(py::init< const ibex::Interval&, const TFunction&>(), "domain"_a, "f"_a, py::keep_alive<1,3>())
      // .def(py::init< const ibex::Interval&, const TFunction&,double>(), "domain"_a, "f"_a, "timestep"_a, py::keep_alive<1,3>())
      // .def(py::init< const std::map<double, double>&>(), "m_map_values"_a)
      // .def(py::init< const tubex::Trajectory&>(), "traj"_a)

      
      // .def("size", &Trajectory::size)
      // .def("domain", &Trajectory::domain)
      // .def("sampled_map", &Trajectory::sampled_map)
      // .def("function", &Trajectory::function)
      // .def("codomain", &Trajectory::codomain)
      // .def("first_value", &Trajectory::first_value)
      // .def("last_value", &Trajectory::last_value)
      // .def("not_defined", &Trajectory::not_defined)
      // .def("set", &Trajectory::set, "y"_a, "t"_a)
      // .def("truncate_domain", &Trajectory::truncate_domain, "domain"_a)
      // .def("shift_domain", &Trajectory::shift_domain, "a"_a)
      // .def("sample", (void (Trajectory::*)(double) )&Trajectory::sample, "timestep"_a)
      // .def("sample", (void (Trajectory::*)( const tubex::Trajectory&) )&Trajectory::sample, "x"_a)
      // .def("primitive", (const Trajectory (Trajectory::*)(double) const)&Trajectory::primitive, "c"_a=0)
      // .def("primitive", (const Trajectory (Trajectory::*)(double,double) const)&Trajectory::primitive, "c"_a, "timestep"_a)
      // .def("diff", &Trajectory::diff)
      // .def("finite_diff", &Trajectory::finite_diff, "t"_a)
      // .def("class_name", &Trajectory::class_name)

  // Assignments operators

    .def("__iadd__", [](Trajectory& s,double o) { return s += o; },
      TRAJECTORY_CONSTTRAJECTORY_OPERATORADDEQ_DOUBLE)

    .def("__iadd__", [](Trajectory& s,const Trajectory & o) { return s += o; },
      TRAJECTORY_CONSTTRAJECTORY_OPERATORADDEQ_TRAJECTORY)

    .def("__isub__", [](Trajectory& s,double o) { return s -= o; },
      TRAJECTORY_CONSTTRAJECTORY_OPERATORMINEQ_DOUBLE)

    .def("__isub__", [](Trajectory& s,const Trajectory & o) { return s -= o; },
      TRAJECTORY_CONSTTRAJECTORY_OPERATORMINEQ_TRAJECTORY)

    .def("__imul__", [](Trajectory& s,double o) { return s *= o; },
      TRAJECTORY_CONSTTRAJECTORY_OPERATORMULEQ_DOUBLE)

    .def("__imul__", [](Trajectory& s,const Trajectory & o) { return s *= o; },
      TRAJECTORY_CONSTTRAJECTORY_OPERATORMULEQ_TRAJECTORY)

    .def("__itruediv__", [](Trajectory& s,double o) { return s /= o; },
      TRAJECTORY_CONSTTRAJECTORY_OPERATORDIVEQ_DOUBLE)

    .def("__itruediv__", [](Trajectory& s,const Trajectory & o) { return s /= o; },
      TRAJECTORY_CONSTTRAJECTORY_OPERATORDIVEQ_TRAJECTORY)

  // Operators

    .def("__add__",      [](const Trajectory& x) { return +x; })
    .def("__add__",      [](const Trajectory& x, const Trajectory& y) { return x+y; })
    .def("__add__",      [](const Trajectory& x, double y) { return x+y; })
    .def("__radd__",     [](const Trajectory& x, double y) { return x+y; })

    .def("__neg__",      [](const Trajectory& x) { return -x; })
    .def("__sub__",      [](const Trajectory& x, const Trajectory& y) { return x-y; })
    .def("__sub__",      [](const Trajectory& x, double y) { return x-y; })
    .def("__rsub__",     [](const Trajectory& x, double y) { return y-x; })

    .def("__mul__",      [](const Trajectory& x, const Trajectory& y) { return x*y; }, "y"_a.noconvert())
    .def("__mul__",      [](const Trajectory& x, double y) { return x*y; }, "y"_a.noconvert())
    .def("__rmul__",     [](const Trajectory& x, double y) { return x*y; }, "y"_a.noconvert())
    // Vector case
    .def("__mul__",      [](const Trajectory& x, const ibex::Vector& y) { return x*y; }, "y"_a.noconvert())

    .def("__truediv__",  [](const Trajectory& x, const Trajectory& y) { return x/y; }) //, "x"_a.noconvert(), "y"_a.noconvert())
    .def("__truediv__",  [](const Trajectory& x, double y) { return x/y; }) //, "x"_a.noconvert(), "y"_a.noconvert())
    .def("__rtruediv__", [](const Trajectory& x, double y) { return y/x; }) //, "x"_a.noconvert(), "y"_a.noconvert())
    // Vector case
    .def("__rtruediv__", [](const Trajectory& x, const ibex::Vector& y) { return y/x; }) //, "x"_a.noconvert(), "y"_a.noconvert())
  ;

    py::class_<TrajectoryVector> trajectoryvector(m, "TrajectoryVector");
    trajectoryvector
        .def(py::init<int>(),TRAJECTORYVECTOR_TRAJECTORYVECTOR_INT, "n"_a)
        .def(py::init<const ibex::Interval &,const TFunction &>(),
            TRAJECTORYVECTOR_TRAJECTORYVECTOR_INTERVAL_TFUNCTION, "domain"_a, "f"_a)
        .def(py::init<const ibex::Interval &,const TFunction &,double>(),
            TRAJECTORYVECTOR_TRAJECTORYVECTOR_INTERVAL_TFUNCTION_DOUBLE, "domain"_a, "f"_a, "timestep"_a)
        .def(py::init<const std::map<double, ibex::Vector> &>(),
            TRAJECTORYVECTOR_TRAJECTORYVECTOR_MAPDOUBLEVECTOR, "m_map_values"_a)
        .def(py::init<const std::vector<std::map<double, double> > &>(),
            TRAJECTORYVECTOR_TRAJECTORYVECTOR_VECTORMAPDOUBLEDOUBLE, "v_map_values"_a)
        .def(py::init<std::initializer_list<Trajectory>>(),
            TRAJECTORYVECTOR_TRAJECTORYVECTOR_INITIALIZERLISTTRAJECTORY, "list"_a)
        .def(py::init<const TrajectoryVector &>(),
            TRAJECTORYVECTOR_TRAJECTORYVECTOR_TRAJECTORYVECTOR, "traj"_a)
        .def(py::init<int,const Trajectory &>(),
            TRAJECTORYVECTOR_TRAJECTORYVECTOR_INT_TRAJECTORY, "n"_a, "x"_a)

        .def("size", &TrajectoryVector::size,TRAJECTORYVECTOR_INT_SIZE)
        .def("tdomain", &TrajectoryVector::tdomain,TRAJECTORYVECTOR_CONSTINTERVAL_TDOMAIN)
        .def("resize", &TrajectoryVector::resize,
            TRAJECTORYVECTOR_VOID_RESIZE_INT, "n"_a)
        .def("subvector", &TrajectoryVector::subvector,
            TRAJECTORYVECTOR_CONSTTRAJECTORYVECTOR_SUBVECTOR_INT_INT, "start_index"_a, "end_index"_a)
        .def("put", &TrajectoryVector::put,
            TRAJECTORYVECTOR_VOID_PUT_INT_TRAJECTORYVECTOR, "start_index"_a, "subvec"_a)
        .def("codomain", &TrajectoryVector::codomain,TRAJECTORYVECTOR_CONSTINTERVALVECTOR_CODOMAIN)
        .def("__call__", [](TrajectoryVector& s,double o) { return s(o);}, 
            TRAJECTORYVECTOR_CONSTVECTOR_OPERATORP_DOUBLE)
        .def("__call__", [](TrajectoryVector& s,const ibex::Interval & o) { return s(o);}, 
            TRAJECTORYVECTOR_CONSTINTERVALVECTOR_OPERATORP_INTERVAL)
        .def("first_value", &TrajectoryVector::first_value,
            TRAJECTORYVECTOR_CONSTVECTOR_FIRST_VALUE)
        .def("last_value", &TrajectoryVector::last_value,
            TRAJECTORYVECTOR_CONSTVECTOR_LAST_VALUE)
        .def("not_defined", &TrajectoryVector::not_defined,
            TRAJECTORYVECTOR_BOOL_NOT_DEFINED)
        .def("__eq__", [](TrajectoryVector& s,const TrajectoryVector & o) { return s == o;}, 
            TRAJECTORYVECTOR_BOOL_OPERATOREQ_TRAJECTORYVECTOR)
        .def("__ne__", [](TrajectoryVector& s,const TrajectoryVector & o) { return s != o;}, 
            TRAJECTORYVECTOR_BOOL_OPERATORNEQ_TRAJECTORYVECTOR)
        .def("set", &TrajectoryVector::set,
            TRAJECTORYVECTOR_VOID_SET_VECTOR_DOUBLE, "y"_a, "t"_a)
        .def("truncate_tdomain", &TrajectoryVector::truncate_tdomain,
            TRAJECTORYVECTOR_TRAJECTORYVECTOR_TRUNCATE_TDOMAIN_INTERVAL, "domain"_a)
        .def("shift_tdomain", &TrajectoryVector::shift_tdomain,
            TRAJECTORYVECTOR_TRAJECTORYVECTOR_SHIFT_TDOMAIN_DOUBLE, "a"_a)
        .def("sample", (TrajectoryVector & (TrajectoryVector::*)(double) )&TrajectoryVector::sample,
            TRAJECTORYVECTOR_TRAJECTORYVECTOR_SAMPLE_DOUBLE, "timestep"_a)
        .def("sample", (TrajectoryVector & (TrajectoryVector::*)(const Trajectory &) )&TrajectoryVector::sample,
            TRAJECTORYVECTOR_TRAJECTORYVECTOR_SAMPLE_TRAJECTORY, "x"_a)
        .def("sample", (TrajectoryVector & (TrajectoryVector::*)(const TrajectoryVector &) )&TrajectoryVector::sample,
            TRAJECTORYVECTOR_TRAJECTORYVECTOR_SAMPLE_TRAJECTORYVECTOR, "x"_a)
        .def("primitive", (const TrajectoryVector (TrajectoryVector::*)(const ibex::Vector &) const)&TrajectoryVector::primitive,
            TRAJECTORYVECTOR_CONSTTRAJECTORYVECTOR_PRIMITIVE_VECTOR, "c"_a)
        .def("primitive", (const TrajectoryVector (TrajectoryVector::*)(const ibex::Vector &,double) const)&TrajectoryVector::primitive,
            TRAJECTORYVECTOR_CONSTTRAJECTORYVECTOR_PRIMITIVE_VECTOR_DOUBLE, "c"_a, "timestep"_a)
        .def("diff", &TrajectoryVector::diff,TRAJECTORYVECTOR_CONSTTRAJECTORYVECTOR_DIFF)
        .def("__iadd__", [](TrajectoryVector& s,double o) { return s += o;}, 
            TRAJECTORYVECTOR_CONSTTRAJECTORYVECTOR_OPERATORADDEQ_DOUBLE)
        .def("__iadd__", [](TrajectoryVector& s,const Trajectory & o) { return s += o;}, 
            TRAJECTORYVECTOR_CONSTTRAJECTORYVECTOR_OPERATORADDEQ_TRAJECTORY)
        .def("__iadd__", [](TrajectoryVector& s,const ibex::Vector & o) { return s += o;}, 
            TRAJECTORYVECTOR_CONSTTRAJECTORYVECTOR_OPERATORADDEQ_VECTOR)
        .def("__iadd__", [](TrajectoryVector& s,const TrajectoryVector & o) { return s += o;}, 
            TRAJECTORYVECTOR_CONSTTRAJECTORYVECTOR_OPERATORADDEQ_TRAJECTORYVECTOR)
        .def("__isub__", [](TrajectoryVector& s,double o) { return s -= o;}, 
            TRAJECTORYVECTOR_CONSTTRAJECTORYVECTOR_OPERATORMINEQ_DOUBLE)
        .def("__isub__", [](TrajectoryVector& s,const Trajectory & o) { return s -= o;}, 
            TRAJECTORYVECTOR_CONSTTRAJECTORYVECTOR_OPERATORMINEQ_TRAJECTORY)
        .def("__isub__", [](TrajectoryVector& s,const ibex::Vector & o) { return s -= o;}, 
            TRAJECTORYVECTOR_CONSTTRAJECTORYVECTOR_OPERATORMINEQ_VECTOR)
        .def("__isub__", [](TrajectoryVector& s,const TrajectoryVector & o) { return s -= o;}, 
            TRAJECTORYVECTOR_CONSTTRAJECTORYVECTOR_OPERATORMINEQ_TRAJECTORYVECTOR)
        .def("__imul__", [](TrajectoryVector& s,double o) { return s *= o;}, 
            TRAJECTORYVECTOR_CONSTTRAJECTORYVECTOR_OPERATORMULEQ_DOUBLE)
        .def("__imul__", [](TrajectoryVector& s,const Trajectory & o) { return s *= o;}, 
            TRAJECTORYVECTOR_CONSTTRAJECTORYVECTOR_OPERATORMULEQ_TRAJECTORY)
        .def("__itruediv__", [](TrajectoryVector& s,double o) { return s /= o;}, 
            TRAJECTORYVECTOR_CONSTTRAJECTORYVECTOR_OPERATORDIVEQ_DOUBLE)
        .def("__itruediv__", [](TrajectoryVector& s,const Trajectory & o) { return s /= o;}, 
            TRAJECTORYVECTOR_CONSTTRAJECTORYVECTOR_OPERATORDIVEQ_TRAJECTORY)
        .def("class_name", &TrajectoryVector::class_name,
            TRAJECTORYVECTOR_CONSTSTRING_CLASS_NAME)


      .def("__getitem__", [](TrajectoryVector& s, size_t index) -> Trajectory&{
              if (index >= static_cast<size_t>(s.size())){
                  throw py::index_error();
              }
                return s[static_cast<int>(index)];
          }, TRAJECTORYVECTOR_TRAJECTORY_OPERATORB_INT, py::return_value_policy::reference_internal)

      .def("__getitem__", [](const TrajectoryVector& s, py::slice slice) -> TrajectoryVector {
            size_t start, stop, step, slicelength;
            if (!slice.compute(s.size(), &start, &stop, &step, &slicelength))
                throw py::error_already_set();
            if (step != 1){
                std::cout << "Warning slice step must be equal to 1\n";
            }
            
            // to respect the python convention, the stop index 
            // is not included in slice
            return s.subvector(start, start + slicelength-1);
        },TRAJECTORYVECTOR_CONSTTRAJECTORYVECTOR_SUBVECTOR_INT_INT)
      .def("__setitem__", [](TrajectoryVector& s, size_t index, Trajectory& t){
              if (index >= static_cast<size_t>(s.size())){
                throw py::index_error();
              }
              s[static_cast<int>(index)] = t;
          }, TRAJECTORYVECTOR_TRAJECTORY_OPERATORB_INT)
        // .def("operator[]", [](TrajectoryVector& s,int o) { return s[o];}, 
        //     DOCS_TRAJECTORYVECTOR_OPERATOR_INDEX_INT)
        // .def("operator[]", [](TrajectoryVector& s,int o) { return s[o];}, 
        //     DOCS_TRAJECTORYVECTOR_OPERATOR_INDEX_INT1)
;
      // .def(py::init<int>(), "n"_a)
      // .def(py::init< const ibex::Interval&, const TFunction&>(), "domain"_a, "f"_a)
      // .def(py::init< const std::map<double, ibex::Vector>&>(), "m_map_values"_a)
      // .def(py::init< const std::vector<std::map<double, double> >&>(), "v_map_values"_a)
      // .def(py::init< const tubex::TrajectoryVector&>(), "traj"_a)
      // .def(py::init<int, const tubex::Trajectory&>(), "n"_a, "x"_a)
      // // .def("__eq__", [](TrajectoryVector& s, const tubex::TrajectoryVector& o) { s r= o;})
      // .def("size", &TrajectoryVector::size)
      // .def("domain", &TrajectoryVector::domain)
      // .def("resize", &TrajectoryVector::resize, "n"_a)
      // .def("subvector", &TrajectoryVector::subvector, "start_index"_a, "end_index"_a)
      // .def("put", &TrajectoryVector::put, "start_index"_a, "subvec"_a)
      // .def("codomain", &TrajectoryVector::codomain)
      // .def("__call__", [](TrajectoryVector& s,int o) { return s[o];})
      // .def("__call__", [](TrajectoryVector& s,int o) { return s[o] ;})
      // .def("__call__", [](TrajectoryVector& s,double o) { return s(o);})
      // .def("__call__", [](TrajectoryVector& s, const ibex::Interval& o) { return s(o);})
      // .def("first_value", &TrajectoryVector::first_value)
      // .def("last_value", &TrajectoryVector::last_value)
      // .def("not_defined", &TrajectoryVector::not_defined)
      // .def("__eq__", [](TrajectoryVector& s, const tubex::TrajectoryVector& o) { s == o;})
      // .def("__ne__", [](TrajectoryVector& s, const tubex::TrajectoryVector& o) { s != o;})
      // .def("set", &TrajectoryVector::set, "y"_a, "t"_a)
      // .def("truncate_domain", &TrajectoryVector::truncate_domain, "domain"_a)
      // .def("shift_domain", &TrajectoryVector::shift_domain, "a"_a)
      // .def("sample", &TrajectoryVector::sample, "timestep"_a)
      // .def("primitive", (const TrajectoryVector (TrajectoryVector::*)( const ibex::Vector&) const)&TrajectoryVector::primitive, "c"_a)
      // .def("primitive", (const TrajectoryVector (TrajectoryVector::*)( const ibex::Vector&,double) const)&TrajectoryVector::primitive, "c"_a, "timestep"_a)
      // .def("diff", &TrajectoryVector::diff)
      // .def("__iadd__", [](TrajectoryVector& s,double o) { s += o;})
      // .def("__iadd__", [](TrajectoryVector& s, const tubex::Trajectory& o) { s += o;})
      // .def("__iadd__", [](TrajectoryVector& s, const ibex::Vector& o) { s += o;})
      // .def("__iadd__", [](TrajectoryVector& s, const tubex::TrajectoryVector& o) { s += o;})
      // .def("__isub__", [](TrajectoryVector& s,double o) { s -= o;})
      // .def("__isub__", [](TrajectoryVector& s, const tubex::Trajectory& o) { s -= o;})
      // .def("__isub__", [](TrajectoryVector& s, const ibex::Vector& o) { s -= o;})
      // .def("__isub__", [](TrajectoryVector& s, const tubex::TrajectoryVector& o) { s -= o;})
      // .def("__imul__", [](TrajectoryVector& s,double o) { s *= o;})
      // .def("__imul__", [](TrajectoryVector& s, const tubex::Trajectory& o) { s *= o;})
      // .def("__itruediv__", [](TrajectoryVector& s,double o) { s /= o;})
      // .def("__itruediv__", [](TrajectoryVector& s, const tubex::Trajectory& o) { s /= o;})
      // .def("class_name", &TrajectoryVector::class_name)
    // ;
      // .def("__repr__", [](TrajectoryVector& s,std::ostream&, const tubex::TrajectoryVector& o) { s << o;});

    py::class_<RandTrajectory> randtrajectory(m, "RandTrajectory", trajectory, "todo");
    randtrajectory
      .def(py::init<const ibex::Interval &,double,const ibex::Interval &>(),
          RANDTRAJECTORY_RANDTRAJECTORY_INTERVAL_DOUBLE_INTERVAL, "tdomain"_a, "timestep"_a, "bounds"_a);

    //py::implicitly_convertible<Trajectory,RandTrajectory>();
}
