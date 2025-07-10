/** 
 *  Codac binding (unsupported)
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Maël Godard
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <codac2_template_tools.h>
 #include <codac2_py_cast.h>
#include <codac2_Vector.h>
#include <codac2_AnalyticFunction.h>
#include <codac2_Parallelepiped.h>
#include <codac2_peibos_capd.h>
#include "codac2_py_peibos_capd_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_IMapWrapper(py::module& m)
{
  py::class_<IMapWrapper> 
    exported(m, "IMapWrapper", IMAPWRAPPER_MAIN);
  exported

  .def(py::init<const string&>(),
    IMAPWRAPPER_IMAPWRAPPER_CONST_STRING_REF,
    "s"_a)

  .def("setParameter", 
    static_cast<void (IMapWrapper::*)(const std::string&, double)>(&IMapWrapper::setParameter),
    VOID_IMAPWRAPPER_SETPARAMETER_CONST_STRING_REF_DOUBLE,
    "name"_a, "value"_a)

  .def("setParameter", 
    static_cast<void (IMapWrapper::*)(const std::string&, Interval)>(&IMapWrapper::setParameter),
    VOID_IMAPWRAPPER_SETPARAMETER_CONST_STRING_REF_INTERVAL,
    "name"_a, "value"_a)

  .def_readonly("gamma", &IMapWrapper::gamma,
    CAPD_IMAP_IMAPWRAPPER_GAMMA)
  
  ;
}

void export_peibos_capd(py::module& m)
{
  m.def("PEIBOS", 
    [](const IMapWrapper& i_map_wrapper, double tf, const py::object& psi_0, const vector<vector<int>>& generators, double epsilon, bool verbose = false)
    {
      return PEIBOS(i_map_wrapper, tf, cast<AnalyticFunction<VectorType>>(psi_0), generators, epsilon, verbose);
    },
    VECTOR_PARALLELEPIPED_PEIBOS_CONST_IMAPWRAPPER_REF_DOUBLE_CONST_ANALYTICFUNCTION_VECTORTYPE_REF_CONST_VECTOR_VECTOR_INT_REF_DOUBLE_BOOL,
    "i_map_wrapper"_a, "tf"_a, "psi_0"_a, "generators"_a, "epsilon"_a, "verbose"_a = false);

  m.def("PEIBOS", 
    [](const IMapWrapper& i_map_wrapper, double tf, const py::object& psi_0, const vector<vector<int>>& generators, double epsilon, const Vector& offset, bool verbose = false)
    {
      return PEIBOS(i_map_wrapper, tf, cast<AnalyticFunction<VectorType>>(psi_0), generators, epsilon, offset, verbose);
    },
    VECTOR_PARALLELEPIPED_PEIBOS_CONST_IMAPWRAPPER_REF_DOUBLE_CONST_ANALYTICFUNCTION_VECTORTYPE_REF_CONST_VECTOR_VECTOR_INT_REF_DOUBLE_CONST_VECTOR_REF_BOOL,
    "i_map_wrapper"_a, "tf"_a, "psi_0"_a, "generators"_a, "epsilon"_a, "offset"_a, "verbose"_a = false);

  m.def("PEIBOS", 
    [](const IMapWrapper& i_map_wrapper, double tf, double dt, const py::object& psi_0, const vector<vector<int>>& generators, double epsilon, bool verbose = false)
    {
      return PEIBOS(i_map_wrapper, tf, dt, cast<AnalyticFunction<VectorType>>(psi_0), generators, epsilon, verbose);
    },
    MAP_DOUBLEVECTOR_PARALLELEPIPED_PEIBOS_CONST_IMAPWRAPPER_REF_DOUBLE_DOUBLE_CONST_ANALYTICFUNCTION_VECTORTYPE_REF_CONST_VECTOR_VECTOR_INT_REF_DOUBLE_BOOL,
    "i_map_wrapper"_a, "tf"_a, "dt"_a, "psi_0"_a, "generators"_a, "epsilon"_a, "verbose"_a = false);

  m.def("PEIBOS", 
    [](const IMapWrapper& i_map_wrapper, double tf, double dt, const py::object& psi_0, const vector<vector<int>>& generators, double epsilon, const Vector& offset, bool verbose = false)
    {
      return PEIBOS(i_map_wrapper, tf, dt, cast<AnalyticFunction<VectorType>>(psi_0), generators, epsilon, offset, verbose);
    },
    MAP_DOUBLEVECTOR_PARALLELEPIPED_PEIBOS_CONST_IMAPWRAPPER_REF_DOUBLE_DOUBLE_CONST_ANALYTICFUNCTION_VECTORTYPE_REF_CONST_VECTOR_VECTOR_INT_REF_DOUBLE_CONST_VECTOR_REF_BOOL,
    "i_map_wrapper"_a, "tf"_a, "dt"_a, "psi_0"_a, "generators"_a, "epsilon"_a, "offset"_a, "verbose"_a = false);
}