//============================================================================
//                               P Y I B E X
// File        : pyIbex_GeoImages.cpp
// Author      : Benoit Desrochers
// Copyright   : Benoit Desrochers
// License     : See the LICENSE file
// Created     : Aug 18, 2016
//============================================================================



#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include <pybind11/operators.h>

#include <codac_IntervalVector.h>

#include "codac_GeoMapper.h"
#include "codac_doc_image.h"
#include "codac_GeoImage.h"
#include "codac_CtcRaster.h"

#include "../../core/contractors/static/codac_py_Ctc.h"
#include <codac_Ctc.h>
#include <ibex_Sep.h>



namespace py = pybind11;
using py::self;
using py::init;
using py::keep_alive;
using py::class_;

using codac::IntervalVector;
using codac::Interval;
using codac::Ctc;
using codac::pyCtc;

using namespace pybind11::literals;

void export_GeoImage(py::module& m)
{
  class_<GeoImage> geo_image(m, "GeoImage");
  geo_image
      .def(init<py::array_t<DATA_TYPE> , double, double , double , double >(), __DOC_GEOIMAGE_CONSTRUCTOR,
           py::keep_alive<1,2>(), "img"_a, "x0"_a, "y0"_a, "dx"_a, "dy"_a)
      .def("world_to_grid", &GeoImage::world_to_grid, __DOC_GEOIMAGE_WORLD_TO_GRID, "box"_a)
      .def("grid_to_world", &GeoImage::grid_to_world, __DOC_GEOIMAGE_GRID_TO_WORLD, "pixel_coords"_a)
      .def("pixelAt", &GeoImage::pixelAt, __DOC_GEOIMAGE_PIXELAT, "x"_a, "y"_a)
      .def("enclosed_pixels", &GeoImage::enclosed_pixels, __DOC_GEOIMAGE_ENCLOSED_PIXELS,
                            "xmin"_a, "xmax"_a, "ymin"_a, "ymax"_a )
//      .def("test", &GeoImage::test, __DOC_GEOIMAGE_TEST, "box"_a)

      .def_property_readonly("boundingBox", [](GeoImage& self){return self.boundingBox;})
      ;
}

void export_CtcRaster(py::module& m, py::class_<Ctc, pyCtc>& ctc)
{
  class_<CtcRaster> ctc_raster(m, "CtcRaster",ctc);
  ctc_raster
      .def(init<py::array_t<DATA_TYPE> , double, double , double , double, bool>(),py::keep_alive<1,2>(),
                "img"_a, "x0"_a, "y0"_a, "dx"_a, "dy"_a, "inner"_a=false )
      .def("contract", &CtcRaster::contract)
      .def_property_readonly("boundingBox", [](CtcRaster& self){return self.getBoundingBox();})
      ;
}

void export_GeoMapper(py::module& m)
{
  class_<GeoMapper> geo_mapper(m, "GeoMapper");
  geo_mapper
      .def(init<double, double, int, int, double, double>(), __DOC_GEOMAPPER_CONSTRUCTOR, "x0"_a, "y0"_a, "x_size"_a, "y_size"_a, "dx"_a, "dy"_a)
      .def("world_to_grid", &GeoMapper::world_to_grid, __DOC_GEOMAPPER_CONSTRUCTOR, "box"_a)
      .def("grid_to_world", &GeoMapper::grid_to_world, __DOC_GEOMAPPER_GRID_TO_WORLD, "pixel_coords"_a)
      .def_property_readonly("boundingBox", [](GeoMapper& self){return self.bbox;})
      ;
}
