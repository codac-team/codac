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
#include <codac2_CtcInter.h>
#include "codac2_py_Ctc.h"
#include "codac2_py_CtcInter_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_CtcInter(py::module& m, py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector>& pyctc)
{
  py::class_<CtcInter<IntervalVector>> exported(m, "CtcInter", pyctc, CTCINTER_MAIN);
  exported

    .def(py::init<Index>(),
      CTCINTER_X_CTCINTER_INDEX,
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

          // Backward compatibility: CtcInter([c1,c2,c3])
          if(args.size() == 1 && py::isinstance<py::list>(args[0]))
          {
            for(const auto& c : args[0].cast<py::list>())
              add(c);
          }
          else
          {
            // New syntax: CtcInter(c1,c2,c3)
            for(const auto& c : args)
              add(c);
          }

          return std::make_unique<CtcInter<IntervalVector>>(ctcs);
        }),
      CTCINTER_X_CTCINTER_INITIALIZER_LIST_C)

    .def("nb", &CtcInter<IntervalVector>::nb,
      SIZET_CTCINTER_X_NB_CONST)

    .def("__iand__", [](CtcInter<IntervalVector>& c1, const CtcBase<IntervalVector>& c2)
        {
          c1 &= std::dynamic_pointer_cast<CtcBase<IntervalVector>>(c2.copy());
          return c1;
        },
      CTCINTER_X_VARIADIC_REF_CTCINTER_X_OPERATORINTEREQ_CONST_C_REF)
  ;

  CONTRACT_METHODS(exported, CtcInter<IntervalVector>,
    VOID_CTCINTER_X_CONTRACT_X_REF_VARIADIC_CONST)
}