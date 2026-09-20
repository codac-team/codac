/** 
 *  Codac binding (core)
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <sstream>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <codac2_Interval.h>
#include <codac2_Slice.h>
#include <codac2_TSlice.h>
#include "codac2_py_SliceBase_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):
#include "codac2_py_Slice_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

template<typename T>
py::class_<Slice<T>> export_Slice(py::module& m, const std::string& name)
{
  py::class_<Slice<T>,
    std::unique_ptr<Slice<T>,py::nodelete> // disables any destructor invocations
  > exported_slice_class(m, name.c_str(), SLICE_MAIN);
  exported_slice_class

  // Methods from class SliceBase
  
    // t0_tf() and tslice() both return a reference to the TSlice of this slice, an
    // element of the TDomain (t0_tf() as an Interval, TSlice deriving from Interval).
    // Without a return value policy, pybind11 returned a copy of it, which was wrong in
    // both cases:
    // - tslice(): copying a TSlice copies its map of std::shared_ptr to the slices of
    //   every tube defined on the TDomain. The copy shared the ownership of those
    //   slices, and kept them alive after their tube was destroyed, as long as the
    //   Python object lived. Not being the element of the TDomain, it could not either
    //   be given back to SlicedTube.slice(), which finds a TSlice by its address.
    // - t0_tf(): pybind11 finds that the Interval is in fact a TSlice (its most derived
    //   type, which is registered) and makes the Python object a TSlice, but copies it
    //   with the copy constructor of the declared type, Interval. The result was a mere
    //   Interval passed off as a TSlice: a TSlice method using the members of TSlice
    //   (slices(), for instance) read past the object, and its std::shared_ptr<TSlice>
    //   holder deleted it as a TSlice, running the destructor of members it did not
    //   have.
    // reference_internal returns the TSlice itself, without any copy. Like the Slice
    // (whose py::nodelete holder never destroys it), it remains owned by the C++ side:
    // by the TDomain, which the tubes defined on it keep alive.
    .def("t0_tf", &Slice<T>::t0_tf,
      py::return_value_policy::reference_internal,
      CONST_INTERVAL_REF_SLICEBASE_T0_TF_CONST)
  
    // Returned by reference, not copied: see t0_tf() above.
    .def("tslice", &Slice<T>::tslice,
      py::return_value_policy::reference_internal,
      CONST_TSLICE_REF_SLICEBASE_TSLICE_CONST)

  // Methods from class Slice<T>
  
    .def("size", &Slice<T>::size,
      INDEX_SLICE_T_SIZE_CONST)

    .def("codomain", [](const Slice<T>& x) { return x.codomain(); },
      CONST_T_REF_SLICE_T_CODOMAIN_CONST)

    .def("is_gate", &Slice<T>::is_gate,
      BOOL_SLICE_T_IS_GATE_CONST)

    .def("prev_slice", (std::shared_ptr<Slice<T>> (Slice<T>::*)()) &Slice<T>::prev_slice,
      SHARED_PTR_SLICE_T_SLICE_T_PREV_SLICE)

    .def("next_slice", (std::shared_ptr<Slice<T>> (Slice<T>::*)()) &Slice<T>::next_slice,
      SHARED_PTR_SLICE_T_SLICE_T_NEXT_SLICE)

    .def("input_gate", &Slice<T>::input_gate,
      T_SLICE_T_INPUT_GATE_CONST)

    .def("output_gate", &Slice<T>::output_gate,
      T_SLICE_T_OUTPUT_GATE_CONST)

    .def("__call__", (T (Slice<T>::*)(double) const) &Slice<T>::operator(),
      T_SLICE_T_OPERATORCALL_DOUBLE_CONST,
      "t"_a)

    .def("__call__", (T (Slice<T>::*)(const Interval&) const) &Slice<T>::operator(),
      T_SLICE_T_OPERATORCALL_CONST_INTERVAL_REF_CONST,
      "t"_a)

    .def("enclosed_bounds", &Slice<T>::enclosed_bounds,
      PAIR_TT_SLICE_T_ENCLOSED_BOUNDS_CONST_INTERVAL_REF_CONST,
      "t"_a)

    .def("set", (void (Slice<T>::*)(const T&,bool)) &Slice<T>::set,
      VOID_SLICE_T_SET_CONST_T_REF_BOOL,
      "x"_a, "propagate"_a=true)

    .def("init", &Slice<T>::init,
      VOID_SLICE_T_INIT)

    .def("set_empty", (void (Slice<T>::*)()) &Slice<T>::set_empty,
      VOID_SLICE_T_SET_EMPTY)

    .def(py::self == py::self,
      BOOL_SLICE_T_OPERATOREQ_CONST_SLICE_REF_CONST,
      "x"_a)

    .def(py::self != py::self,
      "x"_a)

    .def("all_reals_value", &Slice<T>::all_reals_value,
      T_SLICE_T_ALL_REALS_VALUE_CONST)

    .def("empty_value", &Slice<T>::empty_value,
      T_SLICE_T_EMPTY_VALUE_CONST)

    .def("__repr__", [](const Slice<T>& x) {
          std::ostringstream stream;
          stream << x;
          return string(stream.str()); 
        },
      OSTREAM_REF_OPERATOROUT_OSTREAM_REF_CONST_SLICE_REF)
  ;

  if constexpr(std::is_same_v<T,Interval>)
  {
    exported_slice_class

      .def("polygon_slice", &Slice<Interval>::polygon_slice,
        CONVEXPOLYGON_SLICE_T_POLYGON_SLICE_CONST_SLICE_T_REF_CONST,
        "v"_a)

    ;
  }

  else if constexpr(std::is_same_v<T,IntervalVector>)
  {
    exported_slice_class

      .def("polygon_slice_i", &Slice<IntervalVector>::polygon_slice_i,
        CONVEXPOLYGON_SLICE_T_POLYGON_SLICE_I_CONST_SLICE_T_REF_INDEX_CONST,
        "v"_a, "i"_a)

    ;
  }

  if constexpr(std::is_same_v<T,Interval> || std::is_same_v<T,IntervalVector>)
  {
    exported_slice_class

      .def("__call__", (T (Slice<T>::*)(double,const Slice<T>&) const) &Slice<T>::operator(),
        T_SLICE_T_OPERATORCALL_DOUBLE_CONST_SLICE_T_REF_CONST,
        "t"_a, "v"_a)

      .def("__call__", (T (Slice<T>::*)(const Interval&,const Slice<T>&) const) &Slice<T>::operator(),
        T_SLICE_T_OPERATORCALL_CONST_INTERVAL_REF_CONST_SLICE_T_REF_CONST,
        "t"_a, "v"_a)

      .def("invert", (Interval (Slice<T>::*)(const T&,const Interval&) const) &Slice<T>::invert,
        INTERVAL_SLICE_T_INVERT_CONST_T_REF_CONST_INTERVAL_REF_CONST,
        "y"_a, "t"_a=Interval())

      .def("invert", (Interval (Slice<T>::*)(const T&,const Slice<T>&,const Interval&) const) &Slice<T>::invert,
        INTERVAL_SLICE_T_INVERT_CONST_T_REF_CONST_SLICE_T_REF_CONST_INTERVAL_REF_CONST,
        "y"_a, "v"_a, "t"_a=Interval())

    ;
  }

  return exported_slice_class;
}
