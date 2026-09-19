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
#include <codac2_template_tools.h>
#include <codac2_CtcLazy.h>
#include "codac2_py_Ctc.h"
#include "codac2_py_CtcLazy_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_CtcLazy(py::module& m, py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector>& pyctc)
{
  py::class_<CtcLazy> exported(m, "CtcLazy", pyctc, CTCLAZY_MAIN);
  exported

    .def(py::init(
        // The contractor is taken as a CtcBase<IntervalVector>, and not as a
        // pyCtcIntervalVector as it used to be. pyCtcIntervalVector is only the
        // trampoline class of the binding: the contractors written in Python are
        // instances of it, those written in C++ (CtcInverse, CtcWrapper...) are not.
        // pybind11 files the trampoline type under the same registration as its base,
        // so it accepted any contractor for a pyCtcIntervalVector& and treated it as
        // one: given a C++ contractor, c referred to an object of another type, and
        // c.copy() was a member call through a reference of the wrong type, which is
        // undefined behaviour (UBSan's vptr check reports it). Through the base class,
        // copy() is an ordinary virtual call, which reaches the right override
        // whether the contractor comes from C++ or from Python.
        [](const CtcBase<IntervalVector>& c)
        {
          return std::make_unique<CtcLazy>(c.copy());
        }),
      CTCLAZY_CTCLAZY_CONST_C_REF,
      "c"_a)
  ;

  CONTRACT_METHODS(exported, CtcLazy,
    VOID_CTCLAZY_CONTRACT_INTERVALVECTOR_REF_CONST)
}