/** 
 *  \file
 *  VIBesFigPaving class
 * ----------------------------------------------------------------------------
 *  \date       2016
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC_VIBESFIGPAVING_H__
#define __CODAC_VIBESFIGPAVING_H__

#include <map>
#include <string>
#include "codac_VIBesFig.h"
#include "codac_Paving.h"

namespace codac
{
  typedef std::map<SetValue,std::string> SetColorMap;
  extern SetColorMap DEFAULT_SET_COLOR_MAP; //!< predefined DEFAULT_SET_COLOR_MAP

  /**
   * \class VIBesFigPaving
   * \brief Two-dimensional graphical item to display a Paving object
   *
   * One figure is linked to one paving, so that any update on this object
   * can be easily displayed on the figure.
   */
  class VIBesFigPaving : public VIBesFig
  {
    public:

      /**
       * \brief Creates a VIBesFigPaving
       *
       * \param fig_name a reference to the figure that will be displayed in the window's title
       * \param paving a const pointer to the paving to be displayed
       */
      VIBesFigPaving(const std::string& fig_name, const Paving *paving);

      /**
       * \brief Sets a custom color map
       *
       * \param color_map the color map `<paving_value,color_code>` to be set
       */
      void set_color_map(const SetColorMap& color_map);

      /**
       * \brief Updates the display of the Paving object
       */
      void show();

    protected:
      
      /**
       * \brief Draws a paving object
       *
       * This method is recursive, because a Paving is implemented as a binary tree.
       *
       * \param paving a const pointer to the Paving object to be displayed
       */
      void draw_paving(const Paving *paving);

    protected:

      const Paving *m_paving; //!< const pointer to the object to be displayed
      std::map<SetValue,std::string> m_color_map; //!< custom color map
  };
}

#endif