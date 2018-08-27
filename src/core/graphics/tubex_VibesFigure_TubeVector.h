/* ============================================================================
 *  tubex-lib - VibesFigure_TubeVector class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#ifndef __TUBEX_VIBESFIGURETUBEVECTOR_H__
#define __TUBEX_VIBESFIGURETUBEVECTOR_H__

#include "tubex_TubeVector.h"
#include "tubex_TrajectoryVector.h"
#include "tubex_VibesFigure_Tube.h"

namespace tubex
{
  class VibesFigure_TubeVector
  {
    public:

      VibesFigure_TubeVector(const std::string& fig_name);
      VibesFigure_TubeVector(const std::string& fig_name, int start_index, int end_index);
      VibesFigure_TubeVector(const std::string& fig_name, const TubeVector *tubevector, const TrajectoryVector *trajvector = NULL);
      ~VibesFigure_TubeVector();
      int size() const;
      int subfigs_number() const;
      
      void set_properties(int x, int y, int width, int height);

      void add_tubevector(const TubeVector *tubevector, const std::string& name, const std::string& color_frgrnd = DEFAULT_FRGRND_COLOR, const std::string& color_bckgrnd = DEFAULT_BCKGRND_COLOR);
      void set_tubevector_name(const TubeVector *tubevector, const std::string& name);
      void set_tubevector_derivative(const TubeVector *tubevector, const TubeVector *derivative);
      void set_tubevector_color(const TubeVector *tubevector, const std::string& color_frgrnd, const std::string& color_bckgrnd);
      void set_tubevector_color(const TubeVector *tubevector, int color_type, const std::string& color);
      void remove_tubevector(const TubeVector *tubevector);

      void add_trajectoryvector(const TrajectoryVector *trajvector, const std::string& name, const std::string& color = DEFAULT_TRAJ_COLOR);
      void set_trajectoryvector_name(const TrajectoryVector *trajvector, const std::string& name);
      void set_trajectoryvector_color(const TrajectoryVector *trajvector, const std::string& color);
      void remove_trajectoryvector(const TrajectoryVector *trajvector);

      void show(bool detail_slices = false);

      void draw_box(const ibex::IntervalVector& box, const vibes::Params& params);
      void draw_box(const ibex::IntervalVector& box, const std::string& color = "", const vibes::Params& params = vibes::Params());

      // Static methods (shortcuts for fast and simple use)
      static void draw(const std::string& fig_name, int x = 0, int y = 0);
      static void draw(const std::string& fig_name, const TubeVector *tubevector, int x = 0, int y = 0);
      static void draw(const std::string& fig_name, const TrajectoryVector *trajvector, int x = 0, int y = 0);
      static void draw(const std::string& fig_name, const TubeVector *tubevector, const TrajectoryVector *trajvector, int x = 0, int y = 0);
      static void end_drawing();

    protected:

      void create_subfigures(int size);

    protected:

      int m_n = 0;
      VibesFigure_Tube **m_v_figs = NULL;
      int m_start_index = -1, m_end_index = -1;

      // todo: move the following attributes in some abstract class? (common with VibesFigure)
      const std::string m_name;
      int m_x = 100, m_y = 100, m_width = 600, m_height = 300;
  };
}

#endif