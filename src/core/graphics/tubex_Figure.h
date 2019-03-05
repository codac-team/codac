/** 
 *  @file
 *  Figure class
 * ================================================================
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
  /**
   * \class Figure
   * \brief Two-dimensional graphical item
   */

  class Figure
  {
    public:

      /**
       * \brief Creates a Figure
       *
       * \param figure_name name of the figure as displayed in the window title
       */
      Figure(const std::string& figure_name);

      /**
       * \brief Figure destructor
       */
      ~Figure() {};

      /**
       * \brief Returns the horizontal position of this figure
       *
       * \return x-position in pixels coordinates
       */
      int x() const;

      /**
       * \brief Returns the vertical position of this figure
       *
       * \return y-position in pixels coordinates
       */
      int y() const;

      /**
       * \brief Returns the width of this figure
       *
       * \return the width in pixels
       */
      int width() const;

      /**
       * \brief Returns the height of this figure
       *
       * \return the height in pixels
       */
      int height() const;

      /**
       * \brief Returns the name of this figure
       *
       * \return the name
       */
      const std::string name() const;

      /**
       * \brief Returns the real values displayed by this figure
       *
       * \return the view box
       */
      const ibex::IntervalVector& view_box() const;
      
      /**
       * \brief Sets the properties (position and dimensions) of this figure
       *
       * \param x horizontal position (in pixels)
       * \param y vertical position (in pixels)
       * \param width width value (in pixels)
       * \param height height value (in pixels)
       */
      void set_properties(int x, int y, int width, int height);

    protected:
      
      /**
       * \brief Returns a representable value of any double variable `x`
       *
       * A double variable may be defined as `POS_INFINITY` or `NEG_INFINITY`, which is
       * not displayable. The function returns `BOUNDED_INFINITY` values in these cases.
       *
       * \param x the value to be truncated if necessary
       * \return truncated value
       */
      static double trunc_inf(double x);
      
      /**
       * \brief Returns a representable value of any interval \f$[x]\f$
       *
       * A double bound may be defined as `POS_INFINITY` or `NEG_INFINITY`, which is
       * not displayable. The function returns `BOUNDED_INFINITY` values in these cases.
       *
       * \param x the interval for which the bounds will be truncated if necessary
       * \return truncated value
       */
      static const ibex::Interval trunc_inf(const ibex::Interval& x);
      
      /**
       * \brief Adds an integer suffix to a string
       *
       * \param name the string to be updated
       * \param id the integer to add
       * \return the string value `string`_`id`
       */
      static std::string add_suffix(const std::string& name, int id);

    protected:

      const std::string m_name; //!< figure name
      int m_x = 100, m_y = 100, m_width = 600, m_height = 300; //!< figure properties
      ibex::IntervalVector m_view_box = ibex::IntervalVector(2, ibex::Interval::EMPTY_SET); //!< view box
  };
}

#endif