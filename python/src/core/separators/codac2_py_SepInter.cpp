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
#include <codac2_SepInter.h>
#include "codac2_py_Sep.h"
#include "codac2_py_SepInter_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_SepInter(py::module& m, py::class_<SepBase,pySep>& pysep)
{
  py::class_<SepInter> exported(m, "SepInter", pysep, SEPINTER_MAIN);
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

          // Backward compatibility: SepInter([s1,s2,s3])
          if(args.size() == 1 && py::isinstance<py::list>(args[0]))
          {
            for(const auto& s : args[0].cast<py::list>())
              add(s);
          }
          else
          {
            // New syntax: SepInter(s1,s2,s3)
            for(const auto& s : args)
              add(s);
          }

          return std::make_unique<SepInter>(seps);
        }),
      SEPINTER_SEPINTER_CONST_COLLECTION_SEPBASE_REF)

    .def("nb", &SepInter::nb,
      SIZET_SEPINTER_NB_CONST)

    .def("separate", &SepInter::separate,
      BOXPAIR_SEPINTER_SEPARATE_CONST_INTERVALVECTOR_REF_CONST,
      "x"_a)

    ;

    if constexpr(!FOR_MATLAB)
    {
      exported

      .def("__iand__", [](SepInter& s1, const SepBase& s2)
          {
            s1 &= s2.copy();
            return s1;
          },
        SEPINTER_REF_SEPINTER_OPERATORINTEREQ_CONST_S_REF)
      ;
    }

    if constexpr(FOR_MATLAB)
    {
      exported
      .def("self_inter", [](SepInter& s1, const SepBase& s2)
          {
            s1 &= s2.copy();
            return s1;
          },
        SEPINTER_REF_SEPINTER_OPERATORINTEREQ_CONST_S_REF)    
      ;
    }
}