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
     * \param true_values if true values are available, a display is possible
     */
    VibesFigure_Tube(const std::string& figure_name, Tube *tube, std::map<double,double> *true_values = NULL);

    /**
     * \brief Delete this figure.
     */
    ~VibesFigure_Tube();

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
    mutable Tube *m_tube_copy;
    std::map<double,double> *m_true_values;
};

#endif