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

#include <ibex.h>
#include <codac_IntervalVector.h>
#include <codac_Ctc.h>
#include <codac_ThickTest.h>
#include <codac_ThickBox.h>
#include <codac_ThickPaving.h>
#include <codac_GeoMapper.h>
//#include "../image/codac_doc_image.h"

#include "../../core/contractors/static/codac_py_Ctc.h"
#include "../../core/separators/codac_py_Sep.h"


namespace py = pybind11;
using py::self;
using py::init;
using py::keep_alive;
using py::class_;

using namespace codac;
using ibex::opInter;
using ibex::opRestrict;
using ibex::opLower;
using ibex::opUpper;

#define DATA_TYPE unsigned long

class GeoImage : public ThickTest{

  // Georeferenced image with integral image inclusion test
public:
  GeoImage(py::array_t<DATA_TYPE>  data, double x0, double y0, double dx, double dy, ThickBoolean outerVal=MAYBE) :
  // II(data),
  info_i(data.request()),
  boundingBox(2),
  coordMapper(x0, y0, data.request().shape[0], data.request().shape[1], dx, dy),
  outerVal(outerVal),
  ThickTest(2) {
    if( info_i.ndim != 2 )
      throw std::runtime_error("Number of dimensions must be one");
      boundingBox = getBoundingBox();
  }


  DATA_TYPE  pixelAt( int x, int y );
  DATA_TYPE enclosed_pixels( int xmin,int xmax,int ymin,int ymax);
  ThickBoolean test(const IntervalVector& box);
  IntervalVector getBoundingBox();

  ThickBoolean is_projection_included(IntervalVector& box, int dim);
  // return True if box intersects  the image
  bool test_intersects(const IntervalVector& box);

  ThickBoolean test_bb(const ThickBox &tbox);

  PixelCoords world_to_grid(IntervalVector box) { return coordMapper.world_to_grid(box); }
  IntervalVector grid_to_world(const PixelCoords& pixel_coords) {return coordMapper.grid_to_world(pixel_coords); }

  bool checkBorder(PixelCoords& c, const std::vector<bool>& limit, const ThickBoolean & b);
  // void contract(IntervalVector& box) {};
  // void contract(IntervalVector& box);
  // void contract(int& cxmin, int& cxmax, int& cymin, int& cymax);

  IntervalVector boundingBox;
  ThickBoolean outerVal;
private:
  // py::array_t<DATA_TYPE> II;
  GeoMapper coordMapper;
  py::buffer_info  info_i;
  // DATA_TYPE * img_ptr;
};


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
    DATA_TYPE b1 = pixelAt(xmax,ymax);
    DATA_TYPE b2 = pixelAt(xmax,ymin-1);
    DATA_TYPE b3 = pixelAt(xmin-1,ymax);
    DATA_TYPE b4 = pixelAt(xmin-1,ymin-1);
    return b1 - b2 - b3 + b4;
}

inline bool GeoImage::test_intersects(const IntervalVector& box){
  PixelCoords pixel_coords = world_to_grid(box);
  size_t box_pixel_size = (pixel_coords[1]+1 - pixel_coords[0]) * (pixel_coords[3]+1 - pixel_coords[2]);
  unsigned int nPix = enclosed_pixels(pixel_coords[0],pixel_coords[1],pixel_coords[2],pixel_coords[3]);
  return (nPix > 0 );
}

int boxSize(int xmin, int xmax, int ymin, int ymax){
    return (xmax+1 - xmin) * (ymax+1 - ymin);
}

