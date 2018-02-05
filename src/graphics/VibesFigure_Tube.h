/* ============================================================================
 *  tubex-lib - VibesFigure_Tube class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#ifndef VIBESFIGURETUBE_HEADER
#define VIBESFIGURETUBE_HEADER

#include "ibex_Interval.h"
#include "ibex_IntervalVector.h"
#include "VibesFigure.h"
#include "Tube.h"    

namespace tubex
{
  class VibesFigure_Tube : public VibesFigure
  {
    public:

      /**
       * \brief Create a Tube view.
       *
       * \param figure_name a reference to the figure that will be displayed in the window's title
       * \param tube a pointer to the tube to be displayed
       */
      VibesFigure_Tube(const std::string& figure_name, Tube *tube);
      VibesFigure_Tube(const std::string& figure_name, std::vector<Tube*> v_tubes);

      /**
       * \brief Delete this figure.
       */
      ~VibesFigure_Tube();

      /**
       * \brief Set custom colors
       *
       * \param slices_color new color of slices
       * \param background_color new color of slices before contraction
       * \param truth_color new color of represented trajectories
       */
      void setColors(std::string slices_color, std::string slices_contracted_color = "", std::string background_color = "lightGray[lightGray]", std::string truth_color = "red");

      /**
       * \brief Display the tube.
       *
       * This method shows all slices.
       */
      virtual void show();

      /**
       * \brief Display the tube.
       *
       * This method can be used for heavy tubes when it is costly to display all slices.
       * This will leave blanks between slices.
       *
       * \param detail_slices if true, each slice will be shown as a box, polygon envelope otherwise
       * \param slices_limit the max number of slices to display
       * \param update_background if true, only last contraction is displayed (true by default)
       */
      void show(bool detail_slices, int slices_limit, bool update_background = true);

      /**
       * \brief Display scalar values over the tube.
       *
       * \param map_scalar_values a map of the form [t](y) representing a trajectory
       * \param color line's color
       * \param points_size size (in map value) of the points given by map_scalar_values. If 0, a line is drawn, only points otherwise.
       */
      void showScalarValues(const std::map<double,double>& map_scalar_values, const std::string& color = "#00609C", double points_size = 0.) const;

      /**
       * \brief A fast function to display a tube
       *
       * Instantiate dynamically a new VibesFigure_Tube into a map if the
       * corresponding key does not exist yet.
       *
       * Automatic call to vibes::beginDrawing().
       *
       * \param map_graphics a map referencing figure pointers by a tube pointer key
       * \param tube a pointer to the tube to be displayed
       * \param figure_name a reference to the figure that will be displayed in the window's title
       * \param map_scalar_values a map of the form [t](y) representing a trajectory
       * \param x x-position (0 by default)
       * \param y y-position (0 by default)
       */
      static void show(Tube *tube, const std::string& name = "", int x = 0, int y = 0);
      static void show(Tube *tube, const std::string& name, const std::map<double,double>& map_scalar_values, int x = 0, int y = 0);
      static void show(const std::vector<Tube*> v_tubes, const std::string& name, int x, int y);
      static void show(const std::vector<Tube*> v_tubes, const std::string& name, const std::map<double,double>& map_scalar_values, int x, int y);
      
      /**
       * \brief A fast function to end tubes displays
       */
      static void endDrawing();


    protected:

      /**
       * \brief Display one slice of the tube.
       *
       * \param intv_t slice's width
       * \param intv_y slice's height
       * \param params slice's vibes group
       */
      void drawSlice(const ibex::Interval& intv_t, const ibex::Interval& intv_y, const vibes::Params& params) const;

      /**
       * \brief Compute the polygon as the envelope of the tube.
       *
       * This is an optimization instead of displaying each slice: a unique figure is shown.
       *
       * \param tube the tube to be shown as a polygon
       * \param v_x a vector of double storing all x-point
       * \param v_x a vector of double storing all y-point
       */
      void computePolygonEnvelope(const Tube& tube, std::vector<double>& v_x, std::vector<double>& v_y) const;


    protected:

      std::vector<Tube*> m_v_tubes;
      std::vector<std::map<double,double> > *m_true_values;
      mutable std::vector<Tube*> m_v_tubes_copy;
      mutable int m_id_map_scalar_values;
      mutable bool m_need_to_update_axis;
      mutable ibex::IntervalVector m_tubes_box;

      static std::vector<VibesFigure_Tube*> v_graphics;
  };
}

#endif