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
#include <codac2_CtcUnion.h>
#include "codac2_py_Ctc.h"
#include "codac2_py_CtcUnion_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_CtcUnion(py::module& m, py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector>& pyctc)
{
  py::class_<CtcUnion<IntervalVector>> exported(m, "CtcUnion", pyctc, CTCUNION_MAIN);
  exported

    .def(py::init<Index>(),
      CTCUNION_X_CTCUNION_INDEX,
      "n"_a)

    .def(py::init(
        [](py::args args)
        {
          Collection<CtcBase<IntervalVector>> ctcs;

          auto add = [&ctcs](py::handle c)
          {
            ctcs.push_back(
              c.cast<CtcBase<IntervalVector>&>().copy()
            );
          };

          // Backward compatibility: CtcUnion([c1,c2,c3])
          if(args.size() == 1 && py::isinstance<py::list>(args[0]))
          {
            for(const auto& c : args[0].cast<py::list>())
              add(c);
          }
          else
          {
            // New syntax: CtcUnion(c1,c2,c3)
            for(const auto& c : args)
              add(c);
          }

          return std::make_unique<CtcUnion<IntervalVector>>(ctcs);
        }),
      CTCUNION_X_CTCUNION_INITIALIZER_LIST_C)

    .def("nb", &CtcUnion<IntervalVector>::nb,
      SIZET_CTCUNION_X_NB_CONST)

    .def("__ior__", [](CtcUnion<IntervalVector>& c1, const CtcBase<IntervalVector>& c2)
        {
          c1 |= std::dynamic_pointer_cast<CtcBase<IntervalVector>>(c2.copy());
          return c1;
        },
      CTCUNION_X_VARIADIC_REF_CTCUNION_X_OPERATORUNIONEQ_CONST_C_REF)
  ;

  CONTRACT_METHODS(exported, CtcUnion<IntervalVector>,
    VOID_CTCUNION_X_CONTRACT_X_REF_VARIADIC_CONST)
}