inline bool GeoImage::checkBorder(PixelCoords& c, const std::vector<bool>& limit, const ThickBoolean & b){
  // Check the border to see if it is a pixel limit
  int val, bound, nPix;
  for(int i = 0; i < limit.size(); i++){
    if (limit[i] == false) continue;
    switch (i) {
      case 0:
        bound = std::max<int>(0,c[0]-1);
        val = ( b == IN ) ? boxSize(bound, bound, c[2], c[3]) : 0;
        nPix = enclosed_pixels(bound, bound, c[2], c[3]);
        break;
      case 1:
        bound = std::min<int>(info_i.shape[0]-1,c[1]+1);
        val = ( b == IN ) ? boxSize(bound, bound, c[2], c[3]) : 0;
        nPix = enclosed_pixels(bound, bound, c[2], c[3]);
        break;
      case 2:
        bound = std::max(0,c[2]-1);
        val = ( b == IN ) ? boxSize(c[0], c[1], bound, bound) : 0;
        nPix = enclosed_pixels(c[0], c[1], bound, bound);
        break;
      case 3:
        bound = std::min<int>(info_i.shape[1]-1,c[3]+1);
        val = ( b == IN ) ? boxSize(c[0], c[1], bound, bound) : 0;
        nPix = enclosed_pixels(c[0], c[1], bound, bound);
        break;
    }

    if (nPix != val) return false;
  }
  return true;
}

inline ThickBoolean GeoImage::test(const IntervalVector& box){

    assert(box.size() == info_i.ndim);
    if(box.is_empty()) return OUT;
    if(box.is_disjoint(getBoundingBox()) ) return outerVal;
    // if((box & boundingBox).is_flat() ) return MAYBE;


    // Convert world coordinates into pixel coordinates
    PixelCoords pixel_coords = world_to_grid(box & boundingBox);
    size_t box_pixel_size = (pixel_coords[1]+1 - pixel_coords[0]) * (pixel_coords[3]+1 - pixel_coords[2]);
    unsigned int nPix = enclosed_pixels(pixel_coords[0],pixel_coords[1],pixel_coords[2],pixel_coords[3]);
    // std::cout << "box_pixel_size/nPix  " <<   box_pixel_size << " " << nPix << "\n";
    ThickBoolean result;
    if (nPix == box_pixel_size){
      result = checkBorder(pixel_coords, coordMapper.limit, IN ) ? IN : UNK;
    } else if (nPix == 0) {
      result = checkBorder(pixel_coords, coordMapper.limit, OUT ) ? OUT : UNK;
    } else {
      result = UNK;
    }

    // std::cout << getBoundingBox() << "\n";
    // exit(-1);
    if ( !box.is_subset(getBoundingBox()) ){
      // std::cerr << result << " " << opUnion(result,MAYBE)
                // << " " << nPix << " " << pixel_coords[0] << " " << pixel_coords[1] << " " << pixel_coords[2] << " " << pixel_coords[3] << " "
                // <<  "\n";
      return  result | outerVal;
    }
    return result;

}

inline IntervalVector GeoImage::getBoundingBox(){
    int xmax = info_i.shape[0]-1;
    int ymax = info_i.shape[1]-1;
    return grid_to_world({ 0, xmax, 0, ymax});
}

inline ThickBoolean GeoImage::test_bb(const ThickBox &tbox){
  if ( tbox.superset().is_disjoint(boundingBox) ) return MAYBE;
  if ( tbox.superset().is_subset(boundingBox) ) return IN;

  bool intersects = tbox.intersects(boundingBox);
  bool notInclude = tbox.is_not_subset(boundingBox);
  if (intersects && notInclude) return MAYBE;
  if (intersects && !notInclude) return MAYBE_IN;
  if (!intersects && notInclude) return MAYBE;
  return MAYBE_IN;
}

inline ThickBoolean GeoImage::is_projection_included(IntervalVector& box, int dim){
  if(box.is_empty()) return OUT;
  if(box.is_disjoint(getBoundingBox()) ) return MAYBE;

  // Convert world coordinates into pixel coordinates
  PixelCoords pixel_coords = world_to_grid(box & boundingBox);
  // std::cout << (box & boundingBox) << " " << pixel_coords[0] << " " << pixel_coords[1] << " " << pixel_coords[2] << " " << pixel_coords[3]  << "\n";
  bool is_filled_out = true;
  bool is_filled_in = true;
  int idx_deb = (dim == 0) ? pixel_coords[0] : pixel_coords[2];
  int idx_end = (dim == 0) ? pixel_coords[1] : pixel_coords[3];
  int idx_min = (dim == 0) ? pixel_coords[2] : pixel_coords[0];
  int idx_max = (dim == 0) ? pixel_coords[3] : pixel_coords[1];

  for (int i = idx_deb; i <= idx_end; i++){
    size_t box_pixel_size = (idx_max+1 - idx_min) * 1;
    DATA_TYPE nPix = (dim == 0) ? enclosed_pixels(i,i,idx_min, idx_max) : enclosed_pixels(idx_min, idx_max, i, i);
    is_filled_in &= ( nPix > 0 );
    is_filled_out &= ( nPix != box_pixel_size );
  }
  // std::cerr << box  << " " << is_filled_in << " " << is_filled_out << " " << box.is_subset(img_in.boundingBox) << "\n";
  // if ( !box.is_subset(img_in.boundingBox)  && box[0].is_subset(img_in.boundingBox[0]) && (is_filled_out || is_filled_in) )
    // return MAYBE;
  if (is_filled_in && is_filled_out) return MAYBE;
  else if (is_filled_in && !is_filled_out) return MAYBE_IN;
  else if (!is_filled_in && is_filled_out) return MAYBE_OUT;
  return UNK;

}

