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
#include <codac2_SepQInter.h>
#include "codac2_py_Sep.h"
#include "codac2_py_SepQInter_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_SepQInter(py::module& m, py::class_<SepBase,pySep>& pysep)
{
  py::class_<SepQInter> exported(m, "SepQInter", pysep, SEPQINTER_MAIN);
  exported

    .def(py::init(
        [](Index q, py::args args)
        {
          matlab::test_integer(q);
          Collection<SepBase> seps;

          auto add = [&seps](py::handle s)
          {
            seps.push_back(
              s.cast<SepBase&>().copy()
            );
          };

          // Backward compatibility: SepQInter(q, [s1,s2,s3])
          if(args.size() == 1 && py::isinstance<py::list>(args[0]))
          {
            for(const auto& s : args[0].cast<py::list>())
              add(s);
          }
          else
          {
            // New syntax: SepQInter(q, s1,s2,s3)
            for(const auto& s : args)
              add(s);
          }

          return std::make_unique<SepQInter>(q, seps);
        }),
      SEPQINTER_SEPQINTER_UNSIGNED_INT_CONST_COLLECTION_SEPBASE_REF,
      "q"_a)

    .def("nb", &SepQInter::nb,
      SIZET_SEPQINTER_NB_CONST)

    .def("separate", &SepQInter::separate,
      BOXPAIR_SEPQINTER_SEPARATE_CONST_INTERVALVECTOR_REF_CONST,
      "x"_a)

  ;
}