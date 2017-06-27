/* ============================================================================
 *  tubex-lib - VibesFigure class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU General Public License (GPL). See the file LICENSE.
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#ifndef VIBESFIGURE_HEADER
#define VIBESFIGURE_HEADER

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
     * \brief Return x-position of the figure.
     *
     * \return x-position
     */
    double x() const;

    /**
     * \brief Return y-position of the figure.
     *
     * \return y-position
     */
    double y() const;

    /**
     * \brief Return figure's width.
     *
     * \return width
     */
    double width() const;

    /**
     * \brief Return figure's height.
     *
     * \return height
     */
    double height() const;

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
    virtual void show() const = 0;

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
    static std::string rgb2hex(int r, int g, int b, int alpha = -1, const char* prefix = "#");

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


  protected:

    mutable std::string m_name;
    double m_x, m_y, m_width, m_height;
};

#endif
