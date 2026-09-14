/** 
 *  Codac binding (core)
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Simon Rohou
 *  \copyright  Copyright 2025 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <codac2_CtcQInter.h>
#include "codac2_py_Ctc.h"
#include "codac2_py_CtcQInter_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_CtcQInter(py::module& m, py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector>& pyctc)
{
  py::class_<CtcQInter> exported(m, "CtcQInter", pyctc, CTCQINTER_MAIN);
  exported

    .def(py::init(
        [](Index q, py::args args)
        {
          matlab::test_integer(q);
          Collection<CtcBase<IntervalVector>> ctcs;

          auto add = [&ctcs](py::handle c)
          {
            ctcs.push_back(
              c.cast<CtcBase<IntervalVector>&>().copy()
            );
          };

          // Backward compatibility: CtcQInter(q, [c1,c2,c3])
          if(args.size() == 1 && py::isinstance<py::list>(args[0]))
          {
            for(const auto& c : args[0].cast<py::list>())
              add(c);
          }
          else
          {
            // New syntax: CtcQInter(q, c1,c2,c3)
            for(const auto& c : args)
              add(c);
          }

          return std::make_unique<CtcQInter>(q, ctcs);
        }),
      CTCQINTER_CTCQINTER_UNSIGNED_INT_CONST_C_REF,
      "q"_a)

    .def("nb", &CtcQInter::nb,
      SIZET_CTCQINTER_NB_CONST)

  ;

  CONTRACT_METHODS(exported, CtcQInter,
    VOID_CTCQINTER_CONTRACT_INTERVALVECTOR_REF_CONST)
}