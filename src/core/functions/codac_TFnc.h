/** 
 *  TFnc class
 * ----------------------------------------------------------------------------
 *  \date       2018
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC_TFNC_H__
#define __CODAC_TFNC_H__

#include "codac_Interval.h"
#include "codac_IntervalVector.h"

namespace codac
{
  class Tube;
  class TubeVector;
  
  class TFnc
  {
    public:

      TFnc(int nb_vars, int img_dim, bool is_intertemporal);
      virtual ~TFnc();
      const TFnc& operator=(const TFnc& f);

      int nb_var() const;
      int nb_vars() const;
      int image_dim() const;
      bool is_intertemporal() const;

      virtual const Tube eval(const TubeVector& x) const;
      virtual const Interval eval(const IntervalVector& x) const = 0;
      virtual const Interval eval(int slice_id, const TubeVector& x) const = 0;
      virtual const Interval eval(const Interval& t, const TubeVector& x) const = 0;
      
      virtual const TubeVector eval_vector(const TubeVector& x) const;
      virtual const IntervalVector eval_vector(const IntervalVector& x) const = 0;
      virtual const IntervalVector eval_vector(int slice_id, const TubeVector& x) const = 0;
      virtual const IntervalVector eval_vector(const Interval& t, const TubeVector& x) const = 0;

    protected:
      
      TFnc();

      int m_nb_vars, m_img_dim;
      bool m_intertemporal;
  };
}

#endif