ThickBoolean opMask(const ThickBoolean& a, const ThickBoolean& m){
  if(m == IN || is_singleton(a)) return a;
  // if ( is_singleton(a) ) return a;
  else if (m == MAYBE) return m; // && a == MAYBE_IN) return MAYBE;
  // else if (m == MAYBE && a == MAYBE_OUT) return MAYBE;
  // else if (m == MAYBE && a == UNK) return MAYBE;
  else if (m == MAYBE_IN ) return a;
  // if ( a == MAYBE_IN && m == MAYBE) return MAYBE;
  // if ( a == MAYBE_OUT && m == MAYBE) return MAYBE;
  return UNK;
}

class ThickGeoImage : public ThickTest {
public:
  ThickGeoImage(py::array_t<DATA_TYPE>& data_in, py::array_t<DATA_TYPE>& data_out,
    double x0, double y0, double dx, double dy, ThickBoolean outerVal=MAYBE) :
  img_in(data_in, x0, y0, dx, dy, outerVal),
  img_out(data_out, x0, y0, dx, dy, outerVal), ThickTest(2) {
  }

  ThickBoolean test( const IntervalVector& box ){
    ThickBoolean res = opSupSub(img_in.test(box), img_out.test(box));
    // if ( box.is_disjoint(img_in.getBoundingBox()) ) return MAYBE;
    // else if ( box.is_subset(img_in.getBoundingBox()) ) return  res;
    // std::cerr << res << " " << opUnion(res,MAYBE) << "\n";
    return res; // | MAYBE;
  }

  ThickBoolean projection_is_include(IntervalVector& box, int dim);
  // ThickBoolean projection_dim0_is_include(IntervalVector& box);
  ThickBoolean projection_dim1_is_include(IntervalVector& box);
  ThickBoolean intersects( const ThickBox& tbox);
  ThickBoolean intersects_inside(const ThickBox& tbox);
  GeoImage img_in, img_out;
  double timestamp;
};

ThickBoolean opCCCC(const ThickBoolean& in, const ThickBoolean& out){
    if (out == MAYBE && !is_singleton(in)) return MAYBE;
    if (in == MAYBE && !is_singleton(out)) return MAYBE;
    if (in == UNK && out == MAYBE_IN) return MAYBE_IN;
    if (in == MAYBE_OUT && out == UNK) return MAYBE_OUT;
    if (in == MAYBE_OUT && out  == MAYBE_IN) return MAYBE;
    return opUnion(in, out);
}

