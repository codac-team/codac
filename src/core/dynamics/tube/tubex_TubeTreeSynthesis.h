/** 
 *  @file
 *  TubeTreeSynthesis class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#ifndef __TUBEX_TUBETREESYNTHESIS_H__
#define __TUBEX_TUBETREESYNTHESIS_H__

#include "tubex_Slice.h"

namespace tubex
{
  class TubeTreeSynthesis
  {
    public:

      TubeTreeSynthesis(const Tube* tube, int k0, int kf, const std::vector<const Slice*>& v_tube_slices);
      ~TubeTreeSynthesis();

      const ibex::Interval domain() const;
      int nb_slices() const;
      const ibex::Interval operator()(const ibex::Interval& t);
      const ibex::Interval codomain();
      const std::pair<ibex::Interval,ibex::Interval> codomain_bounds();
      const std::pair<ibex::Interval,ibex::Interval> eval(const ibex::Interval& t = ibex::Interval::ALL_REALS);
      
      int input2index(double t) const;
      Slice* slice(int slice_id);
      const Slice* slice(int slice_id) const;

      bool is_leaf() const;
      bool is_root() const;
      TubeTreeSynthesis* root();

      void request_values_update();
      void request_integrals_update(bool propagate_to_other_slices = true);
      void update_values();
      void update_integrals();
      std::pair<ibex::Interval,ibex::Interval> partial_integral(const ibex::Interval& t);
      const std::pair<ibex::Interval,ibex::Interval> partial_primitive_bounds(const ibex::Interval& t = ibex::Interval::ALL_REALS);

    protected:

      // Binary tree structure
      TubeTreeSynthesis *m_parent = NULL;
      TubeTreeSynthesis *m_first_subtree = NULL, *m_second_subtree = NULL;

      // Slices connections
      const Slice *m_slice_ref = NULL;
      const Tube *m_tube_ref = NULL;

      int m_nb_slices;
      ibex::Interval m_domain, m_codomain;
      std::pair<ibex::Interval,ibex::Interval> m_codomain_bounds;
      std::pair<ibex::Interval,ibex::Interval> m_partial_primitive;

      bool m_integrals_update_needed = true;
      bool m_values_update_needed = true;
  };
}

#endif