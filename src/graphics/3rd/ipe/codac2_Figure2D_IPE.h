/** 
 *  \file codac2_Figure2D_IPE.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Maël Godard
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <vector>
#include <fstream>
#include "codac2_Figure2D.h"
#include "codac2_OutputFigure2D.h"
#include "codac2_Vector.h"
#include "codac2_IntervalVector.h"
#include "codac2_Ellipsoid.h"
#include "vibes.h"

namespace codac2
{
  /**
   * \class Figure2D_IPE
   * \brief IPE output class
   * 
   * This class is used to manage the IPE output. It generates an XML file that can be opened with the IPE editor.
   */
  class Figure2D_IPE : public OutputFigure2D
  {
    public:

      Figure2D_IPE(const Figure2D& fig);
      virtual ~Figure2D_IPE();
      void update_axes();
      void update_window_properties();
      void center_viewbox(const Vector& c, const Vector& r);
      void begin_path(const StyleProperties& s,bool tip = false);

      /* For future doc:
      https://github.com/codac-team/codac/pull/126#discussion_r1829030491
      Pour les véhicules (draw_tank et draw_AUV) le header par défaut du begin_path n'est pas suffisant.
      J'ai donc ajouté cette fonction qui fait le même travail que le begin_path, avec en plus le champ
      "matrix" complété.
      Ce champ contient 6 valeurs : les 4 premières sont la matrice de transformation 2D, rotation et
      dilatation, "par colonne" (i.e. m11, m21, m12, m22) et les 2 autres valeurs sont la translation.
      Le tout permet de scale le véhicule, l'orienter et le déplacer au bon endroit.
      Cette fonction écrit dans le xml quelque chose dans le style :
         <path layer="alpha" 
         stroke="codac_color_000000" 
         fill="codac_color_ffd32a" 
         opacity="100%" 
         stroke-opacity="100%" 
         pen="heavier" 
         matrix="0.00948009 11.9048 -11.9047 0.00948009 166.667 166.667">
      */
      void begin_path_with_matrix(const Vector& x, float length, const StyleProperties& s = StyleProperties());

      void draw_text(const Vector& c,const Vector& r, const std::string& text, const StyleProperties& s = StyleProperties());
      void draw_axes();

      // Geometric shapes
      void draw_point(const Vector& c, const StyleProperties& s = StyleProperties());
      void draw_box(const IntervalVector& x, const StyleProperties& s = StyleProperties());
      void draw_circle(const Vector& c, double r, const StyleProperties& s = StyleProperties());
      void draw_ring(const Vector& c, const Interval& r, const StyleProperties& s = StyleProperties());
      void draw_polyline(const std::vector<Vector>& x, float tip_length, const StyleProperties& s = StyleProperties());
      void draw_polygon(const std::vector<Vector>& x, const StyleProperties& s = StyleProperties());
      void draw_pie(const Vector& c, const Interval& r, const Interval& theta, const StyleProperties& s = StyleProperties());
      void draw_ellipse(const Vector& c, const Vector& ab, double theta, const StyleProperties& s = StyleProperties());

      // Robots
      void draw_tank(const Vector& x, float size, const StyleProperties& s = StyleProperties());
      void draw_AUV(const Vector& x, float size, const StyleProperties& s = StyleProperties());
      void draw_motor_boat(const Vector& x, float size, const StyleProperties& s = StyleProperties());

    protected:

      double scale_x(double x) const;
      double scale_y(double y) const;
      double scale_length(double y) const;
      void print_header_page();

      std::ofstream _f, _f_temp_content;
      const double _ipe_grid_size = 500.;
      Vector _ratio { 1., 1. };

      std::vector<double> _x_ticks;
      std::vector<double> _y_ticks;
      double _x_offset;
      double _y_offset;

      std::map<std::string,Color> _colors;
  };
}