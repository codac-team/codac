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

#include <ibex_IntervalVector.h>

#include <GeoMapper.h>
#include <pyibex_doc_image.h>
#include <pyibex_GeoImage.h>
#include <pyibex_CtcRaster.h>

#include <ibex_Ctc.h>
#include <ibex_Sep.h>



namespace py = pybind11;
using py::self;
using py::init;
using py::keep_alive;
using py::class_;

using ibex::IntervalVector;
using ibex::Interval;

using namespace pybind11::literals;

PYBIND11_MODULE(image, m)
{
  // py::module m("image", "python binding CtcIMage Class");


  py::object ctc = (py::object) py::module::import("pyibex").attr("Ctc");
  py::object sep = (py::object) py::module::import("pyibex").attr("Sep");

  class_<GeoImage>(m, "GeoImage")
      .def(init<py::array_t<DATA_TYPE> , double, double , double , double >(), DOC_GEOIMAGE_CONSTRUCTOR,
           py::keep_alive<1,2>(), "img"_a, "x0"_a, "y0"_a, "dx"_a, "dy"_a)
      .def("world_to_grid", &GeoImage::world_to_grid, DOC_GEOIMAGE_WORLD_TO_GRID, "box"_a)
      .def("grid_to_world", &GeoImage::grid_to_world, DOC_GEOIMAGE_GRID_TO_WORLD, "pixel_coords"_a)
      .def("pixelAt", &GeoImage::pixelAt, DOC_GEOIMAGE_PIXELAT, "x"_a, "y"_a)
      .def("enclosed_pixels", &GeoImage::enclosed_pixels, DOC_GEOIMAGE_ENCLOSED_PIXELS,
                            "xmin"_a, "xmax"_a, "ymin"_a, "ymax"_a )
//      .def("test", &GeoImage::test, DOC_GEOIMAGE_TEST, "box"_a)

      .def_property_readonly("boundingBox", [](GeoImage& self){return self.boundingBox;})
      ;

  class_<CtcRaster>(m, "CtcRaster",ctc)
      .def(init<py::array_t<DATA_TYPE> , double, double , double , double, bool>(),py::keep_alive<1,2>(),
                "img"_a, "x0"_a, "y0"_a, "dx"_a, "dy"_a, "inner"_a=false )
      .def("contract", &CtcRaster::contract)
      .def_property_readonly("boundingBox", [](CtcRaster& self){return self.getBoundingBox();})

      ;

  class_<GeoMapper>(m, "GeoMapper")
      .def(init<double, double, int, int, double, double>(), DOC_GEOMAPPER_CONSTRUCTOR, "x0"_a, "y0"_a, "x_size"_a, "y_size"_a, "dx"_a, "dy"_a)
      .def("world_to_grid", &GeoMapper::world_to_grid, DOC_GEOMAPPER_CONSTRUCTOR, "box"_a)
      .def("grid_to_world", &GeoMapper::grid_to_world, DOC_GEOMAPPER_GRID_TO_WORLD, "pixel_coords"_a)
      .def_property_readonly("boundingBox", [](GeoMapper& self){return self.bbox;})
      ;

    // return m.ptr();
}
