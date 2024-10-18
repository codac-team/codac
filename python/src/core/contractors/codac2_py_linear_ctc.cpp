/** 
 *  Codac binding (core)
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <codac2_linear_ctc.h>
#include "codac2_py_linear_ctc_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;


void export_linear_ctc(py::module& m)
{
  py::class_<CtcGaussElim> exported_gauss_elim(m, "CtcGaussElim", CTCGAUSSELIM_MAIN);
  exported_gauss_elim

    .def(py::init(),
      CTCGAUSSELIM_CTCGAUSSELIM)

    .def("contract", &CtcGaussElim::contract,
      VOID_CTCGAUSSELIM_CONTRACT_INTERVALMATRIX_REF_INTERVALVECTOR_REF_INTERVALVECTOR_REF_CONST,
      "A"_a, "x"_a, "b"_a)
    
  ;

  py::class_<CtcGaussSeidel> exported_gauss_seidel(m, "CtcGaussSeidel", CTCGAUSSSEIDEL_MAIN);
  exported_gauss_seidel

    .def(py::init(),
      CTCGAUSSSEIDEL_CTCGAUSSSEIDEL)

    .def("contract", &CtcGaussSeidel::contract,
      VOID_CTCGAUSSSEIDEL_CONTRACT_INTERVALMATRIX_REF_INTERVALVECTOR_REF_INTERVALVECTOR_REF_CONST,
      "A"_a, "x"_a, "b"_a)
    
  ;

  // With preconditioning

  py::class_<CtcLinearPrecond> exported_precond_gauss_elim(m, "CtcLinearPrecond", CTCLINEARPRECOND_MAIN);
  exported_precond_gauss_elim

    .def(py::init(
        [](const CtcGaussElim& c)
        {
          return std::make_unique<CtcLinearPrecond>(c.copy());
        }),
      CTCLINEARPRECOND_CTCLINEARPRECOND_CONST_C_REF)

    .def(py::init(
        [](const CtcGaussSeidel& c)
        {
          return std::make_unique<CtcLinearPrecond>(c.copy());
        }),
      CTCLINEARPRECOND_CTCLINEARPRECOND_CONST_C_REF)

    .def("contract", &CtcLinearPrecond::contract,
      VOID_CTCLINEARPRECOND_CONTRACT_INTERVALMATRIX_REF_INTERVALVECTOR_REF_INTERVALVECTOR_REF_CONST,
      "A"_a, "x"_a, "b"_a)
    
  ;
}