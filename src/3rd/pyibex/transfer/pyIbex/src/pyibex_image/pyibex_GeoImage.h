//============================================================================
//                               P Y I B E X
// File        : pyIbex_GeoImages.cpp
// Author      : Benoit Desrochers
// Copyright   : Benoit Desrochers
// License     : See the LICENSE file
// Created     : Aug 18, 2016
//============================================================================

#ifndef __PYIBEX_GEOIMAGE_H__
#define __PYIBEX_GEOIMAGE_H__

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

#include <ibex_IntervalVector.h>
#include <GeoMapper.h>

namespace py = pybind11;


using ibex::IntervalVector;
using ibex::Interval;

#define DATA_TYPE unsigned long

class GeoImage {

  // Georeferenced image with integral image inclusion test
public:
  GeoImage(py::array_t<DATA_TYPE>  data, double x0, double y0, double dx, double dy) :
  // II(data),
  info_i(data.request()),
  boundingBox(2),
  coordMapper(x0, y0, data.request().shape[0], data.request().shape[1], dx, dy)
  {
    if( info_i.ndim != 2 )
      throw std::runtime_error("Number of dimensions must be one");
      boundingBox = getBoundingBox();
  }

  DATA_TYPE  pixelAt( int x, int y );
  DATA_TYPE enclosed_pixels( int xmin,int xmax,int ymin,int ymax);
  IntervalVector getBoundingBox();


  PixelCoords world_to_grid(IntervalVector box) { return coordMapper.world_to_grid(box); }
  IntervalVector grid_to_world(const PixelCoords& pixel_coords) {return coordMapper.grid_to_world(pixel_coords); }

  bool checkBorder(PixelCoords& c, const std::vector<bool>& limit, const bool & b);

  IntervalVector boundingBox;

protected:
  // py::array_t<DATA_TYPE> II;
  GeoMapper coordMapper;
  py::buffer_info  info_i;
  // DATA_TYPE * img_ptr;
};


//##############################################################################
//#################   INLINE IMPLEMENTATION   ##################################
//##############################################################################


inline DATA_TYPE GeoImage::pixelAt( int x, int y){
  // std::cerr << x << " " << y << "\n";
  if (x < 0 || y < 0)
    return 0;
  if ( !(x < info_i.shape[0]) ){
    std::cout << "x " << x <<  " " << info_i.shape[0] << "\n";
    assert(x < info_i.shape[0]);
  }
  if ( !(y < info_i.shape[1]) ) {
    std::cout << "y " << y << " " << info_i.shape[1] << "\n";
    assert(y < info_i.shape[1]);
  }
  DATA_TYPE* ptr = (DATA_TYPE*) info_i.ptr;
  long idx = (x*info_i.strides[0] + y*info_i.strides[1] ) / info_i.itemsize; //sizeof(DATA_TYPE);
  // std::cout << idx << " ";
  return ((DATA_TYPE*)info_i.ptr)[idx];
}
//
inline DATA_TYPE GeoImage::enclosed_pixels( int xmin,int xmax,int ymin,int ymax) {
    // std::cerr << "enclosed_pixels" << ymin << " " << ymin -1 << "\n";
    DATA_TYPE b1 = pixelAt(xmax,ymax);
    DATA_TYPE b2 = pixelAt(xmax,ymin-1);
    DATA_TYPE b3 = pixelAt(xmin-1,ymax);
    DATA_TYPE b4 = pixelAt(xmin-1,ymin-1);
    return b1 - b2 - b3 + b4;
}

inline int boxSize(int xmin, int xmax, int ymin, int ymax){
    return (xmax+1 - xmin) * (ymax+1 - ymin);
}

inline bool GeoImage::checkBorder(PixelCoords& c, const std::vector<bool>& limit, const bool & b){
  // b initialy a ThickBoolean IN == true
  // Check the border to see if it is a pixel limit
  int val, bound, nPix;
  for(int i = 0; i < limit.size(); i++){
    if (limit[i] == false) continue;
    switch (i) {
      case 0:
        bound = std::max<int>(0,c[0]-1);
        val = ( b == true ) ? boxSize(bound, bound, c[2], c[3]) : 0;
        nPix = enclosed_pixels(bound, bound, c[2], c[3]);
        break;
      case 1:
        bound = std::min<int>(info_i.shape[0]-1,c[1]+1);
        val = ( b == true ) ? boxSize(bound, bound, c[2], c[3]) : 0;
        nPix = enclosed_pixels(bound, bound, c[2], c[3]);
        break;
      case 2:
        bound = std::max(0,c[2]-1);
        val = ( b == true ) ? boxSize(c[0], c[1], bound, bound) : 0;
        nPix = enclosed_pixels(c[0], c[1], bound, bound);
        break;
      case 3:
        bound = std::min<int>(info_i.shape[1]-1,c[3]+1);
        val = ( b == true ) ? boxSize(c[0], c[1], bound, bound) : 0;
        nPix = enclosed_pixels(c[0], c[1], bound, bound);
        break;
    }

    if (nPix != val) return false;
  }
  return true;
}

inline IntervalVector GeoImage::getBoundingBox(){
    int xmax = info_i.shape[0]-1;
    int ymax = info_i.shape[1]-1;
    return grid_to_world({ 0, xmax, 0, ymax});
}

#endif //__PYIBEX_GEOIMAGE_H__
