//============================================================================
//                               P Y I B E X
// File        : GeoMapper.h
// Author      : Benoit Desrochers
// Copyright   : Benoit Desrochers
// License     : LGPLv3 See the LICENSE file
// Created     : 08/08/2016
// Update      : 16/07/2017
//============================================================================


#ifndef __GEO_MAPPER_H__
#define __GEO_MAPPER_H__

#include <codac_IntervalVector.h>
#include <iomanip>      // std::setprecision
using codac::IntervalVector;
using codac::Interval;
using PixelCoords = std::array<int, 4>;

// Class for converting world coordinates to image ones

class GeoMapper {
public:
  GeoMapper(double x_lb, double y_ub, int x_size, int y_size,
            double x_res, double y_res ) :
            origin_{x_lb, y_ub}, shape_{x_size, y_size}, leaf_size_{x_res, y_res}, ndim(2), limit(4, false),
            bbox(2)
  {
    bbox = grid_to_world({0,x_size, 0, y_size});
  }

  PixelCoords world_to_grid(const IntervalVector& box);
  IntervalVector grid_to_world(const PixelCoords& pixel_coords);

  double origin_[2];
  double leaf_size_[2];
  int shape_[2];
  int ndim;
  std::vector<bool> limit;
  IntervalVector bbox;

};


//##############################################################################
//###### INLINE IMPLEMENTATION
//##############################################################################


inline PixelCoords GeoMapper::world_to_grid(const IntervalVector& box) {
    PixelCoords pixel_coords;
    if (!box.is_subset(bbox)){
      std::stringstream ss;
      ss << "box " << box << " isn't a subset of " << bbox <<  " " << box.is_subset(bbox) <<"\n";
      std::cerr << box << " " << bbox << " " << box.is_subset(bbox) << "\n";
      throw std::runtime_error(ss.str());
    }
    // if (box.is_empty()){
    //   return {{-1, -1, -1, -1}};
    // }
    std::fill(this->limit.begin(), limit.end(), false);
    for(int i = 0; i < box.size(); i++) {
        // Limit range to image size on pixel_coord
        Interval eps = Interval(0) | Interval(leaf_size_[i]);
        Interval itv  = (box[i] - origin_[i]) / leaf_size_[i] - Interval(0,1);
        // std::cout << "[" << i << "]0 " << itv.lb() << " " << itv.ub() << " " << origin_[i] << " " << leaf_size_[i]  << " "<<"\n";

        itv &= Interval(0,shape_[i]-1);
        // std::cout.precision(10);
        if (itv.is_empty()){
          std::cerr << box[i] << "\n";
          assert(itv.is_empty() == false);
        }
        // if (itv.is_empty())

        // std::cout << "[" << i << "] " << itv.lb() << " " << itv.ub() << "\n";
        // std::cout << "bbox " << bbox << box[i].is_subset(bbox[i]) << "\n";
        pixel_coords[2*i]   = ceil(itv.lb());
        if (float(pixel_coords[2*i]) == itv.lb()) limit[2*i] = true;
        pixel_coords[2*i+1] = floor(itv.ub());
        if (float(pixel_coords[2*i+1]) == itv.ub()) limit[2*i+1] = true;
        // pixel_coords[2*i]   = floor(itv.lb());
        // if (float(pixel_coords[2*i]) == itv.lb()) limit[2*i] = true;
        // pixel_coords[2*i+1] = ceil(itv.ub());
        // if (float(pixel_coords[2*i+1]) == itv.ub()) limit[2*i+1] = true;

        if(pixel_coords[2*i] > pixel_coords[2*i+1]){
          std::swap(pixel_coords[2*i], pixel_coords[2*i+1]);
        }
        // std::cout.precision(10);
        // std::cout  << std::setprecision(9) << itv.lb() << " " << itv.ub() << " " << pixel_coords[2*i] << " " << pixel_coords[2*i+1] << "\n";
    }
    return pixel_coords;
}

//------------------------------------------------------------------------------
inline IntervalVector GeoMapper::grid_to_world(const PixelCoords& pixel_coords) {
    IntervalVector box(2);
    for(unsigned int i = 0; i < ndim; i++) {
        Interval eps = Interval(0) | Interval(leaf_size_[i]);
        // Interval itv  = (box[i] - origin_[i] - eps ) / leaf_size_s[i];
        box[i] &= Interval(pixel_coords[2*i], pixel_coords[2*i+1]) * leaf_size_[i] + origin_[i] + eps;
        if(box[i].is_empty()){
            box.set_empty();
            return box;
        }
    }
    return box;
}

#endif // __GEO_MAPPER_H__
