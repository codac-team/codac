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
#include "codac2_py_core.h"
#include <codac2_IntervalVector.h>
#include "codac2_py_IntervalVector_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)
#include "codac2_py_IntervalVectorTemplate_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)
#include "codac2_py_IntervalMatrixTemplate_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

py::class_<IntervalVector> export_IntervalVector(py::module& m)
{
  py::class_<IntervalVector> exported_intervalvector_class(m, "IntervalVector", INTERVALVECTOR_MAIN);
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

    .def(py::init<const Interval&>(),
      INTERVALVECTOR_INTERVALVECTOR_CONST_INTERVAL_REF,
      "x"_a)

    .def(py::init<const Vector&>(),
      INTERVALVECTOR_INTERVALVECTOR_CONST_VECTOR__M_REF,
      "v"_a)

    // IntervalVector(size_t_type n, const double bounds[][2])

    .def(py::init<const Vector&,const Vector&>(),
      INTERVALVECTOR_INTERVALVECTOR_CONST_VECTOR_REF_CONST_VECTOR_REF,
      "lb"_a, "ub"_a)

    .def(py::init(
        [](const std::vector<Interval>& v)
        {
          auto iv = std::make_unique<IntervalVector>(v.size());
          for(size_t i = 0 ; i < v.size() ; i++)
            (*iv)[i] = v[i];
          return iv;
        }),
      INTERVALVECTOR_INTERVALVECTOR_INITIALIZER_LIST_INTERVAL,
      "v"_a)

    .def(py::init(
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
  
    .def(py::init<const IntervalVector&>(),
      INTERVALVECTOR_INTERVALVECTOR_CONST_INTERVALVECTOR_REF,
      "x"_a)

    .def("transpose", &IntervalVector::transpose,
      INTERVALMATRIX_INTERVALVECTOR_TRANSPOSE_CONST)

    .def("as_diag", &IntervalVector::as_diag,
      INTERVALMATRIX_INTERVALVECTOR_AS_DIAG_CONST)







    

    .def(py::self |= py::self,
      AUTO_INTERVALMATRIXTEMPLATE__SVRC_OPERATOROREQ_CONST_S_REF,
      "x"_a)

    // For MATLAB compatibility
    .def("self_union", &IntervalVector::operator|=,
      AUTO_INTERVALMATRIXTEMPLATE__SVRC_OPERATOROREQ_CONST_S_REF,
      "x"_a)

    .def(py::self &= py::self,
      AUTO_INTERVALMATRIXTEMPLATE__SVRC_OPERATORANDEQ_CONST_S_REF,
      "x"_a)

    // For MATLAB compatibility
    .def("self_inter", &IntervalVector::operator&=,
      AUTO_INTERVALMATRIXTEMPLATE__SVRC_OPERATORANDEQ_CONST_S_REF,
      "x"_a)

    .def(py::self += py::self,
      INTERVALVECTOR_INTERVALVECTOR_OPERATORPLUSEQ_CONST_EIGEN_MATRIXBASE_OTHERDERIVED_REF,
      "x"_a)

    //.def(py::self += Vector(),
    //  INTERVALVECTOR_INTERVALVECTOR_OPERATORPLUSEQ_CONST_VECTOR_REF,
    //  "x"_a)

    .def(-py::self,
      INTERVALVECTOR_INTERVALVECTOR_OPERATORMINUS_CONST)

    .def(py::self -= py::self,
      INTERVALVECTOR_INTERVALVECTOR_OPERATORMINUSEQ_CONST_EIGEN_MATRIXBASE_OTHERDERIVED_REF,
      "x"_a)

    //.def(py::self -= Vector(),
    //  INTERVALVECTOR_INTERVALVECTOR_OPERATORMINUSEQ_CONST_VECTOR_REF,
    //  "x"_a)

    .def("__repr__", [](const IntervalVector& x) {
          std::ostringstream stream;
          stream << x;
          return string(stream.str()); 
        },
      OSTREAM_REF_OPERATOROUT_OSTREAM_REF_CONST_INTERVALVECTORTEMPLATE__SVN_REF)
  ;

  py::implicitly_convertible<py::list,IntervalVector>();
  return exported_intervalvector_class;
}