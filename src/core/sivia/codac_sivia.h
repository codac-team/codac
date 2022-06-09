/** 
 *  \file
 *  SIVIA
 * ----------------------------------------------------------------------------
 *  \date       2022
 *  \author     Simon Rohou
 *  \copyright  Copyright 2022 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC_SIVIA_H__
#define __CODAC_SIVIA_H__

#include <map>
#include <list>
#include <ibex_Sep.h>
#include "codac_Ctc.h"
#include "codac_VIBesFigPaving.h"
#include "codac_IntervalVector.h"

namespace codac
{
  /// \name SIVIA for contractors
  /// @{

  /**
   * \brief Executes a SIVIA algorithm from a contractor, and displays the result.
   *        SIVIA: Set Inversion Via Interval Analysis.
   * 
   * The result is displayed in the current VIBes figure.
   * 
   * \param x initial box
   * \param ctc Contractor operator for the set inversion
   * \param precision accuracy of the paving algorithm
   * \param display_result display information if true
   * \param fig_name name of the figure on which boxes are drawn. If empty, default figure is used
   * \param return_result if true, boxes will be stored in the returned map
   * \param color_map color map used to draw boxes, see SetColorMap
   * \return return a map of lists of boxes. Keys of the map are IN/OUT/UNKNOWN. The lists are empty if return_result if false.
   */
  std::map<SetValue,std::list<IntervalVector>> SIVIA(const IntervalVector& x, Ctc& ctc, float precision,
    bool display_result = true, const std::string& fig_name = "", bool return_result = false, const SetColorMap& color_map = DEFAULT_SET_COLOR_MAP);
  
  /// @}
  /// \name SIVIA for separators
  /// @{

  /**
   * \brief Executes a SIVIA algorithm from a separator, and displays the result.
   *        SIVIA: Set Inversion Via Interval Analysis.
   * 
   * The result is displayed in the current VIBes figure.
   * 
   * \param x initial box
   * \param sep Separator operator for the set inversion
   * \param precision accuracy of the paving algorithm
   * \param display_result display information if true
   * \param fig_name name of the figure             if None use the current figure. If empty, default figure is used
   * \param return_result if true, boxes will be stored in the returned map
   * \param color_map color map used to draw boxes, see SetColorMap
   * \return return a map of lists of boxes. Keys of the map are IN/OUT/UNKNOWN. The lists are empty if return_result if false.
   */
  std::map<SetValue,std::list<IntervalVector>> SIVIA(const IntervalVector& x, ibex::Sep& sep, float precision,
    bool display_result = true, const std::string& fig_name = "", bool return_result = false, const SetColorMap& color_map = DEFAULT_SET_COLOR_MAP);

  /// @}
}

#endif