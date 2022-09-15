/** 
 *  \file
 *  ColorMap class
 * ----------------------------------------------------------------------------
 *  \date       2019
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC_COLORMAP_H__
#define __CODAC_COLORMAP_H__

#include "codac_colors.h"
#include "codac_Trajectory.h"

namespace codac
{
  /**
   * \enum InterpolMode
   * \brief Defines in which color space (HSV, RGB) the
   *        color interpolation will be made.
   */
  enum class InterpolMode { RGB, HSV };

  /**
   * \class ColorMap
   * \brief Associates colors to a range of values.
   */
  class ColorMap
  {
    public:

      /**
       * \brief Creates a color map
       *
       * \param interpol_mode mode defining in which color space the
       *        color interpolation will be made (RGB by default)
       */
      ColorMap(InterpolMode interpol_mode = InterpolMode::RGB);

      /**
       * \brief ColorMap destructor
       */
      ~ColorMap() {};

      /**
       * \brief Returns a copy of a ColorMap
       *
       * \param x the ColorMap object to be copied
       * \return a new ColorMap object with same values
       */
      const ColorMap& operator=(const ColorMap& x);

      /**
       * \brief Adds a RGB color point to the map
       *
       * \param color RGB color to be added
       * \param index key associated to the color (any float value)
       */
      void add_color_point(rgb color, float index);

      /**
       * \brief Adds a HSV color point to the map
       *
       * \param color HSV color to be added
       * \param index key associated to the color (any float value)
       */
      void add_color_point(hsv color, float index);

      /**
       * \brief Reverses the keys of the color map
       */
      void reverse();

      /**
       * \brief Sets a constant opacity to all colors of the map
       *
       * \param alpha opacity value, between 0. (transparent) and 1. (opaque)
       */
      void set_opacity(float alpha);

      /**
       * \brief Returns `true` of all colors are opaque
       *
       * \return opacity test of the map
       */
      bool is_opaque() const;

      /**
       * \brief Evaluates the color map at \f$r\f$, \f$r\in[0,1]\f$
       *
       * If `r` is not a key of the map, an interpolation is made to compute
       * the corresponding color, according to the selected mode.
       *
       * \param r input, between 0 and 1
       * \return an RGB color
       */
      rgb color(double r) const;

      /**
       * \brief Evaluates the color map at \f$f(t)\f$, \f$t\in[t_0,t_f]\f$
       *
       * If \f$f(t)\f$ is not a key of the map, an interpolation is made to compute
       * the corresponding color, according to the selected mode.
       *
       * \param t input, between 0 and 1
       * \param f the trajectory function
       * \return an RGB color
       */
      rgb color(double t, const Trajectory& f) const;

      /**
       * \brief Creates a VIBesFig with the colors of the map
       *
       * \param fig_name a reference to the figure that will be displayed in the window's title
       */
      void displayColorMap(const std::string& fig_name = "Color map") const;

      static const ColorMap HAXBY; //!< predefined HAXBY color map (mainly used for DEM)
      static const ColorMap DEFAULT; //!< a predefined default color map
      static const ColorMap BLUE_TUBE; //!< a predefined color map for tubes
      static const ColorMap RED_TUBE; //!< a predefined color map for tubes
      static const ColorMap RAINBOW; //!< a predefined color map


    protected:

      InterpolMode m_interpol_mode = InterpolMode::RGB; //!< interpolation mode
      std::map<float,rgb> m_colormap; //!< map of colors
  };
}

#endif