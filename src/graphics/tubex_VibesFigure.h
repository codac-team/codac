/* ============================================================================
 *  tubex-lib - VibesFigure class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#ifndef __TUBEX_VIBESFIGURE_H__
#define __TUBEX_VIBESFIGURE_H__

#include <string>
#include "vibes.h"
#include "ibex_IntervalVector.h"

namespace tubex
{
  class VibesFigure
  {
    public:

      /**
       * \brief Create an object view: abstract class.
       *
       * \param figure_name a reference to the figure that will be displayed in the window's title
       */
      VibesFigure(const std::string& figure_name, int nb_layers = 1);

      /**
       * \brief Delete this figure.
       */
      ~VibesFigure() {};

      /**
       * \brief Return the name of the figure.
       *
       * \return figure's name
       */
      const std::string name() const;

      /**
       * \brief Return x-position of the figure.
       *
       * \return x-position
       */
      int x() const;

      /**
       * \brief Return y-position of the figure.
       *
       * \return y-position
       */
      int y() const;

      /**
       * \brief Return figure's width.
       *
       * \return width
       */
      int width() const;

      /**
       * \brief Return figure's height.
       *
       * \return height
       */
      int height() const;

      /**
       * \brief Return figure's view box.
       *
       * \return IntervalVector
       */
      const ibex::IntervalVector& viewBox() const;

      /**
       * \brief Set figure's properties: position and dimensions.
       *
       * \param x x-position
       * \param y y-position
       * \param width width value (px)
       * \param height height value (px)
       */
      void setProperties(int x, int y, int width, int height);

      /**
       * \brief Set figure's axis limits.
       *
       * \param x_min 
       * \param x_max 
       * \param y_min 
       * \param y_max 
       * \param keep_ratio view box is set to fit with the params, without changing its ratio (false by default)
       * \return the limits box
       */
      const ibex::IntervalVector& axisLimits(double x_min, double x_max, double y_min, double y_max, bool keep_ratio = false);
      const ibex::IntervalVector& axisLimits(const ibex::IntervalVector& viewbox, bool keep_ratio = false);

      /**
       * \brief Save vibes-figure in SVG/PNG/... format.
       *
       * A file named {figure_name}{suffix}.svg is created in the current directory.
       *
       * \param suffix optional part name that can be added to figure_name, empty by default
       * \param extension optional part to specify image type, ".svg" by default
       */
      void saveImage(const std::string& suffix = "", const std::string& extension = ".svg") const;

      /**
       * \brief Display the figure.
       *
       * This is a virtual method to overload.
       */
      virtual void show() = 0;

      /**
       * \brief Convert RGB grayscales values to hexa notation.
       *
       * \param r red intensity in [0,255]
       * \param g green intensity in [0,255]
       * \param b blue intensity in [0,255]
       * \param alpha transparency in [0,255] (no transparency by default)
       * \param prefix character to prefix the hexa value ('#' by default)
       * \return string containing hexa value
       */
      static const std::string rgb2hex(int r, int g, int b, int alpha = -1, const char* prefix = "#");

      /**
       * \brief Convert hexa notation to RGB values.
       *
       * \param string containing hexa value
       * \param r red intensity in [0,255]
       * \param g green intensity in [0,255]
       * \param b blue intensity in [0,255]
       * \param alpha transparency in [0,255]
       * \return 
       */
      //static void hex2rgb(const std::string& hexa, int &r, int &g, int &b, int &alpha);

      static double truncInf(double x);
      static const ibex::Interval truncInf(const ibex::Interval& x);

    protected:

      void setCurrentLayer(int layer);
      const std::string& figName();
      static std::string addSuffix(const std::string& name, int id);


    protected:

      mutable std::string m_name;
      int m_x, m_y, m_width, m_height;
      ibex::IntervalVector m_view_box;
      int m_nb_layers = 1, m_current_layer = 0;
  };
}

#endif