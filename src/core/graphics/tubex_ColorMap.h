/** 
 *  \file
 *  ColorMap class
 * ----------------------------------------------------------------------------
 *  \date       2019
 *  \author     Simon Rohou
 *  \copyright  Copyright 2019 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_COLORMAP_H__
#define __TUBEX_COLORMAP_H__

#include "tubex_colors.h"
#include "tubex_Trajectory.h"

namespace tubex
{
  /**
   * \enum TubeColorType
   * \brief Defines a set of colors for tube display
   */
  enum InterpolationMode { RGB, HSV };

  /**
   * \class ColorMap
   * \brief ...
   */
  class ColorMap
  {
    public:

      /**
       * \brief 
       *
       * \param 
       */
      ColorMap(int interpol_mode = InterpolationMode::RGB);

      /**
       * \brief ColorMap destructor
       */
      ~ColorMap() {};

      /**
       * \brief 
       */
      void add_color_point(rgb color, float index);

      /**
       * \brief 
       */
      void add_color_point(hsv color, float index);

      /**
       * \brief 
       */
      rgb color(double ratio) const;

      /**
       * \brief 
       */
      rgb color(double t, const Trajectory& traj) const;

      /**
       * \brief 
       */
      void displayColorMap() const;

    protected:

      const int m_interpol_mode = InterpolationMode::RGB;
      std::map<float,rgb> m_colormap;
  };
}

#endif