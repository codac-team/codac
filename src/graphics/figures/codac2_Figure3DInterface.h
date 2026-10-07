/** 
 *  \file codac2_Figure3DInterface.h
 * ----------------------------------------------------------------------------
 *  \date       2026
 *  \author     Quentin Brateau
 *  \copyright  Copyright 2026 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <vector>
#include <string>
#include <functional>
#include "codac2_Vector.h"
#include "codac2_Matrix.h"
#include "codac2_IntervalVector.h"
#include "codac2_IntervalMatrix.h"
#include "codac2_StyleProperties.h"
#include "codac2_StyleGradientProperties.h"
#include "codac2_Ellipsoid.h"
#include "codac2_Parallelepiped.h"
#include "codac2_Zonotope.h"
#include "codac2_SampledTraj.h"
#include "codac2_AnalyticTraj.h"
#include "codac2_SlicedTube.h"

namespace codac2
{
  class PavingOut;
  class PavingInOut;
  template<typename P>
  class Subpaving;

  /**
   * \class Figure3DInterface
   * \brief Interface for 3D figures
   * 
   * This class is used to display 3D figures. It defines the basic drawing functions.
   * Purely virtual interface defining low-level and high-level graphic primitives.
   */
  class Figure3DInterface
  {
    public:

      virtual ~Figure3DInterface() = default;

      // Geometric shapes

      /**
       * \brief Draws a point in 3D
       * 
       * \param c Coordinates of the point
       * \param style Style of the point
       */
      virtual void draw_point(const Vector& c, const StyleProperties& style = StyleProperties()) = 0;

      /**
       * \brief Draws a 3D box
       * 
       * \param x Box to draw
       * \param style Style of the box
       */
      virtual void draw_box(const IntervalVector& x, const StyleProperties& style = StyleProperties()) = 0;

      /**
       * \brief Draws a 3D line between two points
       * 
       * \param p1 First point
       * \param p2 Second point
       * \param style Style of the line
       */
      virtual void draw_line(const Vector& p1, const Vector& p2, const StyleProperties& style = StyleProperties()) = 0;

      /**
       * \brief Draws a 3D polyline connecting multiple points
       * 
       * \param x Vector of points
       * \param style Style of the polyline
       */
      virtual void draw_polyline(const std::vector<Vector>& x, const StyleProperties& style = StyleProperties()) = 0;

      /**
       * \brief Draws a triangle with translation and scaling
       *
       * \param c Translation
       * \param A Scaling / rotation
       * \param p1 First point
       * \param p2 Second point
       * \param p3 Third point
       * \param style Style
       */
      virtual void draw_triangle(const Vector &c, const Matrix &A,
                                 const Vector &p1, const Vector &p2, const Vector &p3, 
                                 const StyleProperties& style = StyleProperties()) = 0;
     
      /**
       * \brief Draws a triangle
       *
       * \param p1 First point
       * \param p2 Second point
       * \param p3 Third point
       * \param style Style
       */
      virtual void draw_triangle(const Vector &p1, const Vector &p2, const Vector &p3, 
                                 const StyleProperties& style = StyleProperties()) = 0;

      /**
       * \brief Draws a star-shaped polygon as a sequence of adjacent triangles
       *
       * \param c Translation
       * \param A Scaling / rotation
       * \param l Points
       * \param style Style
       */
      virtual void draw_polygon(const Vector &c, const Matrix &A, const std::vector<Vector> &l, 
                                const StyleProperties& style = StyleProperties()) = 0;

      /**
       * \brief Draws a parallelogram c + A (p + [-1,1]*v1 + [-1,1]*v2)
       *
       * \param c Translation
       * \param A Scaling / rotation
       * \param p Base point
       * \param v1 First direction vector
       * \param v2 Second direction vector
       * \param style Style
       */
      virtual void draw_parallelogram(const Vector &c, const Matrix &A,
                                      const Vector &p, const Vector &v1, const Vector &v2,
                                      const StyleProperties& style = StyleProperties()) = 0;

      /**
       * \brief Draws a parallelepiped
       * 
       * \param p Parallelepiped to draw
       * \param style Style
       */
      virtual void draw_parallelepiped(const Parallelepiped& p, const StyleProperties& style = StyleProperties()) = 0;

      /**
       * \brief Draws a zonotope
       * 
       * \param z Zonotope to draw
       * \param style Style
       */
      virtual void draw_zonotope(const Zonotope& z, const StyleProperties& style = StyleProperties()) = 0;

      /**
       * \brief Draws an arrow
       * 
       * \param c Start position
       * \param A Orientation matrix (first column is direction)
       * \param style Style
       */
      virtual void draw_arrow(const Vector &c, const Matrix& A, const StyleProperties& style = StyleProperties()) = 0;

      /**
       * \brief Draws the (x,y,z) axes
       * 
       * \param size Size of the axes
       * \param origin Origin of the axes
       */
      virtual void draw_axes(double size = 1.0, const Vector& origin = Vector::Zero(3)) = 0;

      /**
       * \brief Draws a parametric surface
       */
      virtual void draw_surface(const Vector &c, const Matrix &A,
                                const Interval &Ip1, double dp1,
                                const Interval &Ip2, double dp2,
                                std::function<Vector(double,double)> f,
                                const StyleProperties& style = StyleProperties()) = 0;

      /**
       * \brief Draws a sphere
       */
      virtual void draw_sphere(const Vector &c, const Matrix &A,
                               const StyleProperties& style = StyleProperties()) = 0;

      /**
       * \brief Draws an ellipsoid
       */
      virtual void draw_ellipsoid(const Ellipsoid &e,
                                  const StyleProperties& style = StyleProperties()) = 0;

      // Vehicles / Robots

      /** 
       * \brief Draws a car in 3D
       */
      virtual void draw_car(const Vector &c, const Matrix &A,
                            const StyleProperties& style = StyleProperties()) = 0;
     
      /** 
       * \brief Draws an airplane in 3D
       */
      virtual void draw_plane(const Vector &c, const Matrix &A, bool yaw_is_up = true,
                              const StyleProperties& style = StyleProperties()) = 0;

      /**
       * \brief Draws an AUV / submarine in 3D
       */
      virtual void draw_AUV(const Vector &c, const Matrix &A,
                            const StyleProperties& style = StyleProperties()) = 0;

      // Pavings

      /**
       * \brief Draws a paving boundary (PavingOut)
       */
      virtual void draw_paving(const PavingOut& p,
                               const StyleProperties& bound_style = StyleProperties()) = 0;

      /**
       * \brief Draws a paving with boundary and inside (PavingInOut)
       */
      virtual void draw_paving(const PavingInOut& p,
                               const StyleProperties& bound_style = StyleProperties(),
                               const StyleProperties& in_style = StyleProperties()) = 0;

      // Trajectories & Tubes

      /**
       * \brief Draws a sampled trajectory in 3D
       */
      virtual void draw_trajectory(const SampledTraj<Vector>& x,
                                   const StyleProperties& style = StyleProperties()) = 0;

      /**
       * \brief Draws an analytic trajectory in 3D
       */
      virtual void draw_trajectory(const AnalyticTraj<VectorType>& x,
                                   const StyleProperties& style = StyleProperties()) = 0;

      /**
       * \brief Draws a tube in 3D
       */
      virtual void draw_tube(const SlicedTube<IntervalVector>& x,
                             const StyleProperties& style = StyleProperties()) = 0;
  };
}