ThickBoolean ThickGeoImage::projection_is_include(IntervalVector& box, int dim){

  ThickBoolean bin =  img_in.is_projection_included(box, dim);
  ThickBoolean bout =  img_out.is_projection_included(box, dim);
  ThickBoolean res = opCCCC(bin, bout);
  // std::cerr  << bin << " " << bout << " " << res << "\n";
  return res;

  // PixelCoords pixel_coords = img_in.world_to_grid(box&img_in.boundingBox);
  // bool is_filled_out = true;
  // bool is_filled_in = true;
  // for (int i = pixel_coords[0]; i <= pixel_coords[1]; i++){
  //   size_t box_pixel_size = (pixel_coords[3]+1 - pixel_coords[2]) * 1;
  //   is_filled_in &= ( img_in.enclosed_pixels(i,i,pixel_coords[2], pixel_coords[3]) > 0 );
  //   is_filled_out &= ( img_in.enclosed_pixels(i,i,pixel_coords[2], pixel_coords[3]) != box_pixel_size );
  // }
  // std::cerr << box  << " " << is_filled_in << " " << is_filled_out << " " << box.is_subset(img_in.boundingBox) << "\n";
  // // if ( !box.is_subset(img_in.boundingBox)  && box[0].is_subset(img_in.boundingBox[0]) && (is_filled_out || is_filled_in) )
  //   // return MAYBE;
  // if (is_filled_in && is_filled_out) return MAYBE;
  // else if (is_filled_in && !is_filled_out) return MAYBE_IN;
  // else if (!is_filled_in && is_filled_out) return MAYBE_OUT;
  // return UNK;
}

ThickBoolean ThickGeoImage::projection_dim1_is_include(IntervalVector& box){
  PixelCoords pixel_coords = img_in.world_to_grid(box&img_in.boundingBox);
  bool is_filled_out = true;
  bool is_filled_in = true;
  for (int i = pixel_coords[2]; i <= pixel_coords[3]; i++){
    size_t box_pixel_size = (pixel_coords[1]+1 - pixel_coords[0]) * 1;
    is_filled_in &= ( img_in.enclosed_pixels(pixel_coords[0], pixel_coords[1], i, i) > 0 );
    is_filled_out &= ( img_out.enclosed_pixels(pixel_coords[0], pixel_coords[1], i, i) != box_pixel_size );
  }
  // std::cerr << box  << " " << is_filled_in << " " << is_filled_out << "\n";
  if (is_filled_in && is_filled_out) return MAYBE;
  else if (is_filled_in && !is_filled_out) return MAYBE_IN;
  else if (!is_filled_in && is_filled_out) return MAYBE_OUT;
  // if ( !box.is_subset(img_in.boundingBox) && (is_filled_out) )
    // return MAYBE;
  return UNK;
}


inline ThickBoolean ThickGeoImage::intersects_inside(const ThickBox& tbox){
  assert(tbox.superset().is_subset(img_in.getBoundingBox() ) );
  IntervalVector X = tbox.superset();
  ThickBoolean res0 = this->test(X);

  // Check of res0 is a singleton
  if ( is_singleton(res0) ){
    return res0;
  }

  // Need to use lower and upper bounds
  IntervalVector inner(2);
  std::vector<bool> status(2);
  std::tie(inner, status) = tbox.inner_box();

  if ( status[0] && status[1] ){
    ThickBoolean tmp =  img_in.test( inner );
    if (tmp == IN ) return MAYBE_IN;
    if (tmp == OUT) return MAYBE_OUT;
    return tmp;
  }  else if (status[0] == false && status[1] == false){
        bool t1 =  (img_in.test(inner) != IN);
        bool t2 =  (img_out.test(inner) != OUT);
        // std::cerr << ">>>> T1/T2 " << t1 << " " << t2 << "\n";
        if(t1 && t2 ) return MAYBE;
        if(t1 && !t2) return MAYBE_OUT;
        if(!t1 && t2) return MAYBE_IN;
        return UNK;
  } else if (status[0] ^ status[1]){
    if (status[0] && !status[1]){
      return projection_is_include(inner, 0);
    } else {
      return projection_is_include(inner, 1);
    }
  }
  return UNK;
}


