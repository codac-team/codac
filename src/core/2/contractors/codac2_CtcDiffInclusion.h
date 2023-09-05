/** 
 *  \file
 *  CtcDiffInclusion class
 * ----------------------------------------------------------------------------
 *  \date       2022
 *  \author     Damien Massé
 *  \copyright  Copyright 2022 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC2_CTCDIFFINCLUSION_H__
#define __CODAC2_CTCDIFFINCLUSION_H__

#include "codac_TFunction.h"
#include "codac_DynCtc.h"
#include "codac2_Tube.h"
#include "codac2_Slice.h"
#include "codac2_IParals.h"

namespace codac2
{
  using codac::TFunction;
  using codac::TimePropag;

  /**
   * \class CtcDiffInclusion
   * \brief ...
   */
  class CtcDiffInclusion
  {
    public:

      CtcDiffInclusion(const TFunction& t);


      /** evaluate function on box + time interval
       */
      const IntervalVector eval_function(const Interval &tim,
                 const IntervalVector& cdom, const IntervalVector* u) const;
      const IntervalVector eval_function(const Interval &tim,
                 const IParals& cdom, const IntervalVector* u) const;
      /** evaluate function on box + time
       */
      const IntervalVector eval_function(double tim,
                 const IntervalVector& cdom, const IntervalVector* u) const;
      const IntervalVector eval_function(double tim,
                 const IParals& cdom, const IntervalVector* u) const;
      /** evaluate function on point + time
       */
      const IntervalVector eval_function(double tim,
                 const Vector& cdom, const IntervalVector* u) const;

      IntervalMatrix jacobian(const IParals& codom,
                 const IntervalVector* u,
                 const Interval& tdom,
                 IntervalVector& tvec) const;



      IParals extend_box_basic(const IParals& frame,
                        const IParals& startIV,
                        const IntervalVector* u,
                        const Interval& tim,
                        double inflation_factor,
                        TimePropag t_propa,
                        int nb_tries) const;

      bool compute_step(const IParals& frame,
                                const IntervalVector* u,
                                const IParals& actState,
                                IParals& tauState,
                                IParals& finState,
                                const Interval& timeslice,
                                TimePropag t_propa) const;


      void contract(Tube<IParals>& x, const Tube<IParals>* u, TimePropag t_propa = TimePropag::FORWARD | TimePropag::BACKWARD);
      void contract_from_slice(Tube<IParals>& x, const Tube<IParals>* u, std::shared_ptr<Slice<IParals>>& gate, TimePropag t_propa = TimePropag::FORWARD | TimePropag::BACKWARD);
      void contract(Slice<IParals>& x, const Slice<IParals>& u, TimePropag t_propa = TimePropag::FORWARD | TimePropag::BACKWARD);
      void contract(Slice<IParals>& x, const std::shared_ptr<const IParals>& uDom, TimePropag t_propa = TimePropag::FORWARD | TimePropag::BACKWARD);

      const TFunction& f() const;

    protected:

      //friend class Slice; // to be removed
      const TFunction _f;

      /** inflation factor in extend_box_basic */
      constexpr static double default_inflation_factor = 1.01; 
      constexpr static int narrow_factor = 3;


  };
}

#endif
