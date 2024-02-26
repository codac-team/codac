/** 
 *  \file
 *  Bisectors Python binding
 *  Originated from the former pyIbex library (Benoît Desrochers)
 * ----------------------------------------------------------------------------
 *  \date       2022
 *  \author     Benoît Desrochers
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <string>
#include <sstream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <pybind11/functional.h>
#include <pybind11/numpy.h>
#include "codac_type_caster.h"

#include "ibex_Bsc.h"
#include "ibex_LargestFirst.h"
// Generated file from Doxygen XML (doxygen2docstring.py):
#include "codac_py_bisectors_docs.h" // todo: generate this file from Doxygen doc

using namespace std;
using namespace ibex;
namespace py = pybind11;
using namespace pybind11::literals;

typedef pair<IntervalVector,IntervalVector> ItvPair;

class py_Bsc : public Bsc 
{
  public:

    using Bsc::Bsc; // inherit the constructors

    // Trampoline (need one for each virtual function)
    virtual ItvPair bisect(const IntervalVector& box)
    {
      PYBIND11_OVERLOAD_PURE(
        ItvPair, // return type
        Bsc,     // Parent class
        bisect,  // Name of function
        box      // Argument(s)
      );
    }
};

void export_bisectors(py::module& m)
{
  // Bsc Class binding

  py::class_<Bsc, unique_ptr<Bsc>, py_Bsc> bsc(m, "Bsc", DOCS_BSC_TYPE);
  bsc

    .def("bisect", (ItvPair (Bsc::*) (const IntervalVector& box))(&Bsc::bisect), DOCS_BSC_BISECT, py::arg("x"))
    .def("default_ratio", &Bsc::default_ratio, DOCS_BSC_DEFAULT_RATIO)
    .def("uniform_prec", &Bsc::uniform_prec, DOCS_BSC_UNIFORM_PREC)
    .def("prec", &Bsc::prec, DOCS_BSC_PREC)
    .def("too_small", &Bsc::too_small, DOCS_BSC_TOO_SMALL)
  ;

  // LargestFirst Bisector binding
  
  py::class_<LargestFirst> lf(m, "LargestFirst", bsc, DOCS_LARGEST_FIRST);
  lf

    .def(py::init<double, double>(), py::arg("prec")=0, py::arg("ratio")=Bsc::default_ratio())
    .def(py::init<Vector&, double>(), py::arg("prec"), py::arg("ratio")=Bsc::default_ratio())
    .def("bisect", (ItvPair (LargestFirst::*) (const IntervalVector& box)) &LargestFirst::bisect)
  ;
}