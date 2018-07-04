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

#ifndef Fnc_HEADER
#define Fnc_HEADER

#include "tubex_TubeVector.h"

namespace tubex
{
  class TubeVector;
  
  class Fnc
  {
    public:

      Fnc();
      Fnc(int n, int m);
      ~Fnc();

      int nbVar() const;
      int imageDim() const;

      virtual TubeVector eval_vector(const TubeVector& x) const = 0;

    protected:

      int m_nb_vars = -1, m_img_dim = -1;
  };
}

#endif