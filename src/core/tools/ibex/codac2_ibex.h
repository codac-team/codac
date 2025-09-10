/** 
 *  \file codac2_ibex.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#ifdef _IBEX_INTERVAL_H_ // only available if IBEX has been included beforehand

#include <ibex_Interval.h>
#include <ibex_Vector.h>
#include <ibex_Matrix.h>
#include <ibex_IntervalVector.h>
#include <ibex_IntervalMatrix.h>
#include "codac2_Interval.h"
#include "codac2_Vector.h"
#include "codac2_IntervalVector.h"
#include "codac2_Matrix.h"
#include "codac2_IntervalMatrix.h"

namespace codac2
{
  /**
   * \brief Casts a Codac Interval object into an IBEX Interval object
   * 
   * \param x const Codac type Interval
   * \return const IBEX type Interval
   */
  ibex::Interval to_ibex(const codac2::Interval& x);

  /**
   * \brief Casts an IBEX Interval object into a Codac Interval object
   * 
   * \param x const IBEX type Interval
   * \return Codac type Interval
   */
  codac2::Interval to_codac(const ibex::Interval& x);

  /**
   * \brief Casts a Codac Vector object into an IBEX Vector object
   * 
   * \param x const Codac type Vector
   * \return IBEX type Vector
   */
  ibex::Vector to_ibex(const codac2::Vector& x);

  /**
   * \brief Casts an IBEX Vector object into a Codac Vector object
   * 
   * \param x const IBEX type Vector
   * \return Codac type Vector
   */
  codac2::Vector to_codac(const ibex::Vector& x);

  /**
   * \brief Casts a Codac IntervalVector object into an IBEX IntervalVector object
   * 
   * \param x const Codac type IntervalVector
   * \return IBEX type IntervalVector
   */
  ibex::IntervalVector to_ibex(const codac2::IntervalVector& x);

  /**
   * \brief Casts an IBEX IntervalVector object into a Codac IntervalVector object
   * 
   * \param x const IBEX type IntervalVector
   * \return Codac type IntervalVector
   */
  codac2::IntervalVector to_codac(const ibex::IntervalVector& x);

  /**
   * \brief Casts a Codac Matrix object into an IBEX Matrix object
   * 
   * \param x const Codac type Matrix
   * \return IBEX type Matrix
   */
  ibex::Matrix to_ibex(const codac2::Matrix& x);

  /**
   * \brief Casts an IBEX Matrix object into a Codac Matrix object
   * 
   * \param x const IBEX type Matrix
   * \return Codac type Matrix
   */
  codac2::Matrix to_codac(const ibex::Matrix& x);

  /**
   * \brief Casts a Codac IntervalMatrix object into an IBEX IntervalMatrix object
   * 
   * \param x const Codac type IntervalMatrix
   * \return IBEX type IntervalMatrix
   */
  ibex::IntervalMatrix to_ibex(const codac2::IntervalMatrix& x);

  /**
   * \brief Casts an IBEX IntervalMatrix object into a Codac IntervalMatrix object
   * 
   * \param x const IBEX type IntervalMatrix
   * \return Codac type IntervalMatrix
   */
  codac2::IntervalMatrix to_codac(const ibex::IntervalMatrix& x);
}

#include "codac2_ibex_impl.h"

#endif