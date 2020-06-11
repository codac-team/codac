/** 
 *  \file
 *  TFnc Python binding
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou, Benoît Desrochers
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <pybind11/functional.h>
#include "pyIbex_type_caster.h"

#include "tubex_Tube.h"
#include "tubex_TubeVector.h"
#include "tubex_TFnc.h"
// Generated file from Doxygen XML (doxygen2docstring.py):
#include "tubex_py_TFnc_docs.h"

using namespace std;
using namespace ibex;
using namespace tubex;
namespace py = pybind11;
using namespace pybind11::literals;


class pyTFnc : public TFnc
{
  public:

    using TFnc::TFnc;

    const Tube eval(const TubeVector &x) const override
    {
      PYBIND11_OVERLOAD_PURE(const Tube, TFnc, eval, x);
    }

    const ibex::Interval eval(const ibex::IntervalVector &x) const override
    {
      PYBIND11_OVERLOAD_PURE(const ibex::Interval, TFnc, eval, x);
    }

    const ibex::Interval eval(int slice_id,const TubeVector &x) const override
    {
      PYBIND11_OVERLOAD_PURE(const ibex::Interval, TFnc, eval, slice_id, x);
    }

    const ibex::Interval eval(const ibex::Interval &t,const TubeVector &x) const override
    {
      PYBIND11_OVERLOAD_PURE(const ibex::Interval, TFnc, eval, t, x);
    }

    const TubeVector eval_vector(const TubeVector &x) const override
    {
      PYBIND11_OVERLOAD_PURE(const TubeVector, TFnc, eval_vector, x);
    }

    const ibex::IntervalVector eval_vector(const ibex::IntervalVector &x) const override
    {
      PYBIND11_OVERLOAD_PURE(const ibex::IntervalVector, TFnc, eval_vector, x);
    }

    const ibex::IntervalVector eval_vector(int slice_id,const TubeVector &x) const override
    {
      PYBIND11_OVERLOAD_PURE(const ibex::IntervalVector, TFnc, eval_vector, slice_id, x);
    }

    const ibex::IntervalVector eval_vector(const ibex::Interval &t,const TubeVector &x) const override
    {
      PYBIND11_OVERLOAD_PURE(const ibex::IntervalVector, TFnc, eval_vector, t, x);
    }
};

py::class_<TFnc, pyTFnc> export_TFnc(py::module& m)
{
  py::class_<TFnc, pyTFnc> fnc(m, "TFnc", "todo");
  fnc

    .def(py::init<int,int,bool>(),
      TFNC_TFNC_INT_INT_BOOL,
      "n"_a, "m"_a, "is_intertemporal"_a)

    .def("nb_vars", &TFnc::nb_vars,
      TFNC_INT_NB_VARS)

    .def("image_dim", &TFnc::image_dim,
      TFNC_INT_IMAGE_DIM)

    .def("is_intertemporal", &TFnc::is_intertemporal,
      TFNC_BOOL_IS_INTERTEMPORAL)
  ;

  return fnc;
}