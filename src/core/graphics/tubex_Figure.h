/* ============================================================================
 *  tubex-lib - Figure class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#ifndef __TUBEX_FIGURE_H__
#define __TUBEX_FIGURE_H__

#include <string>
#include "ibex_IntervalVector.h"

namespace tubex
{
  class Figure
  {
    public:

      Figure(const std::string& figure_name);
      ~Figure() {};

      const std::string name() const;
      int x() const;
      int y() const;
      int width() const;
      int height() const;

      virtual void set_properties(int x, int y, int width, int height);
 
      static double trunc_inf(double x);
      static const ibex::Interval trunc_inf(const ibex::Interval& x);

    protected:

      static std::string add_suffix(const std::string& name, int id);

    protected:

      const std::string m_name;
      int m_x = 100, m_y = 100, m_width = 600, m_height = 300;
  };
}

#endif