inline ThickBoolean ThickGeoImage::intersects( const ThickBox& tbox){

  ThickBox subtBox = tbox.inter(ThickBox( img_in.getBoundingBox()));
  // std::cout << subtBox << " "  << img_in.getBoundingBox() << "\n";
  if (subtBox.is_empty()) return img_in.outerVal;
  ThickBoolean res = intersects_inside(subtBox);
  if (tbox.superset().is_subset(img_in.getBoundingBox()) ) return res;
//  return res | img_in.outerVal;
  // return intersects_inside(subtBox);

  IntervalVector X = tbox.superset();
  ThickBoolean res0 = this->test(X);

  // Check of res0 is a singleton
  if ( is_singleton(res0) ){
      if ( !X.is_subset(img_in.getBoundingBox()) )
        // Case when tbox not completely included in the image
        return  res0 | MAYBE;
      else
        return res0;
  }


  // Need to use lower and upper bounds
  IntervalVector inner(2);
  std::vector<bool> status(2);
  std::tie(inner, status) = tbox.inner_box();

  if ( status[0] && status[1] ){
    ThickBoolean tmp =  img_in.test( inner );
    if (tmp == IN ) return MAYBE_IN;
    if (tmp == OUT) return MAYBE_OUT;
    return tmp;
  }  else if (status[0] == false && status[1] == false){
        bool t1 =  (img_in.test(inner) != IN);
        bool t2 =  (img_out.test(inner) != OUT);
        // std::cerr << ">>>> T1/T2 " << t1 << " " << t2 << "\n";
        if(t1 && t2 ) return MAYBE;
        if(t1 && !t2) return MAYBE_OUT;
        if(!t1 && t2) return MAYBE_IN;
        return UNK;
  } else if (status[0] ^ status[1]){
    static int nb = 0;
    if(inner.is_disjoint(img_in.boundingBox) ) return MAYBE;

    if (status[0] && !status[1]){
      return opMask(projection_is_include(inner, 0), img_in.test_bb(tbox) );
      // return projection_dim0_is_include(inner);
      // ThickBoolean result = projection_dim0_is_include(inner);
      // std::cerr << ">>>>>>>>>>>>>>> " << result << "\n";
      // if (!inner.is_subset(img_in.boundingBox))
        // return opMask(projection_dim0_is_include(inner), img_in.test_bb(tbox) );
        // return result; //projection_dim0_is_include(inner);//, img_in.test_bb(tbox) );
        // std::cerr << "NONONON\n";
      // else
        // return result;
    } else {
      return  opMask(projection_is_include(inner, 1), img_in.test_bb(tbox) );
      // return projection_dim1_is_include(inner);
      // if ( !inner.is_subset(img_in.boundingBox) )
        // return MAYBE | result;
      // return result;
    }
    return UNK;
  }

    // else {
    //   tmp = tbox.inner_box();
    //   for (int i = 0 ; i < sub.first.size(), i++){
    //       if ( !sub.first[i].is_empty() ) continue;
    //
    //   }
    // }
    return UNK;
}


//
//
//
// class CtcImage : public Ctc
// {
// public:
//   CtcImage(py::array_pypolart<int>& data, double x0, double y0, double dx, double dy):
//   I(data,x0,y0,dx, dy) {}
//
//   void contract(IntervalVector& box);
//
//   void contract()
//
//
// private:
//   GeoImage I;
// }
//
//
// //----------------------------------------------------------------------------------------------------------------
// void CtcPixelMap::contract(IntervalVector& box) {
//
//     assert(box.size() == (int)I.ndim);
//     if(box.is_empty()) return;
//
//     // Convert world coordinates into pixel coordinates
//     world_to_grid(box);
//
//     // Contractor the box
//     if( I.ndim == 2) {
//         contract(pixel_coords[0],pixel_coords[1],pixel_coords[2],pixel_coords[3]);
//     } else if ( I.ndim == 3){
//         contract(pixel_coords[0],pixel_coords[1],pixel_coords[2],pixel_coords[3],pixel_coords[4],pixel_coords[5]);
//     }
//     // Check the result
//     if(pixel_coords[0] == -1) {
//         box.set_empty();
//         return;
//     }
//
//     // Convert pixel coordinates into world coordinates
//     grid_to_world(box);
// }
//
// //------------------------------------------------------------------------------
// //psi contraction
// void CtcPixelMap::contract(int& cxmin, int& cxmax, int& cymin, int& cymax) {
//
//     //compute enclosed pixels on consecutive lines from all dimensions of the box
//
//     cxmax = std::max(0,std::min(I.grid_size_[0]-1,cxmax));
//     cxmin = std::min(I.grid_size_[0]-1,std::max(0,cxmin));
//     cymax = std::max(0,std::min(I.grid_size_[1]-1,cymax));
//     cymin = std::min(I.grid_size_[1]-1,std::max(0,cymin));
//
//     if (enclosed_pixels(cxmin,cxmax,cymin,cymax) == 0){
//         cxmin = -1;
//         return;
//     }
//
//     //right
//     for (int i = cxmin; i <= cxmax; i++)
//     {
//         if(enclosed_pixels(cxmin,i,cymin,cymax)>0) {
//             cxmin = i;
//             break;
//         }
//     }
//     //left
//     for (int i = cxmax; i >= cxmin ; i--)
//     {
//         if(enclosed_pixels(i,cxmax,cymin,cymax)>0) {
//             cxmax = i;
//             break;
//         }
//     }
//     //down
//     for (int j = cymin; j <=cymax; j++)
//     {
//         if(enclosed_pixels(cxmin,cxmax,cymin,j)>0) {
//             cymin = j;
//             break;
//         }
//     }
//     //up
//     for (int j = cymax; j >= cymin ; j--)
//     {
//         if(enclosed_pixels(cxmin,cxmax,j,cymax)>0) {
//             cymax = j;
//             break;
//         }
//     }
// }



