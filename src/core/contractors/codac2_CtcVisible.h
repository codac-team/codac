/** 
 *  \file codac2_CtcVisible.h
 * ----------------------------------------------------------------------------
 * \date       2026
 * \author     Quentin Brateau
 * \copyright  Copyright 2026 Codac Team
 * \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_Ctc.h"
#include "codac2_Segment.h"
#include "codac2_Polygon.h"

namespace codac2
{
  struct VisibilityEdgeData {
      IntervalVector e1, e2;
      IntervalVector v_e2e1, v_ae1, v_ae2;
      IntervalVector s_box;
      double k;
  };

  class CtcVisibleBase
  {
    protected:

      CtcVisibleBase(const IntervalVector& a, const std::vector<Segment>& edges);

      const IntervalVector _a;
      std::vector<VisibilityEdgeData> _edges;
  };

  class CtcVisible : public Ctc<CtcVisible, IntervalVector>, CtcVisibleBase
  {
    public:
      /**
       * \brief Constructor for visibility from point 'a' relative to segment 's'.
       */
      CtcVisible(const IntervalVector& a, const Segment& s);

      /**
       * \brief Constructor for visibility from point 'a' relative to a list of segments 's'.
       */
      CtcVisible(const IntervalVector& a, const std::vector<Segment>& s);

      /**
       * \brief Constructor for visibility from point 'a' relative to polygon 'p'.
       */
      CtcVisible(const IntervalVector& a, const Polygon& p);

      void contract(IntervalVector& x) const;
  };

  class CtcNoVisible : public Ctc<CtcNoVisible, IntervalVector>, CtcVisibleBase
  {
    public:
      /**
       * \brief Constructor for non-visibility from point 'a' relative to segment 's'.
       */
      CtcNoVisible(const IntervalVector& a, const Segment& s);

      /**
       * \brief Constructor for non-visibility from point 'a' relative to a list of segments 's'.
       */
      CtcNoVisible(const IntervalVector& a, const std::vector<Segment>& s);
      
      /**
       * \brief Constructor for non-visibility from point 'a' relative to polygon 'p'.
       */
      CtcNoVisible(const IntervalVector& a, const Polygon& p);

      void contract(IntervalVector& x) const;
  };
}