/** 
 *  Codac binding (core)
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <sstream>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <codac2_Interval.h>
#include <codac2_SlicedTube.h>
#include <codac2_SlicedTube_operations.h>
#include <codac2_TubeBase.h>
#include <codac2_SampledTraj.h>
#include "codac2_py_matlab.h"
#include "codac2_py_SlicedTubeBase_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):
#include "codac2_py_SlicedTube_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):
#include "codac2_py_SlicedTube_operations_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

template<typename T>
py::class_<SlicedTube<T>,TubeBase> export_SlicedTube(py::module& m, const std::string& name)
{
  py::class_<SlicedTube<T>,TubeBase> exported_slicedtube_class(m, name.c_str(), SLICEDTUBEBASE_MAIN);
  exported_slicedtube_class

    .def(py::init<const std::shared_ptr<TDomain>&,const T&>(),
      SLICEDTUBE_T_SLICEDTUBE_CONST_SHARED_PTR_TDOMAIN_REF_CONST_T_REF,
      "tdomain"_a, "codomain"_a)

    .def(py::init<const std::shared_ptr<TDomain>&,const AnalyticFunction<typename ExprType<T>::Type>&>(),
      SLICEDTUBE_T_SLICEDTUBE_CONST_SHARED_PTR_TDOMAIN_REF_CONST_ANALYTICFUNCTION_TYPENAME_EXPRTYPE_T_TYPE_REF,
      "tdomain"_a, "f"_a)

    .def(py::init<const std::shared_ptr<TDomain>&,const AnalyticTraj<typename ExprType<T>::Type>&>(),
      SLICEDTUBE_T_SLICEDTUBE_CONST_SHARED_PTR_TDOMAIN_REF_CONST_ANALYTICTRAJ_V_REF,
      "tdomain"_a, "x"_a)

    .def(py::init<const std::shared_ptr<TDomain>&,const SampledTraj<typename ExprType<T>::Type::Scalar>&>(),
      SLICEDTUBE_T_SLICEDTUBE_CONST_SHARED_PTR_TDOMAIN_REF_CONST_SAMPLEDTRAJ_V_REF,
      "tdomain"_a, "x"_a)

    .def(py::init<const SlicedTube<T>&>(),
      SLICEDTUBE_T_SLICEDTUBE_CONST_SLICEDTUBE_T_REF,
      "x"_a)

    .def("__iter__", [](const SlicedTube<T>& x)
        {
          return py::make_iterator(x.begin(), x.end());
        },
        py::keep_alive<0,1>()) // essential: keep object alive while iterator exists

    // From SlicedTubeBase class:
    .def("nb_slices", &SlicedTube<T>::nb_slices,
      SIZET_SLICEDTUBEBASE_NB_SLICES_CONST)

    .def("size", &SlicedTube<T>::size,
      INDEX_SLICEDTUBE_T_SIZE_CONST)
    
    .def("volume", &SlicedTube<T>::volume,
      DOUBLE_SLICEDTUBE_T_VOLUME_CONST)
    
    .def("first_slice", [](SlicedTube<T>& x) -> Slice<T>&
        {
          return *x.first_slice();
        },
      py::return_value_policy::reference,
      SHARED_PTR_SLICE_T_SLICEDTUBE_T_FIRST_SLICE)
    
    .def("last_slice", [](SlicedTube<T>& x) -> Slice<T>&
        {
          return *x.last_slice();
        },
      py::return_value_policy::reference,
      SHARED_PTR_SLICE_T_SLICEDTUBE_T_LAST_SLICE)
    
    .def("slice", [](SlicedTube<T>& x, TSlice& it) -> Slice<T>&
        {
          return *x.slice(
            std::shared_ptr<TSlice>(&it, [](TSlice*) {})
          );
        },
      py::return_value_policy::reference,
      SHARED_PTR_SLICE_T_SLICEDTUBE_T_SLICE_SHARED_PTR_TSLICE,
      "it"_a)
    
    .def("is_empty", &SlicedTube<T>::is_empty,
      BOOL_SLICEDTUBE_T_IS_EMPTY_CONST)
    
    .def("is_unbounded", &SlicedTube<T>::is_unbounded,
      BOOL_SLICEDTUBE_T_IS_UNBOUNDED_CONST)
    
    .def("codomain", &SlicedTube<T>::codomain,
      T_SLICEDTUBE_T_CODOMAIN_CONST)
    
    .def("__call__", [](const SlicedTube<T>& x, const Interval& t)
        {
          return x(t);
        },
      T_SLICEDTUBE_T_OPERATORCALL_CONST_INTERVAL_REF_CONST,
      "t"_a)

    .def("enclosed_bounds", &SlicedTube<T>::enclosed_bounds,
      PAIR_TT_SLICEDTUBE_T_ENCLOSED_BOUNDS_CONST_INTERVAL_REF_CONST,
      "t"_a)

    .def("set", (void (SlicedTube<T>::*)(const T&)) &SlicedTube<T>::set,
      VOID_SLICEDTUBE_T_SET_CONST_T_REF,
      "codomain"_a)
    
    .def("set", (void (SlicedTube<T>::*)(const T&,double)) &SlicedTube<T>::set,
      VOID_SLICEDTUBE_T_SET_CONST_T_REF_DOUBLE,
      "codomain"_a, "t"_a)
    
    .def("set", (void (SlicedTube<T>::*)(const T&,const Interval&)) &SlicedTube<T>::set,
      VOID_SLICEDTUBE_T_SET_CONST_T_REF_CONST_INTERVAL_REF,
      "codomain"_a, "t"_a)
    
    .def("set_ith_slice", &SlicedTube<T>::set_ith_slice,
      VOID_SLICEDTUBE_T_SET_ITH_SLICE_CONST_T_REF_INDEX,
      "codomain"_a, "i"_a)
    
    .def("inflate", (const SlicedTube<T>& (SlicedTube<T>::*)(const double&)) &SlicedTube<T>::inflate,
      CONST_SLICEDTUBE_T_REF_SLICEDTUBE_T_INFLATE_CONST_V_REF,
      "rad"_a)
    
    .def("inflate", (const SlicedTube<T>& (SlicedTube<T>::*)(const SampledTraj<double>&)) &SlicedTube<T>::inflate,
      CONST_SLICEDTUBE_T_REF_SLICEDTUBE_T_INFLATE_CONST_SAMPLEDTRAJ_V_REF,
      "rad"_a)
  ;

  if constexpr(std::is_same_v<T,IntervalVector>)
  {
    exported_slicedtube_class

    .def("inflate", (const SlicedTube<T>& (SlicedTube<T>::*)(const Vector&)) &SlicedTube<T>::inflate,
      CONST_SLICEDTUBE_T_REF_SLICEDTUBE_T_INFLATE_CONST_V_REF,
      "rad"_a)
    
    .def("inflate", (const SlicedTube<T>& (SlicedTube<T>::*)(const SampledTraj<Vector>&)) &SlicedTube<T>::inflate,
      CONST_SLICEDTUBE_T_REF_SLICEDTUBE_T_INFLATE_CONST_SAMPLEDTRAJ_V_REF,
      "rad"_a)
    ;
  }

  exported_slicedtube_class

    .def(py::self == py::self,
      BOOL_SLICEDTUBE_T_OPERATOREQ_CONST_SLICEDTUBE_REF_CONST,
      "x"_a)

    .def(py::self != py::self,
      "x"_a)

    .def("as_function", &SlicedTube<T>::as_function,
      ANALYTICFUNCTION_TYPENAME_EXPRTYPE_T_TYPE_SLICEDTUBE_T_AS_FUNCTION_CONST)

    .def("all_reals_value", &SlicedTube<T>::all_reals_value,
      T_SLICEDTUBE_T_ALL_REALS_VALUE_CONST)

    .def("empty_value", &SlicedTube<T>::empty_value,
      T_SLICEDTUBE_T_EMPTY_VALUE_CONST)

    .def("__repr__", [](const SlicedTube<T>& x) {
          std::ostringstream stream;
          stream << x;
          return string(stream.str()); 
        },
      OSTREAM_REF_OPERATOROUT_OSTREAM_REF_CONST_SLICEDTUBE_T_REF)
  ;

  if constexpr(std::is_same_v<T,Interval> || std::is_same_v<T,IntervalVector>)
  {
    exported_slicedtube_class
    
      .def("__call__", [](const SlicedTube<T>& x, const Interval& t, const SlicedTube<T>& v)
          {
            return x(t,v);
          },
        T_SLICEDTUBE_T_OPERATORCALL_CONST_INTERVAL_REF_CONST_SLICEDTUBE_T_REF_CONST,
        "t"_a, "v"_a)

      .def("integral", (T (SlicedTube<T>::*)(const Interval&) const) &SlicedTube<T>::integral,
        T_SLICEDTUBE_T_INTEGRAL_CONST_INTERVAL_REF_CONST,
        "t"_a)
      
      .def("integral", (T (SlicedTube<T>::*)(const Interval&,const Interval&) const) &SlicedTube<T>::integral,
        T_SLICEDTUBE_T_INTEGRAL_CONST_INTERVAL_REF_CONST_INTERVAL_REF_CONST,
        "t1"_a, "t2"_a)
      
      .def("partial_integral", (std::pair<T,T> (SlicedTube<T>::*)(const Interval&) const) &SlicedTube<T>::partial_integral,
        PAIR_TT_SLICEDTUBE_T_PARTIAL_INTEGRAL_CONST_INTERVAL_REF_CONST,
        "t"_a)
      
      .def("partial_integral", (std::pair<T,T> (SlicedTube<T>::*)(const Interval&,const Interval&) const) &SlicedTube<T>::partial_integral,
        PAIR_TT_SLICEDTUBE_T_PARTIAL_INTEGRAL_CONST_INTERVAL_REF_CONST_INTERVAL_REF_CONST,
        "t1"_a, "t2"_a)

      .def("primitive", (SlicedTube<T> (SlicedTube<T>::*)() const) &SlicedTube<T>::primitive,
        SLICEDTUBE_T_SLICEDTUBE_T_PRIMITIVE_CONST)

      .def("primitive", (SlicedTube<T> (SlicedTube<T>::*)(const T&) const) &SlicedTube<T>::primitive,
        SLICEDTUBE_T_SLICEDTUBE_T_PRIMITIVE_CONST_T_REF_CONST,
        "x0"_a)

      .def("invert", (Interval (SlicedTube<T>::*)(const T&) const) &SlicedTube<T>::invert,
        INTERVAL_SLICEDTUBE_T_INVERT_CONST_T_REF_CONST,
        "y"_a)

      .def("invert", (Interval (SlicedTube<T>::*)(const T&,const Interval&) const) &SlicedTube<T>::invert,
        INTERVAL_SLICEDTUBE_T_INVERT_CONST_T_REF_CONST_INTERVAL_REF_CONST,
        "y"_a, "t"_a)

      .def("invert", [](const SlicedTube<T>& x, const T& y, py::list& v_t)
          {
            vector<Interval> vector_t;
            x.invert(y, vector_t);
            v_t.clear();
            for(const auto& ti : vector_t)
              v_t.append(ti);
          },
        VOID_SLICEDTUBE_T_INVERT_CONST_T_REF_VECTOR_INTERVAL_REF_CONST,
        "y"_a, "v_t"_a)
      
      .def("invert", [](const SlicedTube<T>& x, const T& y, py::list& v_t, const Interval& t)
          {
            vector<Interval> vector_t;
            x.invert(y, vector_t, t);
            v_t.clear();
            for(const auto& ti : vector_t)
              v_t.append(ti);
          },
        VOID_SLICEDTUBE_T_INVERT_CONST_T_REF_VECTOR_INTERVAL_REF_CONST_INTERVAL_REF_CONST,
        "y"_a, "v_t"_a, "t"_a)
      
      .def("invert", (Interval (SlicedTube<T>::*)(const T&,const SlicedTube<T>&) const) &SlicedTube<T>::invert,
        INTERVAL_SLICEDTUBE_T_INVERT_CONST_T_REF_CONST_SLICEDTUBE_T_REF_CONST,
        "y"_a, "v"_a)
      
      .def("invert", (Interval (SlicedTube<T>::*)(const T&,const SlicedTube<T>&,const Interval&) const) &SlicedTube<T>::invert,
        INTERVAL_SLICEDTUBE_T_INVERT_CONST_T_REF_CONST_SLICEDTUBE_T_REF_CONST_INTERVAL_REF_CONST,
        "y"_a, "v"_a, "t"_a)
      
      .def("invert", [](const SlicedTube<T>& x, const T& y, py::list& v_t, const SlicedTube<T>& v)
          {
            vector<Interval> vector_t;
            x.invert(y, vector_t, v);
            v_t.clear();
            for(const auto& ti : vector_t)
              v_t.append(ti);
          },
        VOID_SLICEDTUBE_T_INVERT_CONST_T_REF_VECTOR_INTERVAL_REF_CONST_SLICEDTUBE_T_REF_CONST,
        "y"_a, "v_t"_a, "v"_a)
      
      .def("invert", [](const SlicedTube<T>& x, const T& y, py::list& v_t, const SlicedTube<T>& v, const Interval& t)
          {
            vector<Interval> vector_t;
            x.invert(y, vector_t, v, t);
            v_t.clear();
            for(const auto& ti : vector_t)
              v_t.append(ti);
          },
        VOID_SLICEDTUBE_T_INVERT_CONST_T_REF_VECTOR_INTERVAL_REF_CONST_SLICEDTUBE_T_REF_CONST_INTERVAL_REF_CONST,
        "y"_a, "v_t"_a, "v"_a, "t"_a)

      .def("mid", &SlicedTube<T>::mid,
        SAMPLEDTRAJ_TYPENAME_SCALAR_T_TYPE_SLICEDTUBE_T_MID_CONST);
    ;
  }

  if constexpr(std::is_same_v<T,IntervalVector>)
  {
    exported_slicedtube_class

      .def(
          #if FOR_MATLAB
            "get_item"
          #else
            "__getitem__"
          #endif
          ,
          [](const SlicedTube<IntervalVector>& x, Index_type i) -> SlicedTube<Interval>
          {
            matlab::test_integer(i);
            return x[matlab::input_index(i)];
          },
        SLICEDTUBE_INTERVAL_SLICEDTUBE_T_OPERATORCOMPO_INDEX_CONST,
        "i"_a)
        
      .def("get_item_0",
          [](const SlicedTube<IntervalVector>& x, Index_type i) -> SlicedTube<Interval>
          {
            matlab::test_integer(i);
            return x[i];
          },
        SLICEDTUBE_INTERVAL_SLICEDTUBE_T_OPERATORCOMPO_INDEX_CONST,
        "i"_a)

      .def("subvector",
          [](const SlicedTube<IntervalVector>& x, Index_type i, Index_type j) -> SlicedTube<IntervalVector>
          {
            matlab::test_integer(i,j);
            return x.subvector(matlab::input_index(i),matlab::input_index(j));
          },
        SLICEDTUBE_INTERVALVECTOR_SLICEDTUBE_T_SUBVECTOR_INDEX_INDEX_CONST,
        "i"_a, "j"_a)

      .def("subvector_0",
          [](const SlicedTube<IntervalVector>& x, Index_type i, Index_type j) -> SlicedTube<IntervalVector>
          {
            matlab::test_integer(i,j);
            return x.subvector(i,j);
          },
        SLICEDTUBE_INTERVALVECTOR_SLICEDTUBE_T_SUBVECTOR_INDEX_INDEX_CONST,
        "i"_a, "j"_a)
    ;
  }

  return exported_slicedtube_class;
}