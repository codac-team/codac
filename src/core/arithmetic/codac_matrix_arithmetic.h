/** 
 *  \file
 *  Arithmetic operations on matrices
 * ----------------------------------------------------------------------------
 *  \date       2022
 *  \author     Simon Rohou
 *  \copyright  Copyright 2022 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC_MATRIX_ARITHMETIC_H__
#define __CODAC_MATRIX_ARITHMETIC_H__

#include "codac_IntervalMatrix.h"

namespace codac
{
  /** \brief \f$[\mathbf{X}]\sqcup[\mathbf{Y}]\f$
    * \param x
    * \param y
    * \return IntervalMatrix output
    */
  const IntervalMatrix operator|(const IntervalMatrix& x, const IntervalMatrix& y);

  /** \brief \f$[\mathbf{X}]\cap[\mathbf{Y}]\f$
    * \param x
    * \param y
    * \return IntervalMatrix output
    */
  const IntervalMatrix operator&(const IntervalMatrix& x, const IntervalMatrix& y);
}

#endif