/* ============================================================================
 *  tubex-lib - VIBesFigure_Paving class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#ifndef __TUBEX_VIBESFIGUREPAVING_H__
#define __TUBEX_VIBESFIGUREPAVING_H__

#include "tubex_VIBesFigure.h"
#include "tubex_Paving.h"

namespace tubex
{
  class VIBesFigure_Paving : public VIBesFigure
  {
    public:

      VIBesFigure_Paving(const std::string& fig_name, const Paving *paving);

      void show();
      void set_color_map(const std::map<int,std::string> color_map);

    protected:

      void draw_paving(const Paving *paving);

    protected:

      const Paving *m_paving;
      std::map<int,std::string> m_color_map;
  };
}

#endif