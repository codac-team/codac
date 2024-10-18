/** 
 *  Interval Python binding
 *  Originated from the former pyIbex library (Benoît Desrochers)
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Benoît Desrochers, Simon Rohou, Fabrice Le Bars
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <sstream>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <codac2_IntervalVector.h>

#include "codac2_py_VectorBase_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)
#include "codac2_py_MatrixBase_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)
#include "codac2_py_IntervalMatrixBase_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)
#include "codac2_py_IntervalVector_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)

#include "codac2_py_VectorBase.h"
#include "codac2_py_IntervalMatrixBase.h"

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

py::class_<IntervalVector> export_IntervalVector(py::module& m)
{
  py::class_<IntervalVector> exported_intervalvector_class(m, "IntervalVector", INTERVALVECTOR_MAIN);
  export_IntervalMatrixBase<IntervalVector,Vector,true>(m, exported_intervalvector_class);
  export_VectorBase<IntervalVector,IntervalMatrix,Interval>(m, exported_intervalvector_class);

  exported_intervalvector_class

    .def(py::init(
        [](size_t_type n)
        {
          matlab::test_integer(n);
          return std::make_unique<IntervalVector>(n);
        }),
      INTERVALVECTOR_INTERVALVECTOR_SIZET,
      "n"_a)

    .def(py::init(
        [](size_t_type n, const Interval& x)
        {
          matlab::test_integer(n);
          return std::make_unique<IntervalVector>(n,x);
        }),
      INTERVALVECTOR_INTERVALVECTOR_SIZET_CONST_INTERVAL_REF,
      "n"_a, "x"_a)

    .def(py::init<const IntervalVector&>(),
      "x"_a)

    .def(py::init<const Vector&>(),
      INTERVALVECTOR_INTERVALVECTOR_CONST_VECTOR_REF,
      "x"_a)

    .def(py::init<const Vector&,const Vector&>(),
      INTERVALVECTOR_INTERVALVECTOR_CONST_VECTOR_REF_CONST_VECTOR_REF,
      "lb"_a, "ub"_a)

    .def(py::init( // this constructor must be the last one to be declared
        [](const std::vector<Interval>& v)
        {
          auto iv = std::make_unique<IntervalVector>(v.size());
          for(size_t i = 0 ; i < v.size() ; i++)
            (*iv)[i] = v[i];
          return iv;
        }),
      INTERVALVECTOR_INTERVALVECTOR_INITIALIZER_LIST_INTERVAL,
      "v"_a)

    .def(py::init( // this constructor must be the last one to be declared
        [](const std::vector<std::vector<double>>& v)
        {
          auto iv = std::make_unique<IntervalVector>(v.size());
          for(size_t i = 0 ; i < v.size() ; i++)
          {
            if(v[i].size() == 1)
              (*iv)[i] = Interval(v[i][0]);
            else if(v[i].size() == 2)
              (*iv)[i] = Interval(v[i][0],v[i][1]);
            else
              throw invalid_argument("Interval is not made of one or two values.");
          }
          return iv;
        }),
      INTERVALVECTOR_INTERVALVECTOR_INITIALIZER_LIST_INTERVAL,
      "v"_a)

    .def("complementary", &IntervalVector::complementary,
      LIST_INTERVALVECTOR_INTERVALVECTOR_COMPLEMENTARY_CONST)

    .def("diff", &IntervalVector::diff,
      LIST_INTERVALVECTOR_INTERVALVECTOR_DIFF_CONST_INTERVALVECTOR_REF_BOOL_CONST,
      "y"_a, "compactness"_a = true)
    
    .def_static("empty", [](size_t_type n)
        {
          matlab::test_integer(n);
          return IntervalVector::empty(n);
        },
      STATIC_INTERVALVECTOR_INTERVALVECTOR_EMPTY_SIZET,
      "n"_a)

    .def("__repr__", [](const IntervalVector& x)
        {
          std::ostringstream s;
          s << x;
          return string(s.str()); 
        },
      OSTREAM_REF_OPERATOROUT_OSTREAM_REF_CONST_INTERVALVECTOR_REF)

  ;

  py::implicitly_convertible<py::list,IntervalVector>();

  m.def("cart_prod_boxes", [](const std::list<IntervalVector>& l)
      {
        IntervalVector c = *l.begin();
        for(auto it = std::next(l.cbegin()); it != l.cend(); it++)
          c = cart_prod(c,*it);
        return c;
      },
    INTERVALVECTOR_CART_PROD_CONST_X_REF_VARIADIC);
  // The variadic version of this function is defined in __init__.py file

  m.def("hull", &codac2::hull,
    INTERVALVECTOR_HULL_CONST_LIST_INTERVALVECTOR_REF,
    "l"_a);

  return exported_intervalvector_class;
}