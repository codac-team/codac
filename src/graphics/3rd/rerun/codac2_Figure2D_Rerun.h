/** 
 *  \file codac2_Figure2D_Rerun.h
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
#include "codac2_OutputFigure2D.h"
#include "codac2_Rerun_common.h"

namespace codac2
{
  class Figure2D;

  /**
   * \class Figure2D_Rerun
   * \brief 2D Rerun graphics backend
   */
  class Figure2D_Rerun : public OutputFigure2D
  {
    public:

      /**
       * \brief Creates a new Figure2D_Rerun object linked to a given figure
       * 
       * \param fig Figure2D to use
       */
      Figure2D_Rerun(const Figure2D& fig);

      /**
       * \brief Destructor for the Figure2D_Rerun object
       */
      virtual ~Figure2D_Rerun();

      /**
       * \brief Updates the axes of the figure
       */
      void update_axes() override;

      /**
       * \brief Updates the window properties of the figure
       */
      void update_window_properties() override;

      /**
       * \brief Centers the viewbox
       */
      void center_viewbox(const Vector& c, const Vector& r) override;

      /**
       * \brief Clears the figure
       */
      void clear() override;

      /**
       * \brief Saves the figure to a file
       * 
       * \param filename Name of the file to save the figure to (.rrd)
       */
      void save(const std::string& filename) override;

      // Geometric shapes

      void draw_point(const Vector& c, const StyleProperties& style = StyleProperties()) override;
      void draw_box(const IntervalVector& x, const StyleProperties& style = StyleProperties()) override;
      void draw_circle(const Vector& c, double r, const StyleProperties& style = StyleProperties()) override;
      void draw_ring(const Vector& c, const Interval& r, const StyleProperties& style = StyleProperties()) override;
      void draw_polyline(const std::vector<Vector>& x, float tip_length, const StyleProperties& style = StyleProperties()) override;
      void draw_polygon(const std::vector<Vector>& x, const StyleProperties& style = StyleProperties()) override;
      void draw_pie(const Vector& c, const Interval& r, const Interval& theta, const StyleProperties& style = StyleProperties()) override;
      void draw_ellipse(const Vector& c, const Vector& ab, double theta, const StyleProperties& style = StyleProperties()) override;

      // Robots

      void draw_tank(const Vector& x, float size, const StyleProperties& style = StyleProperties()) override;
      void draw_AUV(const Vector& x, float size, const StyleProperties& style = StyleProperties()) override;
      void draw_motor_boat(const Vector& x, float size, const StyleProperties& style = StyleProperties()) override;

      // Miscellaneous

      void draw_text(const std::string& text, const Vector& ul, double scale, const StyleProperties& style = StyleProperties()) override;
      void draw_raster(const std::string& filename, const IntervalVector& bbox, const StyleProperties& style = StyleProperties()) override;

    private:

#ifdef CODAC_WITH_RERUN
      rerun::RecordingStream _rec;
#endif
      size_t _item_count = 0;
      bool _is_saved = false;
      std::string _last_saved_filename;
  };
}
