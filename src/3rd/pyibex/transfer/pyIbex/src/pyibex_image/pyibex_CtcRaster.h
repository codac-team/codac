//============================================================================
//                               P Y I B E X
// File        : pyibex_CtcRaster.h
// Author      : Benoit Desrochers, inspired from the work of Jan Sliswka
// Copyright   : Benoit Desrochers
// License     : See the LICENSE file
// Created     : Aug 18, 2016
//============================================================================


#ifndef __CTCRASTER_H__
#define __CTCRASTER_H__


#include <pybind11/numpy.h>

#include <GeoMapper.h>
#include <ibex_Ctc.h>

#include <ibex_IntervalVector.h>



namespace py = pybind11;

using ibex::IntervalVector;
using ibex::Interval;


class CtcRaster : public ibex::Ctc, public GeoImage {

  // Georeferenced image with integral image inclusion test
public:
  CtcRaster(py::array_t<DATA_TYPE>  data, double x0, double y0, double dx, double dy, bool inner) :
  Ctc(2),
  GeoImage(data, x0, y0, dx, dy), inner(inner)
  {
    // constructor
  }
  int enclosed_pixels(PixelCoords& pixel_coords);
  bool contractDim(PixelCoords& coords, int nPix);

  void contract(IntervalVector& box);
  // Boolean to avoid contraction when used as an inner contractor
  bool inner;
};


//##############################################################################
//#################   INLINE IMPLEMENTATION   ##################################
//##############################################################################

inline int CtcRaster::enclosed_pixels(PixelCoords& pixel_coords){
  return GeoImage::enclosed_pixels(pixel_coords[0],pixel_coords[1],pixel_coords[2],pixel_coords[3]);
}


inline bool CtcRaster::contractDim(PixelCoords& coords, int nPix){
  for(int dim = 0; dim < 2; dim++){
      // std::cerr << "\n";
      PixelCoords coords_tmp(coords);
      for(int i = coords[2*dim]; i <= coords[2*dim+1]; i++){
        coords_tmp[2*dim+1] = i;
        if (enclosed_pixels(coords_tmp) > 0 ) break;
      }
      coords[2*dim] = coords_tmp[2*dim+1];
      coords_tmp = coords;
      for(int i = coords[2*dim+1]; i >= coords[2*dim]; i--){
        coords_tmp[2*dim] = i;
        if (enclosed_pixels(coords_tmp) > 0 ) break;
      }
      coords[2*dim+1] = coords_tmp[2*dim];
    }

  return true;
}


inline void CtcRaster::contract(IntervalVector& box){
    assert(box.size() == this->nb_var);
    if(box.is_empty()) return;
    if (inner == true && !box.is_subset(boundingBox) ) return;
    box &= boundingBox;
    if(box.is_empty()) return;

    // std::cerr << boundingBox << " " << box << "\n";
    // Convert world coordinates into pixel coordinates
    PixelCoords pixel_coords = world_to_grid(box);
    // std::cerr << pixel_coords[0] << " " << pixel_coords[1] << " "<< pixel_coords[2] << " "<< pixel_coords[3] << "\n";
    // IntervalVector res2 = grid_to_world(pixel_coords);
    // std::cerr << "RES2 " << res2 << std::endl;
    // std::cerr << box << "\n";
    size_t box_pixel_size = (pixel_coords[1]+1 - pixel_coords[0]) * (pixel_coords[3]+1 - pixel_coords[2]);
    // std::cerr << pixel_coords[0] << " " << pixel_coords[1] << " "<< pixel_coords[2] << " "<< pixel_coords[3] << " "<< "\n";
    unsigned int nPix = enclosed_pixels(pixel_coords);//[0],pixel_coords[1],pixel_coords[2],pixel_coords[3]);
    // std::cerr << nPix << "\n";
    if ( nPix == 0 ) {// && checkBorder(pixel_coords, coordMapper.limit, false ) == true){
      box.set_empty();
    } else {

      // std::cerr << pixel_coords[0] << " " << pixel_coords[1] << " "<< pixel_coords[2] << " "<< pixel_coords[3] << " "<< " " << nPix <<  " " << enclosed_pixels(pixel_coords) << "\n";
      contractDim(pixel_coords, nPix);
      // std::cerr << "done\n";


      IntervalVector res = grid_to_world(pixel_coords);
      // std::cerr << " "  << box << " " << res << " " << (box&res) << std::endl;
      box &= res;
    }










    //
    //
    // // std::cout << "box_pixel_size/nPix  " <<   box_pixel_size << " " << nPix << "\n";
    // ThickBoolean result;
    // if (nPix == box_pixel_size){
    //   result = checkBorder(pixel_coords, coordMapper.limit, true ) ? IN : UNK;
    // } else if (nPix == 0) {
    //   result = checkBorder(pixel_coords, coordMapper.limit, false ) ? OUT : UNK;
    // } else {
    //   result = UNK;
    // }
    //
    // // std::cout << getBoundingBox() << "\n";
    // // exit(-1);
    // if ( !box.is_subset(getBoundingBox()) ){
    //   // std::cerr << result << " " << opUnion(result,MAYBE)
    //             // << " " << nPix << " " << pixel_coords[0] << " " << pixel_coords[1] << " " << pixel_coords[2] << " " << pixel_coords[3] << " "
    //             // <<  "\n";
    //   return  result | outerVal;
    // }
    // return result;
}




#endif //__CTCRASTER_H__
