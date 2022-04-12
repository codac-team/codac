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
   * \param color_map color map used to draw boxes, see SetColorMap
   */
  void SIVIA(const IntervalVector& x, Ctc* ctc, float precision,
    const SetColorMap& color_map = DEFAULT_SET_COLOR_MAP);
  
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
   * \param color_map color map used to draw boxes, see SetColorMap
   */
  void SIVIA(const IntervalVector& x, ibex::Sep* sep, float precision,
    const SetColorMap& color_map = DEFAULT_SET_COLOR_MAP);

  /// @}
}

#endif