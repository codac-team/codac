//============================================================================
//                                P Y I B E X
// File        : pyIbex_doc_Geoimage.h
// Author      : Benoit Desrochers
// Copyright   : Benoit Desrochers
// License     : See the LICENSE file
// Created     : Aug 23, 2016
//============================================================================


const char* __DOCS_CTC_TYPE_IMAGE=
R"_doc(This module provides image contractor and (thick) inclusion test
on a georeferenced occupancy grid based on Integral Image or Summed table area.

Initialy introduced by Jan SLIWKA (see his thesis for more detail), it allows
the used of unstructured data strored as a binary image to represent solution set.
The geoimage is described as a 2-dimentionnal occupancy grid with
1-valued pixels are said to be occupied while 0-valued are not.

)_doc";

const char* __DOC_GEOIMAGE_CONSTRUCTOR=
R"_doc(Construct a GeoImage inclusion test
Args:
  img ( numpy array ): summed table area of a binary image.
  x0 (float): x coordinate of the top left corner
  y0 (float): y coordinate of the top left corner
  dx (float): pixel resolution along the x axis
  dy (float): pixel resolution along the y axis
  outerVal (ThickBoolean): optional value for boxed outside tht bbox

Retuns a GeoImage object which inherits from ThickTest class
)_doc";


const char * __DOC_GEOIMAGE_WORLD_TO_GRID=
R"_doc(Converts coordinates from world frame to the image frame
  Args:
    box (IntervalVector): box in world frame
  Returns:
    tuple<int> : An tuple of 4 integers ( x_min, x_max, y_min, y_max )
)_doc";

const char * __DOC_GEOIMAGE_GRID_TO_WORLD=
R"_doc(Converts coordinates from image frame (pixel) to world frame coordinates.
Args:
  pixels_coords (tuple<int>): ( xmin, xmax, ymin, ymax )
Returns:
  IntervalVector : a box in world coordinates
)_doc";

const char * __DOC_GEOIMAGE_PIXELAT=
R"_doc(Return the value of the pixel at position x,y

Args:
  x (int): coodinates from upper left corner
  y (int): coodinates from upper left corner
returns:
  int : value of the integral image a position (x,y)

)_doc";

const char * __DOC_GEOIMAGE_ENCLOSED_PIXELS=
R"_doc(Return the number of 1-valued pixels in the box [xmin,xmax] x [ymin, ymax].

Args:
  xmin (int): x coordinate of the lower left corner
  xmax (int): x coordinate of the upper right corner
  ymin (int): y coordinate of the lower left corner
  ymax (int): y coordinate of the upper right corner
)_doc";

const char * __DOC_GEOIMAGE_TEST=
R"_doc(Thick inclusion test

Args:
  box (IntervalVector): box to test
Returns:
  ThickBoolean: IN if box is completly included in image (1-valued pixel)
                OUT if not
                INK otherwise
)_doc";

//------------------------------------------------------------------------------
// CLASS GEOMAPPER
//------------------------------------------------------------------------------
const char* __DOC_GEOMAPPER_CONSTRUCTOR=
R"_doc(Construct a GeoImage inclusion test
Args:
  x0 (float): x coordinate of the top left corner
  y0 (float): y coordinate of the top left corner
  x_size (int): number of pixel along the x axis
  y_size (int): number of pixel along the y axis
  dx (float): pixel resolution along the x axis
  dy (float): pixel resolution along the y axis

Retuns a GeoMapper object to transform world and image coordiante
)_doc";


const char * __DOC_GEOMAPPER_WORLD_TO_GRID=
R"_doc(Converts coordinates from world frame to the image frame
  Args:
    box (IntervalVector): box in world frame
  Returns:
    tuple<int> : An tuple of 4 integers ( x_min, x_max, y_min, y_max )
)_doc";

const char * __DOC_GEOMAPPER_GRID_TO_WORLD=
R"_doc(Converts coordinates from image frame (pixel) to world frame coordinates.
Args:
  pixels_coords (tuple<int>): ( xmin, xmax, ymin, ymax )
Returns:
  IntervalVector : a box in world coordinates
)_doc";

//------------------------------------------------------------------------------
