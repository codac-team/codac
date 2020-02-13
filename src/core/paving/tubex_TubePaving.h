/** 
 *  \file
 *  TubePaving class
 * ----------------------------------------------------------------------------
 *  \date       2019
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_TUBEPAVING_H__
#define __TUBEX_TUBEPAVING_H__

#include "tubex_Paving.h"
#include "tubex_TubeVector.h"

namespace tubex
{
  /**
   * \class TubePaving
   * \brief Multi-dimensional paving as projection of a vector tube
   *
   * The temporal information is lost.
   */
  class TubePaving : public Paving
  {
    public:

      /// \name Basics
      /// @{

      /**
       * \brief Initializes a tube paving
       *
       * \param init_box n-dimensional box defining the hull of the paving
       */
      TubePaving(const ibex::IntervalVector& init_box);

      /**
       * \brief Computes the paving from the tube \f$[\mathbf{x}](\cdot)\f$.
       *
       * \param precision precision \f$\epsilon\f$ of the SIVIA approximation
       * \param x TubeVector \f$[\mathbf{x}](\cdot)\f$
       */
      void compute(float precision, const TubeVector& x);

      /// @}
  };
}

#endif