/** 
 *  \file codac2_OutputFigure2D.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <string>
#include <vector>
#include "codac2_Vector.h"
#include "codac2_IntervalVector.h"
#include "codac2_Figure2DInterface.h"

namespace codac2
{
  class Figure2D;

  /**
   * \class OutputFigure2D
   * \brief Output figure class, used to manage the output figures
   * 
   * This class is used to manage the output figures. It is purely virtual and should not be used directly
   * 
   * It inherits from the Figure2DInterface class and is currently used to manage the output figures for VIBes and IPE
   */
  class OutputFigure2D : public Figure2DInterface
  {
    public:

      /**
       * \brief Creates a new OutputFigure2D object linked to a given figure
       * 
       * \param fig Figure2D to use
       */
      OutputFigure2D(const Figure2D& fig)
        : _fig(fig)
      { }

      /**
       * \brief Getter for the horizontal Index
       * 
       * \return The horizontal Index
       */
      const Index& i() const;

      /**
       * \brief Getter for the vertical Index
       * 
       * \return The vertical Index
       */
      const Index& j() const;
      
      /**
       * \brief Updates the axes of the figure
        */
      virtual void update_axes() = 0;
      /**
       * \brief Updates the position and size of the window
       */
      virtual void update_window_properties() = 0;
      /**
       * \brief Centers the viewbox
       */
      virtual void center_viewbox(const Vector& c, const Vector& r) = 0;

    protected:

      const Figure2D& _fig;
      std::vector<std::string> _layers;
  };
}