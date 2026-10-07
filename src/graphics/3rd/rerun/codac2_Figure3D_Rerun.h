/** 
 *  \file codac2_Figure3D_Rerun.h
 * ----------------------------------------------------------------------------
 *  \date       2026
 *  \author     Quentin Brateau
 *  \copyright  Copyright 2026 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <string>
#include <vector>
#include <memory>
#include "codac2_OutputFigure3D.h"
#include "codac2_Rerun_common.h"

namespace codac2
{
  class Figure3D;

  /**
   * \class Figure3D_Rerun
   * \brief 3D Rerun graphics backend
   */
  class Figure3D_Rerun : public OutputFigure3D
  {
    public:

      /**
       * \brief Creates a new Figure3D_Rerun object linked to a given figure
       * 
       * \param fig Figure3D to use
       */
      Figure3D_Rerun(const Figure3D& fig);

      /**
       * \brief Destructor for the Figure3D_Rerun object
       */
      virtual ~Figure3D_Rerun();

      /**
       * \brief Updates the axes of the figure
       */
      void update_axes() override;

      /**
       * \brief Clears the figure
       */
      void clear() override;

      /**
       * \brief Saves the figure to a file (.rrd)
       */
      void save(const std::string& filename) override;

      // Geometric shapes

      void draw_point(const Vector& c, const StyleProperties& style = StyleProperties()) override;
      void draw_box(const IntervalVector& x, const StyleProperties& style = StyleProperties()) override;
      void draw_polyline(const std::vector<Vector>& x, const StyleProperties& style = StyleProperties()) override;
      void draw_triangle(const Vector &c, const Matrix &A,
                         const Vector &p1, const Vector &p2, const Vector &p3, 
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

      // Vehicles / Robots

      void draw_car(const Vector &c, const Matrix &A, const StyleProperties& style = StyleProperties()) override;
      void draw_plane(const Vector &c, const Matrix &A, bool yaw_is_up = true, const StyleProperties& style = StyleProperties()) override;
      void draw_AUV(const Vector &c, const Matrix &A, const StyleProperties& style = StyleProperties()) override;

      // Pavings

      void draw_paving(const PavingOut& p, const StyleProperties& boundary_style = StyleProperties()) override;
      void draw_paving(const PavingInOut& p, const StyleProperties& boundary_style = StyleProperties(),
                       const StyleProperties& inside_style = StyleProperties()) override;

    private:

#ifdef CODAC_WITH_RERUN
      rerun::RecordingStream _rec;
#endif
      size_t _item_count = 0;
      bool _is_saved = false;
      std::string _last_saved_filename;
  };
}
