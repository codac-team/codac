/** 
 *  \file
 *  DataLoader Python binding
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

#include "tubex_DataLoader.h"
// Generated file from Doxygen XML (doxygen2docstring.py):
#include "tubex_py_DataLoader_docs.h"

using namespace std;
using namespace ibex;
using namespace tubex;
namespace py = pybind11;
using namespace pybind11::literals;


void export_DataLoader(py::module& m)
{
  py::class_<DataLoader> data_loader(m, "DataLoader", DATALOADER_MAIN);
  data_loader

    .def(py::init<>(),
      DATALOADER_DATALOADER)

    .def(py::init<const std::string &>(),
      DATALOADER_DATALOADER_STRING,
      "file_path"_a)

    .def_static("generate_landmarks_boxes", &DataLoader::generate_landmarks_boxes,
      DATALOADER_VECTORINTERVALVECTOR_GENERATE_LANDMARKS_BOXES_INTERVALVECTOR_INT,
      "map_box"_a, "nb_landmarks"_a=100)

    .def_static("generate_static_observations", &DataLoader::generate_static_observations,
      DATALOADER_VECTORINTERVALVECTOR_GENERATE_STATIC_OBSERVATIONS_VECTOR_VECTORINTERVALVECTOR_BOOL_INTERVAL_INTERVAL,
      "x"_a, "map"_a, "random"_a=true, "visi_range"_a=ibex::Interval::POS_REALS, "visi_angle"_a=ibex::Interval::ALL_REALS)

    .def_static("generate_observations", &DataLoader::generate_observations_along_traj,
      DATALOADER_VECTORINTERVALVECTOR_GENERATE_OBSERVATIONS_ALONG_TRAJ_TRAJECTORYVECTOR_VECTORINTERVALVECTOR_INT_BOOL_INTERVAL_INTERVAL_INTERVAL,
      "x"_a, "map"_a, "nb_obs"_a=50, "random"_a=true, "visi_range"_a=ibex::Interval(0,50), "visi_angle"_a=ibex::Interval(-M_PI/4,M_PI/4), "tdomain"_a=ibex::Interval::ALL_REALS)
  ;
}