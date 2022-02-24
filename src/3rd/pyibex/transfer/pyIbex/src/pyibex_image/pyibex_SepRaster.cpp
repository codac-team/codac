//============================================================================
//                               P Y I B E X
// File        : pyibex_SepRaster.cpp
// Author      : Benoit Desrochers
// Copyright   : Benoit Desrochers
// License     : See the LICENSE file
// Created     : Aug 18, 2016
//============================================================================

#include <pybind11/numpy.h>

#include <pyibex_SepRaster.h>


namespace py = pybind11;

using ibex::IntervalVector;
using ibex::Interval;

int SepRaster::enclosed_pixels(PixelCoords& pixel_coords){
  return GeoImage::enclosed_pixels(pixel_coords[0],pixel_coords[1],pixel_coords[2],pixel_coords[3]);
}


bool SepRaster::contractDim(PixelCoords& coords, int nPix){
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



void SepRaster::separate(IntervalVector& xin, IntervalVector& xout){
  IntervalVector box(xin & xout);

  contractor(box);

  
  assert(box.size() == this->nb_var);

  if(box.is_empty()) return;
  if (inner == true && !box.is_subset(boundingBox) ) return;
  box &= boundingBox;
  if(box.is_empty()) return;


    size_t box_pixel_size = (pixel_coords[1]+1 - pixel_coords[0]) * (pixel_coords[3]+1 - pixel_coords[2]);
    unsigned int nPix = enclosed_pixels(pixel_coords);//[0],pixel_coords[1],pixel_coords[2],pixel_coords[3]);
    // std::cerr << nPix << "\n";
    if ( nPix == 0 ) {// && checkBorder(pixel_coords, coordMapper.limit, false ) == true){
      box.set_empty();
    } else {
      contractDim(pixel_coords, nPix);



}

void SepRaster::contract(IntervalVector& box){
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

    size_t box_pixel_size = (pixel_coords[1]+1 - pixel_coords[0]) * (pixel_coords[3]+1 - pixel_coords[2]);
    unsigned int nPix = enclosed_pixels(pixel_coords);//[0],pixel_coords[1],pixel_coords[2],pixel_coords[3]);
    // std::cerr << nPix << "\n";
    if ( nPix == 0 ) {// && checkBorder(pixel_coords, coordMapper.limit, false ) == true){
      box.set_empty();
    } else {
      contractDim(pixel_coords, nPix);
      // std::cerr << pixel_coords[0] << " " << pixel_coords[1] << " "<< pixel_coords[2] << " "<< pixel_coords[3] << " "<< " " << nPix <<  " " << enclosed_pixels(pixel_coords) << "\n";


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
