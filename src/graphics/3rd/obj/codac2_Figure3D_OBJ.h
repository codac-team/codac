/** 
 *  \file codac2_Figure3D_OBJ.h
 * ----------------------------------------------------------------------------
 *  \date       2026
 *  \author     Maël Godard, Quentin Brateau
 *  \copyright  Copyright 2026 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <fstream>
#include <string>
#include <vector>
#include "codac2_OutputFigure3D.h"

namespace codac2
{
  /**
   * \class Figure3D_OBJ
   * \brief OBJ export implementation for 3D figures
   */
  class Figure3D_OBJ : public OutputFigure3D
  {
    public:

      Figure3D_OBJ(const Figure3D& fig);
      virtual ~Figure3D_OBJ();

      void update_axes() override;
      void clear() override;
      void save(const std::string& filename) override;

      void draw_point(const Vector& c, const StyleProperties& style = StyleProperties()) override;
      void draw_box(const IntervalVector& x, const StyleProperties& style = StyleProperties()) override;
      void draw_polyline(const std::vector<Vector>& x, const StyleProperties& style = StyleProperties()) override;
      void draw_triangle(const Vector &c, const Matrix &A,
                         const Vector &p1, const Vector &p2, const Vector &p3, 
                         const StyleProperties& style = StyleProperties()) override;
      void draw_triangle(const Vector &p1, const Vector &p2, const Vector &p3, 
                         const StyleProperties& style = StyleProperties()) override;
      void draw_polygon(const Vector &c, const Matrix &A, const std::vector<Vector> &l, 
                        const StyleProperties& style = StyleProperties()) override;
      void draw_parallelogram(const Vector &c, const Matrix &A,
                              const Vector &p, const Vector &v1, const Vector &v2, 
                              const StyleProperties& style = StyleProperties()) override;
      void draw_parallelepiped(const Parallelepiped& p, const StyleProperties& style = StyleProperties()) override;
      void draw_zonotope(const Zonotope& z, const StyleProperties& style = StyleProperties()) override;
      void draw_arrow(const Vector &c, const Matrix& A, const StyleProperties& style = StyleProperties()) override;
      void draw_axes(double size = 1.0, const Vector& origin = Vector::Zero(3)) override;
      void draw_surface(const Vector &c, const Matrix &A,
                        const Interval &Ip1, double dp1,
                        const Interval &Ip2, double dp2,
                        std::function<Vector(double,double)> f,
                        const StyleProperties& style = StyleProperties()) override;
      void draw_sphere(const Vector &c, const Matrix &A, const StyleProperties& style = StyleProperties()) override;
      void draw_ellipsoid(const Ellipsoid &e, const StyleProperties& style = StyleProperties()) override;
      void draw_car(const Vector &c, const Matrix &A, const StyleProperties& style = StyleProperties()) override;
      void draw_plane(const Vector &c, const Matrix &A, bool yaw_is_up = true, const StyleProperties& style = StyleProperties()) override;
      void draw_AUV(const Vector &c, const Matrix &A, const StyleProperties& style = StyleProperties()) override;

    private:

      size_t move_write_v(const Vector &c, const Matrix &A, const Vector &p);
      void set_color_internal(const Color &c);
      void set_style_internal(const StyleProperties& style);

      std::ofstream _file;
      size_t vertex_count = 0;
      bool lock_style = false;
      bool _is_open = false;
  };
}
