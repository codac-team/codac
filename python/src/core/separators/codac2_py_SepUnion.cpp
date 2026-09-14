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
#include <codac2_SepUnion.h>
#include "codac2_py_Sep.h"
#include "codac2_py_SepUnion_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_SepUnion(py::module& m, py::class_<SepBase,pySep>& pysep)
{
  py::class_<SepUnion> exported(m, "SepUnion", pysep, SEPUNION_MAIN);
  exported

    .def(py::init(
        [](py::args args)
        {
          Collection<SepBase> seps;

          auto add = [&seps](py::handle s)
          {
            seps.push_back(
              s.cast<SepBase&>().copy()
            );
          };

          // Backward compatibility: SepUnion([s1,s2,s3])
          if(args.size() == 1 && py::isinstance<py::list>(args[0]))
          {
            for(const auto& s : args[0].cast<py::list>())
              add(s);
          }
          else
          {
            // New syntax: SepUnion(s1,s2,s3)
            for(const auto& s : args)
              add(s);
          }

          return std::make_unique<SepUnion>(seps);
        }),
      SEPUNION_SEPUNION_CONST_COLLECTION_SEPBASE_REF)

    .def("nb", &SepUnion::nb,
      SIZET_SEPUNION_NB_CONST)

    .def("separate", &SepUnion::separate,
      BOXPAIR_SEPUNION_SEPARATE_CONST_INTERVALVECTOR_REF_CONST,
      "x"_a)

    .def("__ior__", [](SepUnion& s1, const SepBase& s2)
        {
          s1 |= s2.copy();
          return s1;
        },
      SEPUNION_REF_SEPUNION_OPERATORUNIONEQ_CONST_S_REF)
  ;
}