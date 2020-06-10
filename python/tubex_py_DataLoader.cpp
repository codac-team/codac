
/** 
 *  \file
 *  DataLoader binding
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou, Benoît Desrochers
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "tubex_DataLoader.h"
#include "tubex_py_DataLoader_docs.h"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <pybind11/functional.h>
#include "pyIbex_type_caster.h"

namespace py = pybind11;
using namespace pybind11::literals;
using py::class_;
using py::init;

using namespace tubex;
using ibex::Interval;
using ibex::IntervalVector;


void export_DataLoader(py::module& m){

    py::class_<DataLoader> data_loader(m, "DataLoader", DOCS_DATALOADER);
    data_loader
      .def(py::init<>(),DOCS_DATALOADER_DATALOADER)
      .def(py::init<const std::string &>(),
          DOCS_DATALOADER_DATALOADER_STRING, "file_path"_a)
    //  .def("load_data", &DataLoader::load_data,
    //      DOCS_DATALOADER_LOAD_DATA_TUBEVECTOR_TRAJECTORYVECTOR_FLOAT_INTERVAL, "x"_a, "truth"_a, "timestep"_a, "tdomain"_a=ibex::Interval::ALL_REALS)
    //  .def("serialized_data_available", &DataLoader::serialized_data_available,
    //      DOCS_DATALOADER_SERIALIZED_DATA_AVAILABLE)
    //  .def("deserialize_data", &DataLoader::deserialize_data,
    //      DOCS_DATALOADER_DESERIALIZE_DATA_TUBEVECTOR_TRAJECTORYVECTOR, "x"_a, "traj"_a)
    //  .def("serialize_data", &DataLoader::serialize_data,
    //      DOCS_DATALOADER_SERIALIZE_DATA_TUBEVECTOR_TRAJECTORYVECTOR, "x"_a, "traj"_a)

      .def_static("generate_landmarks_boxes", &DataLoader::generate_landmarks_boxes,
          DOCS_DATALOADER_GENERATE_LANDMARKS_BOXES_INTERVALVECTOR_INT, "map_box"_a, "nb_landmarks"_a=100)
      .def_static("generate_static_observations", &DataLoader::generate_static_observations,
          "todo", "x"_a, "map"_a, "random"_a=true, "visi_range"_a=ibex::Interval::POS_REALS, "visi_angle"_a=ibex::Interval::ALL_REALS)


    //  //.def("generate_observations", (static std::vector<ibex::IntervalVector> (DataLoader::*)(const ibex::Vector &,const std::vector<Beacon> &,bool,const ibex::Interval &,const ibex::Interval &) )&DataLoader::generate_observations,
    //  //    DOCS_DATALOADER_GENERATE_OBSERVATIONS_VECTOR_VECTOR_BEACON__BOOL_INTERVAL_INTERVAL, "x"_a, "map"_a, "random"_a=true, "visi_range"_a=ibex::Interval::POS_REALS, "visi_angle"_a=ibex::Interval::ALL_REALS)
    //  .def_static("generate_observations", &DataLoader::DOCS_DATALOADER_GENERATE_OBSERVATIONS_TRAJECTORYVECTOR_VECTOR_BEACON__INT_BOOL_INTERVAL_INTERVAL_INTERVAL,
    //      "todo", "x"_a, "map"_a, "random"_a=true, "visi_range"_a=ibex::Interval::POS_REALS, "visi_angle"_a=ibex::Interval::ALL_REALS)
    //  //.def("generate_observations", (static std::vector<ibex::IntervalVector> (DataLoader::*)(const TrajectoryVector &,const std::vector<Beacon> &,int,bool,const ibex::Interval &,const ibex::Interval &,const ibex::Interval &) )&DataLoader::generate_observations,
    //  //    DOCS_DATALOADER_GENERATE_OBSERVATIONS_TRAJECTORYVECTOR_VECTOR_BEACON__INT_BOOL_INTERVAL_INTERVAL_INTERVAL, "x"_a, "map"_a, "nb_obs"_a=50, "random"_a=true, "visi_range"_a=ibex::Interval(0,50), "visi_angle"_a=ibex::Interval(-M_PI/4,M_PI/4), "tdomain"_a=ibex::Interval::ALL_REALS)
      .def_static("generate_observations", &DataLoader::generate_observations_along_traj,
          DOCS_DATALOADER_GENERATE_OBSERVATIONS_TRAJECTORYVECTOR_VECTOR_INTERVALVECTOR__INT_BOOL_INTERVAL_INTERVAL_INTERVAL, "x"_a, "map"_a, "nb_obs"_a=50, "random"_a=true, "visi_range"_a=ibex::Interval(0,50), "visi_angle"_a=ibex::Interval(-M_PI/4,M_PI/4), "tdomain"_a=ibex::Interval::ALL_REALS)

;
}