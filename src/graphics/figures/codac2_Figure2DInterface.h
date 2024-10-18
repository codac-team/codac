/** 
 *  \file codac2_Figure2DInterface.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <vector>
#include <string>
#include "codac2_Vector.h"
#include "codac2_IntervalVector.h"
#include "codac2_StyleProperties.h"

namespace codac2
{
  class Figure2DInterface
  {
    public:

      // Geometric shapes
      virtual void draw_point(const Vector& c, const StyleProperties& s = StyleProperties()) = 0;
      virtual void draw_box(const IntervalVector& x, const StyleProperties& s = StyleProperties()) = 0;
      virtual void draw_circle(const Vector& c, double r, const StyleProperties& s = StyleProperties()) = 0;
      virtual void draw_ring(const Vector& c, const Interval& r, const StyleProperties& s = StyleProperties()) = 0;
      virtual void draw_polyline(const std::vector<Vector>& x, float tip_length, const StyleProperties& s = StyleProperties()) = 0;
      virtual void draw_polygone(const std::vector<Vector>& x, const StyleProperties& s = StyleProperties()) = 0;
      virtual void draw_pie(const Vector& c, const Interval& r, const Interval& theta, const StyleProperties& s = StyleProperties()) = 0;
      virtual void draw_ellipse(const Vector& c, const Vector& ab, double theta, const StyleProperties& s = StyleProperties()) = 0;

      // Robots
      virtual void draw_tank(const Vector& x, float size, const StyleProperties& s = StyleProperties()) = 0;
      virtual void draw_AUV(const Vector& x, float size, const StyleProperties& s = StyleProperties()) = 0;
  };
}