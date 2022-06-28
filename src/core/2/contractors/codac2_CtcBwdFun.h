/** 
 *  \file  contraction on a gate from a "mesure"
 *  CtcBwdFun class
 * ----------------------------------------------------------------------------
 *  \date       2022
 *  \author     Damien Massé
 *  \copyright  Copyright 2022 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC2_CTCBWDFUN_H__
#define __CODAC2_CTCBWDFUN_H__

#include <vector>
#include "codac_Function.h"
#include "codac_DynCtc.h"
#include "codac2_SliceVector.h"
#include "codac2_IParals.h"
#include "codac2_expIMat.h"

namespace codac2
{
  using codac::Function;

  /**
   * \class CtcBwdFun
   * \brief Contraction on a measure on IParals
   *         from fun : R^n times R^m -> R^p
   *               we allow several simultaneous contractions
   */
  class CtcBwdFun
  {
    public:

      CtcBwdFun(const Function& t);


      /** evaluate function on box
       */
      const IntervalVector eval_function(const IntervalVector& cdom,
			 const IntervalVector* u) const;
      const IntervalVector eval_function(const IntervalVector& cdom,
	 const std::vector<std::pair<const IntervalVector*, 
				     const IntervalVector>>& rhs) const;
      const IntervalVector eval_function(const IParals& cdom, 
			 const IntervalVector* u) const;
      const IntervalVector eval_function(const IParals& cdom,
	 const std::vector<std::pair<const IntervalVector*, 
				     const IntervalVector>>& rhs) const;

      /** compute the jacobian : return the Jacobian of a convex
       *   superset of codom */
      IntervalMatrix jacobian(const IParals& codom,
                 const IntervalVector* u) const;

      /** commute a «~global~» Jacobian */
      IntervalMatrix jacobian(const IParals& codom,
	 const std::vector<std::pair<const IntervalVector*, 
				     const IntervalVector>>& rhs) const;

      /** apply Newton approach to approximate a potential solution;
       *  the return must be inside the convex superset used to compute
       *  the jacobian */
      Vector newton_Approx(const IParals& codom, const Matrix& mjac,
	 const std::vector<std::pair<const IntervalVector*, 
				     const IntervalVector>>& rhs) const;
      Vector newton_Approx(const IParals& codom, const Matrix& mjac,
	 const IntervalVector* u, const IntervalVector& rs) const;

      /** contraction in IParals */
      void contract(IParals& val,
		const IntervalVector* u, const IntervalVector& rs) const;
      void contract(IParals& val,
		const std::vector<std::pair<const IntervalVector*,
                                     const IntervalVector>>& rhs) const;

      /** contraction on a SliceVector (assumed to be gate) */
      void contract(SliceVector& x,
		const IntervalVector* u, const IntervalVector& rs) const;
      void contract(SliceVector& x, 
		const std::vector<std::pair<const IntervalVector*,
                                     const IntervalVector>>& rhs) const;

      const Function& f() const;

    protected:

      friend class SliceVector; // to be removed
      const Function _f;

  };
}

#endif
