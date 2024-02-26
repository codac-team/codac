//============================================================================
//                               P Y I B E X
// File        : pyibex_SepRaster.h
// Author      : Benoit Desrochers
// Copyright   : Benoit Desrochers
// License     : See the LICENSE file
// Created     : Aug 18, 2016
//============================================================================


#ifndef __SEPRASTER_H__
#define __SEPRASTER_H__


#include <pybind11/numpy.h>

#include <codac_GeoMapper.h>
#include <ibex_Sep.h>


#include <codac_IntervalVector.h>



namespace py = pybind11;

using codac::IntervalVector;
using codac::Interval;


class SepRaster : public codac::Sep, public CtcRaster {

  // Georeferenced image with integral image inclusion test
public:
  SepRaster(py::array_t<unsigned char>  img_map,
     py::array_t<unsigned long>  img_ii,
    double x0, double y0, double dx, double dy, bool inner) :
  Sep(2),
  CtcRaster(img_ii, x0, y0, dx, dy, inner), inner(inner),
  info_img(img_map.request())
  {
    // constructor
  }
  
  void separate(IntervalVector& xin, IntervalVector& xout);
  // Boolean to avoid contraction when used as an inner contractor
  bool inner;

  py::buffer_info  info_img;
};

#endif //__SEPRASTER_H__
