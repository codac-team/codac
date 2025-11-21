/** 
 *  SampledTraj binding
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
#include <pybind11/numpy.h>
#include <codac2_SampledTraj.h>
#include <codac2_Traj_operator.h>
#include <codac2_SampledTraj_operations.h>
#include "codac2_py_SampledTraj_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):
#include "codac2_py_TrajBase_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):
#include "codac2_py_SampledTraj_operations_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):
#include "codac2_py_TrajBase.h"

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

template<typename T>
py::class_<SampledTraj<T>> _export_SampledTraj(py::module& m, const string& class_name)
{
  py::class_<SampledTraj<T>> exported_class(m, class_name.c_str(), SAMPLEDTRAJ_MAIN);
  export_TrajBase<SampledTraj<T>,T>(exported_class);

  exported_class

    .def(py::init<>(),
      SAMPLEDTRAJ_T_SAMPLEDTRAJ)
    
  ;

  if constexpr(std::is_same_v<T,double>)
  {
    exported_class
      .def(py::init<const std::list<double>&,const std::list<T>&>(),
        SAMPLEDTRAJ_T_SAMPLEDTRAJ_CONST_LIST_DOUBLE_REF_CONST_LIST_T_REF,
        "l_t"_a, "l_x"_a)
    ;
  }

  else if constexpr(std::is_same_v<T,Vector>)
  {
    exported_class

      .def(py::init(
          [](const py::array_t<double>& l_t, const py::array_t<double>& l_x)
            {
              if constexpr(std::is_same_v<T,Vector>)
              {
                assert_release(l_t.size() > 0 && l_x.size() > 0);

                list<Vector> l_vx;

                py::buffer_info info = l_x.request();
                auto ptr = static_cast<double*>(info.ptr);

                size_t nb_values = 1;
                for(const auto& r : info.shape)
                  nb_values *= r;
                
                assert(nb_values % l_t.size() == 0);
                size_t n = nb_values / l_t.size();

                for(size_t i = 0 ; i < nb_values ; i+=n)
                {
                  Vector x(n);
                  for(size_t k = 0 ; k < n ; k++)
                    x[k] = *ptr++;
                  l_vx.push_back(x);
                }

                return std::make_unique<SampledTraj<T>>(l_t.cast<std::list<double>>(), l_vx);
              }

              else
                return std::make_unique<SampledTraj<T>>();
            }),
        SAMPLEDTRAJ_T_SAMPLEDTRAJ_CONST_LIST_DOUBLE_REF_CONST_LIST_T_REF,
        "l_t"_a, "l_x"_a)

    .def(
        #if FOR_MATLAB
          "__call__"
        #else
          "__getitem__"
        #endif
        , [](const SampledTraj<T>& x, Index_type i) -> SampledTraj<double>
        {
          matlab::test_integer(i);
          return x[matlab::input_index(i)];
        }, py::return_value_policy::reference_internal)

    .def("subvector", [](const SampledTraj<T>& x, Index_type i, Index_type j) -> SampledTraj<Vector>
        {
          matlab::test_integer(i,j);
          return x.subvector(matlab::input_index(i),matlab::input_index(j));
        }, py::return_value_policy::reference_internal)
    ;
  }

  exported_class

    .def(py::init<const std::map<double,T>&>(),
      SAMPLEDTRAJ_T_SAMPLEDTRAJ_CONST_MAP_DOUBLET_REF,
      "m"_a)

    .def("nb_samples", &SampledTraj<T>::nb_samples,
      SIZET_SAMPLEDTRAJ_T_NB_SAMPLES_CONST)

    .def("__len__", &SampledTraj<T>::nb_samples,
      SIZET_SAMPLEDTRAJ_T_NB_SAMPLES_CONST)

    .def("__iter__", [](const SampledTraj<T>& x)
        {
          return py::make_iterator(x.begin(), x.end());
        },
        py::keep_alive<0, 1>()) // essential: keep object alive while iterator exists

    .def("sampled", [](const SampledTraj<T>& x, double dt, bool keep_original_values)
        {
          return x.sampled(dt,keep_original_values);
        },
      SAMPLEDTRAJ_T_SAMPLEDTRAJ_T_SAMPLED_DOUBLE_BOOL_CONST,
      "dt"_a, "keep_original_values"_a)

    .def("sampled_as", [](const SampledTraj<T>& this_traj, const SampledTraj<T>& x, bool keep_original_values)
        {
          return this_traj.sampled_as(x,keep_original_values);
        },
      SAMPLEDTRAJ_T_SAMPLEDTRAJ_T_SAMPLED_AS_CONST_SAMPLEDTRAJ_Q_REF_BOOL_CONST,
      "x"_a, "keep_original_values"_a)

    .def("shift_tdomain", &SampledTraj<T>::shift_tdomain,
      SAMPLEDTRAJ_T_REF_SAMPLEDTRAJ_T_SHIFT_TDOMAIN_DOUBLE,
      "shift"_a)

    .def("stretch_tdomain", &SampledTraj<T>::stretch_tdomain,
      SAMPLEDTRAJ_T_REF_SAMPLEDTRAJ_T_STRETCH_TDOMAIN_CONST_INTERVAL_REF,
      "tdomain"_a)

    .def("__call__", [](const SampledTraj<T>& x, double t) -> T
        {
          return x(t);
        },
      VIRTUAL_T_SAMPLEDTRAJ_T_OPERATORCALL_DOUBLE_CONST,
      "t"_a)

    .def("__call__", [](const SampledTraj<T>& x, const Interval& t) -> typename Wrapper<T>::Domain
        {
          return x(t);
        },
      VIRTUAL_WRAPPER_T_DOMAIN_SAMPLEDTRAJ_T_OPERATORCALL_CONST_INTERVAL_REF_CONST,
      "t"_a)

    .def("set", &SampledTraj<T>::set,
      VOID_SAMPLEDTRAJ_T_SET_CONST_T_REF_DOUBLE,
      "xi"_a, "ti"_a)

    .def("as_function", &SampledTraj<T>::as_function,
      ANALYTICFUNCTION_TYPE_SAMPLEDTRAJ_T_AS_FUNCTION_CONST)

    .def("primitive", (SampledTraj<T> (SampledTraj<T>::*)() const) &SampledTraj<T>::primitive,
      SAMPLEDTRAJ_T_SAMPLEDTRAJ_T_PRIMITIVE_CONST)

    .def("derivative", &SampledTraj<T>::derivative,
      SAMPLEDTRAJ_T_SAMPLEDTRAJ_T_DERIVATIVE_CONST)

    .def("mean", &SampledTraj<T>::mean,
      T_SAMPLEDTRAJ_T_MEAN_CONST)

    .def("__repr__", [](const SampledTraj<T>& x) {
          std::ostringstream stream;
          stream << x;
          return string(stream.str()); 
        },
      OSTREAM_REF_OPERATOROUT_OSTREAM_REF_CONST_SAMPLEDTRAJ_T_REF)
  ;

  return exported_class;
}

template<typename T>
void add_operators(py::class_<SampledTraj<T>>& pyclass)
{
  pyclass

    .def("__add__", [](const SampledTraj<T>& x1) { return x1; },
      CONST_SAMPLEDTRAJ_T_REF_OPERATORPLUS_CONST_SAMPLEDTRAJ_T_REF,
      py::is_operator())

    .def("__add__", [](const SampledTraj<T>& x1, const SampledTraj<T>& x2) { return x1+x2; },
      SAMPLEDTRAJ_T_OPERATORPLUS_CONST_SAMPLEDTRAJ_T_REF_CONST_SAMPLEDTRAJ_T_REF,
      py::is_operator())

    .def("__add__", [](const SampledTraj<T>& x1, const T& x2) { return x1+x2; },
      SAMPLEDTRAJ_T_OPERATORPLUS_CONST_SAMPLEDTRAJ_T_REF_CONST_Q_REF,
      py::is_operator())

    .def("__add__", [](const T& x1, const SampledTraj<T>& x2) { return x1+x2; },
      SAMPLEDTRAJ_T_OPERATORPLUS_CONST_Q_REF_CONST_SAMPLEDTRAJ_T_REF,
      py::is_operator())

    .def("__sub__", [](const SampledTraj<T>& x1) { return -x1; },
      SAMPLEDTRAJ_T_OPERATORMINUS_CONST_SAMPLEDTRAJ_T_REF,
      py::is_operator())

    .def("__sub__", [](const SampledTraj<T>& x1, const SampledTraj<T>& x2) { return x1-x2; },
      SAMPLEDTRAJ_T_OPERATORMINUS_CONST_SAMPLEDTRAJ_T_REF_CONST_SAMPLEDTRAJ_T_REF,
      py::is_operator())

    .def("__sub__", [](const SampledTraj<T>& x1, const T& x2) { return x1-x2; },
      SAMPLEDTRAJ_T_OPERATORMINUS_CONST_SAMPLEDTRAJ_T_REF_CONST_Q_REF,
      py::is_operator())

    .def("__sub__", [](const T& x1, const SampledTraj<T>& x2) { return x1-x2; },
      SAMPLEDTRAJ_T_OPERATORMINUS_CONST_Q_REF_CONST_SAMPLEDTRAJ_T_REF,
      py::is_operator())

    .def("__mul__", [](const SampledTraj<T>& x1, const SampledTraj<T>& x2) { return x1*x2; },
      SAMPLEDTRAJ_T_OPERATORMUL_CONST_SAMPLEDTRAJ_T_REF_CONST_SAMPLEDTRAJ_T_REF,
      py::is_operator())

    .def("__mul__", [](double x1, const SampledTraj<T>& x2) { return x1*x2; },
      SAMPLEDTRAJ_T_OPERATORMUL_DOUBLE_CONST_SAMPLEDTRAJ_T_REF,
      py::is_operator())

    .def("__mul__", [](const SampledTraj<T>& x1, double x2) { return x1*x2; },
      SAMPLEDTRAJ_T_OPERATORMUL_CONST_SAMPLEDTRAJ_T_REF_DOUBLE,
      py::is_operator())

    .def("__mul__", [](const T& x1, const SampledTraj<T>& x2) { return x1*x2; },
      SAMPLEDTRAJ_T_OPERATORMUL_CONST_Q_REF_CONST_SAMPLEDTRAJ_T_REF,
      py::is_operator())

    .def("__truediv__", [](const SampledTraj<T>& x1, double x2) { return x1/x2; },
      SAMPLEDTRAJ_T_OPERATORDIV_CONST_SAMPLEDTRAJ_T_REF_DOUBLE,
      py::is_operator())
  ;
}

void export_SampledTraj(py::module& m)
{
  auto py_SampledTraj_double = _export_SampledTraj<double>(m, "SampledScalarTraj");
  auto py_SampledTraj_Vector = _export_SampledTraj<Vector>(m, "SampledVectorTraj");
  auto py_SampledTraj_Matrix = _export_SampledTraj<Matrix>(m, "SampledMatrixTraj");

  add_operators<double>(py_SampledTraj_double);

  m.def("continuous_traj", &continuous_traj,
    SAMPLEDTRAJ_DOUBLE_CONTINUOUS_TRAJ_CONST_SAMPLEDTRAJ_DOUBLE_REF,
    "x"_a);

  py_SampledTraj_double

    .def(py::self * py::self,
      SAMPLEDTRAJ_T_OPERATORMUL_CONST_SAMPLEDTRAJ_T_REF_CONST_SAMPLEDTRAJ_T_REF,
      py::is_operator())

    .def(py::self / py::self,
      SAMPLEDTRAJ_T_OPERATORDIV_CONST_SAMPLEDTRAJ_T_REF_CONST_SAMPLEDTRAJ_T_REF,
      py::is_operator())

    .def(double() / py::self,
      SAMPLEDTRAJ_T_OPERATORDIV_CONST_Q_REF_CONST_SAMPLEDTRAJ_T_REF,
      py::is_operator())
  ;

  add_operators<Matrix>(py_SampledTraj_Matrix);
  add_operators<Vector>(py_SampledTraj_Vector);
  py_SampledTraj_Matrix

    .def("__mul__", [](const SampledTraj<Matrix>& x1, const SampledTraj<Vector>& x2) { return x1*x2; },
      SAMPLEDTRAJ_VECTOR_OPERATORMUL_CONST_SAMPLEDTRAJ_MATRIX_REF_CONST_SAMPLEDTRAJ_VECTOR_REF,
      py::is_operator())

    .def("__mul__", [](const SampledTraj<Matrix>& x1, const SampledTraj<Matrix>& x2) { return x1*x2; },
      SAMPLEDTRAJ_T_OPERATORMUL_CONST_SAMPLEDTRAJ_T_REF_CONST_SAMPLEDTRAJ_T_REF,
      py::is_operator())

  ;

  m

    .def("sqr", (SampledTraj<double> (*)(const SampledTraj<double>&)) &codac2::sqr,
      SAMPLEDTRAJ_DOUBLE_SQR_CONST_SAMPLEDTRAJ_DOUBLE_REF,
      "x1"_a, py::is_operator())

    .def("sqrt", (SampledTraj<double> (*)(const SampledTraj<double>&)) &codac2::sqrt,
      SAMPLEDTRAJ_DOUBLE_SQRT_CONST_SAMPLEDTRAJ_DOUBLE_REF,
      "x1"_a, py::is_operator())

    .def("pow", (SampledTraj<double> (*)(const SampledTraj<double>&,int)) &codac2::pow,
      SAMPLEDTRAJ_DOUBLE_POW_CONST_SAMPLEDTRAJ_DOUBLE_REF_INT,
      "x1"_a, "x2"_a, py::is_operator())

    .def("pow", (SampledTraj<double> (*)(const SampledTraj<double>&,double)) &codac2::pow,
      SAMPLEDTRAJ_DOUBLE_POW_CONST_SAMPLEDTRAJ_DOUBLE_REF_DOUBLE,
      "x1"_a, "x2"_a, py::is_operator())

    .def("root", (SampledTraj<double> (*)(const SampledTraj<double>&,int)) &codac2::root,
      SAMPLEDTRAJ_DOUBLE_ROOT_CONST_SAMPLEDTRAJ_DOUBLE_REF_INT,
      "x1"_a, "x2"_a, py::is_operator())

    .def("exp", (SampledTraj<double> (*)(const SampledTraj<double>&)) &codac2::exp,
      SAMPLEDTRAJ_DOUBLE_EXP_CONST_SAMPLEDTRAJ_DOUBLE_REF,
      "x1"_a, py::is_operator())

    .def("log", (SampledTraj<double> (*)(const SampledTraj<double>&)) &codac2::log,
      SAMPLEDTRAJ_DOUBLE_LOG_CONST_SAMPLEDTRAJ_DOUBLE_REF,
      "x1"_a, py::is_operator())

    .def("cos", (SampledTraj<double> (*)(const SampledTraj<double>&)) &codac2::cos,
      SAMPLEDTRAJ_DOUBLE_COS_CONST_SAMPLEDTRAJ_DOUBLE_REF,
      "x1"_a, py::is_operator())

    .def("sin", (SampledTraj<double> (*)(const SampledTraj<double>&)) &codac2::sin,
      SAMPLEDTRAJ_DOUBLE_SIN_CONST_SAMPLEDTRAJ_DOUBLE_REF,
      "x1"_a, py::is_operator())

    .def("tan", (SampledTraj<double> (*)(const SampledTraj<double>&)) &codac2::tan,
      SAMPLEDTRAJ_DOUBLE_TAN_CONST_SAMPLEDTRAJ_DOUBLE_REF,
      "x1"_a, py::is_operator())

    .def("acos", (SampledTraj<double> (*)(const SampledTraj<double>&)) &codac2::acos,
      SAMPLEDTRAJ_DOUBLE_ACOS_CONST_SAMPLEDTRAJ_DOUBLE_REF,
      "x1"_a, py::is_operator())

    .def("asin", (SampledTraj<double> (*)(const SampledTraj<double>&)) &codac2::asin,
      SAMPLEDTRAJ_DOUBLE_ASIN_CONST_SAMPLEDTRAJ_DOUBLE_REF,
      "x1"_a, py::is_operator())

    .def("atan", (SampledTraj<double> (*)(const SampledTraj<double>&)) &codac2::atan,
      SAMPLEDTRAJ_DOUBLE_ATAN_CONST_SAMPLEDTRAJ_DOUBLE_REF,
      "x1"_a, py::is_operator())

    .def("atan2", (SampledTraj<double> (*)(const SampledTraj<double>&,const SampledTraj<double>&)) &codac2::atan2,
      SAMPLEDTRAJ_DOUBLE_ATAN2_CONST_SAMPLEDTRAJ_DOUBLE_REF_CONST_SAMPLEDTRAJ_DOUBLE_REF,
      "x1"_a, "x2"_a, py::is_operator())

    .def("atan2", (SampledTraj<double> (*)(const SampledTraj<double>&,double)) &codac2::atan2,
      SAMPLEDTRAJ_DOUBLE_ATAN2_CONST_SAMPLEDTRAJ_DOUBLE_REF_DOUBLE,
      "x1"_a, "x2"_a, py::is_operator())

    .def("atan2", (SampledTraj<double> (*)(double,const SampledTraj<double>&)) &codac2::atan2,
      SAMPLEDTRAJ_DOUBLE_ATAN2_DOUBLE_CONST_SAMPLEDTRAJ_DOUBLE_REF,
      "x1"_a, "x2"_a, py::is_operator())

    .def("cosh", (SampledTraj<double> (*)(const SampledTraj<double>&)) &codac2::cosh,
      SAMPLEDTRAJ_DOUBLE_COSH_CONST_SAMPLEDTRAJ_DOUBLE_REF,
      "x1"_a, py::is_operator())

    .def("sinh", (SampledTraj<double> (*)(const SampledTraj<double>&)) &codac2::sinh,
      SAMPLEDTRAJ_DOUBLE_SINH_CONST_SAMPLEDTRAJ_DOUBLE_REF,
      "x1"_a, py::is_operator())

    .def("tanh", (SampledTraj<double> (*)(const SampledTraj<double>&)) &codac2::tanh,
      SAMPLEDTRAJ_DOUBLE_TANH_CONST_SAMPLEDTRAJ_DOUBLE_REF,
      "x1"_a, py::is_operator())

    .def("acosh", (SampledTraj<double> (*)(const SampledTraj<double>&)) &codac2::acosh,
      SAMPLEDTRAJ_DOUBLE_ACOSH_CONST_SAMPLEDTRAJ_DOUBLE_REF,
      "x1"_a, py::is_operator())

    .def("asinh", (SampledTraj<double> (*)(const SampledTraj<double>&)) &codac2::asinh,
      SAMPLEDTRAJ_DOUBLE_ASINH_CONST_SAMPLEDTRAJ_DOUBLE_REF,
      "x1"_a, py::is_operator())

    .def("atanh", (SampledTraj<double> (*)(const SampledTraj<double>&)) &codac2::atanh,
      SAMPLEDTRAJ_DOUBLE_ATANH_CONST_SAMPLEDTRAJ_DOUBLE_REF,
      "x1"_a, py::is_operator())

    .def("abs", (SampledTraj<double> (*)(const SampledTraj<double>&)) &codac2::abs,
      SAMPLEDTRAJ_DOUBLE_ABS_CONST_SAMPLEDTRAJ_DOUBLE_REF,
      "x1"_a, py::is_operator())

    .def("min", (SampledTraj<double> (*)(const SampledTraj<double>&,const SampledTraj<double>&)) &codac2::min,
      SAMPLEDTRAJ_DOUBLE_MIN_CONST_SAMPLEDTRAJ_DOUBLE_REF_CONST_SAMPLEDTRAJ_DOUBLE_REF,
      "x1"_a, "x2"_a, py::is_operator())

    .def("min", (SampledTraj<double> (*)(const SampledTraj<double>&,double)) &codac2::min,
      SAMPLEDTRAJ_DOUBLE_MIN_CONST_SAMPLEDTRAJ_DOUBLE_REF_DOUBLE,
      "x1"_a, "x2"_a, py::is_operator())

    .def("min", (SampledTraj<double> (*)(double,const SampledTraj<double>&)) &codac2::min,
      SAMPLEDTRAJ_DOUBLE_MIN_DOUBLE_CONST_SAMPLEDTRAJ_DOUBLE_REF,
      "x1"_a, "x2"_a, py::is_operator())

    .def("max", (SampledTraj<double> (*)(const SampledTraj<double>&,const SampledTraj<double>&)) &codac2::max,
      SAMPLEDTRAJ_DOUBLE_MAX_CONST_SAMPLEDTRAJ_DOUBLE_REF_CONST_SAMPLEDTRAJ_DOUBLE_REF,
      "x1"_a, "x2"_a, py::is_operator())

    .def("max", (SampledTraj<double> (*)(const SampledTraj<double>&,double)) &codac2::max,
      SAMPLEDTRAJ_DOUBLE_MAX_CONST_SAMPLEDTRAJ_DOUBLE_REF_DOUBLE,
      "x1"_a, "x2"_a, py::is_operator())

    .def("max", (SampledTraj<double> (*)(double,const SampledTraj<double>&)) &codac2::max,
      SAMPLEDTRAJ_DOUBLE_MAX_DOUBLE_CONST_SAMPLEDTRAJ_DOUBLE_REF,
      "x1"_a, "x2"_a, py::is_operator())

    .def("sign", (SampledTraj<double> (*)(const SampledTraj<double>&)) &codac2::sign,
      SAMPLEDTRAJ_DOUBLE_SIGN_CONST_SAMPLEDTRAJ_DOUBLE_REF,
      "x1"_a, py::is_operator())

    .def("integer", (SampledTraj<double> (*)(const SampledTraj<double>&)) &codac2::integer,
      SAMPLEDTRAJ_DOUBLE_INTEGER_CONST_SAMPLEDTRAJ_DOUBLE_REF,
      "x1"_a, py::is_operator())

    .def("floor", (SampledTraj<double> (*)(const SampledTraj<double>&)) &codac2::floor,
      SAMPLEDTRAJ_DOUBLE_FLOOR_CONST_SAMPLEDTRAJ_DOUBLE_REF,
      "x1"_a, py::is_operator())

    .def("ceil", (SampledTraj<double> (*)(const SampledTraj<double>&)) &codac2::ceil,
      SAMPLEDTRAJ_DOUBLE_CEIL_CONST_SAMPLEDTRAJ_DOUBLE_REF,
      "x1"_a, py::is_operator())

  ;
}