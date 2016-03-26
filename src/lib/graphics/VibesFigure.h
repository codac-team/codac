/* ============================================================================
 *  IBEX-Robotics - VibesFigure class
 * ============================================================================
 *  Copyright : Simon Rohou
 *  License   : This program can be distributed under the terms of
 *              the MIT license. See the file LICENSE.
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#ifndef VibesFigure_HEADER
#define VibesFigure_HEADER

#include "ibex.h"
#include "vibes.h"
#include <string>

class VibesFigure
{
  public:

    /**
     * \brief Create an object view: abstract class.
     *
     * \param figure_name a reference to the figure that will be displayed in the window's title
     */
    VibesFigure(const std::string& figure_name);

    /**
     * \brief Delete this figure.
     */
    ~VibesFigure() {};

    /**
     * \brief Return the name of the figure.
     *
     * \return figure's name
     */
    std::string name() const;

    /**
     * \brief Set figure's properties: position and dimensions.
     *
     * \param x x-position
     * \param y y-position
     * \param width width value
     * \param height height value
     */
    void setProperties(int x, int y, int width, int height);

    /**
     * \brief Save vibes-figure in SVG format.
     *
     * A file named {figure_name}.svg is created in the current directory.
     */
    void saveSVG() const;

    /**
     * \brief Display the figure.
     *
     * This is a virtual method to overload.
     */
    virtual void show() const = 0;

  protected:
    std::string m_name;
};

#endif