class GeoImageTranslate : public ThickTest {
public:
  GeoImageTranslate(ThickGeoImage& test, IntervalVector T): ThickTest(test.nb_var), thickImg(test), T(T) {}

  ThickBoolean test(const IntervalVector& box){
    return thickImg.intersects(ThickBox(box + T.lb(), box + T.ub() ) );
  }
  void setParams(const IntervalVector& box) { T = box;}
private:
  ThickGeoImage& thickImg;
  IntervalVector T;
};


class ThickImgAEqualB : public ThickTest{

public:
  ThickImgAEqualB(ThickGeoImage& A, ThickGeoImage&B, double eps ) :ThickTest(2), A(A), B(B), eps(eps){}

  ThickBoolean is_AsubsetB(ThickGeoImage& A, ThickGeoImage& B, const  IntervalVector& dX, double eps){

    codac::ThickPaving P_lb(B.img_in.boundingBox, MAYBE);
    GeoImageTranslate T(A, dX);
    codac::FuncTest test_lb = [&T, &B](const IntervalVector &X) { return opRestrict(opLower(T.test(X)), opUpper(B.test(X)));};
    P_lb.FastSivia(test_lb, eps, opInter);
    ThickBoolean b2 = (P_lb.check_empty());

    if (b2 == IN)
      return OUT;
    return UNK;
  }


  ThickBoolean test(const IntervalVector& dX){

    ThickBoolean b1 = is_AsubsetB(A, B, dX, eps);
    ThickBoolean b2 = is_AsubsetB(B, A, -dX, eps);
    // return b1;
    return opAnd(b1, b2);

    // return res;
  }
private:
  ThickGeoImage &A, &B;
  double eps;
};

using namespace pybind11::literals;

