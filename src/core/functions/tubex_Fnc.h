/* ============================================================================
 *  tubex-lib - Fnc class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#ifndef __TUBEX_FNC_H__
#define __TUBEX_FNC_H__

#include "ibex_IntervalVector.h"

namespace tubex
{
  class TubeVector;
  
  class Fnc
  {
    public:

      Fnc(int n, int m);
      ~Fnc();
      const Fnc& operator=(const Fnc& f);

      int nb_vars() const;
      int image_dim() const;

      const TubeVector eval(const TubeVector& x) const;
      virtual const ibex::IntervalVector eval(const ibex::Interval& t, const TubeVector& x) const = 0;

    protected:
      
      Fnc();

      int m_nb_vars = -1, m_img_dim = -1;
  };
}

#endif