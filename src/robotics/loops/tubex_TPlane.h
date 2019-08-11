/** 
 *  TPlane class
 * ----------------------------------------------------------------------------
 *  \date       2015
 *  \author     Simon Rohou
 *  \copyright  Copyright 2019 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_TPLANE_H__
#define __TUBEX_TPLANE_H__

#include "tubex_Paving.h"
#include "tubex_TubeVector.h"
#include "tubex_ConnectedSubset.h"

namespace tubex
{
  class TPlane : public Paving
  {
    public:

      TPlane();
      void compute_detections(float precision, const TubeVector& x, const TubeVector& v, bool extract_subsets = true);
      void compute_proofs(ibex::IntervalVector (*f)(const ibex::IntervalVector& b));
      int nb_loops_detections() const;
      int nb_loops_proofs() const;
      const std::vector<ConnectedSubset>& get_detected_loops() const;
      const std::vector<ConnectedSubset>& get_proven_loops() const;
      Trajectory traj_computed_loops() const;

    protected:

      ibex::IntervalVector f(const ibex::IntervalVector& input);

      float m_precision = 0.;
      std::vector<ConnectedSubset> m_v_detected_loops;
      std::vector<ConnectedSubset> m_v_proven_loops;
  };
}

#endif