// now in codac_thickset.cpp: void export_unsupported_thickgeoimage(py::module& m, py::class_<Ctc, pyCtc>& ctc, py::class_<ibex::Sep, pySep>& sep)
// now in codac_thickset.cpp: {
// now in codac_thickset.cpp:   // py::module m("image", "python binding CtcIMage Class");
// now in codac_thickset.cpp: 
// now in codac_thickset.cpp:   py::object thickTest = (py::object) py::module::import("codac.unsupported").attr("ThickTest");
// now in codac_thickset.cpp: 
// now in codac_thickset.cpp:   // py::object ctc = (py::object) py::module::import("codac").attr("Ctc");
// now in codac_thickset.cpp:   // class_<GeoImage>(m, "GeoImage", thickTest)
// now in codac_thickset.cpp:   //     .def(init<py::array_t<DATA_TYPE> , double, double , double , double, ThickBoolean >(), DOC_GEOIMAGE_CONSTRUCTOR,
// now in codac_thickset.cpp:   //          py::keep_alive<1,2>(), "img"_a, "x0"_a, "y0"_a, "dx"_a, "dy"_a, "outerVal"_a=MAYBE)
// now in codac_thickset.cpp:   //     .def("world_to_grid", &GeoImage::world_to_grid, DOC_GEOIMAGE_WORLD_TO_GRID, "box"_a)
// now in codac_thickset.cpp:   //     .def("grid_to_world", &GeoImage::grid_to_world, DOC_GEOIMAGE_GRID_TO_WORLD, "pixel_coords"_a)
// now in codac_thickset.cpp:   //     .def("pixelAt", &GeoImage::pixelAt, DOC_GEOIMAGE_PIXELAT, "x"_a, "y"_a)
// now in codac_thickset.cpp:   //     .def("enclosed_pixels", &GeoImage::enclosed_pixels, DOC_GEOIMAGE_ENCLOSED_PIXELS,
// now in codac_thickset.cpp:   //                           "xmin"_a, "xmax"_a, "ymin"_a, "ymax"_a )
// now in codac_thickset.cpp:   //     .def("test", &GeoImage::test, DOC_GEOIMAGE_TEST, "box"_a)
// now in codac_thickset.cpp:   //     .def("test_bb", &GeoImage::test_bb)
// now in codac_thickset.cpp:   //     .def_property_readonly("boundingBox", [](GeoImage& self){return self.boundingBox;})
// now in codac_thickset.cpp:   //     // .def("is_projection_included", GeoImage::is_projection_included)
// now in codac_thickset.cpp:   //     ;
// now in codac_thickset.cpp: 
// now in codac_thickset.cpp:   class_<ThickGeoImage>(m, "ThickGeoImage", thickTest)
// now in codac_thickset.cpp:       .def(init<py::array_t<DATA_TYPE>&, py::array_t<DATA_TYPE>&, double, double, double, double, ThickBoolean>(),
// now in codac_thickset.cpp:            py::keep_alive<1,2>(), py::keep_alive<1,3>(),
// now in codac_thickset.cpp:            "img_in"_a, "img_out"_a, "x0"_a, "y0"_a, "dx"_a, "dy"_a, "outerVal"_a=MAYBE)
// now in codac_thickset.cpp:       .def("test", &ThickGeoImage::test)
// now in codac_thickset.cpp:       .def("intersects", &ThickGeoImage::intersects)
// now in codac_thickset.cpp:       .def_readonly("img_in", &ThickGeoImage::img_in)
// now in codac_thickset.cpp:       .def_readonly("img_out", &ThickGeoImage::img_out)
// now in codac_thickset.cpp:       .def_readwrite("timestamp", &ThickGeoImage::timestamp)
// now in codac_thickset.cpp:       // .def_property_readinly("boundingBox", [](ThickGeoImage& self){return self.boundingBox;})
// now in codac_thickset.cpp:   ;
// now in codac_thickset.cpp: 
// now in codac_thickset.cpp:   // class_<GeoMapper>(m, "GeoMapper")
// now in codac_thickset.cpp:   //     .def(init<double, double, int, int, double, double>(), DOC_GEOMAPPER_CONSTRUCTOR, "x0"_a, "y0"_a, "x_size"_a, "y_size"_a, "dx"_a, "dy"_a)
// now in codac_thickset.cpp:   //     .def("world_to_grid", &GeoMapper::world_to_grid, DOC_GEOMAPPER_CONSTRUCTOR, "box"_a)
// now in codac_thickset.cpp:   //     .def("grid_to_world", &GeoMapper::grid_to_world, DOC_GEOMAPPER_GRID_TO_WORLD, "pixel_coords"_a)
// now in codac_thickset.cpp:   //     ;
// now in codac_thickset.cpp: 
// now in codac_thickset.cpp:   class_<GeoImageTranslate>(m, "GeoImageTranslate", thickTest)
// now in codac_thickset.cpp:       .def(init<ThickGeoImage&, IntervalVector&>(), py::keep_alive<1,2>())
// now in codac_thickset.cpp:       .def("test", &GeoImageTranslate::test)
// now in codac_thickset.cpp:       ;
// now in codac_thickset.cpp: 
// now in codac_thickset.cpp:     class_<ThickImgAEqualB>(m, "ThickImgAEqualB", thickTest)
// now in codac_thickset.cpp:         .def(init<ThickGeoImage&,ThickGeoImage&, double>(), py::keep_alive<1,2>())
// now in codac_thickset.cpp:         .def("test", &ThickImgAEqualB::test)
// now in codac_thickset.cpp:         ;
// now in codac_thickset.cpp:     // return m.ptr();
// now in codac_thickset.cpp: }