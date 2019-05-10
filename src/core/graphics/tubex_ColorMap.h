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
      ColorMap();

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
      rgb color(double ratio) const;

      /**
       * \brief 
       */
      rgb color(double t, const Trajectory& traj) const;

    protected:

      std::map<float,rgb> m_colormap;
  };
}

#endif