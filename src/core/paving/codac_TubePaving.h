/** 
 *  \file
 *  TubePaving class
 * ----------------------------------------------------------------------------
 *  \date       2019
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC_TUBEPAVING_H__
#define __CODAC_TUBEPAVING_H__

#include "codac_Paving.h"
#include "codac_TubeVector.h"

namespace codac
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
      TubePaving(const ibex::IntervalVector& init_box, SetValue value = SetValue::UNKNOWN);

      /**
       * \brief Computes the paving from the tube \f$[\mathbf{x}](\cdot)\f$.
       *
       * \param precision precision \f$\epsilon\f$ of the SIVIA approximation
       * \param x TubeVector \f$[\mathbf{x}](\cdot)\f$
       */
      void compute(float precision, const TubeVector& x);

      /**
       * \brief Computes the paving from the tube \f$[x](\cdot)\f$.
       *
       * \param precision precision \f$\epsilon\f$ of the SIVIA approximation
       * \param x Tube \f$[x](\cdot)\f$
       */
      void compute(float precision, const Tube& x);





      /// @}
  };
}

#endif