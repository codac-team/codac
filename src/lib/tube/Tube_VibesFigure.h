/* ============================================================================
 *  IBEX-Robotics - Tube_VibesFigure class
 * ============================================================================
 *  Copyright : Simon Rohou
 *  License   : This program can be distributed under the terms of
 *              the MIT license. See the file LICENSE.
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#ifndef VibesFigureTube_HEADER
#define VibesFigureTube_HEADER

#include "VibesFigure.h"
#include "Tube.h"

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

    /**
     * \brief Delete this figure.
     */
    ~VibesFigure_Tube();

    /**
     * \brief Set a custom color for tube's slices.
     *
     * \param slices_color the new color
     */
    void setColor(std::string slices_color);

    /**
     * \brief Display the tube.
     *
     * This method shows all slices.
     */
    virtual void show() const;

    /**
     * \brief Display the tube.
     *
     * This method can be used for heavy tubes when it is costly to display all slices.
     * This will leave blanks between slices.
     *
     * \param slices_limit the max number of slices to display
     */
    void show(int slices_limit) const;

    /**
     * \brief Display scalar values on top of the tube.
     *
     * \param map_scalar_values a map of the form [t](y) representing a trajectory
     * \param color line's color
     */
    void showScalarValues(const std::map<double,double>& map_scalar_values, const std::string& color = "red") const;

  protected:

    /**
     * \brief Display one slice of the tube.
     *
     * \param intv_t slice's width
     * \param intv_y slice's height
     * \param params slice's vibes group
     */
    void drawSlice(const ibex::Interval& intv_t, const ibex::Interval& intv_y, const vibes::Params& params) const;

  protected:
    Tube *m_tube;
    std::vector<std::map<double,double> > *m_true_values;
    mutable Tube *m_tube_copy;
    mutable int m_id_map_scalar_values;
};

#endif